#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

static int year1 = 0, year2 = 0, year3 = 0, day1 = 0 ,day2 = 0 ,day3 = 0;
GLfloat m[16];

void init(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   glColor3f (1.0, 1.0, 1.0);

   glPushMatrix();
   glutWireSphere(1.0, 20, 16);   /* draw sun */

   glRotatef ((GLfloat) year1, 0.0, 0.0, 1.0);
   glTranslatef (2.0, 0.0, 0.0);

   glRotatef ((GLfloat) day1, 0.0, 0.0, 1.0);
   glutWireSphere(0.2, 10, 8);    /* draw smaller planet */

   glLoadMatrixf(m);
   glRotatef ((GLfloat) year2, 0.0, 0.0, 1.0);
   glTranslatef(4.0,0.0,0.0);
   glRotatef ((GLfloat) day2, 0.0, 0.0, 1.0);
   glutWireSphere(0.2, 10, 8);    /* draw smaller planet */

   glLoadMatrixf(m);
   glRotatef ((GLfloat) year3, 0.0, 0.0, 1.0);
   glTranslatef(6.0,0.0,0.0);
   glRotatef ((GLfloat) day3, 0.0, 0.0, 1.0);
   glutWireSphere(0.2, 10, 8);    /* draw smaller planet */

   glPopMatrix();
   glutSwapBuffers();

   for(int i=0;i<5000000;i++){
   }
    day1 = (day1 + 1) % 360;
    year1 = (year1 + 1) % 360;
    day2 = (day2 + 2) % 360;
    year2 = (year2 + 2) % 360;
    day3 = (day3 + 3) % 360;
    year3 = (year3 + 3) % 360;
    glutPostRedisplay();


}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(90.0, (GLfloat) w/(GLfloat) h, 0.0, 40.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt (0.0, 0.0, 10.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);
   glGetFloatv(GL_MODELVIEW_MATRIX, m);
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
   glutMainLoop();
   return 0;
}
