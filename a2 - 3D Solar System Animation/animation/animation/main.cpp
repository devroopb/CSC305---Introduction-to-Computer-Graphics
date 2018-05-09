/*
Devroop Banerjee
V00837868
CSC 305 - Assignment 2
*/

#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

int connection = 1, showPath = 0;
double distanceFromSun[8] = { 1.5, 3, 4.5, 6, 8, 10, 12, 14 };
float kuklos = 4 * (sqrt(2) - 1) / 3;				//factor by which coods are calculated for bezier

static float	day = 0,							//setting initial planetary positions
				month = 0,
				year = 0,

				mercuryYear = 45,
				venusYear = 90,
				marsYear = 135,
				jupiterYear = 180,
				saturnYear = 225,
				uranusYear = 270,
				neptuneYear = 315;

GLfloat mercuryPath[4][4][3] = {					//control points for bezier curves to form paths for planets
			{ { distanceFromSun[0],0,0 },{ distanceFromSun[0],0,-distanceFromSun[0] * kuklos },{ distanceFromSun[0] * kuklos,0,-distanceFromSun[0] },{ 0,0,-distanceFromSun[0] } },
			{ { 0,0,-distanceFromSun[0] },{ -distanceFromSun[0] * kuklos,0,-distanceFromSun[0] },{ -distanceFromSun[0],0,-distanceFromSun[0] * kuklos },{ -distanceFromSun[0],0,0 } },
			{ { -distanceFromSun[0],0,0 },{ -distanceFromSun[0],0,distanceFromSun[0] * kuklos },{ -distanceFromSun[0] * kuklos,0,distanceFromSun[0] },{ 0,0,distanceFromSun[0] } },
			{ { 0,0,distanceFromSun[0] },{ distanceFromSun[0] * kuklos,0,distanceFromSun[0] },{ distanceFromSun[0],0,distanceFromSun[0] * kuklos },{ distanceFromSun[0],0,0 } } },

		venusPath[4][4][3] = { 
			{ { distanceFromSun[1],0,0 },{ distanceFromSun[1],0,-distanceFromSun[1] * kuklos },{ distanceFromSun[1] * kuklos,0,-distanceFromSun[1] },{ 0,0,-distanceFromSun[1] } },
			{ { 0,0,-distanceFromSun[1] },{ -distanceFromSun[1] * kuklos,0,-distanceFromSun[1] },{ -distanceFromSun[1],0,-distanceFromSun[1] * kuklos },{ -distanceFromSun[1],0,0 } },
			{ { -distanceFromSun[1],0,0 },{ -distanceFromSun[1],0,distanceFromSun[1] * kuklos },{ -distanceFromSun[1] * kuklos,0,distanceFromSun[1] },{ 0,0,distanceFromSun[1] } },
			{ { 0,0,distanceFromSun[1] },{ distanceFromSun[1] * kuklos,0,distanceFromSun[1] },{ distanceFromSun[1],0,distanceFromSun[1] * kuklos },{ distanceFromSun[1],0,0 } } },

		earthPath[4][4][3] = {
			{ { distanceFromSun[2],0,0 },{ distanceFromSun[2],0,-distanceFromSun[2] * kuklos },{ distanceFromSun[2] * kuklos,0,-distanceFromSun[2] },{ 0,0,-distanceFromSun[2] } },
			{ { 0,0,-distanceFromSun[2] },{ -distanceFromSun[2] * kuklos,0,-distanceFromSun[2] },{ -distanceFromSun[2],0,-distanceFromSun[2] * kuklos },{ -distanceFromSun[2],0,0 } },
			{ { -distanceFromSun[2],0,0 },{ -distanceFromSun[2],0,distanceFromSun[2] * kuklos },{ -distanceFromSun[2] * kuklos,0,distanceFromSun[2] },{ 0,0,distanceFromSun[2] } },
			{ { 0,0,distanceFromSun[2] },{ distanceFromSun[2] * kuklos,0,distanceFromSun[2] },{ distanceFromSun[2],0,distanceFromSun[2] * kuklos },{ distanceFromSun[2],0,0 } } },

		marsPath[4][4][3] = {
			{ { distanceFromSun[3],0,0 },{ distanceFromSun[3],0,-distanceFromSun[3] * kuklos },{ distanceFromSun[3] * kuklos,0,-distanceFromSun[3] },{ 0,0,-distanceFromSun[3] } },
			{ { 0,0,-distanceFromSun[3] },{ -distanceFromSun[3] * kuklos,0,-distanceFromSun[3] },{ -distanceFromSun[3],0,-distanceFromSun[3] * kuklos },{ -distanceFromSun[3],0,0 } },
			{ { -distanceFromSun[3],0,0 },{ -distanceFromSun[3],0,distanceFromSun[3] * kuklos },{ -distanceFromSun[3] * kuklos,0,distanceFromSun[3] },{ 0,0,distanceFromSun[3] } },
			{ { 0,0,distanceFromSun[3] },{ distanceFromSun[3] * kuklos,0,distanceFromSun[3] },{ distanceFromSun[3],0,distanceFromSun[3] * kuklos },{ distanceFromSun[3],0,0 } } },

		jupiterPath[4][4][3] = {
			{ { distanceFromSun[4],0,0 },{ distanceFromSun[4],0,-distanceFromSun[4] * kuklos },{ distanceFromSun[4] * kuklos,0,-distanceFromSun[4] },{ 0,0,-distanceFromSun[4] } },
			{ { 0,0,-distanceFromSun[4] },{ -distanceFromSun[4] * kuklos,0,-distanceFromSun[4] },{ -distanceFromSun[4],0,-distanceFromSun[4] * kuklos },{ -distanceFromSun[4],0,0 } },
			{ { -distanceFromSun[4],0,0 },{ -distanceFromSun[4],0,distanceFromSun[4] * kuklos },{ -distanceFromSun[4] * kuklos,0,distanceFromSun[4] },{ 0,0,distanceFromSun[4] } },
			{ { 0,0,distanceFromSun[4] },{ distanceFromSun[4] * kuklos,0,distanceFromSun[4] },{ distanceFromSun[4],0,distanceFromSun[4] * kuklos },{ distanceFromSun[4],0,0 } } },

		saturnPath[4][4][3] = {
			{ { distanceFromSun[5],0,0 },{ distanceFromSun[5],0,-distanceFromSun[5] * kuklos },{ distanceFromSun[5] * kuklos,0,-distanceFromSun[5] },{ 0,0,-distanceFromSun[5] } },
			{ { 0,0,-distanceFromSun[5] },{ -distanceFromSun[5] * kuklos,0,-distanceFromSun[5] },{ -distanceFromSun[5],0,-distanceFromSun[5] * kuklos },{ -distanceFromSun[5],0,0 } },
			{ { -distanceFromSun[5],0,0 },{ -distanceFromSun[5],0,distanceFromSun[5] * kuklos },{ -distanceFromSun[5] * kuklos,0,distanceFromSun[5] },{ 0,0,distanceFromSun[5] } },
			{ { 0,0,distanceFromSun[5] },{ distanceFromSun[5] * kuklos,0,distanceFromSun[5] },{ distanceFromSun[5],0,distanceFromSun[5] * kuklos },{ distanceFromSun[5],0,0 } } },

		uranusPath[4][4][3] = {
			{ { distanceFromSun[6],0,0 },{ distanceFromSun[6],0,-distanceFromSun[6] * kuklos },{ distanceFromSun[6] * kuklos,0,-distanceFromSun[6] },{ 0,0,-distanceFromSun[6] } },
			{ { 0,0,-distanceFromSun[6] },{ -distanceFromSun[6] * kuklos,0,-distanceFromSun[6] },{ -distanceFromSun[6],0,-distanceFromSun[6] * kuklos },{ -distanceFromSun[6],0,0 } },
			{ { -distanceFromSun[6],0,0 },{ -distanceFromSun[6],0,distanceFromSun[6] * kuklos },{ -distanceFromSun[6] * kuklos,0,distanceFromSun[6] },{ 0,0,distanceFromSun[6] } },
			{ { 0,0,distanceFromSun[6] },{ distanceFromSun[6] * kuklos,0,distanceFromSun[6] },{ distanceFromSun[6],0,distanceFromSun[6] * kuklos },{ distanceFromSun[6],0,0 } } },

		neptunePath[4][4][3] = {
			{ { distanceFromSun[7],0,0 },{ distanceFromSun[7],0,-distanceFromSun[7] * kuklos },{ distanceFromSun[7] * kuklos,0,-distanceFromSun[7] },{ 0,0,-distanceFromSun[7] } },
			{ { 0,0,-distanceFromSun[7] },{ -distanceFromSun[7] * kuklos,0,-distanceFromSun[7] },{ -distanceFromSun[7],0,-distanceFromSun[7] * kuklos },{ -distanceFromSun[7],0,0 } },
			{ { -distanceFromSun[7],0,0 },{ -distanceFromSun[7],0,distanceFromSun[7] * kuklos },{ -distanceFromSun[7] * kuklos,0,distanceFromSun[7] },{ 0,0,distanceFromSun[7] } },
			{ { 0,0,distanceFromSun[7] },{ distanceFromSun[7] * kuklos,0,distanceFromSun[7] },{ distanceFromSun[7],0,distanceFromSun[7] * kuklos },{ distanceFromSun[7],0,0 } }
};

