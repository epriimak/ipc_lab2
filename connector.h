#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <iostream>
#include <sys/mman.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstdio>
#include <sys/socket.h>
#include <cstdlib>
#include <unistd.h>
#include <stdio.h>
#include <syslog.h>
#include <cstring>
#include <mqueue.h>

class Connector {
	public:
  		Connector();

  		bool conRead(void* rBuf, int rBufSize);
  		bool conWrite(void* wBuf, int wBufSize);
  
  		~Connector();
};

#endif 