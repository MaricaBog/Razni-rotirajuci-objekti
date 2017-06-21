#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/timeb.h>
#include "image.h"

#define EPSILON 0.1
#define PI 3.1415926

/* Imena fajlova sa teksturama. */
#define FILENAME0 "teksture/sky.bmp"
#define FILENAME1 "teksture/2.bmp"
#define FILENAME2 "teksture/zid.bmp"


/* Identifikatori tekstura. */
static GLuint names[3];
static int zid1=0;
static int prom =0;

// velicina crtica za minute
const float minStart  = 0.9f, minEnd = 1.0f;

// velicina crtica za sate
const float stepStart = 0.8f, stepEnd   = 1.0f;

float angleHour = 0, angleMin  = 0, angleSec  = 0;
const float clockR = 1.0f, clockVol  = 2.0f, angle1min = M_PI / 30.0f;

// Uglovi u sfernom koordinatnom sistemu koji odredjuju polozaj vidne 
// tacke.
static float phi, theta, delta_phi, delta_theta,ugao=0,ugao1=0,ugao2=0;

float pomeraj=0.0f, pomeraj1=0.0f,pomeraj3=0.0f, pomeraj4 = 0.0f,pomeraj2=0.0f,pomeraj5=0.0f, pomeraj6=0.0f, pomeraj7=0.0f;
int frameNumber = 0;


//deklaracije funkcija
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width,int height);
void on_display(void);
void on_timer(int value);
static void init_lights();
static void set_material(int id);
void mis(int dugme, int stanje, int x, int y);

static void vetrenjaca();
static void elisa();
static void motor();
static void drzac();
static void ploce();
static void crtajPrsten(float rZ, float rR);
static void crtajZubac(float rZ, float rR, int br_zubaca, float visina_zubca);
static void crtajZupcanik(float rZ, float rR, float visina_zubca, int br_zubaca);
static void initialize(void);
static int zid(float rZ, float rR, int br_zubaca, float visina_zubca);
static void dovodnici();
static void pokretac_sata();
static void vetar();

static void setVertexAndNormal(float t, int indikator, float r, float h);
void newLine(float rStart, float rEnd, float angle);
static void nebo_i_zemlja(float visina_zubca);


int main(int argc, char* argv[]){

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1450,650);
    glutCreateWindow(argv[0]);

    glutDisplayFunc(on_display);
    glutReshapeFunc(on_reshape);
    glutKeyboardFunc(on_keyboard);
    glutTimerFunc(33, on_timer, 1);
    glutMouseFunc(mis);

    phi = theta = 0;
    delta_phi = delta_theta = PI / 90;

    initialize();

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);

    glutMainLoop();

  return 0;
}


void mis(int dugme, int stanje, int x, int y)
{
   int Sdugme = glutGetModifiers();
   
   if((stanje == GLUT_DOWN))
   {
	//levo dugme misa ubrzava elisu
	if(dugme == GLUT_LEFT_BUTTON)	
	{
	  pomeraj2+=10;
          prom++;
	  if(prom>3)
	    prom=0;
        }
	//desno dugme ubrzava torus
        else if(dugme == GLUT_RIGHT_BUTTON)
 	{
	  frameNumber+=30;
	}
   }
   //desno dugme i sift pomeraju zupcanike 
   if((stanje == GLUT_DOWN) && Sdugme == GLUT_ACTIVE_SHIFT)
   {
	if(dugme == GLUT_RIGHT_BUTTON )   
	{
 	  pomeraj7+=10;
	  pomeraj1+=10;
	  pomeraj3+=4;
	}
    }	
  

}