void init(void) {
	glShadeModel(GL_SMOOTH);								//Parameter handling
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);							//polygon rendering mode
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);					//Black background
}

void pathMaker(GLfloat planetaryPath[4][4][3]) {		//uses 4 bezier curves to make a dotted path for planets
	for (int i = 0; i < 4; i++) {
		glMap1f(GL_MAP1_VERTEX_3, 0, 1, 3, 4, &planetaryPath[i][0][0]);
		glEnable(GL_MAP1_VERTEX_3);
		
		glLineWidth(0.1);
		glColor3f(0.2, 0.3, 0.3);
		glBegin(GL_LINES);
			for (int j = 0; j <= connection; j++) {
				glEvalCoord1f((GLfloat)j / (GLfloat)connection);
			}
		glEnd();

		glPointSize(1);
		glColor3f(0, 0, 0);
		glBegin(GL_POINTS);
			for (int a = 0; a < 4; a++) {
				for (int b = 0; b < 4; b++) {
					glVertex3fv(&planetaryPath[a][b][0]);
				}
			}
		glEnd();

	}
}

void solarSystem(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			//sets the bitplane area of the window to values previously selected by glClearColor. Indicates the buffers currently enabled for color writing ad its depth
	glPushMatrix();												//pushes the current matrix stack down by one, duplicating the current matrix
	glRotatef((GLfloat)year, 0, 1, 0);							//original position of the sun
	glColor3f(0.9, 0.7, 0.0);									//colouring the sun
	glutSolidSphere(1.2, 100, 100);								//drawing the sun

	glRotatef((GLfloat)mercuryYear, 0, 1, 0);
	glTranslatef(distanceFromSun[0], 0, 0);
	glRotatef((GLfloat)day, 0, 1, 0);
	glColor3f(0.7, 0.7, 0.7);
	glutSolidSphere(0.08, 100, 100);			//mercury

	glPopMatrix();														//pops current matrix model (specs) on stack
	if (showPath == 1) { pathMaker(mercuryPath); }						//animation path for mercury
	glPushMatrix();														//pushes current matrix model (specs) on stack so further calculations are dependant

	glRotatef((GLfloat)venusYear, 0, 1, 0);
	glTranslatef(distanceFromSun[1], 0, 0);
	glRotatef((GLfloat)day, 0, 1, 0);
	glColor3f(0.6, 0.8, 0.2);
	glutSolidSphere(0.1, 100, 100);				//venus

	glPopMatrix();
	if (showPath == 1) { pathMaker(venusPath); }						//animation path for venus
	glPushMatrix();

	glRotatef((GLfloat)year, 0, 1, 0);
	glTranslatef(distanceFromSun[2], 0, 0);
	glRotatef((GLfloat)day, 0, 1, 0);
	glColor3f(0.0, 0.0, 0.8);
	glutSolidSphere(0.2, 100, 100);				//earth

	glTranslatef(0.5, 0, 0);
	glRotatef((GLfloat)month, 0, 1, 0);
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(0.07, 100, 100);			//moon

	glPopMatrix();
	if (showPath == 1) { pathMaker(earthPath); }						//animation path for earth
	glPushMatrix();

	glRotatef((GLfloat)marsYear, 0, 1, 0);
	glTranslatef(distanceFromSun[3], 0, 0);
	glRotatef((GLfloat)day, 0, 1, 0);
	glColor3f(1.0, 0.0, 0.0);
	glutSolidSphere(0.15, 100, 100);			//mars

	glPushMatrix();

	glTranslatef(0.5, 0, 0);
	glRotatef((GLfloat)month, 1, 1, 0);
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(0.05, 100, 100);			//mars' moon1

	glPopMatrix();

	glTranslatef(-0.5, 0, 0);
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(0.05, 100, 100);			//mars' moon2

	glPopMatrix();
	if (showPath == 1) { pathMaker(marsPath); }						//animation path for mars
	glPushMatrix();

	glRotatef((GLfloat)jupiterYear, 0, 1, 0);
	glTranslatef(distanceFromSun[4], 0, 0);
	glRotatef((GLfloat)day, 0, 1, 0);
	glColor3f(1.0, 0.3, 0.0);
	glutSolidSphere(0.6, 100, 100);				//jupiter

	glTranslatef(0.75, 0, 0);
	glRotatef((GLfloat)month, 0.3, 1, 0);
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(0.07, 100, 100);			//jupiter's moon

	glPopMatrix();
	if (showPath == 1) { pathMaker(jupiterPath); }						//animation path for jupiter
	glPushMatrix();

	glRotatef((GLfloat)saturnYear, 0, 1, 0);
	glTranslatef(distanceFromSun[5], 0, 0);
	glRotatef((GLfloat)day, 0, 1, 0);
	glColor3f(0.8, 0.6, 0.1);
	glutSolidSphere(0.3, 100, 100);				//saturn

	GLUquadricObj *disk;
	disk = gluNewQuadric();
	gluQuadricOrientation(disk, GLU_OUTSIDE);
	glRotatef(90, 1, 0, 0);
	glColor3f(0.3, 0.3, 0.3);
	gluDisk(disk, 0.4, 0.5, 30, 1);				//saturn inner ring
	gluDeleteQuadric(disk);

	GLUquadricObj *disk2;
	disk2 = gluNewQuadric();
	gluQuadricOrientation(disk2, GLU_OUTSIDE);
	glRotatef(0, 1, 0, 0);
	glColor3f(0.7, 0.7, 0.7);
	gluDisk(disk2, 0.5, 0.65, 30, 1);			//saturn outer ring
	gluDeleteQuadric(disk2);

	glPopMatrix();
	if (showPath == 1) { pathMaker(saturnPath); }						//animation path for saturn
	glPushMatrix();

	glRotatef((GLfloat)uranusYear, 0, 1, 0);
	glTranslatef(distanceFromSun[6], 0, 0);
	glRotatef((GLfloat)day, 0, 1, 0);
	glColor3f(0.5, 0.8, 0.9);
	glutSolidSphere(0.2, 100, 100);				//uranus

	glPopMatrix();
	if (showPath == 1) { pathMaker(uranusPath); }						//animation path for uranus
	glPushMatrix();

	glRotatef((GLfloat)neptuneYear, 0, 1, 0);
	glTranslatef(distanceFromSun[7], 0, 0);
	glRotatef((GLfloat)day, 0, 1, 0);
	glColor3f(0.4, 0.4, 0.8);
	glutSolidSphere(0.2, 100, 100);				//neptune

	glPopMatrix();
	
	if (showPath == 1) { pathMaker(neptunePath); }						//animation path for neptune
	glutSwapBuffers();
}

void reshape(int w, int h) {
	if (h == 0) { h = 1; }
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, w / (GLfloat)h, 3, 90);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(5, 6, 20, 0, -2, 0, 0, 1, 0);								//observing position (eye,eye,eye,centre,centre,centre,up,up,up)
}

void idleFunc(void) {													//this is used for animation
	day += 1;
	month += 3;

	mercuryYear += 0.4;
	venusYear += 0.35;
	year += 0.3;
	marsYear += 0.25;
	jupiterYear += 0.2;
	saturnYear += 0.15;
	uranusYear += 0.1;
	neptuneYear += 0.05;

	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'p':
			if (showPath == 0) { showPath = 1; } else { showPath = 0; }					//toggle the planetary paths
			break;
		case 27:
			exit(0);
			break;
		case '+':
			connection++;
			break;
		case '-':
			connection--;
			break;
	}

	if (connection < 0) {
		connection = 0;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1920, 1080);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("CSC 305 - A2 - Solar System");
	
	init();

	glutDisplayFunc(solarSystem);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutIdleFunc(idleFunc);

	glEnable(GL_DEPTH_TEST);
	glutMainLoop();

	return 0;
}