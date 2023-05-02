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

#include "walker.h"
#include <SDL_draw.h>

using namespace std;


Walker::Walker()
{
	TRACE_IN

	//Initialize the offsets
	x = 0;
	y = 0;

	walker = NULL;

	eye_l = NULL;
	eye_r = NULL;
	eye_sleep_l = NULL;
	eye_sleep_r = NULL;
	eye_pupil = NULL;
	wow = NULL;
	cheers = NULL;

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
	


	geom = dCreateSphere (TheWorld->space,(dReal)WALKER_WIDTH/2);	
	p=NULL;

	body =0;
	ship_body =0;

	nBodyNum_iam_on = -1 ;
	nBodyNum_next_geom_to_go = -1;
	on_the_ship = false;

	dGeomSetCategoryBits (geom, WALKER_CATEGORY);
	dGeomDisable (geom);
	nBodyNumber =0;
	disable_motion = false;
	// Load image and sound files
	load_images(); 
	load_sound();


	TRACE_STRING ("Created walker. Body #%d",nBodyNumber);
	TRACE_OUT
}

Walker::~Walker()
{
	TRACE_IN
;
	TRACE_OUT
}

void Walker::CleanUp()
{
	if(p)
		free(p);

	if(walker)
		SDL_FreeSurface(walker);

	if(eye_l)
		SDL_FreeSurface(eye_l);

	if(eye_r)
		SDL_FreeSurface(eye_r);

	if(eye_sleep_l)
		SDL_FreeSurface(eye_sleep_l);

	if(eye_sleep_r)
		SDL_FreeSurface(eye_sleep_r);

	if(eye_pupil)
		SDL_FreeSurface(eye_pupil);
	
	if(wow)
		Mix_FreeChunk(wow);

		
	if(cheers)
		Mix_FreeChunk(cheers);

}
void Walker::setposition(dReal X, dReal Y)
{
	TRACE_IN
	dGeomSetPosition (geom,OBJECT_DEPTH,X, Y);
	TRACE_OUT;
}

void Walker::init_center(int nX, int nY)
{
	TRACE_IN
	x= nX-WALKER_WIDTH/2+TheWorld->bg_x;
	y= SCREEN_HEIGHT - WALKER_HEIGHT/2 -nY-TheWorld->bg_y;
	setposition((dReal) nX, (dReal)nY);
	TRACE_OUT
}

void Walker::setvelocity(float xV, float yV)
{
	TRACE_IN
	
	TRACE_OUT
}

void Walker::getvelocity(float *xV, float *yV)
{
	TRACE_IN

	TRACE_OUT
}
void Walker::set_force_on_walker(float fx, float fy, float desX, float desY)
{
	TRACE_IN

	TRACE_OUT
}

void Walker::move()
{
	
}



