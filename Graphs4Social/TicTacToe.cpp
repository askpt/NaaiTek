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
#include "TicTacToe.h"

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
}TicGame;

const char * ticStringVetor[2] = { "You won!", "You Loose!" };
const char * ticStringHelp[2] = { "F1->New Game", "F10->Exit" };

int TicTacToeWindowWidth = 600;
int TicTacToeWindowHeight = 600;
int GLUTWindowHandleTic = 0;
float ticSquareWidth = TicTacToeWindowWidth / 5.0;
TicGame ticgame;

/**function to init the game parametrers**/
void initGameData()
{
	ticgame.matrix[0][0] = '1';
	ticgame.matrix[0][1] = '2';
	ticgame.matrix[0][2] = '3';
	ticgame.matrix[1][0] = '4';
	ticgame.matrix[1][1] = '5';
	ticgame.matrix[1][2] = '6';
	ticgame.matrix[2][0] = '7';
	ticgame.matrix[2][1] = '8';
	ticgame.matrix[2][2] = '9';
	ticgame.winMachine = false;
	ticgame.winUser = false;

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
	writeTextOnBox(60, 510, ticStringHelp[0]);
	writeTextOnBox(60, 530, ticStringHelp[1]);
}

/*function to initialize OpenGL*/

void ticTacToeInit()
{


	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glViewport(0, 0, TicTacToeWindowWidth, TicTacToeWindowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)TicTacToeWindowWidth, (GLdouble)TicTacToeWindowHeight, 0.0);

}

