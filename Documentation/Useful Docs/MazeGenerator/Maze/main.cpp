//
//  main.cpp
//  Maze
//
//  Created by João Carreira on 06/01/14.
//  Copyright (c) 2014 João Carreira. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include <vector>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "text3d.h"
#include "Maze.h"

using namespace std;


//************************************************************************
// global variables
//************************************************************************

// windows
int mainWindow;

// rotation angle
float g_angle = -30.0f;
float g_cameraAngle = 0.0f;

// constante z-distance
float g_zDistance = -5.0f;

// square size (used in maze's wall drawing)
float g_wallSize;
int mazeSizeHorizontal;
int mazeSizeVertical;

float g_positionXToTopScreen = -2.0;
float g_positionYToTopScreen = 1.7;

/*
float g_positionXToTopScreenRight = 1.7;
float g_positionYToTopScreenRight = 1.5;
*/

// translation factor (used in maze's wall and path drawing)
float g_translationFactorOnHorizontalAxis;
float g_translationFactorOnVerticalAxis;

// textures
GLuint g_textureId;
GLuint g_pathTexture;

// scale for drawing text
float g_scale;

// data structure for maze
vector < vector < int > > maze;

// struct the coordinates of both the player and goal in the maze
struct position
{
    int x;
    int y;
}
player, target;


// variables to follow game outcome
bool playerDidWin;
bool playerDidQuit;
int totalHelpRequest;
bool didRequestHelp;


// variables to draw menu
float topTranslationFactorHorizontal;
float topTranslationFactorVertical;
float bottomTranslationFactorVertical;
char *menuOptions[4] = { "Get Help (h)", "Quit (q)", "", "" };
char *systemMsg[4] = { "Get to the target!", "Confirm quit? (Y/N)", "You Lost! (press C)", "You Won! (press C)" };
char *instructionMsg[4] = {"P (up)", "L (down)", "Z (left)", "X (right)" };
int msgToDisplay = 0;


//************************************************************************
// forward declarations of OpenGL callback functions
//************************************************************************
void handleKeypress(unsigned char key, int x, int y);
void handleMouseButtonClicks(int button, int state, int x, int y);
void initRendering();
void handleResize(int w, int h);
void drawScene();
void drawMaze();
void drawMazeBootStrap();
float computeScaleForSquareSize(const char* strs[], int numberOfStrings, float squareSize);
void cleanUp();
vector<vector<int>> mazeBuilder(int sizeX, int sizeY);
void drawWallAtScreenPosition(float x, float y);
void drawPathAtScreenPosition(float x, float y);
void drawHelpAtScreenPosition(float x, float y);
void drawSquareAtScreenPositionWithColor(float x, float y, float color[]);
void resetHorizontalTranslationFactor();
void updateHorizontalTranslationFactor();
void resetVerticalTranlationFactor();
void updateVerticalTranslationFactor();
void setPositionToLeftScreen();
void drawPlayerAtPosition(int x, int y);
void setPlayerPosition(int x, int y);
void setTargetPosition(int x, int y);
void drawPlayerAtScreenPosition(float x, float y);
void drawTargetAtScreenPosition(float x, float y);
bool isPossibleToMovePlayerToPosition(int x, int y);
int getFirstPossibleX();
int getFirstPossibleY();
int getLastPossibleX();
int getLastPossibleY();
void resetGameSettings();
void checkIfPlayerWon();
void requestHelp();
void clearHelp();
void drawMenu();
void drawLabelAtScreenPositionWithTextAndColor(float x, float y, char *str, float color[]);
float computeScaleForSquareSize(char* strs[], int numberOfStrings, float squareSize);
void setupAmbientLight();
void setupPositionedLight();
void drawSystemMessage(int i);
void drawInstructionMessages();


/**
 * main
 *
 * @return: 0 (player quit), 1 (player won), 2 (error building maze)
 */
