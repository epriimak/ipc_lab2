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
        		sem_wait(semHost);
        		sem_post(semClient);
    	
        		//std::cout << "Client: " << std::endl;
        		int posY = genPosY();
        		connector.conWrite(&posY, sizeof(posY));
        	}
        	exit(0);
		}

};

#endif