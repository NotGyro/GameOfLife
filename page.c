#include "page.h"
#include "null.h"

PageListEntry headPage = { 0, 0 };

//Retrieves a page from the global index of them.
Page* GetPage(int x, int y)
{
	PageListEntry* current = &headPage;
	while(current->next != NULL)
	{
		if((current->ours->x == x) 
			&& (current->ours->y == y))
		{
			return current->ours;
		}
		current = current->next;	
	}
	return NULL;
}

void InitializePage(Page* p, int x, int y)
{
	//TODO
}

bool GetCell(Page* p, cellcoord x, cellcoord y)
{
	//TODO
	return false;
}
//Used for the behavior of the player clicking on the cell.
void FlipCell(Page* p, cellcoord x, cellcoord y)
{
	//TODO
}
//Does the actual Conway's Game of Life behavior
void UpdateCell(Page* p, cellcoord x, cellcoord y)
{
	//TODO
}
