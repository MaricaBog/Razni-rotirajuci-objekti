projekat: projekat.c image.o 
	gcc projekat.c -o 1 image.o -lGL -lGLU -lglut -lm -Wall 

clean:
	rm 1
	rm *.o	
	rm *.~
