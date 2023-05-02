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
#ifndef _BOX_H
#define _BOX_H  1

#include "ode_world.h"
#include "sdl_world.h"

//The Box

class Box : public SDL_World , public MyWorld
{
	private:
		//The offsets
		int x, y;
		
		
	public: 
		
		//Initializes
		Box();
		//Cleans up particles
		~Box();

		void init_center(int x, int y);
		int o_x;
		int o_y;

		//Handles keypresses
		//void handle_input(int nDotCount);

		//is box under the mouse
		bool is_under_mouse(int X, int Y);
		
		//Moves the Box
		void move();

		void stop();

		//Shows the Box
		void show();

		char szName[20];
   
		bool bFreeFall;
		
		bool awake;
		
		SDL_Surface *box ;
		
		dWorldID world;
		dSpaceID space;
		
		dJointGroupID BoxContacts;
		
		
		dGeomID ground, ceiling;
		dGeomID rightwall,leftwall;		
		
		dBodyID body;
		dGeomID geom;
		dMass mass;
        PTBlist p;

		unsigned long  nBodyNumber ;

		void setposition(float,float);
		void setvelocity(float,float);
		void getvelocity(float*,float*);
		void set_force_on_box(float , float , float , float );
		bool load_images();
		void CleanUp()
		{
			if(p)
				free(p);

			if(box)
				SDL_FreeSurface(box);

			dJointGroupEmpty (BoxContacts);
			dJointGroupDestroy (BoxContacts);
		}
};

#endif
