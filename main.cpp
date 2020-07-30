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

float d2r = 3.14159265 / 180.0; //grau para radiano
static float theta1 = 0.0, thetar1 = 0.0, theta2 = 0.0, thetar2 = 0.0, theta3 = 0.0, thetar3 = 0.0;

static int window;
static int menu_id;
static int submenu_id;
static int texture_submenu_id;

char* current_text;
bool add_ring;
float planTilt = 0.0;

int stop = 1;

char* texturas[] = {"C:\\Users\\lu_fe\\Downloads\\projetoCG-master\\projetoCG-master\\sun.bmp",  //1° Sol
                    "C:\\Users\\lu_fe\\Downloads\\projetoCG-master\\projetoCG-master\\moon.bmp",   //2° Lua
                    "C:\\Users\\lu_fe\\Downloads\\projetoCG-master\\projetoCG-master\\mercury.bmp",   //3° Mercurio
                    "C:\\Users\\lu_fe\\Downloads\\projetoCG-master\\projetoCG-master\\venus.bmp", //4° Venus
                    "C:\\Users\\lu_fe\\Downloads\\projetoCG-master\\projetoCG-master\\earth.bmp", //5° Terra
                    "C:\\Users\\lu_fe\\Downloads\\projetoCG-master\\projetoCG-master\\mars.bmp", //6° Marte
                    "C:\\Users\\lu_fe\\Downloads\\projetoCG-master\\projetoCG-master\\jupiter.bmp", //7° Jupiter
                    "C:\\Users\\lu_fe\\Downloads\\projetoCG-master\\projetoCG-master\\saturn.bmp", //8° Saturno
                    "C:\\Users\\lu_fe\\Downloads\\projetoCG-master\\projetoCG-master\\uranus.bmp", //9° Urano
                    "C:\\Users\\lu_fe\\Downloads\\projetoCG-master\\projetoCG-master\\neptune.bmp", //10° Netuno
                    "C:\\Users\\lu_fe\\Downloads\\projetoCG-master\\projetoCG-master\\pluto.bmp", //11° Plutao
                    "C:\\Users\\lu_fe\\Downloads\\projetoCG-master\\projetoCG-master\\ring.bmp", //12° aneis
                    "C:\\Users\\lu_fe\\Downloads\\projetoCG-master\\projetoCG-master\\stars.bmp", //13° estrelas
                       };

char* curent_text = "C:\\Users\\lu_fe\\Downloads\\projetoCG-master\\projetoCG-master\\earth.bmp"; //A textura que os planetas extras serao desenhados


GLuint solText, merText, terText, luaText, venText, marText, jupText, satText, uraText, netText, pluText, ringText,staText;

struct planet{

//Como os planetas extras sao armazenados

GLdouble radius;
GLdouble day;
GLfloat rotate;
float vel;
GLuint texture;
float m_theta;
float pos;
bool ring;
float tilt;
};
std::vector<planet> planets;


//direcao das luzes que "saem" do sol
GLfloat lightDir0[] = { -1.0, 0.0, 0.0};
GLfloat lightDir1[] = { 0.0, -1.0, 0.0};
GLfloat lightDir2[] = { 1.0, 0.0, 0.0};
GLfloat lightDir3[] = { 0.0, 1.0, 0.0};

static float anguloLuz = 90; // Cone de luz
static float atenuacao = 1.0; // fator de atenuacao
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[] = { 75.0 };


//funcao para carregar a textura da imagem bmp para o opengl
GLuint loadTexture(Image* image) {
	//http://www.codeincodeblock.com/2012/05/simple-method-for-texture-mapping-on.html
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	return textureId;
}

//criar estrutura dos planetas adicionais
void makePlanet(GLdouble radius=1.0, char* texture = "C:\\Users\\lu_fe\\Downloads\\projetoCG-master\\projetoCG-master\\earth.bmp",
                float vel = 1.0, float _tilt = 0.0){

  Image* planet_text = loadBMP(texture);

  planets.push_back({radius, 1.0, 0, vel,loadTexture(planet_text),0, (2 * 3.1415926) /50 ,add_ring, _tilt});

  delete planet_text;
}

//usa os numeros gerados pelo createMenu para realizar tarefas
void menu(int num){
  if(num == 0){
    glutDestroyWindow(window);
    exit(0);
  }else if(num < 6){
      makePlanet(static_cast<GLdouble>(num)/10, curent_text, add_ring, planTilt); //  Cria um planeta com determinado raio, presenca de anel e inclinacao
      return;
  }else if (num < 15){
     curent_text = texturas[num - 4]; //Carrega a textura a ser desenhada
     add_ring = 0;          //anel desligado por default
     planTilt = 5*num;      //inclinacao progressiva quanto mairo numero de planetas
     if(num == 11){
        add_ring = 1;       //somente saturno com anel
     }
  }else {
    stop = !stop;           //paraliza animacao
  }
  glutPostRedisplay();
}

