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

#include "baloon.h"
#include <SDL_draw.h>

using namespace std;

int BALOON_WIDTH ;
int BALOON_HEIGHT ;

Baloon::Baloon()
{
	TRACE_IN

	//Initialize the offsets
	x = 0;
	y = 0;

	anchor_x =0;
	anchor_y =0;

	bFreeFall = false;
	//The surfaces
	baloon = NULL;

    disable_motion = true;

	if(TheWorld!=NULL)
	{
		//inhiret the world 
		world = TheWorld->world ;
	
		space = TheWorld->space ;
	
		contactgroup = TheWorld->contactgroup ;
	
		ground = TheWorld->ground ;
		ceiling = TheWorld->ceiling ;
		leftwall = TheWorld->leftwall;
		rightwall = TheWorld->rightwall;
	}
	else
		return;

	nBodyNumber =0;

	// Load image and sound files
	load_images(); 
	load_sound();


	TRACE_STRING ("Created baloon. Body #%d",nBodyNumber);
	TRACE_OUT
}

bool Baloon::Create()
{
	TRACE_IN
	
	if(!load_images())
		return false;
	if(!load_sound())
		return false;

	BALOON_WIDTH = baloon->w;
	BALOON_HEIGHT = baloon->h;

	// Construct the ODE Baloon Object 
	body = dBodyCreate (TheWorld->world);

    dMassSetSphere (&mass,TheWorld->balloon_density, BALOON_WIDTH/2);
    dBodySetMass (body, &mass);

	geom = dCreateSphere (TheWorld->space,(dReal)BALOON_WIDTH/2);	
	p=NULL;


	dGeomSetBody (geom,body);
	dGeomSetCategoryBits (geom, BALOON_CATEGORY);
	BaloonContacts = dJointGroupCreate (0);	
	dBodyEnable (body);	
	
	disable_motion = false;
	if((p=AddBody(&nBodyNumber,body,BALOON_TYPE,geom))==NULL)
		TRACE_STRING ("FATAL ERROR: AddBody failed. BALOON"); 
	else
	{
		p->TmpLinks[0]		= 0;		

		p->contacts = BaloonContacts;
		p->bBeingMoved = false;
		p->nJoints = 0;
	
	}
    dBodySetData (body, (void*)p);
	dGeomSetData (geom, (void *)p);

	dAttachCoolDown = GetTime();
	is_poped = false;
	balloon_lift_force = TheWorld->balloon_lift_force;

	return true;
	TRACE_OUT
}

Baloon::~Baloon()
{
	TRACE_IN


	TRACE_OUT
}

void Baloon::CleanUp()
{

	if(BaloonContacts)
	{
		dJointGroupEmpty (BaloonContacts);
		dJointGroupDestroy (BaloonContacts);
		BaloonContacts = NULL;
	}
	
	if(geom)
	{
		//PTBlist thisp = (PTBlist)dGeomGetData( geom);
		if(p)
		{
			DeleteBody(p);
			p = NULL;
		}

		if(body)
		{
			dBodyDestroy(body);
			body = NULL;
		}

		dGeomDestroy(geom);
		geom = NULL;
	}

	if(baloon)
		SDL_FreeSurface(baloon);

	if(pop)
		Mix_FreeChunk(pop);

}
void Baloon::setposition(dReal X, dReal Y)
{
	TRACE_IN
	dBodySetPosition (body,OBJECT_DEPTH,X, Y);
	TRACE_OUT;
}

void Baloon::init_center(int nX, int nY)
{
	TRACE_IN
	x= nX-BALOON_WIDTH/2+TheWorld->bg_x;
	y= SCREEN_HEIGHT - BALOON_HEIGHT/2 -nY-TheWorld->bg_y;
	TRACE_OUT
}

void Baloon::setvelocity(float xV, float yV)
{
	TRACE_IN
	dBodySetLinearVel  (body, 0, xV, yV);
	TRACE_OUT
}

void Baloon::getvelocity(float *xV, float *yV)
{
	TRACE_IN
	const dReal *pos = dBodyGetLinearVel(body);
	*xV = (float)pos[1];
	*yV = (float)pos[2];
	TRACE_OUT
}
void Baloon::set_force_on_baloon(float fx, float fy, float desX, float desY)
{
	TRACE_IN

	dBodyAddForce(body,0, 10.0,10.0);
	TRACE_STRING ("adding force ");
	TRACE_OUT
}

