//
//  main.cpp
//  GameMenu
//
//  Created by João Carreira on 30/12/13.
//  Copyright (c) 2013 João Carreira. All rights reserved.
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

using namespace std;


//************************************************************************
// global variables
//************************************************************************

// rotation angle
float g_angle = -30.0f;
float g_cameraAngle = 0.0f;

// textures
GLuint g_textureId;

// scale for drawing text
float g_scale;

// text to be written on rotating cube
const char * stringVectorForRotatingCube[4] = { "NaaiTek", "presents", "NaaiTek", "presents" };

// text to be displayed in a "label"
const char* stringForLabel = "GRAPHS4SOCIAL";

// windows
int mainWindow, loginWindow;


//************************************************************************
// forward declarations of OpenGL callback functions
//************************************************************************
void handleKeypress(unsigned char key, int x, int y);
void initRendering();
void handleResize(int w, int h);
void drawScene();
void update(int value);
GLuint loadTexture(Image* image);
float computeScaleForSquareSize(const char* strs[], int numberOfStrings, float squareSize);
void cleanUp();

// drawing subrotines
void drawRotatingCubeAtScreenPosition(float x, float y, float z);
void drawLabelWithTextAtScreenPosition(float x, float y, float z);
void setupAmbientLight();
void setupPositionedLight();
void drawBackgroundAtScreenPosition(float x, float y, float z);


/**
 * main
 */
int main(int argc, char *argv[])
{
	// initializing GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
    
	// creating the main window
	mainWindow = glutCreateWindow("Graphs4Social");
	
    initRendering();
    
    g_scale = computeScaleForSquareSize(stringVectorForRotatingCube, 3, 3.0);
    
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
    
    // loading the image as texture
    //Image* image = loadBMP("/Users/joaocarreira/Desktop/OpenGL/Text/Text/vtr.bmp");
	//g_textureId = loadTexture(image);
	//delete image;

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


/**
 * draws a scene
 */
void drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
    
    // enabling lighting
    setupAmbientLight();
    setupPositionedLight();
    
    // drawing rotating cube with "NaaiTek Presents"
    drawRotatingCubeAtScreenPosition(0.0, 5.0, -15.0);
    
    // drawing "Graphs4Social" label
    drawLabelWithTextAtScreenPosition(0.0, 2.0, -10.0);
    
    // drawing background
    drawBackgroundAtScreenPosition(0.0f, 1.0f, -5.0f);
    
    glutSwapBuffers();
}


void setupAmbientLight()
{
    glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -8.0f);
	
    // ambient light
	GLfloat ambientColor[] = {0.4f, 0.4f, 0.4f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
}


void setupPositionedLight()
{
    glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -8.0f);
    
    // positioned light
	GLfloat lightColor0[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat lightPos0[] = {0.5f, 0.5f, 1.0f, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
}


void drawRotatingCubeAtScreenPosition(float x, float y, float z)
{
	glLoadIdentity();
	glTranslatef(x, y, z);
	
    // "inclining" the camera to see from above
	glRotatef(40.0f, 1.0f, 0.0f, 0.0f);
    
    // rotation animation
	glRotatef(-g_angle, 0.0f, 1.0f, 0.0f);
	
	//setting up the correct scale for the text
	glScalef(g_scale, g_scale, g_scale);
	
    // text color
    glColor3f(1.0f, 0.3f, 0.3f);
    
    // drawing 4 strings
	for(int i = 0; i < 4; i++)
    {
		glPushMatrix();
		glRotatef(90 * i, 0, 1, 0);
		glTranslatef(0, 0, 1.5f / g_scale);
		t3dDraw3D(stringVectorForRotatingCube[i], 0, 0, 0.2f);
		glPopMatrix();
	}
}


void drawLabelWithTextAtScreenPosition(float x, float y, float z)
{
    glLoadIdentity();
	glTranslatef(x, y, z);
    
    // "inclining" the camera to see from above
	glRotatef(20.0f, 0.0f, 0.0f, 0.0f);
    
    //setting up the correct scale for the text
	glScalef(g_scale, g_scale, g_scale);
	
    // text color
    glColor3f(0.3f, 0.3f, 1.0f);
    
    glPushMatrix();
        glTranslatef(0, 0, 1.5f / g_scale);
        t3dDraw3D(stringForLabel, 0, 0, 0.2f);
    glPopMatrix();

}


void drawBackgroundAtScreenPosition(float x, float y, float z)
{
    glLoadIdentity();
	glTranslatef(x, y, z);
    
    // applying a color to the texture in the redish
    glColor3f(0.2f, 0.2f, 0.2f);
    
    // "floor"
    glBegin(GL_QUADS);
        glNormal3f(0.0, 1.0f, 0.0f);
    
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-2.5f, -2.5f, 2.5f);
    
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(2.5f, -2.5f, 2.5f);
    
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(2.5f, -2.5f, -2.5f);
    
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-2.5f, -2.5f, -2.5f);
    glEnd();
    
    
    // "left wall"
    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_QUADS);
        glNormal3f(1.0f, 0.0f, 0.0f);
	
        glVertex3f(-2.5f, -2.5f, 2.5f);
        glVertex3f(-2.5f, -2.5f, -2.5f);
        glVertex3f(-2.5f, 2.5f, -2.5f);
        glVertex3f(-2.5f, 2.5f, 2.5f);
	glEnd();
    
    // "right wall"
    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_QUADS);
        glNormal3f(1.0f, 0.0f, 0.0f);
	
        glVertex3f(2.5f, -2.5f, 2.5f);
        glVertex3f(2.5f, -2.5f, -2.5f);
        glVertex3f(2.5f, 2.5f, -2.5f);
        glVertex3f(2.5f, 2.5f, 2.5f);
	glEnd();
}


void update(int value)
{
	// rotates the shapes 1.5 degrees in every 25 miliseconds
	g_angle += 1.5f;
	if (g_angle > 360)
	{
		g_angle = 0;
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