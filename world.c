#include "world.h"

void _UpdateCell(World *const w, unsigned int cellX, unsigned int cellY)
{
}
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
}

//Basically a convenience function over InitializeWorld.
void MakeWorld(unsigned int width, unsigned int height)
{

}


