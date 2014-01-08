#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>     
#include <GL\glut.h>
#include <iostream>
#include "graphs.h"

using namespace std;

#define degrees(X) (double)((X)*180/M_PI)
#define rad(X)   (double)((X)*M_PI/180)
#define K_SPHERE 2.1

// luzes e materiais

const GLfloat mat_ambient[][4] = { { 0.33, 0.22, 0.03, 1.0 },	// brass
{ 0.0, 0.0, 0.0 },			// red plastic
{ 0.0215, 0.1745, 0.0215 },	// emerald
{ 0.02, 0.02, 0.02 },		// slate
{ 0.0, 0.0, 0.1745 },		// blue
{ 0.02, 0.02, 0.02 },		// black
{ 0.1745, 0.1745, 0.1745 } };// grey

const GLfloat mat_diffuse[][4] = { { 0.78, 0.57, 0.11, 1.0 },		// brass
{ 0.5, 0.0, 0.0 },				// red plastic
{ 0.07568, 0.61424, 0.07568 },	// emerald
{ 0.78, 0.78, 0.78 },			// slate
{ 0.0, 0.0, 0.61424 },			// blue
{ 0.08, 0.08, 0.08 },			// black
{ 0.61424, 0.61424, 0.61424 } };	// grey

const GLfloat mat_specular[][4] = { { 0.99, 0.91, 0.81, 1.0 },			// brass
{ 0.7, 0.6, 0.6 },					// red plastic
{ 0.633, 0.727811, 0.633 },		// emerald
{ 0.14, 0.14, 0.14 },				// slate
{ 0.0, 0.0, 0.727811 },			// blue
{ 0.03, 0.03, 0.03 },				// black
{ 0.727811, 0.727811, 0.727811 } };	// grey

const GLfloat mat_shininess[] = { 27.8,	// brass
32.0,	// red plastic
76.8,	// emerald
18.78,	// slate
30.0,	// blue
75.0,	// black
60.0 };	// grey

enum material_type { brass, red_plastic, emerald, slate, blue, black, grey };

#ifdef __cplusplus
inline material_type operator++(material_type &rs, int) {
	return rs = (material_type)(rs + 1);
}
#endif

typedef	GLdouble Vertex[3];
typedef	GLdouble Vector[4];

typedef struct Keys_t{
	GLboolean   up, down, left, right, q, a;
}Keys_t;

typedef struct Camera{
	GLfloat fov;
	GLdouble dir_lat;
	GLdouble dir_long;
	GLfloat dist;
	Vertex center;
	GLfloat dimension;
	GLdouble velv, velh;

}Camera;

typedef struct State{
	Camera		camera;
	int			xMouse, yMouse;
	GLboolean	light;
	GLboolean	showNormals;
	GLint		lightViewer;
	GLint		translateAxis;
	GLdouble	axis[3];
	Keys_t		keys;
	GLint		timer;
}State;

typedef struct Model {
	GLfloat g_pos_light1[4];
	GLfloat g_pos_light2[4];

	GLfloat scale;
	GLUquadric *quad;
}Model;

State state;
Model model;

void initState(){
	state.camera.dir_lat = M_PI / 4;
	state.camera.dir_long = -M_PI / 4;
	state.camera.fov = 60;
	state.camera.dist = 5;
	state.axis[0] = 0;
	state.axis[1] = 0;
	state.axis[2] = 0;
	state.camera.center[0] = 0;
	state.camera.center[1] = 0;
	state.camera.center[2] = 0;
	state.camera.dimension = 2;
	state.camera.velv = 10;
	state.camera.velh = 10;
	state.light = GL_FALSE;
	state.showNormals = GL_FALSE;
	state.lightViewer = 1;
	state.timer = 100;
}

void initModel(){
	model.scale = 0.2;

	//model.cor_cubo = brass;
	model.g_pos_light1[0] = -5.0;
	model.g_pos_light1[1] = 5.0;
	model.g_pos_light1[2] = 5.0;
	model.g_pos_light1[3] = 0.0;
	model.g_pos_light2[0] = 5.0;
	model.g_pos_light2[1] = -15.0;
	model.g_pos_light2[2] = 5.0;
	model.g_pos_light2[3] = 0.0;
}

