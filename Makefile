projekat: projekat.c image.o zid.o
	gcc projekat.c -o 1 image.o zid.o -lGL -lGLU -lglut -lm -Wall

clean:
	rm 1
	rm *.o	
	rm *.~
