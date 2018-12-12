#include "connector.h"

static int fifoFileDesc = 0;

Connector::Connector() {
	if (mkfifo("fifo", S_IRWXU) == -1){
		syslog(LOG_INFO, "Error making fifo");
	}
	
	if ((fifoFileDesc = open("fifo", O_RDWR | O_NONBLOCK)) == -1 ) {
		syslog(LOG_INFO, "Error opening fifo");	
	}
}

bool Connector::conRead(void* rBuf, int rBufSize) {
	if (read(fifoFileDesc, rBuf, rBufSize) == -1) {
		syslog(LOG_INFO, "Error reading");
		return false;
	}

	return true;
}

bool Connector::conWrite(void* wBuf, int wBufSize) {
	if (write(fifoFileDesc, wBuf, wBufSize) == -1) {
		syslog(LOG_INFO, "Error writing");
		return false;
	}

	return true;
}

Connector::~Connector() {
	close(fifoFileDesc);
	unlink("fifo");
 }