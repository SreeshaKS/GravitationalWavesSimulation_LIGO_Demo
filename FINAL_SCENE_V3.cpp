
#include <GL/glut.h>  
#include <stdlib.h>
#include<math.h>
#include<stdio.h>

char title[] = "Gravitational Wave Simulation";
GLfloat mainWindow = 0.0;
GLfloat creditWindow = 0.0;

GLfloat cameraAngle = 0.0f;
float time;
GLfloat waveRecedence = 0.0;
GLfloat sunColor[4] = { 0.8,.5,0.0,1 };
//GLfloat meshColor[4] = { 0.9,.6,0.0,1 };
GLfloat meshColor[4] = { 1.0,1.5,1.0,1 };

GLfloat translateDistanceZ = 0.0;
GLfloat translateDistanceX = 0.0;

/*CHANGE VALUE TO ZERO*/
int transitionToMilkyWay = 0.0;
int zoomIntoDiffractionPattern = 0.0;
int isDisplayingLIGOMotion = 0.0;
/*--------------------------------------------------------------------------------------*/

GLfloat milyWayTrasitionStartTime = 0.0;
GLfloat milyWayTrasitionTimeElapsed = 0.0;
GLfloat milkyWayTransitionMaxTime = 20;

GLfloat LIGOTrasitionStartTime = 0.0;
GLfloat IGOTrasitionTimeElapsed = 0.0;
GLfloat LIGOTransitionMaxTime = 100;

GLfloat zoomToEarthX = 0.0;
GLfloat zoomToEarthY = 0.0;
GLfloat zoomToEarthZ = 100;

/*GLfloat currentEarthFocusX = -100 + translateDistanceX;
GLfloat currentEarthFocusY = 0.0;
GLfloat currentEarthFocusZ = 50 - translateDistanceZ;*/

GLfloat currentEarthFocusX = 1.5;
GLfloat currentEarthFocusY = 0.0;
GLfloat currentEarthFocusZ = -50;

/*Ligo Varibles*/
GLUquadricObj *quadratic;
float originEnterAxis1;
float originEnterAxis2;

GLfloat positiveZAxisMirRadius = 0.9;
GLfloat negativeXAxisMirRadius = 0.9;
GLfloat negativeXMirrorAmp = 0.3;
GLfloat positiveZMirrorAmp = 0.3;
GLfloat originMirRadius = 1.5;
GLfloat diffractionScreenRadius = 1.2;
GLfloat diffractionPatternRadius = 0.8;
GLfloat frequency = 0.06;
GLfloat waveFrequency = 0.6;

GLfloat positiveZAxisMirWaveMonitor = (positiveZMirrorAmp)*sin((2 * 3.147 * frequency  * time) + 90);
GLfloat negativeXAxisMirWaveMonitor = (negativeXMirrorAmp)*sin((2 * 3.147 * frequency  * time));

int demoPhaze = 0;
/*Ligo Varibles End*/

