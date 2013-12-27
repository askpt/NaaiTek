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
float g_angle = -70.0f;
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
	glutCreateWindow("Lightning");
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

	// enabling color
	glEnable(GL_COLOR_MATERIAL);

	// enable lightning
	glEnable(GL_LIGHTING);

	// enabling 2 light sources
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	// normalizing normals
	glEnable(GL_NORMALIZE);

	// enabling smooth shading
	glShadeModel(GL_SMOOTH);
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -8.0f);

	// adding ambient light 
	// (intesity of 0.2 for RGB; if we used 1 we would have the same effect as not having ambient light at all)
	GLfloat ambientColor [] = { 0.2f, 0.2f, 0.2f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	// adding positioned light
	GLfloat lightColor0 [] = { 0.5f, 0.5f, 0.5f, 1.0f };	// 0.5 for all is intense light
	GLfloat lightPos0 [] = { 4.0f, 0.0f, 8.0f, 1.0f };		// light positioned at (4, 0, 8) relative to the current transformation
	// setting the color and intensity of the light
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	// setting the ligth source position
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	// adding 2nd light (on the contrary to light0, this one IS NOT positional, due to 0.0 as last element of array lightPos1
	GLfloat lightColor1[] = { 0.5f, 0.2f, 0.2f, 1.0f };		// on the red size
	GLfloat lightPos1[] = { -1.0f, 0.5f, 0.5f, 0.0f };		// light coming from (-1, 0.5, 0.5)
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

	glRotatef(g_angle, 0.0f, 1.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 0.0f);

	// without smooth shading
	/*
	glBegin(GL_QUADS);
		// front
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-1.5f, -1.0f, 1.5f);
		glVertex3f(1.5f, -1.0f, 1.5f);
		glVertex3f(1.5f, 1.0f, 1.5f);
		glVertex3f(-1.5f, 1.0f, 1.5f);

		// right
		glNormal3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.5f, -1.0f, -1.5f);
		glVertex3f(1.5f, 1.0f, -1.5f);
		glVertex3f(1.5f, 1.0f, 1.5f);
		glVertex3f(1.5f, -1.0f, 1.5f);

		// back 
		glNormal3f(0.0f, 0.0f, -1.0f);
		glVertex3f(-1.5f, -1.0f, -1.5f);
		glVertex3f(-1.5f, 1.0f, -1.5f);
		glVertex3f(1.5f, 1.0f, -1.5f);
		glVertex3f(1.5f, -1.0f, -1.5f);

		// left 
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glVertex3f(-1.5f, -1.0f, -1.5f);
		glVertex3f(-1.5f, -1.0f, 1.5f);
		glVertex3f(-1.5f, 1.0f, 1.5f);
		glVertex3f(-1.5f, 1.0f, -1.5f);
	glEnd();
	*/

	// with smooth shading
	// (this is achieved by specifying a normal for each vertex corresponding to the real normal a sphere would have)
	glBegin(GL_QUADS);
	// front
	glNormal3f(-1.0f, 0.0f, 1.0f);
	glVertex3f(-1.5f, -1.0f, 1.5f);
	glNormal3f(1.0f, 0.0f, 1.0f);
	glVertex3f(1.5f, -1.0f, 1.5f);
	glNormal3f(1.0f, 0.0f, 1.0f);
	glVertex3f(1.5f, 1.0f, 1.5f);
	glNormal3f(-1.0f, 0.0f, 1.0f);
	glVertex3f(-1.5f, 1.0f, 1.5f);

	// right
	glNormal3f(1.0f, 0.0f, -1.0f);
	glVertex3f(1.5f, -1.0f, -1.5f);
	glNormal3f(1.0f, 0.0f, -1.0f);
	glVertex3f(1.5f, 1.0f, -1.5f);
	glNormal3f(1.0f, 0.0f, 1.0f);
	glVertex3f(1.5f, 1.0f, 1.5f);
	glNormal3f(1.0f, 0.0f, 1.0f);
	glVertex3f(1.5f, -1.0f, 1.5f);

	// back 
	glNormal3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-1.5f, -1.0f, -1.5f);
	glNormal3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-1.5f, 1.0f, -1.5f);
	glNormal3f(1.0f, 0.0f, -1.0f);
	glVertex3f(1.5f, 1.0f, -1.5f);
	glNormal3f(1.0f, 0.0f, -1.0f);
	glVertex3f(1.5f, -1.0f, -1.5f);

	// left 
	glNormal3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-1.5f, -1.0f, -1.5f);
	glNormal3f(-1.0f, 0.0f, 1.0f);
	glVertex3f(-1.5f, -1.0f, 1.5f);
	glNormal3f(-1.5f, 0.0f, 1.0f);
	glVertex3f(-1.5f, 1.0f, 1.5f);
	glNormal3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-1.5f, 1.0f, -1.5f);
	glEnd();

	glutSwapBuffers();
}


void update(int value)
{
	// rotates the shapes 1.5 degrees in every 25 miliseconds
	g_angle += 1.5f;
	if (g_angle > 360)
	{
		g_angle -= 360;
	}

	// informing GLUT that the scene has changed
	glutPostRedisplay();

	// calling this method in every 25 ms
	glutTimerFunc(25, update, 0);
}