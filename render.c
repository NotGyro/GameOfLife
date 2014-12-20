#include "render.h"
#include <stdio.h>

#define MAX_ZOOM_IN 6
#define MAX_ZOOM_OUT 10

void InitGameRenderer(GameRenderer* g, World* world, unsigned char cellSize, const char* wtitle, unsigned int wwidth, unsigned int wheight)
{
	//Set up the look n' feel of the game area.
	
	g->gridProps.color.a = 255;
	//Sky blue
	g->gridProps.color.r = 120;
	g->gridProps.color.g = 190;
	g->gridProps.color.b = 255;
	g->gridProps.cellSize = cellSize;
	
	//Our window 
	g->window = NULL; 

	g->windowWidth = wwidth;
	g->windowHeight = wheight;

	g->zoom = 0;
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL did not initialize, SDL_GetError() says: %s\n", SDL_GetError() ); 
		return 1;
	}


	 //Create the window 
	g->window = SDL_CreateWindow( wtitle, SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, wwidth, wheight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
	if( g->window == NULL ) 
	{ 
		printf( "Window wasn't initialized, SDL_GetError() says: %s\n", 
			SDL_GetError() ); 
		return 1;
	}
 
	g->renderer = 0;
	g->renderer = SDL_CreateRenderer( g->window, -1, SDL_RENDERER_TARGETTEXTURE );
	if( g->renderer == NULL )
	{
		printf( "Renderer didn't initialize properly, SDL_GetError() says: %s\n", SDL_GetError() );

		return 1;
	}

	//We will need a function for updating game area size,
	//called on world resize, that reallocates this.
	g->gameTexture = SDL_CreateTexture(g->renderer, SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET, world->width*cellSize, world->height*cellSize);

	g->screenGame.x = 0;
	g->screenGame.y = 0;
	g->screenGame.w = world->width*cellSize;
	g->screenGame.h = world->height*cellSize;
}


void DrawGame(GameRenderer* g, World* world)
{

	//Prepare to render to texture.
	SDL_SetRenderTarget(g->renderer, g->gameTexture);
	
	//Clear screen
	SDL_SetRenderDrawColor( g->renderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( g->renderer );
	
	//Draw our grid
	DrawBackgroundGrid( g->renderer, g->screenGame.w, g->screenGame.h, &g->gridProps );		
	DrawCells(g->renderer, &g->gridProps, world);		
	SDL_RenderPresent( g->renderer );

	//Now draw the rest of the screen
	SDL_SetRenderTarget(g->renderer, NULL);
	SDL_SetRenderDrawColor( g->renderer, 180, 180, 180, 255 );
	SDL_RenderClear( g->renderer );

	SDL_RenderCopy(g->renderer, g->gameTexture, NULL, &(g->screenGame));
	SDL_RenderPresent(g->renderer);
}

void DrawCells(SDL_Renderer* render, DrawableGrid* grid, World* w)
{
	bool current = false;
	
	SDL_SetRenderDrawColor(render, 
		grid->color.r, grid->color.g, grid->color.b, grid->color.a);
	for(int x = 0; x < w->width; ++x)
	{
		for(int y = 0; y < w->height; ++y)
		{
			current = GetCell(w, x, y);
			if(current)
			{
				SDL_Rect rectCell =
				 	{x*(grid->cellSize), y*(grid->cellSize),
				 	grid->cellSize, grid->cellSize};
				
				SDL_RenderFillRect(render, &rectCell );
			}
		}
	}	
}

void DrawBackgroundGrid(SDL_Renderer* renderer, unsigned int width, unsigned int height, 
	DrawableGrid* grid)
{
	SDL_SetRenderDrawColor(renderer, grid->color.r, grid->color.g, grid->color.b, grid->color.a);
	//Draw vertical lines.
	for(int x = 0; x < width; x += grid->cellSize)
	{
		SDL_RenderDrawLine( renderer, x, 0, x, height);
	}
	//Draw horizontal lines.
	for(int y = 0; y < height; y += grid->cellSize)
	{
		SDL_RenderDrawLine( renderer, 0, y, width, y);
	}
}

void MoveCamera(GameRenderer* g, int x, int y)
{
	g->screenGame.x += x;
	g->screenGame.y += y;
}
void ZoomIn(GameRenderer* g)
{
	if(g->zoom < MAX_ZOOM_IN)
	{
		g->screenGame.x *= 2;
		g->screenGame.y *= 2;
		g->screenGame.w *= 2;
		g->screenGame.h *= 2;
		++(g->zoom);
	}
}
void ZoomOut(GameRenderer* g)
{
	if(g->zoom > (- 8))
	{
		g->screenGame.x /= 2;
		g->screenGame.y /= 2;
		g->screenGame.w /= 2;
		g->screenGame.h /= 2;
		--(g->zoom);
	}
}
void CleanupRendering(GameRenderer* g)
{
	SDL_DestroyTexture(g->gameTexture);
	SDL_DestroyRenderer(g->renderer);
	SDL_DestroyWindow(g->window);
	SDL_Quit();
}
