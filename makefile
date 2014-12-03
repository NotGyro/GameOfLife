all: game 

game: main.o page.o
		gcc main.o page.o -o lifegame

main.o: main.c
		gcc -c main.c

page.o: page.c
		gcc -c page.c

clean:
		rm -rf *o lifegame
		
