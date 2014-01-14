#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <SWI-cpp.h>
#include <math.h>
#include <stdlib.h>
#include <string>
#include <stdlib.h>    
#include <GL\glut.h>
#include <iostream>
#include <fstream>
#include "Json.h"

using namespace std;

/*struct with game usefull data*/
typedef struct
{
	char wrongChar[9];
	char writeChar[25];
	bool endGame;
	bool userWin;
	int numErrors;
	string categories[20];
	string word;
	char halfWord[25];
	bool chooseCategories;
}Game;

/*global variables*/
int WindowWidth = 1000;
int WindowHeight = 600;
const float DEG2RAD = 314159 / 180;
int GLUTWindowHandle = 0;
const string help[3] = { "F1->New Game", "Choose a category and play!", "F10->Exit" };
Game game;

/*instructions to hide console when the project are executing*/
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

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
void drawHangman();
void drawRectangleWithText(GLfloat x, GLfloat y, GLfloat width, GLfloat height, string text);
void drawGameInstructions();
void IAGetCategories();
void IAGetPhrases(string category);
void IACheckIfBelongs(char c, string word);
void wsGetPhrases();
void checkWord(string word, char c);
void checkError(char c, string word);
void initHalfWord(int size);

/*function to init game data*/

void initGameData()
{
	for (int i = 0; i < 9; i++)
	{
		game.wrongChar[i] = NULL;
	}
	for (int i = 0; i < 25; i++)
	{
		game.writeChar[i] = NULL;
		game.halfWord[i] = NULL;

	}
	game.endGame = false;
	game.userWin = false;
	game.chooseCategories = false;
	game.numErrors = 0;
	for (int i = 0; i < 20; i++)
	{
		game.categories[i] = "";
	}
	game.word = "";

}
/*function to initialize char array*/
void initHalfWord(int size)
{
	for (int i = 0; i < size; i++)
	{
		if (game.word.at(i) == '_')
			game.halfWord[i] = '-';
		else
			game.halfWord[i] = '*';
	}
}
/*function when the window is resixed*/
void handleResize(int x, int y)
{
	glViewport(0, 0, x, y);
	glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	gluPerspective(45.0, (double)x / (double)y, 1.0, 1.0);
}

/*function to draw main Instructions*/
void drawGameInstructions()
{
	glColor3b(0.0, 0.0, 0.0);
	glRasterPos2d(10, 20);
	for (int i = 0; i < help[0].size(); i++)

		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, help[0].at(i));

	glRasterPos2d(10, 40);
	for (int i = 0; i < help[1].size(); i++)

		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, help[1].at(i));

	glRasterPos2d(10, 60);
	for (int i = 0; i < help[2].size(); i++)

		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, help[2].at(i));
	glFlush();
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
void drawCircle(float centerX, float centerY, float radius, int num, GLfloat lineWidth)
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
void drawLine(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat lineWidth)
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
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
}

