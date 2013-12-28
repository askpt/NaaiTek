//
//  main.cpp
//  Review1
//
//  Created by João Carreira on 28/12/13.
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

using namespace std;

//************************************************************************
// global variables
//************************************************************************

// lenght of each side of the cube
const float g_cubeSide = 7.0f;

// angle for cube rotation
float g_angle = 0.7;

// id for texture
GLuint g_textureId;


//************************************************************************
// forward declarations of OpenGL callback functions
//************************************************************************
void handleKeypress(unsigned char key, int x, int y);
void initRendering();
void handleResize(int w, int h);
void drawScene();
void update(int value);
GLuint loadTexture(Image* image);


/**
 * main
 */
int main(int argc, char *argv[])
{
	// initializing GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
    
	// creating the window
	glutCreateWindow("Lightning");
	initRendering();
    
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
	   
	// normalizing normals
	glEnable(GL_NORMALIZE);
    
	// enabling smooth shading
	glShadeModel(GL_SMOOTH);
    
    // loading the image as texture
    Image* image = loadBMP("/Users/joaocarreira/Desktop/OpenGL/Review1/Review1/vtr.bmp");
	g_textureId = loadTexture(image);
	delete image;
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
    // clearing previous draw
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
    // switching to "normal" transformation mode
    glMatrixMode(GL_MODELVIEW);
    
    // making sure we're at the origin and facing the negative z axis
	glLoadIdentity();
    
    // putting the draw 20 units away from the camera
	glTranslatef(0.0f, 1.0f, -20.0f);
    
	// adding ambient light
	GLfloat ambientLight [] = { 0.3f, 0.3f, 0.3f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
    
	// adding positioned light
	GLfloat directLight [] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat directLightPos [] = { 2 * g_cubeSide, g_cubeSide, 4 * g_cubeSide, 0.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, directLight);
	// setting the ligth source position
	glLightfv(GL_LIGHT0, GL_POSITION, directLightPos);
    
    // setting up cube rotation on the vector (1, 1, 0)
    glRotatef(-g_angle, 1.0f, 1.0f, 0.0f);
    
    // drawing the faces
    glBegin(GL_QUADS);
    
        // Top face (normal color)
        glColor3f(1.0f, 1.0f, 0.0f);
        glNormal3f(0.0, 1.0f, 0.0f);
        glVertex3f(-g_cubeSide / 2, g_cubeSide / 2, -g_cubeSide / 2);
        glVertex3f(-g_cubeSide / 2, g_cubeSide / 2, g_cubeSide / 2);
        glVertex3f(g_cubeSide / 2, g_cubeSide / 2, g_cubeSide / 2);
        glVertex3f(g_cubeSide / 2, g_cubeSide / 2, -g_cubeSide / 2);
    
        // Bottom face (normal color)
        glColor3f(1.0f, 0.0f, 1.0f);
        glNormal3f(0.0, -1.0f, 0.0f);
        glVertex3f(-g_cubeSide / 2, -g_cubeSide / 2, -g_cubeSide / 2);
        glVertex3f(g_cubeSide / 2, -g_cubeSide / 2, -g_cubeSide / 2);
        glVertex3f(g_cubeSide / 2, -g_cubeSide / 2, g_cubeSide / 2);
        glVertex3f(-g_cubeSide / 2, -g_cubeSide / 2, g_cubeSide / 2);
    
        // Left face (establishing a gradient with 1 color followed by 2 vertices)
        glNormal3f(-1.0, 0.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 1.0f);
        glVertex3f(-g_cubeSide / 2, -g_cubeSide / 2, -g_cubeSide / 2);
        glVertex3f(-g_cubeSide / 2, -g_cubeSide / 2, g_cubeSide / 2);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-g_cubeSide / 2, g_cubeSide / 2, g_cubeSide / 2);
        glVertex3f(-g_cubeSide / 2, g_cubeSide / 2, -g_cubeSide / 2);
    
        //Right face (gradient too)
        glNormal3f(1.0, 0.0f, 0.0f);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(g_cubeSide / 2, -g_cubeSide / 2, -g_cubeSide / 2);
        glVertex3f(g_cubeSide / 2, g_cubeSide / 2, -g_cubeSide / 2);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(g_cubeSide / 2, g_cubeSide / 2, g_cubeSide / 2);
        glVertex3f(g_cubeSide / 2, -g_cubeSide / 2, g_cubeSide / 2);
    
    glEnd();
    
    // setting up textures before drawing the textured faces
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, g_textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glColor3d(1.0f, 1.0f, 1.0f);
    
    
    glBegin(GL_QUADS);
        //Front face
        glNormal3f(0.0, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-g_cubeSide / 2, -g_cubeSide / 2, g_cubeSide / 2);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(g_cubeSide / 2, -g_cubeSide / 2, g_cubeSide / 2);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(g_cubeSide / 2, g_cubeSide / 2, g_cubeSide / 2);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-g_cubeSide / 2, g_cubeSide / 2, g_cubeSide / 2);
    
        //Back face
        glNormal3f(0.0, 0.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-g_cubeSide / 2, -g_cubeSide / 2, -g_cubeSide / 2);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-g_cubeSide / 2, g_cubeSide / 2, -g_cubeSide / 2);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(g_cubeSide / 2, g_cubeSide / 2, -g_cubeSide / 2);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(g_cubeSide / 2, -g_cubeSide / 2, -g_cubeSide / 2);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
    
	glutSwapBuffers();
}


void update(int value)
{
	// rotates the shapes 1.5 degrees in every 25 miliseconds
	g_angle += 1.0f;
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