static void initialize(void)
{
    // Objekat koji predstavlja teskturu ucitanu iz fajla.
    Image * image;

    // Ukljucuje se testiranje z-koordinate piksela. 
   // glEnable(GL_DEPTH_TEST);

    // Ukljucuju se teksture.
    glEnable(GL_TEXTURE_2D);

    /*
     * Podesava se rezim iscrtavanja tekstura tako da boje na teksturi
     * potpuno odredjuju boju objekata.
     */
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    /*
     * Inicijalizuje se objekat koji ce sadrzati teksture ucitane iz
     * fajla.
     */
    image = image_init(0, 0);

    // Kreira se prva tekstura. 
    image_read(image, FILENAME0);

    // Generisu se identifikatori tekstura. 
    glGenTextures(3, names);

    glBindTexture(GL_TEXTURE_2D, names[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    // Kreira se druga tekstura. 
    image_read(image, FILENAME1);

    glBindTexture(GL_TEXTURE_2D, names[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    glBindTexture(GL_TEXTURE_2D, 0);

   // Kreira se treca tekstura. 
    image_read(image, FILENAME2);

    glBindTexture(GL_TEXTURE_2D, names[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    // Iskljucujemo aktivnu teksturu 
    glBindTexture(GL_TEXTURE_2D, 1);

    // Unistava se objekat za citanje tekstura iz fajla.
    image_done(image);

}


void on_timer(int value){

	struct timeb tb;
	time_t tim=time(0);
	struct tm* t;
	t=localtime(&tim);
	ftime(&tb);

	angleSec = (float)(t->tm_sec+ (float)tb.millitm/1000.0f)/30.0f * M_PI;
	angleMin = (float)(t->tm_min)/30.0f * M_PI + angleSec/60.0f;
  	angleHour = (float)(t->tm_hour > 12 ? t->tm_hour-12 : t->tm_hour)/6.0f * M_PI + angleMin/12.0f;

	if(pomeraj6>0)
 	{
	 pomeraj6+=0.05;
	 if(pomeraj6>=3)
	 {
	    pomeraj5+=0.1;
	    frameNumber++;
	    pomeraj2+=0.3;

	    pomeraj4+=0.6;
	    if(pomeraj4>2)
	       pomeraj4--;
	    else if(pomeraj4<2)
 	       pomeraj4++;
	  
   	    pomeraj3+=0.24;
	    pomeraj+=0.3; 
	    pomeraj1+=0.6;
	    pomeraj7+=0.3;
	 }
 	}


	if(pomeraj3>=28.7)
	  pomeraj3=0;
  

	glutPostRedisplay();

	glutTimerFunc(50, on_timer, 0);
}


static void on_keyboard(unsigned char key, int x, int y)
{
    switch(key)
     {
     case 27:
		exit(0);
	break;

     case 'p':
     case 'P':
          // Dekrementira se ugao phi i ponovo iscrtava scena. 
         phi -= delta_phi;
         if (phi > PI) 
	 {
            phi -= 2*PI;
         }  else if (phi < 0)
	 {
            phi += 2*PI;
         }
        glutPostRedisplay();
        break;

    case 's':
    case 'S':
        // Resetuju se uglovi phi i theta na pocetne vrednosti. 
        phi = theta = 0;
        glutPostRedisplay();
        break;

    case 't':
                 // Dekrementira se ugao theta i ponovo iscrtava scena. Ovaj
         //ugao se odrzava u intervalu [-89,89] stepeni.
         
        theta -= delta_theta;
        if (theta < -(PI / 2 - PI / 180))
	{
            theta = -(PI / 2 - PI / 180);
        }
        glutPostRedisplay();
        break;

    case 'T':
         // Inkrementira se ugao theta i ponovo iscrtava scena. Ovaj
         // ugao se odrzava u intervalu [-89,89] stepeni.
         
        theta += delta_theta;
        if (theta > PI / 2 - PI / 180) 
	{
            theta = PI / 2 - PI / 180;
        }
        glutPostRedisplay();
        break;

	//reset 
    case 'R':
    case 'r':

	pomeraj1=0;
	pomeraj2=0;
	pomeraj3=0;
	pomeraj4=0;
	pomeraj7=0;
	frameNumber=0;

	glutPostRedisplay();
	break;		

	//kada se pritisne taster k, aktivira se animacija	
    case 'K':
    case 'k':    
	pomeraj6+=0.1;	
	glutPostRedisplay();
 	break;	

    //rotira se prvi zid
     case 'm':
  	ugao++;
	glutPostRedisplay();
	break;

    //rotira se drugi zid
    case 'M':
	ugao1++;
	glutPostRedisplay();
	break;

    //rotira se treci zid
    case'q':
	ugao2++;
	glutPostRedisplay();
	break;


    case 'f':
	pomeraj7+=0.5;
	pomeraj1+=0.5;
	pomeraj3+=0.2;  //pomerac sata
        glutPostRedisplay();
	break;
  }
  

}
   



static void on_reshape(int width,int height)
{
	//postavlja se viewport
	glViewport(0, 0, width, height);
	
	/*podesavanje projekcije*/
	glMatrixMode(GL_PROJECTION);

	/*menja tekucu matricu jedinicnom*/
	glLoadIdentity();
	
	/*postavalja parametre perspektive*/
	/*60-ugao vidnog polja, u pravcu y ose,  razlomak- vidno polje po x osi i kolicnik je sirine i visine,*/
	/*1-bliza ravan - near, 10 dalja ravan -far*/
	gluPerspective(60, width/(float)height, 1, 10);
}



static void vetrenjaca()
{
 
	int br_zubaca=10;
	float rZ=1.0, visina_zubca=2.0;
	float x= (2*rZ*PI)/(1.6*br_zubaca);
	float greska=rZ-sqrt(rZ*rZ-(0.3*x)*(0.3*x));
	visina_zubca+=greska;
	int i=0;
	float pat=0.2*x;
	float s=sqrt(visina_zubca*visina_zubca-(0.2*x)*(0.2*x));

	set_material(3);
        glPushMatrix();
          glTranslatef(0,2.25,-0.03);  
          glutSolidCube(0.53);
        glPopMatrix();
	
	//pod
	glBegin(GL_QUADS);
    		glVertex3f(-0.5*x, 0, -0.3);
   		glNormal3f(0,-1,0);
     
    		glVertex3f(-0.5*x, 0, 0.3);
    		glNormal3f(0,-1,0);

    		glVertex3f(0.5*x, 0, 0.3);
    		glNormal3f(0,-1,0);

    		glVertex3f(0.5*x, 0, -0.3);
    		glNormal3f(0,-1,0);
  	glEnd();


	//krov
	glBegin(GL_QUADS);
		glVertex3f(-0.3*x, visina_zubca, -0.3); 
		glNormal3f(0,1,0);

   		glVertex3f(-0.3*x, visina_zubca, 0.3);
    		glNormal3f(0,1,0);

    		glVertex3f(0.3*x, visina_zubca, 0.3);
    		glNormal3f(0,1,0);

    		glVertex3f(0.3*x, visina_zubca, -0.3);
    		glNormal3f(0,1,0);
  	glEnd();
 
	//lice

	glBegin(GL_QUADS);
    		glVertex3f(-0.5*x, 0, 0.3);
   		glNormal3f(0,0,1);

		glVertex3f(0.5*x, 0, 0.3);
    		glNormal3f(0,0,1);

    		glVertex3f(0.3*x, visina_zubca, 0.3);
    		glNormal3f(0,0,1);

   		glVertex3f(-0.3*x, visina_zubca, 0.3);
    		glNormal3f(0,0,1);
  	glEnd();

  	//nalicje
	glBegin(GL_QUADS);
		glVertex3f(-0.5*x, 0, -0.3);
    		glNormal3f(0,0,-1);

		glVertex3f(0.5*x, 0, -0.3);
    		glNormal3f(0,0,-1);

    		glVertex3f(0.3*x, visina_zubca, -0.3);
    		glNormal3f(0,0,-1);

   		glVertex3f(-0.3*x, visina_zubca, -0.3);
    		glNormal3f(0,0,-1);
  	glEnd();

	//desna strana
	glBegin(GL_QUADS);

	      glVertex3f(0.5*x, 0, 0.3);
    	      glNormal3f(visina_zubca/s,pat/s,0);

    	      glVertex3f(0.5*x, 0, -0.3);
    	      glNormal3f(visina_zubca/s,pat/s,0);

    	      glVertex3f(0.3*x, visina_zubca, -0.3);
	      glNormal3f(visina_zubca/s,pat/s,0);

    	      glVertex3f(0.3*x, visina_zubca, 0.3);
    	      glNormal3f(visina_zubca/s,pat/s,0);
       glEnd();
  
	//leva strana
       glBegin(GL_QUADS);

    	     glVertex3f(-0.5*x, 0, 0.3);
    	     glNormal3f(-visina_zubca/s,pat/s,0);

    	     glVertex3f(-0.5*x, 0, -0.3);
    	     glNormal3f(-visina_zubca/s,pat/s,0);

             glVertex3f(-0.3*x, visina_zubca, -0.3);
	     glNormal3f(-visina_zubca/s,pat/s,0);

   	     glVertex3f(-0.3*x, visina_zubca, 0.3);
    	     glNormal3f(-visina_zubca/s,pat/s,0);
  	glEnd();

	//elisa
          glRotatef(5,0,1,0);
  	  glTranslatef(0.05, 2.2, -0.32);
	  glRotated(-pomeraj2 * (180.0/140.0), 0, 0, 1);
        
	    for (i =1; i < 9; i++) 
	    {
	        glRotatef(45*i,0,0,1);
		glScalef(0.035,0.035,0.035);
		 glutSolidSphere(3.5,30,30);
		 elisa();
		glRotatef(-45*i,0,0,1);
		glScalef(1/0.035,1/0.035,1/0.035);
	    }

	  glRotated(pomeraj2 * (180.0/140.0), 0, 0, 1);
  	  glTranslatef(-0.05, -2.2, 0.32);
          glRotatef(-5,0,1,0);

}

static void elisa()
{
	set_material(0);

	int br_zubaca=4;
	float rZ=10.0,visina_zubca=30.0;
	float x= (2*rZ*PI)/(1.6*br_zubaca);
	float greska=rZ-sqrt(rZ*rZ-(0.3*x)*(0.3*x));
	visina_zubca+=greska;

	//pod
	glBegin(GL_QUADS);
    		glVertex3f(-0.4*x, 0, -0.5);
   		glNormal3f(0,-1,0);

    		glVertex3f(-0.4*x, 0, 0.5);
    		glNormal3f(0,-1,0);

    		glVertex3f(0.4*x, 0, 0.5);
    		glNormal3f(0,-1,0);

    		glVertex3f(0.4*x, 0, -0.5);
    		glNormal3f(0,-1,0);
  	glEnd();

	//krov
	glBegin(GL_QUADS);
		glVertex3f(-0.5*x, visina_zubca, -0.5);
		glNormal3f(0,1,0);
        
   		glVertex3f(-0.5*x, visina_zubca, 0.5);
    		glNormal3f(0,1,0);
        
    		glVertex3f(0.5*x, visina_zubca, 0.5);
    		glNormal3f(0,1,0);
        
    		glVertex3f(0.5*x, visina_zubca, -0.5);
    		glNormal3f(0,1,0);
  	glEnd();
 
	//lice
	glBegin(GL_QUADS);
           	glVertex3f(-0.1*x, 0, 0.5);
   		glNormal3f(0,0,1);

		glVertex3f(0.1*x, 0, 0.5);
    		glNormal3f(0,0,1);

    		glVertex3f(0.5*x, visina_zubca, 0.5);
    		glNormal3f(0,0,1);

   		glVertex3f(-0.5*x, visina_zubca, 0.5);
    		glNormal3f(0,0,1);
  	glEnd();

  	//nalicje
	glBegin(GL_QUADS);
		glVertex3f(-0.1*x, 0, -0.5);
    		glNormal3f(0,0,-1);

		glVertex3f(0.1*x, 0, -0.5);
    		glNormal3f(0,0,-1);

    		glVertex3f(0.5*x, visina_zubca, -0.5);
    		glNormal3f(0,0,-1);

   		glVertex3f(-0.5*x, visina_zubca, -0.5);
    		glNormal3f(0,0,-1);
  	glEnd();

	//desna strana
	float pat=0.2*x;
	float s=sqrt(visina_zubca*visina_zubca-(0.2*x)*(0.2*x));
  
	glBegin(GL_QUADS);
	      glVertex3f(0.4*x, 0, 0.5);
    	      glNormal3f(visina_zubca/s,pat/s,0);

    	      glVertex3f(0.4*x, 0, -0.5);
    	      glNormal3f(visina_zubca/s,pat/s,0);

    	      glVertex3f(0.5*x, visina_zubca, -0.5);
	      glNormal3f(visina_zubca/s,pat/s,0);

    	      glVertex3f(0.5*x, visina_zubca, 0.5);
    	      glNormal3f(visina_zubca/s,pat/s,0);
       glEnd();
  
	//leva strana
       glBegin(GL_QUADS);
    	     glVertex3f(-0.4*x, 0, 0.5);
    	     glNormal3f(-visina_zubca/s,pat/s,0);

    	     glVertex3f(-0.4*x, 0, -0.5);
    	     glNormal3f(-visina_zubca/s,pat/s,0);

             glVertex3f(-0.5*x, visina_zubca, -0.5);
	     glNormal3f(-visina_zubca/s,pat/s,0);

   	     glVertex3f(-0.5*x, visina_zubca, 0.5);
    	     glNormal3f(-visina_zubca/s,pat/s,0);
  	glEnd();

}

static void motor()
{
	//drzac 1
	glPushMatrix();
	  set_material(6);
          glTranslatef(-0.8,0.5,-0.7); 
          glRotatef(70,0,1,0);
	  drzac();
	glPopMatrix();

	//drzac 2
	glPushMatrix();
	  set_material(6);
          glTranslatef(-1.33,0.5,0.7); 
          glRotatef(70,0,1,0);
	  drzac();
	glPopMatrix();

	//ploce unutar kocke kroz koje prolazi energija
        ploce();
	
	//kocka
	glEnable(GL_BLEND);
	glBlendFunc(GL_DST_COLOR,GL_ONE);
	glPushMatrix();
	  set_material(4);
	  glColor3f(0,0.2,0.7);
	  glTranslatef(-1,-0.5,0);
          glRotatef(-20,0,1,0);
	  glutSolidCube(2);
	glPopMatrix();
	glDisable(GL_BLEND);


	//rotirajuci torus koji proizvodi energiju za sat
          glPushMatrix();
 	   glRotatef(-120,0,1,0);
	   glTranslatef(0.53,1.6,0.9);
	   set_material(3);

  	   for(int i=0;i<10;i++)
	   {
	    glRotated(frameNumber * (180.0/(130.0-i)), 1, 0, 0);
            glutSolidTorus(0.01,0.66,15,20);
	   }
       	 glPopMatrix();     
}

static void drzac()
{
	glColor3f(1,0.2,0.3);
	int br_zubaca=5;
	float rZ=0.1,visina_zubca=1.2;
	float x = (2*rZ*PI)/(1.6*br_zubaca);
	float greska=rZ-sqrt(rZ*rZ-(0.3*x)*(0.3*x));
	visina_zubca+=greska;
	
	//pod
	glBegin(GL_QUADS);
    		glVertex3f(-1*x, 0, -0.5);
   		glNormal3f(0,-1,0);
    		glVertex3f(-1*x, 0, 0.5);
    		glNormal3f(0,-1,0);
    		glVertex3f(1*x, 0, 0.5);
    		glNormal3f(0,-1,0);
    		glVertex3f(1*x, 0, -0.5);
    		glNormal3f(0,-1,0);
  	glEnd();

	//krov
	glBegin(GL_QUADS);
		glVertex3f(-1*x, visina_zubca, -0.5);
		glNormal3f(0,1,0);
   		glVertex3f(-1*x, visina_zubca, 0.5);
    		glNormal3f(0,1,0);
    		glVertex3f(1*x, visina_zubca, 0.5);
    		glNormal3f(0,1,0);
    		glVertex3f(1*x, visina_zubca, -0.5);
    		glNormal3f(0,1,0);
  	glEnd();
 
	//lice
	glBegin(GL_QUADS);

    		glVertex3f(-1*x, 0, 0.5);
   		glNormal3f(0,0,1);
       		
		glVertex3f(1*x, 0, 0.5); 
    		glNormal3f(0,0,1);
		
    		glVertex3f(1*x, visina_zubca, 0.5);
    		glNormal3f(0,0,1);

   		glVertex3f(-1*x, visina_zubca, 0.5);
    		glNormal3f(0,0,1);
  	glEnd();

  	//nalicje
	glBegin(GL_QUADS);

		glVertex3f(-1*x, 0, -0.5);
    		glNormal3f(0,0,-1);
	
		glVertex3f(1*x, 0, -0.5);
    		glNormal3f(0,0,-1);
		
    		glVertex3f(1*x, visina_zubca, -0.5);
    		glNormal3f(0,0,-1);
		
   		glVertex3f(-1*x, visina_zubca, -0.5);
    		glNormal3f(0,0,-1);
  	glEnd();

	//desna strana
	float pat=0.2*x;
	float s=sqrt(visina_zubca*visina_zubca-(0.2*x)*(0.2*x));
  
	glBegin(GL_QUADS);
	      glVertex3f(1*x, 0, 0.5);
    	      glNormal3f(visina_zubca/s,pat/s,0);
    	      glVertex3f(1*x, 0, -0.5);
    	      glNormal3f(visina_zubca/s,pat/s,0);
    	      glVertex3f(1*x, visina_zubca, -0.5);
	      glNormal3f(visina_zubca/s,pat/s,0);
    	      glVertex3f(1*x, visina_zubca, 0.5);
    	      glNormal3f(visina_zubca/s,pat/s,0);
       glEnd();
  
	//leva strana
       glBegin(GL_QUADS);
    	     glVertex3f(-1*x, 0, 0.5);
    	     glNormal3f(-visina_zubca/s,pat/s,0);
    	     glVertex3f(-1*x, 0, -0.5);
    	     glNormal3f(-visina_zubca/s,pat/s,0);
             glVertex3f(-1*x, visina_zubca, -0.5);
	     glNormal3f(-visina_zubca/s,pat/s,0);
   	     glVertex3f(-1*x, visina_zubca, 0.5);
    	     glNormal3f(-visina_zubca/s,pat/s,0);
  	glEnd();
}

static void vetar()
{
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0b1100100011001110);

	set_material(2);
	glTranslatef(0,0,pomeraj6);
	glRotatef(90,1,0,0);
	glBegin(GL_LINE_STRIP);
          for(int i=0;i<10;i++)
	  {
		glVertex3f(0.2*i,-10,-1.5);
		glVertex3f(0.2*i,-8,-1.5);
	  }	
	glEnd();

	glRotatef(-90,1,0,0);
	glTranslatef(0,0,-pomeraj6);
	
	glDisable(GL_LINE_STIPPLE);
}

static void ploce()
{

	//energija
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0b1100100011001110);

	glPushMatrix();
          set_material(1);
   	  glTranslatef(-0.9,0,-0.7);
   	  glRotatef(90,0,0,1);
    	  glRotatef(70,1,0,0);
	  drzac();
	glPopMatrix();

	glPushMatrix();
        set_material(1);
   	  glTranslatef(-0.9,-0.3,-0.7);
	  glRotatef(90,0,0,1);
    	  glRotatef(70,1,0,0);
	  drzac();   
	glPopMatrix();


	glPushMatrix();
        set_material(1);
   	  glTranslatef(-0.9,-0.6,-0.7);
    	  glRotatef(90,0,0,1);
    	  glRotatef(70,1,0,0);
	 drzac();
	glPopMatrix();

	glPushMatrix();
        set_material(1);
   	  glTranslatef(-0.9,-0.9,-0.7);
    	  glRotatef(90,0,0,1);
    	  glRotatef(70,1,0,0);
	 drzac();
	glPopMatrix();

	//energija
	set_material(2);
	for(int i=0;i<7;i++)
	{
	  glTranslatef(0,0,-0.4);
	  glRotatef(-20,0,1,0);
	  glTranslatef(0,-pomeraj4/10,0);
	
	  glBegin(GL_LINE_STRIP);
		glVertex3f(-1,0.4,0.2*i);
	        glVertex3f(-1,-1,0.2*i);
	 glEnd();

	  glTranslatef(0,pomeraj4/10,0);
	  glRotatef(20,0,1,0);
	  glTranslatef(0,0,0.4);
}

	 glDisable(GL_LINE_STIPPLE);

}


static void crtajZupcanik(float rZ, float rR, float visina_zubca, int br_zubaca)
{ 
	 crtajPrsten(rZ, rR);
 
	 float x= (2*rZ*PI)/(1.6*br_zubaca);
  	 int i=0;
  	 double delta_angle= 360.0/br_zubaca;
  	 double angle=0.0;
 
	 for (;i < br_zubaca; i++) {
	   glPushMatrix();
   	      glRotatef(angle, 0,0,1);
    	      glTranslatef(0,0.95*sqrt(rZ*rZ-(0.3*x)*(0.3*x)), 0);
    	      crtajZubac(rZ,rR,br_zubaca,visina_zubca);
   	   glPopMatrix();
    
	angle+=delta_angle;
  }
}

static void setVertexAndNormal(float t, int indikator, float r, float h) 
{   
	 if (indikator == 1)
	 { 
	      glNormal3f(r*cos(t), r*sin(t), 0);
	 }
         else if (! indikator) 
	{
     	      glNormal3f(0,0,h);
    	}
   	 else 
	{
	      glNormal3f(r*cos(t), r*sin(t), 0);
        }
    		
   	glVertex3f(r*cos(t), r*sin(t), h);
     
}


static void crtajPrsten(float rZ, float rR) {

	//crtamo lice
	float t, h=0.1;  

	glBegin(GL_TRIANGLE_STRIP);
	for (t=0; t <= 2*PI + EPSILON; t+=PI/90) 
        {
        	setVertexAndNormal(t, 0, rR, h);
        	setVertexAndNormal(t, 0, rZ, h);
        }
    	glEnd();

	//crtamo nalicje
	h=-0.1;

	glBegin(GL_TRIANGLE_STRIP);
	for (t=0; t <= 2*PI + EPSILON; t+=PI/90)
	{
     	 	setVertexAndNormal(t, 0, rR, h);
     	 	setVertexAndNormal(t, 0, rZ, h);
    	}
    	glEnd();
    
	//crtamo unustrasnju traku
	glBegin(GL_TRIANGLE_STRIP);
    	for (t=0; t <= 2*PI + EPSILON; t+=PI/90) 
	{
	      setVertexAndNormal(t, 1, rR, h);
	      setVertexAndNormal(t, 1, rR, h+0.2);
    	}
    	glEnd();
    
	//crtamo spoljasnju traku
	glBegin(GL_TRIANGLE_STRIP);
    	for (t=0; t <= 2*PI + EPSILON; t+=PI/90) 
	{
      	      setVertexAndNormal(t, 2, rZ, h);
      	      setVertexAndNormal(t, 2, rZ, h+0.2);
    	}
    	glEnd();

}


static void crtajZubac(float rZ, float rR, int br_zubaca, float visina_zubca){

	float x= (2*rZ*PI)/(1.6*br_zubaca);
	float greska=rZ-sqrt(rZ*rZ-(0.3*x)*(0.3*x));
	visina_zubca+=greska;
	
	//pod
	glBegin(GL_QUADS);
    		glVertex3f(-0.5*x, 0, -.09);
   		glNormal3f(0,-1,0);
    		glVertex3f(-0.5*x, 0, .09);
    		glNormal3f(0,-1,0);
    		glVertex3f(0.5*x, 0, .09);
    		glNormal3f(0,-1,0);
    		glVertex3f(0.5*x, 0, -.09);
    		glNormal3f(0,-1,0);
  	glEnd();

	//krov
	glBegin(GL_QUADS);
		glVertex3f(-0.3*x, visina_zubca, -.09);
		glNormal3f(0,1,0);
   		glVertex3f(-0.3*x, visina_zubca, .09);
    		glNormal3f(0,1,0);
    		glVertex3f(0.3*x, visina_zubca, .09);
    		glNormal3f(0,1,0);
    		glVertex3f(0.3*x, visina_zubca, -.09);
    		glNormal3f(0,1,0);
  	glEnd();
 
	//lice
	glBegin(GL_QUADS);

    		glVertex3f(-0.5*x, 0, .09);
   		glNormal3f(0,0,1);
       		
		glVertex3f(0.5*x, 0, .09);
    		glNormal3f(0,0,1);
		
    		glVertex3f(0.3*x, visina_zubca, .09);
    		glNormal3f(0,0,1);

   		glVertex3f(-0.3*x, visina_zubca, .09);
    		glNormal3f(0,0,1);
  	glEnd();

  	//nalicje
	glBegin(GL_QUADS);

		glVertex3f(-0.5*x, 0, -.09);
    		glNormal3f(0,0,-1);
	
		glVertex3f(0.5*x, 0, -.09);
    		glNormal3f(0,0,-1);
		
    		glVertex3f(0.3*x, visina_zubca, -.09);
    		glNormal3f(0,0,-1);
		
   		glVertex3f(-0.3*x, visina_zubca, -.09);
    		glNormal3f(0,0,-1);
  	glEnd();

	//desna strana
	float pat=0.2*x;
	float s=sqrt(visina_zubca*visina_zubca-(0.2*x)*(0.2*x));
  
	glBegin(GL_QUADS);
	      glVertex3f(0.5*x, 0, .09);
    	      glNormal3f(visina_zubca/s,pat/s,0);
    	      glVertex3f(0.5*x, 0, -.09);
    	      glNormal3f(visina_zubca/s,pat/s,0);
    	      glVertex3f(0.3*x, visina_zubca, -.09);
	      glNormal3f(visina_zubca/s,pat/s,0);
    	      glVertex3f(0.3*x, visina_zubca, .09);
    	      glNormal3f(visina_zubca/s,pat/s,0);
       glEnd();
  
	//leva strana
       glBegin(GL_QUADS);
    	     glVertex3f(-0.5*x, 0, .09);
    	     glNormal3f(-visina_zubca/s,pat/s,0);
    	     glVertex3f(-0.5*x, 0, -.09);
    	     glNormal3f(-visina_zubca/s,pat/s,0);
             glVertex3f(-0.3*x, visina_zubca, -.09);
	     glNormal3f(-visina_zubca/s,pat/s,0);
   	     glVertex3f(-0.3*x, visina_zubca, .09);
    	     glNormal3f(-visina_zubca/s,pat/s,0);
  	glEnd();

}

void newLine(float rStart, float rEnd, float angle)
{
	float c = cos(angle)/1.1;
 	float s = sin(angle);
  
  	glVertex2f( clockR * rStart * c, clockR * rStart * s);
  	glVertex2f( clockR * rEnd * c, clockR * rEnd * s);
}


static void nebo_i_zemlja(float visina_zubca)
{
	float rZ=13;
	int br_zubaca=10;

	float x= (2*rZ*PI)/(1.6*br_zubaca);
	float greska=rZ-sqrt(rZ*rZ-(0.3*x)*(0.3*x));
	visina_zubca+=greska;

	//nebo
	glPushMatrix();
         glBindTexture(GL_TEXTURE_2D, names[0]);
  
	 glRotatef(90,0,1,0);

	glBegin(GL_QUADS);
	        glTexCoord2f(0, 0);
		glVertex3f(-3*x, -2, -4);
    		glNormal3f(0,0,-1);

       		glTexCoord2f(1, 0);	
		glVertex3f(3*x, -2, -4);
    		glNormal3f(0,0,-1);

		glTexCoord2f(1, 1);
    		glVertex3f(1*x, visina_zubca, -4);
    		glNormal3f(0,0,-1);

		glTexCoord2f(0, 1);
   		glVertex3f(-1*x, visina_zubca, -4);
    		glNormal3f(0,0,-1);
  	glEnd();

	glPopMatrix();

	//zemlja
        glPushMatrix();
          glBindTexture(GL_TEXTURE_2D, names[1]);
 	  glRotatef(90,0,1,0);
	  glRotatef(90,1,0,0);
   	  glTranslatef(0,0,5);

	glBegin(GL_QUADS);
	        glTexCoord2f(0, 0);
		glVertex3f(-3*x, 1, -3);
    		glNormal3f(0,0,-1);

       		glTexCoord2f(1, 0);	
		glVertex3f(3*x, 1, -3);
    		glNormal3f(0,0,-1);

		glTexCoord2f(1, 1);
    		glVertex3f(1*x, -visina_zubca, -3);
    		glNormal3f(0,0,-1);

		glTexCoord2f(0, 1);
   		glVertex3f(-1*x, -visina_zubca, -3);
    		glNormal3f(0,0,-1);
  	glEnd();

	glPopMatrix();

        glBindTexture(GL_TEXTURE_2D, 0); 

}

static int zid(float rZ, float rR, int br_zubaca, float visina_zubca){
{
	float x= (2*rZ*PI)/(1.6*br_zubaca);
	float greska=rZ-sqrt(rZ*rZ-(0.3*x)*(0.3*x));
	visina_zubca+=greska;
	glBindTexture(GL_TEXTURE_2D, names[2]);

	//pod
	glBegin(GL_QUADS);
	        glTexCoord2f(0, 0);
    		glVertex3f(-0.2*x, -1.99, 1);
   		glNormal3f(0,-1,0);
	        glTexCoord2f(1, 0);
    		glVertex3f(-0.2*x, -1.99, 0.5);
    		glNormal3f(0,-1,0);
	        glTexCoord2f(1, 1);
    		glVertex3f(0.2*x, -1.99, 0.5);
    		glNormal3f(0,-1,0);
	        glTexCoord2f(0, 1);
    		glVertex3f(0.2*x, -1.99, 1);
    		glNormal3f(0,-1,0);
  	glEnd();

	//krov
	glBegin(GL_QUADS);
	        glTexCoord2f(0, 0);
		glVertex3f(-0.2*x, visina_zubca-2.9, 1);
		glNormal3f(0,1,0);
	        glTexCoord2f(1, 0);
   		glVertex3f(-0.2*x, visina_zubca-2.9, 0.5);
    		glNormal3f(0,1,0);
	        glTexCoord2f(1, 1);
    		glVertex3f(0.2*x, visina_zubca-2.9, 0.5);
    		glNormal3f(0,1,0);
	        glTexCoord2f(0, 1);
    		glVertex3f(0.2*x, visina_zubca-2.9, 1);
    		glNormal3f(0,1,0);
  	glEnd();
 
	//lice
	glBegin(GL_QUADS);
	        glTexCoord2f(0, 0);
    		glVertex3f(-0.2*x, -1.99, 1);
   		glNormal3f(0,0,1);

	        glTexCoord2f(1, 0);
		glVertex3f(0.2*x, -1.99, 1);
    		glNormal3f(0,0,1);

	        glTexCoord2f(1, 1);		
    		glVertex3f(0.2*x, visina_zubca-2.9, 1);
    		glNormal3f(0,0,1);

	        glTexCoord2f(0, 1);
   		glVertex3f(-0.2*x, visina_zubca-2.9, 1);
    		glNormal3f(0,0,1);
  	glEnd();

  	//nalicje
	glBegin(GL_QUADS);
	        glTexCoord2f(0, 0);
		glVertex3f(-0.2*x, -1.99, 0.5);
    		glNormal3f(0,0,-1);
	
	        glTexCoord2f(1, 0);
		glVertex3f(0.2*x, -1.99, 0.5);
    		glNormal3f(0,0,-1);
		
	        glTexCoord2f(1, 1);
    		glVertex3f(0.2*x, visina_zubca-2.9, 0.5);
    		glNormal3f(0,0,-1);
		
	        glTexCoord2f(0, 1);
   		glVertex3f(-0.2*x, visina_zubca-2.9, 0.5);
    		glNormal3f(0,0,-1);
  	glEnd();

	//desna strana
	float pat=0.2*x;
	float s=sqrt(visina_zubca*visina_zubca-(0.2*x)*(0.2*x));
  
	glBegin(GL_QUADS);
	      glTexCoord2f(0, 0);
	      glVertex3f(0.2*x, -1.99, 1);
    	      glNormal3f(visina_zubca/s,pat/s,0);

    	      glTexCoord2f(1, 0);
	      glVertex3f(0.2*x, -1.99, 0.5);
    	      glNormal3f(visina_zubca/s,pat/s,0);

	      glTexCoord2f(1, 1);
    	      glVertex3f(0.2*x, visina_zubca-2.9, 0.5);
	      glNormal3f(visina_zubca/s,pat/s,0);

	      glTexCoord2f(0, 1);
    	      glVertex3f(0.2*x, visina_zubca-2.9, 1);
    	      glNormal3f(visina_zubca/s,pat/s,0);
       glEnd();
  
	//leva strana
       glBegin(GL_QUADS);
	     glTexCoord2f(0, 0);
    	     glVertex3f(-0.2*x, -1.99, 0.5);
    	     glNormal3f(-visina_zubca/s,pat/s,0);

    	     glTexCoord2f(1, 0);
	     glVertex3f(-0.2*x, -1.99, 1);
    	     glNormal3f(-visina_zubca/s,pat/s,0);

	     glTexCoord2f(1, 1);
             glVertex3f(-0.2*x, visina_zubca-2.9, 1);
	     glNormal3f(-visina_zubca/s,pat/s,0);

	     glTexCoord2f(0, 1);
   	     glVertex3f(-0.2*x, visina_zubca-2.9, 0.5);
    	     glNormal3f(-visina_zubca/s,pat/s,0);
  	glEnd();
}

          glBindTexture(GL_TEXTURE_2D, 1);
   	  return 1;
}

static void dovodnici()
{
	//kablovi koji prenose energiju
	set_material(2);
	glPushMatrix();
	 glBegin(GL_LINES);
	
	  //kabl od vetrenjace do motora
	  glVertex3f(0.7,-2,-2.8);
	  glVertex3f(0.7,-2.2,-2.8);
	  glVertex3f(0.7,-2.2,-2.8);
	  glVertex3f(-1.5,-2.2,0.2);
	  glVertex3f(-1.5,-2.2,0.2);
	  glVertex3f(-1.5,-2,0.2);
        
	  //kabl od motora do pokretaca sata
	  glVertex3f(-1.5,-2.2,0.4);
	  glVertex3f(-1.5,-2,0.4);
	  glVertex3f(-1.5,-2.2,0.4);
	  glVertex3f(-1.5,-2.2,3);
	  glVertex3f(-1.5,-2.2,3);
	  glVertex3f(-1.5,-2,3);

 	 glEnd();
 	glPopMatrix();

}

static void pokretac_sata()
{
	//takt koji pokrece zupcanike
	set_material(6);
	glPushMatrix();
	  glRotatef(90,0,1,0);
	  glTranslatef(-2.07,-1.82,-0.5);	
	  
	  drzac();
	glPopMatrix();

	set_material(6);
	glPushMatrix();
	  glRotatef(90,0,1,0);
	  glRotatef(90,0,0,1);
	  glTranslatef(-1.9,1.99,-0.5);	
	
   	  drzac();
	glPopMatrix();

	set_material(0);
	glTranslatef(-0.2,-1,2);
        glRotatef(pomeraj3*1.9/2.4,1,0,0);
	glLineWidth(2);	

	glBegin(GL_LINES);
  	  glVertex3f(0,0,0);
	  glVertex3f(0,1.04,0.94);
	glEnd();

	glTranslatef(0.2,1,-2);

}


void on_display(void){

	int i=0;
        
        init_lights();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);


 	// Postavljaju se vidni parametri. 
    	gluLookAt(5 * cos(theta) * cos(phi),
              5 * cos(theta) * sin(phi),
              5 * sin(theta),
              0, 0, 0, 0, 1, 0);

	//zid

	for(i=0;i<prom;i++)
	{	
	glPushMatrix();
	 glTranslatef(0,0,-1);	
  	 glRotatef(90,0,1,0);
         glTranslatef(0,0,-pomeraj/8);
	 glTranslatef(-1*i,0,0);
         glRotatef(ugao,0,1,0);
	  zid1=zid(5,5,5,5);
	glPopMatrix();
         
	glPushMatrix();
	 glTranslatef(0,0,-1);	
  	 glRotatef(90,0,1,0);
         glTranslatef(0,0,-pomeraj/8);
	 glTranslatef(-1.5*i,0,0);
         glRotatef(ugao1,0,1,0);
	  zid1=zid(5,5,5,5);
	glPopMatrix();

	glPushMatrix();
	 glTranslatef(0,0,-1);	
  	 glRotatef(90,0,1,0);
         glTranslatef(-pomeraj/8,0,0);
	 glTranslatef(-2*i,0,0);
         glRotatef(ugao2,0,1,0);
	  zid1=zid(5,5,5,5);
	glPopMatrix();

	}

 	
	nebo_i_zemlja(14);
	

        //vetrenjaca
        glPushMatrix();
         glTranslatef(0.5,-1.99,-3);
         glRotatef(-120,0,1,0); 
         vetrenjaca();
        glPopMatrix();

	vetar();

        //torus i kocka koji proizvode energiju
 	glTranslatef(0,-0.48,0);
	 motor();
 	glTranslatef(0,0.48,0);

	//sat
	// minuti i sati
	
	glPushMatrix();	
 	glRotatef(90,0,1,0);
        glTranslatef(-4.5,1.8,0);
	glBegin(GL_LINES);
	    for(i=0; i<60; i++)
	    {
   	      if(i%5){ // normal minute
        	if(i%5 == 1)
                 glColor3f(1.0f, 0.0f, 1.0f);
	         set_material(2);
        	  newLine(minStart, minEnd, i*angle1min);
      		}else{
                 set_material(0);
        	  glColor3f(1.0f, 0.0f, 0.0f);  
     		  newLine(stepStart, stepEnd, i*angle1min);
      		}
    	    }
	glEnd();
	glPopMatrix();

 
	//kazaljke

        //velika i mala kazaljka
        glPushMatrix();
	  glRotatef(-90,0,1,0);
          glTranslatef(4.5,1.8,0);
	  glLineWidth(3.0f);	
	  set_material(0);
  	  glBegin(GL_LINES);
    	    newLine(0.0f, 0.5f, angleHour+M_PI/2);
    	    newLine(0.0f, 0.8f, angleMin+M_PI/2);
	  glEnd();
	glPopMatrix();


	//kazaljka koja pokazuje sekunde
        glPushMatrix();
	  glLineWidth(2.0f);
	  glRotatef(-90,0,1,0);
          glTranslatef(4.5,1.8,0);
	  set_material(0);
	  glBegin(GL_LINES);
	    newLine(0.0f, 0.8f, angleSec+M_PI/2);
	  glEnd();
        glPopMatrix();	
  
	//zuti zubac
	glColor3f(0,0.5,0);
	glPushMatrix();
	  set_material(4);
	  glRotatef(90,0,1,0);
          glTranslatef(-4.6,1.8,-0.2); 
	  glRotatef(pomeraj7, 0,0,-1);	
  
	  crtajZupcanik(0.5,0.3,0.5,20);
	glPopMatrix();


	//zeleni zubac        
	 glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  	 glColor3f(1,1,0);
	 glPushMatrix();
	   set_material(1);  	
 	   glRotatef(90,0,1,0);	
 	   glTranslatef(-4.2,0.50, -0.2);		
	   glRotatef(pomeraj1, 0,0,1);

	   crtajZupcanik(0.28,0.2,0.3,10);
 	  glPopMatrix();


	//najmanji zubac
 	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	 glPushMatrix();
	   set_material(0);  	
 	   glRotatef(90,0,1,0);	
 	   glTranslatef(-3.55,-0.16, -0.2);		
	   glRotatef(-pomeraj1, 0,0,1);

	   crtajZupcanik(0.2,0.2,0.3,5);
 	  glPopMatrix();
  
	//dovodnici energije
	dovodnici();  

	//opruga koja dobija energiju od kocke i prenosi je na zubac sata
	pokretac_sata();
		
	
	glutSwapBuffers();
}


static void init_lights()
{
    /* Pozicija svetla (u pitanju je direkcionalno svetlo). */
    GLfloat light_position[] = { 0, 0, 1, 1 };

    /* Ambijentalna boja svetla. */
    GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1 };

    /* Difuzna boja svetla. */
    GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1 };

    /* Spekularna boja svetla. */
    GLfloat light_specular[] = { 0.9, 0.9, 0.9, 1 };

    /* Ukljucuje se osvjetljenje i podesavaju parametri svetla. */
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}

static void set_material(int id)
{
    /* Koeficijenti ambijentalne refleksije materijala. */
    GLfloat ambient_coeffs[] = { 0.3, 0.3, 0.3, 1 };

    /* Koeficijenti difuzne refleksije materijala. */
    GLfloat diffuse_coeffs[] = { 0.4, 0.4, 0.4, 1 };

    /* Koeficijenti spekularne refleksije materijala. */
    GLfloat specular_coeffs[] = { 1, 1, 1, 1 };

    GLfloat shininess = 30;
 

    switch (id) {
        case 0:
            // Narandzasta 
	      diffuse_coeffs[0] = 1.0;
	      diffuse_coeffs[1] = 0.7;
	      ambient_coeffs[0] = 1.0;
            break;
        case 1:
            // Zelena
            diffuse_coeffs[1] = 1.0;
            ambient_coeffs[1] = 1.0;
            specular_coeffs[1] = 1.0;
            break;
        case 2:
            // Plava
            diffuse_coeffs[2] = 1.0;
 	    ambient_coeffs[2] = 1.0;
            specular_coeffs[2] = 1.0;
            break;
	case 3:
            // Braon - siva
 	    ambient_coeffs[0] = 0.8;
 	    ambient_coeffs[1] = 0.5;
 	    ambient_coeffs[2] = 0.2;
            
            break;
	case 4:
            // Zuta
 	    ambient_coeffs[0] = 1;
 	    ambient_coeffs[1] = 1;
            diffuse_coeffs[0] = 1;
            diffuse_coeffs[1] = 1;
            specular_coeffs[0] = 1;
            specular_coeffs[1] = 1;
            break;
	case 5:
            // Siva
 	    ambient_coeffs[0] = 0.6;
 	    ambient_coeffs[1] = 0.6;
 	    ambient_coeffs[2] = 0.6;
            break;
  	case 6:
            // Bordo
            diffuse_coeffs[0] = 1.0;
            diffuse_coeffs[3] = 0.7;
            break;            	   
    }

    /* Podesavaju se parametri materijala. */
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}


