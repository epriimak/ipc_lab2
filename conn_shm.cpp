#include "connector.h"

#define BUF_SIZE 100

static void* buf = NULL;

static int shmFileDesc = 0;
static int curBufferSize = 0;

Connector::Connector() {
	if ((shmFileDesc = shm_open("/shm", O_CREAT | O_RDWR, S_IRWXU)) == -1) {
		syslog(LOG_INFO, "Error opening shm");
	}

	if (ftruncate(shmFileDesc, BUF_SIZE) == -1) {
		syslog(LOG_INFO, "Error ftruncate");	
	}

	buf = mmap(NULL, BUF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmFileDesc, 0);    
}

bool Connector::conRead(void* rBuf, int rBufSize) {
	if (rBufSize < curBufferSize) {
		syslog(LOG_INFO, "Error buffs sizes");	
		return false;
	}	

    rBuf = memcpy(rBuf, buf, BUF_SIZE);

    if (rBuf == NULL) {
   		syslog(LOG_INFO, "Error memcpy");	
		return false;	
    }

    return true;
}

bool Connector::conWrite(void* wBuf, int wBufSize) {
	if (wBufSize > BUF_SIZE) {
		syslog(LOG_INFO, "Error buffs sizes");	
		return false;
	}

    buf = memcpy(buf, wBuf, wBufSize);
    
    if (buf == NULL) {
    	syslog(LOG_INFO, "Error memcpy");	
		return false;
	}

    curBufferSize = wBufSize;

    return true;
}

Connector::~Connector() {
	munmap(buf, BUF_SIZE);
	shm_unlink("/shm");
}