#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include "imageloader.h"
#include <math.h>
#include <iostream>

static int day1 = 1 ,day2 = 2 ,day3 = 3, camera = 1;
float  rot1 = 0, rot2 = 0, rot3 = 0, zoom  = 10;
float mouseX = 0, mouseY = 0, mouseXp = 0, mouseYp = 0, movX=0, movY=0, cx = 0, cy = 0;
bool primeiroMov = 1;
GLsizei alt,larg;

float d2r = 3.14159265 / 180.0;
static float theta = 0.0, thetar = 0.0, phi = 0.0, phir = 0.0;


float lightPos0[] = { 1.1, 0.0, 0.0, 0.0}; // Spotlight position.
float lightPos1[] = { 0.0, 1.1, 0.0, 0.0}; // Spotlight position.
float lightPos2[] = { 0.0, 0.0, 1.1, 0.0}; // Spotlight position.
float lightPos3[] = { -1.1, 0.0, 0.0, 0.0}; // Spotlight position.
float lightPos4[] = { 0.0, -1.1, 0.0, 0.0}; // Spotlight position.
float lightPos5[] = { 0.0, 0.0, -1.1, 0.0}; // Spotlight position.

float lightDir0[] = { 1.0, 0.0, 0.0};
float lightDir1[] = { 0.0, 1.0, 0.0};
float lightDir2[] = { 0.0, 0.0, 1.0};
float lightDir3[] = { -1.0, 0.0, 0.0};
float lightDir4[] = { 0.0, -1.0, 0.0};
float lightDir5[] = { 0.0, 0.0, -1.0};

static float spotAngle = 180; // Spotlight cone half-angle.
static float spotExponent = 2.0; // Spotlight exponent = attenuation factor.
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[] = { 50.0 };

GLuint loadTexture(Image* image) {
	//http://www.codeincodeblock.com/2012/05/simple-method-for-texture-mapping-on.html
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	return textureId;
}

GLuint solText, terText, luaText, venText;

void init(void)
{


    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);


    Image* lua= loadBMP("C://Users//lu_fe//Downloads//Untitled Folder 1//CG//proj//moon.bmp");
    luaText = loadTexture(lua);
    delete lua;
	Image* sol = loadBMP("C://Users//lu_fe//Downloads//Untitled Folder 1//CG//proj//sun.bmp");
	solText = loadTexture(sol);
	delete sol;
	Image* ter = loadBMP("C://Users//lu_fe//Downloads//Untitled Folder 1//CG//proj//earth.bmp");
	terText = loadTexture(ter);
	delete ter;
	Image* ven = loadBMP("C://Users//lu_fe//Downloads//Untitled Folder 1//CG//proj//venus.bmp");
	venText = loadTexture(ven);
	delete ven;

////////////////////
    glEnable(GL_LIGHTING);
	float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
	float lightDifAndSpec[] = { 0.5, 0.5, 0.5, 1.0 };

	float globAmb[] = { 0.5, 0.5, 0.5, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
	glLightf(GL_LIGHT0,GL_SPOT_CUTOFF, spotAngle);
	glLightf(GL_LIGHT0,GL_SPOT_EXPONENT, spotExponent);

	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDifAndSpec);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightDifAndSpec);
	glLightf(GL_LIGHT1,GL_SPOT_CUTOFF, spotAngle);
	glLightf(GL_LIGHT1,GL_SPOT_EXPONENT, spotExponent);

	glLightfv(GL_LIGHT2, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, lightDifAndSpec);
	glLightfv(GL_LIGHT2, GL_SPECULAR, lightDifAndSpec);
	glLightf(GL_LIGHT2,GL_SPOT_CUTOFF, spotAngle);
	glLightf(GL_LIGHT2,GL_SPOT_EXPONENT, spotExponent);

	glLightfv(GL_LIGHT3, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, lightDifAndSpec);
	glLightfv(GL_LIGHT3, GL_SPECULAR, lightDifAndSpec);
	glLightf(GL_LIGHT3,GL_SPOT_CUTOFF, spotAngle);
	glLightf(GL_LIGHT3,GL_SPOT_EXPONENT, spotExponent);

	glLightfv(GL_LIGHT4, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, lightDifAndSpec);
	glLightfv(GL_LIGHT4, GL_SPECULAR, lightDifAndSpec);
	glLightf(GL_LIGHT4,GL_SPOT_CUTOFF, spotAngle);
	glLightf(GL_LIGHT4,GL_SPOT_EXPONENT, spotExponent);

	glLightfv(GL_LIGHT5, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT5, GL_DIFFUSE, lightDifAndSpec);
	glLightfv(GL_LIGHT5, GL_SPECULAR, lightDifAndSpec);
	glLightf(GL_LIGHT5,GL_SPOT_CUTOFF, spotAngle);
	glLightf(GL_LIGHT5,GL_SPOT_EXPONENT, spotExponent);


   glLightfv(GL_LIGHT0,GL_POSITION,lightPos0);
   glLightfv(GL_LIGHT1,GL_POSITION,lightPos1);
   glLightfv(GL_LIGHT2,GL_POSITION,lightPos2);
   glLightfv(GL_LIGHT3,GL_POSITION,lightPos3);
   glLightfv(GL_LIGHT4,GL_POSITION,lightPos4);
   glLightfv(GL_LIGHT5,GL_POSITION,lightPos5);

   glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,lightDir0);
   glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,lightDir1);
   glLightfv(GL_LIGHT2,GL_SPOT_DIRECTION,lightDir2);
   glLightfv(GL_LIGHT3,GL_SPOT_DIRECTION,lightDir3);
   glLightfv(GL_LIGHT4,GL_SPOT_DIRECTION,lightDir4);
   glLightfv(GL_LIGHT5,GL_SPOT_DIRECTION,lightDir5);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

