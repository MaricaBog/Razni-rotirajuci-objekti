#include<GL/glut.h>

/*deklaracija funkcije*/
static void on_display(void);


//proba git-a
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





