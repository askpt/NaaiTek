#include <cstdlib>
#include <iostream>
#include <ctime>

#define _USE_MATH_DEFINES
#include <math.h>

#define FREEGLUT_STATIC
#include <GL\freeglut.h>

using namespace std;


//************************************************************************
// global variables
//************************************************************************


//************************************************************************
// forward declarations of OpenGL callback functions
//************************************************************************
void handleKeypress(unsigned char key, int x, int y);
void initRendering();
void handleResize(int w, int h);
void drawScene();

/**
* main
*/
int main(int argc, char *argv[])
{
	// initializing GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);

	// creating the window
	glutCreateWindow("Basic Shapes");
	initRendering();

	// setting handler functions
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);

	glutMainLoop();

	return 0;
}


/**
 * called when a key is pressed
 * key is the keyboard key
 * x and y are mouse coordinates
 */
void handleKeypress(unsigned char key, int x, int y)
{
	switch (key)
	{
		// ESC key
		case 27:
			exit(0);
	}
}


/**
 * initializes 3D rendering
 */
void initRendering()
{
	// allows 3D drawing when objects are in front of other objects
	glEnable(GL_DEPTH_TEST);
}


/**
 * called when a window is resized
 * (w e h is the window width and height)
 */
void handleResize(int w, int h)
{
	// converting coordinates to pixel values
	glViewport(0, 0, w, h);

	// swith to and setting camera perspective
	glMatrixMode(GL_PROJECTION);

	// setting the camera perspective
	// reseting the camera
	glLoadIdentity();
	gluPerspective(45.0, (double) w / (double) h, 1.0, 200.0);
}


/**
 * draws a scene
 */
void drawScene()
{
	// clearing from last draw
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// switching and reseting drawing perspective
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// drawing trapezoid
	glBegin(GL_QUADS);
		glVertex3f(-0.7f, -1.5f, -5.0f);
		glVertex3f(0.7f, -1.5f, -5.0f);
		glVertex3f(0.4f, -0.5f, -5.0f);
		glVertex3f(-0.4f, -0.5f, -5.0f);
	glEnd();

	// drawing pentagon with 3 triangles
	glBegin(GL_TRIANGLES);
		glVertex3f(0.5f, 0.5f, -5.0f);
		glVertex3f(1.5f, 0.5f, -5.0f);
		glVertex3f(0.5f, 1.0f, -5.0f);

		glVertex3f(0.5f, 1.0f, -5.0f);
		glVertex3f(1.5f, 0.5f, -5.0f);
		glVertex3f(1.5f, 1.0f, -5.0f);

		glVertex3f(0.5f, 1.0f, -5.0f);
		glVertex3f(1.5f, 1.0f, -5.0f);
		glVertex3f(1.0f, 1.5f, -5.0f);

		glVertex3f(-0.5f, 0.5f, -5.0f);
		glVertex3f(-1.0f, 1.5f, -5.0f);
		glVertex3f(-1.5f, 0.5f, -5.0f);
	glEnd();

	// sending the 3D scene to the screen
	glutSwapBuffers();
}