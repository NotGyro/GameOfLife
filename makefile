EXTRA_CFLAGS := -std=c99

C_FLAGS = -std=c99 -w -g 
C_FLAGS_LINK = -lSDL2 -lm

all: game 

game: main.o world.o render.o
		gcc main.o world.o render.o $(C_FLAGS_LINK) -o lifegame
		chmod +x lifegame
	
main.o: main.c
		gcc $(C_FLAGS) -c main.c 

world.o: world.c
		gcc $(C_FLAGS) -c world.c

render.o: render.c
		gcc $(C_FLAGS) -c render.c

clean:
		rm -rf *o lifegame
		