void Baloon::move()
{
	
}



void Baloon::show()
{
	TRACE_IN
	const dReal *pos, *pos_link2;
	int x1,y1,x2,y2;
	int i=0;
	dBodyID tmpBody = 0;

    PTBlist pB = (PTBlist)dGeomGetData( geom);
	Uint32 c_white = SDL_MapRGB(TheWorld->screen->format, 255,255,255);

    // First show baloon
	pos = dBodyGetPosition ( body ); // center of baloon
	init_center(int(pos[1]), (int)pos[2]); // updated x and y of the baloon
	setposition((pos[1]), pos[2]);	

	apply_surface( x, y, baloon , TheWorld->screen );	

	// Set the start 1 of the link
	// Adjust for world view
	x1=x+BALOON_WIDTH/2; // middle x of the baloon
	y1=y+BALOON_HEIGHT;  // bottom y of the baloon

	for(i=0;i<pB->nJoints;i++)
	{
		tmpBody=dJointGetBody (pB->joints[i], 1);

		if(tmpBody)
		{			
			pos_link2 = dBodyGetPosition(tmpBody); // get center of the linked body
			x2= (int) pos_link2[1]+TheWorld->bg_x;  // update it for world view
			y2= SCREEN_HEIGHT - SPHERE_HEIGHT/2 -(int)pos_link2[2]-TheWorld->bg_y;

			//sDrawLine(  x1, y1, x2, y2 , c_white );
			sDrawLine2(x1, y1, x2, y2 ,  255,255,255, 255);

			
		}
		else
		{

			x1 = int(pos[1]+TheWorld->bg_x) ;
			y1 = int(SCREEN_HEIGHT - pos[2]-TheWorld->bg_y + BALOON_HEIGHT/2) ;
			x2 = int(anchor_x+TheWorld->bg_x) ;
			y2 = int(SCREEN_HEIGHT - anchor_y -TheWorld->bg_y ) ;
		
			//sDrawLine(  x1, y1, x2, y2, c_white );
			sDrawLine2(x1, y1, x2, y2 ,  255,255,255, 255);
			//cout<<"Hinge Angel = "<<theta <<endl;
		}

	}
	//dSpaceCollide (TheWorld->space,0,&nearCallback);


	TRACE_OUT
}

void Baloon::stop()
{
	TRACE_IN
	TRACE_OUT
}


