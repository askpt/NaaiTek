#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <stdlib.h>     
#include <GL\glut.h>
#include <iostream>

using namespace std;

struct player1
{
	bool matrix[3][3];
};

struct player2
{
	bool matrix[3][3];
};


int WindowWidth = 600;
int WindowHeight =600;


int GLUTWindowHandle = 0;

/*function to initialize OpenGL*/

void myInit()
{
	glClearColor(1.0, 0.0, 0.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	glShadeModel(GL_FLAT);

	glViewport(0, 0, WindowWidth, WindowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0.0, (GLdouble)WindowWidth, (GLdouble)WindowHeight, 0.0);
	glMatrixMode(GL_MODELVIEW);


}

void drawTable()
{
	float squareWidth = WindowWidth / 5.0;

	glColor3b(1.0, 0.0, 0.0);

	glLineWidth(4.0);
	glBegin(GL_LINES);
	glVertex2f(100, 100);
	glVertex2f(100+squareWidth, 100);

	glVertex2f(100+squareWidth, squareWidth+100);
	glVertex2f(100, 100+squareWidth);

	glVertex2f(100, 100);
	glVertex2f(100, 100 + squareWidth);

	glVertex2f(100 + squareWidth, 100 + squareWidth);
	glVertex2f(100 + squareWidth, 100);
	glEnd();

	glColor3b(1.0, 0.0, 0.0);
	glLineWidth(4.0);
	glBegin(GL_LINES);

	glVertex2f(100 + squareWidth, 100);
	glVertex2f(100 + 2 * squareWidth, 100);

	glVertex2f(100 + 2 * squareWidth, 100 + squareWidth);
	glVertex2f(100 + squareWidth, 100 + squareWidth);

	glVertex2f(100 + 2 * squareWidth, 100);
	glVertex2f(100 + 2 * squareWidth, 100 + squareWidth);

	glEnd();

	glColor3b(1.0, 0.0, 0.0);
	glLineWidth(4.0);
	glBegin(GL_LINES);

	glVertex2f(100 + 2 * squareWidth, 100);
	glVertex2f(100 + 3 * squareWidth, 100);

	glVertex2f(100 + 2 * squareWidth, 100 + squareWidth);
	glVertex2f(100 + 3 * squareWidth, 100 + squareWidth);

	glVertex2f(100 + 3 * squareWidth, 100);
	glVertex2f(100 + 3 * squareWidth, 100 + squareWidth);

	glEnd();

	glColor3b(1.0, 0.0, 0.0);
	glLineWidth(4.0);

	glBegin(GL_LINES);
	glVertex2f(100, 100 + squareWidth);
	glVertex2f(100, 100 + 2 * squareWidth);

	glVertex2f(100 + squareWidth, 100 + squareWidth);
	glVertex2f(100 + squareWidth, 100 + 2 * squareWidth);

	glVertex2f(100, 100 + 2 * squareWidth);
	glVertex2f(100 + squareWidth, 100 + 2 * squareWidth);

	glVertex2f(100+squareWidth, 100 + 2 * squareWidth);
	glVertex2f(100 + 2 * squareWidth, 100 + 2 * squareWidth);

	glVertex2f(100 + 2 * squareWidth, 100 + squareWidth);
	glVertex2f(100 + 2 * squareWidth, 100 + 2 * squareWidth);

	glVertex2f(100 + 2 * squareWidth, 100 + 2 * squareWidth);
	glVertex2f(100 + 3 * squareWidth, 100 + 2 * squareWidth);

	glVertex2f(100 + 3 * squareWidth, 100 + squareWidth);
	glVertex2f(100 + 3 * squareWidth, 100 + 2 * squareWidth);

	glEnd();

	glColor3b(1.0, 0.0, 0.0);
	glLineWidth(4.0);

	glBegin(GL_LINES);
	glVertex2f(100, 100 + 2 * squareWidth);
	glVertex2f(100, 100 + 3 * squareWidth);

	glVertex2f(100, 100 + 3 * squareWidth);
	glVertex2f(100 + squareWidth, 100 + 3 * squareWidth);

	glVertex2f(100+squareWidth, 100 + 3 * squareWidth);
	glVertex2f(100 + 2*squareWidth, 100 + 3 * squareWidth);

	glVertex2f(100 + 2*squareWidth, 100 + 3 * squareWidth);
	glVertex2f(100 + 3 * squareWidth, 100 + 3 * squareWidth);


	glVertex2f(100+squareWidth, 100 + 2 * squareWidth);
	glVertex2f(100+squareWidth, 100 + 3 * squareWidth);

	glVertex2f(100 +2* squareWidth, 100 + 2 * squareWidth);
	glVertex2f(100 + 2*squareWidth, 100 + 3 * squareWidth);

	glVertex2f(100 + 3 * squareWidth, 100 + 2 * squareWidth);
	glVertex2f(100 + 3 * squareWidth, 100 + 3 * squareWidth);


	
	glEnd();

}
void display(void)
{
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);
	drawTable();
	glFlush();


}
void myReshape(int w, int h){

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glMatrixMode(GL_MODELVIEW);
	display();

}


void mouse(int btn, int mouseState, int x, int y)
{
}

int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutCreateWindow("TIC-TAC-TOE");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	//glutKeyboardFunc(keyboard);
	//glutSpecialFunc(Special);
	glutMouseFunc(mouse);

	myInit();
	glutMainLoop();
	return 0;
}