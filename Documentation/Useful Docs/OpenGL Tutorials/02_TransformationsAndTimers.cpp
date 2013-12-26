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

// rotation angle
float g_angle = 30.0f;
float g_cameraAngle = 0.0f;


//************************************************************************
// forward declarations of OpenGL callback functions
//************************************************************************
void handleKeypress(unsigned char key, int x, int y);
void initRendering();
void handleResize(int w, int h);
void drawScene();
void update(int value);

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
	glutCreateWindow("Transformations");
	initRendering();

	// setting handler functions
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);

	// adding a timer
	glutTimerFunc(25, update, 0);

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

	// switch to and setting camera perspective
	glMatrixMode(GL_PROJECTION);

	// setting the camera perspective
	// reseting the camera
	glLoadIdentity();
	// camera angle, width-to-heigth ration, near z clipping coordinate, far z clipping coordinate
	gluPerspective(45.0, (double) w / (double) h, 1.0, 200.0);
}


/**
 * draws a scene
 */
void drawScene()
{

	/* glPushMatrix and glPopMatrix are used to save and restore the state of the initial scene
	* glTranslatef, glRotatef and glScalef may not be caleed within a glBegin-glEnd block
	*/

	// clearing from last draw
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// switching to drawing perspective and reseting
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// changing the camera angle by rotating the whole scene in the opposite direction along the y-axis 
	glRotatef(-g_cameraAngle, 0.0f, 1.0f, 0.0f);

	// moving the whole scene 5 units in the -z
	glTranslatef(0.0f, 0.0f, -5.0f);
	// rotating ALL scenes 30 degrees around the z axis
	glRotatef(g_angle, 0.0f, 0.0f, 1.0f);

	// saving all previous transformations
	glPushMatrix();
		// moving to the center of the trapezoid
		glTranslatef(0.0f, -1.0f, 0.0f);

		// drawing trapezoid
		glBegin(GL_QUADS);
			glVertex3f(-0.7f, -0.5f, 0.0f);
			glVertex3f(0.7f, -0.5f, 0.0f);
			glVertex3f(0.4f, 0.5f, 0.0f);
			glVertex3f(-0.4f, 0.5f, 0.0f);
		glEnd();
	// undo move to center of trapezoid
	glPopMatrix();

	// saving all previous transformations
	glPushMatrix();
		// move to the center of the pentagon
		glTranslatef(1.0f, 1.0f, 0.0f);
		// scaling down the pentagon to 70%
		glScalef(0.7f, 0.7f, 0.7f);
		// drawing pentagon with 3 triangles
		glBegin(GL_TRIANGLES);
			glVertex3f(-0.5f, -0.5f, 0.0f);
			glVertex3f(0.5f, -0.5f, 0.0f);
			glVertex3f(-0.5f, 0.0f, 0.0f);

			glVertex3f(-0.5f, 0.0f, 0.0f);
			glVertex3f(0.5f, -0.5f, 0.0f);
			glVertex3f(0.5f, 0.0f, 0.0f);

			glVertex3f(-0.5f, 0.0f, 0.0f);
			glVertex3f(0.5f, 0.0f, 0.0f);
			glVertex3f(0.0f, 0.5f, 0.0f);
		glEnd();
	// undo move to center of trapezoid
	glPopMatrix();

	// saving all previous transformations
	glPushMatrix();
		// moving to the center of the triangle
		glTranslatef(-1.0f, 1.0f, 0.0f);
		// rotate about the vector (1,2,3)
		glRotatef(g_angle, 1.0f, 2.0f, 3.0f);

		// drawing a triangle
		glBegin(GL_TRIANGLES);
			glVertex3f(0.5f, -0.5f, 0.0f);
			glVertex3f(0.0f, 0.5f, 0.0f);
			glVertex3f(-0.5f, -0.5f, 0.0f);
		glEnd();
	glPopMatrix();

	// sending the 3D scene to the screen
	glutSwapBuffers();
}


void update(int value)
{
	// rotates the shapes 2 degrees in every 25 miliseconds
	g_angle += 2.0f;
	if (g_angle > 360)
	{
		g_angle -= 360;
	}

	// informing GLUT that the scene has changed
	glutPostRedisplay();

	// calling this method in every 25 ms
	glutTimerFunc(25, update, 0);
}