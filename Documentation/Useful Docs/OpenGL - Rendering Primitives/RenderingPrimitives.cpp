#include <cstdlib>
#include <iostream>
#include <ctime>

#define _USE_MATH_DEFINES
#include <math.h>

#define FREEGLUT_STATIC
#include <GL\freeglut.h>


//************************************************************************
// global variables
//************************************************************************
int g_windowWidth = 800;
int g_windowHeight = 600;
int g_windowHandle = 0;		// "id" for the window, initialized to 0					
int g_errorCode = 0;		// error code, initialized to 0			

// general rotation variables (degrees per second)
float g_Rotation1 = 0.0;
float g_Rotation2 = 0.0;
float g_Rotation3 = 0.0;
float g_Rotation4 = 0.0;
float g_Rotation5 = 0.0;
float g_rotationRate = 50.0;

// registering time
std::clock_t g_prevTicks;
std::clock_t g_currentTicks;

// pausing the animation
bool g_isPaused = FALSE;

// structs used as vector type objects for 2D and 3D space
struct float2
{
	float2(float _x = 0.0f, float _y = 0.0f) : x(_x), y(_y){}
	float x, y;
};

struct float3
{
	float3(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) : x(_x), y(_y), z(_z){}
	float x, y, z;
};

// with several scenes it's usefull to use an enumeration to store different scene types
enum sceneType
{
	scene1 = 0,
	scene2,
	scene3,
	scene4,
	scene5
};

// variable to store current scene
sceneType g_currentScene = scene1;



//************************************************************************
// forward declarations of OpenGL callback functions
//************************************************************************

void initGlut(int argc, char *argv[]);
void displayGL();
void idleGL();
void keyboardGL(unsigned char c, int x, int y);
void mouseGL(int button, int state, int x, int y);
void motionGL(int x, int y);
void reshapeGL(int w, int h);


//************************************************************************
// forward declarations of primitive drawing functions
//************************************************************************
void drawRectangle(float w, float h);
void drawCircle(float r, int numSides = 20);
void drawTriangle(float2 p1, float2 p2, float2 p3);
void drawCube(float w, float h, float d);
void drawSphere(float r);
void drawPyramid(float sc = 1.0f);


//************************************************************************
// forward declarations of functions to render different scenes
//************************************************************************
void renderScene1();
void renderScene2();
void renderScene3();
void renderScene4();
void renderScene5();


//************************************************************************
// forward declarations of other functions
//************************************************************************
void cleanUp(int exitCode, bool bExit = true);


/**
* main
*/
int main(int argc, char *argv[])
{
	initGlut(argc, argv);

	// initializing global variables
	g_prevTicks = std::clock();					// getting current time

	glutMainLoop();
}


/**
* initGlut -> OpenGL callback function
*
*/
void initGlut(int argc, char *argv[])
{
	glutInit(&argc, argv);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
	int screenHeigh = glutGet(GLUT_SCREEN_HEIGHT);

	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowPosition((screenWidth - g_windowWidth) / 2, (screenHeigh - g_windowHeight) / 2);
	glutInitWindowSize(g_windowWidth, g_windowHeight);

	g_windowHandle = glutCreateWindow("Rendering Primitives");

	// registering callbacks
	glutDisplayFunc(displayGL);
	glutIdleFunc(idleGL);
	glutKeyboardFunc(keyboardGL);
	glutMouseFunc(mouseGL);
	glutMotionFunc(motionGL);
	glutReshapeFunc(reshapeGL);

	// enabling depth
	glEnable(GL_DEPTH_TEST);
}


/**
* displayGL -> OpenGL callback function
* function registered as the render function that is invoked by GLUT when the current window needs to be redisplayed
*/
void displayGL()
{
	// rendering in immediate mode
	renderScene1();
}


/**
* idleGL -> OpenGL callback function
* function registered as the render function that is invoked when windows system events are not being received
* (ideal for background processing tasks such as animation's update methods)
*/
void idleGL()
{
	// getting current time
	g_currentTicks = std::clock();
	float deltaTicks = (float) (g_currentTicks - g_prevTicks);
	g_prevTicks = g_currentTicks;

	// converting to secondss
	float deltaTime = deltaTicks / (float) CLOCKS_PER_SEC;

	if (!g_isPaused)
	{
		// updating roation (based on elapsed time and NOT in frame rate)
		g_Rotation1 += g_rotationRate * deltaTime;

		// reseting rotation when it gets to 360 degrees
		g_Rotation1 = fmodf(g_Rotation1, 360.0);
	}

	// redrawing window
	glutPostRedisplay();
}

/**
* keyboardGL -> OpenGL callback function
* whenever the user presses a key this method is used as a callback function
* @param c		ASCII keyboard key code
* @param x		mouse's location in the window-relative x coordinate in the moment the key is pressed
* @param y		mouse's location in the window-relative y coordinate in the moment the key is pressed
*/
void keyboardGL(unsigned char c, int x, int y)
{
	// printing pressed keys (for test purpose)
	std::cout << "Pressed key: " << (int) c << std::endl;

	switch (c)
	{

		// SPACE pauses animation
		case ' ':
			g_isPaused = !g_isPaused;
			break;

		// ESC key closes the window
		case 27:
			glutLeaveMainLoop();
			break;
	}
}


