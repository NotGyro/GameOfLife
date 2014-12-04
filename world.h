#ifndef WORLD_H
#define WORLD_H

#include <stdbool.h>

typedef struct World World;

//A datatype to tell us things like "which buffer are
//we currently writing to?" 
typedef enum {
	RED,
	BLUE
} bufferid;

struct World
{
	bool redBuffer*;
	bool blueBuffer*;

	int height;
	int width;

	/* This implementation of Conway's Game of Life
	 * works by reading one buffer (the previous /
	 * current one) in order to produce the next
	 * generation (the results of this buffer) in
	 * the other one, and then swapping the two.
	 *
	 * So, let's say you've got buffer Red and buffer
	 * Blue. Red is read-only and has nothing in it,
	 * as it was initialized that way. The player
	 * clicks somewhere, flipping some cells buffer
	 * blue, and then the game updates a tick.
	 * This tick update switches the roles of the
	 * two buffers, such that blue buffer is the one
	 * from which we read, and the results of that
	 * are written into the red buffer.*/

	//TODO: More concise explanation. :P
	bufferid writeBuf;

	bool redBuffer*;
	bool blueBuffer*;
};

void Update(World*);

/* 
 * First int pair is new size, second is
 * offset by which we blit the old map
 * onto the new one.
 */
void Resize(World*, unsigned int, unsigned int, int, int);

void FlipCell(World*, int, int);

#endif
