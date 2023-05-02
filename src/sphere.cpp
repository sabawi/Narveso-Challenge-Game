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

#include "sphere.h"
#include <SDL_draw.h>

using namespace std;


Sphere::Sphere()
{
	TRACE_IN

	//Initialize the offsets
	x = 0;
	y = 0;

	awake = false;
	awake_mad = false;
	next_awake = 0;

	//The surfaces
	sphere = NULL;
	link1 = NULL;
	link2 = NULL;
	link3 = NULL;
	link4 = NULL;

	eye_l = NULL;
	eye_r = NULL;
	eye_sleep_l = NULL;
	eye_sleep_r = NULL;
	eye_pupil = NULL;

	frame_count =0;
	red = NULL;
	green = NULL;
	blue = NULL;
	shimmer = NULL;
	suf_images = 5;
	surf_inc = false;
	next_surf = 0;
	shadow_images = 5;
	shadow_inc = false;
	next_shadow = 0;
	for(int i=0;i<32;i++)
	{
		BallArray[i] = NULL;
		BallShadowArray[i] = NULL;
	}
	//The sound effects that will be used
	scratch = NULL;
	high = NULL;
	med = NULL;
	low = NULL;
	m_sound1=NULL;	
	m_sound2=NULL;
	m_sound3=NULL;
	m_sound4=NULL;
	m_sound5=NULL;

	wow = NULL;
	iluvit = NULL;
	PTBlist p=NULL;
	momentum_x =0;
	momentum_y =0;

	TRACE_OUT
}

Sphere::~Sphere()
{
	TRACE_IN

	TRACE_OUT
}

bool Sphere::Create(double x, double y)
{
	TRACE_IN
	bool ret = true;	
	
	if(!Sphere::load_images())
		return false;

	if(!Sphere::load_sound())
		return false;  

	if(TheWorld!=NULL)
	{
		//inhiret the world 
		world = TheWorld->world ;
	
		space = TheWorld->space ;
	
		ground = TheWorld->ground ;
		ceiling = TheWorld->ceiling ;
		leftwall = TheWorld->leftwall;
		rightwall = TheWorld->rightwall;
	}
	else
		return false;

	Type = REGULAR;
	body = dBodyCreate (TheWorld->world);

	//density = 0.2f;
	density = TheWorld->sphere_density;
    dMassSetSphere (&mass,(dReal) density, SPHERE_WIDTH/2);

    dBodySetMass (body, &mass);

	geom = dCreateSphere (TheWorld->space,(dReal)SPHERE_WIDTH/2);	

	SphereContacts = dJointGroupCreate (0); 
	
	dGeomSetBody (geom,body);
	dGeomSetCategoryBits (geom, HARDBALL_CATEGORY);

    nBodyNumber =0;
 
	
	if((p=AddBody(&nBodyNumber,body,SPHERE_TYPE,geom))==NULL)
		TRACE_STRING("FATAL ERROR: AddBody failed."); 
	else
	{
		p->links_surface[0] = link1;
		p->links_surface[1] = link2;
		p->links_surface[2] = link1;
		p->links_surface[3] = link2;
		p->contacts = SphereContacts;
		p->bBeingMoved = false;
		p->nJoints = 0;
	}
    dBodySetData (body, (void*)p);
	dGeomSetData (geom, (void *)p);
	dAttachCoolDown = GetTime();

	init_center(int(x),int(y));
	setposition(x,y);

	return true;

	TRACE_STRING("Created sphere. Body #%d",nBodyNumber);
	TRACE_OUT
}


