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

// lights and materials

const GLfloat mat_ambient[][4] = { { 0.33, 0.22, 0.03, 1.0 },	// brass
{ 0.1, 0.0, 0.0 },			// red plastic
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
	GLboolean   w, a, s, d, up, down, tictactoe;
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
	GLint		lightViewer;
	GLint		pickedObjID;
	Keys_t		keys;
	GLint		timer;
}State;

typedef struct Model {
	GLfloat g_pos_light1[4];
	GLfloat g_pos_light2[4];

	GLfloat scale;

	// insert username here after login
	string regUser;
	GLUquadricObj* pQuadric;
}Model;

State state;
Model model;

/* initial values of the state*/
void initState(){
	state.camera.dir_lat = -0.2;
	state.camera.dir_long = -M_PI / 4;
	state.camera.fov = 60;
	state.camera.dist = 5;
	state.camera.center[0] = 0;
	state.camera.center[1] = 0;
	state.camera.center[2] = nodes[0].z + state.camera.dimension + 2;
	state.camera.dimension = 2;
	state.camera.velv = 10;
	state.camera.velh = 10;
	state.light = GL_FALSE;
	state.lightViewer = 1;
	state.timer = 100;
}

/* initial values of the model*/
void initModel(){
	model.scale = 0.2;

	model.g_pos_light1[0] = -5.0;
	model.g_pos_light1[1] = 5.0;
	model.g_pos_light1[2] = 5.0;
	model.g_pos_light1[3] = 0.0;
	model.g_pos_light2[0] = 5.0;
	model.g_pos_light2[1] = -15.0;
	model.g_pos_light2[2] = 5.0;
	model.g_pos_light2[3] = 0.0;

	model.regUser;
	model.pQuadric = gluNewQuadric();
}

/* Call both initiators, and enable necessary methods. */
void myInit(string user)
{

	GLfloat AmbientLight[] = { 0.5, 0.5, 0.5, 0.0 };

	glClearColor(0.55, 0.75, 1.0, 0.0);

	glEnable(GL_SMOOTH); /*enable smooth shading */
	glEnable(GL_LIGHTING); /* enable lighting */
	glEnable(GL_DEPTH_TEST); /* enable z buffer */
	glEnable(GL_NORMALIZE);

	glDepthFunc(GL_LESS);


	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, AmbientLight);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, state.lightViewer);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	model.regUser = user;

	//readGraph();
	readGraphUser(model.regUser);

	initModel();
	initState();
	state.keys.tictactoe = 0;

}

/* Print the Help Menu to help us debugging mainly */
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
	printf("e,E - PolygonMode Fill \n");
	printf("q,Q - PolygonMode Wireframe \n");
	printf("r,R - PolygonMode Point \n");
	printf("******* Graphs ******* \n");
	printf("F1  - Save graph to file \n");
	printf("F2  - Read graph from file \n");
	printf("******* Camera ******* \n");
	printf("WASD - Control free flight\n");
	printf("Right Buttton  - Rotate camera\n");
	printf("ESC - Leave\n");
}


/* Attributes the color to the material,
   deppending on the material and the light that is shining in it */
void material(enum material_type mat)
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient[mat]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse[mat]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular[mat]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess[mat]);
}

const GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };

/* Puts the light in-scene. */
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

/* Draws the node in the form of a sphere */
void drawNode(Node node)
{
	GLfloat x0 = node.x;
	GLfloat y0 = node.y;
	GLfloat z0 = node.z;

	GLfloat r = K_SPHERE * node.width / 2.0;

	glPushMatrix();
	glTranslatef(x0, y0, z0);

	gluSphere(model.pQuadric, r, 32, 20);

	glPopMatrix();
}

/* Draws the path in the form of a cylinder */
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

	glPushMatrix();
	glTranslatef(xi, yi, zi);
	glRotatef(aij, 0, 0, 1.0);
	glRotatef(90 - bij, 0.0, 1.0, 0.0);


	gluCylinder(model.pQuadric, wij / 2.0, wij / 2.0, sij, 32, 4);

	glPopMatrix();
}

/* Calls the drawNode and the drawPath method
 * One time for each existant node and path*/
