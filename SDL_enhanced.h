#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "SDL2-2.0.10/include/SDL.h"
#include "SDL2-2.0.10/include/SDL_main.h"

typedef struct {
	SDL_Renderer* renderer;
	SDL_Window* window;
	SDL_Texture* scrtex;
	SDL_Surface* screen, * charset;

	uint32_t tick;
	double delta;
} EnvironmentVars;

// draw a text txt on surface screen, starting from the point (x, y)
// charset is a 128x128 bitmap containing character images
void DrawString(SDL_Surface* screen, int x, int y, const char* text,
    SDL_Surface* charset);

// draw a surface sprite on a surface screen in point (x, y)
// (x, y) is the center of sprite on screen
void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y);

// draw a single pixel
void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color);

// draw a vertical (when dx = 0, dy = 1) or horizontal (when dx = 1, dy = 0) line
void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color);

// draw a rectangle of size l by k
void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor);

double getDeltaTime(uint32_t* lastTick);

// Get texture from given source
// Returns NULL on fail
SDL_Texture* GetTexture(SDL_Renderer* renderer, char* src);

// Create rectangle inline
SDL_Rect CreateRect(int x, int y, int w, int h);
