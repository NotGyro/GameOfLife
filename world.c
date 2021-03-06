#include "world.h"
#include "null.h"

void SetCellUnwrapped(World *const w, int cellX, int cellY, bool setTo)
{
	bool* buf;
	if(w->writeBuf == RED)
	{
		buf = w->redBuffer; 
	}
	else if(w->writeBuf == BLUE)
	{ 
 		buf = w->blueBuffer;
	}
	if(((cellX >= 0) && (cellX < w->width))
			&& ((cellY >= 0) && (cellY < w->height)))
	{
		//Packed array element retrieval.
		buf[(cellY*w->width)+cellX] = setTo;
	}

}
int WrapX(World *const w, int cellX)
{
	/*Coordinate shift for world wrapping functionality.*/
	if(cellX < 0)
	{
		//Go over left border
		cellX = w->width + cellX;
	}
	else if(cellX >= w->width)
	{
		//Go over right border
		cellX = cellX - w->width;
	}
	return cellX;
}

int WrapY(World *const w, int cellY)
{
	if(cellY < 0)
	{
		//Go over top border.
		cellY = w->height + cellY;
	}
	else if(cellY >= w->height)
	{
		//Go over right border
		cellY = cellY - w->height;
	}
	return cellY;
};
void SetCell(World *const w, int cellX, int cellY, bool setTo)
{
	SetCellUnwrapped(w, WrapX(w, cellX), WrapY(w, cellY), setTo);
}
void UpdateCell(World *const w, unsigned int cellX, unsigned int cellY)
{
	
	unsigned char count = 0;
	//Set up the bounds for our search, ensuring that they are
	//within the array.
	int fromX = cellX - 1;
	//Commented-out functionality is pre world-wrapping
	//if(fromX < 0) {
	//	fromX = 0;
	//}
	int toX = cellX + 1;
	//if(toX >= w->width) {
	//	toX = w->width-1;
	//}
	int fromY = cellY - 1;
	//if(fromY < 0) {
	//	fromY = 0;
	//}
	int toY = cellY + 1;
	//if(toY >= w->height) {
	//	toY = w->height-1;
	//}
	bool ourCell = false; //TODO: Should it default false?
	//Search for adjacent tiles.
	for(int itY = fromY; itY <= toY; ++itY)
	{
		for(int itX = fromX; itX <= toX; ++itX)
		{
		//Is this just our cell?
			if((itX == cellX) && (itY == cellY))
			{
				ourCell = GetCell(w, itX, itY);	
			}
			else if(GetCell(w, itX, itY) == true)
			{
				++count;
			}
		}
	}
	if((count < 2) && (ourCell == true))
	{
		//Death by underpopulation
		SetCell(w, cellX, cellY, false);
	}
	else if((count > 3) && (ourCell == true))
	{
		//Death by overcrowding
		SetCell(w, cellX, cellY, false);
	}
	//Two neighbors on a live cell does nothing.
	//Three on an empty cell creates a live cell (reproduction):
	else if((count == 3) && (ourCell == false))
	{
		SetCell(w, cellX, cellY, true);
	}
};

void FlipBuffers(World *const w)
{
	if(w->writeBuf == RED)
	{
		w->writeBuf = BLUE;
		memcpy(w->blueBuffer, w->redBuffer,
			(w->width * w->height) * sizeof(bool));
	}	
	if(w->writeBuf == BLUE)
	{
		w->writeBuf = RED;
		memcpy(w->redBuffer, w->blueBuffer,
			(w->width * w->height) * sizeof(bool));
	}	
}

void Update(World *const w)
{
	for(int y = 0; y < w->height; ++y)
	{
		for(int x = 0; x < w->width; ++x)
		{
			UpdateCell(w, x, y);
		}
	}
	FlipBuffers(w);	
}

/* 
 * First int pair is new size, second is
 * offset by which we blit the old map
 * onto the new one.
 */
void ResizeWorld(World *const w, unsigned int width, unsigned int height,
			int offX, int offY)
{
	bool* newBlue = calloc( width * height, sizeof(bool) );
	bool* newRed = calloc( width * height, sizeof(bool) );
	if((newBlue != 0) && (newRed != 0))
	{	
	//Iterate over every element in the old world
	for(int x = 0; x < w->width; ++x)
	{
		for(int y = 0; y < w->height; ++y)
		{
			int insertX = x + offX;
			int insertY = y + offY;
			//Copy over the old world to the new
			if(((insertX < width) && (insertX >= 0))
			&& ((insertY < height) && (insertY >= 0 )))
			{
				//printf("X: %d \n", insertX);
				//printf("Y: %d \n", insertY);
				newBlue[(insertY*width)+insertX] = 
					w->blueBuffer[(y * (w->width))+x];
				
				newRed[(insertY*width)+insertX] = 
					w->redBuffer[(y * (w->width))+x];
			}
		}
	}
	//Set up the world's properties, free old buffers.

	free(w->redBuffer);
	free(w->blueBuffer);
	w->redBuffer = newRed;
	w->blueBuffer = newBlue;

	w->width = width;
	w->height = height;
	++(w->resizeRevision);
	}
}
void FlipCell(World *const w, int cellX, int cellY)
{
	SetCell(w, cellX, cellY, !(GetCell(w, cellX, cellY)));
}

void InitializeWorld(World *const w, unsigned int width, unsigned int height)
{
	w->writeBuf = RED;
	w->redBuffer = calloc(width*height, sizeof(bool));
	w->blueBuffer = calloc(width*height, sizeof(bool));
	w->width = width;
	w->height = height;
	
	w->resizeRevision = 0;
}

//Basically a convenience function over InitializeWorld.
World* MakeWorld(unsigned int width, unsigned int height)
{
	World* w = malloc(sizeof(World));
	InitializeWorld(w, width, height);
	return w;
}

//Buffer, width, height, positionx, positiony
bool GetCellFromBuffer(bool* buf, unsigned int width, unsigned int height,
			int posX, int posY)
{
	if(((posX >= 0) && (posX < width))
			&& ((posY >= 0) && (posY < height)))
	{
		//Packed array element retrieval.
		return buf[(posY*width)+posX];
	}
	return -1;
};
/* For rendering and other such output functions.
 * Wraps/hides buffering and such.
 * Gets from the non-writing buffer.
 * */
bool GetCellUnwrapped(World *const w, int x, int y)
{
	//Bounds checking
	if((x < 0) || (x >= w->width))
	{
		return -1;
	}
	if((y < 0) || (y >= w->height))
	{
		return -1;
	}
	if(w->writeBuf == RED)
	{
		return GetCellFromBuffer(w->blueBuffer, 
				w->width, w->height, x, y);
	}
	else if(w->writeBuf == BLUE)
	{ 
		return GetCellFromBuffer(w->redBuffer, 
				w->width, w->height, x, y);
	}
	else
	{
		return -1;
	}
};

bool GetCell(World *const w, int x, int y)
{
	return GetCellUnwrapped(w, WrapX(w, x), WrapY(w, y));
}

void DestroyWorld(World *const w)
{
	free(w->redBuffer);
	free(w->blueBuffer);
}