void drawGraph(){
	int nodeID = _MAX_NODES_GRAPH;
	for (int i = 0; i < numNodes; i++){
		(i == 0) ? material(blue) : material(slate);
		glPushName(nodeID + i);
		drawNode(nodes[i]);
		glPopName();
	}
	material(red_plastic);
	int pathID = _MAX_PATHS_GRAPH;
	for (int i = 0; i < numPaths; i++){
		glPushName(pathID + i);
		Node* noi = &nodes[paths[i].connection.nodei];
		Node* nof = &nodes[paths[i].connection.nodef];
		//drawPath(*noi, *nof, paths[i]);
		drawPath(*nof, *noi, paths[i]);
		glPopName();
	}
}

/* Detects if the camera is going to colide with any part of the graph */
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



/* Used to see if the camera colides with some part of the graph */
void Timer(int value)
{
	static int time;

	GLuint curr = glutGet(GLUT_ELAPSED_TIME);

	glutTimerFunc(state.timer, Timer, 0);

	double vel = 10, k = 0.1;

	GLdouble xp = state.camera.center[0];
	GLdouble yp = state.camera.center[1];
	GLdouble zp = state.camera.center[2];

	if (state.keys.w)
	{
		xp = state.camera.center[0] + k * state.camera.velh * cos(state.camera.dir_long);
		yp = state.camera.center[1] + k * state.camera.velh * sin(state.camera.dir_long);
		zp = state.camera.center[2] + k * state.camera.velv * state.camera.dir_lat;

	}
	else if (state.keys.s)
	{
		xp = state.camera.center[0] - k * state.camera.velh * cos(state.camera.dir_long);
		yp = state.camera.center[1] - k * state.camera.velh * sin(state.camera.dir_long);
		zp = state.camera.center[2] - k * state.camera.velv * state.camera.dir_lat;
	}

	if (state.keys.down)
	{
		zp = state.camera.center[2] - k * vel / 4.0;
	}
	else if (state.keys.up)
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

	if (state.keys.a)
	{
		state.camera.dir_long += rad(5);
	}
	if (state.keys.d)
	{
		state.camera.dir_long -= rad(5);

	}

	if (state.keys.tictactoe)
	{
	}
	glutPostRedisplay();
}

/* Sets the light either to camera or eye*/
/* Sets the camera positioning */
void setCamera(){
	if (!state.light)
		putLights((GLfloat*)white_light);
	glLoadIdentity();
	glTranslated(0.0, 0.0, -state.camera.dist);
	glRotated(degrees(-M_PI / 2.0 - state.camera.dir_lat), 1.0, 0.0, 0.0);
	glRotated(degrees(M_PI / 2.0 - state.camera.dir_long), 0.0, 0.0, 1.0);
	glTranslated(-state.camera.center[0], -state.camera.center[1], -state.camera.center[2]);
	if (state.light)
		putLights((GLfloat*)white_light);
}

/* Ran in a loop to update the screen picture */
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	setCamera();

	drawGraph();

	glFlush();
	glutSwapBuffers();
}

/* Where the keyboard keys use is defined */
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
	case 'q':
	case 'Q':
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glutPostRedisplay();
		break;
	case 'r':
	case 'R':
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		glutPostRedisplay();
		break;
	case 'e':
	case 'E':
		glEnable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glutPostRedisplay();
		break;
	case 'i':
	case 'I':
		initState();
		initModel();
		glutPostRedisplay();
		break;
	case 'w':
	case 'W':
		state.keys.w = GL_TRUE;
		break;
	case 'a':
	case 'A':
		state.keys.a = GL_TRUE;
		break;
	case 's':
	case 'S':
		state.keys.s = GL_TRUE;
		break;
	case 'd':
	case 'D':
		state.keys.d = GL_TRUE;
		break;
	}
}

/* Checks if this keys stopped being continously pressed */
void keyboardUp(unsigned char key, int x, int y)
{

	switch (key)
	{
	case 'w':
	case 'W':
		state.keys.w = GL_FALSE;
		break;
	case 'a':
	case 'A':
		state.keys.a = GL_FALSE;
		break;
	case 's':
	case 'S':
		state.keys.s = GL_FALSE;
		break;
	case 'd':
	case 'D':
		state.keys.d = GL_FALSE;
		break;
	}
}

/* Checks if this special keys stopped being continously pressed */
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
	}

}

