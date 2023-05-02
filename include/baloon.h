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
#ifndef _BALOON_H
#define _BALOON_H  1

#include "ode_world.h"
#include "sdl_world.h"


//The Baloon

class Baloon : public SDL_World , public MyWorld
{
	private:
		//The offsets
		int x, y;
		int next_awake;
		
		
	public: 
		
		//Initializes
		Baloon();
		//Cleans up particles
		~Baloon();

		void init_center(int x, int y);
		
		//Handles keypresses
		void handle_input(int nDotCount);

		//is baloon under the mouse
		bool is_under_mouse()
		{
			return SDL_World ::is_under_mouse(x,y, BALOON_WIDTH, BALOON_HEIGHT);
		};
		bool disable_motion;
		
		//Moves the Baloon
		void move();

		void stop();

		//Shows the Baloon
		void show();

		char szName[20];
   
		bool bFreeFall;
		

		SDL_Surface *baloon ;
		double dAttachCoolDown ;
		Mix_Chunk *pop ;
		PTBlist p;
		bool is_poped;
		dWorldID world;
		dSpaceID space;
		dJointGroupID BaloonContacts;
		double balloon_lift_force;
		
		dGeomID ground, ceiling;
		dGeomID rightwall,leftwall;	

		double anchor_x;
		double anchor_y;
		
		dBodyID body;

		dGeomID geom;

		dMass mass;
		double density;
		const dReal *init_bpos;
        unsigned long  nBodyNumber ;
		double momentum_x;
		double momentum_y;

		
		bool Create();

		void setposition(dReal,dReal);
		void setvelocity(float,float);
		void getvelocity(float*,float*);
		void set_force_on_baloon(float , float , float , float );
		void rotate(double);
		void CleanUp(void);
		
		bool load_sound();

		bool load_images()		
		{
			return SDL_World :: load_images(".\\images\\baloon1.png", &baloon);
		};
		double volume()
		{
			return ((PI/6)*BALOON_WIDTH * BALOON_WIDTH * BALOON_WIDTH);
		}
		double weight()
		{
			//std::cout<<"volume = "<<volume()<<std::endl;
			return (density * volume());
		}
		int get_next_awake() {if (next_awake==(MAX_BALOON_AWAKE-1)) next_awake=0;else next_awake++;return next_awake;}		
};
#endif

