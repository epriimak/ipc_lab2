#include <GL/glut.h>
#include <GL/freeglut.h>
#include <termios.h>
#include <unistd.h>
#include <syslog.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "host.h"
#include "client.h"
#include "connector.h"

static Connector connector;

static Host host;
static Client client;

static sem_t * semHost = NULL;
static sem_t * semClient = NULL;

int	interval = 10;

void update(int value){
	host.updateField();
   glutTimerFunc (interval, update, 0);
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y){
    if(key == 27){
        host.end = 1;
    }

}

void enable2D (int width, int height)
{
	glViewport(0 , 0 , width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f , width, 0.0f , height, 0.0f , 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void draw ()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity ();
	host.drawHost();
	glutSwapBuffers();
}

static void render_init(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (GAME_WIDTH, GAME_HEIGHT);
	glutCreateWindow("Pong");
	glutDisplayFunc (draw);
    glutKeyboardFunc(keyboard);
	glutTimerFunc(interval, update, 0);
	enable2D(GAME_WIDTH, GAME_HEIGHT);
	glColor3f (1.0f , 0.0f , 1.0f);
}


int main(int argc, char** argv) {

    int pid;

    semHost = sem_open("/semHost", O_CREAT | O_EXCL, S_IRWXU, 0);
    semClient  = sem_open("/semClient", O_CREAT | O_EXCL, S_IRWXU, 0);

    if (semHost == SEM_FAILED || semClient == SEM_FAILED) {
    	sem_unlink("/semHost");
    	sem_unlink("/semClient");

    	syslog(LOG_INFO, "Semaphore open error");
    	exit(1);
    }
      
    pid = fork();	
 
    if (pid) {     
        render_init(argc, argv);
       
    	host.run(connector, semHost, semClient);
 
    } 
    else if (!pid) {
  
    	client.run(connector, semHost, semClient);
    } 
    else {
    	syslog(LOG_INFO, "Fork was failed");
    	sem_close(semHost);
    	sem_close(semClient);
        sem_unlink("/semHost");
        sem_unlink("/semClient");
    	exit(1);
    }
    
           
	exit(0);
}