//menu de geracao dos planetas. Primeiro escolhe textura, depois cria o planeta na determinacao do tamanho
void createMenu(void){
    texture_submenu_id =  glutCreateMenu(menu);
    glutAddMenuEntry("Mercury",6);
    glutAddMenuEntry("Venus", 7);
    glutAddMenuEntry("Earth", 8);
    glutAddMenuEntry("Mars", 9);
    glutAddMenuEntry("Jupiter", 10);
    glutAddMenuEntry("Saturn", 11);
    glutAddMenuEntry("Uranus", 12);
    glutAddMenuEntry("Neptune", 13);
    glutAddMenuEntry("Pluto", 14);

    submenu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Small", 2);
    glutAddMenuEntry("Medium", 3);
    glutAddMenuEntry("Big", 4);
    glutAddMenuEntry("Huge", 5);
    menu_id = glutCreateMenu(menu);
    glutAddSubMenu("Texture", texture_submenu_id);
    glutAddSubMenu("Planet", submenu_id);
    glutAddMenuEntry("Stop/Resume", 15);
    glutAddMenuEntry("Quit", 0);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}


//Desenho das trajetorias elipticas das orbitas, translacao eliptica feita por combinacao de seno e cosseno especifica
static void DrawEllipse(float cx, float cy, float rx, float ry, int num_segments)
{
    float theta = 2 * 3.1415926 / float(num_segments);
    float c = cosf(theta);//pre-calcula seno e cosseno do angulo
    float s = sinf(theta);
    float t;

    float x = 1;//angulo inicial de 0 graus
    float y = 0;

    glBegin(GL_LINE_LOOP);
    for(int ii = 0; ii < num_segments; ii++)
    {
        //aplica raio e offset
        glVertex2f(x * rx + cx, y * ry + cy);//vertex de saida

        //aplica matriz de rotacao
        t = x;
        x = c * x - s * y;
        y = s * t + c * y;
    }
    glEnd();
}

//onde desenho dos planetas extras ocorre
void updatePlanets(void){
  GLUquadric *quadric;
  quadric = gluNewQuadric();


  float i = 0.5;
  float k = 0.0;

 for(auto &planet : planets){

    planet.m_theta = (planet.pos) * d2r*stop;  //posicao do planeta, grau pra radiano e paralizacao da animacao

     planet.rotate += planet.day;              //velocidade de rotacao
     if(planet.rotate > 360.0) planet.rotate = 0.0;

   DrawEllipse(2.0 + i, 0.0, 9.5 + k,9.0 + k,50); //desenho da orbita

   glPushMatrix();
   glTranslatef(2.0 + i,0.0,0.0);              //desloca orbita do planeta do centro da elipse, se aproximando de um dos seus focos
   glTranslatef ((9.5 + k)*sin(planet.m_theta), (9.0+k)*cos(planet.m_theta), 0.0f);  //translacao eliptica
   glRotatef (planet.rotate, 0.0, 0.0, 1.0);     //rotacao do planeta

   //aplicacao da textura do planeta
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, planet.texture);  //textura especifica do planeta criado
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   gluQuadricTexture(quadric, 1);
   glRotatef(planet.tilt,1.0,0.0,0.0);  //inclinacao do planeta
   gluSphere(quadric, planet.radius , 20.0, 20.0);   //desenho do planeta
   glRotatef(-planet.tilt,1.0,0.0,0.0); //desfazer inclinacao para colocar possivel anel
   glDisable(GL_TEXTURE_2D);
   if(planet.ring){ //se planeta tiver anel
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, ringText);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        gluDisk(quadric, 0.6,1.0,50,2);
        glDisable(GL_TEXTURE_2D);
   }
   glPopMatrix();



   i += 0.5;
   k += 2.5;

   if(!pare) planet.pos += 2.5/k; //Isso faz a velocidade reduzir proporcionalmente a distancia

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


    //Carrega texturas
	Image* sol = loadBMP(texturas[0]);
    Image* lua = loadBMP(texturas[1]);
	Image* mer = loadBMP(texturas[2]);
	Image* ven = loadBMP(texturas[3]);
	Image* ter = loadBMP(texturas[4]);
	Image* mar = loadBMP(texturas[5]);
	Image* jup = loadBMP(texturas[6]);
	Image* sat = loadBMP(texturas[7]);
	Image* ura = loadBMP(texturas[8]);
	Image* net = loadBMP(texturas[9]);
	Image* plu = loadBMP(texturas[10]);
	Image* rng = loadBMP(texturas[11]);
	Image* sta = loadBMP(texturas[12]);


    luaText = loadTexture(lua);
    solText = loadTexture(sol);
    merText = loadTexture(mer);
    venText = loadTexture(ven);
    terText = loadTexture(ter);
    marText = loadTexture(mar);
    jupText = loadTexture(jup);
    satText = loadTexture(sat);
    uraText = loadTexture(ura);
    netText = loadTexture(net);
    pluText = loadTexture(plu);
    ringText= loadTexture(rng);
    staText= loadTexture(sta);

    delete sol;
    delete lua;
    delete mer;
    delete ven;
	delete ter;
    delete mar;
    delete jup;
    delete sat;
    delete ura;
    delete net;
    delete plu;
    delete rng;
    delete sta;

