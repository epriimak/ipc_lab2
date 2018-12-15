#include "connector.h"

#define BUF_SIZE 100
static void* buf = NULL;

Connector::Connector() {
	buf = mmap(NULL, BUF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
}

bool Connector::conRead(void* rBuf, int rBufSize) {
    if (rBufSize > BUF_SIZE) {
        syslog(LOG_INFO, "Error buff size");
		return false;
	}

    rBuf = std::memcpy(rBuf, buf, BUF_SIZE);

    if (rBuf == NULL) {
        syslog(LOG_INFO, "Error memcpy");
		return false;	
    }

    return true;
}

bool Connector::conWrite(void* wBuf, int wBufSize) {	
    if (wBufSize > BUF_SIZE) {
        syslog(LOG_INFO, "Error buff size");
		return false;
	}

    buf = std::memcpy(buf, wBuf, wBufSize);

    if (buf == NULL) {
        syslog(LOG_INFO, "Error memcpy");
		return false;	
    }

    return true;
}

Connector::~Connector() {
    munmap(buf, BUF_SIZE);
}