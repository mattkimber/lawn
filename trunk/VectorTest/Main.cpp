/*
 * Copyright (C) 2011 by Matt Kimber
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "SDL.h"
#include "Main.h"
#include "CatmullRom.h"


void BlitData(SDL_Surface* surf)
{
	// Very raw (and crappy) blit
	// routine.  Only works for 32
	// bits!
	for(int i = 0; i < TILE_SIZE; ++i)
	{
		for(int j = 0; j < TILE_SIZE; ++j)
		{
			// Set the r,g,b values
			*((Uint16 *)(0+(Uint8*)surf->pixels + (i * (surf->pitch)) + (j*4)+0)) = data[(i*TILE_SIZE)+j];
			*((Uint16 *)(0+(Uint8*)surf->pixels + (i * (surf->pitch)) + (j*4)+1)) = data[(i*TILE_SIZE)+j];
			*((Uint16 *)(0+(Uint8*)surf->pixels + (i * (surf->pitch)) + (j*4)+2)) = data[(i*TILE_SIZE)+j];
		}
	}
}

void Draw()
{
	data = new Uint8[TILE_SIZE*TILE_SIZE];

	for(int i = 0; i < TILE_SIZE; ++i)
	{
		for(int j = 0; j < TILE_SIZE; ++j)
		{
			data[(i*TILE_SIZE)+j] = 127;
		}
	}
}


int main( int argc, char* args[] ) 
{
	CatmullRom c;

	c.AddPoint(1.0, 2.0, 3.0);
	c.AddPoint(3.0, 2.0, 3.0);
	c.AddPoint(5.0, 2.0, 3.0);
	c.AddPoint(7.0, 2.0, 4.0);

	SplinePoint sp = c.GetPoint(2);

	//Start SDL 
	SDL_Init( SDL_INIT_EVERYTHING ); 
	
	// Create an SDL surface ptr for the screen
	SDL_Surface* screen = NULL;

	// Set the screen up
	screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);

	SDL_Event evt;
	bool quit = false;

	// Draw this set of vector data
	Draw();

	// Blit it to the surface
	BlitData(screen);

	SDL_Flip(screen);

	// Main loop
	while(!quit)
	{
		// Poll events
		while(SDL_PollEvent(&evt))
		{
			// A reason to exit.
			if(evt.type == SDL_QUIT)
			{
				quit = true;
			}
		}
	}


	//Quit SDL
	SDL_Quit(); 
	
	return 0; 

}