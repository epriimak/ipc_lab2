#ifndef HOST_H
#define HOST_H

#include <math.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <sstream>
#include <termios.h>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "connector.h"

#define GAME_WIDTH 500
#define GAME_HEIGHT 500

typedef struct{
	float x;
	float y;
}point;

class Host {

private:
	int width;
	int height;

	int scoreHost;
	int scoreClient;

	int racketWidth;
	int racketHeight;
	
	point racketHost;
    point racketClient;
	int racketSpeed;

	point ballPos;
	point ballDir;
	int ballSize;
	int ballSpeed;
	
	bool flagGen;

	int maxDeltaScore;

  	bool isInRacket(point p);

  	void updateClientRacket(int posY);
  	void updateHostRacket();

public:
	Host();
	
  	void updateBall();
  	void drawHost ();
  	
  	void run(Connector & connector, sem_t * semHost, sem_t * semClient);
};

#endif