void myInit()
{

	GLfloat LuzAmbiente[] = { 0.5, 0.5, 0.5, 0.0 };

	glClearColor(0.55, 0.75, 1.0, 0.0);

	glEnable(GL_SMOOTH); /*enable smooth shading */
	glEnable(GL_LIGHTING); /* enable lighting */
	glEnable(GL_DEPTH_TEST); /* enable z buffer */
	glEnable(GL_NORMALIZE);

	glDepthFunc(GL_LESS);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LuzAmbiente);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, state.lightViewer);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	initModel();
	initState();

	readGraph();
	//readGraphUser("Andre");
}

void printHelp(void)
{
	printf("\n\nDraw the graph\n");
	printf("h,H - Help \n");
	printf("i,I - Reset Values \n");
	printf("******* Diverse ******* \n");

	char* stateLightViewer;
	(state.lightViewer != 1) ? stateLightViewer = "EYE" : stateLightViewer = "Z";
	printf("l,L - Alternate light calculus between Z and eye.\nCurrent: %s \n", stateLightViewer);

	char* stateLight;
	(state.light) ? stateLight = "CAMERA" : stateLight = "GLOBAL";
	printf("k,K - Alternate camera light with global light.\nCurrent: %s \n", stateLight);
	printf("s,S - PolygonMode Fill \n");
	printf("w,W - PolygonMode Wireframe \n");
	printf("p,P - PolygonMode Point \n");
	printf("c,C - On/Off Cull Face \n");
	printf("n,N - On/Off normal presentation \n");
	printf("******* Graphs ******* \n");
	printf("F1  - Save graph to file \n");
	printf("F2  - Read graph from file \n");
	printf("******* Camera ******* \n");
	printf("Left Buttton - Drag Axis (centro da camera)\n");
	printf("Right Buttton  - Rotate camera\n");
	printf("Right Buttton with CTRL - Zoom-in/out\n");
	printf("PAGE_UP, PAGE_DOWN - Alternate distance from camera \n");
	printf("ESC - Leave\n");
}


void material(enum material_type mat)
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient[mat]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse[mat]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular[mat]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess[mat]);
}

const GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };

void putLights(GLfloat* diffuse)
{
	const GLfloat white_amb[] = { 0.2, 0.2, 0.2, 1.0 };

	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
	glLightfv(GL_LIGHT0, GL_AMBIENT, white_amb);
	glLightfv(GL_LIGHT0, GL_POSITION, model.g_pos_light1);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);
	glLightfv(GL_LIGHT1, GL_AMBIENT, white_amb);
	glLightfv(GL_LIGHT1, GL_POSITION, model.g_pos_light2);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
}

#define nodeID 43
void drawNode(Node node)
{
	GLfloat x0 = node.x;
	GLfloat y0 = node.y;
	GLfloat z0 = node.z + INFINITESIMAL;

	GLfloat r = K_SPHERE * node.width / 2.0;

	glPushName(nodeID);
	glPushMatrix();
	glTranslatef(x0, y0, z0);

	glBegin(GL_POLYGON);
	GLUquadricObj* pQuadric = gluNewQuadric();

	gluSphere(pQuadric, r, 32.0, 32.0 * 4.0);

	gluDeleteQuadric(pQuadric);
	glEnd();
	glPopMatrix();
	glPopName();
}

#define pathID 59
void drawPath(Node noi, Node nof, Path p)
{
	GLfloat xf = nof.x;
	GLfloat yf = nof.y;
	GLfloat zf = nof.z;

	GLfloat xi = noi.x;
	GLfloat yi = noi.y;
	GLfloat zi = noi.z;

	GLfloat pij = sqrtf((xf - xi)*(xf - xi) + (yf - yi)*(yf - yi));
	GLfloat hij = zf - zi;
	GLfloat sij = sqrt(pij*pij + hij*hij);
	GLfloat wij = p.width;
	GLfloat r = wij / 2.0;
	GLfloat aij = degrees(atan2((yf - yi), (xf - xi)));
	GLfloat bij = degrees(atan2(hij, pij));

	glPushName(pathID);
	glPushMatrix();
	glTranslatef(xi, yi, zi);
	glRotatef(aij, 0, 0, 1.0);
	glRotatef(90 - bij, 0.0, 1.0, 0.0);

	material(red_plastic);
	GLUquadricObj* pQuadric = gluNewQuadric();
	gluCylinder(pQuadric, wij / 2.0, wij / 2.0, sij, 32, 4);
	gluDeleteQuadric(pQuadric);
	glPopMatrix();
	glPopName();
}

