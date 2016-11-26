projekat: projekat.c
	gcc projekat.c -lglut -lGL -lGLU -o projekat
	./projekat

.PHONY: all projekat clean

clean:	
	rm -rf *.c projekat
all:	projekat
