#include "world.h"
#include "null.h"

void _UpdateCell(World *const w, unsigned int cellX, unsigned int cellY)
{
};
void Update(World *const w)
{
}

/* 
 * First int pair is new size, second is
 * offset by which we blit the old map
 * onto the new one.
 */
void ResizeWorld(World *const w, unsigned int width, unsigned int height,
	       	int offX, int offY)
{

}

void FlipCell(World *const w, int cellX, int cellY)
{
}

void InitializeWorld(World *const w, unsigned int width, unsigned int height)
{
	w->redBuffer = malloc(sizeof(bool)*(width*height));
	w->blueBuffer = calloc(sizeof(bool)*(width*height));
}

//Basically a convenience function over InitializeWorld.
World* MakeWorld(unsigned int width, unsigned int height)
{
	World* w = malloc(sizeof(World));
	InitializeWorld(w, width, height);
	return w;
}

//Buffer, width, height, positionx, positiony
bool GetCellFromBuffer(bool buf*, unsigned int width, unsigned int height,
			int posX, int posY)
{
	if(((posX > 0) && (posX < width))
			&& ((posY > 0) && (posY < height)))
	{
		//Packed array element retrieval.
		return buf[(posY*height)+posX];
	}
	return -1;
};
/* For rendering and other such output functions.
 * Wraps/hides buffering and such. */
bool GetCell(World *const w, int x, int y)
{
	if(w->writeBuf == RED)
	{
		return GetCellFromBuffer(w->redBuffer, 
				w->width, w->height, x, y);
	}
	else if(w->writeBuf == BLUE)
	{ 
		return GetCellFromBuffer(w->blueBuffer, 
				w->width, w->height, x, y);
	}
	else
	{
		return -1;
	}
}
