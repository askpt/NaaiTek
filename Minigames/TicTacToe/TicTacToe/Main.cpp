#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <string>
#include <stdlib.h>     
#include <GL\glut.h>
#include <SWI-cpp.h>
#include <iostream>
#include "ImageLoader.h"
#include "text3d.h"
#include "Json.h"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

using namespace std;
//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
typedef struct
{
	char matrix[3][3];
	bool gameOver;
	bool winUser;
	bool winMachine;
	int win;
	int loose;
}Game;

float g_scale;

bool requestSent = false;
const char * stringVetor[2] = { "You won!", "You Loose!" };
const char * stringHelp[2] = { "F1->New Game", "F10->Exit" };

string user, friendUser;

const float DEG2RAD = 3.14159 / 180;
int WindowWidth = 600;
int WindowHeight = 600;
int GLUTWindowHandle = 0;
float squareWidth = WindowWidth / 5.0;
Game game;

/**function to init the game parametrers**/
void initGameData()
{
	game.matrix[0][0] = '1';
	game.matrix[0][1] = '2';
	game.matrix[0][2] = '3';
	game.matrix[1][0] = '4';
	game.matrix[1][1] = '5';
	game.matrix[1][2] = '6';
	game.matrix[2][0] = '7';
	game.matrix[2][1] = '8';
	game.matrix[2][2] = '9';
	game.winMachine = false;
	game.winUser = false;

}

void IAConnection(char play);

/*function to draw the result of the game*/

void writeTextOnBox(int x, int y, const char *string)
{
	//glLoadIdentity();
	int len, i;
	glColor3f(1.0f, 0.0f, 0.0f);
	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);
	}
	glFlush();
}