bool Sphere::CreateRed(double x, double y)
{
	TRACE_IN
	bool ret = true;
	if(TheWorld!=NULL)
	{
		//inhiret the world 
		world = TheWorld->world ;
	
		space = TheWorld->space ;
	
	
		ground = TheWorld->ground ;
		ceiling = TheWorld->ceiling ;
		leftwall = TheWorld->leftwall;
		rightwall = TheWorld->rightwall;
	}
	else
		return false;
	Type = RED;
	body = dBodyCreate (TheWorld->world);

    dMassSetSphere (&mass,TheWorld->sphere_density, SPHERE_WIDTH/2);

    dBodySetMass (body, &mass);

	geom = dCreateSphere (TheWorld->space,(dReal)SPHERE_WIDTH/2);	

	SphereContacts = dJointGroupCreate (0); 
	
	dGeomSetBody (geom,body);
	dGeomSetCategoryBits (geom, HARDBALL_CATEGORY);

    nBodyNumber =0;
	if(!Sphere::load_imagesHHRed())
		return false;

	if(!Sphere::load_sound())
		return false;   
	
	if((p=AddBody(&nBodyNumber,body,SPHERE_TYPE,geom))==NULL)
		TRACE_STRING("FATAL ERROR: AddBody failed."); 
	else
	{
		p->links_surface[0] = link1;
		p->links_surface[1] = link2;
		p->links_surface[2] = link1;
		p->links_surface[3] = link2;
		p->contacts = SphereContacts;
		p->bBeingMoved = false;
		p->nJoints = 0;
	}
    dBodySetData (body, (void*)p);
	dGeomSetData (geom, (void *)p);
	dAttachCoolDown = GetTime();

	init_center(int(x),int(y));
	setposition(x,y);

	return true;

	TRACE_STRING("Created sphere. Body #%d",nBodyNumber);
	TRACE_OUT
}

void Sphere::CleanUp()
{

	if(!body || !geom)
		return;

	if(SphereContacts)
	{
		dJointGroupEmpty (SphereContacts);
		dJointGroupDestroy (SphereContacts);
		SphereContacts = NULL;
	}
	
	if(geom)
	{
		//PTBlist thisp = (PTBlist)dGeomGetData( geom);
		if(p)
		{
			DeleteBody(p);
			p = NULL;
		}
		dGeomDestroy(geom);
		geom = NULL;
		if(body)
		{
			dBodyDestroy(body);
			body = NULL;
		}

		
		
	}

	if(sphere)	
		SDL_FreeSurface(sphere) ;
	sphere = NULL;

	if(sphere_shadow)	
		SDL_FreeSurface(sphere_shadow) ;
	sphere_shadow = NULL;

	if(red)	
		SDL_FreeSurface(red) ;
	red = NULL;

	if(green)	
		SDL_FreeSurface(green) ;
	green = NULL;

	if(blue)	
		SDL_FreeSurface(blue) ;	
	blue = NULL;
	
	if(shimmer)	
		SDL_FreeSurface(shimmer) ;
	shimmer = NULL;


	if(link1)	
		SDL_FreeSurface(link1) ;	
	link1 = NULL;
	
	if(link2)	
		SDL_FreeSurface(link2) ;
	link2 = NULL;

	if(link3)	
		SDL_FreeSurface(link3) ;
	link3  = NULL;

	if(blue)	
		SDL_FreeSurface(blue) ;	
	blue = NULL;
	
	if(link4)	
		SDL_FreeSurface(link4) ;

	if(eye_l)	
		SDL_FreeSurface(eye_l) ;
	link4 = NULL;
	
	if(eye_r)	
		SDL_FreeSurface(eye_r) ;
	eye_r = NULL;

	if(eye_sleep_l)	
		SDL_FreeSurface(eye_sleep_l) ;
	eye_sleep_l = NULL;

	if(eye_sleep_r)	
		SDL_FreeSurface(eye_sleep_r) ;	
	eye_sleep_r = NULL;
	
	if(eye_pupil)	
		SDL_FreeSurface(eye_pupil) ;
	eye_pupil = NULL;

			
	for(int i=0;i<32;i++)
	{
		if(BallShadowArray[i])	
			SDL_FreeSurface(BallShadowArray[i]) ;
		BallShadowArray[i] = NULL;
	}
	for(int i=0;i<32;i++)
	{
		if(BallArray[i])	
			SDL_FreeSurface(BallArray[i]) ;
		BallArray[i] = NULL;
	}


	if(scratch)
		Mix_FreeChunk(scratch);

	if(high)
		Mix_FreeChunk(high);

	if(med)
		Mix_FreeChunk(med);

	if(low)
		Mix_FreeChunk(low);

	if(iluvit)
		Mix_FreeChunk(iluvit);

	if(wow)
		Mix_FreeChunk(wow);

	if(mad_sounds[0])
		Mix_FreeChunk(mad_sounds[0]);

	if(mad_sounds[1])
		Mix_FreeChunk(mad_sounds[1]);

	if(mad_sounds[2])
		Mix_FreeChunk(mad_sounds[2]);
	
	if(mad_sounds[3])
		Mix_FreeChunk(mad_sounds[3]);
	
	if(mad_sounds[4])
		Mix_FreeChunk(mad_sounds[4]);
 

}
void Sphere::setposition(dReal X, dReal Y)
{
	TRACE_IN
	if(!geom)
		return;
	dGeomSetPosition (geom,OBJECT_DEPTH,X, Y);	
	TRACE_OUT
}

