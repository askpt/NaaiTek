#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <ctime>
#include <GL/glut.h>

int g_iWindowWidth = 512;
int g_iWindowHeight = 512;
int g_iGLUTWindowHandle = 0;

int g_iErrorCode = 0;

float g_fRotate1 = 0.0f;
float g_fRotate2 = 0.0f;
float g_fRotate3 = 0.0f;

std::clock_t g_PreviousTicks;
std::clock_t g_CurrentTicks;

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

enum ESceneType
{
	ST_Scene1 = 0,
	ST_Scene2,
	ST_Scene3,
	ST_Scene4,
	ST_NumScenes
};
ESceneType g_eCurrentScene = ST_Scene1;

void Cleanup(int errorCode, bool bExit)
{
	if (g_iGLUTWindowHandle != 0)
	{
		glutDestroyWindow(g_iGLUTWindowHandle);
		g_iGLUTWindowHandle = 0;
	}

	if (bExit)
	{
		exit(errorCode);
	}
}

void DrawTriangle(float2 p1, float2 p2, float2 p3)
{
	glBegin(GL_TRIANGLES);
	glVertex2f(p1.x, p1.y);
	glVertex2f(p2.x, p2.y);
	glVertex2f(p3.x, p3.y);
	glEnd();
}

void DrawRectangle(float width, float height)
{
	const float w = width / 2.0f;
	const float h = height / 2.0f;

	glBegin(GL_QUADS);
	glVertex2f(-w, h);   // Top-Left
	glVertex2f(w, h);   // Top-Right
	glVertex2f(w, -h);   // Bottom-Right
	glVertex2f(-w, -h);   // Bottom-Left
	glEnd();

}

void DrawCircle(float radius, int numSides /* = 8 */)
{
	const float step = M_PI / numSides;
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0.0f, 0.0f);
	for (float angle = 0.0f; angle < (2.0f * M_PI); angle += step)
	{
		glVertex2f(radius * sinf(angle), radius * cosf(angle));
	}
	glVertex2f(0.0f, radius); // One more vertex to close the circle
	glEnd();
}

void RenderScene1()
{
	glMatrixMode(GL_MODELVIEW);                                           // Switch to modelview matrix mode
	glLoadIdentity();                                                       // Load the identity matrix

	glTranslatef(-1.5f, 1.0f, -6.0f);                                     // Translate our view matrix back and a bit to the left.
	glColor3f(1.0f, 0.0f, 0.0f);                                          // Set Color to red
	DrawTriangle(float2(0.0f, 1.0f), float2(-1.0f, -1.0f), float2(1.0f, -1.0f));

	glTranslatef(3.0f, 0.0f, 0.0f);                                       // Shift view 3 units to the right
	glColor3f(0.0f, 0.0f, 1.0f);                                          // Set Color to blue
	DrawRectangle(2.0f, 2.0f);

	glTranslatef(-1.5f, -3.0f, 0.0f);                                     // Back to center and lower screen
	glColor3f(1.0f, 1.0f, 0.0f);                                          // Set color to yellow
	DrawCircle(1.0f, 16);
}
void RenderScene2()
{
	glMatrixMode(GL_MODELVIEW);                                           // Switch to modelview matrix mode
	glLoadIdentity();                                                       // Load the identity matrix

	glTranslatef(-1.5f, 1.0f, -6.0f);                                     // Translate back and to the left
	// Draw a triangle with different colors on each vertex
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);                                      // Red
	glVertex2f(0.0f, 1.0f);                                           // Top-Center

	glColor3f(0.0f, 1.0f, 0.0f);                                      // Green
	glVertex2f(-1.0f, -1.0f);                                         // Bottom-Left

	glColor3f(0.0f, 0.0f, 1.0f);                                      // Blue
	glVertex2f(1.0f, -1.0f);                                          // Bottom-Right
	glEnd();

	glTranslatef(3.0f, 0.0f, 0.0f);                                       // Translate right
	// Draw a rectangle with different colors on each vertex
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 0.0f);                                      // Red
	glVertex2f(-1.0f, 1.0f);                                          // Top-Left

	glColor3f(0.0f, 1.0f, 0.0f);                                      // Green
	glVertex2f(1.0f, 1.0f);                                           // Top-Right

	glColor3f(0.0f, 0.0f, 1.0f);                                      // Blue
	glVertex2f(1.0f, -1.0f);                                          // Bottom-Right

	glColor3f(1.0f, 1.0f, 1.0f);                                      // White
	glVertex2f(-1.0f, -1.0f);                                         // Bottom-Left
	glEnd();

	glTranslatef(-1.5f, -3.0f, 0.0f);                                     // Back to center and lower screen

	// Draw a circle with blended red/blue vertices.
	const float step = M_PI / 16;
	const float radius = 1.0f;

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(0.0f, 0.0f);
	for (float angle = 0.0f; angle < (2.0f * M_PI); angle += step)
	{
		float fSin = sinf(angle);
		float fCos = cosf(angle);

		glColor3f((fCos + 1.0f) * 0.5f, (fSin + 1.0f) * 0.5f, 0.0f);
		glVertex2f(radius * fSin, radius * fCos);
	}
	glColor3f(1.0f, 0.5f, 0.0f);
	glVertex2f(0.0f, radius); // One more vertex to close the circle
	glEnd();
}