/*function to draw char that does not belong in the word*/
void drawErrorChar()
{
	int cont = 0;
	int j = 0;
	for (int i = 0; i < 9; i++)
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
			if (i < 8 && i != 0)
				drawChar(100 + (50 * i), 100, game.wrongChar[i]);
		}
		else
		if (i >= 4 && i < 8){
			drawChar(100 + (50 * j), 130, game.wrongChar[i]);
			j++;
		}
		else
			drawChar(100, 160, game.wrongChar[i]);

	}

}
/*function to draw char lines of a word*/
void drawWord()
{

	for (int i = 0; i < 25; i++)
	{
		if (game.halfWord[i] != NULL){
			if (game.halfWord[i] == '*')
				drawLine(20 + (i * 40), 550, 40 + (i * 40), 550, 3.0);

			else{
				glColor3d(1.0, 1.0, 1.0);
				drawLine(20 + (i * 40), 550, 40 + (i * 40), 550, 3.0);
				glColor3b(0.0, 0.0, 0.0);
				drawChar(20 + (i * 40), 550, game.halfWord[i]);
			}
		}
	}
	glFlush();
}
/*function to draw hangman parts*/
void drawHangman()
{
	if (game.numErrors == 1)
	{
		drawLine(WindowWidth / 2.0 - 100, WindowHeight - 100, WindowWidth / 2.0 - 50, WindowHeight - 200, 4.0);
		drawLine(WindowWidth / 2.0 - 50, WindowHeight - 200, WindowWidth / 2.0, WindowHeight - 100, 4.0);
	}
	else
	if (game.numErrors == 2){
		drawLine(WindowWidth / 2.0 - 100, WindowHeight - 100, WindowWidth / 2.0 - 50, WindowHeight - 200, 4.0);
		drawLine(WindowWidth / 2.0 - 50, WindowHeight - 200, WindowWidth / 2.0, WindowHeight - 100, 4.0);
		drawLine(WindowWidth / 2.0 - 50, WindowHeight - 200, WindowWidth / 2.0 - 50, 100, 4.0);
	}
	else
	if (game.numErrors == 3){
		drawLine(WindowWidth / 2.0 - 100, WindowHeight - 100, WindowWidth / 2.0 - 50, WindowHeight - 200, 4.0);
		drawLine(WindowWidth / 2.0 - 50, WindowHeight - 200, WindowWidth / 2.0, WindowHeight - 100, 4.0);
		drawLine(WindowWidth / 2.0 - 50, WindowHeight - 200, WindowWidth / 2.0 - 50, 100, 4.0);
		drawLine(WindowWidth / 2.0 - 50, 100, WindowWidth - 200, 100, 4.0);
		drawLine(WindowWidth - 200, 100, WindowWidth - 200, 150, 4.0);
	}
	else
	if (game.numErrors == 4){
		drawLine(WindowWidth / 2.0 - 100, WindowHeight - 100, WindowWidth / 2.0 - 50, WindowHeight - 200, 4.0);
		drawLine(WindowWidth / 2.0 - 50, WindowHeight - 200, WindowWidth / 2.0, WindowHeight - 100, 4.0);
		drawLine(WindowWidth / 2.0 - 50, WindowHeight - 200, WindowWidth / 2.0 - 50, 100, 4.0);
		drawLine(WindowWidth / 2.0 - 50, 100, WindowWidth - 200, 100, 4.0);
		drawLine(WindowWidth - 200, 100, WindowWidth - 200, 150, 4.0);
		drawCircle(WindowWidth - 200, 180, 30, 1000, 4.0);
	}
	else
	if (game.numErrors == 5){
		drawLine(WindowWidth / 2.0 - 100, WindowHeight - 100, WindowWidth / 2.0 - 50, WindowHeight - 200, 4.0);
		drawLine(WindowWidth / 2.0 - 50, WindowHeight - 200, WindowWidth / 2.0, WindowHeight - 100, 4.0);
		drawLine(WindowWidth / 2.0 - 50, WindowHeight - 200, WindowWidth / 2.0 - 50, 100, 4.0);
		drawLine(WindowWidth / 2.0 - 50, 100, WindowWidth - 200, 100, 4.0);
		drawLine(WindowWidth - 200, 100, WindowWidth - 200, 150, 4.0);
		drawCircle(WindowWidth - 200, 180, 30, 1000, 4.0);
		drawLine(WindowWidth - 200, 210, WindowWidth - 200, 280, 4.0);
	}
	else
	if (game.numErrors == 6)
	{
		drawLine(WindowWidth / 2.0 - 100, WindowHeight - 100, WindowWidth / 2.0 - 50, WindowHeight - 200, 4.0);
		drawLine(WindowWidth / 2.0 - 50, WindowHeight - 200, WindowWidth / 2.0, WindowHeight - 100, 4.0);
		drawLine(WindowWidth / 2.0 - 50, WindowHeight - 200, WindowWidth / 2.0 - 50, 100, 4.0);
		drawLine(WindowWidth / 2.0 - 50, 100, WindowWidth - 200, 100, 4.0);
		drawLine(WindowWidth - 200, 100, WindowWidth - 200, 150, 4.0);
		drawCircle(WindowWidth - 200, 180, 30, 1000, 4.0);
		drawLine(WindowWidth - 200, 210, WindowWidth - 200, 280, 4.0);
		drawLine(WindowWidth - 200, 230, WindowWidth - 235, 260, 4.0);
	}
	else
	if (game.numErrors == 7){
		drawLine(WindowWidth / 2.0 - 100, WindowHeight - 100, WindowWidth / 2.0 - 50, WindowHeight - 200, 4.0);
		drawLine(WindowWidth / 2.0 - 50, WindowHeight - 200, WindowWidth / 2.0, WindowHeight - 100, 4.0);
		drawLine(WindowWidth / 2.0 - 50, WindowHeight - 200, WindowWidth / 2.0 - 50, 100, 4.0);
		drawLine(WindowWidth / 2.0 - 50, 100, WindowWidth - 200, 100, 4.0);
		drawLine(WindowWidth - 200, 100, WindowWidth - 200, 150, 4.0);
		drawCircle(WindowWidth - 200, 180, 30, 1000, 4.0);
		drawLine(WindowWidth - 200, 210, WindowWidth - 200, 280, 4.0);
		drawLine(WindowWidth - 200, 230, WindowWidth - 235, 260, 4.0);
		drawLine(WindowWidth - 200, 230, WindowWidth - 165, 260, 4.0);
	}
	else
	if (game.numErrors == 8){
		drawLine(WindowWidth / 2.0 - 100, WindowHeight - 100, WindowWidth / 2.0 - 50, WindowHeight - 200, 4.0);
		drawLine(WindowWidth / 2.0 - 50, WindowHeight - 200, WindowWidth / 2.0, WindowHeight - 100, 4.0);
		drawLine(WindowWidth / 2.0 - 50, WindowHeight - 200, WindowWidth / 2.0 - 50, 100, 4.0);
		drawLine(WindowWidth / 2.0 - 50, 100, WindowWidth - 200, 100, 4.0);
		drawLine(WindowWidth - 200, 100, WindowWidth - 200, 150, 4.0);
		drawCircle(WindowWidth - 200, 180, 30, 1000, 4.0);
		drawLine(WindowWidth - 200, 210, WindowWidth - 200, 280, 4.0);
		drawLine(WindowWidth - 200, 230, WindowWidth - 235, 260, 4.0);
		drawLine(WindowWidth - 200, 230, WindowWidth - 165, 260, 4.0);
		drawLine(WindowWidth - 200, 280, WindowWidth - 235, 320, 4.0);
	}
	else
	if (game.numErrors == 9)
	{
		drawLine(WindowWidth / 2.0 - 100, WindowHeight - 100, WindowWidth / 2.0 - 50, WindowHeight - 200, 4.0);
		drawLine(WindowWidth / 2.0 - 50, WindowHeight - 200, WindowWidth / 2.0, WindowHeight - 100, 4.0);
		drawLine(WindowWidth / 2.0 - 50, WindowHeight - 200, WindowWidth / 2.0 - 50, 100, 4.0);
		drawLine(WindowWidth / 2.0 - 50, 100, WindowWidth - 200, 100, 4.0);
		drawLine(WindowWidth - 200, 100, WindowWidth - 200, 150, 4.0);
		drawCircle(WindowWidth - 200, 180, 30, 1000, 4.0);
		drawLine(WindowWidth - 200, 210, WindowWidth - 200, 280, 4.0);
		drawLine(WindowWidth - 200, 230, WindowWidth - 235, 260, 4.0);
		drawLine(WindowWidth - 200, 230, WindowWidth - 165, 260, 4.0);
		drawLine(WindowWidth - 200, 280, WindowWidth - 235, 320, 4.0);
		drawLine(WindowWidth - 200, 280, WindowWidth - 165, 320, 4.0);
	}
	glFlush();

}
/*function to draw rectangle with text (button)*/
void drawRectangleWithText(GLfloat x, GLfloat y, GLfloat width, GLfloat height, string word, GLfloat red, GLfloat green, GLfloat blue)
{

	int len;
	glColor3d(red, green, blue);
	glBegin(GL_QUADS);
	glVertex2d(x, y);
	glVertex2d(x + width, y);

	glVertex2d(x + width, y + width);
	glVertex2d(x, y + height);
	glEnd();

	glColor3b(0.0, 0.0, 0.0);
	glRasterPos2f(x + 70, y + 60);
	len = word.length();
	for (int i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, word.at(i));
	}
	glutSwapBuffers();


}
void drawCompleteWord()
{
	glColor3d(0.0, 3.0, 0.0);
	for (int i = 0; i < game.word.size(); i++)
	{

		drawChar(20 + (i * 40), 520, game.word.at(i));
	}
	glFlush();
	glColor3d(0.0, 0.0, 0.0);
}
/*function to draw main Menu with all categories for the user select one of them to can play*/
void drawMainMenu()
{
	int len = 0;
	glColor3d(0.0, 0.0, 1.0);
	glBegin(GL_QUADS);
	glVertex2d(WindowWidth / 2.0 - 200, 50);
	glVertex2d(WindowWidth / 2.0 + 100, 50);
	glVertex2d(WindowWidth / 2.0 + 100, 100);
	glVertex2d(WindowWidth / 2.0 - 200, 100);
	glEnd();

	glColor3b(0.0, 0.0, 0.0);
	glRasterPos2f(WindowWidth / 2.0 - 200 + 10, 50 + 30);
	len = game.categories[0].length();
	for (int i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, game.categories[0].at(i));
	}

	len = 0;
	glColor3d(0.0, 0.0, 1.0);
	glBegin(GL_QUADS);
	glVertex2d(WindowWidth / 2.0 - 200, 120);
	glVertex2d(WindowWidth / 2.0 + 100, 120);
	glVertex2d(WindowWidth / 2.0 + 100, 170);
	glVertex2d(WindowWidth / 2.0 - 200, 170);
	glEnd();

	glColor3b(0.0, 0.0, 0.0);
	glRasterPos2f(WindowWidth / 2.0 - 200 + 10, 120 + 30);
	len = game.categories[1].length();
	for (int i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, game.categories[1].at(i));
	}


	len = 0;
	glColor3d(0.0, 0.0, 1.0);
	glBegin(GL_QUADS);
	glVertex2d(WindowWidth / 2.0 - 200, 190);
	glVertex2d(WindowWidth / 2.0 + 100, 190);
	glVertex2d(WindowWidth / 2.0 + 100, 240);
	glVertex2d(WindowWidth / 2.0 - 200, 240);
	glEnd();

	glColor3b(0.0, 0.0, 0.0);
	glRasterPos2f(WindowWidth / 2.0 - 200 + 10, 190 + 30);
	len = game.categories[2].length();
	for (int i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, game.categories[2].at(i));
	}

	len = 0;
	glColor3d(0.0, 0.0, 1.0);
	glBegin(GL_QUADS);
	glVertex2d(WindowWidth / 2.0 - 200, 260);
	glVertex2d(WindowWidth / 2.0 + 100, 260);
	glVertex2d(WindowWidth / 2.0 + 100, 310);
	glVertex2d(WindowWidth / 2.0 - 200, 310);
	glEnd();

	glColor3b(0.0, 0.0, 0.0);
	glRasterPos2f(WindowWidth / 2.0 - 200 + 10, 260 + 30);
	len = game.categories[3].length();
	for (int i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, game.categories[3].at(i));
	}


	len = 0;
	glColor3d(0.0, 0.0, 1.0);
	glBegin(GL_QUADS);
	glVertex2d(WindowWidth / 2.0 - 200, 330);
	glVertex2d(WindowWidth / 2.0 + 100, 330);
	glVertex2d(WindowWidth / 2.0 + 100, 380);
	glVertex2d(WindowWidth / 2.0 - 200, 380);
	glEnd();

	glColor3b(0.0, 0.0, 0.0);
	glRasterPos2f(WindowWidth / 2.0 - 200 + 10, 330 + 30);
	len = game.categories[4].length();
	for (int i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, game.categories[4].at(i));
	}
	glFlush();


}
/*function to draw the main scene*/
void drawScene()
{
	//glColor3b(1.0, 0.0, 0.0);
	drawGameInstructions();
	if (game.chooseCategories == false){
		IAGetCategories();
		drawMainMenu();
	}
	glFlush();

}

