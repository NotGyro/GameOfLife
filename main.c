#include <stdio.h>
#include <stdint.h>
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
	bool displayNew = false;
	int mousex = 0;
	int mousey = 0;
	
	uint32_t lastUpdate = 0;	
	uint32_t tickDelay = 80;
	while(!quit)
	{
		//World logic
		if(!paused)
		{
			uint32_t currentTime = SDL_GetTicks();
			if((currentTime -lastUpdate) >= tickDelay)
			{ 
				Update(&ourWorld);
				++generation;
				
				lastUpdate = currentTime;
			}
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

				mousex = event.motion.x;
				mousey = event.motion.y;
				
			}
			else if( event.type == SDL_MOUSEBUTTONDOWN )
			{
				if(event.button.button == SDL_BUTTON_MIDDLE)
				{
					dragMode = true;
				}
			}
			else if( event.type == SDL_MOUSEBUTTONUP)
			{
				if(event.button.button == SDL_BUTTON_MIDDLE)
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
					
					//Slow down
					case SDLK_PAGEDOWN:
					if(tickDelay < 1000)
					{
						tickDelay += 20;
					}
					break;
					
					//Speed up
					case SDLK_PAGEUP:
					if(tickDelay > 10)
					{
						tickDelay -= 10;
					}
					break;

					default: 
					break; 
				} 
			}	
		}

		if (SDL_GetMouseState(NULL, NULL) 
			& SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			WorldCoord wc = 
			ScreenToWorldCoord(&ren, mousex, mousey);	
			SetCell(&ourWorld, wc.x, wc.y, true);
			displayNew = true;
		}
		else if (SDL_GetMouseState(NULL, NULL) 
			& SDL_BUTTON(SDL_BUTTON_RIGHT))
		{
			WorldCoord wc = 
			ScreenToWorldCoord(&ren, mousex, mousey);	
			SetCell(&ourWorld, wc.x, wc.y, false);
			displayNew = true;
		}
		//Flip the buffers so you can see any cells you've clicked in
		if(displayNew)
		{
			FlipBuffers(&ourWorld);
			displayNew = false;	
		}		
		//Rendering happens here:
		DrawGame(&ren, &ourWorld);		
	}
	DestroyWorld(&ourWorld);
	CleanupRendering(&ren);
	return 0;
}
