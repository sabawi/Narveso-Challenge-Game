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
#ifndef _SIGN_H
#define _SIGN_H  1

#include "ode_world.h"
#include "sdl_world.h"

//The Sign

class Sign : public SDL_World , public MyWorld
{
	private:
		//The offsets
		int x, y;
		SDL_Surface *sign ;
		char * sign_filename ;		

		bool load_images()
		{
			if(sign_filename!=NULL)
				return SDL_World :: load_images(sign_filename, &sign);
			else 
				return false;
		};	

	public: 
		
		//Initializes
		Sign();
		//Cleans up particles
		~Sign();


		//Shows the Sign
		void show();

		int nSignWidth ;
		int nSignHeight;

		void SetSignDim(int w, int h)
		{
			nSignWidth = w;
			nSignHeight = h;
		}

		//is sign under the mouse
		bool is_under_mouse()
		{
			return SDL_World ::is_under_mouse(x,y, nSignWidth, nSignHeight);
			
		};
		
		void place_signdull(char * , int , int );
		void place_signbright(char * , int , int );
		void place_signcolor(char * , int , int , int , Uint32 );
		void place_sign(char * , int , int );
		void CleanUp(void)
		{
			if(sign)
				SDL_FreeSurface(sign);
		
		}
};
#endif