void drawGraph(){
	material(blue);
	for (int i = 0; i < numNodes; i++){
		drawNode(nodes[i]);
	}
	material(red_plastic);
	for (int i = 0; i < numPaths; i++){
		Node* noi = &nodes[paths[i].nodei];
		Node* nof = &nodes[paths[i].nodef];
		drawPath(*noi, *nof, paths[i]);
	}
}


int detectCameraColision(GLfloat xp, GLfloat yp, GLfloat zp){
	int i, n, objid = 0;
	double zmin = 10.0;
	GLuint buffer[100], *ptr;

	glSelectBuffer(100, buffer);
	glRenderMode(GL_SELECT);
	glInitNames();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix(); // saves projection
	glLoadIdentity();
	glOrtho(-state.camera.dimension / 2.0, state.camera.dimension / 2.0,
		-state.camera.dimension / 2.0, state.camera.dimension / 2.0,
		0.0, state.camera.dimension / 2.0 + (state.camera.velv + state.camera.velh));

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotatef(degrees(-M_PI / 2.0 - atan2(state.camera.velv, state.camera.velh)), 1.0, 0.0, 0.0);
	glRotatef(degrees(M_PI / 2.0 - state.camera.dir_long), 0.0, 0.0, 1.0);
	glTranslatef(-xp, -yp, -zp);
	drawGraph();

	n = glRenderMode(GL_RENDER);
	if (n > 0)
	{
		ptr = buffer;
		for (i = 0; i < n; i++)
		{
			if (zmin >(double) ptr[1] / UINT_MAX) {
				zmin = (double)ptr[1] / UINT_MAX;
				objid = ptr[3];
			}
			ptr += 3 + ptr[0]; // ptr[0] contains the number of names (usually 1); 3 corresponds to numnomes, zmin e zmax
		}
	}


	glMatrixMode(GL_PROJECTION); //puts back projection matrix
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	/*switch (objid){
	case 43:
	cout << "NODE" << endl;
	break;
	case 59:
	cout << "PATH" << endl;
	break;
	case 0:
	cout << "FREE ROAM" << endl;
	break;
	default:
	cout << "FIX ME" << endl;
	break;
	}*/
	return objid;
}

void Timer(int value)
{
	static int time;

	GLuint curr = glutGet(GLUT_ELAPSED_TIME);

	glutTimerFunc(state.timer, Timer, 0);

	double vel = 10, k = 0.25;

	GLdouble xp = state.camera.center[0];
	GLdouble yp = state.camera.center[1];
	GLdouble zp = state.camera.center[2];
	// get the y pos
	if (state.keys.up)
	{
		state.camera.dir_lat = -state.camera.dir_long;
		xp = state.camera.center[0] + k * vel * cos(state.camera.dir_lat);
		yp = state.camera.center[1] - k * vel * sin(state.camera.dir_lat);
	}
	else if (state.keys.down)
	{
		state.camera.dir_lat = -state.camera.dir_long;
		xp = state.camera.center[0] - k * vel * cos(state.camera.dir_lat);
		yp = state.camera.center[1] + k * vel * sin(state.camera.dir_lat);
	}

	if (state.keys.a)
	{
		zp = state.camera.center[2] - k * vel / 4.0;
	}
	else if (state.keys.q)
	{
		zp = state.camera.center[2] + k * vel / 4.0;
	}

	int colide = detectCameraColision(state.camera.center[0], state.camera.center[1], state.camera.center[2]);
	if (colide == 0){
		state.camera.center[0] = xp;
		state.camera.center[1] = yp;
		state.camera.center[2] = zp;
	}
	else 
	{
		state.camera.center[2] = state.camera.center[2] + 1;
	}

	if (state.keys.left)
	{
		state.camera.dir_long += rad(5);
	}
	if (state.keys.right)
	{
		state.camera.dir_long -= rad(5);

	}
	glutPostRedisplay();
}

