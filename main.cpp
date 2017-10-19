#include <stdio.h> 
#include <stdlib.h>
/***freeglut***/
#include <GL\freeglut.h>
#include <iostream>
#include <math.h>
#include <time.h>
#include <string>
#include <vector>
#include<fstream>
#include<sstream>
using namespace std;
#define PI 3.14159265 
string line;
vector<float> v;
vector<string> f;
string Mode = "points";
string ColorMode = "";
float xMin = 0;
float xMax = 0;
float yMin = 0;
float yMax = 0;
float zMin = 0;
float zMax = 0;
float tx, ty, tz;
float mx = 0, my = 0;
float thetaX, thetaY, thetaZ, theta;

int width = 400;
int height = 400;
void openFile(string);
void ChangeSize(int, int);
void RenderScene(void);
void myKeyboard(unsigned char, int, int);
void mySpecialKey(int, int, int);
void SetupRC();
void mouse(int btn, int state, int x, int y);
void setupMenus();
void menu(int);
void createObj(string);
GLfloat multiMatrix[16] = { 1,0,0,0
,0,1,0,0
,0,0,1,0
,0,0,0,1 };
GLfloat R = 0, G = 0, B = 0;
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	//  Display Nothing

	glFlush();
}
int main(int argc, char** argv)
{
	//These are variable that you will need 
	//to move your cube

	tx = 0; ty = 0; tz = 0;
	thetaX = 0; thetaY = 0; thetaZ = 0; theta = 0;
	//

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(600, 80);
	glutCreateWindow("Moving Cube");

	setupMenus();


	SetupRC();
	glutReshapeFunc(ChangeSize);


	glutDisplayFunc(RenderScene);
	glutKeyboardFunc(myKeyboard);
	glutSpecialFunc(mySpecialKey);
	glutMouseFunc(mouse);

	glutMainLoop();

	return 0;
}

void mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

		int h = glutGet(GLUT_WINDOW_HEIGHT);
		int w = glutGet(GLUT_WINDOW_WIDTH);
		int dis = w < h ? w : h;
		//printf("h=%d \n",h);
		//printf("w=%d \n", w);
		mx = x - ((0 + 1)*(dis / 2));
		my = (y - ((0 + 1)*(dis / 2)))*-1;

		printf("x = %d\n", x);
		printf("y = %d\n", y);
		printf("mx = %f\n", mx);
		printf("my = %f\n", my);
		glFlush();
	}
}
// This function does any needed initialization on the rendering
// context.  Here it sets up and initializes the lighting for
// the scene.
void MatrixReset(void) {
	for (int i = 0; i <= 4; i++) {
		for (int j = 0; j <= 4; j++) {
			if (i == j) {
				multiMatrix[i * 4 + j] = 1;
			}
			else {
				multiMatrix[i * 4 + j] = 0;
			}
		}
	}
}
void MX_Translate(GLfloat x, GLfloat y, GLfloat z) {
	MatrixReset();
	multiMatrix[12] = x;
	multiMatrix[13] = y;
	multiMatrix[14] = z;
	glMultMatrixf(multiMatrix);

}
void MX_Rotate(float angle, GLfloat  x, GLfloat  y, GLfloat  z) {
	MatrixReset();
	double magnitude = sqrt(x*x + y*y + z*z);
	if (magnitude != 0) {

		x /= magnitude;

		y /= magnitude;

		z /= magnitude;
	}
	float  Cos = cos(angle * PI / 180);
	float  Sin = sin(angle * PI / 180);
	multiMatrix[0] = Cos + (1 - Cos)*x*x;
	multiMatrix[1] = (1 - Cos)*y*x + Sin * z;
	multiMatrix[2] = (1 - Cos)*z*x - Sin * y;

	multiMatrix[4] = (1 - Cos)*x*y - Sin * z;
	multiMatrix[5] = Cos + (1 - Cos)*y*y;
	multiMatrix[6] = (1 - Cos)*z*y + Sin * x;

	multiMatrix[8] = (1 - Cos)*x*z + Sin * y;
	multiMatrix[9] = (1 - Cos)*y*z - Sin * x;
	multiMatrix[10] = Cos + (1 - Cos)*z*z;

	glMultMatrixf(multiMatrix);
}
void SetupRC()
{
	// Light values and coordinates
	GLfloat  whiteLight[] = { 0.45f, 0.45f, 0.45f, 1.0f };
	GLfloat  sourceLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
	GLfloat	 lightPos[] = { 0.f, 25.0f, 20.0f, 0.0f };

	// Enable lighting
	glEnable(GL_LIGHTING);

	// Setup and enable light 0
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, sourceLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sourceLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);

	// Enable color tracking
	glEnable(GL_COLOR_MATERIAL);

	// Set Material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_DEPTH_TEST);

}