/* Where the keyboard special keys use is defined */
void Special(int key, int x, int y){

	switch (key){
	case GLUT_KEY_F1:
		saveGraph();
		break;
	case GLUT_KEY_F2:
		readGraph();
		glutPostRedisplay();
		break;
	case GLUT_KEY_F3:
		state.keys.tictactoe = GL_TRUE;
		
		break;
	case GLUT_KEY_UP:
		state.keys.up = GL_TRUE;
		break;
	case GLUT_KEY_DOWN:
		state.keys.down = GL_TRUE;
		break;
	}

	if (state.keys.tictactoe)
	{
		state.keys.tictactoe = GL_FALSE;
		char ret[100];
		sprintf_s(ret, "TicTacToe %s %s", "T1", "T2");
		system(ret);
	}
}

/* Checks if it's a valid picking place and sets the projection */
void setProjection(int x, int y, GLboolean picking){
	glLoadIdentity();
	if (picking) { // if its in "picking mode", reads the viewport and defines picking zone
		GLint vport[4];
		glGetIntegerv(GL_VIEWPORT, vport);
		gluPickMatrix(x, glutGet(GLUT_WINDOW_HEIGHT) - y, 4, 4, vport); // Inverts mouse Y axis, to correspond to window
	}

	gluPerspective(state.camera.fov, (GLfloat)glutGet(GLUT_WINDOW_WIDTH) / glutGet(GLUT_WINDOW_HEIGHT), 1, 500);

}

/* Reshapes the drawing depending on the projection, viewport... */
void myReshape(int w, int h){
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	setProjection(0, 0, GL_FALSE);
	glMatrixMode(GL_MODELVIEW);
}

/* Used to rotate the camera */
void motionRotate(int x, int y){
#define DRAG_SCALE	0.01
	double lim = M_PI / 4 - 0.1;
	state.camera.dir_long += (state.xMouse - x)*DRAG_SCALE;
	state.camera.dir_lat += (state.yMouse - y)*DRAG_SCALE*0.5;
	if (state.camera.dir_lat > lim)
		state.camera.dir_lat = lim;
	else
	if (state.camera.dir_lat < -lim)
		state.camera.dir_lat = -lim;
	state.xMouse = x;
	state.yMouse = y;
	glutPostRedisplay();
}

/* Used to know what object was picked by the user */
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

	drawGraph();

	n = glRenderMode(GL_RENDER);
	cout << "Hits:" << n << endl;

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

void motionPicking(int x, int y){
	state.pickedObjID = picking(x, y);
	if (state.pickedObjID >= _MAX_NODES_GRAPH)
	{
		wcout << L"Node: " + nodes[state.pickedObjID - _MAX_NODES_GRAPH].user->name << endl;
	}
	else
	{
		cout << "Left down - object:" << state.pickedObjID << endl;
	}
}

/* Used to see what mouse action will be executed */
void mouse(int btn, int mouseState, int x, int y){
	switch (btn) {
	case GLUT_RIGHT_BUTTON:
		if (mouseState == GLUT_DOWN){
			state.xMouse = x;
			state.yMouse = y;
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
			//glutMotionFunc(motionPicking);
			state.pickedObjID = picking(x, y);
			if (state.pickedObjID >= _MAX_PATHS_GRAPH)
			{
				cout << "Path Strenght ";
				cout << paths[state.pickedObjID - _MAX_PATHS_GRAPH].connection.strength << endl;
				wcout << L"User1: " + nodes[paths[state.pickedObjID - _MAX_PATHS_GRAPH].connection.nodei].user->name << endl;
				wcout << L"User2: " + nodes[paths[state.pickedObjID - _MAX_PATHS_GRAPH].connection.nodef].user->name << endl;

			}
			else if (state.pickedObjID >= _MAX_NODES_GRAPH)
			{
				wcout << L"Node: " + nodes[state.pickedObjID - _MAX_NODES_GRAPH].user->name << endl;
			}
			else
			{
				cout << "Left down - object:" << state.pickedObjID << endl;
			}
		}
		else{
			glutMotionFunc(NULL);
			cout << "Left up\n";
		}
		break;
	}
}

/* Main, runs in a loop. */
void main(int argc, char **argv)
{
	string username, pass;

	cout << "username" << endl;
	cin >> username;
	cout << "password" << endl;
	cin >> pass;


	if (TryAuth(username, pass))
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

		myInit(username);
		printHelp();

		glutMainLoop();

	}

}
