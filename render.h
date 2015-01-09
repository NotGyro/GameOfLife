#ifndef RENDER_H
#define RENDER_H
#include <SDL2/SDL.h>
#include <stdint.h>
#include "world.h"

typedef struct DrawableGrid DrawableGrid;

#define GRID_COLOR_BYTE_DEPTH 3

struct DrawableGrid 
{
	SDL_Color color;

	uint8_t cellSize;
};

//Wraps all rendering state for the game.

typedef struct GameRenderer GameRenderer;

struct GameRenderer
{
	//The window we'll be rendering to 
	SDL_Window* window; 

	//unsigned int gameWidth;
	//unsigned int gameHeight;

	unsigned int windowWidth;
	unsigned int windowHeight;

	int zoom;

	SDL_Renderer* renderer;

	DrawableGrid gridProps;

	//Size and relative position of the game on the screen.	
	SDL_Rect screenGame;  

	SDL_Texture* gameTexture;

	//Tracks the variable of the same name in the world struct.
	unsigned int resizeRevision;	
};

void InitGameRenderer(GameRenderer*, World*, unsigned char, const char*, unsigned int, unsigned int);
void CleanupRendering(GameRenderer*);

void DrawGame(GameRenderer*, World*);
void DrawBackgroundGrid(SDL_Renderer*, unsigned int, unsigned int, DrawableGrid*);

void MoveCamera(GameRenderer*, int, int);
void ZoomIn(GameRenderer*);
void ZoomOut(GameRenderer*);

WorldCoord ScreenToWorldCoord(GameRenderer*, int, int);
#endif
