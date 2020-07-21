#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <math.h>
#include <iostream>

static int year1 = 1, year2 = 2, year3 = 0, day1 = 0 ,day2 = 0 ,day3 = 0, camera =1;
float  pos1 = 0, pos2 = 0, pos3 = 0, pos4 = 0, zoom  = 10, xc=0 ,yc=0 ,zc=0;
float mouseX = 0, mouseY = 0, mouseXp = 0, mouseYp = 0, movX=0, movY=0, cx = 0, cy = 0;
bool primeiroMov = 1;
GLsizei alt,larg;
/*
float d2r = 3.14159265 / 180.0;
static float theta = 0.0, thetar = 0.0, phi = 0.0, phir = 0.0;
*/

float lightPos[] = { 0.0, 0.0, -75.0, 1.0 }; // Spotlight position.
static float spotAngle = 40; // Spotlight cone half-angle.
float spotDirection[] = { 1.0, 0.0, 0.0 }; // Spotlight direction.
static float spotExponent = 1.0; // Spotlight exponent = attenuation factor.

void init(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);
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
	glutPostRedisplay();
    //theta +=10;

}

void display(void)
{
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
   gluSphere(quadric, 1.0 , 20.0, 20.0);
   glPopMatrix();


   //Planeta

   glPushMatrix();
   glRotatef ((GLfloat) pos1 , 0.0, 0.0, 1.0);
   glTranslatef (2.0 ,0.0, 0.0f);
   glRotatef ((GLfloat) day1, 0.0, 0.0, 1.0);
   gluSphere(quadric, 0.2 , 20.0, 20.0);
   glPopMatrix();

   //Planeta Com lua
   glPushMatrix();
   glRotatef ((GLfloat) pos2, 0.0, 0.0, 1.0);
   glTranslatef (5.0, 0.0, 0.0);
   glRotatef ((GLfloat) day2, 0.0, 0.0, 1.0);
   glutSolidSphere(0.2, 10, 8);
   glRotatef ((GLfloat) pos2, 0.0, 0.0, 1.0);
   glTranslatef (1.0, 0.0, 0.0);
   glutSolidSphere(0.05, 10, 8);


   glPopMatrix();

   glPopMatrix();
   glutSwapBuffers();

   glutTimerFunc(30, animate, 1);

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
	if (direction > 0 && zoom > 2){
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
