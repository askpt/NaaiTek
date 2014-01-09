//
//  main.cpp
//  Maze
//
//  Created by João Carreira on 06/01/14.
//  Copyright (c) 2014 João Carreira. All rights reserved.
//

#include <iostream>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "ImageLoader.h"
#include "text3d.h"
#include "Maze.h"

#include <vector>


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
const float g_wallSize = 0.075;
int mazeSizeHorizontal;
int mazeSizeVertical;

float g_positionXToTopScreen = -2.0;
float g_positionYToTopScreen = 1.5;

// translation factor (used in maze's wall and path drawing)
float g_translationFactorOnHorizontalAxis;
float g_translationFactorOnVerticalAxis;

// textures
GLuint g_textureId;
GLuint g_pathTexture;

// scale for drawing text
float g_scale;

// text to be written on screen
//const char * stringVector[4] = { "Naai Tek", "presents", "LAPR5", "2013" };

// data structure for maze
vector < vector < int > > maze;


// struct for 2D points
struct float2
{
    float2( float _x = 0.0f, float _y = 0.0f ) : x(_x), y(_y) {}
    float x;
    float y;
};


// struct the coordinates of both the player and goal in the maze
struct position
{
    int x;
    int y;
}player, target;


// variables to follow game outcome
bool playerDidWin;
bool playerDidQuit;
int totalHelpRequest;

// global variables for subwindow
int menuWindow;
char *title = "Maze Menu";
int numberOfOptions = 3;
char * optionsVector[3] = { "Maze Menu", "Start Game", "Quit"};
const char *stringVectorForRotatingCube[4] = { "NaaiTek", "presents", "NaaiTek", "presents" };


//************************************************************************
// forward declarations of OpenGL callback functions
//************************************************************************
void handleKeypress(unsigned char key, int x, int y);
void initRendering();
void handleResize(int w, int h);
void drawScene();
void drawMaze();
void drawMazeBootStrap();
void update(int value);
GLuint loadTexture(Image* image);
float computeScaleForSquareSize(const char* strs[], int numberOfStrings, float squareSize);
void cleanUp();
vector<vector<int>> mazeBuilder();
void drawWallAtScreenPosition(float x, float y);
void drawPathAtScreenPosition(float x, float y);
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

// fwd declarations of menu window
int menuBuilder();
void handleMenuKeypress(unsigned char key, int x, int y);
void handleMenuResize(int w, int h);
void drawMenuScene();
void setupMenuAmbientLight();
void setupMenuPositionedLight();
void drawTextAtScreenPosition(char *str, float x, float y, float z);
float computeMenuScaleForSquareSize(const char* strs[], int numberOfStrings, float squareSize);



/**
 * main
 *
 * @return: 0 (player quit), 1 (player won), 2 (error building maze)
 */
int main(int argc, char *argv[])
{
	// initializing GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
	// Window 1 - Game
    glutInitWindowSize(800, 800);
	mainWindow = glutCreateWindow("Maze");
    initRendering();
    
    //g_scale = computeScaleForSquareSize(stringVector, 4, 3.0);
    
    // building a random maze
    maze = mazeBuilder();
    mazeSizeHorizontal = 20;
    mazeSizeVertical = 20;
    
    // defining player's and target's positions
    setPlayerPosition(getFirstPossibleX(), getFirstPossibleY());
    setTargetPosition(getLastPossibleX(), getLastPossibleY());
    
    // reseting game settings
    resetGameSettings();
    
	// setting handler functions
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
    
	// adding a timer
	//glutTimerFunc(25, update, 0);
    
    // Window 2 - Menu
    //glutInitWindowSize(800, 800);
    glutInitWindowSize(300, 800);
    menuWindow = menuBuilder();
    
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
            if(isPossibleToMovePlayerToPosition(player.x - 1, player.y))
            {
                setPlayerPosition(player.x - 1, player.y);
                drawScene();
            }
            break;
            
        // DOWN
        case 'l':
        case 'L':
            if(isPossibleToMovePlayerToPosition(player.x + 1, player.y))
            {
                setPlayerPosition(player.x + 1, player.y);
                drawScene();
            }
            break;
            
        // LEFT
        case 'z':
        case 'Z':
            if(isPossibleToMovePlayerToPosition(player.x, player.y - 1))
            {
                setPlayerPosition(player.x, player.y - 1);
                drawScene();
            }
            break;
            
        // RIGHT
        case 'x':
        case 'X':
            if(isPossibleToMovePlayerToPosition(player.x, player.y + 1))
            {
                setPlayerPosition(player.x, player.y + 1);
                drawScene();
            }
        break;
            
        // requesting help
        case 'h':
        case 'H':
            totalHelpRequest++;
            cout << totalHelpRequest << endl;
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
}


/**
 * draws a scene
 */
void drawScene()
{
    // standard stuff
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
    
    // draws maze
    //drawMazeBootStrap();
    drawMaze();
    
	glutSwapBuffers();
}


