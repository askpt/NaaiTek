#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>     
#include <GL\glut.h>
#include <iostream>
#include "graphs.h"

using namespace std;

#define degrees(X) (double)((X)*180/M_PI)
#define rad(X)   (double)((X)*M_PI/180)

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


typedef struct Camera{
	GLfloat fov;
	GLdouble dir_lat;
	GLdouble dir_long;
	GLfloat dist;
	Vertex center;

}Camera;

typedef struct State{
	Camera		camera;
	int			xMouse, yMouse;
	GLboolean	light;
	GLboolean	showNormals;
	GLint		lightViewer;
	GLint		translateAxis;
	GLdouble	axis[3];
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
	state.camera.dist = 100;
	state.axis[0] = 0;
	state.axis[1] = 0;
	state.axis[2] = 0;
	state.camera.center[0] = 0;
	state.camera.center[1] = 0;
	state.camera.center[2] = 0;
	state.light = GL_FALSE;
	state.showNormals = GL_FALSE;
	state.lightViewer = 1;
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

	glClearColor(0.0, 0.0, 0.0, 0.0);

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
	model.quad = gluNewQuadric();
	gluQuadricDrawStyle(model.quad, GLU_FILL);
	gluQuadricNormals(model.quad, GLU_OUTSIDE);

	readGraph();
}

