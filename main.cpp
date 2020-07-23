#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include "imageloader.h"
#include <math.h>
#include <iostream>

static int year1 = 3, year2 = 1, year3 = 2 , day1 = 1 ,day2 = 2 ,day3 = 3, camera = 1;
float  pos1 = 0, pos2 = 0, pos3 = 0, pos4 = 0, zoom  = 10, xc=0 ,yc=0 ,zc=0;
float mouseX = 0, mouseY = 0, mouseXp = 0, mouseYp = 0, movX=0, movY=0, cx = 0, cy = 0;
bool primeiroMov = 1;
GLsizei alt,larg;

   float d2r = 3.14159265 / 180.0;
   static float theta = 0.0, thetar = 0.0, phi = 0.0, phir = 0.0;


float lightPos[] = { 0.0, 0.0, 0.0, 1.0 }; // Spotlight position.
static float spotAngle = 60; // Spotlight cone half-angle.
static float spotExponent = 1.0; // Spotlight exponent = attenuation factor.
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


    glEnable(GL_LIGHTING);
	float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
	float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 };

	float globAmb[] = { 0.5, 0.5, 0.5, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, spotAngle);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, spotExponent);


   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
}

void animate(int n){
	//thetar = theta*d2r;
	pos1 += year1;
	pos2 += year2;
	pos3 += day1;
	pos4 += day2;
	if (pos1, pos2, pos3, pos4 > 360.0){
		pos1, pos2, pos3, pos4 -= 360.0;
	}
    theta +=1;
	glutPostRedisplay();


}

void display(void)
{

   thetar = theta * d2r;


   glLoadIdentity();

   if(camera == 1){
        gluLookAt (cx, 1.5*cy, zoom, cx+xc, cy , 1.0, 0.0, 1.0, 0.0);
   }
   if(camera == 2){
        gluLookAt (cx +xc, zoom , cy +xc, cx, 1.0,cy , 1.0, 0.5, -1.0);
   }
   if(camera == 3){
        gluLookAt (cx+xc, zoom, cy, 1.0, cx + xc , cy, 0.0, 0.0 , 1.0);
   }

   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glColor3f (1.0, 1.0, 1.0);


   GLUquadric *quadric;
   quadric = gluNewQuadric();


   //Sol
   glPushMatrix();
   glRotatef ((GLfloat) pos3, 0.0, 0.0, 1.0);
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
   glTranslatef(1.0,0.0,0.0);
   glTranslatef (3.5*sin(thetar), 2.0*cos(thetar), 0.0f);
   glRotatef ((GLfloat) pos1, 0.0, 0.0, 1.0);

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
   glRotatef ((GLfloat) pos2, 0.0, 0.0, 1.0);
   glTranslatef (5.0, 0.0, 0.0);
   glRotatef ((GLfloat) pos4, 0.0, 0.0, 1.0);

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, terText);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   gluQuadricTexture(quadric, 1);
   gluSphere(quadric, 0.2 , 20.0, 20.0);
   glDisable(GL_TEXTURE_2D);

   glRotatef ((GLfloat) pos2, 0.0, 0.0, 1.0);
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
         xc = xc-0.5;
         glutPostRedisplay();
         break;
      case GLUT_KEY_RIGHT:
         xc = xc+0.5;
         glutPostRedisplay();
         break;
      case GLUT_KEY_UP:
         zoom = zoom-0.5;
         glutPostRedisplay();
         break;
      case GLUT_KEY_DOWN:
         zoom = zoom+0.5;
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
