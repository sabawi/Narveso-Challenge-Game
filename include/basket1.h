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
#ifndef _BASKET1_H
#define _BASKET1_H  1

#include "ode_world.h"
#include "sdl_world.h"

//The Basket1

class Basket1 : public SDL_World , public MyWorld
{
	private:
		//The offsets
		int x, y;
		
		
	public: 
		
		//Initializes
		Basket1();
		//Cleans up particles
		~Basket1();

		void init_center(int x, int y);
		
		//Handles keypresses
		void handle_input();

		//is box under the mouse
		bool is_under_mouse(int X, int Y);
		
		//Moves the Basket1
		void move();

		void stop();

		//Shows the Basket1
		void show();

		char szName[20];
   
		bool bFreeFall;
		
		bool awake;
		
		double rot_theta;
		double dTimeKeeper1 ;
		double dTimeKeeper2 ;
		SDL_Surface *basket1 ;
		
		dWorldID world;
		dSpaceID space;
		dJointGroupID BasketContacts;
		
		
		dGeomID ground, ceiling;
		dGeomID rightwall,leftwall;		
		
		dBodyID body;
		dGeomID geom;
		dMass mass;
        PTBlist p;
		dMatrix3 dMat; // rotation matrix
		unsigned long  nBodyNumber ;

		void setposition(double,double);
		void setvelocity(float,float);
		void getvelocity(float*,float*);
		void CleanUp(void);
		void set_force_on_basket1(float , float , float , float );
		bool load_images();
		bool is_under_mouse()
		{
			return SDL_World ::is_under_mouse(x,y, (int)BASKET_WIDTH, (int)BASKET_HEIGHT);
		};
};

#endif