/*function to insert text into a specific area of the window*/
void drawTextBox()
{
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex2d(50, 490);
	glVertex2d(550, 490);
	glVertex2d(550, 590);
	glVertex2d(50, 590);
	glEnd();
	writeTextOnBox(60, 510, stringHelp[0]);
	writeTextOnBox(60, 530, stringHelp[1]);
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

/*
Square1:

A(100,100)
B(100+squareWidth,100)
C(100+squareWidth,100+squareWidth)
D(100,100+squareWidth)

Square2:

B(100+squareWidth,100)
C(100+squareWidth,100+squareWidth)
E(100+2*squareWidth,100)
F(100+2*squareWidth,100+squareWidth)

Square3:

E(100+2*squareWidth,100)
F(100+2*squareWidth,100+squareWidth)
G(100+3*squareWidth,100)
H(100+3*squareWidth,100+squareWidth)

Square4:

C(100+squareWidth,100+squareWidth)
D(100,100+squareWidth)
I(100,100+2*squareWidth)
J(100+squareWidth,100+2*squareWidth)

Square5:

C(100+squareWidth,100+squareWidth)
J(100+squareWidth,100+2*squareWidth)
F(100+2*squareWidth,100+squareWidth)
K(100+2*squareWidth,100+2*squareWidth)

Square6:

F(100+2*squareWidth,100+squareWidth)
H(100+3*squareWidth,100+squareWidth)
K(100+2*squareWidth,100+2*squareWidth)
L(100+3*squareWidth,100+2*squareWidth)

Square7:

I(100,100+2*squareWidth)
J(100+squareWidth,100+2*squareWidth)
M(100,100+3*squareWidth)
N(100+squareWidth,100+3*squareWidth)

Square8:

J(100+squareWidth,100+2*squareWidth)
K(100+2*squareWidth,100+2*squareWidth)
N(100+squareWidth,100+3*squareWidth)
O(100+2*squareWidth,100+3*squareWidth)

Square9:

K(100+2*squareWidth,100+2*squareWidth)
L(100+3*squareWidth,100+2*squareWidth)
O(100+2*squareWidth,100+3*squareWidth)
P(100+3*squareWidth,100+3*squareWidth)


/*this function draw 9 squares forming one big square*/
void drawTable()
{
	glColor3b(1.0, 0.0, 0.0);

	glLineWidth(4.0);
	glBegin(GL_LINES);
	glVertex2d(100, 100);
	glVertex2d(100 + squareWidth, 100);

	glVertex2d(100 + squareWidth, squareWidth + 100);
	glVertex2d(100, 100 + squareWidth);

	glVertex2d(100, 100);
	glVertex2d(100, 100 + squareWidth);

	glVertex2d(100 + squareWidth, 100 + squareWidth);
	glVertex2d(100 + squareWidth, 100);
	glEnd();

	glLineWidth(4.0);
	glBegin(GL_LINES);

	glVertex2d(100 + squareWidth, 100);
	glVertex2d(100 + 2 * squareWidth, 100);

	glVertex2d(100 + 2 * squareWidth, 100 + squareWidth);
	glVertex2d(100 + squareWidth, 100 + squareWidth);

	glVertex2d(100 + 2 * squareWidth, 100);
	glVertex2d(100 + 2 * squareWidth, 100 + squareWidth);

	glEnd();

	glLineWidth(4.0);
	glBegin(GL_LINES);

	glVertex2d(100 + 2 * squareWidth, 100);
	glVertex2d(100 + 3 * squareWidth, 100);

	glVertex2d(100 + 2 * squareWidth, 100 + squareWidth);
	glVertex2d(100 + 3 * squareWidth, 100 + squareWidth);

	glVertex2d(100 + 3 * squareWidth, 100);
	glVertex2d(100 + 3 * squareWidth, 100 + squareWidth);

	glEnd();

	glLineWidth(4.0);

	glBegin(GL_LINES);
	glVertex2d(100, 100 + squareWidth);
	glVertex2d(100, 100 + 2 * squareWidth);

	glVertex2d(100 + squareWidth, 100 + squareWidth);
	glVertex2d(100 + squareWidth, 100 + 2 * squareWidth);

	glVertex2d(100, 100 + 2 * squareWidth);
	glVertex2d(100 + squareWidth, 100 + 2 * squareWidth);

	glVertex2d(100 + squareWidth, 100 + 2 * squareWidth);
	glVertex2d(100 + 2 * squareWidth, 100 + 2 * squareWidth);

	glVertex2d(100 + 2 * squareWidth, 100 + squareWidth);
	glVertex2d(100 + 2 * squareWidth, 100 + 2 * squareWidth);

	glVertex2d(100 + 2 * squareWidth, 100 + 2 * squareWidth);
	glVertex2d(100 + 3 * squareWidth, 100 + 2 * squareWidth);

	glVertex2d(100 + 3 * squareWidth, 100 + squareWidth);
	glVertex2d(100 + 3 * squareWidth, 100 + 2 * squareWidth);

	glEnd();


	glLineWidth(4.0);

	glBegin(GL_LINES);
	glVertex2d(100, 100 + 2 * squareWidth);
	glVertex2d(100, 100 + 3 * squareWidth);

	glVertex2d(100, 100 + 3 * squareWidth);
	glVertex2d(100 + squareWidth, 100 + 3 * squareWidth);

	glVertex2d(100 + squareWidth, 100 + 3 * squareWidth);
	glVertex2d(100 + 2 * squareWidth, 100 + 3 * squareWidth);

	glVertex2d(100 + 2 * squareWidth, 100 + 3 * squareWidth);
	glVertex2d(100 + 3 * squareWidth, 100 + 3 * squareWidth);

	glVertex2d(100 + squareWidth, 100 + 2 * squareWidth);
	glVertex2d(100 + squareWidth, 100 + 3 * squareWidth);

	glVertex2d(100 + 2 * squareWidth, 100 + 2 * squareWidth);
	glVertex2d(100 + 2 * squareWidth, 100 + 3 * squareWidth);

	glVertex2d(100 + 3 * squareWidth, 100 + 2 * squareWidth);
	glVertex2d(100 + 3 * squareWidth, 100 + 3 * squareWidth);

	glEnd();
	drawTextBox();
	

}

/*function called by the mouse callback to draw X's on the specific squad*/
void drawX(int x, int y)
{
	glColor3b(1.0, 0.0, 0.0);

	glLineWidth(4.0);

	//square1
	if (x > 100 && x<(100 + squareWidth) && y>100 && y < (100 + squareWidth) && game.matrix[0][0] != 'x' && game.matrix[0][0] != 'o' && game.winMachine == false && game.winUser == false)
	{

		glBegin(GL_LINES);
		glVertex2d(100, 100);
		glVertex2d(100 + squareWidth, 100 + squareWidth);

		glVertex2d(100 + squareWidth, 100);
		glVertex2d(100, 100 + squareWidth);

		glEnd();
		glFlush();

		IAConnection('1');
	}
	else

		//square2
	if (x > 100 + squareWidth  && x< 100 + 2 * squareWidth && y>100 && y < 100 + squareWidth && game.matrix[0][1] != 'x' && game.matrix[0][1] != 'o'  && game.winMachine == false && game.winUser == false)
	{
		glBegin(GL_LINES);
		glVertex2d(100 + squareWidth, 100);
		glVertex2d(100 + 2 * squareWidth, 100 + squareWidth);

		glVertex2d(100 + 2 * squareWidth, 100);
		glVertex2d(100 + squareWidth, 100 + squareWidth);

		glEnd();
		glFlush();
		IAConnection('2');
	}
	else
		//square3
	if (x > 100 + 2 * squareWidth  && x< 100 + 3 * squareWidth && y>100 && y < 100 + squareWidth && game.matrix[0][2] != 'x' && game.matrix[0][2] != 'o'  && game.winMachine == false && game.winUser == false)
	{
		glBegin(GL_LINES);
		glVertex2d(100 + 2 * squareWidth, 100);
		glVertex2d(100 + 3 * squareWidth, 100 + squareWidth);

		glVertex2d(100 + 3 * squareWidth, 100);
		glVertex2d(100 + 2 * squareWidth, 100 + squareWidth);

		glEnd();
		glFlush();
		IAConnection('3');
	}
	else
		//square4
	if (x > 100 && x<(100 + squareWidth) && y>100 + squareWidth && y < (100 + 2 * squareWidth) && game.matrix[1][0] != 'x' && game.matrix[1][0] != 'o'  && game.winMachine == false && game.winUser == false)
	{
		glBegin(GL_LINES);
		glVertex2d(100, 100 + squareWidth);
		glVertex2d(100 + squareWidth, 100 + 2 * squareWidth);

		glVertex2d(100 + squareWidth, 100 + squareWidth);
		glVertex2d(100, 100 + 2 * squareWidth);

		glEnd();
		glFlush();
		IAConnection('4');
	}

	else
		//square5
	if (x > 100 + squareWidth  && x< 100 + 2 * squareWidth && y>100 + squareWidth && y < 100 + 2 * squareWidth && game.matrix[1][1] != 'x' && game.matrix[1][1] != 'o'  && game.winMachine == false && game.winUser == false)
	{
		glBegin(GL_LINES);
		glVertex2d(100 + squareWidth, 100 + squareWidth);
		glVertex2d(100 + 2 * squareWidth, 100 + 2 * squareWidth);

		glVertex2d(100 + 2 * squareWidth, 100 + squareWidth);
		glVertex2d(100 + squareWidth, 100 + 2 * squareWidth);


		glEnd();
		glFlush();
		IAConnection('5');
	}
	else
		//square6
	if (x > 100 + 2 * squareWidth  && x< 100 + 3 * squareWidth && y>100 + squareWidth && y < 100 + 2 * squareWidth && game.matrix[1][2] != 'x' && game.matrix[1][2] != 'o'  && game.winMachine == false && game.winUser == false)
	{
		glBegin(GL_LINES);
		glVertex2d(100 + 2 * squareWidth, 100 + squareWidth);
		glVertex2d(100 + 3 * squareWidth, 100 + 2 * squareWidth);

		glVertex2d(100 + 3 * squareWidth, 100 + squareWidth);
		glVertex2d(100 + 2 * squareWidth, 100 + 2 * squareWidth);

		glEnd();
		glFlush();
		IAConnection('6');
	}
	else
		//square7
	if (x > 100 && x<(100 + squareWidth) && y>100 + 2 * squareWidth && y < (100 + 3 * squareWidth) && game.matrix[2][0] != 'x' && game.matrix[2][0] != 'o'  && game.winMachine == false && game.winUser == false)
	{

		glBegin(GL_LINES);
		glVertex2d(100, 100 + 2 * squareWidth);
		glVertex2d(100 + squareWidth, 100 + 3 * squareWidth);

		glVertex2d(100 + squareWidth, 100 + 2 * squareWidth);
		glVertex2d(100, 100 + 3 * squareWidth);

		glEnd();
		glFlush();
		IAConnection('7');
	}
	else
		//square8
	if (x > 100 + squareWidth  && x< 100 + 2 * squareWidth && y>100 + 2 * squareWidth && y < 100 + 3 * squareWidth && game.matrix[2][1] != 'x' && game.matrix[2][1] != 'o'  && game.winMachine == false && game.winUser == false)
	{
		glBegin(GL_LINES);
		glVertex2d(100 + squareWidth, 100 + 2 * squareWidth);
		glVertex2d(100 + 2 * squareWidth, 100 + 3 * squareWidth);

		glVertex2d(100 + 2 * squareWidth, 100 + 2 * squareWidth);
		glVertex2d(100 + squareWidth, 100 + 3 * squareWidth);

		glEnd();
		glFlush();
		IAConnection('8');
	}
	else
		//square9
	if (x > 100 + 2 * squareWidth  && x< 100 + 3 * squareWidth && y>100 + 2 * squareWidth && y < 100 + 3 * squareWidth && game.matrix[2][2] != 'x' && game.matrix[2][2] != 'o'  && game.winMachine == false && game.winUser == false)
	{
		glBegin(GL_LINES);
		glVertex2d(100 + 2 * squareWidth, 100 + 2 * squareWidth);
		glVertex2d(100 + 3 * squareWidth, 100 + 3 * squareWidth);

		glVertex2d(100 + 3 * squareWidth, 100 + 2 * squareWidth);
		glVertex2d(100 + 2 * squareWidth, 100 + 3 * squareWidth);

		glEnd();
		glFlush();
		IAConnection('9');
	}

}

/*function called by the mouse callback to draw X's on the specific squad*/
void drawO(int x, int y)
{
	glColor3b(1.0, 0.0, 0.0);
	//square1
	if (x > 100 && x<(100 + squareWidth) && y>100 && y < (100 + squareWidth) && game.winMachine == false && game.winUser == false)
	{
		GLint i;
		GLfloat t, xTemp, yTemp;
		GLfloat x0 = (2 * 100 + squareWidth) / 2.0;
		GLfloat y0 = (2 * 100 + squareWidth) / 2.0;
		t = 0.0, xTemp = 0.0, yTemp = 0.0;
		GLfloat r = squareWidth / 2.5;
		glBegin(GL_POLYGON);
		for (i = 0; i < 50; i++)
		{
			xTemp = r * cos(t) + x0;
			yTemp = r * sin(t) + y0;
			glVertex2f(xTemp, yTemp);
			t += 2.0*M_PI / 50;
		}
		glEnd();
		glFlush();
	}
	else
		//square2
	if (x > 100 + squareWidth  && x< 100 + 2 * squareWidth && y>100 && y < 100 + squareWidth  && game.winMachine == false && game.winUser == false)
	{
		GLint i;
		GLfloat t, xTemp, yTemp;
		GLfloat x0 = (100 + squareWidth + (squareWidth / 2.0));
		GLfloat y0 = (100 + (squareWidth / 2.0));
		t = 0.0, xTemp = 0.0, yTemp = 0.0;
		GLfloat r = squareWidth / 2.5;
		glBegin(GL_POLYGON);
		for (i = 0; i < 50; i++)
		{
			xTemp = r * cos(t) + x0;
			yTemp = r * sin(t) + y0;
			glVertex2f(xTemp, yTemp);
			t += 2.0*M_PI / 50;
		}
		glEnd();
		glFlush();
	}

	else
		//square3
	if (x > 100 + 2 * squareWidth  && x< 100 + 3 * squareWidth && y>100 && y < 100 + squareWidth  && game.winMachine == false && game.winUser == false)
	{
		GLint i;
		GLfloat t, xTemp, yTemp;
		GLfloat x0 = (100 + 2 * squareWidth + (squareWidth / 2.0));
		GLfloat y0 = (100 + (squareWidth / 2.0));
		t = 0.0, xTemp = 0.0, yTemp = 0.0;
		GLfloat r = squareWidth / 2.5;
		glBegin(GL_POLYGON);
		for (i = 0; i < 50; i++)
		{
			xTemp = r * cos(t) + x0;
			yTemp = r * sin(t) + y0;
			glVertex2f(xTemp, yTemp);
			t += 2.0*M_PI / 50;
		}
		glEnd();
		glFlush();
	}

	else
		//square4
	if (x > 100 && x<(100 + squareWidth) && y>100 + squareWidth && y < (100 + 2 * squareWidth) && game.winMachine == false && game.winUser == false)
	{
		GLint i;
		GLfloat t, xTemp, yTemp;
		GLfloat x0 = (100 + (squareWidth / 2.0));
		GLfloat y0 = (100 + squareWidth + (squareWidth / 2.0));
		t = 0.0, xTemp = 0.0, yTemp = 0.0;
		GLfloat r = squareWidth / 2.5;
		glBegin(GL_POLYGON);
		for (i = 0; i < 50; i++)
		{
			xTemp = r * cos(t) + x0;
			yTemp = r * sin(t) + y0;
			glVertex2f(xTemp, yTemp);
			t += 2.0*M_PI / 50;
		}
		glEnd();
		glFlush();
	}
	else
		//square5
	if (x > 100 + squareWidth  && x< 100 + 2 * squareWidth && y>100 + squareWidth && y < 100 + 2 * squareWidth  && game.winMachine == false && game.winUser == false)
	{
		GLint i;
		GLfloat t, xTemp, yTemp;
		GLfloat x0 = (100 + squareWidth + (squareWidth / 2.0));
		GLfloat y0 = (100 + squareWidth + (squareWidth / 2.0));
		t = 0.0, xTemp = 0.0, yTemp = 0.0;
		GLfloat r = squareWidth / 2.5;
		glBegin(GL_POLYGON);
		for (i = 0; i < 50; i++)
		{
			xTemp = r * cos(t) + x0;
			yTemp = r * sin(t) + y0;
			glVertex2f(xTemp, yTemp);
			t += 2.0*M_PI / 50;
		}
		glEnd();
		glFlush();
	}
	else
		//square6
	if (x > 100 + 2 * squareWidth  && x< 100 + 3 * squareWidth && y>100 + squareWidth && y < 100 + 2 * squareWidth  && game.winMachine == false && game.winUser == false)
	{
		GLint i;
		GLfloat t, xTemp, yTemp;
		GLfloat x0 = (100 + 2 * squareWidth + (squareWidth / 2.0));
		GLfloat y0 = (100 + squareWidth + (squareWidth / 2.0));
		t = 0.0, xTemp = 0.0, yTemp = 0.0;
		GLfloat r = squareWidth / 2.5;
		glBegin(GL_POLYGON);
		for (i = 0; i < 50; i++)
		{
			xTemp = r * cos(t) + x0;
			yTemp = r * sin(t) + y0;
			glVertex2f(xTemp, yTemp);
			t += 2.0*M_PI / 50;
		}
		glEnd();
		glFlush();
	}

	else
		//square7
	if (x > 100 && x<(100 + squareWidth) && y>100 + 2 * squareWidth && y < (100 + 3 * squareWidth) && game.winMachine == false && game.winUser == false)
	{
		GLint i;
		GLfloat t, xTemp, yTemp;
		GLfloat x0 = (100 + (squareWidth / 2.0));
		GLfloat y0 = (100 + 2 * squareWidth + (squareWidth / 2.0));
		t = 0.0, xTemp = 0.0, yTemp = 0.0;
		GLfloat r = squareWidth / 2.5;
		glBegin(GL_POLYGON);
		for (i = 0; i < 50; i++)
		{
			xTemp = r * cos(t) + x0;
			yTemp = r * sin(t) + y0;
			glVertex2f(xTemp, yTemp);
			t += 2.0*M_PI / 50;
		}
		glEnd();
		glFlush();
	}
	else
		//square8
	if (x > 100 + squareWidth  && x< 100 + 2 * squareWidth && y>100 + 2 * squareWidth && y < 100 + 3 * squareWidth  && game.winMachine == false && game.winUser == false)
	{
		GLint i;
		GLfloat t, xTemp, yTemp;
		GLfloat x0 = (100 + squareWidth + (squareWidth / 2.0));
		GLfloat y0 = (100 + 2 * squareWidth + (squareWidth / 2.0));
		t = 0.0, xTemp = 0.0, yTemp = 0.0;
		GLfloat r = squareWidth / 2.5;
		glBegin(GL_POLYGON);
		for (i = 0; i < 50; i++)
		{
			xTemp = r * cos(t) + x0;
			yTemp = r * sin(t) + y0;
			glVertex2f(xTemp, yTemp);
			t += 2.0*M_PI / 50;
		}
		glEnd();
		glFlush();
	}
	else
		//square9
	if (x > 100 + 2 * squareWidth  && x< 100 + 3 * squareWidth && y>100 + 2 * squareWidth && y < 100 + 3 * squareWidth  && game.winMachine == false && game.winUser == false)
	{
		GLint i;
		GLfloat t, xTemp, yTemp;
		GLfloat x0 = (100 + 2 * squareWidth + (squareWidth / 2.0));
		GLfloat y0 = (100 + 2 * squareWidth + (squareWidth / 2.0));
		t = 0.0, xTemp = 0.0, yTemp = 0.0;
		GLfloat r = squareWidth / 2.5;
		glBegin(GL_POLYGON);
		for (i = 0; i < 50; i++)
		{
			xTemp = r * cos(t) + x0;
			yTemp = r * sin(t) + y0;
			glVertex2f(xTemp, yTemp);
			t += 2.0*M_PI / 50;
		}
		glEnd();
		glFlush();
	}

}
void display(void)
{

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

/*callback mouse*/
void mouse(int btn, int mouseState, int x, int y)
{
	switch (btn){
	case GLUT_LEFT_BUTTON:

		if (mouseState == GLUT_DOWN)
		{
			drawX(x, y);
		}
	}
}

/*callback special key of keyobard*/
void keyBoard(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_F1:
		glutPostRedisplay();
		initGameData();
		break;
	case GLUT_KEY_F10:
		exit(0);
		break;
	}

}

/*function to check if machine win*/

void checkMachineWin()
{
	if (game.matrix[0][0] == 'o' && game.matrix[0][1] == 'o' && game.matrix[0][2] == 'o')
	{
		game.winMachine = true;
	}
	if (game.matrix[1][0] == 'o' && game.matrix[1][1] == 'o' && game.matrix[1][2] == 'o')
	{
		game.winMachine = true;
	}
	if (game.matrix[2][0] == 'o' && game.matrix[2][1] == 'o' && game.matrix[2][2] == 'o')
	{
		game.winMachine = true;
	}
	if (game.matrix[0][0] == 'o' && game.matrix[1][0] == 'o' && game.matrix[2][0] == 'o')
	{
		game.winMachine = true;
	}
	if (game.matrix[0][1] == 'o' && game.matrix[1][1] == 'o' && game.matrix[2][1] == 'o')
	{
		game.winMachine = true;
	}
	if (game.matrix[0][2] == 'o' && game.matrix[1][2] == 'o' && game.matrix[2][2] == 'o')
	{
		game.winMachine = true;
	}

	if (game.matrix[0][0] == 'o' && game.matrix[1][1] == 'o' && game.matrix[2][2] == 'o')
	{
		game.winMachine = true;
	}
	if (game.matrix[2][0] == 'o' && game.matrix[1][1] == 'o' && game.matrix[0][2] == 'o')
	{
		game.winMachine = true;
	}
}

/*function to check if user win*/

void checkUserWin()
{

	if (game.matrix[0][0] == 'x' && game.matrix[0][1] == 'x' && game.matrix[0][2] == 'x')
	{
		game.winUser = true;
	}
	if (game.matrix[1][0] == 'x' && game.matrix[1][1] == 'x' && game.matrix[1][2] == 'x')
	{
		game.winUser = true;

	}
	if (game.matrix[2][0] == 'x' && game.matrix[2][1] == 'x' && game.matrix[2][2] == 'x')
	{
		game.winUser = true;
	}
	if (game.matrix[0][0] == 'x' && game.matrix[1][0] == 'x' && game.matrix[2][0] == 'x')
	{
		game.winUser = true;
	}
	if (game.matrix[0][1] == 'x' && game.matrix[1][1] == 'x' && game.matrix[2][1] == 'x')
	{
		game.winUser = true;
	}
	if (game.matrix[0][2] == 'x' && game.matrix[1][2] == 'x' && game.matrix[2][2] == 'x')
	{
		game.winUser = true;
	}

	if (game.matrix[0][0] == 'x' && game.matrix[1][1] == 'x' && game.matrix[2][2] == 'x')
	{
		game.winUser = true;
	}
	if (game.matrix[2][0] == 'x' && game.matrix[1][1] == 'x' && game.matrix[0][2] == 'x')
	{
		game.winUser = true;
	}
}

void SendRequest()
{
	wstring userWs(user.begin(), user.end());
	wstring friendWs(friendUser.begin(), friendUser.end());

	wstring url = L"http://uvm061.dei.isep.ipp.pt/accept_response?personA=" + userWs + L"&personB=" + friendWs;
	wcout << url << endl;
}


/*function to update the bordergame*/

void updateBorderGame(string border)
{
	char square1 = border.at(2);
	char square2 = border.at(4);
	char square3 = border.at(6);
	char square4 = border.at(10);
	char square5 = border.at(12);
	char square6 = border.at(14);
	char square7 = border.at(18);
	char square8 = border.at(20);
	char square9 = border.at(22);

	if (square1 != game.matrix[0][0])
	{
		game.matrix[0][0] = square1;
		if (square1 == 'o')
		{
			drawO(110, 110);
		}
	}
	if (square2 != game.matrix[0][1])
	{
		game.matrix[0][1] = square2;
		if (square2 == 'o')
		{
			drawO(110 + squareWidth, 110);
		}
	}
	if (square3 != game.matrix[0][2])
	{
		game.matrix[0][2] = square3;
		if (square3 == 'o')
		{
			drawO(110 + 2 * squareWidth, 110);
		}
	}
	if (square4 != game.matrix[1][0])
	{
		game.matrix[1][0] = square4;
		if (square4 == 'o')
		{
			drawO(110, 110 + squareWidth);
		}
	}
	if (square5 != game.matrix[1][1])
	{
		game.matrix[1][1] = square5;
		if (square5 == 'o')
		{
			drawO(110 + squareWidth, 110 + squareWidth);
		}
	}
	if (square6 != game.matrix[1][2])
	{
		game.matrix[1][2] = square6;
		if (square6 == 'o')
		{
			drawO(110 + 2 * squareWidth, 110 + squareWidth);
		}
	}
	if (square7 != game.matrix[2][0])
	{
		if (square7 == 'o')
		{
			drawO(110, 110 + 2 * squareWidth);
		}
		game.matrix[2][0] = square7;
	}
	if (square8 != game.matrix[2][1])
	{
		game.matrix[2][1] = square8;
		if (square8 == 'o')
		{
			drawO(110 + squareWidth, 110 + 2 * squareWidth);
		}
	}
	if (square9 != game.matrix[2][2])
	{
		game.matrix[2][2] = square9;
		if (square9 == 'o')
		{
			drawO(110 + 2 * squareWidth, 110 + 2 * squareWidth);
		}
	}
	checkMachineWin();
	checkUserWin();
	if (game.winMachine)
	{
		writeTextOnBox(60, 550, stringVetor[1]);
	}
	if (game.winUser)
	{
		if (!requestSent)
		{
			SendRequest();
			requestSent = true;
		}
		writeTextOnBox(60, 550, stringVetor[0]);

	}

}
/*function to return wich user win*/
bool checkWinnerToMainGame()
{
	if (game.winUser)
	{
		return true;
	}
	else
		return false;
}

/*this method will be called every time that the user select a square*/
void IAConnection(char play)
{
	char *plargv[] = { "swipl.dll", "-s", "ttt-off.pl", NULL };
	PlEngine e(3, plargv);
	PlTermv av(3);

	char boardgame[100];
	sprintf_s(boardgame, "[[%c,%c,%c],[%c,%c,%c],[%c,%c,%c]]", game.matrix[0][0], game.matrix[0][1], game.matrix[0][2], game.matrix[1][0], game.matrix[1][1], game.matrix[1][2], game.matrix[2][0], game.matrix[2][1], game.matrix[2][2]);

	av[0] = PlCompound(boardgame);
	string borderReceived;
	char userPlay[10];
	sprintf_s(userPlay, "%c", play);
	av[1] = PlCompound(userPlay);
	PlQuery q("receive", av);
	bool flag = false;


	while (q.next_solution())
	{
		flag = true;

		borderReceived = (string)av[2];
	}
	if (flag)
		updateBorderGame(borderReceived);

}

int main(int argc, char **argv)
{
	argv[1] = "aaa";
	argv[2] = "bbb";

	user = argv[1];
	friendUser = argv[2];

	char fakeParam[] = "fake";
	char *fakeargv[] = { fakeParam, NULL };
	int fakeargc = 1;


	initGameData();
	glutInit(&fakeargc, fakeargv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutCreateWindow("TIC-TAC-TOE");
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutSpecialFunc(keyBoard);
	myInit();
	glutMainLoop();
	return 0;
}