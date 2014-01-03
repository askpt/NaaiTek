#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <stdlib.h>     
#include <GL\glut.h>
#include <SWI-cpp.h>
#include <iostream>


#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

using namespace std;

typedef struct
{
	int matrix[3][3];
	int win;
	int loose;
}Game;



const float DEG2RAD = 3.14159 / 180;
int WindowWidth = 600;
int WindowHeight = 600;


int GLUTWindowHandle = 0;

float squareWidth = WindowWidth / 5.0;

Game game;


/**function to init the game parametrers**/
void initGameData()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3;j++)
		{
			game.matrix[i][j] = 0;
			
		}
	}
}

/*function to call prolog and send border game*/

void requestAnswer()
{

}
/*function to initialize OpenGL*/

void myInit()
{
	initGameData();
	glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glShadeModel(GL_FLAT);

	glViewport(0, 0, WindowWidth, WindowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0.0, (GLdouble)WindowWidth, (GLdouble)WindowHeight, 0.0);
	glMatrixMode(GL_MODELVIEW);
	


}

/*this function draw 9 squares forming one big square
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
*/

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

	glColor3b(1.0, 0.0, 0.0);
	glLineWidth(4.0);
	glBegin(GL_LINES);

	glVertex2d(100 + squareWidth, 100);
	glVertex2d(100 + 2 * squareWidth, 100);

	glVertex2d(100 + 2 * squareWidth, 100 + squareWidth);
	glVertex2d(100 + squareWidth, 100 + squareWidth);

	glVertex2d(100 + 2 * squareWidth, 100);
	glVertex2d(100 + 2 * squareWidth, 100 + squareWidth);

	glEnd();

	glColor3b(1.0, 0.0, 0.0);
	glLineWidth(4.0);
	glBegin(GL_LINES);

	glVertex2d(100 + 2 * squareWidth, 100);
	glVertex2d(100 + 3 * squareWidth, 100);

	glVertex2d(100 + 2 * squareWidth, 100 + squareWidth);
	glVertex2d(100 + 3 * squareWidth, 100 + squareWidth);

	glVertex2d(100 + 3 * squareWidth, 100);
	glVertex2d(100 + 3 * squareWidth, 100 + squareWidth);

	glEnd();

	glColor3b(1.0, 0.0f, 0.0f);
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

	glColor3b(1.0, 0.0, 0.0);
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

}

/*function called by the mouse callback to draw X's on the specific squad*/
void drawX(int x, int y)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3b(1.0, 0.0, 0.0);

	glLineWidth(4.0);

	//square1
	if (x > 100 && x<(100 + squareWidth) && y>100 && y < (100 + squareWidth))
	{

		glBegin(GL_LINES);
		glVertex2d(100, 100);
		glVertex2d(100 + squareWidth, 100 + squareWidth);

		glVertex2d(100 + squareWidth, 100);
		glVertex2d(100, 100 + squareWidth);

		glEnd();
		glFlush();
	}
	else

	//square2
	if (x > 100 + squareWidth  && x< 100 + 2 * squareWidth && y>100 && y < 100 + squareWidth)
	{
		glBegin(GL_LINES);
		glVertex2d(100 + squareWidth, 100);
		glVertex2d(100 + 2 * squareWidth, 100 + squareWidth);

		glVertex2d(100 + 2 * squareWidth, 100);
		glVertex2d(100 + squareWidth, 100 + squareWidth);

		glEnd();
		glFlush();
	}
	else
		//square3
	if (x > 100 + 2 * squareWidth  && x< 100 + 3 * squareWidth && y>100 && y < 100 + squareWidth)
	{
		glBegin(GL_LINES);
		glVertex2d(100 + 2 * squareWidth, 100);
		glVertex2d(100 + 3 * squareWidth, 100 + squareWidth);

		glVertex2d(100 + 3 * squareWidth, 100);
		glVertex2d(100 + 2 * squareWidth, 100 + squareWidth);

		glEnd();
		glFlush();
	}
	else
		//square4
	if (x > 100 && x<(100 + squareWidth) && y>100 + squareWidth && y < (100 + 2 * squareWidth))
	{
		glBegin(GL_LINES);
		glVertex2d(100, 100 + squareWidth);
		glVertex2d(100 + squareWidth, 100 + 2 * squareWidth);

		glVertex2d(100 + squareWidth, 100 + squareWidth);
		glVertex2d(100, 100 + 2 * squareWidth);

		glEnd();
		glFlush();
	}

	else
		//square5
	if (x > 100 + squareWidth  && x< 100 + 2 * squareWidth && y>100 + squareWidth && y < 100 + 2 * squareWidth)
	{
		glBegin(GL_LINES);
		glVertex2d(100 + squareWidth, 100 + squareWidth);
		glVertex2d(100 + 2 * squareWidth, 100 + 2 * squareWidth);

		glVertex2d(100 + 2 * squareWidth, 100 + squareWidth);
		glVertex2d(100 + squareWidth, 100 + 2 * squareWidth);


		glEnd();
		glFlush();
	}
	else
		//square6
	if (x > 100 + 2 * squareWidth  && x< 100 + 3 * squareWidth && y>100 + squareWidth && y < 100 + 2 * squareWidth)
	{
		glBegin(GL_LINES);
		glVertex2d(100 + 2 * squareWidth, 100 + squareWidth);
		glVertex2d(100 + 3 * squareWidth, 100 + 2 * squareWidth);

		glVertex2d(100 + 3 * squareWidth, 100 + squareWidth);
		glVertex2d(100 + 2 * squareWidth, 100 + 2 * squareWidth);

		glEnd();
		glFlush();
	}
	else
		//square7
	if (x > 100 && x<(100 + squareWidth) && y>100 + 2 * squareWidth && y < (100 + 3 * squareWidth))
	{

		glBegin(GL_LINES);
		glVertex2d(100, 100 + 2 * squareWidth);
		glVertex2d(100 + squareWidth, 100 + 3 * squareWidth);

		glVertex2d(100 + squareWidth, 100 + 2 * squareWidth);
		glVertex2d(100, 100 + 3 * squareWidth);

		glEnd();
		glFlush();
	}
	else
		//square8
	if (x > 100 + squareWidth  && x< 100 + 2 * squareWidth && y>100 + 2 * squareWidth && y < 100 + 3 * squareWidth)
	{
		glBegin(GL_LINES);
		glVertex2d(100 + squareWidth, 100 + 2 * squareWidth);
		glVertex2d(100 + 2 * squareWidth, 100 + 3 * squareWidth);

		glVertex2d(100 + 2 * squareWidth, 100 + 2 * squareWidth);
		glVertex2d(100 + squareWidth, 100 + 3 * squareWidth);

		glEnd();
		glFlush();
	}
	else
		//square9
	if (x > 100 + 2 * squareWidth  && x< 100 + 3 * squareWidth && y>100 + 2 * squareWidth && y < 100 + 3 * squareWidth)
	{
		glBegin(GL_LINES);
		glVertex2d(100 + 2 * squareWidth, 100 + 2 * squareWidth);
		glVertex2d(100 + 3 * squareWidth, 100 + 3 * squareWidth);

		glVertex2d(100 + 3 * squareWidth, 100 + 2 * squareWidth);
		glVertex2d(100 + 2 * squareWidth, 100 + 3 * squareWidth);

		glEnd();
		glFlush();
	}

}