void setLight(){
	Vertex eye;

	eye[0] = state.camera.center[0] + state.camera.dist*cos(state.camera.dir_long)*cos(state.camera.dir_lat);
	eye[1] = state.camera.center[1] + state.camera.dist*sin(state.camera.dir_long)*cos(state.camera.dir_lat);
	eye[2] = state.camera.center[2] + state.camera.dist*sin(state.camera.dir_lat);

	if (state.light){
		gluLookAt(eye[0], eye[1], eye[2], state.camera.center[0], state.camera.center[1], state.camera.center[2], 0, 0, 1);
		putLights((GLfloat*)white_light);
	}
	else{
		putLights((GLfloat*)white_light);
		gluLookAt(eye[0], eye[1], eye[2], state.camera.center[0], state.camera.center[1], state.camera.center[2], 0, 0, 1);
	}
}

void setCamera(){
	glLoadIdentity();
	glRotated(degrees(-M_PI / 2.0), 1.0, 0.0, 0.0);
	glRotated(degrees(M_PI / 2.0 - state.camera.dir_long), 0.0, 0.0, 1.0);
	glTranslated(-state.camera.center[0], -state.camera.center[1], -state.camera.center[2]);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	setLight();
	setCamera();

	drawGraph();

	glFlush();
	glutSwapBuffers();
}


void keyboard(unsigned char key, int x, int y)
{

	switch (key)
	{
	case 27:
		exit(0);
		break;
	case 'h':
	case 'H':
		printHelp();
		break;
	case 'l':
	case 'L':
		if (state.lightViewer)
			state.lightViewer = 0;
		else
			state.lightViewer = 1;
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, state.lightViewer);
		glutPostRedisplay();
		break;
	case 'k':
	case 'K':
		state.light = !state.light;
		glutPostRedisplay();
		break;
	case 'w':
	case 'W':
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glutPostRedisplay();
		break;
	case 'p':
	case 'P':
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		glutPostRedisplay();
		break;
	case 's':
	case 'S':
		glEnable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glutPostRedisplay();
		break;
	case 'c':
	case 'C':
		if (glIsEnabled(GL_CULL_FACE))
			glDisable(GL_CULL_FACE);
		else
			glEnable(GL_CULL_FACE);
		glutPostRedisplay();
		break;
	case 'n':
	case 'N':
		state.showNormals = !state.showNormals;
		glutPostRedisplay();
		break;
	case 'i':
	case 'I':
		initState();
		initModel();
		glutPostRedisplay();
		break;
	case 'q':
	case'Q':
		state.keys.q = GL_TRUE;
		break;
	case 'a':
	case 'A':
		state.keys.a = GL_TRUE;
		break;

	}
}

void keyboardUp(unsigned char key, int x, int y)
{

	switch (key)
	{
	case 'q':
	case'Q':
		state.keys.q = GL_FALSE;
		break;
	case 'a':
	case 'A':
		state.keys.a = GL_FALSE;
		break;

	}
}

void SpecialUp(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		state.keys.up = GL_FALSE;
		break;
	case GLUT_KEY_DOWN:
		state.keys.down = GL_FALSE;
		break;
	case GLUT_KEY_LEFT:
		state.keys.left = GL_FALSE;
		break;
	case GLUT_KEY_RIGHT:
		state.keys.right = GL_FALSE;
		break;
	}

}

void Special(int key, int x, int y){

	switch (key){
	case GLUT_KEY_F1:
		saveGraph();
		break;
	case GLUT_KEY_F2:
		readGraph();
		glutPostRedisplay();
		break;
	case GLUT_KEY_UP:
		state.keys.up = GL_TRUE;
		break;
	case GLUT_KEY_DOWN:
		state.keys.down = GL_TRUE;
		break;
	case GLUT_KEY_LEFT:
		state.keys.left = GL_TRUE;
		break;
	case GLUT_KEY_RIGHT:
		state.keys.right = GL_TRUE;
		break;
	}
}