/*
Square1:

A(100,100)
B(100+ticSquareWidth,100)
C(100+ticSquareWidth,100+ticSquareWidth)
D(100,100+ticSquareWidth)

Square2:

B(100+ticSquareWidth,100)
C(100+ticSquareWidth,100+ticSquareWidth)
E(100+2*ticSquareWidth,100)
F(100+2*ticSquareWidth,100+ticSquareWidth)

Square3:

E(100+2*ticSquareWidth,100)
F(100+2*ticSquareWidth,100+ticSquareWidth)
G(100+3*ticSquareWidth,100)
H(100+3*ticSquareWidth,100+ticSquareWidth)

Square4:

C(100+ticSquareWidth,100+ticSquareWidth)
D(100,100+ticSquareWidth)
I(100,100+2*ticSquareWidth)
J(100+ticSquareWidth,100+2*ticSquareWidth)

Square5:

C(100+ticSquareWidth,100+ticSquareWidth)
J(100+ticSquareWidth,100+2*ticSquareWidth)
F(100+2*ticSquareWidth,100+ticSquareWidth)
K(100+2*ticSquareWidth,100+2*ticSquareWidth)

Square6:

F(100+2*ticSquareWidth,100+ticSquareWidth)
H(100+3*ticSquareWidth,100+ticSquareWidth)
K(100+2*ticSquareWidth,100+2*ticSquareWidth)
L(100+3*ticSquareWidth,100+2*ticSquareWidth)

Square7:

I(100,100+2*ticSquareWidth)
J(100+ticSquareWidth,100+2*ticSquareWidth)
M(100,100+3*ticSquareWidth)
N(100+ticSquareWidth,100+3*ticSquareWidth)

Square8:

J(100+ticSquareWidth,100+2*ticSquareWidth)
K(100+2*ticSquareWidth,100+2*ticSquareWidth)
N(100+ticSquareWidth,100+3*ticSquareWidth)
O(100+2*ticSquareWidth,100+3*ticSquareWidth)

Square9:

K(100+2*ticSquareWidth,100+2*ticSquareWidth)
L(100+3*ticSquareWidth,100+2*ticSquareWidth)
O(100+2*ticSquareWidth,100+3*ticSquareWidth)
P(100+3*ticSquareWidth,100+3*ticSquareWidth)


/*this function draw 9 squares forming one big square*/
void drawTable()
{
	glLoadIdentity();
	glColor3b(1.0, 0.0, 0.0);

	glLineWidth(4.0);
	glBegin(GL_LINES);
	glVertex2d(100, 100);
	glVertex2d(100 + ticSquareWidth, 100);

	glVertex2d(100 + ticSquareWidth, ticSquareWidth + 100);
	glVertex2d(100, 100 + ticSquareWidth);

	glVertex2d(100, 100);
	glVertex2d(100, 100 + ticSquareWidth);

	glVertex2d(100 + ticSquareWidth, 100 + ticSquareWidth);
	glVertex2d(100 + ticSquareWidth, 100);
	glEnd();

	glLineWidth(4.0);
	glBegin(GL_LINES);

	glVertex2d(100 + ticSquareWidth, 100);
	glVertex2d(100 + 2 * ticSquareWidth, 100);

	glVertex2d(100 + 2 * ticSquareWidth, 100 + ticSquareWidth);
	glVertex2d(100 + ticSquareWidth, 100 + ticSquareWidth);

	glVertex2d(100 + 2 * ticSquareWidth, 100);
	glVertex2d(100 + 2 * ticSquareWidth, 100 + ticSquareWidth);

	glEnd();

	glLineWidth(4.0);
	glBegin(GL_LINES);

	glVertex2d(100 + 2 * ticSquareWidth, 100);
	glVertex2d(100 + 3 * ticSquareWidth, 100);

	glVertex2d(100 + 2 * ticSquareWidth, 100 + ticSquareWidth);
	glVertex2d(100 + 3 * ticSquareWidth, 100 + ticSquareWidth);

	glVertex2d(100 + 3 * ticSquareWidth, 100);
	glVertex2d(100 + 3 * ticSquareWidth, 100 + ticSquareWidth);

	glEnd();

	glLineWidth(4.0);

	glBegin(GL_LINES);
	glVertex2d(100, 100 + ticSquareWidth);
	glVertex2d(100, 100 + 2 * ticSquareWidth);

	glVertex2d(100 + ticSquareWidth, 100 + ticSquareWidth);
	glVertex2d(100 + ticSquareWidth, 100 + 2 * ticSquareWidth);

	glVertex2d(100, 100 + 2 * ticSquareWidth);
	glVertex2d(100 + ticSquareWidth, 100 + 2 * ticSquareWidth);

	glVertex2d(100 + ticSquareWidth, 100 + 2 * ticSquareWidth);
	glVertex2d(100 + 2 * ticSquareWidth, 100 + 2 * ticSquareWidth);

	glVertex2d(100 + 2 * ticSquareWidth, 100 + ticSquareWidth);
	glVertex2d(100 + 2 * ticSquareWidth, 100 + 2 * ticSquareWidth);

	glVertex2d(100 + 2 * ticSquareWidth, 100 + 2 * ticSquareWidth);
	glVertex2d(100 + 3 * ticSquareWidth, 100 + 2 * ticSquareWidth);

	glVertex2d(100 + 3 * ticSquareWidth, 100 + ticSquareWidth);
	glVertex2d(100 + 3 * ticSquareWidth, 100 + 2 * ticSquareWidth);

	glEnd();


	glLineWidth(4.0);

	glBegin(GL_LINES);
	glVertex2d(100, 100 + 2 * ticSquareWidth);
	glVertex2d(100, 100 + 3 * ticSquareWidth);

	glVertex2d(100, 100 + 3 * ticSquareWidth);
	glVertex2d(100 + ticSquareWidth, 100 + 3 * ticSquareWidth);

	glVertex2d(100 + ticSquareWidth, 100 + 3 * ticSquareWidth);
	glVertex2d(100 + 2 * ticSquareWidth, 100 + 3 * ticSquareWidth);

	glVertex2d(100 + 2 * ticSquareWidth, 100 + 3 * ticSquareWidth);
	glVertex2d(100 + 3 * ticSquareWidth, 100 + 3 * ticSquareWidth);

	glVertex2d(100 + ticSquareWidth, 100 + 2 * ticSquareWidth);
	glVertex2d(100 + ticSquareWidth, 100 + 3 * ticSquareWidth);

	glVertex2d(100 + 2 * ticSquareWidth, 100 + 2 * ticSquareWidth);
	glVertex2d(100 + 2 * ticSquareWidth, 100 + 3 * ticSquareWidth);

	glVertex2d(100 + 3 * ticSquareWidth, 100 + 2 * ticSquareWidth);
	glVertex2d(100 + 3 * ticSquareWidth, 100 + 3 * ticSquareWidth);

	glEnd();
	drawTextBox();


}

