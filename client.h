#ifndef CLIENT_H
#define CLIENT_H

#include <sys/types.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <cstdlib>
#include <iostream>

#include "connector.h"


class Client{
	public:
		Client(){
		}

		int genPosY(){
			return rand() % 420 + 40;
		}

		void run(Connector & connector, sem_t * semHost, sem_t * semClient) {
    		while (true) {
        		sem_wait(semClient);
        		//sem_post(semClient);
    			int end = 0;
    			connector.conRead(&end, sizeof(end));

    			if (end == 1){
        			sem_close(semClient);
    				sem_unlink("/semClient");
        			break;
        		}
    			int posY = genPosY();
        		connector.conWrite(&posY, sizeof(posY));
        		sem_post(semHost);		
        	}
        	sem_post(semHost);
        	exit(0);
		}

};

#endif