/*callback mouse*/
void mouse(int btn, int mouseState, int x, int y)
{
	switch (btn){
	case GLUT_LEFT_BUTTON:

		if (mouseState == GLUT_DOWN)
		{
			if ((x > WindowWidth / 2.0 - 200) && (x<WindowWidth / 2.0 + 100) && y>50 && y < 100 && game.chooseCategories == false)
			{
				glClear(GL_COLOR_BUFFER_BIT);
				IAGetPhrases(game.categories[0]);
				game.chooseCategories = true;
			}
			if ((x > WindowWidth / 2.0 - 200) && (x<WindowWidth / 2.0 + 100) && y>120 && y < 170 && game.chooseCategories == false)
			{
				glClear(GL_COLOR_BUFFER_BIT);
				IAGetPhrases(game.categories[1]);
				game.chooseCategories = true;

			}
			if ((x > WindowWidth / 2.0 - 200) && (x<WindowWidth / 2.0 + 100) && y>190 && y < 240 && game.chooseCategories == false)
			{
				glClear(GL_COLOR_BUFFER_BIT);
				IAGetPhrases(game.categories[2]);
				game.chooseCategories = true;

			}
			if ((x > WindowWidth / 2.0 - 200) && (x<WindowWidth / 2.0 + 100) && y>260 && y < 310 && game.chooseCategories == false)
			{
				glClear(GL_COLOR_BUFFER_BIT);
				IAGetPhrases(game.categories[3]);
				game.chooseCategories = true;

			}
			if ((x > WindowWidth / 2.0 - 200) && (x<WindowWidth / 2.0 + 100) && y>330 && y < 380 && game.chooseCategories == false)
			{
				glClear(GL_COLOR_BUFFER_BIT);
				wsGetPhrases();
				game.chooseCategories = true;
			}

		}
	}
}
/*special keyboard function*/
void keyboardSpecial(int key, int x, int y)
{
	switch (key){

	case GLUT_KEY_F1:
		glutPostRedisplay();
		initGameData();
		initHalfWord(game.word.size());
		break;
	case GLUT_KEY_F10:
		if (game.userWin)
			exit(5);
		else
			exit(6);
		break;

	}
}
/*function to check if that char c are already on the list*/
bool checkIfWrong(char c)
{
	for (int i = 0; i < 9; i++)
	{
		if (c == game.wrongChar[i])
			return true;
	}
	return false;
}
/*callback keyboard*/
void keyBoard(unsigned char key, int x, int y)
{
	if (((int)key>96 && (int)key < 123) && game.word != "" && game.numErrors <9 && game.endGame == false){
		if (!checkIfWrong(key))
			IACheckIfBelongs(key, game.word);
	}
	else
	if ((int)key > 64 && (int)key < 91 && game.word != "" && game.numErrors < 9 && game.endGame == false)
	{

		if (!checkIfWrong((int)key + 32))
			IACheckIfBelongs((int)key + 32, game.word);
	}
}