int main(int argc, char *argv[])
{
    // dificulty level (assigned randomly)
    srand(time(0));
    int dificultyLevel = rand() % 3 + 1;
    // "easy" mode
    if(dificultyLevel == 1)
    {
        g_wallSize = 0.075;
        mazeSizeHorizontal = 20;
        mazeSizeVertical = 20;
    }
    // "moderate" mode
    else if(dificultyLevel == 2)
    {
        g_wallSize = 0.045;
        mazeSizeHorizontal = 40;
        mazeSizeVertical = 40;
    }
    // "hard" mode
    else if(dificultyLevel == 3)
    {
        g_wallSize = 0.020;
        mazeSizeHorizontal = 80;
        mazeSizeVertical = 80;
    }
    
	// initializing GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
	// window settings
    glutInitWindowSize(1200, 800);
	mainWindow = glutCreateWindow("Maze");
    initRendering();
    
    g_scale = computeScaleForSquareSize(menuOptions, 3, 3.0);
    
    // building a random maze
    maze = mazeBuilder(mazeSizeHorizontal, mazeSizeVertical);
    
    // defining player's and target's positions
    setPlayerPosition(getFirstPossibleX(), getFirstPossibleY());
    setTargetPosition(getLastPossibleX(), getLastPossibleY());
    
    // reseting game settings
    resetGameSettings();
    
	// setting handler functions
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
    glutMouseFunc(handleMouseButtonClicks);
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
            playerDidQuit = true;
			exit(0);
            break;
        
        // UP
        case 'p':
        case'P':
            if(didRequestHelp)
            {
                clearHelp();
            }
            if(isPossibleToMovePlayerToPosition(player.x - 1, player.y))
            {
                setPlayerPosition(player.x - 1, player.y);
                drawScene();
            }
            break;
            
        // DOWN
        case 'l':
        case 'L':
            if(didRequestHelp)
            {
                clearHelp();
            }
            if(isPossibleToMovePlayerToPosition(player.x + 1, player.y))
            {
                setPlayerPosition(player.x + 1, player.y);
                drawScene();
            }
            break;
            
        // LEFT
        case 'z':
        case 'Z':
            if(didRequestHelp)
            {
                clearHelp();
            }
            if(isPossibleToMovePlayerToPosition(player.x, player.y - 1))
            {
                setPlayerPosition(player.x, player.y - 1);
                drawScene();
            }
            break;
            
        // RIGHT
        case 'x':
        case 'X':
            if(didRequestHelp)
            {
                clearHelp();
            }
            if(isPossibleToMovePlayerToPosition(player.x, player.y + 1))
            {
                setPlayerPosition(player.x, player.y + 1);
                drawScene();
            }
        break;
            
        // requesting help
        case 'h':
        case 'H':
            requestHelp();
            drawScene();
            break;
            
        // requesting quit
        case 'q':
        case 'Q':
            msgToDisplay = 1;
            drawScene();
            break;
            
        // option NO when prompted to quit
        case 'n':
        case 'N':
            if(msgToDisplay == 1)
            {
                msgToDisplay = 0;
                drawScene();
            }
            break;
        
        // option YES when prompted to quit
        case 'y':
        case 'Y':
            if(msgToDisplay == 1)
            {
                playerDidQuit = true;
                msgToDisplay = 2;
                drawScene();
            }
            break;
            
        // c to exit when player wins or quits
        case 'c':
        case 'C':
            if(msgToDisplay == 3)
            {
                exit(1);
            }
            if(msgToDisplay == 2)
            {
                exit(0);
            }
    }
}


/**
 * This funcion is invoked when the user presses a mouse button on the current window.
 * Each press and release action will generate an event.
 * int button will be one of GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON.
 * int state will be either GLUT_DOWN, or GLUT_UP indicating whether the event was a pressed, or released event.
 * int x, int y store the position of the mouse relative to the current window.
 */
void handleMouseButtonClicks(int button, int state, int x, int y)
{
    cout << "Button: " << button << endl;
    cout << "State:" << state << endl;
    cout << "X = " << x << endl;
    cout << "Y = " << y << endl;
}