////////////////////
	float luzAmb[] = { 0.0, 0.0, 0.0, 1.0 };
	float luzDifEspec[] = { 0.5, 0.5, 0.5, 1.0 };
	float globAmb[] = { 0.5, 0.5, 0.5, 1.0 };

	//Todas as 5 luzes com mesmos parametross
	glLightfv((GL_LIGHT0,GL_LIGHT1,GL_LIGHT2,GL_LIGHT3,GL_LIGHT4), GL_AMBIENT, luzAmb);
	glLightfv((GL_LIGHT0,GL_LIGHT1,GL_LIGHT2,GL_LIGHT3,GL_LIGHT4), GL_DIFFUSE, luzDifEspec);
	glLightfv((GL_LIGHT0,GL_LIGHT1,GL_LIGHT2,GL_LIGHT3,GL_LIGHT4), GL_SPECULAR, luzDifEspec);
	glLightf((GL_LIGHT0,GL_LIGHT1,GL_LIGHT2,GL_LIGHT3,GL_LIGHT4),GL_SPOT_CUTOFF, anguloLuz);
	glLightf((GL_LIGHT0,GL_LIGHT1,GL_LIGHT2,GL_LIGHT3,GL_LIGHT4),GL_SPOT_EXPONENT, atenuacao);

	//Luzes "emitidas" pelo sol tem suas direcoes definidas
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
   glEnable(GL_LIGHT4);
   glEnable(GL_DEPTH_TEST);  //z-buffer

///////////////
//Desenho das orbitas dos planetas pre estabelecidos
   orbitaPlan  = glGenLists (1);
   glNewList(orbitaPlan, GL_COMPILE);
   DrawEllipse(0.5,0.0,2.0,1.5,50);
   DrawEllipse(1.0,0.0,3.5,3.0,50);
   DrawEllipse(1.5,0.0,5.5,5.0,50);
   glEndList();

   //Desnho da orbita lunar
   orbitaLuas = glGenLists (1);
   glNewList(orbitaLuas, GL_COMPILE);
   DrawEllipse(0.0,0.0,1.0,1.0,20);
   glEndList();
////////////////
}


//animacao ocorre em intervalo "x" de tempo. atualiza angulo de rotacao e angulo de translacao dos planetas pre estabelecidos
void animate(int n){
	if(!pare){
	rot1 += day1;
	rot2 += day2;
	rot3 += day3;
	if (rot1,rot2,rot3 > 360.0){
        rot1,rot2,rot3 -= 360.0;
	}
    theta1 +=3;
    theta2 +=2;
    theta3 +=1;
	}
    glutPostRedisplay();
}

//Como o "olho" da camera muda, a posicao da luz precisa mudar de acordo
void setLightParameters(GLfloat lightPos0[], GLfloat lightPos1[], GLfloat lightPos2[], GLfloat lightPos3[]){

       glLightfv(GL_LIGHT0,GL_POSITION,lightPos0);
       glLightfv(GL_LIGHT1,GL_POSITION,lightPos1);
       glLightfv(GL_LIGHT2,GL_POSITION,lightPos2);
       glLightfv(GL_LIGHT3,GL_POSITION,lightPos3);

}