/////////////////////

   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_LIGHT1);
   glEnable(GL_LIGHT2);
   glEnable(GL_LIGHT3);
   glEnable(GL_LIGHT4);
   glEnable(GL_LIGHT5);
   glEnable(GL_DEPTH_TEST);
}

void animate(int n){
	//thetar = theta*d2r;
	rot1 += day1;
	rot2 += day2;
	rot3 += day3;
	if (rot1,rot2,rot3 > 360.0){
        rot1,rot2,rot3 -= 360.0;
	}
    theta +=3;
    phi+=1;
	glutPostRedisplay();


}

void display(void)
{

   thetar = theta * d2r;
   phir = phi*d2r;

   glLoadIdentity();
   if(camera == 1){
        gluLookAt (cx, cy, zoom, cx, cy , 0.0, 0.0, 1.0, 0.0);
   }
   if(camera == 2){
        gluLookAt (cx, zoom , cy, cx, 0.0,cy , 1.0, 0.5, 1.0);
   }
   if(camera == 3){
        gluLookAt (cx, zoom, cy, 0.0, cx , cy, 0.0, 0.0 , 1.0);
   }

   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glColor3f (1.0, 1.0, 1.0);


   GLUquadric *quadric;
   quadric = gluNewQuadric();
   //Sol
   glPushMatrix();
   glRotatef ((GLfloat) rot1, 0.0, 0.0, 1.0);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, solText);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   gluQuadricTexture(quadric, 1);
   gluSphere(quadric, 1.0 , 20.0, 20.0);
   glDisable(GL_TEXTURE_2D);
   glPopMatrix();

   //Planeta
   glPushMatrix();
   glTranslatef(0.9,0.0,0.0);
   glTranslatef (3.5*sin(thetar), 3.0*cos(thetar), 0.0f);
   glRotatef ((GLfloat) rot1, 0.0, 0.0, 1.0);

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, venText);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   gluQuadricTexture(quadric, 1);
   gluSphere(quadric, 0.2 , 20.0, 20.0);
   glDisable(GL_TEXTURE_2D);

   glPopMatrix();

   //Planeta Com lua
   glPushMatrix();
   glTranslatef(1.5,0.0,0.0);
   glTranslatef (5.5*sin(phir), 5.0*cos(phir), 0.0f);
   glRotatef ((GLfloat) rot2, 0.0, 0.0, 1.0);

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, terText);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   gluQuadricTexture(quadric, 1);
   gluSphere(quadric, 0.2 , 20.0, 20.0);
   glDisable(GL_TEXTURE_2D);

   glRotatef ((GLfloat) rot3, 0.0, 0.0, 1.0);
   glTranslatef (1.0, 0.0, 0.0);

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, luaText);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   gluQuadricTexture(quadric, 1);
   gluSphere(quadric, 0.05 , 20.0, 20.0);
   glDisable(GL_TEXTURE_2D);

   glPopMatrix();

   glPopMatrix();
   glutSwapBuffers();

   glutTimerFunc(50, animate, 1);

}


void reshape (int w, int h)
{
   alt = (GLsizei) h;
   larg = (GLsizei) w;
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(90.0, (GLfloat) w/(GLfloat) h, 0.1, 40.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}



void mouseWheel(int wheel, int direction, int x, int y)
{
	if (direction < 0 && zoom < 60){
      zoom++;
      mouseX, mouseY, mouseXp, mouseYp = 0;
      primeiroMov = 1;
	}
	if (direction > 0 && zoom > -1){
      zoom--;
      mouseX, mouseY, mouseXp, mouseYp = 0;
      primeiroMov = 1;
	}
	glutPostRedisplay();
}



void MouseMove( int x, int y)
{

    if(primeiroMov){
        mouseX = x;
        mouseY = y;
        mouseXp = x;
        mouseYp = y;
        movX = 0;
        movY = 0;
        primeiroMov = 0;
        return;
    }
    mouseX = x;
    mouseY = y;
    movX = mouseX - mouseXp;
    movY = mouseY - mouseYp;

    if(cx,cy <20 && cx,cy >-20 && !primeiroMov){
        cx-=(movX/50);
        cy+=(movY/50);
    }
    std::cout<<cx<<" - "<<cy<<std::endl;

    mouseXp = x;
    mouseYp = y;

    primeiroMov = 1;
	glutPostRedisplay();
}

void MyKeyboardFunc(unsigned char Key, int x, int y)
{
    switch(Key)
    {
    case 'r':
        cx = 0, cy = 0, zoom = 10, primeiroMov = 1;
        break;
    case '1':
        camera = 1;
        break;
    case '2':
        camera = 2;
        break;
    case '3':
        camera = 3;
        break;
    case '+':
        zoom -= 0.5;
        break;
    case '-':
        zoom += 0.5;

    case ' ':
    break;
    exit(1);
break;
};
}

void SpecialKeyboard(int key, int x, int y)
{
   switch (key) {
      case GLUT_KEY_LEFT:
         cx = cx-0.5;
         glutPostRedisplay();
         break;
      case GLUT_KEY_RIGHT:
         cx = cx+0.5;
         glutPostRedisplay();
         break;
      case GLUT_KEY_UP:
         cy = cy-0.5;
         glutPostRedisplay();
         break;
      case GLUT_KEY_DOWN:
         cy = cy+0.5;
         glutPostRedisplay();
         break;
    }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutMouseWheelFunc(mouseWheel);
   glutMotionFunc(MouseMove);
   glutSpecialFunc(SpecialKeyboard);
   glutKeyboardFunc(MyKeyboardFunc);
   glutMainLoop();
   return 0;
}
