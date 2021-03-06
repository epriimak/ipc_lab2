#include "host.h"

void norm(float& x, float &y) {
	float length = sqrt((x * x) + (y * y));
	if (length != 0.0f) {
		x /= length;
		y /= length;
	}
}


void drawScore(float x, float y, int num1, int num2) {
	glRasterPos2f(x, y);
	std::stringstream ss;
	ss << num1;
	ss << " : ";
	ss << num2;
	std::string text = ss.str();

	glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char*)text.c_str());
}


void drawRect(point p, float width, float height){
	glBegin (GL_QUADS);
	glVertex2f (p.x - width / 2, p.y - height / 2);
	glVertex2f (p.x - width / 2, p.y + height / 2);
	glVertex2f (p.x + width / 2, p.y + height / 2);
	glVertex2f (p.x + width / 2, p.y - height / 2);
	glEnd ();
}


Host::Host(){
	width = GAME_WIDTH;
	height = GAME_HEIGHT;
	scoreHost = 0;
	scoreClient = 0;

	racketWidth = 10;
	racketHeight = 80;
	racketSpeed = 5;

	racketHost.x = 5.0f;
	racketHost.y = racketHeight * 2;
	racketClient.x = width - racketWidth/2;
	racketClient.y = height - racketHeight * 2;

	newPosYClient = 0;

	ballPos.x = width / 2;
	ballPos.y = height / 2;
	ballDir.x = -1.0f;
	ballDir.y = 0.1f;
	ballSize = 15;
	ballSpeed = 2;

	flagGen = true;
	end = 0;
}


void Host::drawHost(){
	drawRect(racketHost, racketWidth, racketHeight);
	drawRect(racketClient, racketWidth, racketHeight);
	drawRect(ballPos, ballSize, ballSize);
	drawScore(width / 2 - 10, height - 15, scoreHost, scoreClient);
}


bool Host::isInRacket(point p){
	return ballPos.x < p.x + racketWidth / 2 && ballPos.x > p.x - racketWidth / 2 && ballPos.y < p.y + racketHeight / 2 && ballPos.y > p.y-racketHeight/2;
}


void Host::updateField() {
	// fly a bit
	flagGen = false;
	ballPos.x += ballDir.x * ballSpeed;
	ballPos.y += ballDir.y * ballSpeed;

	if(isInRacket(racketHost)){
		float t = ((ballPos.y - racketHost.y) / racketHeight) - 0.5f;
		ballDir.x = fabs(ballDir.x);
		ballDir.y = t;
		//scoreHost++;
		flagGen = true;
	}

	if (isInRacket(racketClient)) {
		float t = ((ballPos.y - racketClient.y) / racketHeight) - 0.5f;
		ballDir.x = -fabs(ballDir.x);
		ballDir.y = t;
		//scoreClient++;
	}

	//left wall
	if (ballPos.x - ballSize / 2 <= 0) {
		ballPos.x = width / 2;
		ballPos.y = height / 2;
		
		ballDir.x = fabs(ballDir.x); 
		ballDir.y = 0;
		
		flagGen = true;
		scoreClient++;
	}

	//right wall
	if (ballPos.x + ballSize / 2 >= width) {
		ballPos.x = width / 2;
		ballPos.y = height / 2;
		
		ballDir.x = -fabs(ballDir.x); 
		ballDir.y = 0;
		
		scoreHost++;
	}

	//top wall
	if (ballPos.y + ballSize / 2 > height) {
		ballDir.y = -fabs(ballDir.y); 
	}

	//bottom wall
	if (ballPos.y - ballSize / 2 < 0) {
		ballDir.y = fabs(ballDir.y); 
	}

	//norm ball direction
	norm(ballDir.x, ballDir.y);

	//host racket
	if(ballPos.x < width / 2 && ballDir.x <= 0){
		if(racketHost.y >= ballPos.y){
			racketHost.y -= 1;
		}
		if(racketHost.y <= ballPos.y){
			racketHost.y += 1;	
		}
	}
	
	if(racketHost.y + racketHeight / 2 > height){
		racketHost.y -= racketHeight / 2;
	}
	
	if(racketHost.y - racketHeight / 2 < 0){
		racketHost.y += racketHeight / 2;
	}

	//client racket
	if(ballPos.x > width / 2 && ballDir.x >= 0){
		if(racketClient.y < newPosYClient){
				racketClient.y += 1;	
		}
		if(racketClient.y > newPosYClient){
			racketClient.y -= 1;
		}
	}
}


void Host::updateClientRacket(int posY ){
	if(flagGen){
		newPosYClient = posY;
		flagGen = false;
	}
}


void Host::run(Connector & connector, sem_t * semHost, sem_t * semClient) {
    while (true) {      
        connector.conWrite(&end, sizeof(end));
        sem_post(semClient);

        if(end == 1){       	
       		sem_close(semHost);
        	sem_unlink("/semHost");
     		break;	
     	} 
     	sem_wait(semHost); 
        int dY = 0; 
        connector.conRead(&dY, sizeof(dY));
        
        updateClientRacket(dY);
     	glutMainLoopEvent();  	
	}

    sem_post(semClient);
    exit(0);
}