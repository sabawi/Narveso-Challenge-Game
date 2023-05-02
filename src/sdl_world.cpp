/***************************************************************************
 *   Copyright (C) 2009 by Al Sabawi   *
 *   sabawi@sabawi1   *
 *                                                                         *
 *   Permission is hereby granted, free of charge, to any person obtaining *
 *   a copy of this software and associated documentation files (the       *
 *   "Software"), to deal in the Software without restriction, including   *
 *   without limitation the rights to use, copy, modify, merge, publish,   *
 *   distribute, sublicense, and/or sell copies of the Software, and to    *
 *   permit persons to whom the Software is furnished to do so, subject to *
 *   the following conditions:                                             *
 *                                                                         *
 *   The above copyright notice and this permission notice shall be        *
 *   included in all copies or substantial portions of the Software.       *
 *                                                                         *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       *
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    *
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. *
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
 *   OTHER DEALINGS IN THE SOFTWARE.                                       *
 ***************************************************************************/
#include "common.h"
#include "sdl_world.h"
#include "sdl_utils.h"
#include <iostream>

using namespace std;

SDL_World :: SDL_World()
{
	TRACE_IN


	TRACE_OUT
	
}

SDL_World :: ~SDL_World(void)
{
	TRACE_IN

	TRACE_OUT
	
}
bool SDL_World :: is_under_mouse(int x, int y, int w, int h)
{
	TRACE_IN
	
	int X = TheWorld->event.motion.x; 
	int Y = TheWorld->event.motion.y;
	int topY, botY, leftX, rightX;
   
 
	leftX = x ;
	rightX = x + w;
	topY = y;
	botY = y + h;

	if ( (X>=leftX && X<=rightX) && (Y<=botY && Y>= topY) )
	{
		TRACE_OUT
		return true;
	}
	else
	{ 
		TRACE_OUT
		return false;
	}
}

bool SDL_World ::load_images(char* img_file, SDL_Surface** destination)
{
	
	TRACE_IN
	load_files( img_file, destination);
	  
	//If there was a problem in loading the walkert
	if( *destination == NULL )
	{
		return false;
		TRACE_STRING("Failed to load image file %s",img_file);
	}

	TRACE_OUT
	return true;
	
}
bool  SDL_World::load_sound(char* wave_file,Mix_Chunk ** mix_ptr)
{
	TRACE_IN    
	//Load the sound effects
	*mix_ptr = Mix_LoadWAV( wave_file );
	TRACE_OUT
	return true;
}

// Default input handler
void SDL_World::handle_input()
{
	// if key board key was pressed
	if( TheWorld->event.type == SDL_KEYDOWN )
	{
		switch( TheWorld->event.key.keysym.sym )
		{
			case SDLK_f: 
				SDL_WM_ToggleFullScreen(TheWorld->screen); 
				break;
			case SDLK_z:
				TheWorld->rotozoom.Enable = true;
			case SDLK_UP:  break;
			case SDLK_DOWN:  break;
			case SDLK_LEFT:  break;
			case SDLK_RIGHT:  break;
			default:
				break;
		}
	} 
	//If a key was released
	else if( TheWorld->event.type == SDL_KEYUP )
	{
		switch( TheWorld->event.key.keysym.sym )
		{
			case SDLK_UP: 
				SDL_Delay(1000); // sleep for 1 second
				break;

			case SDLK_DOWN:  break;
			case SDLK_LEFT:  break;
			case SDLK_RIGHT:  break;
			default:
				break;
		}
	}
}