/*function called by the mouse callback to draw X's on the specific squad*/
void drawX(int x, int y)
{
	glColor3b(1.0, 0.0, 0.0);

	glLineWidth(4.0);

	//square1
	if (x > 100 && x<(100 + ticSquareWidth) && y>100 && y < (100 + ticSquareWidth) && ticgame.matrix[0][0] != 'x' && ticgame.matrix[0][0] != 'o' && ticgame.winMachine == false && ticgame.winUser == false)
	{

		glBegin(GL_LINES);
		glVertex2d(100, 100);
		glVertex2d(100 + ticSquareWidth, 100 + ticSquareWidth);

		glVertex2d(100 + ticSquareWidth, 100);
		glVertex2d(100, 100 + ticSquareWidth);

		glEnd();
		glFlush();

		IAConnection('1');
	}
	else

		//square2
	if (x > 100 + ticSquareWidth  && x< 100 + 2 * ticSquareWidth && y>100 && y < 100 + ticSquareWidth && ticgame.matrix[0][1] != 'x' && ticgame.matrix[0][1] != 'o'  && ticgame.winMachine == false && ticgame.winUser == false)
	{
		glBegin(GL_LINES);
		glVertex2d(100 + ticSquareWidth, 100);
		glVertex2d(100 + 2 * ticSquareWidth, 100 + ticSquareWidth);

		glVertex2d(100 + 2 * ticSquareWidth, 100);
		glVertex2d(100 + ticSquareWidth, 100 + ticSquareWidth);

		glEnd();
		glFlush();
		IAConnection('2');
	}
	else
		//square3
	if (x > 100 + 2 * ticSquareWidth  && x< 100 + 3 * ticSquareWidth && y>100 && y < 100 + ticSquareWidth && ticgame.matrix[0][2] != 'x' && ticgame.matrix[0][2] != 'o'  && ticgame.winMachine == false && ticgame.winUser == false)
	{
		glBegin(GL_LINES);
		glVertex2d(100 + 2 * ticSquareWidth, 100);
		glVertex2d(100 + 3 * ticSquareWidth, 100 + ticSquareWidth);

		glVertex2d(100 + 3 * ticSquareWidth, 100);
		glVertex2d(100 + 2 * ticSquareWidth, 100 + ticSquareWidth);

		glEnd();
		glFlush();
		IAConnection('3');
	}
	else
		//square4
	if (x > 100 && x<(100 + ticSquareWidth) && y>100 + ticSquareWidth && y < (100 + 2 * ticSquareWidth) && ticgame.matrix[1][0] != 'x' && ticgame.matrix[1][0] != 'o'  && ticgame.winMachine == false && ticgame.winUser == false)
	{
		glBegin(GL_LINES);
		glVertex2d(100, 100 + ticSquareWidth);
		glVertex2d(100 + ticSquareWidth, 100 + 2 * ticSquareWidth);

		glVertex2d(100 + ticSquareWidth, 100 + ticSquareWidth);
		glVertex2d(100, 100 + 2 * ticSquareWidth);

		glEnd();
		glFlush();
		IAConnection('4');
	}

	else
		//square5
	if (x > 100 + ticSquareWidth  && x< 100 + 2 * ticSquareWidth && y>100 + ticSquareWidth && y < 100 + 2 * ticSquareWidth && ticgame.matrix[1][1] != 'x' && ticgame.matrix[1][1] != 'o'  && ticgame.winMachine == false && ticgame.winUser == false)
	{
		glBegin(GL_LINES);
		glVertex2d(100 + ticSquareWidth, 100 + ticSquareWidth);
		glVertex2d(100 + 2 * ticSquareWidth, 100 + 2 * ticSquareWidth);

		glVertex2d(100 + 2 * ticSquareWidth, 100 + ticSquareWidth);
		glVertex2d(100 + ticSquareWidth, 100 + 2 * ticSquareWidth);


		glEnd();
		glFlush();
		IAConnection('5');
	}
	else
		//square6
	if (x > 100 + 2 * ticSquareWidth  && x< 100 + 3 * ticSquareWidth && y>100 + ticSquareWidth && y < 100 + 2 * ticSquareWidth && ticgame.matrix[1][2] != 'x' && ticgame.matrix[1][2] != 'o'  && ticgame.winMachine == false && ticgame.winUser == false)
	{
		glBegin(GL_LINES);
		glVertex2d(100 + 2 * ticSquareWidth, 100 + ticSquareWidth);
		glVertex2d(100 + 3 * ticSquareWidth, 100 + 2 * ticSquareWidth);

		glVertex2d(100 + 3 * ticSquareWidth, 100 + ticSquareWidth);
		glVertex2d(100 + 2 * ticSquareWidth, 100 + 2 * ticSquareWidth);

		glEnd();
		glFlush();
		IAConnection('6');
	}
	else
		//square7
	if (x > 100 && x<(100 + ticSquareWidth) && y>100 + 2 * ticSquareWidth && y < (100 + 3 * ticSquareWidth) && ticgame.matrix[2][0] != 'x' && ticgame.matrix[2][0] != 'o'  && ticgame.winMachine == false && ticgame.winUser == false)
	{

		glBegin(GL_LINES);
		glVertex2d(100, 100 + 2 * ticSquareWidth);
		glVertex2d(100 + ticSquareWidth, 100 + 3 * ticSquareWidth);

		glVertex2d(100 + ticSquareWidth, 100 + 2 * ticSquareWidth);
		glVertex2d(100, 100 + 3 * ticSquareWidth);

		glEnd();
		glFlush();
		IAConnection('7');
	}
	else
		//square8
	if (x > 100 + ticSquareWidth  && x< 100 + 2 * ticSquareWidth && y>100 + 2 * ticSquareWidth && y < 100 + 3 * ticSquareWidth && ticgame.matrix[2][1] != 'x' && ticgame.matrix[2][1] != 'o'  && ticgame.winMachine == false && ticgame.winUser == false)
	{
		glBegin(GL_LINES);
		glVertex2d(100 + ticSquareWidth, 100 + 2 * ticSquareWidth);
		glVertex2d(100 + 2 * ticSquareWidth, 100 + 3 * ticSquareWidth);

		glVertex2d(100 + 2 * ticSquareWidth, 100 + 2 * ticSquareWidth);
		glVertex2d(100 + ticSquareWidth, 100 + 3 * ticSquareWidth);

		glEnd();
		glFlush();
		IAConnection('8');
	}
	else
		//square9
	if (x > 100 + 2 * ticSquareWidth  && x< 100 + 3 * ticSquareWidth && y>100 + 2 * ticSquareWidth && y < 100 + 3 * ticSquareWidth && ticgame.matrix[2][2] != 'x' && ticgame.matrix[2][2] != 'o'  && ticgame.winMachine == false && ticgame.winUser == false)
	{
		glBegin(GL_LINES);
		glVertex2d(100 + 2 * ticSquareWidth, 100 + 2 * ticSquareWidth);
		glVertex2d(100 + 3 * ticSquareWidth, 100 + 3 * ticSquareWidth);

		glVertex2d(100 + 3 * ticSquareWidth, 100 + 2 * ticSquareWidth);
		glVertex2d(100 + 2 * ticSquareWidth, 100 + 3 * ticSquareWidth);

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
	if (x > 100 && x<(100 + ticSquareWidth) && y>100 && y < (100 + ticSquareWidth) && ticgame.winMachine == false && ticgame.winUser == false)
	{
		GLint i;
		GLfloat t, xTemp, yTemp;
		GLfloat x0 = (2 * 100 + ticSquareWidth) / 2.0;
		GLfloat y0 = (2 * 100 + ticSquareWidth) / 2.0;
		t = 0.0, xTemp = 0.0, yTemp = 0.0;
		GLfloat r = ticSquareWidth / 2.5;
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
	if (x > 100 + ticSquareWidth  && x< 100 + 2 * ticSquareWidth && y>100 && y < 100 + ticSquareWidth  && ticgame.winMachine == false && ticgame.winUser == false)
	{
		GLint i;
		GLfloat t, xTemp, yTemp;
		GLfloat x0 = (100 + ticSquareWidth + (ticSquareWidth / 2.0));
		GLfloat y0 = (100 + (ticSquareWidth / 2.0));
		t = 0.0, xTemp = 0.0, yTemp = 0.0;
		GLfloat r = ticSquareWidth / 2.5;
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
	if (x > 100 + 2 * ticSquareWidth  && x< 100 + 3 * ticSquareWidth && y>100 && y < 100 + ticSquareWidth  && ticgame.winMachine == false && ticgame.winUser == false)
	{
		GLint i;
		GLfloat t, xTemp, yTemp;
		GLfloat x0 = (100 + 2 * ticSquareWidth + (ticSquareWidth / 2.0));
		GLfloat y0 = (100 + (ticSquareWidth / 2.0));
		t = 0.0, xTemp = 0.0, yTemp = 0.0;
		GLfloat r = ticSquareWidth / 2.5;
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
	if (x > 100 && x<(100 + ticSquareWidth) && y>100 + ticSquareWidth && y < (100 + 2 * ticSquareWidth) && ticgame.winMachine == false && ticgame.winUser == false)
	{
		GLint i;
		GLfloat t, xTemp, yTemp;
		GLfloat x0 = (100 + (ticSquareWidth / 2.0));
		GLfloat y0 = (100 + ticSquareWidth + (ticSquareWidth / 2.0));
		t = 0.0, xTemp = 0.0, yTemp = 0.0;
		GLfloat r = ticSquareWidth / 2.5;
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
	if (x > 100 + ticSquareWidth  && x< 100 + 2 * ticSquareWidth && y>100 + ticSquareWidth && y < 100 + 2 * ticSquareWidth  && ticgame.winMachine == false && ticgame.winUser == false)
	{
		GLint i;
		GLfloat t, xTemp, yTemp;
		GLfloat x0 = (100 + ticSquareWidth + (ticSquareWidth / 2.0));
		GLfloat y0 = (100 + ticSquareWidth + (ticSquareWidth / 2.0));
		t = 0.0, xTemp = 0.0, yTemp = 0.0;
		GLfloat r = ticSquareWidth / 2.5;
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
	if (x > 100 + 2 * ticSquareWidth  && x< 100 + 3 * ticSquareWidth && y>100 + ticSquareWidth && y < 100 + 2 * ticSquareWidth  && ticgame.winMachine == false && ticgame.winUser == false)
	{
		GLint i;
		GLfloat t, xTemp, yTemp;
		GLfloat x0 = (100 + 2 * ticSquareWidth + (ticSquareWidth / 2.0));
		GLfloat y0 = (100 + ticSquareWidth + (ticSquareWidth / 2.0));
		t = 0.0, xTemp = 0.0, yTemp = 0.0;
		GLfloat r = ticSquareWidth / 2.5;
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
	if (x > 100 && x<(100 + ticSquareWidth) && y>100 + 2 * ticSquareWidth && y < (100 + 3 * ticSquareWidth) && ticgame.winMachine == false && ticgame.winUser == false)
	{
		GLint i;
		GLfloat t, xTemp, yTemp;
		GLfloat x0 = (100 + (ticSquareWidth / 2.0));
		GLfloat y0 = (100 + 2 * ticSquareWidth + (ticSquareWidth / 2.0));
		t = 0.0, xTemp = 0.0, yTemp = 0.0;
		GLfloat r = ticSquareWidth / 2.5;
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
	if (x > 100 + ticSquareWidth  && x< 100 + 2 * ticSquareWidth && y>100 + 2 * ticSquareWidth && y < 100 + 3 * ticSquareWidth  && ticgame.winMachine == false && ticgame.winUser == false)
	{
		GLint i;
		GLfloat t, xTemp, yTemp;
		GLfloat x0 = (100 + ticSquareWidth + (ticSquareWidth / 2.0));
		GLfloat y0 = (100 + 2 * ticSquareWidth + (ticSquareWidth / 2.0));
		t = 0.0, xTemp = 0.0, yTemp = 0.0;
		GLfloat r = ticSquareWidth / 2.5;
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
	if (x > 100 + 2 * ticSquareWidth  && x< 100 + 3 * ticSquareWidth && y>100 + 2 * ticSquareWidth && y < 100 + 3 * ticSquareWidth  && ticgame.winMachine == false && ticgame.winUser == false)
	{
		GLint i;
		GLfloat t, xTemp, yTemp;
		GLfloat x0 = (100 + 2 * ticSquareWidth + (ticSquareWidth / 2.0));
		GLfloat y0 = (100 + 2 * ticSquareWidth + (ticSquareWidth / 2.0));
		t = 0.0, xTemp = 0.0, yTemp = 0.0;
		GLfloat r = ticSquareWidth / 2.5;
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
void displayTicTacToe(void)
{

	glClear(GL_COLOR_BUFFER_BIT);
	drawTable();
	glFlush();


}

/*callback mouse*/
void mouseTicTacToe(int btn, int mouseState, int x, int y)
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
void keyboardTicTacToe(int key, int x, int y)
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
	if (ticgame.matrix[0][0] == 'o' && ticgame.matrix[0][1] == 'o' && ticgame.matrix[0][2] == 'o')
	{
		ticgame.winMachine = true;
	}
	if (ticgame.matrix[1][0] == 'o' && ticgame.matrix[1][1] == 'o' && ticgame.matrix[1][2] == 'o')
	{
		ticgame.winMachine = true;
	}
	if (ticgame.matrix[2][0] == 'o' && ticgame.matrix[2][1] == 'o' && ticgame.matrix[2][2] == 'o')
	{
		ticgame.winMachine = true;
	}
	if (ticgame.matrix[0][0] == 'o' && ticgame.matrix[1][0] == 'o' && ticgame.matrix[2][0] == 'o')
	{
		ticgame.winMachine = true;
	}
	if (ticgame.matrix[0][1] == 'o' && ticgame.matrix[1][1] == 'o' && ticgame.matrix[2][1] == 'o')
	{
		ticgame.winMachine = true;
	}
	if (ticgame.matrix[0][2] == 'o' && ticgame.matrix[1][2] == 'o' && ticgame.matrix[2][2] == 'o')
	{
		ticgame.winMachine = true;
	}

	if (ticgame.matrix[0][0] == 'o' && ticgame.matrix[1][1] == 'o' && ticgame.matrix[2][2] == 'o')
	{
		ticgame.winMachine = true;
	}
	if (ticgame.matrix[2][0] == 'o' && ticgame.matrix[1][1] == 'o' && ticgame.matrix[0][2] == 'o')
	{
		ticgame.winMachine = true;
	}
}

/*function to check if user win*/

void checkUserWin()
{

	if (ticgame.matrix[0][0] == 'x' && ticgame.matrix[0][1] == 'x' && ticgame.matrix[0][2] == 'x')
	{
		ticgame.winUser = true;
	}
	if (ticgame.matrix[1][0] == 'x' && ticgame.matrix[1][1] == 'x' && ticgame.matrix[1][2] == 'x')
	{
		ticgame.winUser = true;

	}
	if (ticgame.matrix[2][0] == 'x' && ticgame.matrix[2][1] == 'x' && ticgame.matrix[2][2] == 'x')
	{
		ticgame.winUser = true;
	}
	if (ticgame.matrix[0][0] == 'x' && ticgame.matrix[1][0] == 'x' && ticgame.matrix[2][0] == 'x')
	{
		ticgame.winUser = true;
	}
	if (ticgame.matrix[0][1] == 'x' && ticgame.matrix[1][1] == 'x' && ticgame.matrix[2][1] == 'x')
	{
		ticgame.winUser = true;
	}
	if (ticgame.matrix[0][2] == 'x' && ticgame.matrix[1][2] == 'x' && ticgame.matrix[2][2] == 'x')
	{
		ticgame.winUser = true;
	}

	if (ticgame.matrix[0][0] == 'x' && ticgame.matrix[1][1] == 'x' && ticgame.matrix[2][2] == 'x')
	{
		ticgame.winUser = true;
	}
	if (ticgame.matrix[2][0] == 'x' && ticgame.matrix[1][1] == 'x' && ticgame.matrix[0][2] == 'x')
	{
		ticgame.winUser = true;
	}
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

	if (square1 != ticgame.matrix[0][0])
	{
		ticgame.matrix[0][0] = square1;
		if (square1 == 'o')
		{
			drawO(110, 110);
		}
	}
	if (square2 != ticgame.matrix[0][1])
	{
		ticgame.matrix[0][1] = square2;
		if (square2 == 'o')
		{
			drawO(110 + ticSquareWidth, 110);
		}
	}
	if (square3 != ticgame.matrix[0][2])
	{
		ticgame.matrix[0][2] = square3;
		if (square3 == 'o')
		{
			drawO(110 + 2 * ticSquareWidth, 110);
		}
	}
	if (square4 != ticgame.matrix[1][0])
	{
		ticgame.matrix[1][0] = square4;
		if (square4 == 'o')
		{
			drawO(110, 110 + ticSquareWidth);
		}
	}
	if (square5 != ticgame.matrix[1][1])
	{
		ticgame.matrix[1][1] = square5;
		if (square5 == 'o')
		{
			drawO(110 + ticSquareWidth, 110 + ticSquareWidth);
		}
	}
	if (square6 != ticgame.matrix[1][2])
	{
		ticgame.matrix[1][2] = square6;
		if (square6 == 'o')
		{
			drawO(110 + 2 * ticSquareWidth, 110 + ticSquareWidth);
		}
	}
	if (square7 != ticgame.matrix[2][0])
	{
		if (square7 == 'o')
		{
			drawO(110, 110 + 2 * ticSquareWidth);
		}
		ticgame.matrix[2][0] = square7;
	}
	if (square8 != ticgame.matrix[2][1])
	{
		ticgame.matrix[2][1] = square8;
		if (square8 == 'o')
		{
			drawO(110 + ticSquareWidth, 110 + 2 * ticSquareWidth);
		}
	}
	if (square9 != ticgame.matrix[2][2])
	{
		ticgame.matrix[2][2] = square9;
		if (square9 == 'o')
		{
			drawO(110 + 2 * ticSquareWidth, 110 + 2 * ticSquareWidth);
		}
	}
	checkMachineWin();
	checkUserWin();
	if (ticgame.winMachine)
	{
		writeTextOnBox(60, 550, ticStringVetor[1]);
	}
	if (ticgame.winUser)
	{
		writeTextOnBox(60, 550, ticStringVetor[0]);

	}

}
/*function to return wich user win*/
bool checkWinnerToMainGame()
{
	if (ticgame.winUser)
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
	sprintf_s(boardgame, "[[%c,%c,%c],[%c,%c,%c],[%c,%c,%c]]", ticgame.matrix[0][0], ticgame.matrix[0][1], ticgame.matrix[0][2], ticgame.matrix[1][0], ticgame.matrix[1][1], ticgame.matrix[1][2], ticgame.matrix[2][0], ticgame.matrix[2][1], ticgame.matrix[2][2]);

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

int startTicTacToe()
{
	initGameData();
	
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(TicTacToeWindowWidth, TicTacToeWindowHeight);
	glutCreateWindow("TIC-TAC-TOE");
	glutDisplayFunc(displayTicTacToe);
	glutMouseFunc(mouseTicTacToe);
	glutSpecialFunc(keyboardTicTacToe);
	ticTacToeInit();
	glutMainLoop();
	return 0;
}