void RenderScene3()
{
	glMatrixMode(GL_MODELVIEW);                                           // Switch to modelview matrix mode
	glLoadIdentity();                                                       // Load the identity matrix

	glTranslatef(-1.5f, 1.0f, -6.0f);                                     // Translate back and to the left
	glPushMatrix();                                                         // Push the current transformation onto the matrix stack
	glRotatef(g_fRotate1, 0.0f, 0.0f, 1.0f);
	// Draw a triangle with different colors on each vertex
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);                                      // Red
	glVertex2f(0.0f, 1.0f);                                           // Top-Center

	glColor3f(0.0f, 1.0f, 0.0f);                                      // Green
	glVertex2f(-1.0f, -1.0f);                                         // Bottom-Left

	glColor3f(0.0f, 0.0f, 1.0f);                                      // Blue
	glVertex2f(1.0f, -1.0f);                                          // Bottom-Right
	glEnd();
	glPopMatrix();

	glTranslatef(3.0f, 0.0f, 0.0f);                                       // Translate right
	glPushMatrix();
	glRotatef(g_fRotate2, 0.0f, 0.0f, 1.0f);
	// Draw a rectangle with different colors on each vertex
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 0.0f);                                      // Red
	glVertex2f(-1.0f, 1.0f);                                          // Top-Left

	glColor3f(0.0f, 1.0f, 0.0f);                                      // Green
	glVertex2f(1.0f, 1.0f);                                           // Top-Right

	glColor3f(0.0f, 0.0f, 1.0f);                                      // Blue
	glVertex2f(1.0f, -1.0f);                                          // Bottom-Right

	glColor3f(1.0f, 1.0f, 1.0f);                                      // White
	glVertex2f(-1.0f, -1.0f);                                         // Bottom-Left
	glEnd();
	glPopMatrix();

	glTranslatef(-1.5f, -3.0f, 0.0f);                                     // Back to center and lower screen
	glPushMatrix();
	glRotatef(g_fRotate3, 0.0f, 0.0f, 1.0f);
	// Draw a circle with blended red/blue vertecies
	const float step = M_PI / 16;
	const float radius = 1.0f;

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(0.0f, 0.0f);
	for (float angle = 0.0f; angle < (2.0f * M_PI); angle += step)
	{
		float fSin = sinf(angle);
		float fCos = cosf(angle);

		glColor3f((fCos + 1.0f) * 0.5f, (fSin + 1.0f) * 0.5f, 0.0f);
		glVertex2f(radius * fSin, radius * fCos);
	}
	glColor3f(1.0f, 0.5f, 0.0f);
	glVertex2f(0.0f, radius); // One more vertex to close the circle
	glEnd();
	glPopMatrix();
}

