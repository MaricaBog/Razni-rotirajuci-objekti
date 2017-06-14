#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/timeb.h>
#include "image.h"

#define EPSILON 0.1
#define PI 3.1415926

static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width,int height);
void on_display(void);
void on_timer(int value);
static void init_lights();
static void set_material(int id);

static void vetrenjaca();
static void elisa();
static void motor();
static void drzac();

/* Uglovi u sfernom koordinatnom sistemu koji
   odredjuju polozaj vidne tacke. */

static float phi, theta, delta_phi, delta_theta;
float pomeraj=0.0f, pomeraj1=0.0f,pomeraj3=0.0f, pomeraj4 = 0.0f;
int frameNumber = 0;


int main(int argc, char* argv[]){

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1450,650);
    glutCreateWindow(argv[0]);

    glutDisplayFunc(on_display);
    glutReshapeFunc(on_reshape);
    glutKeyboardFunc(on_keyboard);
    glutTimerFunc(33, on_timer, 1);


    phi = theta = 0;
    delta_phi = delta_theta = PI / 90;

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);

    glutMainLoop();

  return 0;
}

void on_timer(int value){

	pomeraj+=0.3;
	pomeraj1+=0.6;
	pomeraj3+=0.3;
	frameNumber++;

  pomeraj4+=0.6;
  if(pomeraj4>2)
   pomeraj4--;
  else if(pomeraj4<2)
   pomeraj4++;



  if(pomeraj3>=20)
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
	       

	case 'h':
	 case 'H':
        // Invertuje se fleg koji odredjuje pozicioniranje svetla. 
        glutPostRedisplay();
        break;

    case 'p':
     case 'P':
        // Dekrementira se ugao phi i ponovo iscrtava scena. 
        phi -= delta_phi;
        if (phi > 2*PI) {
            phi -= 2 * PI;
        } else if (phi < 0) {
            phi += 2 * PI;
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
        if (theta < -(PI / 2 - PI / 180)) {
            theta = -(PI / 2 - PI / 180);
        }
        glutPostRedisplay();
        break;

    case 'T':
        
         // Inkrementira se ugao theta i ponovo iscrtava scena. Ovaj
         // ugao se odrzava u intervalu [-89,89] stepeni.
         
        theta += delta_theta;
        if (theta > PI / 2 - PI / 180) {
            theta = PI / 2 - PI / 180;
        }
        glutPostRedisplay();
        break;

	//reset zupcanika i elise
    case 'R':
    case 'r':

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
	/*0-bliza ravan - near, 20 dalja ravan -far*/
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

        glPushMatrix();
	glColor3f(0.8f, 0.8f, 0);  
        glTranslatef(-0.2,2.25,-0.03);  

        glutSolidCube(0.53);
        glPopMatrix();

	 glTranslatef(-0.2,0,0);
 
	glColor3f(0.8f, 0.8f, 0.9f);
	
	//pod
	glBegin(GL_QUADS);
    		glVertex3f(-0.5*x, 0, -0.3);//0.5
   		glNormal3f(0,-1,0);
     
    		glVertex3f(-0.5*x, 0, 0.3);
    		glNormal3f(0,-1,0);

    		glVertex3f(0.5*x, 0, 0.3);
    		glNormal3f(0,-1,0);

    		glVertex3f(0.6*x, 0, -0.3);
    		glNormal3f(0,-1,0);
  	glEnd();


	//krov
	glBegin(GL_QUADS);
		glVertex3f(-0.3*x, visina_zubca, -0.3); //ok
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
    		glVertex3f(-0.5*x, 0, 0.3);//1
   		glNormal3f(0,0,1);

		glVertex3f(0.5*x, 0, 0.3);//1
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

		glVertex3f(0.6*x, -0.1, -0.3);
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

    	      glVertex3f(0.6*x, -0.1, -0.3);
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
	  glRotated(pomeraj * (180.0/140.0), 0, 0, 1);
        
	/*****set_material(3);******/
 	  glColor3f(0.64f, 0.16f, 0.16f);
	    for (i =1; i < 9; i++) {
	        glRotatef(45*i,0,0,1);
		glScalef(0.035,0.035,0.035);
		elisa();
		  glRotatef(-45*i,0,0,1);
		glScalef(1/0.035,1/0.035,1/0.035);
	    }

	  glRotated(-pomeraj * (180.0/140.0), 0, 0, 1);
  	  glTranslatef(-0.05, -2.2, 0.32);
          glRotatef(-5,0,1,0);

    glTranslatef(0.2,0,0);

}


static void elisa()
{
	glColor3f(1,0.2,0.3);
	int br_zubaca=4;
	float rZ=10.0,visina_zubca=30.0;
	float x= (2*rZ*PI)/(1.6*br_zubaca);
	float greska=rZ-sqrt(rZ*rZ-(0.3*x)*(0.3*x));
	visina_zubca+=greska;
	glRotated(frameNumber * (180.0/140.0), 0, 0, 1);

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

		glRotated(-frameNumber * (180.0/140.0), 0, 0, 1);

}


static void motor()
{
//drzac 1
	glPushMatrix();
	//set_material(4);
          glTranslatef(-0.8,0.5,-0.7); 
          glRotatef(70,0,1,0);
	  drzac();
	glPopMatrix();

//drzac 2
	glPushMatrix();
	//set_material(4);
          glTranslatef(-1.33,0.5,0.7); 
          glRotatef(70,0,1,0);
	  drzac();
	glPopMatrix();

//kocka
	glPushMatrix();
	//set_material(4);
	  glColor3f(0,0.2,0.7);
	  glTranslatef(-1,-0.5,0);
          glRotatef(-20,0,1,0);
	  glutSolidCube(2);

	glPopMatrix();

 //ploce();

// rotirajuci torus koji proizvodi energiju za sat
          glPushMatrix();

 	   glRotatef(-120,0,1,0);
	   glTranslatef(0.53,1.6,0.9);
	   glColor3f(0.2,0.2,0.2);

  	   for(int i=0;i<10;i++)
	   {
	    glRotated(frameNumber * (180.0/(130.0-i)), 1, 0, 0);
            glutSolidTorus(0.01,0.66,15,20);
	    glRotated(-frameNumber * (180.0/(130.0-i)), 1, 0, 0);
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
    		glVertex3f(1*x, 0, 0.5);
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
       		
		glVertex3f(1*x, 0, 0.5); //y=-1
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
		
    		glVertex3f(1*x, visina_zubca-1, -0.5);//bez -1
    		glNormal3f(0,0,-1);
		
   		glVertex3f(-1*x, visina_zubca-1, -0.5);//bez -1
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

void on_display(void){


        // init_lights();
	GLfloat light_position[] = { 0, 0, -1, 1 };
 	//GLfloat light_position[] = { 1, 15, 5, 0 };

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);


 /* Postavljaju se vidni parametri. */
    	gluLookAt(5 * cos(theta) * cos(phi),
              5 * cos(theta) * sin(phi),
              5 * sin(theta),
              0, 0, 0, 0, 1, 0);
	
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	
        //vetrenjaca
        glPushMatrix();
         glTranslatef(0.7,-2,-3);
         glRotatef(-120,0,1,0); 
         vetrenjaca();
        glPopMatrix();


	motor();

  
	glutSwapBuffers();
}



static void init_lights()
{
    /* Pozicija svetla (u pitanju je direkcionalno svetlo). */
    GLfloat light_position[] = { 1, 15, 5, 0 };

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
            /* Difuzna komponenta se postavlja na crvenu */
	      diffuse_coeffs[0] = 1.0;
	      diffuse_coeffs[1] = 0.7;
	      ambient_coeffs[0] = 1.0;
            break;
        case 1:
            /* Difuzna komponenta se postavlja na zelenu */
            diffuse_coeffs[1] = 1.0;
            ambient_coeffs[1] = 1.0;
            specular_coeffs[1] = 1.0;
            break;
        case 2:
            /* Difuzna komponenta se postavlja na plavu */

            diffuse_coeffs[2] = 1.0;
 	    ambient_coeffs[2] = 1.0;
            specular_coeffs[2] = 1.0;
            break;
	case 3:
            /* Difuzna komponenta se postavlja na braon */
 	    ambient_coeffs[0] = 0.8;
 	    ambient_coeffs[1] = 0.5;
 	    ambient_coeffs[2] = 0.2;
            
            break;
	case 4:
            /* Difuzna komponenta se postavlja na zuta */
 	    ambient_coeffs[0] = 1;
 	    ambient_coeffs[1] = 1;
            diffuse_coeffs[0] = 1;
            diffuse_coeffs[1] = 1;
            specular_coeffs[0] = 1;
            specular_coeffs[1] = 1;
            break;
	case 5:
            /* Difuzna komponenta se postavlja na siva */
 	    ambient_coeffs[0] = 0.6;
 	    ambient_coeffs[1] = 0.6;
 	    ambient_coeffs[2] = 0.6;
            break;
	case 6:
            /* Difuzna komponenta se postavlja na narandzasta */
 	    ambient_coeffs[0] = 1;
 	    ambient_coeffs[1] = 0.6;
	    diffuse_coeffs[0] = 1;
            diffuse_coeffs[1] = 0.6;
	    specular_coeffs[0] = 1;
            specular_coeffs[1] = 0.6;
            break; 	   



    }

    /* Podesavaju se parametri materijala. */
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}