/*function called by the mouse callback to draw X's on the specific squad*/
void drawO(int x, int y)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3b(1.0, 0.0, 0.0);


	//square1
	if (x > 100 && x<(100 + squareWidth) && y>100 && y < (100 + squareWidth))
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
	if (x > 100 + squareWidth  && x< 100 + 2 * squareWidth && y>100 && y < 100 + squareWidth)
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
	if (x > 100 + 2 * squareWidth  && x< 100 + 3 * squareWidth && y>100 && y < 100 + squareWidth)
	{
		GLint i;
		GLfloat t, xTemp, yTemp;
		GLfloat x0 = (100 + 2*squareWidth + (squareWidth / 2.0));
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
	if (x > 100 && x<(100 + squareWidth) && y>100 + squareWidth && y < (100 + 2 * squareWidth))
	{
		GLint i;
		GLfloat t, xTemp, yTemp;
		GLfloat x0 = (100+(squareWidth/2.0));
		GLfloat y0 = (100 + squareWidth+(squareWidth / 2.0));
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
	if (x > 100 + squareWidth  && x< 100 + 2 * squareWidth && y>100 + squareWidth && y < 100 + 2 * squareWidth)
	{
		GLint i;
		GLfloat t, xTemp, yTemp;
		GLfloat x0 = (100 + squareWidth+(squareWidth / 2.0));
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
	if (x > 100 + 2 * squareWidth  && x< 100 + 3 * squareWidth && y>100 + squareWidth && y < 100 + 2 * squareWidth)
	{
		GLint i;
		GLfloat t, xTemp, yTemp;
		GLfloat x0 = (100 + 2*squareWidth + (squareWidth / 2.0));
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
	if (x > 100 && x<(100 + squareWidth) && y>100 + 2 * squareWidth && y < (100 + 3 * squareWidth))
	{
		GLint i;
		GLfloat t, xTemp, yTemp;
		GLfloat x0 = (100 + (squareWidth / 2.0));
		GLfloat y0 = (100 + 2*squareWidth + (squareWidth / 2.0));
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
	if (x > 100 + squareWidth  && x< 100 + 2 * squareWidth && y>100 + 2 * squareWidth && y < 100 + 3 * squareWidth)
	{
		GLint i;
		GLfloat t, xTemp, yTemp;
		GLfloat x0 = (100 + squareWidth+ (squareWidth / 2.0));
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
	if (x > 100 + 2 * squareWidth  && x< 100 + 3 * squareWidth && y>100 + 2 * squareWidth && y < 100 + 3 * squareWidth)
	{
		GLint i;
		GLfloat t, xTemp, yTemp;
		GLfloat x0 = (100 + 2*squareWidth+(squareWidth / 2.0));
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

/*callback mouse*/
void mouse(int btn, int mouseState, int x, int y)
{
	switch (btn){
	case GLUT_LEFT_BUTTON:

		if (mouseState == GLUT_DOWN)
		{
			//drawX(x, y);
			drawO(x, y);
		}
	}
}

/*this method will be called every time that the user select a square*/
void IAConnection()
{
	char *plargv[] = { "swipl.dll", "-s", "ttt-off.pl", NULL };
	PlEngine e(3, plargv);
	PlTermv av(3);
	av[1] = PlCompound("['1','2','3'],['4','5','6'],['7','8','9']");
	//av[2] = PlCompound("2");
	//av[3] = PlCompound("L");
	PlQuery q("receive", av);
	while (q.next_solution())
	{
		cout << (char*)av[0] << endl;
	}
	cin.get();
}

int main(int argc, char **argv)
{
	
	IAConnection();
	
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(WindowWidth, WindowHeight); 
	glutCreateWindow("TIC-TAC-TOE");
	//glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	//glutKeyboardFunc(keyboard);
	//glutSpecialFunc(Special);
	glutMouseFunc(mouse);

	myInit();
	glutMainLoop();
	
	
	
	return 0;
}