void ChangeSize(int w, int h)
{
	int dis = w < h ? w : h;
	glViewport(0, 0, dis, dis);
	glMatrixMode(GL_PROJECTION); // load the projection matrix
	glLoadIdentity();
	//glOrtho(-2,2,-2,2,-10,20);
	glOrtho(xMin, xMax, yMin, yMax, zMin * 10, zMax * 30);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void RenderScene(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); // load the modelview matrix
	glLoadIdentity();
	gluLookAt(0, 0, 10.0f, 0, 0, 0, 0, 1, 0);

	//perform transformation for the cube
	//use:
	//glRotatef(theta, x, y, z);
	//glTranslatef(tx, ty, tz);

	//draw X-axis, Y-axis and Z-axis
	//use:
	//glColor3f(1, 0, 0);
	//glBegin(GL_LINES);
	//glVertex3f(-50, 0, 0);
	//glVertex3f(50, 0, 0);
	//glEnd();
	//glColor3f(0, 1, 0);
	//glBegin(GL_LINES);
	//glVertex3f(0, -50, 0);
	//glVertex3f(0, 50, 0);
	//glEnd();
	//glColor3f(0, 0, 1);
	//glBegin(GL_LINES);
	//glVertex3f(0, 0, -50);
	//glVertex3f(0, 0, 50);
	//glEnd();
	//glColor3f(1, 1, 1);
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex3f(mx, my, 0);
	glVertex3f(0, 0, 0);
	glEnd();
	MX_Rotate(thetaX, 1, 0, 0);
	MX_Rotate(thetaY, 0, 1, 0);
	MX_Rotate(thetaZ, 0, 0, 1);
	MX_Rotate(theta, mx / 10, my / 10, 0);
	MX_Translate(tx, ty, tz);



	//cube
	//glColor3f( 1, 1, 0);
	//glutSolidCube(3);
	//glutSwapBuffers(); 
	createObj(Mode);
	glutSwapBuffers();
}




void myKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'r':
		//reset translation & rotation
		tx = 0;
		ty = 0;
		thetaX = 0;
		thetaY = 0;
		thetaZ = 0;
		theta = 0;
		break;
	case 'a':
		//change the rotation angle thetaX along x-axis
		thetaX = thetaX + 2;
		break;
	case 'd':
		//change the rotation angle thetaX along x-axis
		thetaX = thetaX - 2;
		break;
	case 'w':
		//change the rotation angle thetaY along y-axis
		thetaY = thetaY + 2;
		break;
	case 's':
		//change the rotation angle thetaY along y-axis
		thetaY = thetaY - 2;
		break;
	case 'z':
		//change the rotation angle thetaZ along z-axis
		thetaZ = thetaZ + 2;
		break;
	case 'x':
		//change the rotation angle thetaZ along z-axis
		thetaZ = thetaZ - 2;
		break;
	case 'c':
		theta = theta + 2;
		break;
	case 'v':
		theta = theta - 2;
		break;
	default:
		break;
	}
	glutPostRedisplay();

}


