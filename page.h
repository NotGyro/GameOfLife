#ifndef PAGE_H
#define PAGE_H

#include <stdbool.h>

#define PAGE_SIZE 256

typedef enum {RIGHT, ABOVE, LEFT, BELOW} PageDirection;

typedef unsigned short cellcoord;

//Prototype the struct so we can get on with this self-referential type business

typedef struct Page Page;

struct Page
{
	int x, y;
	bool cells[PAGE_SIZE * PAGE_SIZE];
	// These are pointers to the adjacent pages.
	Page* right;
	Page* above;
	Page* left;
	Page* below;
};

typedef struct PageListEntry PageListEntry;

//For a linked list of pages.
struct PageListEntry
{
	Page* ours;
	PageListEntry* next;
};

//The head of our global page list.
extern PageListEntry headPage;

/*The very first optimization to write will be a hash table of 
 * pages to make GetPage() more efficient. */
//Refers to our global page index, however it may be implemented.
Page* GetPage(int, int);

void InitializePage(Page*, int, int);

bool GetCell(Page*, cellcoord, cellcoord);
//Used for the behavior of the player clicking on the cell.
void FlipCell(Page*, cellcoord, cellcoord);
//Does the actual Conway's Game of Life behavior
void UpdateCell(Page*, cellcoord, cellcoord);

#endif
