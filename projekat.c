#include<GL/glut.h>
#include<math.h>
#include<stdlib.h>

//vreme proteklo od pocetka simulacije
static float hours;

//fleg za stajanje tajmera
static int timer_active;

/*deklaracija funkcije*/
static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);
static void initialize(void);
static void on_reshape(int width, int height);
static void on_timer(int value);

static int w_width, h_height;

int main(int argc, char **argv)
{
	/*inicijalizuje glut*/
	glutInit(&argc,argv);

	/*rgb - rezim rgb boja,
	double- iscrtava na jednom prozoru, pa tu sliku prebacuje na ekran-drugi prozor*/
	glutInitDisplayMode(GLUT_RGB /*| GLUT_DEPTH */| GLUT_DOUBLE);
 
	/*kreiranje prozora*/
	glutInitWindowSize(1450,650);
	glutInitWindowPosition(100,100);
	glutCreateWindow(argv[0]);

	/*poziv funkcije za prikaz prozora*/
	glutDisplayFunc(on_display);	
	glutKeyboardFunc(on_keyboard);
        glutReshapeFunc(on_reshape);
	
	hours=0;
	timer_active=0;

	initialize();

	glutMainLoop();
	return 0;
}

static void initialize(void)
{
	/*boja pozadine*/
	glClearColor(0.75,0,0.80,0);	

	glEnable(GL_DEPTH_TEST);
	glLineWidth(2);
	/*bafer dubine*/


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
	gluPerspective(60, width/(float)height, 1, 5);
	/*1-bliza ravan - near, 5 dalja ravan -far*/

}

static void on_timer(int value)
{
    // Proverava se da li callback dolazi od odgovarajuceg tajmera. 
    if (value != 0)
        return;

    // Azurira se vreme simulacije. 
    hours += 6;

    // Forsira se ponovno iscrtavanje prozora. 
    glutPostRedisplay();

    // Po potrebi se ponovo postavlja tajmer. 
    if (timer_active)
        glutTimerFunc(50, on_timer, 0);
}

static void on_display(void)
{

	float rotacija_kocke, rotacija_svere;
	/*brise se prethodni sadrzaj prozora,ali nije zamenjen*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	/*definise pogled, kamera je u 0,0 koordinatama-donji-levi ugao,w i h sirina i visina prozora*/

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	/*transformacija pogleda*/
	/*1,2,3 - polozaj posmatraca,  0,0,0 - tacka u koju gledamo, 0,1,0 - vektor normale na posmatraca, rotacija oko y*/
	gluLookAt(1,2,3,0,0,0,0,1,0);

	/*da se odnosi samo na objekat izmedju push i pop*/
	rotacija_kocke = 360 * hours / (15 * 24);
	glPushMatrix();
   	  glRotatef(rotacija_kocke,0,1,0);		
	  glColor3f(0,0,1);
	  glutWireCube(1);		
	glPopMatrix();

	rotacija_svere = 360 * hours / (10*24);
	//glTranslatef(2,0,0);

	glPushMatrix();
	  glRotatef(rotacija_svere, 1,0,0);
	  glColor3f(0,1,0);
	  glutWireSphere(1.5,5,5);
	glPopMatrix();

	glutSwapBuffers();

}

static void on_keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{

   	  case 27:
		exit(0);
		break;

	  case 'g':
   	  case 'G':
        	//Pokrece se simulacija. 
           if (!timer_active) 
	    {
            	glutTimerFunc(50, on_timer, 0);
            	timer_active = 1;
            }
           break;

    	case 's':
    	case 'S':
    	    // Zaustavlja se simulacija. 
    	    timer_active = 0;
         break;
        }
}