void initGL() {
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.2f, 0.3f, 0.3f);
	static GLfloat lightpos0[4] = { 0.0,3.0,3.0,0.0 };
	static GLfloat lightpos1[4] = { 0.0,-3.0,3.0,0.0 };
	static GLfloat lightamb1[4] = { 0.2,0.2,0.2,0.0 };
	static GLfloat lightdif1[4] = { 1.0,1.0,1.0,1.0 };
	static GLfloat lightape1[4] = { 1.0,1.0,1.0,1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos0);
	glLightfv(GL_LIGHT1, GL_POSITION, lightpos1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightamb1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightdif1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightape1);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	quadratic = gluNewQuadric();
}
GLdouble get_secs()
{
	return glutGet(GLUT_ELAPSED_TIME) / 500.0;
}
void drawText(float x, float y, float z, int r, int g, int b, const char *string)
{
	int j4 = strlen(string);
	glColor3f(r, g, b);
	glRasterPos3f(x, y, z);
	for (int i3 = 0; i3<j4; i3++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i3]);
	}
}
void renderPhazeOne(GLfloat time) {
	/*Disk in negative x axis */
	glPushMatrix();
	glColor4f(0.0, 0.0, 1.0, 1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, -7.0);
	gluDisk(quadratic, 0.0f, negativeXAxisMirRadius, 32, 32);
	glPopMatrix();
	/*Disk in positive z axis */
	glPushMatrix();
	glColor4f(0.0, 0.0, 1.0, 1.0);
	glTranslatef(0.0, 0.0, +7);
	gluDisk(quadratic, 0.0f, positiveZAxisMirRadius, 32, 32);
	glPopMatrix();

	/*Rotated Disk*/
	glPushMatrix();

	glColor4f(0.0, 0.0, 1.0, 1.0);
	glTranslatef(0.0, 0.0, 0.0);
	glRotatef(45, 0.0, 1.0, 0.0);
	gluDisk(quadratic, 0.0f, originMirRadius, 100, 100);
	glPopMatrix();
	/*DiffractionScreen*/
	glPushMatrix();
	glColor4f(0.0, 0.0, 1.0, 1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, 4.0);
	gluDisk(quadratic, 0.0f, diffractionScreenRadius, 32, 32);
	glPopMatrix();
	/*Lazer SOurce*/
	glPushMatrix();
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glTranslatef(0.0, 0.0, -7);
	gluCylinder(quadratic, 0.5f, 0.5f, 1.0f, 32, 32);
	glPopMatrix();

	glPushMatrix();
	glColor4f(1.0, 0.0, 0.0, 1.0);
	glLineWidth(5.0);
	float scaleX = 0.0;
	glBegin(GL_LINES);
	//glVertex3f(0.0, sin(-6.0 + time), -6.0 + time);
	if ((-6.0 + time) <= 7.0) {
		glVertex3f(0.0, 0.0, -7.0);
		glVertex3f(0.0, 0.0, -7.0 + time);
	}
	else {
		glVertex3f(0.0, 0.0, -7.0);
		glVertex3f(0.0, 0.0, 7.0);
	}
	if ((-6.0 + time) >= 0.0) {
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(scaleX -= 7.0, 0.0, 0.0);
	}
	//glScalef(0.0, 0.0, 20.0*time);
	glEnd();
	glPointSize(2.0);
	glColor3f(1.0, 0.0, 0.0);
	/*PLot Sine Wave*/
	float k = 7.0;
	glBegin(GL_POINTS);
	glColor3f(1.5, 1.0, 0.5);
	for (GLfloat i = -6; i < 18; i = i + 0.001) {
		if (i >= 0.0&&i <= 7.0) {
			glVertex3f(-i, 0.0, (0.3)*sin((2 * 3.147 * waveFrequency * i)));

		}
		if (i < 7) {
			glVertex3f((0.3)*sin((2 * 3.147 * waveFrequency * i)), 0.0, i);

		}
		else {


			glVertex3f((0.3)*sin((2 * 3.147 * waveFrequency * k)), 0.3, k);
			glVertex3f((0.3)*sin((2 * 3.147 * waveFrequency * k)), 0.3, k);

			glVertex3f(-k, 0.3, (0.3)*sin((2 * 3.147 * waveFrequency * k) + 90));
			k = k - 0.001;
		}
		if (k < 0.0)break;
	}
	for (float i = 0; i < 4; i = i + 0.001) {
		glColor3f(1.5, 1.0, 0.5);
		glVertex3f(i, 0.0, (0.3)*sin((2 * 3.147 * waveFrequency * i)));
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(i, 0.0, (0.3)*sin((2 * 3.147 * waveFrequency * i + 90)));
	}
	glEnd();

	glPopMatrix();
	glLineWidth(1.0);

	if (cameraAngle >= -230.0) {
		cameraAngle -= 0.09f;

	}
	else {
		demoPhaze = 1;
	}
	if (((int)cameraAngle % 50) == 0)
		printf("Angle : %f", cameraAngle);

}
void renderPhazeTwo(GLfloat time) {


	/*DiffractionPattern*/
	glPushMatrix();
	/*Calculate Color Intensity*/
	GLfloat value = ((positiveZMirrorAmp)*sin((2 * 3.147 * frequency  * time) + 90) + (negativeXMirrorAmp)*sin((2 * 3.147 * frequency * time)));
	value = (value < 0.0 ? (-value) : value);
	glColor4f(value, 0.0, 0.0, 1.0);

	glRotatef(90, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, 3.75);

	drawText(2.0, 3.0, 3.5, 1, 0, 0, "Diffraction Pattern ");
	glColor4f(value, 0.0, 0.0, 1.0);

	gluDisk(quadratic, 0.0f, diffractionPatternRadius, 100, 100);
	glPopMatrix();
	/*DiffractionScreen*/
	glPushMatrix();

	glColor4f(0.75, 0.75, 0.75, 1.0);

	glRotatef(90, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, 4.0);

	drawText(0.0, 3.0, 4.0, 1, 0, 0, "Diffraction Screen");
	glColor4f(0.75, 0.75, 0.75, 1.0);

	gluDisk(quadratic, 0.0f, diffractionScreenRadius, 100, 100);
	glPopMatrix();

	/*Rotated Disk*/
	glPushMatrix();

	glColor4f(0.0, 0.0, 1.0, 1.0);
	drawText(0.0, -3.0, 0.0, 1, 0, 0, "Beam Splitter");
	glTranslatef(0.0, 0.0, 0.0);
	glRotatef(45, 0.0, 1.0, 0.0);
	glColor4f(0.0, 0.0, 1.0, 1.0);
	gluDisk(quadratic, 0.0f, originMirRadius, 100, 100);
	glPopMatrix();

	/*Lazer SOurce*/
	glPushMatrix();
	glColor4f(1.0, 1.0, 1.0, 1.0);
	drawText(0.0, 1.0, -7.0, 1, 0, 0, "Lazer Source");
	glTranslatef(0.0, 0.0, -7);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	gluCylinder(quadratic, 0.5f, 0.5f, 1.0f, 32, 32);
	glPopMatrix();
	/*Origin To the Disk Wave negative x axis disk*/
	glColor4f(1.0, 0.0, 0.0, 1.0);
	glBegin(GL_POINTS);
	for (GLfloat i = 0; i < 7 - (negativeXMirrorAmp)*sin(2 * 3.147*frequency*time); i = i + 0.001) {
		glVertex3f(-i, 0.3, (0.3)*sin((2 * 3.147 * waveFrequency * i) + 90));
	}
	glEnd();
	/*Disk in negative x axis */
	glPushMatrix();
	glColor4f(0.0, 0.0, 1.0, 1.0);
	glRotatef(90, 0.0, 1.0, 0.0);

	glTranslatef(0.0, 0.0, -7.0 + (negativeXMirrorAmp)*sin(2 * 3.147*frequency*time));
	drawText(0.0, 1.0, -7.0 + (negativeXMirrorAmp)*sin(2 * 3.147*frequency*time), 1, 0, 0, "X-Axis Mirror");
	glColor4f(0.0, 0.0, 1.0, 1.0);

	gluDisk(quadratic, 0.0f, negativeXAxisMirRadius, 32, 32);
	glPopMatrix();

	glPushMatrix();
	glColor4f(1.0, 0.0, 0.0, 1.0);
	glTranslatef(-7.0 + (negativeXMirrorAmp)*sin(2 * 3.147*frequency*time), 0.0, 0.0);
	glBegin(GL_POINTS);
	for (GLfloat i = 0.0; i < 11 - (negativeXMirrorAmp)*sin((2 * 3.147*frequency*time)); i = i + 0.001) {
		if (i >= 0.0)glColor4f(0.0, 0.0, 1.0, 1.0);
		else glColor4f(1.0, 0.0, 0.0, 1.0);
		glVertex3f(i, 0.3, (0.3)*sin((2 * 3.147*waveFrequency*i)));
	}
	/*glVertex3f(0.0,0.0,0.0);
	glVertex3f(11-sin((2 * 3.147*frequency*time)), 0.0, 0.0);*/
	glEnd();
	glPopMatrix();

	glColor4f(1.0, 0.0, 0.0, 1.0);
	glBegin(GL_POINTS);
	for (GLfloat i = -6; i < 7 - (positiveZMirrorAmp)*sin((2 * 3.147 *frequency * time) + 90); i = i + 0.001) {
		glVertex3f((0.3)*sin((2 * 3.147 * waveFrequency * i)), 0.0, i);
	}
	glEnd();

	/*Disk in positive z axis */
	glPushMatrix();
	glColor4f(0.0, 0.0, 1.0, 1.0);
	glTranslatef(0.0, 0.0, 7.0 - (positiveZMirrorAmp)*sin((2 * 3.147 *frequency * time) + 90));

	drawText(0.0, 2.0, 7.0 - (positiveZMirrorAmp)*sin((2 * 3.147 *frequency * time) + 90), 1, 0, 0, "Z-Axis Mirror");
	glColor4f(0.0, 0.0, 1.0, 1.0);

	gluDisk(quadratic, 0.0f, positiveZAxisMirRadius, 32, 32);
	glPopMatrix();
	/*Origin to Positive Z Axis Mirror reflection Wave*/
	glPushMatrix();
	glColor3f(1.5, 1.0, 0.5);
	glTranslatef(0.0, 0.0, 7.0 - (positiveZMirrorAmp)*sin((2 * 3.147 *frequency * time) + 90));
	glBegin(GL_POINTS);
	for (GLfloat i = 0.0; i <= 7 - (positiveZMirrorAmp)*sin((2 * 3.147 *frequency * time) + 90); i = i + 0.001) {
		glVertex3f((0.3)*sin((2 * 3.147 * waveFrequency * i)), 0.3, -i);
	}
	glEnd();
	glPopMatrix();
	/*Origin to Difractionscreen wave for positive z axis mirror*/
	glPushMatrix();
	glColor3f(1.5, 1.0, 0.5);
	glTranslatef((positiveZMirrorAmp)*sin((2 * 3.147 *frequency * time) + 90), 0.0, 0.0);
	glBegin(GL_POINTS);
	for (GLfloat i = 0.0 - (positiveZMirrorAmp)*sin((2 * 3.147 *frequency * time) + 90); i <= 4 - (positiveZMirrorAmp)*sin((2 * 3.147 *frequency * time) + 90); i = i + 0.001) {
		glVertex3f(i, 0.3, (0.3)*sin((2 * 3.147 * waveFrequency * i)));
	}
	glEnd();
	glPopMatrix();

	//Update variable to enable Zomming into DiffractionPattern
	isDisplayingLIGOMotion = 1.0;
}
void drawAxes() {
	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);

	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-100.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, -100.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, -100.0f);
	glEnd();
}
/*Start Binary System*/
void drawBinarySystem() {

	GLfloat theta1 = 0.0;
	GLfloat maxTime1 = 40;
	GLfloat rVal1 = 0.0;

	GLfloat theta2 = 0;
	GLfloat maxTime2 = 40;
	GLfloat rVal2 = 0.0;

	GLfloat theta = 0;
	GLfloat maxTime = 20;
	GLfloat rVal = 0.0;
	GLfloat blackHoleColor[] = { 1.0f, 1.0f, 1.0f,1.0f };

	if ((maxTime1 - time) >= 0) {
		theta1 += 0.1;
		rVal1 = theta1*(maxTime1 - time);

		theta2 += 0.1;
		rVal2 = theta2*(maxTime2 - time);

		glPushMatrix();/* planet */
		rVal1 = theta1*(maxTime1 - time);
		theta1 = time * 2 + 10;
		theta = theta1;

		glPushMatrix();

		for (GLfloat i = rVal1 / theta1; i < (time); i += 0.0001) {

			theta += 0.001;
			rVal = theta*(i);

			//Draw Point Waves
			glBegin(GL_POINTS);
			glColor3f(1.0, 1.0, 1.0);

			glVertex3f((rVal)*sin(theta), 0.0, ((rVal))*cos(theta));
			glVertex3f(((rVal))*sin((theta + 90)), 0.0, ((rVal))*cos((theta + 90)));

			glEnd();
			/*
			//Draw Lines
			glBegin(GL_LINES);
			glColor3f(1.0, 1.0, 1.0);
			glVertex3f((rVal)*sin(theta), 0.0, ((rVal))*cos(theta));
			glVertex3f((rVal)*sin(theta) , 3.0, ((rVal))*cos(theta) );

			glVertex3f(((rVal))*sin((theta + 90)), 0.0, ((rVal))*cos((theta + 90)));
			glVertex3f(((rVal))*sin((theta + 90)) , 3.0, ((rVal))*cos((theta + 90)) );
			glEnd();*/

			/*//Draw Sine Wave
			glBegin(GL_POINTS);
			GLfloat sineZ = ((rVal))*cos(theta + 90);
			glColor3f(1.0, 1.0, 1.0);
			for (GLfloat sineX = (rVal)*sin(theta) - 2.5; sineX < (rVal)*sin(theta) + 2.5; sineX += 0.1) {

			glVertex3f(sineX, sin(2*3.14*0.2*sineX) , ((rVal))*cos(theta) +0.1);
			sineZ += 0.1;
			}
			sineZ = ((rVal))*cos(theta + 90);
			for (GLfloat sineX = (rVal)*sin(theta+90) - 2.5; sineX < (rVal)*sin(theta+90) + 2.5; sineX += 0.1) {
			glVertex3f(sineX, sin(2 * 3.14*0.2*sineX), sineZ);
			sineZ += 0.1;
			}
			glEnd();*/
		}
		glColor3f(0.0, 0.0, 0.0);
		glPopMatrix();

		glPushMatrix();
		glTranslatef((rVal1)*sin(theta1), 0.0, ((rVal1))*cos(theta1));

		GLfloat blackHole1Color[] = { 0.0f, 0.0f, 0.0f ,1.0f };
		GLfloat blackHole1LightPos[4] = { (rVal1)*sin(theta1),0.0,((rVal1))*cos(theta1),1.0 };
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blackHole1Color);
		glLightfv(GL_LIGHT2, GL_POSITION, blackHole1LightPos);
		glEnable(GL_LIGHT2);

		glutSolidSphere(1.5, 50, 50);
		glPopMatrix();


		glPushMatrix();
		rVal2 = theta2*(maxTime2 - time);
		theta2 = time * 2 + 10;
		glTranslatef((rVal2)*sin(theta2 + 90), 0.0, ((rVal2))*cos(theta2 + 90));

		GLfloat blackHole2Color[] = { 0.0f, 0.0f, 0.0f ,1.0f };
		GLfloat blackHole2LightPos[4] = { (rVal2)*sin(theta2 + 90),0.0,((rVal2))*cos(theta2 + 90),1.0 };
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blackHole2Color);
		glLightfv(GL_LIGHT3, GL_POSITION, blackHole2LightPos);
		glEnable(GL_LIGHT3);

		glutSolidSphere(1.5, 50, 50);

		glPopMatrix();
	}
	else {
		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);
		glTranslatef(0.0, 0.0, 0.0);
		/*GLfloat finalBlackHoleColor[] = { 0.0f, 0.0f, 0.0f ,1.0f };
		GLfloat finalBlackHoleColorLightPos[4] = { 0.0,0.0,0.0,1.0 };
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, finalBlackHoleColor);
		glLightfv(GL_LIGHT3, GL_POSITION, finalBlackHoleColorLightPos);
		glEnable(GL_LIGHT4);*/

		glutSolidSphere(2.0, 100, 100);
		glPopMatrix();

		for (GLfloat i = (waveRecedence += (time / 100)); i<100; i += 0.001) {

			theta += 0.06;
			rVal = theta*(i);
			glBegin(GL_POINTS);
			glColor3f(1.0, 1.0, 1.0);
			glVertex3f((rVal)*sin(theta), 0.0, ((rVal))*cos(theta));
			glVertex3f(((rVal))*sin((theta + 90)), 0.0, ((rVal))*cos((theta + 90)));
			glEnd();
		}
		if (waveRecedence > 100) {
			transitionToMilkyWay = 1;
		}
	}
}
/*End of binary System*/

