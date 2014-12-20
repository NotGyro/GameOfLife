#include <stdio.h>
#include <SDL2/SDL.h>

#include "world.h"
#include "render.h"

int main()
{
	World ourWorld;
	InitializeWorld(&ourWorld, 64, 64);


	GameRenderer ren;
	InitGameRenderer(&ren, &ourWorld, 16, "Conway's Game of Life", 640, 480);	
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
	bool dragMode = false;
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
			else if( event.type == SDL_MOUSEWHEEL )
			{
				if(event.wheel.y > 0)
				{
					ZoomIn(&ren);
				}
				else if(event.wheel.y < 0)
				{
					ZoomOut(&ren);
				}
			}
			else if( event.type == SDL_MOUSEMOTION )
			{
				if(dragMode == true)
				{
					MoveCamera(&ren, event.motion.xrel, event.motion.yrel);	
				}
			}
			else if( event.type == SDL_MOUSEBUTTONDOWN)
			{
				if(event.button.button == SDL_BUTTON_RIGHT)
				{
					dragMode = true;
				}
			}
			else if( event.type == SDL_MOUSEBUTTONUP)
			{
				if(event.button.button == SDL_BUTTON_RIGHT)
				{
					dragMode = false;
				}
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
	DestroyWorld(&ourWorld);
	CleanupRendering(&ren);
	return 0;
}
