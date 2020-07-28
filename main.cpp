#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include "imageloader.h"
#include <math.h>
#include <iostream>
#include <vector>
#include <fstream> 
// #include <string>

// std::ifstream texturas("texturas.txt");

static int day1 = 1 ,day2 = 2 ,day3 = 3, camera = 1;
float  rot1 = 0, rot2 = 0, rot3 = 0, zoom  = 10;
float mouseX = 0, mouseY = 0, mouseXp = 0, mouseYp = 0, movX=0, movY=0, cx = 0, cy = 0;
bool primeiroMov = 1, pare = 0;
GLsizei alt,larg;

GLuint orbitaPlan, orbitaLuas;

float d2r = 3.14159265 / 180.0;
static float theta = 0.0, thetar = 0.0, phi = 0.0, phir = 0.0;

static int window;
static int menu_id;
static int submenu_id;
static int texture_submenu_id;

char* current_text;

int stop = 1;

GLuint solText, terText, luaText, venText;

struct planet{

GLdouble radius;
GLdouble day;
GLfloat rotate;
float vel;
char* texture; // a implementar

float m_theta;

float pos;


};

std::vector<planet> planets;



GLfloat lightDir0[] = { -1.0, 0.0, 0.0};
GLfloat lightDir1[] = { 0.0, -1.0, 0.0};
GLfloat lightDir2[] = { 1.0, 0.0, 0.0};
GLfloat lightDir3[] = { 0.0, 1.0, 0.0};

static float spotAngle = 90; // Spotlight cone half-angle.
static float spotExponent = 1.0; // Spotlight exponent = attenuation factor.
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[] = { 75.0 };

void makePlanet(GLdouble radius=1, float vel = 1.0, char *texture = "/home/reynej/CompGraf/final/projetoCG/venus.bmp"){

   
  planets.push_back({radius, 1.0, 0, vel,texture,0, (2 * 3.1415926) /50});
  //std::cout<<"aki!\n"<<radius<<" "<<vel<<"\n";
}


//A funcao menu recebe um int de acordo com o definido dentro d
//glutCreateMenu()
void menu(int num){
  if(num == 0){
    glutDestroyWindow(window);
    exit(0);
  }else if(num < 5){
      makePlanet(((GLdouble) num)/5);
  }else {
     stop = !stop;
  }
  glutPostRedisplay();
} 

void createMenu(void){    
    texture_submenu_id =  glutCreateMenu(menu);
    glutAddMenuEntry("Red", 5);
    glutAddMenuEntry("Blue", 6);
    glutAddMenuEntry("Green", 7);
    submenu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Small", 2);
    glutAddMenuEntry("Medium", 3);
    glutAddMenuEntry("Big", 4);
    menu_id = glutCreateMenu(menu);
    glutAddSubMenu("Texture", texture_submenu_id);
    glutAddSubMenu("Planet", submenu_id);
    glutAddMenuEntry("Stop/Resume", 8);
    glutAddMenuEntry("Quit", 0);     
    glutAttachMenu(GLUT_RIGHT_BUTTON);
} 

GLuint loadTexture(Image* image) {
	//http://www.codeincodeblock.com/2012/05/simple-method-for-texture-mapping-on.html
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	return textureId;
}



static void DrawEllipse(float cx, float cy, float rx, float ry, int num_segments)
{
    float theta = 2 * 3.1415926 / float(num_segments);
    float c = cosf(theta);//precalculate the sine and cosine
    float s = sinf(theta);
    float t;

    float x = 1;//we start at angle = 0
    float y = 0;

    glBegin(GL_LINE_LOOP);
    for(int ii = 0; ii < num_segments; ii++)
    {
        //apply radius and offset
        glVertex2f(x * rx + cx, y * ry + cy);//output vertex

        //apply the rotation matrix
        t = x;
        x = c * x - s * y;
        y = s * t + c * y;
    }
    glEnd();
}


void updatePlanets(void){
  GLUquadric *quadric;
  quadric = gluNewQuadric();

  
  float i = 0.5;
  float k = 0.0;

 for(auto &planet : planets){

    planet.m_theta = (planet.pos) * d2r*stop;

     planet.rotate += planet.day;
     if(planet.rotate > 360.0) planet.rotate = 0.0;

   glPushMatrix();
   glTranslatef(2.0 + i,0.0,0.0);
   glTranslatef ((9.5 + k)*sin(planet.m_theta), (9.0+k)*cos(planet.m_theta), 0.0f);
   glRotatef (planet.rotate, 0.0, 0.0, 1.0);

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, venText);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   gluQuadricTexture(quadric, 1);
   gluSphere(quadric, planet.radius , 20.0, 20.0);
   glDisable(GL_TEXTURE_2D);

   

   glPopMatrix();

   DrawEllipse(2.0 + i, 0.0, 9.5 + k,9.0 + k,50);

   i += 0.5;
   k += 2.5;

   planet.pos += 2.5/k;

 }

}

