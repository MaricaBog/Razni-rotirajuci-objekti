#include<GL/glut.h>

/*deklaracija funkcije*/
static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);


//proba git-a
int main(int argc, char **argv)
{
	/*inicijalizuje glut*/
	glutInit(&argc,argv);

	/*rgb - rezim rgb boja,
	double- iscrtava na jednom prozoru, pa tu sliku prebacuje na ekran-drugi prozor*/
	glutInitDisplayMode(GLUT_RGB | /*GLUTH_DEPTH*/ GLUT_DOUBLE);
 
	/*kreiranje prozora*/
	glutInitWindowSize(1450,650);
	glutInitWindowPosition(100,100);
	glutCreateWindow(argv[0]);

	/*poziv funkcije za prikaz prozora*/
	glutDisplayFunc(on_display);	
	glutKeyboardFunc(on_keyboard);
	
	glClearColor(0.75,0,0.80,0);	
	/*bafer dubine*/
	glEnable(GL_DEPTH_TEST);


	glutMainLoop();
	return 0;
}

static void on_display(void)
{
	/*brise se prethodni sadrzaj prozora,ali nije zamenjen*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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




