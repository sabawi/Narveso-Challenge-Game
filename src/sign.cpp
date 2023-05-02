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
#include "ode_world.h"

#include "sign.h"
#include <SDL_draw.h>

using namespace std;


Sign::Sign()
{
	TRACE_IN

	//Initialize the offsets
	x = 0;
	y = 0;

	sign = NULL;
	sign_filename = NULL;

	nSignWidth = 60;
	nSignHeight = 40;


	TRACE_OUT
}

Sign::~Sign()
{
	TRACE_IN
	


	TRACE_OUT
}

void Sign::place_signdull(char * text, int ax, int ay)
{
	x= ax, y= ay;
	drawText(text,36, ax, ay,100, 100, 100, 10, 10, 10);
}
void Sign::place_signbright(char * text, int ax, int ay)
{
	x= ax, y= ay;
	drawText(text, 36,ax, ay, 255, 255, 255, 0, 0, 0);

}
void Sign::place_signcolor(char *text, int ax, int ay, int font_size, Uint32 color)
{
	x= ax, y= ay;
	drawText2( text, font_size ,ax, ay ,color);

}

void Sign::place_sign(char * filename, int ax, int ay)
{
	TRACE_IN
	x= ax, y= ay;
	sign_filename = filename;
	load_images();
	TRACE_OUT

}


void Sign::show()
{
	TRACE_IN

	//Uint32 color = SDL_MapRGB(TheWorld->screen->format, 00,255,00);

	//apply_surface( x,  y, sign , TheWorld->screen );	

	TRACE_OUT
}