//imagem de fundo deve ser estampada em um "quadrado" no fundo do desenho. Z-buffer deve ser desligado para nao ocorrer conflitos na hora do desenho
void setBackground(GLint x, GLint y, GLint z,GLint w){
       glDisable(GL_DEPTH_TEST);
       glDisable(GL_LIGHTING);
       glDepthMask(GL_FALSE);
       glEnable(GL_TEXTURE_2D);
       glBindTexture(GL_TEXTURE_2D, staText);
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	   glBegin(GL_POLYGON);
	   glTexCoord2f(0.0, 0.0); glVertex3i(-x,-y, -z);
	   glTexCoord2f(1.0, 0.0); glVertex3i(x, -y, -z);
	   glTexCoord2f(1.0, 1.0); glVertex3i(x,  y*w, -z*w);  //parametro w serve para inverter sinais quando necessario
	   glTexCoord2f(0.0, 1.0); glVertex3i(-x, y*w, -z*w);
	   glEnd();
       glDisable(GL_TEXTURE_2D);
	   glDepthMask(GL_TRUE);
       glEnable(GL_DEPTH_TEST);
       glEnable(GL_LIGHTING);
}

void getTextureParameters(){

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

}



void display(void)
{
   //atualizacao do angulo em radiano da translacao do planeta
   thetar1 = theta1*d2r*stop;
   thetar2 = theta2*d2r*stop;
   thetar3 = theta3*d2r*stop;
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glColor3f (1.0, 1.0, 1.0);

   glLoadIdentity();

   //cada posicao de camera muda a posicao do olho, de onde ta apontando e do vetor Up
   if(camera == 1){
       gluLookAt (cx, cy, zoom, cx, cy , 0.0, 0.0, 1.0, 0.0);

       GLfloat lightPos0[] = { 1.0, 0.0, 0.0, 1.0}; // Spotlight position.
       GLfloat lightPos1[] = { 0.0, 1.0, 0.0, 1.0}; // Spotlight position.
       GLfloat lightPos2[] = { -1.0, 0.0,0.0, 1.0}; // Spotlight position.
       GLfloat lightPos3[] = { 0.0, -1.0,0.0, 1.0}; // Spotlight position.

       //ajusta luzes e background
       setLightParameters(lightPos0, lightPos1, lightPos2, lightPos3);

       setBackground(100,100,10,1);
   }
   if(camera == 2){
       gluLookAt (cx, zoom, cy +10, cx, 0.0,cy , 0.0, 0.0, 1.0);
       GLfloat lightPos0[] = { 1.0, 0.0, 0.0, 1.0}; // Spotlight position.
       GLfloat lightPos1[] = { 0.0, 0.0, 1.0, 1.0}; // Spotlight position.
       GLfloat lightPos2[] = {-1.0, 0.0, 0.0, 1.0}; // Spotlight position.
       GLfloat lightPos3[] = { 0.0, 0.0,-1.0, 1.0}; // Spotlight position.

       setLightParameters(lightPos0, lightPos1, lightPos2, lightPos3);

       setBackground(400,80,400,-1);
   }
   if(camera == 3){
       gluLookAt (cx,zoom,cy,cx,0.0,cy, 0.0, 0.0 , 1.0);
       GLfloat lightPos0[] = { 1.0, 0.0, 0.0, 1.0}; // Spotlight position.
       GLfloat lightPos1[] = { 0.0, 0.0, 1.0, 1.0}; // Spotlight position.
       GLfloat lightPos2[] = {-1.0, 0.0, 0.0, 1.0}; // Spotlight position.
       GLfloat lightPos3[] = { 0.0, 0.0,-1.0, 1.0}; // Spotlight position.

       setLightParameters(lightPos0, lightPos1, lightPos2, lightPos3);

       setBackground(400,80,400,-1);
   }

   GLUquadric *quadric;
   quadric = gluNewQuadric();

   //orbitas sao desenhadas
   glCallList(orbitaPlan);


   //Sol

   glEnable(GL_LIGHT4);  //luz exclusiva do sol
   glPushMatrix();
   glRotatef ((GLfloat) rot1, 0.0, 0.0, 1.0);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, solText);
   getTextureParameters();
   gluQuadricTexture(quadric, 1);
   gluSphere(quadric, 1.0 , 20.0, 20.0);
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_LIGHT4);
   glPopMatrix();

   //Mercurio
   glPushMatrix();
   glTranslatef(0.5,0.0,0.0);  //offset
   glTranslatef (2.0*sin(thetar1), 1.5*cos(thetar1), 0.0f); //elipse de altura 2.0 e largura 1.5
   glRotatef ((GLfloat) rot1, 0.0, 0.0, 1.0); //rotacao do planeta

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, merText);
   getTextureParameters();
   gluQuadricTexture(quadric, 1);
   gluSphere(quadric, 0.2 , 20.0, 20.0);
   glDisable(GL_TEXTURE_2D);

   glPopMatrix();  //reseta matriz para nao interferir com outros planetas (caso contrario, planeta orbitariam outros planetas)



   //Venus
   glPushMatrix();
   glTranslatef(1.0,0.0,0.0);
   glTranslatef (3.5*sin(thetar2), 3.0*cos(thetar2), 0.0f);
   glRotatef ((GLfloat) rot1, 0.0, 0.0, 1.0);

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, venText);
   getTextureParameters();
   gluQuadricTexture(quadric, 1);
   gluSphere(quadric, 0.2 , 20.0, 20.0);
   glDisable(GL_TEXTURE_2D);

   glPopMatrix();

   //Terra Com lua
   glPushMatrix();
   glTranslatef(1.5,0.0,0.0);
   glTranslatef (5.5*sin(thetar3), 5.0*cos(thetar3), 0.0f);
   glRotatef ((GLfloat) rot2, 0.0, 0.0, 1.0);

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, terText);
   getTextureParameters();
   gluQuadricTexture(quadric, 1);
   gluSphere(quadric, 0.2 , 20.0, 20.0);
   glDisable(GL_TEXTURE_2D);

   //nao ocorre PopMatrix pois lua deve orbitar o planeta Terra
   glCallList(orbitaLuas);
   glRotatef ((GLfloat) rot3, 0.0, 0.0, 1.0);
   glTranslatef (1.0, 0.0, 0.0);


   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, luaText);
   getTextureParameters();
   gluQuadricTexture(quadric, 1);
   gluSphere(quadric, 0.05 , 20.0, 20.0);
   glDisable(GL_TEXTURE_2D);

   glPopMatrix();

   glPopMatrix();  //reseta para matriz original

   updatePlanets(); //desenha todos os planetas extras

   glutSwapBuffers();

   glutTimerFunc(50, animate, 1); //atualiza a cada 50ms
}