/*function to call webservice and get phrases from the category user_added*/
void wsGetPhrases()
{
	utility::string_t url = L"http://uvm061.dei.isep.ipp.pt:5000/get_user_added";

	json::value obj = RequestJSONValueAsync(url).get();


	wstring words = obj[L"details"].to_string();
	int size = words.size();
	char c_mb_str[80];
	wcstombs(c_mb_str, &words[1], 80);
	string s(c_mb_str);
	game.word = s.substr(0, size - 2);
	initHalfWord(game.word.size());
	drawWord();

}
/*function to connect with prolog file and get alll categories on knowledge basis*/
void IAGetCategories()
{
	int i = 0;
	char *plargv[] = { "swipl.dll", "-s", "h-off.pl", NULL };
	PlEngine e(3, plargv);
	PlTermv av(1);
	PlQuery q("category", av);
	try{
		while (q.next_solution()){
			game.categories[i] = (string)av[0];
			i++;
		}
	}
	catch (PlException &ex){
		cout << (char *)ex << endl;
	}

}

/*function to connect with prolog and get all phrases of that category*/
void IAGetPhrases(string category)
{

	char *plargv[] = { "swipl.dll", "-s", "h-off.pl", NULL };
	PlEngine e(3, plargv);
	PlTermv av(2);
	char* cs = new char[category.size() + 1];
	strcpy_s(cs, category.size() + 1, category.c_str());
	av[0] = PlTerm(cs);
	PlQuery q("getPhrase", av);
	try{
		while (q.next_solution()){
			game.word = (string)av[1];
			initHalfWord(game.word.size());
		}
	}
	catch (PlException &ex){
		cout << (char *)ex << endl;
	}
	drawWord();

}