/**
* mouseGL -> OpenGL callback function
* method registered with GLUT and invoked when the user presses a mouse button on the current window
* (each press and release action will generate an event)
* @param button		GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON or GLUT_RIGHT_BUTTON
* @param state		    GLUT_DOWN or GLUT_UP
* @param x				mouse's x position relative to the current window
* @param y				mouse's y position relative to the current window
*/
void mouseGL(int button, int state, int x, int y)
{
	// add code here
}


/**
* motionGL -> OpenGL callback function
* method registered with GLUT and invoked when the mouse moves within the window while one or more mouse buttons are being pressed
* @param x				mouse's x position relative to the current window
* @param y				mouse's y position relative to the current window
*/
void motionGL(int x, int y)
{
	// add code here
}


/**
* reshapeGL -> OpenGL callback function
* method registered with GLUT and invoked when the render window is resized
* (it's also called the first time the window is displayed which means it can be used to setup the projection matrix)
* @param w				new window width size, in pixels
* @param h				new window heigth size, in pixels
*/
void reshapeGL(int w, int h)
{
	// defining the camera
	if (h == 0)
	{
		h = 1;
	}

	g_windowWidth = w;
	g_windowHeight = h;

	glViewport(0, 0, g_windowWidth, g_windowHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, g_windowWidth / (GLdouble) g_windowHeight, 0.1, 100.0);

	glutPostRedisplay();
}


/**
* drawRectangle
* @param w		width
* @param h		height
*/
void drawRectangle(float w, float h)
{
	// add code here
}


/**
* drawCircle
* @param r			radius
* @param numSides	number of sides
*/
void drawCircle(float r, int numSides)
{
	// add code here
}


/**
* drawTriangle
* @param p1		side 1
* @param p2		side 2
* @param p3		side 3
*/
void drawTriangle(float2 p1, float2 p2, float2 p3)
{
	// add code here
}


/**
* drawCube
* @param w		width
* @param h		height
* @param d		depth
*/
void drawCube(float w, float h, float d)
{
	// add code here
}


/**
* drawSphere
* @param r		radius
*/
void drawSphere(float r)
{
	// add code here
}


/**
* drawPyramid
* @param sc	scale
*/
void drawPyramid(float sc)
{
	// add code here
}


/**
* renderScene1: rendering primitives using immediate mode
*/
void renderScene1()
{
	// clearing screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -6.0f);
	glRotatef(g_Rotation1, 0.0f, 1.0f, 0.0f);

	// rendering a cube
	glBegin(GL_QUADS);

		//top face
		glColor3f(0.0f, 1.0f, 0.0f);		// green
		glVertex3f(1.0f, 1.0f, -1.0f);		// top-right
		glVertex3f(-1.0f, 1.0f, -1.0f);		// top-left
		glVertex3f(-1.0f, 1.0f, 1.0f);		// bottom-left
		glVertex3f(1.0f, 1.0f, 1.0f);		// bottom-right

		//bottom face
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);		// top-right
		glVertex3f(-1.0f, -1.0f, -1.0f);	// top-left
		glVertex3f(-1.0f, -1.0f, 1.0f);		// bottom-left
		glVertex3f(1.0f, -1.0f, 1.0f);		// bottom-right

		//front face
		glColor3f(1.0f, 0.0f, 0.0f);		// red
		glVertex3f(1.0f, 1.0f, 1.0f);		// top-right
		glVertex3f(-1.0f, 1.0f, 1.0f);		// top-left
		glVertex3f(-1.0f, -1.0f, 1.0f);		// bottom-left
		glVertex3f(1.0f, -1.0f, 1.0f);		// bottom-right

		//back face
		glColor3f(1.0f, 1.0f, 0.0f);		// yellow
		glVertex3f(1.0f, 1.0f, -1.0f);		// top-right
		glVertex3f(-1.0f, 1.0f, -1.0f);		// top-left
		glVertex3f(-1.0f, -1.0f, -1.0f);	// bottom-left
		glVertex3f(1.0f, -1.0f, -1.0f);		// bottom-right

		//left face
		glColor3f(0.0f, 0.0f, 1.0f);		// blue
		glVertex3f(-1.0f, 1.0f, 1.0f);		// top-right
		glVertex3f(-1.0f, 1.0f, -1.0f);		// top-left
		glVertex3f(-1.0f, -1.0f, -1.0f);	// bottom-left
		glVertex3f(-1.0f, -1.0f, 1.0f);		// bottom-right

		//right face
		glColor3f(1.0f, 0.0f, 1.0f);		// violet
		glVertex3f(1.0f, 1.0f, -1.0f);		// top-right
		glVertex3f(1.0f, 1.0f, 1.0f);		// top-left
		glVertex3f(1.0f, -1.0f, 1.0f);		// bottom-left
		glVertex3f(1.0f, -1.0f, -1.0f);		// bottom-right

	glEnd();

	glutSwapBuffers();
	glutPostRedisplay();
}


/**
* renderScene2
*/
void renderScene2()
{
	// add code here
}


/**
* renderScene3
*/
void renderScene3()
{
	// add code here
}


/**
* renderScene4
*/
void renderScene4()
{
	// add code here
}


/**
* renderScene5
*/
void renderScene5()
{
	// add code here
}


/**
* cleanUp
* cleans up resources
*/
void cleanUp(int exitCode, bool bExit)
{
	// add code here
}