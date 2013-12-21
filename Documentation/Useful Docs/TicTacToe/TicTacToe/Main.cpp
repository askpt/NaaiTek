#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>     
#include <GL\glut.h>
#include <iostream>


using namespace std;

struct float2
{
	float2(float _x = 0.0f, float _y = 0.0f) : x(_x), y(_y) {}

	float x;
	float y;
};

struct float3
{
	float3(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) : x(_x), y(_y), z(_z) {}

	float x;
	float y;
	float z;
};

int g_iWindowWidth = 512;
int g_iWindowHeight = 512;
int g_iGLUTWindowHandle = 0;

int g_iErrorCode = 0;

/* callbacks declaration*/


void InitGL(int argc, char* argv[]);

/*this function will be registered as the render function that is invoked by GLUT when the current windows needs to be redisplayed*/
void DisplayGL();

/*this function is registered with Glut and it is invoked when windows system events are not being received*/
void IdleGL();

/*callback keyboard*/
void KeyboardGL(unsigned char c, int x, int y);

/*callback mouse*/
void MouseGL(int button, int state, int x, int y);

/*this method is invoked when the mouse moves within the window while one or more mouse buttons are pressed*/
void MotionGL(int x, int y);

/*this callback is invoked when the render window is resized*/
void ReshapeGL(int w, int h);

/*draw callbacks*/

/*callback to draw the square*/
void DrawSquare(float width);

/*callback to draw the scene*/

void RenderScene();


/*callback to clear the scene*/

void CleanScene(int exitCode, bool bExit = true);

/*main method*/

int main(int argc, char * argv[])
{
	InitGL(argc, argv);
	glutMainLoop();
}

/*callback to draw the X*/

void DrawX();

/*callback to draw the O*/

void DrawO();

/*callback to draw the 9 squares forming one square*/



