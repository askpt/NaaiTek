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

/*struct with game usefull data*/
typedef struct
{
	char wrongChar[8];
	char writeChar[25];
	bool endGame;
	bool userWin;
	int numErrors;
}Game;

/*global variables*/
int WindowWidth = 800;
int WindowHeight = 600;
const float DEG2RAD = 314159 / 180;
int GLUTWindowHandle = 0;
Game game;

/*instructions to hide console when the project are executing*/
//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

/*initial declaration of some importante functions*/
void drawScene();
void display();
void resizeWindow(int x, int y);
void mouse(int btn, int mouseState, int x, int y);
void keyboard(int key, int x, int y);
void drawCircle(float centerX, float centerY, float radius, int num, GLfloat lineWidth);
void drawLine(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat lineWidth);
void drawChar(int x, int y, char c);
void initGameData();
void drawErrorChar();
void drawWord(string word);


/*function to init game data*/

void initGameData()
{
	for (int i = 0; i < 8; i++)
	{
		game.wrongChar[i] = NULL;
	}
	for (int i = 0; i < 25; i++)
	{
		game.writeChar[i] = NULL;
	}
	game.endGame = false;
	game.userWin = false;
	game.numErrors = 0;
}

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
void drawCircle(float centerX, float centerY, float radius, int num,GLfloat lineWidth)
{
	glLineWidth(lineWidth);
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
void drawLine(GLfloat x1,GLfloat y1, GLfloat x2, GLfloat y2, GLfloat lineWidth)
{
	glLineWidth(lineWidth);
	glBegin(GL_LINES);

	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
}

/*function to draw a character c into a specific position (x,y)*/
void drawChar(int x, int y, char c)
{
	glRasterPos2d(x, y);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
}

/*function to draw char that does not belong in the word*/
void drawErrorChar()
{
	int cont = 0;
	for (int i = 0; i < 8; i++)
	{
		if (game.wrongChar[i] != NULL)
		{
			cont++;
		}
	}
	for (int i = 0; i < cont; i++)
	{
		if (i < 4)
		{
			if (i == 0)
			{
				drawChar(100, 100, game.wrongChar[i]);
			}
			else
				drawChar(100 + (50 * i), 100, game.wrongChar[i]);
		}
		else
		for (int j = 0; j < 4;j++){
			drawChar(100 + (50 * j), 130, game.wrongChar[i]);
		}
	}

}
/*function to draw char lines of a word*/
void drawWord(string word)
{
	int wordSize = word.length();
	for (int i = 0; i < wordSize; i++)
	{
		drawLine(20+(i*60), 550, 60+(i*60), 550, 3.0);
	}
}
/*function to draw the main scene*/
void drawScene()
{
	glColor3b(1.0, 0.0, 0.0);
	//drawLine(400, 500, 450, 400,6.0);
	for (int i = 0; i<8; i++)
	{
		game.wrongChar[i] = 'A';
	}
	drawErrorChar();
	drawWord("arroz");
}



int main(int argc, char**argv)
{
	initGameData();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutCreateWindow("HANGMAN");
	glutDisplayFunc(display);
	myInit();
	glutMainLoop();
	return 0; 
}









