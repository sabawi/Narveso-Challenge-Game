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
#ifndef _WALKER_H
#define _WALKER_H  1

#include "ode_world.h"
#include "sdl_world.h"

//The Walker

class Walker : public SDL_World , public MyWorld
{
	private:
		//The offsets
		int x, y;
		int next_awake;
		
		
	public: 
		
		//Initializes
		Walker();
		//Cleans up particles
		~Walker();

		void init_center(int x, int y);
		
		//Handles keypresses
		void handle_input(int nDotCount);

		//is walker under the mouse
		bool is_under_mouse(int X, int Y);
		bool disable_motion;
		
		//Moves the Walker
		void move();

		void stop();

		//Shows the Walker
		void show(const dReal *);

		char szName[20];
   
		bool bFreeFall;
		

		SDL_Surface *walker ;
		SDL_Surface *eye_l;
		SDL_Surface *eye_r;
		SDL_Surface *eye_sleep_l;
		SDL_Surface *eye_sleep_r;
		SDL_Surface *eye_pupil;
		double dAttachCoolDown ;
		Mix_Chunk *wow ;
		Mix_Chunk *cheers;
		PTBlist p;
		bool on_the_ship;
		dWorldID world;
		dSpaceID space;
		dJointGroupID WalkerContacts;
		
		dGeomID ground, ceiling;
		dGeomID rightwall,leftwall;		
		
		dBodyID body;
		dBodyID ship_body;

		dGeomID geom;
		int nBodyNum_iam_on, nBodyNum_next_geom_to_go;

		double density;
		dMass mass;
		const dReal *init_bpos;
        unsigned long  nBodyNumber ;

		double wt;
		
		void setposition(dReal,dReal);
		void setvelocity(float,float);
		void getvelocity(float*,float*);
		void set_force_on_walker(float , float , float , float );
		void rotate(double);
		void CleanUp(void);
		void MakeBody(void);
		bool load_sound();
		bool load_images();
		double volume()
		{
			return ((PI/6)*WALKER_WIDTH * WALKER_WIDTH * WALKER_WIDTH);
		}
		double weight()
		{
			//std::cout<<"volume = "<<volume()<<std::endl;
			return (density * volume());
		}
		int get_next_awake() {if (next_awake==(MAX_WALKER_AWAKE-1)) next_awake=0;else next_awake++;return next_awake;}		
};
#endif