// clears "help path" on the screen
void clearHelp()
{
    for(int i = 0; i < mazeSizeHorizontal; i++)
    {
        for(int j = 0; j < mazeSizeVertical; j++)
        {
            if(maze[i][j] == 2)
            {
                maze[i][j] = 1;
            }
        }
    }
    didRequestHelp = false;
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
    
    t3dInit();
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


void resetGameSettings()
{
    playerDidWin = false;
    playerDidQuit = false;
    totalHelpRequest = 0;
    didRequestHelp = false;
}


/**
 * draws a scene
 */
void drawScene()
{
    // standard stuff
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
    
    // enabling lighting
    setupAmbientLight();
    setupPositionedLight();
    
    // draws maze
    drawMaze();
    
    // draws instructions msg
    drawInstructionMessages();
    
    // draws menu
    drawMenu();
    
    // draws standard system message
    drawSystemMessage(msgToDisplay);
    
	glutSwapBuffers();
}


/**
 * draws the maze based on the matrix where 0 is a wall, 1 is a path and 2 is a help request
 */
void drawMaze()
{
    glLoadIdentity();
	glTranslatef(0.0f, 0.0f, g_zDistance);
    
    // this is needed before starting the iteration through
    resetHorizontalTranslationFactor();
    resetVerticalTranlationFactor();
    
    // iterates through the entire matrix
    for(int i = 0; i < mazeSizeHorizontal; i++)
    {
        for(int j = 0; j < mazeSizeVertical; j++)
        {
            float xPositionOnScreen = g_wallSize * g_translationFactorOnHorizontalAxis;
            float yPositionOnScreen = g_wallSize * g_translationFactorOnVerticalAxis;
            
            // checking if the player is in this position
            if(player.x == i && player.y == j)
            {
                drawPlayerAtScreenPosition(xPositionOnScreen, yPositionOnScreen);
            }
            
            // checking if the target is in this position
            if(target.x == i && target.y == j)
            {
                drawTargetAtScreenPosition(xPositionOnScreen, yPositionOnScreen);
            }
            
            // draw normal path if none of the above is in this position
            if(maze[i][j] == 1)
            {
                drawPathAtScreenPosition(xPositionOnScreen, yPositionOnScreen);
                
            }
            
            // draw help
            if(maze[i][j] == 2)
            {
                drawHelpAtScreenPosition(xPositionOnScreen, yPositionOnScreen);
            }
            else
            {
                drawWallAtScreenPosition(g_wallSize * g_translationFactorOnHorizontalAxis, g_wallSize * g_translationFactorOnVerticalAxis);
            }
            updateHorizontalTranslationFactor();
            
            // saving menu top limit (only once, at the end of drawing the first row)
            if(i == 0 && j == mazeSizeVertical - 1)
            {
                topTranslationFactorHorizontal = g_translationFactorOnHorizontalAxis + 12;
                topTranslationFactorVertical = g_translationFactorOnVerticalAxis - 4;
            }
            
            // saving menu bottom limit
            if(i == mazeSizeHorizontal - 1 && j == mazeSizeVertical - 1)
            {
                bottomTranslationFactorVertical = g_translationFactorOnVerticalAxis;
            }
        }
        resetHorizontalTranslationFactor();
        updateVerticalTranslationFactor();
    }
    
    // after each draw we need to check if game has ended
    checkIfPlayerWon();
}


/**
 * draws a help path
 */
void requestHelp()
{
    // bootstrap help (to be removed)
    maze[1][1] = 2;
    maze[2][2] = 2;
    maze[3][3] = 2;
    maze[4][4] = 2;
    
    didRequestHelp = true;
    totalHelpRequest++;
}


/**
 * sets player's position in the matrix maze
 */
void setPlayerPosition(int x, int y)
{
    player.x = x;
    player.y = y;
}


/**
 * sets target's position in the matrix maze
 */
void setTargetPosition(int x, int y)
{
    target.x = x;
    target.y = y;
}


/**
 * checks if it's possible to move to a given maze position
 */
bool isPossibleToMovePlayerToPosition(int x, int y)
{
    // player can't move beyond maze's limits
    if(x < 0 || y < 0)
    {
        return false;
    }
    
    // player can't move if there's a wall
    if(maze[x][y] == 0)
    {
        return false;
    }

    return true;
}


/**
 * sets the X coordinate for the player when the maze is loaded for the first time
 */
int getFirstPossibleX()
{
    for(int i = 0; i < mazeSizeHorizontal; i++)
    {
        for(int j = 0; j < mazeSizeVertical; j++)
        {
            if(maze[i][j] == 1)
            {
                return i;
            }
        }
    }
    exit(2);
}


/**
 * sets the Y coordinate for the player when the maze is loaded for the first time
 */
int getFirstPossibleY()
{
    for(int i = 0; i < mazeSizeHorizontal - 1; i++)
    {
        for(int j = 0; j < mazeSizeVertical - 1; j++)
        {
            if(maze[i][j] == 1)
            {
                return j;
            }
        }
    }
    exit(2);
}


/**
 * sets the X coordinate for the target when the maze is loaded for the first time
 */
int getLastPossibleX()
{
    for(int i = mazeSizeHorizontal - 1; i > 0; i--)
    {
        for(int j = mazeSizeVertical - 1; j > 0; j--)
        {
            if(maze[i][j] == 1)
            {
                return i;
            }
        }
    }
    exit(2);
}


/**
 * sets the Y coordinate for the player when the maze is loaded for the first time
 */
int getLastPossibleY()
{
    for(int i = mazeSizeHorizontal - 1; i > 0; i--)
    {
        for(int j = mazeSizeVertical - 1; j > 0; j--)
        {
            if(maze[i][j] == 1)
            {
                return j;
            }
        }
    }
    exit(2);
}


/**
 * player wins the game when he reaches the target
 */
void checkIfPlayerWon()
{
    if(player.x == target.x && player.y == target.y)
    {
        playerDidWin = true;
        msgToDisplay = 3;
    }
}


/**
 * bootstrap maze, for tests only
 */
void drawMazeBootStrap()
{
    // first wall block
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, g_zDistance);
    
    resetHorizontalTranslationFactor();
    drawWallAtScreenPosition(g_wallSize * g_translationFactorOnHorizontalAxis, g_wallSize * g_translationFactorOnVerticalAxis);
    
    // second wall block
    updateHorizontalTranslationFactor();
    drawWallAtScreenPosition(g_wallSize * g_translationFactorOnHorizontalAxis, g_wallSize * g_translationFactorOnVerticalAxis);
    
    // third wall block
    updateHorizontalTranslationFactor();
    drawPathAtScreenPosition(g_wallSize * g_translationFactorOnHorizontalAxis, g_wallSize * g_translationFactorOnVerticalAxis);
    
    // 4th wall block
    updateHorizontalTranslationFactor();
    drawWallAtScreenPosition(g_wallSize * g_translationFactorOnHorizontalAxis, g_wallSize * g_translationFactorOnVerticalAxis);
    
    // 5h wall block (next line)
    updateVerticalTranslationFactor();
    resetHorizontalTranslationFactor();
    drawPathAtScreenPosition(g_wallSize * g_translationFactorOnHorizontalAxis, g_wallSize * g_translationFactorOnVerticalAxis);
}