void Sphere::init_center(int nX, int nY)
{
	TRACE_IN

	x= TheWorld->bg_x -SPHERE_WIDTH/2 + nX;
	y= SCREEN_HEIGHT - SPHERE_HEIGHT/2 -nY-TheWorld->bg_y;

	//setposition((dReal) nX, (dReal)nY);
	TRACE_OUT
}

void Sphere::setvelocity(float xV, float yV)
{
	TRACE_IN
	if(!body)
		return;
	dBodySetLinearVel  (body, 0, xV, yV);
	TRACE_OUT
}

void Sphere::getvelocity(float *xV, float *yV)
{
	TRACE_IN
	if(!body)
		return;
	const dReal *pos = dBodyGetLinearVel(body);
	*xV = (float)pos[1];
	*yV = (float)pos[2];
	TRACE_OUT
}
void Sphere::set_force_on_sphere(float fx, float fy, float desX, float desY)
{
	TRACE_IN
	if(!body)
		return;
	dBodyAddForce(body,0, 10.0,10.0);
	TRACE_STRING("adding force ");
	TRACE_OUT
}

void Sphere::move()
{
	TRACE_IN

	TRACE_OUT
}


void GetRelDirectionToMouse(int x, int y,int *rel_x,int *rel_y)
{
	TRACE_IN
	int m_x=0,m_y=0;
	SDL_GetMouseState(&m_x, &m_y);
	if(m_x>(x+SPHERE_WIDTH/2))  *rel_x=2; else if(m_x<(x+SPHERE_WIDTH/2))  *rel_x=-2;
	if(m_y<(y+SPHERE_HEIGHT/2)) *rel_y=-2; else if(m_y>(y+SPHERE_HEIGHT/2)) *rel_y=8;
	TRACE_OUT
}
void Sphere::show()
{
	TRACE_IN
	int rel_x=0, rel_y=0;
	const dReal *pos, *pos_link2;
	dReal dx,dy;
	int x1,y1,x2,y2;
	int i=0;
	dBodyID tmpBody = 0;

	if(!geom || !body)
		return;

	PTBlist pB = (PTBlist)dGeomGetData( geom);

	if(!pB->bBeingMoved)
	{
		pos = dBodyGetPosition ( body );
		dx=pos[1];
		dy=pos[2];

		Uint32 c_gray = SDL_MapRGB(TheWorld->screen->format, 0,102,102);

		init_center(int(dx), (int)dy);
		setposition(dx,dy);

		// make the sphere's eyes look at mouse cursor
		GetRelDirectionToMouse(x,y,&rel_x,&rel_y);

		// SDL Drawing coordinates
		x1=x+SPHERE_WIDTH/2; 
		y1=y+SPHERE_HEIGHT/2;  

		// Draw the joints 
		for(i=0;i<pB->nJoints;i++)
		{		
			tmpBody=dJointGetBody (pB->joints[i], 1);

			if(tmpBody && (tmpBody!=body))
			{			
				pos_link2 = dBodyGetPosition(tmpBody); // get center of the linked body
				x2= (int) pos_link2[1]+TheWorld->bg_x;  // update it for world view

				y2= SCREEN_HEIGHT  -(int)pos_link2[2]-TheWorld->bg_y;
	            
				// ***************************************************
				// **  Draw the links 
				// ***************************************************
				DrawImageFromLinePoints(link1, x1, y1, x2, y2);	
				UpdateBodyJointForces(pB, i);
				RecordHighestJointForces(pB,tmpBody,i);
				//ZeroJointFeedBack(pB->joints[i], pB->JFB);
			}	
		}
		// check is cursor is close to body, if it is reset frame_count
		if(is_close(TheWorld->event.button.x,TheWorld->event.button.y,x,y,100))
			frame_count = 10;
		else if(frame_count >0)
			frame_count--;
	}
	else // sphere is being moved just draw it
	{
		// center of the body on the mouse cursor
		x =	TheWorld->event.button.x - SPHERE_WIDTH/2;			
		y = TheWorld->event.button.y - SPHERE_HEIGHT/2 ;		
	}

	switch (Type)
	{
		case REGULAR:
			if(awake || frame_count>0)
			{
				if(!pB->bBeingMoved)
				{
					int img = get_next_surface();
					apply_surface( x+(SPHERE_WIDTH/2 - (BallArray[img]->w/2)), y+(SPHERE_WIDTH/2 - BallArray[img]->h/2), BallArray[img] , TheWorld->screen );
					int shadow = get_next_shadow();
					apply_surface( x+(sphere_shadow->w/2 - (BallShadowArray[shadow]->w/2)), y+(sphere_shadow->w/2 - BallShadowArray[shadow]->h/2), BallShadowArray[shadow] , TheWorld->screen );
				}
				else
					apply_surface( x, y, sphere , TheWorld->screen );
				apply_surface( x+4, y+6, eye_l , TheWorld->screen );
				apply_surface( x+32, y+6, eye_r , TheWorld->screen );
				apply_surface( x+15+rel_x, y+12+rel_y, eye_pupil,TheWorld->screen );
				apply_surface( x+40+rel_x, y+12+rel_y, eye_pupil,TheWorld->screen );
        
			}
			else
			{
				apply_surface( x, y, sphere , TheWorld->screen );
				apply_surface( x+4, y+6, eye_l , TheWorld->screen );
				apply_surface( x+32, y+6, eye_r , TheWorld->screen );
				apply_surface( x+15, y+18, eye_sleep_l , TheWorld->screen );
				apply_surface( x+40, y+18, eye_sleep_r , TheWorld->screen );

			}
			break;
		case RED:
			{
				dReal booth_pos[3];
				booth_pos[0] =  TheWorld->EngineerPosition[0];
				booth_pos[1] =	TheWorld->EngineerPosition[1] ;
				booth_pos[2] =  TheWorld->EngineerPosition[2] ;

				double dForceFactor = -1* TheWorld->world_gravity *100.0f;
				double dist = sqrt( pow((pos[1]-booth_pos[1]),2)+pow((pos[2]-booth_pos[2]),2));
				dBodyAddForce(body,0,dForceFactor * (booth_pos[1]-pos[1]),dForceFactor* (booth_pos[2]-pos[2]));
				apply_surface( x, y, sphere , TheWorld->screen );
				apply_surface( x+4, y+22, eye_l , TheWorld->screen );
				apply_surface( x+32, y+22, eye_r , TheWorld->screen );
				apply_surface( x+15+rel_x, y+28+rel_y, eye_pupil,TheWorld->screen );
				apply_surface( x+40+rel_x, y+28+rel_y, eye_pupil,TheWorld->screen );
			}
			break;
		default:
			break;
	}

	TRACE_OUT
}