void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);

   std::vector<std::string> texturas_paths; // Isso irá pegar linha por linha as texturas em texturas.txt
   //O caminho das texturas no txt deve ser: 
   //   /caminho/para/lua.bmp
   //   /caminho/para/sol.bmp
   //   /caminho/para/terra.bmp
   //   /caminho/para/venus.bmp

    // std::getline(texturas, texturas_paths[0]);

    // std::getline(texturas, texturas_paths[1]);
    // std::getline(texturas, texturas_paths[2]);
    // std::getline(texturas, texturas_paths[3]);

    // for(auto linha : texturas_paths) std::cout<<linha<<"\n";

    Image* lua= loadBMP("/home/reynej/CompGraf/final/projetoCG/moon.bmp");//Isso converte para C-style char e carrega bmp
	Image* sol = loadBMP("/home/reynej/CompGraf/final/projetoCG/sun.bmp");
	Image* ter = loadBMP("/home/reynej/CompGraf/final/projetoCG/earth.bmp");
	Image* ven = loadBMP("/home/reynej/CompGraf/final/projetoCG/venus.bmp");

    luaText = loadTexture(lua);
    solText = loadTexture(sol);
    terText = loadTexture(ter);
    venText = loadTexture(ven);


    delete sol;
    delete lua;
	delete ter;
    delete ven;


////////////////////
	float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
	float lightDifAndSpec[] = { 0.5, 0.5, 0.5, 1.0 };

	float globAmb[] = { 0.5, 0.5, 0.5, 1.0 };
	glLightfv((GL_LIGHT0,GL_LIGHT1,GL_LIGHT2,GL_LIGHT3,GL_LIGHT4), GL_AMBIENT, lightAmb);
	glLightfv((GL_LIGHT0,GL_LIGHT1,GL_LIGHT2,GL_LIGHT3,GL_LIGHT4), GL_DIFFUSE, lightDifAndSpec);
	glLightfv((GL_LIGHT0,GL_LIGHT1,GL_LIGHT2,GL_LIGHT3,GL_LIGHT4), GL_SPECULAR, lightDifAndSpec);
	glLightf((GL_LIGHT0,GL_LIGHT1,GL_LIGHT2,GL_LIGHT3,GL_LIGHT4),GL_SPOT_CUTOFF, spotAngle);
	glLightf((GL_LIGHT0,GL_LIGHT1,GL_LIGHT2,GL_LIGHT3,GL_LIGHT4),GL_SPOT_EXPONENT, spotExponent);

   glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,lightDir0);
   glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,lightDir1);
   glLightfv(GL_LIGHT2,GL_SPOT_DIRECTION,lightDir2);
   glLightfv(GL_LIGHT3,GL_SPOT_DIRECTION,lightDir3);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

/////////////////////
   glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_LIGHT1);
   glEnable(GL_LIGHT2);
   glEnable(GL_LIGHT3);
   glEnable(GL_DEPTH_TEST);

////////////////

   orbitaPlan = glGenLists (1);
   glNewList(orbitaPlan, GL_COMPILE);
   DrawEllipse(1.0,0.0,3.5,3.0,50);
   DrawEllipse(1.5,0.0,5.5,5.0,50);
   glEndList();

   orbitaLuas = glGenLists (1);
   glNewList(orbitaLuas, GL_COMPILE);
   DrawEllipse(0.0,0.0,1.0,1.0,20);
   glEndList();
////////////////
}

void animate(int n){
	if(!pare){
	rot1 += day1;
	rot2 += day2;
	rot3 += day3;
	if (rot1,rot2,rot3 > 360.0){
        rot1,rot2,rot3 -= 360.0;
	}
    theta +=3;
    phi+=1;
	}
    glutPostRedisplay();
}

