#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <string>
#include <stdlib.h>    
#include <GL\glut.h>
//#include <SWI-cpp.h>
#include <iostream>


#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

using namespace std;

/*global variables*/
int WindowWidth = 800;
int WindowHeight = 600;
const float DEG2RAD = 314159 / 180;
int GLUTWindowHandle = 0;

/*initial declaration of some importante functions*/
void drawScene();
void display();
void resizeWindow(int x, int y);
void mouse(int btn, int mouseState, int x, int y);
void keyboard(int key, int x, int y);


/*function when the window is resixed*/
void handleResize(int x, int y)
{
	glViewport(0, 0, x, y);
	glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	gluPerspective(45.0, (double)x / (double)y, 1.0, 1.0);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	drawScene();
	glFlush();
}
/*function to initialize OpenGL*/
void myInit()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glViewport(0, 0, WindowWidth, WindowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)WindowWidth, (GLdouble)WindowHeight, 0.0);
}

/*function to draw circles with center (x,y) and radius (radius)*/

void drawCircle(float centerX, float centerY, float radius, int num)
{
	glLineWidth(4.0);
	glBegin(GL_LINE_LOOP);

	for (int i = 0; i < num; i++)
	{
		float angle = 2.0f * 3.1415926f * float(i) / float(num);

		float x = radius * cosf(angle);
		float y = radius * sinf(angle);

		glVertex2f(x + centerX, y + centerY);
	}
	glEnd();
}
/*function to draw line starting on some point and finishing on other point*/

void drawLine(GLfloat x1,GLfloat y1, GLfloat x2, GLfloat y2)
{
	glLineWidth(4.0);
	glBegin(GL_LINES);

	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
}

/*function to draw the main scene*/

void drawScene()
{
	glColor3b(1.0, 0.0, 0.0);
	drawCircle(200, 200, 100, 500);
	drawLine(400, 500, 450, 400);
}

int main(int argc, char**argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutCreateWindow("HANGMAN");
	glutDisplayFunc(display);
	myInit();
	glutMainLoop();
	return 0; 
}