void RenderScene4()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST);

	glTranslatef(-1.5f, 1.0f, -6.0f);                                     // Translate back and to the left

	glPushMatrix();                                                         // Push the current modelview matrix on the matrix stack
	glRotatef(g_fRotate1, 1.0f, 1.0f, 1.0f);                               // Rotate on all 3 axis
	glBegin(GL_TRIANGLES);                                                // Draw a pyramid
	glColor3f(1.0f, 0.0f, 0.0f);                                      // Red
	glVertex3f(0.0f, 1.0f, 0.0f);                                     // Top of front face
	glColor3f(0.0f, 1.0f, 0.0f);                                      // Green
	glVertex3f(-1.0f, -1.0f, 1.0f);                                   // Left of front face
	glColor3f(0.0f, 0.0f, 1.0f);                                      // Blue
	glVertex3f(1.0f, -1.0f, 1.0f);                                    // Right of front face

	glColor3f(1.0f, 0.0f, 0.0f);                                      // Red
	glVertex3f(0.0f, 1.0f, 0.0f);                                     // Top of right face
	glColor3f(0.0f, 0.0f, 1.0f);                                      // Blue
	glVertex3f(1.0f, -1.0f, 1.0f);                                    // Left of right face
	glColor3f(0.0f, 1.0f, 0.0f);                                      // Green
	glVertex3f(1.0f, -1.0f, -1.0f);                                   // Right of right face

	glColor3f(1.0f, 0.0f, 0.0f);                                      // Red
	glVertex3f(0.0f, 1.0f, 0.0f);                                     // Top of back face
	glColor3f(0.0f, 1.0f, 0.0f);                                      // Green
	glVertex3f(1.0f, -1.0f, -1.0f);                                   // Left of back face
	glColor3f(0.0f, 0.0f, 1.0f);                                      // Blue
	glVertex3f(-1.0f, -1.0f, -1.0f);                                  // Right of back face

	glColor3f(1.0f, 0.0f, 0.0f);                                      // Red
	glVertex3f(0.0f, 1.0f, 0.0f);                                     // Top of left face
	glColor3f(0.0f, 0.0f, 1.0f);                                      // Blue
	glVertex3f(-1.0f, -1.0f, -1.0f);                                  // Left of left face
	glColor3f(0.0f, 1.0f, 0.0f);                                      // Green
	glVertex3f(-1.0f, -1.0f, 1.0f);                                   // Right of left face
	glEnd();

	// Render a quad for the bottom of our pyramid
	glBegin(GL_QUADS);
	glColor3f(0.0f, 1.0f, 0.0f);                                      // Green
	glVertex3f(-1.0f, -1.0f, 1.0f);                                   // Left/right of front/left face
	glColor3f(0.0f, 0.0f, 1.0f);                                      // Blue
	glVertex3f(1.0f, -1.0f, 1.0f);                                    // Right/left of front/right face
	glColor3f(0.0f, 1.0f, 0.0f);                                      // Green
	glVertex3f(1.0f, -1.0f, -1.0f);                                   // Right/left of right/back face
	glColor3f(0.0f, 0.0f, 1.0f);                                      // Blue
	glVertex3f(-1.0f, -1.0f, -1.0f);                                  // Left/right of right/back face
	glEnd();
	glPopMatrix();

	glTranslatef(3.0f, 0.0f, 0.0f);                                        // Translate right
	glPushMatrix();                                                         // Push the current modelview matrix on the matrix stack
	glRotatef(g_fRotate2, 1.0f, 1.0f, 1.0f);                              // Rotate the primitive on all 3 axis
	glBegin(GL_QUADS);
	// Top face
	glColor3f(0.0f, 1.0f, 0.0f);                                   // Green
	glVertex3f(1.0f, 1.0f, -1.0f);                                   // Top-right of top face
	glVertex3f(-1.0f, 1.0f, -1.0f);                                   // Top-left of top face
	glVertex3f(-1.0f, 1.0f, 1.0f);                                   // Bottom-left of top face
	glVertex3f(1.0f, 1.0f, 1.0f);                                   // Bottom-right of top face

	// Bottom face
	glColor3f(1.0f, 0.5f, 0.0f);                                  // Orange
	glVertex3f(1.0f, -1.0f, -1.0f);                                  // Top-right of bottom face
	glVertex3f(-1.0f, -1.0f, -1.0f);                                  // Top-left of bottom face
	glVertex3f(-1.0f, -1.0f, 1.0f);                                  // Bottom-left of bottom face
	glVertex3f(1.0f, -1.0f, 1.0f);                                  // Bottom-right of bottom face

	// Front face
	glColor3f(1.0f, 0.0f, 0.0f);                                  // Red
	glVertex3f(1.0f, 1.0f, 1.0f);                                  // Top-Right of front face
	glVertex3f(-1.0f, 1.0f, 1.0f);                                  // Top-left of front face
	glVertex3f(-1.0f, -1.0f, 1.0f);                                  // Bottom-left of front face
	glVertex3f(1.0f, -1.0f, 1.0f);                                  // Bottom-right of front face

	// Back face
	glColor3f(1.0f, 1.0f, 0.0f);                                 // Yellow
	glVertex3f(1.0f, -1.0f, -1.0f);                                 // Bottom-Left of back face
	glVertex3f(-1.0f, -1.0f, -1.0f);                                 // Bottom-Right of back face
	glVertex3f(-1.0f, 1.0f, -1.0f);                                 // Top-Right of back face
	glVertex3f(1.0f, 1.0f, -1.0f);                                 // Top-Left of back face

	// Left face
	glColor3f(0.0f, 0.0f, 1.0f);                                   // Blue
	glVertex3f(-1.0f, 1.0f, 1.0f);                                   // Top-Right of left face
	glVertex3f(-1.0f, 1.0f, -1.0f);                                   // Top-Left of left face
	glVertex3f(-1.0f, -1.0f, -1.0f);                                   // Bottom-Left of left face
	glVertex3f(-1.0f, -1.0f, 1.0f);                                   // Bottom-Right of left face

	// Right face
	glColor3f(1.0f, 0.0f, 1.0f);                                   // Violet
	glVertex3f(1.0f, 1.0f, 1.0f);                                   // Top-Right of left face
	glVertex3f(1.0f, 1.0f, -1.0f);                                   // Top-Left of left face
	glVertex3f(1.0f, -1.0f, -1.0f);                                   // Bottom-Left of left face
	glVertex3f(1.0f, -1.0f, 1.0f);                                   // Bottom-Right of left face
	glEnd();
	glPopMatrix();

	glTranslatef(-1.5f, -3.0f, 0.0f);                                     // Back to center and lower screen
	glPushMatrix();
	glRotatef(g_fRotate3, 1.0f, 1.0f, 1.0f);
	glColor3f(1.0f, 1.0f, 0.0f);                                          // Yellow
	glutSolidSphere(1.0f, 16, 16);                                        // Use GLUT to draw a solid sphere
	glScalef(1.01f, 1.01f, 1.01f);
	glColor3f(1.0f, 0.0f, 0.0f);                                          // Red
	glutWireSphere(1.0f, 16, 16);                                         // Use GLUT to draw a wireframe sphere
	glPopMatrix();
}