/**
 * draws a wall square at the given screen position
 */
void drawWallAtScreenPosition(float x, float y)
{
    // red color
    float color[] = {1.0f, 0.0f, 0.0f};
    drawSquareAtScreenPositionWithColor(x, y, color);
}


/**
 * draws a path square at the given screen position
 */
void drawPathAtScreenPosition(float x, float y)
{
    // white color
    float color[] = {1.0f, 1.0f, 1.0f};
    drawSquareAtScreenPositionWithColor(x, y, color);
}


/**
 * draws a "help" square at the given screen position
 */
void drawHelpAtScreenPosition(float x, float y)
{
    // yellow color
    float color[] = {1.0f, 1.0f, 0.0f};
    drawSquareAtScreenPositionWithColor(x, y, color);
}


/**
 * draws the player square at the given screen position
 */
void drawPlayerAtScreenPosition(float x, float y)
{
    // yellow
    float color[] = {0.0f, 1.0f, 1.0f};
    drawSquareAtScreenPositionWithColor(x, y, color);
}


/**
 * draws the target square at the given screen position
 */
void drawTargetAtScreenPosition(float x, float y)
{
    // blue
    float color[] = {0.0f, 0.0f, 1.0f};
    drawSquareAtScreenPositionWithColor(x, y, color);
}


/**
 * draws any square at the given screen position
 */