void reshape (int w, int h)
{
   alt = (GLsizei) h;
   larg = (GLsizei) w;
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(90.0, (GLfloat) w/(GLfloat) h, 0.1, 1000.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}


//Responsavel pelo zoom
void mouseWheel(int wheel, int direction, int x, int y)
{
	if (direction < 0 && zoom < 60){
      zoom++;
      mouseX, mouseY, mouseXp, mouseYp = 0;
      primeiroMov = 1;
	}
	if (direction > 0 && zoom > 1){
      zoom--;
      mouseX, mouseY, mouseXp, mouseYp = 0;
      primeiroMov = 1;
	}
	glutPostRedisplay();
}


//movimentacao para cima-baixo-esquerda-direita
void MouseMove( int x, int y)
{
    //para nao pegar valores muito dispares ao clicar (pegar valores de cliques passados)
    //usou-se booleano que identifica se eh o primeiro arrasto do mouse naquela interacao
    if(primeiroMov){   //zera movimentacoes de camera
        mouseX = x;
        mouseY = y;
        mouseXp = x;
        mouseYp = y;
        movX = 0;
        movY = 0;
        primeiroMov = 0;
        return;
    }
    mouseX = x; //atualiza para x e y novos
    mouseY = y;
    movX = mouseX - mouseXp;  //diferenca determina a quantidade movimentada pela camera
    movY = mouseY - mouseYp;

    if(cx,cy <50 && cx,cy >-50 && !primeiroMov){ //se nao for primeira interacao, atualiza centros
        cx-=(movX/50);  // foi escolhido como fator de atenuacao da sensibilidade
        cy+=(movY/50);
    }
    //std::cout<<cx<<" - "<<cy<<std::endl;

    mouseXp = x;  //atualiza novo x e y para a posicao passada
    mouseYp = y;

    primeiroMov = 1;  //reseta movimento
	glutPostRedisplay();
}

void MyKeyboardFunc(unsigned char Key, int x, int y)
{
    switch(Key)
    {
    case 'r':  //reseta camera
        cx = 0, cy = 0, zoom = 10, primeiroMov = 1;
        glutPostRedisplay();
        break;
    case '1': //muda posicao inicial da camera
        camera = 1;
        break;
    case '2':
        camera = 2;
        break;
    case '3':
        camera = 3;
        break;
    case '+': //zoom + ou -
        zoom -= 0.5;
        break;
    case '-':
        zoom += 0.5;
        break;
    case 's': //paraliza animacao
        pare = 1;
        break;
    case 'd': //continua
        pare = 0;
        break;
    case ' ':
    break;
    exit(1);
break;
};
}

//movimentacao do centro da camera usando setas do teclado
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