void ReshapeGL(int w, int h)
{
	std::cout << "ReshapGL( " << w << ", " << h << " );" << std::endl;

	if (h == 0)                                        // Prevent a divide-by-zero error
	{
		h = 1;                                      // Making Height Equal One
	}

	g_iWindowWidth = w;
	g_iWindowHeight = h;

	glViewport(0, 0, g_iWindowWidth, g_iWindowHeight);

	// Setup the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble)g_iWindowWidth / (GLdouble)g_iWindowHeight, 0.1, 100.0);

	glutPostRedisplay();
}


void MouseGL(int button, int state, int x, int y)
{

}

void MotionGL(int x, int y)
{

}

void KeyboardGL(unsigned char c, int x, int y)
{
	// Store the current scene so we can test if it has changed later.
	ESceneType currentScene = g_eCurrentScene;

	switch (c)
	{
	case '1':
	{
				glClearColor(1.0f, 1.0f, 1.0f, 1.0f);                         // White background
				g_eCurrentScene = ST_Scene1;
	}
		break;
	case '2':
	{
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);                         // Black background
				g_eCurrentScene = ST_Scene2;
	}
		break;
	case '3':
	{
				glClearColor(0.27f, 0.27f, 0.27f, 1.0f);                      // Dark-Gray background
				g_eCurrentScene = ST_Scene3;
	}
		break;
	case '4':
	{
				glClearColor(0.73f, 0.73f, 0.73f, 1.0f);                      // Light-Gray background
				g_eCurrentScene = ST_Scene4;
	}
		break;
	case 's':
	case 'S':
	{
				std::cout << "Shade Model: GL_SMOOTH" << std::endl;
				// Switch to smooth shading model
				glShadeModel(GL_SMOOTH);
	}
		break;
	case 'f':
	case 'F':
	{
				std::cout << "Shade Model: GL_FLAT" << std::endl;
				// Switch to flat shading model
				glShadeModel(GL_FLAT);
	}
		break;
	case 'r':
	case 'R':
	{
				std::cout << "Reset Parameters..." << std::endl;
				g_fRotate1 = g_fRotate2 = g_fRotate3 = 0.0f;
	}
		break;
	case '\033': // escape quits
	case '\015': // Enter quits
	case 'Q':    // Q quits
	case 'q':    // q (or escape) quits
	{
					 // Cleanup up and quit
					 //Cleanup(0);
	}
		break;
	}

	if (currentScene != g_eCurrentScene)
	{
		std::cout << "Changed Render Scene: " << (g_eCurrentScene + 1) << std::endl;
	}

	glutPostRedisplay();
}


