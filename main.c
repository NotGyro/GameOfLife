#include <stdio.h>
#include <SDL2/SDL.h>

#include "world.h"
#include "render.h"

int main()
{
	World ourWorld;
	InitializeWorld(&ourWorld, 256, 256);
	printf("Hello World!\n");

	DrawableGrid bgGrid;
	bgGrid.color.a = 255;
	bgGrid.color.r = 120;
	bgGrid.color.g = 190;
	bgGrid.color.b = 255;
	bgGrid.cellSize = 16;

	GameRenderer ren;
	ren.gridProps = &bgGrid;
	InitGameRenderer(&ren, "Conway's Game of Life", 640, 480);	
	//Mainloop here.
	bool quit = false;
	bool paused = true;
	unsigned int generation = 0;
	SDL_Event event;
	
	SetCell(&ourWorld, 3, 2, true);
	SetCell(&ourWorld, 4, 2, true);
	SetCell(&ourWorld, 3, 3, true);
	SetCell(&ourWorld, 3, 4, true);
	//Glider
	SetCell(&ourWorld, 9, 4, true);
	SetCell(&ourWorld, 10, 5, true);
	SetCell(&ourWorld, 10, 6, true);
	SetCell(&ourWorld, 9, 6, true);
	SetCell(&ourWorld, 8, 6, true);
	FlipBuffers(&ourWorld);
	while(!quit)
	{
		//World logic
		if(!paused)
		{
			Update(&ourWorld);
			++generation;
		}	
		//Input
		while( SDL_PollEvent(&event) != 0)
		{
			//Closed window or somesuch
			if(event.type == SDL_QUIT)
			{
				quit = true;
			}
			else if( event.type == SDL_KEYDOWN ) 
			{
				switch( event.key.keysym.sym ) 
				{ 
					case SDLK_ESCAPE: 
					quit=true; 
					break;
					
					case SDLK_SPACE:
					paused = !paused;
					break;
					
					case SDLK_n:
					Update(&ourWorld);
					++generation;
					break;

					default: 
					break; 
				} 
			}	
		}
		//Rendering happens here:
		DrawGame(&ren, &ourWorld);		
	}
	CleanupRendering(&ren);
	return 0;
}
