EXTRA_CFLAGS := -std=c99
all: game 

game: main.o world.o
		gcc main.o world.o -o lifegame

main.o: main.c
		gcc -std=c99 -c main.c

world.o: world.c
		gcc -std=c99 -c world.c

clean:
		rm -rf *o lifegame
		