/*Start Milky Way System*/
void drawQuarterDepression(GLfloat wideness, GLfloat gridSpacing, int surroundGrids) {



	glColor3f(1.0f, 1.0f, 1.0f);
	GLfloat apexX = 0.0;
	GLfloat apexY = 0.0;
	GLfloat gridXPoint, gridYPoint;

	GLfloat densityNum = 0.01;
	GLfloat maxZoffset = sqrt(
		pow(
			(sqrt(4 * wideness * (2 - apexY)) + apexX)

			-
			(sqrt(4 * wideness * (-2 - apexY)) + apexX)

			,
			2
			)

		) / densityNum;

	glBegin(GL_POINTS);
	for (GLfloat zOffset1 = -2; zOffset1 < 2; zOffset1 += gridSpacing) {
		apexY = zOffset1;
		for (GLfloat i = -2; i < 2; i += 0.01) {

			glVertex3f(sqrt(4 * wideness * (i - apexY)) + apexX, i, zOffset1);
			glVertex3f(-sqrt(4 * wideness * (i - apexY)) + apexX, i, zOffset1);

			gridXPoint = sqrt(4 * wideness * (i - apexY)) + apexX;
			gridYPoint = i;
		}
		if (surroundGrids == 1) {
			glColor3f(1.0, 1.0, 1.0);
			GLfloat gridX;
			GLfloat gridY;
			for (GLfloat n = -2; n <2; n += 0.01) {
				gridX = n + gridXPoint;
				gridY = gridYPoint + sqrt(4 * 0.1 * (n));
				glVertex3f(n + gridXPoint, gridYPoint + sqrt(4 * 0.1 * (n)), zOffset1);
				glVertex3f(-n - gridXPoint, gridYPoint + sqrt(4 * 0.1 * (n)), zOffset1);
			}
			/*for (GLfloat m = 0.0; m < 3; m += 0.02) {
			glVertex3f(gridX + m, gridY, zOffset1);
			glVertex3f(-gridX - m, gridY, zOffset1);
			}*/
		}
	}

	glEnd();
}
void drawHalfDepression(GLfloat wideness, GLfloat gridSpacing, int surroundGrids) {
	glPushMatrix();
	drawQuarterDepression(wideness, gridSpacing, surroundGrids);

	glPushMatrix();
	glTranslatef(0.0, 0.0, -4.0);
	glRotatef(180, 0.0, 1.0, 0.0);
	drawQuarterDepression(wideness, gridSpacing, surroundGrids);

	glPopMatrix();
	glPopMatrix();
}
void drawFullDepression(GLfloat wideness, GLfloat gridSpacing, int surroundGrids) {
	drawHalfDepression(wideness, gridSpacing, surroundGrids);
	glPushMatrix();
	glRotatef(90, 0.0, 1.0, 0.0);
	glTranslatef(2.0, 0.0, 2.0);

	drawHalfDepression(wideness, gridSpacing, surroundGrids);
	glPopMatrix();
}
void drawMilkyWaySystem() {
	/*Draw the Sun*/
	glPushMatrix();
	glTranslatef(0.0, 7.0, 0.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, sunColor);
	glColor3f(1.0f, 1.5f, 0.0f);
	glutSolidSphere(4, 80, 80);
	glutSolidSphere(4.1, 100, 100);
	GLfloat lightpos1[4] = { 0.0,11.0,0.0,0.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, lightpos1);
	glEnable(GL_LIGHT1);
	glPopMatrix();

	glPushMatrix();

	/*Earth*/
	glColor3f(0.0f, 1.5f, 0.0f);
	GLfloat planetEarthColor[] = { 0.0f, 0.0f, 1.0f };
	glTranslatef(-13.0, 5.0, -0.5);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, planetEarthColor);
	GLfloat earthLightpos[3] = { -13.0, 4.0, -0.5 };
	glLightfv(GL_LIGHT1, GL_POSITION, earthLightpos);
	glEnable(GL_LIGHT1);
	glutSolidSphere(1.5, 50, 50);
	glPopMatrix();

	/*Earth Depression*/
	glPushMatrix();

	glTranslatef(-15.0, 2.0, 1.5);

	glColor3f(1.0f, 1.0f, 1.0f);
	glScalef(0.9, 0.8, 0.8);
	glRotatef(-360, 1.0, 0.0, 0.0);
	glPushMatrix();
	glRotatef(-50, 0.0, 0.0, 1.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, meshColor);
	drawFullDepression(1.4, 3.0, 0);
	glPopMatrix();
	//drawQuarterDepression(1);
	glPopMatrix();


	/*Venus*/
	glPushMatrix();
	glColor3f(0.7f, 0.3f, 0.0f);
	GLfloat planetMarsColor[] = { 0.7f, 0.3f, 0.0f };
	glTranslatef(6.0, 7, 6.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, planetMarsColor);
	glutSolidSphere(1.6, 50, 50);
	glPopMatrix();
	/*Mercury*/
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	GLfloat planetColor[] = { 1.0f, 0.0f, 0.0f };
	glTranslatef(0.0, 7, 5.0);

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, planetColor);
	glutWireSphere(0.5, 50, 50);
	glPopMatrix();



	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, sunColor);
	glScalef(5, 5, 5);
	//glScalef(4, 4, 4);
	glTranslatef(0.0, 0.0, 2.0);
	GLfloat lightpos2[4] = { 0.0,0.0,0.0,0.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, meshColor);
	glEnable(GL_LIGHT1);
	drawFullDepression(1, 0.5, 1);
	//drawQuarterDepression(1, 0.5, 1);

	glPopMatrix();
}
/*End Milky Way System*/
void display() {

	time = (GLfloat)get_secs();


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLoadIdentity();

	//drawAxes();

	if (transitionToMilkyWay == -1.0) {
		drawAxes();
		glTranslatef(0.0, 0.0, 0.0);
		drawText(0.0, 10.0, 0.0, 0.741, 0.717, 0.419, "PROJECT TITLE:");
		drawText(171, 475, 0.0, 0.741, 0.717, 0.419, "DEMONSTRATION OF GRAVITATIONAL WAVES");

		drawText(150.0, 375.0, 0.0, 0.741, 0.717, 0.419, "Developed By:");

		drawText(150.0, 300.0, 0.0, 0.098, 0.098, 0.439, "SREESHA K.S.");
		drawText(190.0, 275.0, 0.0, 0, 0.392, 0, "(1DS13CS744)");

		drawText(150.0, 225.0, 0.0, 0.098, 0.098, 0.439, "SINDHU B.M.");
		drawText(190.0, 200.0, 0.0, 0, 0.392, 0, "(1DS13CS743)");

		drawText(150.0, 150.0, 0.0, 0.098, 0.098, 0.439, "SHWETHA J");
		drawText(190.0, 125.0, 0.0, 0, 0.392, 0, "(1DS13CS741)");
		glFlush();
	}
	else if (transitionToMilkyWay == 0) {
		glDisable(GL_LIGHTING);
		glTranslatef(-100 + translateDistanceX, 0.0f, 50 + translateDistanceZ);


		glPushMatrix();
		drawMilkyWaySystem();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(100.0f, 0.0f, -100.0f);
		glRotatef(cameraAngle, 0.0f, 20.0f, 10.0f);
		drawBinarySystem();
		glPopMatrix();

		cameraAngle -= 0.5f;
	}
	else if (transitionToMilkyWay != 2 && transitionToMilkyWay == 1) {
		glTranslatef(-100 + translateDistanceX, 0.0f, 50 - translateDistanceZ);
		glRotatef(cameraAngle, 0.0f, 20.0f, 10.0f);

		glEnable(GL_LIGHTING);
		//Update TranslateDistance
		if (((50 - translateDistanceZ) >= -50) /*|| ((-100 + translateDistanceX) <= 1.5)*/) {



			glPushMatrix();
			drawMilkyWaySystem();
			glPopMatrix();


			glPushMatrix();
			glTranslatef(100.0f, 0.0f, -100.0f);
			glRotatef(cameraAngle, 0.0f, 20.0f, 10.0f);
			//drawBinarySystem();
			glPopMatrix();

			if ((50 - translateDistanceZ) >= -50) {
				printf("TranlationZ: %f ", (50 - translateDistanceZ));
				translateDistanceZ += 1;
			}
			if (((-100 + translateDistanceX) <= 1.5)) {
				translateDistanceX += 1;
				printf("TranlationX: %f ", (-100 + translateDistanceX));
			}
			cameraAngle -= 0.5f;
		}
		else {

			//printf("TranlationZ: %f + Inside Milky Way Display", (50 - translateDistanceZ));
			//glTranslatef(-100 + translateDistanceX, 0.0f, 50 - translateDistanceZ);
			//glRotatef(cameraAngle, 0.0f, 20.0f, 10.0f);
			if (milyWayTrasitionStartTime == 0.0) {
				//Record The time when the zoom transition Began
				milyWayTrasitionStartTime = time;
			}
			//Compute the total elapsed time of the solarSystem 
			milyWayTrasitionTimeElapsed = time - milyWayTrasitionStartTime;

			glPushMatrix();
			glTranslatef(100.0f, 0.0f, -100.0f);
			glRotatef(cameraAngle, 0.0f, 20.0f, 10.0f);
			//drawBinarySystem();
			glPopMatrix();

			glPushMatrix();
			drawMilkyWaySystem();
			glPopMatrix();
			/*
			Display Explanation
			*/


			//If total Time alloted has been exceeded then transition to next Phaze
			if (milyWayTrasitionTimeElapsed > milkyWayTransitionMaxTime) {
				//Update Earth Focus Values Needed for the next Scene
				currentEarthFocusX = -100 + translateDistanceX;
				currentEarthFocusY = 0.0f;
				currentEarthFocusZ = 50 - translateDistanceZ;

				transitionToMilkyWay = 2;

			}
		}
		cameraAngle -= 0.5f;

	}
	else if (transitionToMilkyWay == 2) {
		if (currentEarthFocusX <= 13 || currentEarthFocusY >= -5.0 || currentEarthFocusZ <= -2.0) {

			glTranslatef(currentEarthFocusX, currentEarthFocusY, currentEarthFocusZ);
			//glRotatef(cameraAngle, 0.0f, 20.0f, 10.0f);
			cameraAngle -= 0.5;
			drawMilkyWaySystem();
			if (currentEarthFocusX <= 13)
				currentEarthFocusX += 0.1;
			if (currentEarthFocusY >= -5.0)
				currentEarthFocusY -= 0.01;
			if (currentEarthFocusZ <= -2.0)
				currentEarthFocusZ += .05;

			/*Display Text Regarding */
		}
		else {
			if (LIGOTrasitionStartTime == 0.0) {
				LIGOTrasitionStartTime = time;
				glutPostRedisplay();
				cameraAngle = 0.0;
				glDisable(GL_LIGHTING);
			}
			if (demoPhaze == 0) {
				if (((int)cameraAngle) % 50 == 0)
					printf("Phaze 1 --- CamAngle : %f", cameraAngle);

				glTranslatef(1.5f, 0.0f, -12.0f);
				glRotatef(cameraAngle, 0.0f, 20.0f, 10.0f);

				renderPhazeOne(time - LIGOTrasitionStartTime);
			}
			else {
				printf("Phaze 2--- CamAngle : %f", cameraAngle);

				drawText(0.0, 2.0, 5.0, 1, 0, 0, "Click Right Mouse Button to Increase Frequency of oscillation");
				drawText(0.0, 2.0, 6.0, 1, 0, 0, "Click The Middle Button to Decrease Frequency of oscillation");

				if (zoomIntoDiffractionPattern == 0) {
					glTranslatef(1.5f, 0.0f, -12.0f);
					drawText(6, 0.0, -3, 1, 0, 0, "Click To Zoom Into Diffraction Pattern");
				}
				else {
					glTranslatef(4.0f, 0.0f, -2.0f);
					drawText(-6, 0.0, +3, 1, 0, 0, "Click To Zoom Out of Diffraction Pattern");
				}
				glRotatef(cameraAngle, 0.0f, 20.0f, 10.0f);

				renderPhazeTwo(time - LIGOTrasitionStartTime);
			}
		}
	}
	else if (transitionToMilkyWay == 3.0) {
		glPushMatrix();
		glTranslatef(0.0, 0.0, -50);
		glRotatef(cameraAngle, 0.0f, 20.0f, 10.0f);
		drawMilkyWaySystem();
		glPopMatrix();


		glPushMatrix();
		glTranslatef(0.0, 0.0, -5);
		glRotatef(-cameraAngle, 0.0f, 60.0f, 10.0f);

		glEnable(GL_BLEND); //Enable blending.
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);

		drawText(0.0, 2.5, -5.0, 1.0, 0.5, 0.0, "Gravitational Waves Simulation");
		drawText(0.0, 1.5, -5.0, 1.0, 0.5, 0.0, "        Developed By          ");
		drawText(0.0, 0.5, -5.0, 1.0, 0.5, 0.0, "    Sreesha KS - 1DS13CS744   ");
		drawText(0.0, -0.5, -5.0, 1.0, 0.5, 0.0, "    Sindhu BM  - 1DS13CS743   ");
		drawText(0.0, -1.5, -5.0, 1.0, 0.5, 0.0, "    Shwetha   - 1DS13CS746    ");
		glPopMatrix();


		cameraAngle -= 0.3;
	}//End ElseIf
	 //drawAxes();
	glutSwapBuffers();
}


