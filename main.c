#include <stdio.h>
#include <SDL2/SDL.h>

#include "world.h"
#include "render.h"

int main()
{
	World ourWorld;
	InitializeWorld(&ourWorld, 256, 256);
	printf("Hello World!\n");

	SetCell(&ourWorld, 26, 26, true);
	SetCell(&ourWorld, 26, 27, true);
	SetCell(&ourWorld, 26, 28, true);
	SetCell(&ourWorld, 27, 27, true);

	DrawableGrid bgGrid;
	bgGrid.color.a = 255;
	bgGrid.color.r = 120;
	bgGrid.color.g = 190;
	bgGrid.color.b = 255;
	bgGrid.cellSize = 15;

	GameRenderer ren;
	ren.gridProps = &bgGrid;
	InitGameRenderer(&ren, "Conway's Game of Life", 640, 480);	
	//Mainloop here.
	bool quit = false;
	bool paused = true;
	unsigned int generation = 0;
	SDL_Event event;
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
