#include<GL/glut.h>

/*deklaracija funkcije*/
static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);
static void initialize(void);
static void on_reshape(int width, int height);

static int w_width, h_height;

int main(int argc, char **argv)
{
	/*inicijalizuje glut*/
	glutInit(&argc,argv);

	/*rgb - rezim rgb boja,
	double- iscrtava na jednom prozoru, pa tu sliku prebacuje na ekran-drugi prozor*/
	glutInitDisplayMode(GLUT_RGB | /*GLUTH_DEPTH*/ GLUT_DOUBLE);
 
	/*kreiranje prozora*/
	glutInitWindowSize(800,600);
	glutInitWindowPosition(100,100);
	glutCreateWindow(argv[0]);

	/*poziv funkcije za prikaz prozora*/
	glutDisplayFunc(on_display);	
	glutKeyboardFunc(on_keyboard);
        glutReshapeFunc(on_reshape);
	
	initialize();


	glutMainLoop();
	return 0;
}

static void initialize(void)
{
	/*boja pozadine*/
	glClearColor(0.75,0,0.80,0);	

	glLineWidth(2);
	/*bafer dubine*/
	glEnable(GL_DEPTH_TEST);

}

static void on_reshape(int width,int height)
{
	w_width=width;
	h_height=height;
}

static void on_display(void)
{
	/*brise se prethodni sadrzaj prozora,ali nije zamenjen*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	/*definise pogled, kamera je u 0,0 koordinatama-donji-levi ugao,w i h sirina i visina prozora*/
	glViewport(0,0,w_width,h_height);

	/*podesavanje projekcije*/
	glMatrixMode(GL_PROJECTION);

	/*menja tekucu matricu jedinicnom*/
	glLoadIdentity();
	
	/*postavalja parametre perspektive*/
	/*60-ugao vidnog polja, u pravcu y ose,  razlomak- vidno polje po x osi i kolicnik je sirine i visine,*/
	gluPerspective(60, w_width/(float)h_height, 1, 5);/*1-bliza ravan - near, 5 dalja ravan -far*/


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	/*transformacija pogleda*/
	/*1,2,3 - polozaj posmatraca,  0,0,0 - tacka u koju gledamo, 0,1,0 - vektor normale na posmatraca, rotacija oko y*/
	gluLookAt(1,2,3,0,0,0,0,1,0);

	/*da se odnosi samo na objekat izmedju push i pop*/
	glPushMatrix();
	  glColor3f(0,0,1);
	  glutWireCube(1);		
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
	}

}



