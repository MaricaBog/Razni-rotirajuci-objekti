#include<GL/glut.h>

/*deklaracija funkcije*/
static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);
<<<<<<< HEAD
static void initialize(void);
static void on_reshape(int width, int height);

static int w_width, h_height;

=======


//proba git-a
>>>>>>> bd7b497d58433ac1b72152b3a2bbf20122a89faf
int main(int argc, char **argv)
{
	/*inicijalizuje glut*/
	glutInit(&argc,argv);

	/*rgb - rezim rgb boja,
	double- iscrtava na jednom prozoru, pa tu sliku prebacuje na ekran-drugi prozor*/
	glutInitDisplayMode(GLUT_RGB | /*GLUTH_DEPTH*/ GLUT_DOUBLE);
 
	/*kreiranje prozora*/
<<<<<<< HEAD
	glutInitWindowSize(800,600);
=======
	glutInitWindowSize(1450,650);
>>>>>>> bd7b497d58433ac1b72152b3a2bbf20122a89faf
	glutInitWindowPosition(100,100);
	glutCreateWindow(argv[0]);

	/*poziv funkcije za prikaz prozora*/
	glutDisplayFunc(on_display);	
	glutKeyboardFunc(on_keyboard);
<<<<<<< HEAD
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
=======
	
	glClearColor(0.75,0,0.80,0);	
	/*bafer dubine*/
	glEnable(GL_DEPTH_TEST);


	glutMainLoop();
	return 0;
>>>>>>> bd7b497d58433ac1b72152b3a2bbf20122a89faf
}

static void on_display(void)
{
	/*brise se prethodni sadrzaj prozora,ali nije zamenjen*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
<<<<<<< HEAD
	
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
=======
	glutSwapBuffers();       
>>>>>>> bd7b497d58433ac1b72152b3a2bbf20122a89faf
}

static void on_keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
<<<<<<< HEAD
   	  case 27:
		exit(0);
		break;
	}

=======
	case 27:
		exit(0); 	
		break;
	}
>>>>>>> bd7b497d58433ac1b72152b3a2bbf20122a89faf
}



<<<<<<< HEAD
=======

>>>>>>> bd7b497d58433ac1b72152b3a2bbf20122a89faf