void Walker::show(const dReal *TargetPos)
{
	TRACE_IN
	const dReal *pos, *mypos;
	double new_x, new_y;
	double speed = 0, distance=0;
	dReal goto_pos[3];
	double dist = 0;


	Uint32 color = SDL_MapRGB(TheWorld->screen->format, 00,255,00);

	dGeomID near_geom = 0,next_geom_to_go=0,geom_iam_on=0;
	mypos = dGeomGetPosition(geom);	
	dist = DISTANCE(mypos,TargetPos);

	if(body==0)
	{
		if(nBodyNum_next_geom_to_go == -1)
		{
			const dReal *near_pos;
			near_geom = GetClosestGeom(geom, SHIP_CATEGORY, TheWorld->nJumpToShipDistance);
			if(near_geom)
			{
				near_pos=dGeomGetPosition(near_geom);
				if(fabs(near_pos[2]-mypos[2])>100.0)
					near_geom = 0;
			}
			if(!near_geom)
			{
				near_geom = GetClosestGeomWithJointExcluding(geom, HARDBALL_CATEGORY, TheWorld->world_width, geom_iam_on);			
			}
		}
		else 
		{
			PTBlist p = TheWorld->pBlist;
			PTBlist p2 = p;
			bool bFoundIt = false;
			while(p2)
			{
				if(p2->nBodyNum == nBodyNum_next_geom_to_go)
				{
					bFoundIt = true;
					break;
				}
				else
					p2 = p2->pNext;
			}

			if(bFoundIt && p2)
				near_geom = p2->geom;
			else
				near_geom = 0;
		}
	
		if(near_geom)
		{
			pos = dGeomGetPosition(near_geom);
			if(dGeomGetCategoryBits(near_geom)==SHIP_CATEGORY)
			{
				goto_pos [1] = pos[1];
				goto_pos [2] = pos[2]+30;
			}
			else
			{
				goto_pos [1] = pos[1];
				goto_pos [2] = pos[2];
			}

			//distance = sqrt(pow((pos[1]-mypos[1]),2)+pow((pos[2]-mypos[2]),2));
		}
		else
		{
			near_geom = GetClosestGeomWithJointExcluding(geom, HARDBALL_CATEGORY, TheWorld->world_width, geom_iam_on);
			if(near_geom)
			{
				pos = dGeomGetPosition(near_geom);
				goto_pos [1] = pos[1];
				goto_pos [2] = pos[2];

			}
			else
			{
				goto_pos [1] = rand() % TheWorld->world_width;
				goto_pos [2] = rand() % TheWorld->world_height;
			}

		}
		speed = 3;

		init_center(int(mypos[1]), int(mypos[2])); // updated x and y 
		new_x = mypos[1];
		new_y = mypos[2];	

		//dGeomSetPosition(geom, OBJECT_DEPTH, mypos[1], mypos[2] );

		if(goto_pos[1]>mypos[1])
			new_x  = mypos[1] +  speed;

		if(goto_pos[1]<mypos[1])
			new_x  = mypos[1] - speed;

		if(goto_pos[2]>mypos[2])
			new_y  = mypos[2] +  speed;

		if(goto_pos[2]<mypos[2])
			new_y  = mypos[2] - speed;
	
		if(near_geom && (dGeomGetCategoryBits(near_geom)==SHIP_CATEGORY) && 
			(fabs(goto_pos[1]-mypos[1])<=100) && (fabs(goto_pos[2]-mypos[2])<=100) )
		{
			ship_body = dGeomGetBody(near_geom);
			if(mypos[2] > goto_pos[2])
			{
				MakeBody();
				dBodySetForce(ship_body,0,0,0);
				dBodySetLinearVel(ship_body,0,0,0);
			}

		}
		else if( (fabs(goto_pos[1]-mypos[1])<=3) && (fabs(goto_pos[2]-mypos[2])<=3))
		{
			PTBlist pB1=NULL, pB2=NULL;
			geom_iam_on = near_geom;
			PTBlist pB_iam_on= (PTBlist) dGeomGetData(geom_iam_on);
			nBodyNum_iam_on = pB_iam_on->nBodyNum;

			if(geom_iam_on && !(dGeomGetCategoryBits(geom_iam_on)==SHIP_CATEGORY))
			{
				dBodyID body_iam_on = dGeomGetBody(geom_iam_on);
				pB1 = (PTBlist) dGeomGetData(geom_iam_on);
				dBodyID next_body_to_go=0;
				int nJ = dBodyGetNumJoints(body_iam_on);

				if(pB1 && nJ>0)
				{
					
					int nJ_pick = rand() % nJ ; // roll the dice
					int nBody_pick = rand() % 2; // toss a coin
					int chance1 = rand() % 100 + 1;
					dJointID jnt = 0;

					if(chance1<=70)
					{
						// Go through all joint for the body i am on 
						for (int i=0;i<nJ;i++)
						{	
							const dReal *tmppos1,*tmppos2;
							double tmp_dist1, tmp_dist2;

							jnt = dBodyGetJoint(body_iam_on,i);

							// choose between the 2 bodies of the jnt
							dBodyID b1 = dJointGetBody(jnt,0 );
							dBodyID b2 = dJointGetBody(jnt,1 );

							// 90% of the time pick the algorism that gets the walker closer to the chip
							// which has the TargetPos
							if(	chance1<=90) 
							{		  
								if(b1)
								{								
									tmppos1 = dBodyGetPosition(b1);
									tmp_dist1 = DISTANCE(tmppos1,TargetPos);
									if(tmp_dist1<dist)
									{
										dist = tmp_dist1;
										next_body_to_go = b1;
										pB2 = (PTBlist) dBodyGetData(b1);
										nBodyNum_next_geom_to_go = pB2->nBodyNum;
									}

								}
								if(b2) 
								{
									tmppos2 = dBodyGetPosition(b2);
									tmp_dist2 = DISTANCE(tmppos2,TargetPos);
									if(tmp_dist2<dist)
									{
										dist = tmp_dist2;
										next_body_to_go = b2;
										pB2 = (PTBlist) dBodyGetData(b2);
										nBodyNum_next_geom_to_go = pB2->nBodyNum;
									}
								}
							}
							else // 30 of the time .. do it randomly 
							{
								for(int i=0;i<(rand() % 5);i++)
								{
									nBody_pick = rand() % 2; // toss a coin

									if(nBody_pick ==0 && b1)
									{
										next_body_to_go = b1;
										pB2 = (PTBlist) dBodyGetData(b1);
										nBodyNum_next_geom_to_go = pB2->nBodyNum;
									}
									else if (nBody_pick ==1 && b2)
									{
										next_body_to_go = b2;
										pB2 = (PTBlist) dBodyGetData(b2);
										nBodyNum_next_geom_to_go = pB2->nBodyNum;
									}
								}					
							}
						} // end for loop
					}
					else // chance 50%
					{
						dBodyID b = 0;
						for(int i=0;i<(rand() % 5) && !b;i++)
						{
							nJ = dBodyGetNumJoints(body_iam_on);	
							nJ_pick = rand() % nJ ; // roll the dice
							nBody_pick = rand() % 2; // toss a coin
							jnt = dBodyGetJoint(body_iam_on,nJ_pick);

							// choose between the 2 bodies of the jnt
							b = dJointGetBody(jnt,1);
							if(!b)
								b = dJointGetBody(jnt,0);
							if(b)
							{
								next_body_to_go = b;
								pB2 = (PTBlist) dBodyGetData(b);
								nBodyNum_next_geom_to_go = pB2->nBodyNum;
							}
						}

						if(!b)
							nBodyNum_next_geom_to_go = -1;

					}


					if(nBodyNum_next_geom_to_go == -1)
					{
						const dReal *near_pos;

						//Search for a ship to go to with TheWorld->nJumpToShipDistance mm
						near_geom = GetClosestGeom(geom, SHIP_CATEGORY, TheWorld->nJumpToShipDistance);

						//if found it go to it and it is within a close vertical distance 
						// go to it, else stay on the current course
						if(near_geom)
						{
							near_pos=dGeomGetPosition(near_geom);
							if(fabs(near_pos[2]-mypos[2])<100.0)
							{
								next_body_to_go = dGeomGetBody(near_geom);	
								pB2 = (PTBlist) dBodyGetData(next_body_to_go);
								nBodyNum_next_geom_to_go = pB2->nBodyNum;
							}
						}
						else
						{
							next_body_to_go = dGeomGetBody(GetClosestGeomWithJointExcluding(geom, HARDBALL_CATEGORY, TheWorld->world_width, geom_iam_on));
							pB2 = (PTBlist) dBodyGetData(next_body_to_go);
							nBodyNum_next_geom_to_go = pB2->nBodyNum;
						}
					}

				}
				else
				{
					cout<<"stuck because the body I am on has no joints"<<ends;
				}



			}

			dGeomID ship_geom = GetClosestGeom(geom, SHIP_CATEGORY, TheWorld->nJumpToShipDistance);
			if(ship_geom)
			{
				PTBlist pBShip = (PTBlist) dBodyGetData(dGeomGetBody(ship_geom));
				nBodyNum_next_geom_to_go = pBShip->nBodyNum;
			}

		}


		init_center(int(new_x), int(new_y)); // updated x and y 
		apply_surface( x,  y, walker , TheWorld->screen );	
		//int x2= (int) goto_pos[1]+TheWorld->bg_x;  // update it for world view
		//int y2= SCREEN_HEIGHT  -(int)goto_pos[2]-TheWorld->bg_y;
		//sDrawLine(x+WALKER_WIDTH/2,y+WALKER_HEIGHT/2,x2,y2,color);
	}
	else  // walkers has a body
	{
		int rel_x=0, rel_y=0;
		const dReal *pos,*ship_pos;

		dBodySetAngularVel (body, 0, 0, 0);
		pos = dBodyGetPosition ( body );
		
		ship_pos = dBodyGetPosition(ship_body);
		if(pos[2]>ship_pos[2])
		{
			double dForce =  -1* TheWorld->world_gravity *1.50f;
			double dist = sqrt( pow((pos[1]-ship_pos[1]),2)+pow((pos[2]-ship_pos[2]),2));
			dBodyAddForce(body,0,dForce * (ship_pos[1]-pos[1]),dForce* (ship_pos[2]-pos[2]));
			on_the_ship = true;
		}
		else
		{
			on_the_ship = false;
		}
		init_center(int(pos[1]), int(pos[2]));
		GetRelDirectionToMouse(x,y,&rel_x,&rel_y);
		apply_surface( x, y, walker , TheWorld->screen );
		apply_surface( x+4, y+6, eye_l , TheWorld->screen );
		apply_surface( x+32, y+6, eye_r , TheWorld->screen );
		apply_surface( x+15+rel_x, y+12+rel_y, eye_pupil,TheWorld->screen );
		apply_surface( x+40+rel_x, y+12+rel_y, eye_pupil,TheWorld->screen );
	}	
	



	

	TRACE_OUT
}

