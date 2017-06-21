#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/timeb.h>
#include "image.h"


#define PI 3.1415926

/* Imena fajlova sa teksturama. */
#define FILENAME0 "teksture/sky.bmp"
#define FILENAME1 "teksture/2.bmp"
#define FILENAME2 "teksture/zid.bmp"


/* Identifikatori tekstura. */
static GLuint names[3];


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