void drawMaze()
{
    glLoadIdentity();
	glTranslatef(0.0f, 0.0f, g_zDistance);
    
    resetHorizontalTranslationFactor();
    resetVerticalTranlationFactor();
    
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
            else
            {
                drawWallAtScreenPosition(g_wallSize * g_translationFactorOnHorizontalAxis, g_wallSize * g_translationFactorOnVerticalAxis);
            }
            updateHorizontalTranslationFactor();
        }
        resetHorizontalTranslationFactor();
        updateVerticalTranslationFactor();
    }
    checkIfPlayerWon();
    
}


void setPlayerPosition(int x, int y)
{
    // player's position
    player.x = x;
    player.y = y;
}


void setTargetPosition(int x, int y)
{
    // target's position
    target.x = x;
    target.y = y;
}


bool isPossibleToMovePlayerToPosition(int x, int y)
{
    // player can't move if there's a wall
    if(maze[x][y] == 0)
    {
        return false;
    }
    
    // player can't move if it's maze's limits
    /*
    if(x < 1 || y < 1)
    {
        return false;
    }
     */
    
    return true;
}


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


void checkIfPlayerWon()
{
    if(player.x == target.x && player.y == target.y)
    {
        playerDidWin = true;
        exit(1);
    }
}


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


void drawPlayerAtScreenPosition(float x, float y)
{
    // yellow
    float color[] = {0.0f, 1.0f, 1.0f};
    drawSquareAtScreenPositionWithColor(x, y, color);
}


void drawTargetAtScreenPosition(float x, float y)
{
    // blue
    float color[] = {0.0f, 0.0f, 1.0f};
    drawSquareAtScreenPositionWithColor(x, y, color);
}


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


void resetHorizontalTranslationFactor()
{
    g_translationFactorOnHorizontalAxis = 1;
}


void updateHorizontalTranslationFactor()
{
    g_translationFactorOnHorizontalAxis += 2;
}


void resetVerticalTranlationFactor()
{
    g_translationFactorOnVerticalAxis = 1;
}


void updateVerticalTranslationFactor()
{
    g_translationFactorOnVerticalAxis -= 2;
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


/**
 * takes an image and returns the id that OpenGL gives to it
 */
GLuint loadTexture(Image* image)
{
	GLuint textureId;
    
    // "making room" for the texture (number of textures need and the array that will store the ids)
	glGenTextures(1, &textureId);
    
    // texture to edit
	glBindTexture(GL_TEXTURE_2D, textureId);
    
	// mapping the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                     // Always GL_TEXTURE_2D
				 0,                                 // 0 for now
				 GL_RGB,                            // Format OpenGL uses for image
				 image->width, image->height,       // Width and height
				 0,                                 // The border of the image
				 GL_RGB,                            // GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE,                  // GL_UNSIGNED_BYTE, because pixels are stored as unsigned numbers
				 image->pixels);                    // The actual pixel data
	
    return textureId;
}


float computeScaleForSquareSize(const char* strs[], int numberOfStrings, float squareSize)
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


void cleanUp()
{
    t3dCleanup();
}



// MENU WINDOW


int menuBuilder()
{
    menuWindow = glutCreateWindow("Maze Menu");
    glutPositionWindow(920,20);
    
    g_scale = computeScaleForSquareSize(stringVectorForRotatingCube, 3, 3.0);
    
    glutDisplayFunc(drawMenuScene);
	glutKeyboardFunc(handleMenuKeypress);
	glutReshapeFunc(handleMenuResize);
    return 0;
}


void handleMenuKeypress(unsigned char key, int x, int y)
{
	
}


void drawMenuScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
    
    // enabling lighting
    setupMenuAmbientLight();
    setupMenuPositionedLight();
    
    // text with game modes
    drawTextAtScreenPosition("Online Mode", 0.0, 0.0, -15.0);
    //drawTextAtScreenPosition("Offline Mode", 0.0, -1.0, -15.0);
    //drawTextAtScreenPosition("Exit", 0.0, -2.0, -15.0);
    
    glutSwapBuffers();
}


void handleMenuResize(int w, int h)
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


void setupMenuAmbientLight()
{
    glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -8.0f);
	
    // ambient light
	GLfloat ambientColor[] = {0.4f, 0.4f, 0.4f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
}


void setupMenuPositionedLight()
{
    glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -8.0f);
    
    // positioned light
	GLfloat lightColor0[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat lightPos0[] = {0.5f, 0.5f, 1.0f, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
}


void drawTextAtScreenPosition(char *str, float x, float y, float z)
{
    glLoadIdentity();
	glTranslatef(x, y, z);
    
    // "inclining" the camera to see from above
	glRotatef(20.0f, 1.5f, 1.0f, 0.0f);
    
    //setting up the correct scale for the text
	glScalef(g_scale * 0.7, g_scale * 0.7, g_scale * 0.7);
	
    // text color
    glColor3f(1.0f, 1.0f, 1.0f);
    
    glPushMatrix();
        glTranslatef(0, 0, -1.5f / g_scale);
        t3dDraw3D(str, 0, 0, 0.2f);
    cout << "String" << str << endl;
    cout << "x=" << x << endl;
    cout << "Y=" << y << endl;
    cout << "z=" << z << endl;
    glPopMatrix();
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