void IdleGL()
{
	// Update our simulation
	g_CurrentTicks = std::clock();
	float deltaTicks = (g_CurrentTicks - g_PreviousTicks);
	g_PreviousTicks = g_CurrentTicks;

	float fDeltaTime = deltaTicks / (float)CLOCKS_PER_SEC;

	// Rate of rotation in (degrees) per second
	const float fRotationRate = 50.0f;

	// Update our rotation parameters
	g_fRotate1 += (fRotationRate * fDeltaTime);
	g_fRotate1 = fmodf(g_fRotate1, 360.0f);

	g_fRotate2 += (fRotationRate * fDeltaTime);
	g_fRotate2 = fmodf(g_fRotate2, 360.0f);

	g_fRotate3 += (fRotationRate * fDeltaTime);
	g_fRotate3 = fmodf(g_fRotate3, 360.0f);

	glutPostRedisplay();
}

void DisplayGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (g_eCurrentScene)
	{
	case ST_Scene1:
	{
					  RenderScene1();
	}
		break;
	case ST_Scene2:
	{
					  RenderScene2();
	}
		break;
	case ST_Scene3:
	{
					   RenderScene3();
	}
		break;
	case ST_Scene4:
	{
					  RenderScene4();
	}
		break;
	}

	glutSwapBuffers();
	glutPostRedisplay();
}

void InitGL(int argc, char* argv[])
{
	std::cout << "Initialise OpenGL..." << std::endl;

	glutInit(&argc, argv);
	int iScreenWidth = glutGet(GLUT_SCREEN_WIDTH);
	int iScreenHeight = glutGet(GLUT_SCREEN_HEIGHT);

	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowPosition((iScreenWidth - g_iWindowWidth) / 2,
		(iScreenHeight - g_iWindowHeight) / 2);
	glutInitWindowSize(g_iWindowWidth, g_iWindowHeight);

	g_iGLUTWindowHandle = glutCreateWindow("OpenGL");

	// Register GLUT callbacks
	glutDisplayFunc(DisplayGL);
	glutIdleFunc(IdleGL);
	glutMouseFunc(MouseGL);
	glutMotionFunc(MotionGL);
	glutKeyboardFunc(KeyboardGL);
	glutReshapeFunc(ReshapeGL);

	// Setup initial GL State
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepth(1.0f);

	glShadeModel(GL_SMOOTH);
	std::cout << "Initialise OpenGL: Success!" << std::endl;
}



int main(int argc, char* argv[])
{
	// Capture the previous time to calculate the delta time on the next frame
	g_PreviousTicks = std::clock();

	InitGL(argc, argv);
	glutMainLoop();
}