void display(void)
{

   thetar = theta * d2r*stop;
   phir = phi*d2r*stop;

   glLoadIdentity();
   if(camera == 1){
        gluLookAt (cx, cy, zoom, cx, cy , 0.0, 0.0, 1.0, 0.0);

       GLfloat lightPos0[] = { 1.0, 0.0, 0.0, 1.0}; // Spotlight position.
       GLfloat lightPos1[] = { 0.0, 1.0, 0.0, 1.0}; // Spotlight position.
       GLfloat lightPos2[] = { -1.0, 0.0,0.0, 1.0}; // Spotlight position.
       GLfloat lightPos3[] = { 0.0, -1.0,0.0, 1.0}; // Spotlight position.

       glLightfv(GL_LIGHT0,GL_POSITION,lightPos0);
       glLightfv(GL_LIGHT1,GL_POSITION,lightPos1);
       glLightfv(GL_LIGHT2,GL_POSITION,lightPos2);
       glLightfv(GL_LIGHT3,GL_POSITION,lightPos3);

   }
   if(camera == 2){
       gluLookAt (cx, zoom , cy, cx, 0.0,cy , 1.0, -0.5, 1.0);
       GLfloat lightPos0[] = { 1.0, 0.0, 0.0, 1.0}; // Spotlight position.
       GLfloat lightPos1[] = { 0.0, 0.0, 1.0, 1.0}; // Spotlight position.
       GLfloat lightPos2[] = {-1.0, 0.0, 0.0, 1.0}; // Spotlight position.
       GLfloat lightPos3[] = { 0.0, 0.0,-1.0, 1.0}; // Spotlight position.

       glLightfv(GL_LIGHT0,GL_POSITION,lightPos0);
       glLightfv(GL_LIGHT1,GL_POSITION,lightPos1);
       glLightfv(GL_LIGHT2,GL_POSITION,lightPos2);
       glLightfv(GL_LIGHT3,GL_POSITION,lightPos3);
   }
   if(camera == 3){
       gluLookAt (cx,zoom,cy,cx,0.0,cy, 0.0, 0.0 , 1.0);
       GLfloat lightPos0[] = { 1.0, 0.0, 0.0, 1.0}; // Spotlight position.
       GLfloat lightPos1[] = { 0.0, 0.0, 1.0, 1.0}; // Spotlight position.
       GLfloat lightPos2[] = {-1.0, 0.0, 0.0, 1.0}; // Spotlight position.
       GLfloat lightPos3[] = { 0.0, 0.0,-1.0, 1.0}; // Spotlight position.

       glLightfv(GL_LIGHT0,GL_POSITION,lightPos0);
       glLightfv(GL_LIGHT1,GL_POSITION,lightPos1);
       glLightfv(GL_LIGHT2,GL_POSITION,lightPos2);
       glLightfv(GL_LIGHT3,GL_POSITION,lightPos3);
   }
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glColor3f (1.0, 1.0, 1.0);

   GLUquadric *quadric;
   quadric = gluNewQuadric();


   //orbitas
   glCallList(orbitaPlan);

   //Sol
   glEnable(GL_LIGHT4);
   glPushMatrix();
   glRotatef ((GLfloat) rot1, 0.0, 0.0, 1.0);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, solText);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   gluQuadricTexture(quadric, 1);
   gluSphere(quadric, 1.0 , 20.0, 20.0);
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_LIGHT4);
   glPopMatrix();

   //Planeta
   glPushMatrix();
   glTranslatef(1.0,0.0,0.0);
   glTranslatef (3.5*sin(thetar), 3.0*cos(thetar), 0.0f);
   glRotatef ((GLfloat) rot1, 0.0, 0.0, 1.0);

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, venText);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   gluQuadricTexture(quadric, 1);
   gluSphere(quadric, 0.2 , 20.0, 20.0);
   glDisable(GL_TEXTURE_2D);

   glCallList(orbitaLuas);
   glRotatef ((GLfloat) rot3, 0.0, 0.0, 1.0);
   glTranslatef (1.0, 0.0, 0.0);


   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, luaText);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   gluQuadricTexture(quadric, 1);
   gluSphere(quadric, 0.05 , 20.0, 20.0);
   glDisable(GL_TEXTURE_2D);

   glPopMatrix();

   

   glPopMatrix();

   updatePlanets();

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

    if(cx,cy <50 && cx,cy >-50 && !primeiroMov){
        cx-=(movX/50);
        cy+=(movY/50);
    }
    //std::cout<<cx<<" - "<<cy<<std::endl;

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
        glutPostRedisplay();
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
        break;
    case 's':
        pare = 1;
        break;
    case 'd':
        pare = 0;
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
   window = glutCreateWindow("Nosso Lar");
   createMenu();
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