void Baloon::handle_input(int nBaloonCount)
{
	TRACE_IN
	PTBlist thisp = (PTBlist)dGeomGetData( geom);
	if(TheWorld->MouseLocked && !thisp->bBeingMoved)
		return;

	if(is_under_mouse() && (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1)))
	{
		dGeomID o1=0;
		o1 = DrawToClosest(geom, TheWorld->SearchRadius);
	}

	if( TheWorld->event.type == SDL_MOUSEBUTTONUP )
	{
		disable_motion = false;
		dBodyEnable (body);
		const dReal *mypos, *pos1;

		if(is_under_mouse()  && thisp->bBeingMoved)
		{
			dBodyEnable (body);
			//dBodySetForce(body,0.0,(dReal) momentum_x, (dReal) momentum_y);
			dGeomID Connect2Geom =0;

			if((Connect2Geom = DrawToClosest(geom, TheWorld->SearchRadius))!=0)
			{
				if( (GetTime() - dAttachCoolDown)> 1) // block multiple BUTTON UP messages for 1 sec after the first
				{
					dJointID joint=0;
					pos1 = dGeomGetPosition(Connect2Geom);
				
					// before you attach the hinge, position the baloon 100pts above the object 
					init_center(int(pos1[1]),int(pos1[2]+100));
					setposition(pos1[1],pos1[2]+100);
					mypos  = dBodyGetPosition(body);
					
					// This next line is EXTREMELY INPORTANT TO SET THE POSITION 
					// OF THE OBJECT YOU ARE CONNECTING TO -- OTHERWISE THE CONNECTION
					// WILL BE IN THE WRONG PLACE !!!!!!!!
					dGeomSetPosition(Connect2Geom,OBJECT_DEPTH,pos1[1],pos1[2]  );
					// Attach it to the object 
					joint = HingeAttach2Bodies(thisp, (PTBlist)dGeomGetData (Connect2Geom));
					if(joint)
						dJointSetHingeAnchor (joint, OBJECT_DEPTH,mypos[1],mypos[2]);

					dAttachCoolDown = GetTime();

					//special case for the ship
					if(dGeomGetCategoryBits(Connect2Geom)==SHIP_CATEGORY)
						balloon_lift_force	= -1.0 * TheWorld->world_gravity * 10000.0f;
				}
			}			
			thisp->bBeingMoved = false;
			disable_motion = false;
			TheWorld->MouseLocked = false;
		}

	}
	else if( TheWorld->event.type == SDL_MOUSEBUTTONDOWN )
	{

		if(is_under_mouse())
		{
			disable_motion = true;
			dBodyDisable (body);
			
			y=TheWorld->event.button.y - BALOON_HEIGHT/2+ TheWorld->bg_y;
			x=TheWorld->event.button.x - TheWorld->bg_x;

			dGeomSetPosition (geom,(dReal)OBJECT_DEPTH,(dReal)x, (dReal) (SCREEN_HEIGHT - BALOON_HEIGHT/2 - y));

			dBodySetLinearVel  (body, 0, 0, 0);
			dBodySetForce  (body, 0, 0, 0);

			thisp->bBeingMoved = true;
			DrawToClosest(geom, TheWorld->SearchRadius);
			TheWorld->MouseLocked = true;
		}
		//else if(thisp->bBeingMoved)
		//{
		//	dBodyEnable (body);
		//	thisp->bBeingMoved = false;
		//	disable_motion = false;
		//}		
	}
	else if( TheWorld->event.type == SDL_MOUSEMOTION && (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1)))
	{
		dGeomID o1=0;
		int CutDistance = 150;

		//if(is_under_mouse())
		if(thisp->bBeingMoved)
		{
			dBodyID CutBody =0;
			dJointID jointID = 0;
			
			disable_motion = true;
			dBodyDisable (body);

			int new_y=TheWorld->event.button.y - BALOON_HEIGHT/2 + TheWorld->bg_y;
			int new_x=TheWorld->event.button.x - BALOON_WIDTH/2  - TheWorld->bg_x;

			momentum_x = ( - x	+ new_x		+ TheWorld->bg_x )*weight()*100;
			momentum_y = (   y	- new_y		+ TheWorld->bg_y )*weight()*100;
			
			y = new_y;
			x = new_x + BALOON_WIDTH/2;

			dGeomSetPosition (geom,(dReal)OBJECT_DEPTH,(dReal)x , (dReal)(SCREEN_HEIGHT - BALOON_HEIGHT/2 - y));
			
			dBodySetLinearVel  (body, 0, 0, 0);
			thisp->bBeingMoved = true;

			o1 = DrawToClosest(geom, TheWorld->SearchRadius);

			// sever the connection if the Baloon is pulled away from any object by 150 world coor units
			if(thisp->nJoints>0)
			{
				CutBody = GetFurthestAttached(body, CutDistance, &jointID);
				if(jointID!=0 && CutBody==0)
				{
					// we must have a static joint
					// special case cut
					const dReal * pos = dGeomGetPosition(geom);
					double dTestDist = sqrt(pow((anchor_x-pos[1]),2)+pow((anchor_y-pos[2]),2));
					if (dTestDist > CutDistance)
					{
						dJointAttach(jointID,0,0);
						thisp->nJoints--;
					}
				}
				else if(jointID!=0 && CutBody)
				{
					dJointAttach(jointID,0,0);
					PTBlist pCutB = (PTBlist) dBodyGetData(CutBody);
					if(thisp->nJoints==MAX_JOINTS)
					{
						Mix_PlayChannel( -1, pop, 0 );
						dJointGroupEmpty (thisp->contacts);
						is_poped = true;
					}
					thisp->nJoints--;
					pCutB->nJoints--;
				}
			}	
		}
		//else if(thisp->bBeingMoved)
		//{
			////dBodyEnable (body);
			//thisp->bBeingMoved = false;
			//disable_motion = false;
		//}
	}
	
	// Call default input handler
	SDL_World::handle_input();

	TRACE_OUT
}


bool  Baloon::load_sound(void)
{
	TRACE_IN    
	//Load the sound effects
	SDL_World::load_sound(".\\sounds\\pop.wav",&pop);

	TRACE_OUT
	return true;
}