void printHelp(void)
{
	printf("\n\nDraw the graph\n");
	printf("h,H - Help \n");
	printf("i,I - Reset Values \n");
	printf("******* Diverse ******* \n");
	printf("l,L - Alternate light calculus between Z and eye (GL_LIGHT_MODEL_LOCAL_VIEWER)\n");
	printf("k,K - Alternate camera light with global light \n");
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

const GLfloat red_light[] = { 1.0, 0.0, 0.0, 1.0 };
const GLfloat green_light[] = { 0.0, 1.0, 0.0, 1.0 };
const GLfloat blue_light[] = { 0.0, 0.0, 1.0, 1.0 };
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

void drawGround(){
#define STEP 10
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	for (int i = -300; i < 300; i += STEP)
	for (int j = -300; j < 300; j += STEP){
		glVertex2f(i, j);
		glVertex2f(i + STEP, j);
		glVertex2f(i + STEP, j + STEP);
		glVertex2f(i, j + STEP);
	}
	glEnd();
}

void CrossProduct(GLdouble v1[], GLdouble v2[], GLdouble cross[])
{
	cross[0] = v1[1] * v2[2] - v1[2] * v2[1];
	cross[1] = v1[2] * v2[0] - v1[0] * v2[2];
	cross[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

GLdouble VectorNormalize(GLdouble v[])
{
	int		i;
	GLdouble	length;

	if (fabs(v[1] - 0.000215956) < 0.0001)
		i = 1;

	length = 0;
	for (i = 0; i < 3; i++)
		length += v[i] * v[i];
	length = sqrt(length);
	if (length == 0)
		return 0;

	for (i = 0; i < 3; i++)
		v[i] /= length;

	return length;
}

void drawNormal(GLdouble x, GLdouble y, GLdouble z, GLdouble normal[], material_type mat){

	switch (mat){
	case red_plastic:
		glColor3f(1, 0, 0);
		break;
	case blue:
		glColor3f(0, 0, 1);
		break;
	case emerald:
		glColor3f(0, 1, 0);
		break;
	default:
		glColor3f(1, 1, 0);
	}
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glTranslated(x, y, z);
	glScaled(0.4, 0.4, 0.4);
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3dv(normal);
	glEnd();
	glPopMatrix();
	glEnable(GL_LIGHTING);
}

void drawCircle(GLint n, GLfloat x0, GLfloat y0, GLfloat z0, GLfloat r)
{
	GLint i;
	GLfloat t, x, y;
	t = 0.0;

	glBegin(GL_POLYGON);
	glNormal3f(0, 0, 1);
	for (i = 0; i < n; i++)
	{
		x = r * cos(t) + x0;
		y = r * sin(t) + y0;
		glVertex3f(x, y, z0);
		t += 2.0*M_PI / n;
	}
	glEnd();
}

//angle: 0 -> inclina norte-sul; 1 -> inclina este-oeste; default -> não inclina;
void drawFloor(GLfloat xi, GLfloat yi, GLfloat zi, GLfloat xf, GLfloat yf, GLfloat zf, int angle){
	GLdouble v1[3], v2[3], cross[3];
	GLdouble length;
	v1[0] = xf - xi;
	v1[1] = 0;
	v2[0] = 0;
	v2[1] = yf - yi;

	switch (angle) {
	case 0:
		v1[2] = 0;
		v2[2] = zf - zi;
		CrossProduct(v1, v2, cross);
		//printf("cross x=%lf y=%lf z=%lf",cross[0],cross[1],cross[2]);
		length = VectorNormalize(cross);
		//printf("Normal x=%lf y=%lf z=%lf length=%lf\n",cross[0],cross[1],cross[2]);

		material(red_plastic);
		glBegin(GL_QUADS);
		glNormal3dv(cross);
		glVertex3f(xi, yi, zi);
		glVertex3f(xf, yi, zi);
		glVertex3f(xf, yf, zf);
		glVertex3f(xi, yf, zf);
		glEnd();
		if (state.showNormals) {
			drawNormal(xi, yi, zi, cross, red_plastic);
			drawNormal(xf, yi, zi, cross, red_plastic);
			drawNormal(xf, yf, zf, cross, red_plastic);
			drawNormal(xi, yi, zf, cross, red_plastic);
		}
		break;
	case 1:
		v1[2] = zf - zi;
		v2[2] = 0;
		CrossProduct(v1, v2, cross);
		//printf("cross x=%lf y=%lf z=%lf",cross[0],cross[1],cross[2]);
		length = VectorNormalize(cross);
		//printf("Normal x=%lf y=%lf z=%lf length=%lf\n",cross[0],cross[1],cross[2]);
		material(red_plastic);
		glBegin(GL_QUADS);
		glNormal3dv(cross);
		glVertex3f(xi, yi, zi);
		glVertex3f(xf, yi, zf);
		glVertex3f(xf, yf, zf);
		glVertex3f(xi, yf, zi);
		glEnd();
		if (state.showNormals) {
			drawNormal(xi, yi, zi, cross, red_plastic);
			drawNormal(xf, yi, zf, cross, red_plastic);
			drawNormal(xf, yf, zf, cross, red_plastic);
			drawNormal(xi, yi, zi, cross, red_plastic);
		}
		break;
	default:
		cross[0] = 0;
		cross[1] = 0;
		cross[2] = 1;
		material(red_plastic);
		glBegin(GL_QUADS);
		glNormal3f(0, 0, 1);
		glVertex3f(xi, yi, zi);
		glVertex3f(xf, yi, zf);
		glVertex3f(xf, yf, zf);
		glVertex3f(xi, yf, zi);
		glEnd();

		if (state.showNormals) {
			drawNormal(xi, yi, zi, cross, red_plastic);
			drawNormal(xf, yi, zf, cross, red_plastic);
			drawNormal(xf, yf, zf, cross, red_plastic);
			drawNormal(xi, yi, zi, cross, red_plastic);
		}
		break;
	}
}

void drawNode(int no){
	Path arco = paths[0];
	Node *noi = &nodes[no], *nof;

	material(blue);
	for (int i = 0; i < numNodes; i++){
		drawCircle(32, nodes[no].x, nodes[no].y, nodes[no].z + INFINITESIMAL, nodes[no].width);
	}

}

void drawPath(Path arco){
	Node *noi, *nof;

	if (nodes[arco.nodei].x == nodes[arco.nodef].x){
		// vertical path
		if (nodes[arco.nodei].y < nodes[arco.nodef].y){
			noi = &nodes[arco.nodei];
			nof = &nodes[arco.nodef];
		}
		else{
			nof = &nodes[arco.nodei];
			noi = &nodes[arco.nodef];
		}

		//nof->x + 0.5*arco.width, nof->y - nof->width,     nof->z  --END LOCATION BEFORE
		//nof->x - 0.5*arco.width, nof->y - 0.5*nof->width, nof->z  --END LOCATION AFTER

		//noi->x - 0.5*arco.width, noi->y + noi->width,     noi->z  --BEGIN LOCATION BEFORE
		//noi->x + 0.5*arco.width, noi->y + 0.5*noi->width, noi->z  --BEGIN LOCATION AFTER

		//drawConnectionElement (at Beginnig)
		drawFloor(noi->x + 0.5*arco.width, noi->y + 0.5*noi->width, noi->z, noi->x - 0.5*arco.width, noi->y + noi->width, noi->z, 2);
		//drawConnectionElement (at End)
		drawFloor(nof->x + 0.5*arco.width, nof->y - nof->width, nof->z, nof->x - 0.5*arco.width, nof->y - 0.5*nof->width, nof->z, 2);
		//drawRamp
		drawFloor(noi->x - 0.5*arco.width, noi->y + noi->width, noi->z, nof->x + 0.5*arco.width, nof->y - nof->width, nof->z, 0);
	}
	else{
		if (nodes[arco.nodei].y == nodes[arco.nodef].y){
			//horizontal path
			if (nodes[arco.nodei].x < nodes[arco.nodef].x){
				noi = &nodes[arco.nodei];
				nof = &nodes[arco.nodef];
			}
			else{
				nof = &nodes[arco.nodei];
				noi = &nodes[arco.nodef];
			}


			//nof->x - nof->width,      nof->y + 0.5*arco.width, nof->z  --END LOCATION BEFORE
			//nof->x + 0.25*nof->width, nof->y - 0.5*arco.width, nof->z  --END LOCATION AFTER

			//noi->x + noi->width,      noi->y - 0.5*arco.width, noi->z  --BEGIN LOCATION BEFORE
			//noi->x - 0.25*noi->width, noi->y + 0.5*arco.width, noi->z  --BEGIN LOCATION AFTER

			//drawConnectionElement (at Beginnig)
			drawFloor(noi->x - 0.25*noi->width, noi->y + 0.5*arco.width, noi->z, noi->x + noi->width, noi->y - 0.5*arco.width, noi->z, 2);
			//drawConnectionElement (at End)
			drawFloor(nof->x - nof->width, nof->y + 0.5*arco.width, nof->z, nof->x + 0.25*nof->width, nof->y - arco.width*0.5, nof->z, 2);
			//drawRamp
			drawFloor(noi->x + noi->width, noi->y - 0.5*arco.width, noi->z, nof->x - nof->width, nof->y + 0.5*arco.width, nof->z, 1);
		}
		else{
			//to do: diagonal path
			cout << "Diagonal Path... Not Implemented\n";
		}
	}
}

void drawGraph(){
	glPushMatrix();
	glTranslatef(0, 0, 0.05);
	glScalef(5, 5, 5);
	material(red_plastic);
	for (int i = 0; i < numNodes; i++){
		glPushMatrix();
		glTranslatef(nodes[i].x, nodes[i].y, nodes[i].z + 0.25);
		glPopMatrix();
		glNormal3f(0, 0, 1);
		drawNode(i);
	}
	material(emerald);
	for (int i = 0; i < numPaths; i++)
		drawPath(paths[i]);
	glPopMatrix();
}

void drawAxis(){
	gluCylinder(model.quad, 0.5, 0.5, 20, 16, 15);
	glPushMatrix();
	glTranslatef(0, 0, 20);
	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	gluDisk(model.quad, 0.5, 2, 16, 6);
	glPopMatrix();
	gluCylinder(model.quad, 2, 0, 5, 16, 15);
	glPopMatrix();
}

#define AXIS_X		1
#define AXIS_Y		2
#define AXIS_Z		3

void drawDragPlan(int axis){
	glPushMatrix();
	glTranslated(state.axis[0], state.axis[1], state.axis[2]);
	switch (axis) {
	case AXIS_Y:
		if (abs(state.camera.dir_lat)<M_PI / 4)
			glRotatef(-90, 0, 0, 1);
		else
			glRotatef(90, 1, 0, 0);
		material(red_plastic);
		break;
	case AXIS_X:
		if (abs(state.camera.dir_lat)>M_PI / 6)
			glRotatef(90, 1, 0, 0);
		material(blue);
		break;
	case AXIS_Z:
		if (abs(cos(state.camera.dir_long)) > 0.5)
			glRotatef(90, 0, 0, 1);

		material(emerald);
		break;
	}
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glVertex3f(-100, 0, -100);
	glVertex3f(100, 0, -100);
	glVertex3f(100, 0, 100);
	glVertex3f(-100, 0, 100);
	glEnd();
	glPopMatrix();
}

void drawManyAxis(){

	glPushMatrix();
	glTranslated(state.axis[0], state.axis[1], state.axis[2]);
	material(emerald);
	glPushName(AXIS_Z);
	drawAxis();
	glPopName();
	glPushName(AXIS_Y);
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	material(red_plastic);
	drawAxis();
	glPopMatrix();
	glPopName();
	glPushName(AXIS_X);
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	material(blue);
	drawAxis();
	glPopMatrix();
	glPopName();
	glPopMatrix();
}

void setCamera(){
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

void display(void)
{


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	setCamera();

	material(slate);
	drawGround();


	drawManyAxis();

	drawGraph();

	if (state.translateAxis) {
		// draw translation plan
		cout << "Translate... " << state.translateAxis << endl;
		drawDragPlan(state.translateAxis);

	}

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
		state.camera.dist -= 1;
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
		state.camera.dist += 1;
		glutPostRedisplay();
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

void motionDrag(int x, int y){
	GLuint buffer[100];
	GLint vp[4];
	GLdouble proj[16], mv[16];
	int n;
	GLdouble newx, newy, newz;

	glSelectBuffer(100, buffer);
	glRenderMode(GL_SELECT);
	glInitNames();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix(); // guarda a projecção
	glLoadIdentity();
	setProjection(x, y, GL_TRUE);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	setCamera();
	drawDragPlan(state.translateAxis);

	n = glRenderMode(GL_RENDER);
	if (n > 0) {
		glGetIntegerv(GL_VIEWPORT, vp);
		glGetDoublev(GL_PROJECTION_MATRIX, proj);
		glGetDoublev(GL_MODELVIEW_MATRIX, mv);
		gluUnProject(x, glutGet(GLUT_WINDOW_HEIGHT) - y, (double)buffer[2] / UINT_MAX, mv, proj, vp, &newx, &newy, &newz);
		printf("Novo x:%lf, y:%lf, z:%lf\n\n", newx, newy, newz);
		switch (state.translateAxis) {
		case AXIS_X:
			state.axis[0] = newx;
			//state.axis[1]=newy;
			break;
		case AXIS_Y:
			state.axis[1] = newy;
			//state.axis[2]=newz;
			break;
		case AXIS_Z:
			//state.axis[0]=newx;
			state.axis[2] = newz;
			break;
		}
		glutPostRedisplay();
	}


	glMatrixMode(GL_PROJECTION); //repõe matriz projecção
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
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
	drawManyAxis();

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
			cout << "Left down\n";
		}
		else{
			glutMotionFunc(NULL);
			cout << "Left up\n";
		}
		break;
	case GLUT_LEFT_BUTTON:
		if (mouseState == GLUT_DOWN){
			state.translateAxis = picking(x, y);
			if (state.translateAxis)
				glutMotionFunc(motionDrag);
			cout << "Right down - object:" << state.translateAxis << endl;
		}
		else{
			if (state.translateAxis != 0) {
				state.camera.center[0] = state.axis[0];
				state.camera.center[1] = state.axis[1];
				state.camera.center[2] = state.axis[2];
				glutMotionFunc(NULL);
				state.translateAxis = 0;
				glutPostRedisplay();
			}
			cout << "Right up\n";
		}
		break;
	}
}

void main(int argc, char **argv)
{
	glutInit(&argc, argv);

	/* need both double buffering and z buffer */

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutCreateWindow("OpenGL");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(Special);
	glutMouseFunc(mouse);

	myInit();

	printHelp();

	glutMainLoop();
}
