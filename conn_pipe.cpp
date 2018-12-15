#include "connector.h"

static int pipeFileDesc[2];

Connector::Connector() {
	if (pipe(pipeFileDesc)) {
		syslog(LOG_INFO, "Error piping");
	}
}

bool Connector::conRead(void* rBuf, int rBufSize) {
	if (read(pipeFileDesc[0], rBuf, rBufSize) == -1) {
		syslog(LOG_INFO, "Error reading");
		return false;
	}

	return true;
}

bool Connector::conWrite(void* wBuf, int wBufSize) {
	if (write(pipeFileDesc[1], wBuf, wBufSize) == -1) {
		syslog(LOG_INFO, "Error writing");
		return false;
	}

	return true;
}

Connector::~Connector() {
	close(pipeFileDesc[0]);
	close(pipeFileDesc[1]);
}