void Walker::stop()
{
	TRACE_IN
	TRACE_OUT
}
bool Walker::is_under_mouse(int X, int Y)
{
	TRACE_IN
	int topY, botY, leftX, rightX;
   
 
	leftX = x ;
	rightX = x + WALKER_WIDTH;
	topY = y;
	botY = y+WALKER_HEIGHT;

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
	TRACE_OUT
}
void Walker::handle_input(int nBaloonCount)
{
	TRACE_IN
    
	//If a key was released
	if( TheWorld->event.type == SDL_KEYDOWN )
	{
	}
	else if( TheWorld->event.type == SDL_KEYUP )
	{

	}
	else if( TheWorld->event.type == SDL_MOUSEBUTTONUP )
	{

	}
	else if( TheWorld->event.type == SDL_MOUSEBUTTONDOWN )
	{
			

	}
	else if( TheWorld->event.type == SDL_MOUSEMOTION && (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1)))
	{
		

	}
	TRACE_OUT
}


bool Walker::load_images()
{
	
	TRACE_IN
	load_files( ".\\images\\child1.png", &walker);
	  
	//If there was a problem in loading the walkert
	if( walker == NULL )
	{
		return false;
		cout<<"Walker failed to load"<<endl;
	}

	TRACE_OUT
	return true;
	
}
bool  Walker::load_sound(void)
{
	TRACE_IN    

	TRACE_OUT
	return true;
}
void Walker::MakeBody(void)
{
	// Construct the ODE Baloon Object 
	body = dBodyCreate (TheWorld->world);
    //dMassSetZero (&mass);

	density = TheWorld->sphere_density;

    dMassSetSphere (&mass,(dReal)density , WALKER_WIDTH/2);
    dBodySetMass (body, &mass);

	geom = dCreateSphere (TheWorld->space,(dReal)WALKER_WIDTH/2);	
	p=NULL;
	
	dGeomSetBody (geom,body);
	dGeomSetCategoryBits (geom, WALKER_CATEGORY);
	WalkerContacts = dJointGroupCreate (0);	
	
	wow	= Mix_LoadWAV( ".\\sounds\\wow.wav" );
	Mix_PlayChannel( -1, wow, 0 );


	load_files( ".\\images\\yellowball.png", &walker);
	  
	//If there was a problem in loading the walkert
	if( walker == NULL )
	{
		cout<<"Walker failed to load"<<endl;

	}

	if(!load_files(".\\images\\eye_fem_l1.png", &eye_l))
	{
		TRACE_OUT
		return ;
	}

	if(!load_files(".\\images\\eye_fem_r1.png", &eye_r))
	{
		TRACE_OUT
		return ;
	}

	if(!load_files(".\\images\\eye_sleep_left1.png", &eye_sleep_l))
	{
		TRACE_OUT
		return ;
	}

	if(!load_files(".\\images\\eye_sleep_right1.png", &eye_sleep_r))
	{
		TRACE_OUT
		return ;
	}

	if(!load_files(".\\images\\pupil1.png", &eye_pupil))
	{
		TRACE_OUT
		return ;
	}
	GameResults[nCurrentGame].nBallsSaved++;

	if(GameResults[nCurrentGame].nBallsSaved==TheWorld->nWinScore)
	{
		Mix_FadeOutChannel(-1, 500);
		cheers = Mix_LoadWAV( ".\\sounds\\ccheer.wav" );
		Mix_PlayChannel( -1, cheers , 0 );
	}
}