void mySpecialKey(int key, int x, int y)
{
	switch (key)
	{

	case GLUT_KEY_LEFT:
		//change the translation along x-axis
		tx = tx - 2;
		break;
	case GLUT_KEY_RIGHT:
		//change the translation along x-axis
		tx = tx + 2;
		break;
	case GLUT_KEY_UP:
		//change the translation along y-axis
		ty = ty + 2;
		break;
	case GLUT_KEY_DOWN:
		//change the translation along y-axis
		ty = ty - 2;
		break;
	default:
		break;

	}
	glutPostRedisplay();

}
void createObj(string m) {
	if (m == "points") {
		for (int i = 0; i < v.size(); i += 3) {
			glBegin(GL_POINTS);
			if (ColorMode == "AllRandom") {
				menu(50);
			}
			glColor3f(R, G, B);
			glVertex3f(v[i], v[i + 1], v[i + 2]);
			glEnd();
		}
	}
	else if (m == "lines") {
		for (int i = 0; i < f.size(); i++) {
			vector<int> f1;
			istringstream iss(f[i]);
			string token;
			while (std::getline(iss, token, ' '))
			{
				stringstream ss;
				int temp;
				ss << token;
				ss >> temp;
				//printf("%d ",temp);
				f1.push_back(temp);
			}
			glBegin(GL_LINE_STRIP);
			if (ColorMode == "AllRandom") {
				menu(50);
			}
			glColor3f(R, G, B);
			for (int i = 0; i < f1.size(); i++) {
				//cout << f1[i] << " ";
				int px = (f1[i] - 1) * 3;
				int py = (f1[i] - 1) * 3 + 1;
				int pz = (f1[i] - 1) * 3 + 2;
				//cout << v[px] << " " << v[py] << " " << v[pz] << " " << endl;
				glVertex3f(v[px], v[py], v[pz]);
			}
			glEnd();
			//cout << endl;
			f1.clear();
		}
	}
	else if (m == "faces") {
		for (int i = 0; i < f.size(); i++) {
			vector<int> f1;
			istringstream iss(f[i]);
			string token;
			while (std::getline(iss, token, ' '))
			{
				stringstream ss;
				int temp;
				ss << token;
				ss >> temp;
				//printf("%d ",temp);
				f1.push_back(temp);
			}
			glBegin(GL_POLYGON);
			if (ColorMode == "AllRandom") {
				menu(50);
			}
			glColor3f(R, G, B);
			for (int i = 0; i < f1.size(); i++) {
				//cout << f1[i] << " ";
				int px = (f1[i] - 1) * 3;
				int py = (f1[i] - 1) * 3 + 1;
				int pz = (f1[i] - 1) * 3 + 2;
				//cout << v[px] << " " << v[py] << " " << v[pz] << " " << endl;
				glVertex3f(v[px], v[py], v[pz]);
			}
			glEnd();
			//cout << endl;
			f1.clear();
		}
	}
	//glBegin(GL_TRIANGLES);
	//glColor3f(0, 1, 0);
	//glVertex3f(-1, 0, 0);
	//glVertex3f(0, -1, 0);
	//glVertex3f(0, 0, 1);
	//glEnd();
	//glColor3f(R, G, B);
	//glutWireTeapot(4);
}
void openFile(string fileName) {
	v.clear();
	f.clear();
	xMin = 0;
	xMax = 0;
	yMin = 0;
	yMax = 0;
	zMin = 0;
	zMax = 0;
	fstream fin;
	fin.open(fileName, ios::in);
	if (fin.is_open() == true) {
		ColorMode = "";
		Mode = "points";
		R = 0; G = 0; B = 0;
		while (getline(fin, line)) {
			if (line.substr(0, 2) == "v ") {
				istringstream iss(line);
				string token;
				while (std::getline(iss, token, ' '))
				{
					if (token != "v") {
						stringstream ss;
						float temp;
						ss << token;
						ss >> temp;
						//printf("%f\n",temp);
						v.push_back(temp);
					}
				}
			}
			else if (line.substr(0, 2) == "f ") {
				f.push_back(line.substr(2));
			}
		}
		cout << "success!" << endl;
		for (int i = 0; i < v.size(); i += 3) {
			if (v[i]>xMax) {
				xMax = v[i];
			}
			if (v[i] < xMin) {
				xMin = v[i];
			}
			if (v[i + 1]>yMax) {
				yMax = v[i + 1];
			}
			if (v[i + 1] < yMin) {
				yMin = v[i + 1];
			}
			if (v[i + 2]>zMax) {
				zMax = v[i + 2];
			}
			if (v[i + 2] < zMin) {
				zMin = v[i + 2];
			}
		}
		cout << xMax << endl;
		cout << xMin << endl;
		cout << yMax << endl;
		cout << yMin << endl;
		cout << zMax << endl;
		cout << zMin << endl;
		ChangeSize(600, 600);
	}
	else {
		cout << "can't open file!" << endl;
	}
	fin.close();
}
void setupMenus() {
	int sub1 = glutCreateMenu(menu);
	glutAddMenuEntry("Open", 10);
	int sub2 = glutCreateMenu(menu);
	glutAddMenuEntry("Red", 20);
	glutAddMenuEntry("Green", 30);
	glutAddMenuEntry("Blue", 40);
	glutAddMenuEntry("Ramdom", 50);
	glutAddMenuEntry("AllRamdom", 55);
	int sub3 = glutCreateMenu(menu);
	glutAddMenuEntry("Point", 60);
	glutAddMenuEntry("Line", 70);
	glutAddMenuEntry("Face", 80);
	glutCreateMenu(menu);
	glutAddSubMenu("File", sub1);
	glutAddSubMenu("Colors", sub2);
	glutAddSubMenu("Mode", sub3);

	//glutAddMenuEntry("Test", 0);
	glutAddMenuEntry("Exit", 999);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
void menu(int values) {
	//printf("Menu Item: %d\n", values);
	switch (values) {
	case 0:

		glColor3f(1.0f, 0.0f, 0.0f);
		glutWireTeapot(4);
		//glutSwapBuffers();
		break;
	case 10: {
		//glColor3f(1.0f,0.0f,0.0f);
		//glutWireTeapot(4);
		//glutSwapBuffers();
		int aaa;
		cout << "please input an obj file." << endl;
		string FileName = "";
		cin >> FileName;
		openFile(FileName);

		//scanf_s("%d",&aaa);
		break;
	}
	case 20:
		//glColor3f(1.0f,0.0f,0.0f);
		//glutWireTeapot(4);
		//glutSwapBuffers();
		ColorMode = "";
		R = 1;
		G = 0;
		B = 0;
		break;
	case 30:
		ColorMode = "";
		R = 0;
		G = 1;
		B = 0;
		break;
	case 40:
		ColorMode = "";
		R = 0;
		G = 0;
		B = 1;
		break;
	case 50: {
		int tmp = rand() % 10000;
		float f = tmp*0.0001;
		R = f;
		tmp = rand() % 10000;
		f = tmp*0.0001;
		G = f;
		tmp = rand() % 10000;
		f = tmp*0.0001;
		B = f;
		break;
	}
	case 55: {
		ColorMode = "AllRandom";
		break;
	}
	case 60:
		Mode = "points";
		break;

	case 70:
		Mode = "lines";
		break;
	case 80:
		Mode = "faces";
		break;
	case 999:
		exit(0);
		break;
	default: {

	}
	}//glutPostRedisplay();
}