void drawSquareAtScreenPositionWithColor(float x, float y, float color[])
{
    glLoadIdentity();
    glTranslatef(x + g_positionXToTopScreen, y + g_positionYToTopScreen, g_zDistance);
    
    glColor3f(color[0], color[1], color[2]);
    glBegin(GL_QUADS);
        glVertex3f(-g_wallSize, -g_wallSize, 0.0f);
        glVertex3f(g_wallSize, -g_wallSize, 0.0f);
        glVertex3f(g_wallSize, g_wallSize, 0.0f);
        glVertex3f(-g_wallSize, g_wallSize, 0.0f);
    glEnd();
}


/**
 * resets horizontal translation factor 
 * (helper function need to draw squares adjacent to each other)
 */
void resetHorizontalTranslationFactor()
{
    g_translationFactorOnHorizontalAxis = 1;
}


/**
 * updates horizontal translation factor
 * (helper function need to draw squares adjacent to each other)
 */
void updateHorizontalTranslationFactor()
{
    g_translationFactorOnHorizontalAxis += 2;
}


/**
 * resets vertical translation factor
 * (helper function need to draw squares adjacent to each other)
 */
void resetVerticalTranlationFactor()
{
    g_translationFactorOnVerticalAxis = 1;
}


/**
 * updates vertical translation factor
 * (helper function need to draw squares adjacent to each other)
 */
void updateVerticalTranslationFactor()
{
    g_translationFactorOnVerticalAxis -= 2;
}


/**
 * scale calculation to adjust text to fit a given square size
 */
float computeScaleForSquareSize(char* strs[], int numberOfStrings, float squareSize)
{
	float maxWidth = 0;
	for(int i = 0; i < squareSize; i++)
    {
		float width = t3dDrawWidth(strs[i]);
		if (width > maxWidth)
        {
			maxWidth = width;
		}
	}
	return (0.8 * squareSize) / maxWidth;
}


/**
 * clears text
 */
void cleanUp()
{
    t3dCleanup();
}


float computeMenuScaleForSquareSize(const char* strs[], int numberOfStrings, float squareSize)
{
	float maxWidth = 0;
	for(int i = 0; i < squareSize; i++)
    {
		float width = t3dDrawWidth(strs[i]);
		if (width > maxWidth)
        {
			maxWidth = width;
		}
	}
	return (0.8 * squareSize) / maxWidth;
}


void drawMenu()
{
    int i = 0;
    float color[] = {1.0f, 1.0f, 0.0f};
    for(; i < 4; i++)
    {
        drawLabelAtScreenPositionWithTextAndColor(g_wallSize * topTranslationFactorHorizontal, g_wallSize * topTranslationFactorVertical * i * 2, menuOptions[i], color);
    }
}


void drawSystemMessage(int i)
{
    float color[] = {1.0f, 1.0f, 1.0f};
    drawLabelAtScreenPositionWithTextAndColor(g_wallSize * topTranslationFactorHorizontal, g_wallSize * bottomTranslationFactorVertical, systemMsg[i], color);
}


void drawInstructionMessages()
{
    int i = 0;
    float color[] = {0.0f, 0.0f, 1.0f};
    for(; i < 4; i++)
    {
        drawLabelAtScreenPositionWithTextAndColor(g_wallSize * topTranslationFactorHorizontal, g_wallSize * topTranslationFactorVertical * i - 0.8, instructionMsg[i], color);
    }
}


void drawLabelAtScreenPositionWithTextAndColor(float x, float y, char *str, float color[])
{
    glLoadIdentity();
    glTranslatef(x + g_positionXToTopScreen, y + g_positionYToTopScreen, g_zDistance );
    
    glScalef(g_scale / 6, g_scale / 6, g_scale / 6);
    
    glColor3f(color[0], color[1], color[2]);
    glPushMatrix();
        glTranslatef(0, 0, 1.5f / g_scale);
        t3dDraw3D(str, 0, 0, 0.5f);
    glPopMatrix();
}


void setupAmbientLight()
{
    glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -8.0f);
	
    // ambient light
	GLfloat ambientColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
}


void setupPositionedLight()
{
    glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -8.0f);
    
    // positioned light
	GLfloat lightColor0[] = {0.5f, 0.5f, 0.5f, 0.5f};
	GLfloat lightPos0[] = {0.5f, 0.5f, 15.0f, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
}