void reshape(GLsizei width, GLsizei height) {

	if (height == 0) height = 1;
	GLfloat aspect = (GLfloat)width / (GLfloat)height;


	glViewport(0, 0, width, height);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(90.0f, aspect, 1.0f, 1000.0f);
}
void creditWindowDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLoadIdentity();

	glBegin(GL_POINTS);
	glVertex3f(0.0, 1.0, 0.0);
	glEnd();

	drawText(0.0, 10.0, 0.0, 0.741, 0.717, 0.419, "PROJECT TITLE:");
	drawText(171, 475, 0.0, 0.741, 0.717, 0.419, "DEMONSTRATION OF GRAVITATIONAL WAVES");

	drawText(150.0, 375.0, 0.0, 0.741, 0.717, 0.419, "Developed By:");

	drawText(150.0, 300.0, 0.0, 0.098, 0.098, 0.439, "SREESHA K.S.");
	drawText(190.0, 275.0, 0.0, 0, 0.392, 0, "(1DS13CS744)");

	drawText(150.0, 225.0, 0.0, 0.098, 0.098, 0.439, "SINDHU B.M.");
	drawText(190.0, 200.0, 0.0, 0, 0.392, 0, "(1DS13CS743)");

	drawText(150.0, 150.0, 0.0, 0.098, 0.098, 0.439, "SHWETHA J");
	drawText(190.0, 125.0, 0.0, 0, 0.392, 0, "(1DS13CS741)");
	glFlush();
	glutSwapBuffers();
}
void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (transitionToMilkyWay == 2 && isDisplayingLIGOMotion == 1 && zoomIntoDiffractionPattern == 0) {
			//If not zoomed in then zoom into the pattern
			zoomIntoDiffractionPattern = 1;
			glutPostRedisplay();
		}
		else if (zoomIntoDiffractionPattern == 1) {
			//If zoomed in then zoom into the pattern then zom out
			zoomIntoDiffractionPattern = 0;
			glutPostRedisplay();
		}
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		frequency += 1;
	}
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
		frequency -= 1;
	}
}
void keyboard(unsigned char key, int x, int y) {
	if (key == 's' && transitionToMilkyWay == -1.0) {
		transitionToMilkyWay = 1.0;
		glutPostRedisplay();
	}
	if (key == 'c') {
		transitionToMilkyWay = 3.0;
		glutPostRedisplay();
	}
}
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(50, 50);

	mainWindow = glutCreateWindow(title);
	glutSetWindow(mainWindow);
	glutDisplayFunc(display);

	glutIdleFunc(display);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	initGL();
	glEnable(GL_NORMALIZE);
	glutMainLoop();
	return 0;
}