void Sphere::stop()
{
	TRACE_IN

	TRACE_OUT
}

void Sphere::handle_input(int nSphereCount)
{
	TRACE_IN
	if(!geom || !body)
		return;

    PTBlist thisp = (PTBlist)dGeomGetData( geom);
	if(TheWorld->MouseLocked && !thisp->bBeingMoved)
		return;
	
	if(is_under_mouse() && (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1)))
	{
		dGeomID o1=0,o2=0;
		DrawTo2Closest(geom, TheWorld->SearchRadius, o1, o2);
	}

	if( TheWorld->event.type == SDL_MOUSEBUTTONUP )
	{
		const dReal *mypos,*pos1, *pos2;
		dBodyEnable (body);
		
		if(is_under_mouse() && thisp->bBeingMoved )
		{
			//dBodySetForce(body,0.0,(dReal) momentum_x, (dReal) momentum_y);
			dGeomID Connect2Geom1 =0;
			dGeomID Connect2Geom2 =0;
			DrawTo2Closest(geom, TheWorld->SearchRadius, Connect2Geom1, Connect2Geom2);

			if((Connect2Geom1 != 0) && (Connect2Geom2 != 0))
			{
				if( (GetTime() - dAttachCoolDown)> 1)
				{
					y = TheWorld->event.button.y - SPHERE_HEIGHT/2 + TheWorld->bg_y;
					x =	TheWorld->event.button.x - TheWorld->bg_x;
					dGeomSetPosition (geom,(dReal)OBJECT_DEPTH,(dReal)x, (dReal) ( SCREEN_HEIGHT - SPHERE_HEIGHT/2 - y));

					mypos = dGeomGetPosition(geom);					
					pos1 = dGeomGetPosition(Connect2Geom1);
					pos2 = dGeomGetPosition(Connect2Geom2);	

					// Check that none of the 2 bodies to be connected are too close to this body
					if( !IsWithinDistance(mypos,pos1,(double)SPHERE_WIDTH)  &&
						!IsWithinDistance(mypos,pos2,(double)SPHERE_WIDTH) 	)
					{
						dJointID joint=0;
						dJointFeedback *jfb = new dJointFeedback;

						if(Connect2Geom1)
						{
							
							dGeomSetPosition(Connect2Geom1,OBJECT_DEPTH,pos1[1],pos1[2]  );
							joint=HingeAttach2Bodies(thisp, (PTBlist)dGeomGetData (Connect2Geom1));

							if(joint)
								dJointSetHingeAnchor (joint, OBJECT_DEPTH,mypos[1],mypos[2]);
						}

						if(Connect2Geom2)
						{
							
							dGeomSetPosition(Connect2Geom2,OBJECT_DEPTH,pos2[1],pos2[2] );
							joint = HingeAttach2Bodies(thisp, (PTBlist)dGeomGetData (Connect2Geom2));

							if(joint)
								dJointSetHingeAnchor (joint, OBJECT_DEPTH,mypos[1],mypos[2]);
						}
						dAttachCoolDown = GetTime();
						delete jfb;
					}			

				}			

				thisp->bBeingMoved = false;
				TheWorld->MouseLocked = false;					
			}
			
		}
		if(thisp->bBeingMoved)
		{
			dBodyEnable (body);
			thisp->bBeingMoved = false;
			TheWorld->MouseLocked = false;

		}
	}
	else if( TheWorld->event.type == SDL_MOUSEBUTTONDOWN )
	{
		dGeomID o1=0,o2=0;

		if(is_under_mouse())
		{
			dBodyDisable (body);

			if(!awake)
			{
				Mix_PlayChannel( -1, high, 0 );

				awake = true;			
			}
			
			// Update to the new position -- remember to account for background 
			// view shift in bg_y and bg_x	
			y = TheWorld->event.button.y - SPHERE_HEIGHT/2 + TheWorld->bg_y;
			x =	TheWorld->event.button.x - TheWorld->bg_x;

			dGeomSetPosition (geom,double(OBJECT_DEPTH),double(x), double( SCREEN_HEIGHT - TheWorld->event.button.y - TheWorld->bg_y));
			dBodySetLinearVel  (body, 0, 0, 0);

			thisp->bBeingMoved = true;
			DrawTo2Closest(geom, TheWorld->SearchRadius, o1, o2);
			TheWorld->MouseLocked = true;

		}
	}
	else if( TheWorld->event.type == SDL_MOUSEMOTION )
	{
		dGeomID o1=0,o2=0;

		if(awake && (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1))) 
		{
			if(thisp->bBeingMoved)
			{
				dBodyDisable (body);

				int new_y=TheWorld->event.button.y - SPHERE_HEIGHT/2 + TheWorld->bg_y;
				int new_x=TheWorld->event.button.x - SPHERE_WIDTH/2  - TheWorld->bg_x;

				momentum_x = ( - x	+ new_x		+ TheWorld->bg_x )*weight()*100;
				momentum_y = (   y	- new_y		+ TheWorld->bg_y )*weight()*100;
			
				y = new_y ;
				x = new_x + SPHERE_WIDTH/2;
				dGeomSetPosition (geom,double(OBJECT_DEPTH),double(x) , double(SCREEN_HEIGHT -TheWorld->event.button.y - TheWorld->bg_y));

				dBodySetLinearVel  (body, 0, 0, 0);
				DrawTo2Closest(geom, TheWorld->SearchRadius, o1, o2);
			} 
		}
	}
	
	// Call default input handler
	SDL_World::handle_input();
	TRACE_OUT
}