/*function to connect with prolog that will test if that char c belongs to the word*/
void IACheckIfBelongs(char c, string word)
{
	char *plargv[] = { "swipl.dll", "-s", "h-off.pl", NULL };
	PlEngine e(3, plargv);
	PlTermv av(3);
	char *ct = &c;
	char* cs = new char[word.size() + 1];
	strcpy_s(cs, word.size() + 1, word.c_str());
	av[0] = PlTerm(ct);
	av[1] = PlTerm(cs);
	PlQuery q("checkIfBelongs", av);
	try{
		while (q.next_solution()){

			checkWord((string)av[2], c);
			checkError(c, (string)av[2]);

		}
	}
	catch (PlException &ex){
		cout << (char *)ex << endl;
	}
}

/*function to read the word returned by the prolog and compare with the last return*/
void checkError(char c, string word)
{
	if (!checkIfWrong(c)){
		int cont = 0;
		for (int i = 0; i < word.size(); i++)
		{
			if (word.at(i) != '*' && word.at(i) != '_')
			{
				cont++;
			}
		}
		if (cont == 0)
		{
			game.numErrors++;
			game.wrongChar[game.numErrors - 1] = c;
			drawHangman();
			drawErrorChar();
			glFlush();
			if (game.numErrors == 9){
				game.endGame = true;
				drawRectangleWithText(WindowWidth - 200, WindowHeight - 100, 200, 100, "Lost!", 1.0, 0.0, 0.0);
				drawCompleteWord();
			}
		}

	}
}
/*function to check when the user win*/
bool checkWin()
{
	bool check = true;
	for (int i = 0; i < game.word.size(); i++)
	{
		if (game.halfWord[i] == '*')
			check = false;

	}
	return check;
}
/*function to read the word returned by the prolog*/
void checkWord(string word, char c)
{
	for (int i = 0; i < word.size(); i++)
	{
		if (game.halfWord[i] == '*' && word.at(i) != '*')
		{
			game.halfWord[i] = word.at(i);
		}
	}
	checkError(c, word);
	if (checkWin())
	{
		game.userWin = true;
		game.endGame = true;
		drawRectangleWithText(WindowWidth - 200, WindowHeight - 100, 200, 100, "Win!", 0.0, 1.0, 0.0);
	}

	drawWord();
}
int main(int argc, char**argv)
{
	initGameData();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutCreateWindow("HANGMAN");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyBoard);
	glutMouseFunc(mouse);
	glutSpecialFunc(keyboardSpecial);
	myInit();
	glutMainLoop();
	return 0;
}