void setProjection(int x, int y, GLboolean picking){
	glLoadIdentity();
	if (picking) { // if its in "picking mode", reads the viewport and defines picking zone
		GLint vport[4];
		glGetIntegerv(GL_VIEWPORT, vport);
		gluPickMatrix(x, glutGet(GLUT_WINDOW_HEIGHT) - y, 4, 4, vport); // Inverts mouse Y axis, to correspond to window
	}

	gluPerspective(state.camera.fov, (GLfloat)glutGet(GLUT_WINDOW_WIDTH) / glutGet(GLUT_WINDOW_HEIGHT), 1, 500);

}

void myReshape(int w, int h){
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	setProjection(0, 0, GL_FALSE);
	glMatrixMode(GL_MODELVIEW);
}

void motionRotate(int x, int y){
#define DRAG_SCALE	0.01
	double lim = M_PI / 2 - 0.1;
	state.camera.dir_long += (state.xMouse - x)*DRAG_SCALE;
	state.camera.dir_lat -= (state.yMouse - y)*DRAG_SCALE*0.5;
	if (state.camera.dir_lat > lim)
		state.camera.dir_lat = lim;
	else
	if (state.camera.dir_lat < -lim)
		state.camera.dir_lat = -lim;
	state.xMouse = x;
	state.yMouse = y;
	glutPostRedisplay();
}

// NOT WORKING
void motionZoom(int x, int y){
#define ZOOM_SCALE	0.5
	state.camera.dist -= (state.yMouse - y)*ZOOM_SCALE;
	if (state.camera.dist<5)
		state.camera.dist = 5;
	else
	if (state.camera.dist>200)
		state.camera.dist = 200;
	state.yMouse = y;
	glutPostRedisplay();
}

int picking(int x, int y){
	int i, n, objid = 0;
	double zmin = 10.0;
	GLuint buffer[100], *ptr;

	glSelectBuffer(100, buffer);
	glRenderMode(GL_SELECT);
	glInitNames();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix(); // saves projection
	glLoadIdentity();
	setProjection(x, y, GL_TRUE);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	setCamera();
	//drawManyAxis();

	n = glRenderMode(GL_RENDER);
	if (n > 0)
	{
		ptr = buffer;
		for (i = 0; i < n; i++)
		{
			if (zmin >(double) ptr[1] / UINT_MAX) {
				zmin = (double)ptr[1] / UINT_MAX;
				objid = ptr[3];
			}
			ptr += 3 + ptr[0]; // ptr[0] contains the number of names (usually 1); 3 corresponds to numnomes, zmin e zmax
		}
	}


	glMatrixMode(GL_PROJECTION); //puts back projection matrix
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	return objid;
}

void mouse(int btn, int mouseState, int x, int y){
	switch (btn) {
	case GLUT_RIGHT_BUTTON:
		if (mouseState == GLUT_DOWN){
			state.xMouse = x;
			state.yMouse = y;
			if (glutGetModifiers() & GLUT_ACTIVE_CTRL)
				glutMotionFunc(motionZoom);
			else
				glutMotionFunc(motionRotate);
			cout << "Right down\n";
		}
		else{
			glutMotionFunc(NULL);
			cout << "Right up\n";
		}
		break;
	case GLUT_LEFT_BUTTON:
		if (mouseState == GLUT_DOWN){
			state.translateAxis = picking(x, y);
			cout << "Left down - object:" << state.translateAxis << endl;
		}
		else{
			cout << "Left up\n";
		}
		break;
	}
}

void main(int argc, char **argv)
{
	glutInit(&argc, argv);

	/* need both double buffering and z buffer */

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 450);
	glutCreateWindow("OpenGL");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutSpecialFunc(Special);
	glutSpecialUpFunc(SpecialUp);
	glutMouseFunc(mouse);

	glutTimerFunc(state.timer, Timer, 0);

	myInit();

	printHelp();

	glutMainLoop();
}