bool Sphere::load_images()
{
	TRACE_IN

	if(!load_files(".\\images\\ball.png", &sphere))
	{
		TRACE_OUT
		return false;
	}
	
	if(!load_files(".\\images\\ball_shadow2.png", &sphere_shadow))
	{
		TRACE_OUT
		return false;
	}

	if(!load_files(".\\images\\eye_fem_l1.png", &eye_l))
	{
		TRACE_OUT
		return false;
	}

	if(!load_files(".\\images\\eye_fem_r1.png", &eye_r))
	{
		TRACE_OUT
		return false;
	}

	if(!load_files(".\\images\\eye_sleep_left1.png", &eye_sleep_l))
	{
		TRACE_OUT
		return false;
	}

	if(!load_files(".\\images\\eye_sleep_right1.png", &eye_sleep_r))
	{
		TRACE_OUT
		return false;
	}

	if(!load_files(".\\images\\pupil1.png", &eye_pupil))
	{
		TRACE_OUT
		return false;
	}

	if(!load_files(".\\images\\link.png", &link1))
	{
		TRACE_OUT
		return false;
	}


	for (int i=0;i<suf_images;i++)
	{
		BallArray[i]= rotozoomSurfaceXY(sphere, 0, (100.0+(2*i))/100.0,(100.0-(2*i))/100.0,1);

	}
	for (int i=0;i<shadow_images;i++)
	{
		BallShadowArray[i]= rotozoomSurface(sphere_shadow, 0, (100.0-(4*(shadow_images-i-1)))/100.0,1);
	}
    
	TRACE_OUT
	return true;
	
}
bool Sphere::load_imagesHHRed()
{
	TRACE_IN

	if(!load_files(".\\images\\hhredball2.png", &sphere))
	{
		TRACE_OUT
		return false;
	}

	if(!load_files(".\\images\\ball_shadow2.png", &sphere_shadow))
	{
		TRACE_OUT
		return false;
	}

	if(!load_files(".\\images\\eye_fem_l1.png", &eye_l))
	{
		TRACE_OUT
		return false;
	}

	if(!load_files(".\\images\\eye_fem_r1.png", &eye_r))
	{
		TRACE_OUT
		return false;
	}

	if(!load_files(".\\images\\eye_sleep_left1.png", &eye_sleep_l))
	{
		TRACE_OUT
		return false;
	}

	if(!load_files(".\\images\\eye_sleep_right1.png", &eye_sleep_r))
	{
		TRACE_OUT
		return false;
	}

	if(!load_files(".\\images\\pupil1.png", &eye_pupil))
	{
		TRACE_OUT
		return false;
	}

    
	TRACE_OUT
	return true;
	
}
bool  Sphere::load_sound(void)
{
	TRACE_IN
	int i =0;

	//Load the sound effects
	scratch = Mix_LoadWAV( ".\\sounds\\scratch.wav" );
	high = Mix_LoadWAV( ".\\sounds\\high.wav" );
	med = Mix_LoadWAV( ".\\sounds\\medium.wav" );
	low = Mix_LoadWAV( ".\\sounds\\low.wav" );

	iluvit = Mix_LoadWAV( ".\\sounds\\iluvit.wav" );
	wow	= Mix_LoadWAV( ".\\sounds\\wow.wav" );
	mad_sounds[0]=Mix_LoadWAV( ".\\sounds\\noroom.wav" );
	mad_sounds[1]=Mix_LoadWAV( ".\\sounds\\uba.wav" );
	mad_sounds[2]=Mix_LoadWAV( ".\\sounds\\whaudon.wav" );
	mad_sounds[3]=Mix_LoadWAV( ".\\sounds\\getoff.wav" );
	mad_sounds[4]=Mix_LoadWAV( ".\\sounds\\ocrap.wav" );
 
	//If there was a problem loading the sound effects
	if( ( high == NULL ) || ( med == NULL ) || ( low == NULL ) )
	{
		TRACE_OUT
		return false;    
	}

   
	if(  wow == NULL  )
	{
		TRACE_OUT
		return false;    
	}

	if(  iluvit == NULL  )
	{
		TRACE_OUT
		return false;    
	}
    
	for(i=0;i<5;i++)
	{
		if( mad_sounds[i] == NULL )
		{
			TRACE_OUT
			return false;    
		}
	}

	TRACE_OUT
	return true;
}

