#include "connector.h"

static mqd_t mesQ;

Connector::Connector() {
	mq_attr attribute;

	attribute.mq_flags   = 0;
	attribute.mq_maxmsg  = 1;
	attribute.mq_msgsize = sizeof(int);
	attribute.mq_curmsgs = 0;

    if ((mesQ = mq_open("/mq", O_CREAT | O_RDWR, S_IRWXU, &attribute)) == -1) {
    	syslog(LOG_INFO, "Error opening mesQ");
    }
  
}

bool Connector::conRead(void* rBuf, int rBufSize) {
	unsigned int p;

	if (mq_receive(mesQ, (char *)rBuf, rBufSize, &p) == -1) {
        syslog(LOG_INFO, "Error receiving");
		return false;
	}

	return true;
}

bool Connector::conWrite(void* wBuf, int wBufSize) {
	if (mq_send(mesQ, (char *)wBuf, wBufSize, 0) == -1) {
        syslog(LOG_INFO, "Error sending");
		return false;
	}

	return true;
}

Connector::~Connector() {
	mq_close(mesQ);
}