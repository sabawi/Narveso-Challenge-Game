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

#pragma once

class StaticObj : public SDL_World , public MyWorld
{
	int x,y;
	double init_x, init_y;

public:
	StaticObj(void);
	~StaticObj(void);
	dGeomID geom;
	dBodyID body;
	dMass mass;
	enum STATIC_OBJECT {CLOUD,SHIP, PLANE,BIRD,BOX,INVISIBLE} StaticObject;
	SDL_Surface *object ;
	SDL_Surface *object2 ;
	STATIC_OBJECT ObjectType;
	double rot_theta ;
	PTBlist p;
	unsigned long  nBodyNumber ;
	dJointGroupID ObjContacts;

	void CleanUp()
	{

		if(ObjContacts)
		{
			dJointGroupEmpty (ObjContacts);
			dJointGroupDestroy (ObjContacts);
		}

		if(object)
			SDL_FreeSurface(object);

		if(object2)
			SDL_FreeSurface(object2);

		if(geom)
		{
	
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
	}
	void setBoxJointsPositions()
	{
		const dReal *mypos;
		double dist_x, dist_y, delta_x, delta_y;
		dReal xrot,yrot,zrot;

		PTBlist pB = (PTBlist) dBodyGetData(body);

		mypos = dBodyGetPosition(body);
		get_euler(dBodyGetRotation(body),xrot,yrot,zrot);		

		rot_theta = xrot;

		// left joint 0
		pB->BoxJoints[0].BoxJointPos[0]= mypos[0];
		dist_x = - BOX_WIDTH/2 +  BOX_WIDTH * 1/6 ; // 1/6 of body width from the left edge
		dist_y = pB->BoxJoints[0].BoxJointPos[2] = 0;

		delta_x=(dist_x*cos(-xrot))-(dist_y* sin(-xrot));
		delta_y=(dist_x*sin(-xrot))+(dist_y* cos(-xrot));

		pB->BoxJoints[0].BoxJointPos[1] = mypos[1] + delta_x;
		pB->BoxJoints[0].BoxJointPos[2] = mypos[2] + delta_y; 

		// center join 1
		pB->BoxJoints[1].BoxJointPos[0] = mypos[0];
		pB->BoxJoints[1].BoxJointPos[1] = mypos[1];
		pB->BoxJoints[1].BoxJointPos[2] = mypos[2];

		// right joint 2
		pB->BoxJoints[2].BoxJointPos[0] = mypos[0];

		dist_x = BOX_WIDTH/2 -  BOX_WIDTH * 1/6 ; // 1/6 of body width from the right edge 
		dist_y = pB->BoxJoints[2].BoxJointPos[2] = 0;

		delta_x=(dist_x*cos(-xrot))-(dist_y* sin(-xrot));
		delta_y=(dist_x*sin(-xrot))+(dist_y* cos(-xrot));
		
		pB->BoxJoints[2].BoxJointPos[1] = mypos[1] + delta_x ;
		pB->BoxJoints[2].BoxJointPos[2] = mypos[2] + delta_y ;

		pB->BoxJoints[0].isUsed = false;
		pB->BoxJoints[1].isUsed = false;
		pB->BoxJoints[2].isUsed = false;


	}

	void setposition(double x, double y)
	{
		TRACE_IN
		dGeomSetPosition(geom, OBJECT_DEPTH, x, y);
		TRACE_OUT
	}

	void init_center(int nX, int nY)
	{
		TRACE_IN
		x= nX-object->w/2+TheWorld->bg_x;
		y= SCREEN_HEIGHT - object->h/2 -nY-TheWorld->bg_y;
		TRACE_OUT
	}

	void Box_init_center(int nX, int nY)
	{
		TRACE_IN
		x= nX-BOX_WIDTH/2+TheWorld->bg_x;
		y= SCREEN_HEIGHT - BOX_HEIGHT/2 -nY-TheWorld->bg_y;
		TRACE_OUT
	}

	bool load_images(STATIC_OBJECT StaticObject)
	{
		TRACE_IN
		bool ret = false;

		switch (StaticObject)
		{
		case CLOUD:
			ret = SDL_World ::load_images(".\\images\\cloud500x100.png",&object );
			break;
		case SHIP:
			ret = SDL_World ::load_images(".\\images\\ship1.png",&object );
			if(ret) ret = SDL_World ::load_images(".\\images\\ship2.png",&object2 );
			break;

		case PLANE:
			break;
		case BIRD:
			break;
		case BOX:
			ret = SDL_World ::load_images( ".\\images\\blue.bmp", &object);
			break;
		case INVISIBLE:
			ret = SDL_World ::load_images( ".\\images\\blue.bmp", &object);
			break;
		}

		return ret;
		TRACE_OUT
	}

	void show()
	{
		TRACE_IN
		const dReal *pos;
		dReal xrot,yrot,zrot;
		int x1, x2, y1, y2;
		double center[2], line[4];
		pos = dGeomGetPosition ( geom );
		init_center(int(pos[1]),int(pos[2]));
		const dReal * forces, *vel;
		//sDrawLine(x,y,x+cloud->w,y,SDL_MapRGB(TheWorld->screen->format, 255,104,238));
		apply_surface( x, y, object , TheWorld->screen );

		switch (ObjectType)
		{
		case CLOUD:
			break;
		case INVISIBLE:
			sDrawLine2( x, y, x+200, y, 255, 255, 255, 255);
			break;
		case SHIP:	
			setposition(pos[1],pos[2]);
			rotate_body_x(body, rot_theta);
			reset_YZaccess_drift(body);
			get_euler(dBodyGetRotation(body),xrot,yrot,zrot);

			rot_theta = xrot;
			apply_surface( x, y-object2->h+1, object2 , TheWorld->screen );
			forces = dBodyGetForce(body);
			
			//if(forces[2]>1)
				//std::cout<<"xForce = "<<forces[1]<<" yForce = "<<forces[2]<<std::endl;
			switch(nCurrentGame)
			{
				case GAME1:
				case GAME2:
				case GAME3:
					//std::cout<<"Saved Balls "<<GameResults[nCurrentGame].nBallsSaved<<std::endl;
					if(GameResults[nCurrentGame].nBallsSaved>=TheWorld->nWinScore )
					{
						PTBlist pB = (PTBlist) dGeomGetData(geom);
						bool bEndingConditions = true;

						// Check ending conditions
						if(dBodyGetNumJoints(dGeomGetBody(geom))>=TheWorld->nWinBalloons)
						{
							//for(int i=0;i<pB->nJoints || bEndingConditions == false;i++)
							for(int i=0;i<dBodyGetNumJoints(dGeomGetBody(geom)) ;i++)
							{
								dBodyID connect_body = dJointGetBody(dBodyGetJoint (body, i),0);
								if(connect_body)
								{
									PTBlist pB1 = (PTBlist) dBodyGetData(connect_body);
									if(dGeomGetCategoryBits(pB1->geom) != BALOON_CATEGORY)	
										bEndingConditions = false;
								}

							}
						}
						else
							bEndingConditions = false;

						// Move on to ending stage 1 with music
						if(bEndingConditions == true && GameResults[nCurrentGame].nGameStage==0)
						{
							stop_bg_music();
							Load_bg_music(".\\sounds\\bg_run.ogg");
							play_bg_music();
							GameResults[nCurrentGame].nGameStage = 1;
							GameResults[nCurrentGame].end_time = GetTime();
						}

						if(bEndingConditions == true && GameResults[nCurrentGame].nGameStage>0)
						{
							vel = dBodyGetLinearVel(body);
							//Lift up is the next stage
							if(GameResults[nCurrentGame].nGameStage==1 && (vel[2]>TheWorld->dWinXVelocity )) // vel in positive y direction
								GameResults[nCurrentGame].nGameStage = 2;

							// final stage is take off horizontally 
							if(GameResults[nCurrentGame].nGameStage==2)
							{								
								const dReal *pos;
								pos = dGeomGetPosition(geom);
								dBodySetLinearVel(body,0,TheWorld->EscapeVelocity,vel[2]);										
								// if the left tip of the ship is in the 
								// positive x zone (its x position > 0) give it force to push it left
								if(pos[1]-object->w>50)
									dBodySetLinearVel(body,0,TheWorld->EscapeVelocity,vel[2]);
								else
									GameResults[nCurrentGame].bGameOver = true;
							}
						}
					}
					else
					{
						if(GameResults[nCurrentGame].nGameStage>0)
						{
							// we have regresses to lower stage becuase ending conditions changed
							// reset stage
							stop_bg_music();
							Load_bg_music(".\\sounds\\bg_music.ogg");
							play_bg_music();
							GameResults[nCurrentGame].nGameStage=0;
							GameResults[nCurrentGame].end_time = 0;
						}
					}
					break;
				case 999:  // these cases to be deleted
				case 1000:
					//std::cout<<"Saved Balls "<<GameResults[nCurrentGame].nBallsSaved<<std::endl;
					if(GameResults[nCurrentGame].nBallsSaved>=TheWorld->nWinScore )
					{
						PTBlist pB = (PTBlist) dGeomGetData(geom);
						bool bEndingConditions = true;

						// Check ending conditions
						// 1- Basket not tied down by a structure
						// 2- At least one Ballon is connected to basket
						if(dBodyGetNumJoints(dGeomGetBody(geom))>=TheWorld->nWinBalloons)
						{
							//for(int i=0;i<pB->nJoints || bEndingConditions == false;i++)
							for(int i=0;i<dBodyGetNumJoints(dGeomGetBody(geom)) ;i++)
							{
								dBodyID connect_body = dJointGetBody(dBodyGetJoint (body, i),0);
								if(connect_body)
								{
									PTBlist pB1 = (PTBlist) dBodyGetData(connect_body);
									if(dGeomGetCategoryBits(pB1->geom) != BALOON_CATEGORY)	
										bEndingConditions = false;
								}
							}
						}
						else
							bEndingConditions = false;

						// Move on to ending stage 1 with music
						if(GameResults[nCurrentGame].nGameStage==0)
						{
							if(bEndingConditions == true)
							{
								stop_bg_music();
								Load_bg_music(".\\sounds\\bg_run.ogg");
								play_bg_music();
								GameResults[nCurrentGame].nGameStage++;
								GameResults[nCurrentGame].end_time = GetTime();
							}
						}
						else if(GameResults[nCurrentGame].nGameStage>0)
						{
							if(bEndingConditions == true)
							{
								vel = dBodyGetLinearVel(body);
								//std::cout<<"Ship Vel "<<vel[2]<<std::endl;
								if(vel[2]>TheWorld->dWinXVelocity ) // vel in positive y direction
								{	
									const dReal *pos;
									pos = dGeomGetPosition(geom);
							
									// if the left tip of the ship is in the 
									// positive x zone (its x position > 0) give it force to push it left
									//if(pos[1]-object->w/2<object->w)
									if(pos[1]-object->w>50)
									{
										double dPropForce = -1* TheWorld->world_gravity * 3.0f;
										dBodySetLinearVel(body,0,TheWorld->EscapeVelocity,vel[2]);
									}
									else
										GameResults[nCurrentGame].bGameOver = true;
								}
							}
							else
							{
								// we have regresses to lower stage becuase ending conditions changed
								// reset stage
								stop_bg_music();
								Load_bg_music(".\\sounds\\bg_music.ogg");
								play_bg_music();
								GameResults[nCurrentGame].nGameStage=0;
								GameResults[nCurrentGame].end_time = 0;

							}
						}
					}
					else
					{
						if(GameResults[nCurrentGame].nGameStage>0)
						{
							// we have regresses to lower stage becuase ending conditions changed
							// reset stage
							stop_bg_music();
							Load_bg_music(".\\sounds\\bg_music.ogg");
							play_bg_music();
							GameResults[nCurrentGame].nGameStage=0;
							GameResults[nCurrentGame].end_time = 0;
						}
					}
					break;

			}
			
			break;
		case PLANE:
			break;
		case BIRD:
			break;
		case BOX:
			PTBlist pB = (PTBlist) dBodyGetData(body);
			Uint32 color = SDL_MapRGB(TheWorld->screen->format, 255,104,238);
			setposition(pos[1],pos[2]);
			Box_init_center(int(pos[1]),int(pos[2]));
			short s[4] ;
			short t[4] ;
			int x,y;
			//rotate_body_x(body, rot_theta);
			reset_YZaccess_drift(body);
			get_euler(dBodyGetRotation(body),xrot,yrot,zrot);
			
			rot_theta =xrot ;
			
			rotate_body_x(body,rot_theta);
			//apply_surface( x, y-object2->h+1, object2 , TheWorld->screen );
			//forces = dBodyGetForce(body);
			//vel = dBodyGetLinearVel(body);
			center[0] = (double) pos[1]+TheWorld->bg_x;
			center[1] = (double) SCREEN_HEIGHT - pos[2]-TheWorld->bg_y;


			// draw bottom
			x1 = int(center[0] - BOX_WIDTH/2) ;
			y1 = int(center[1] + BOX_HEIGHT/2) ;
			x2 = int(center[0] + BOX_WIDTH/2) ;
			y2 = int(center[1] + BOX_HEIGHT/2);
			line[0]=x1;
			line[1]=y1;
			line[2]=x2;
			line[3]=y2;

			RotateLine2 ( center, line ,rot_theta, color);
			s[0] = (short)line[0] ;
			t[0] = (short)line[1] ;



			// draw right
			x1 = int(center[0] + BOX_WIDTH/2) ;
			y1 = int(center[1] - BOX_HEIGHT/2) ;
			x2 = int(center[0] + BOX_WIDTH/2) ;
			y2 = int(center[1] + BOX_HEIGHT/2);
			line[0]=x1;
			line[1]=y1;
			line[2]=x2;
			line[3]=y2;

			RotateLine2 ( center, line ,rot_theta,color);
			s[1] = (short)line[2];
			t[1] = (short)line[3];

			// draw top
			x1 = int(center[0] - BOX_WIDTH/2) ;
			y1 = int(center[1] - BOX_HEIGHT/2) ;

			x2 = int(center[0] + BOX_WIDTH/2) ;
			y2 = int(center[1] - BOX_HEIGHT/2);
			line[0]=x1;
			line[1]=y1;
			line[2]=x2;
			line[3]=y2;

			RotateLine2 ( center, line ,rot_theta,color);
			s[2] = (short)line[2] ;
			t[2] = (short)line[3];


			// draw left
			x1 = int(center[0] - BOX_WIDTH/2) ;
			y1 = int(center[1] - BOX_HEIGHT/2) ;
			x2 = int(center[0] - BOX_WIDTH/2) ;
			y2 = int(center[1] + BOX_HEIGHT/2);
			line[0]=x1;
			line[1]=y1;
			line[2]=x2;
			line[3]=y2;

			RotateLine2 ( center, line ,rot_theta,color);
			s[3] = (short)line[0];
			t[3] = (short)line[1] ;



			filledPolygonRGBA(TheWorld->screen,
                        s, t,
                        4,
                        53, 103, 53, 20);
			polygonRGBA(TheWorld->screen,
                        s, t,
                        4,
                        255, 255, 255, 1);

			setBoxJointsPositions();

			x = int(pB->BoxJoints[0].BoxJointPos[1] + TheWorld->bg_x );
			y = int(SCREEN_HEIGHT - pB->BoxJoints[0].BoxJointPos[2]-TheWorld->bg_y);
			apply_surface( x , y, object , TheWorld->screen );	

			x = int(pB->BoxJoints[1].BoxJointPos[1] + TheWorld->bg_x );
			y = int(SCREEN_HEIGHT - pB->BoxJoints[1].BoxJointPos[2]-TheWorld->bg_y);
			apply_surface( x , y, object , TheWorld->screen );	

			x = int(pB->BoxJoints[2].BoxJointPos[1] + TheWorld->bg_x );
			y = int(SCREEN_HEIGHT - pB->BoxJoints[2].BoxJointPos[2]-TheWorld->bg_y);
			apply_surface( x , y, object , TheWorld->screen );	

			break;
		}
		TRACE_OUT
	}
	void HandleInput(void)
	{
		TRACE_IN
		switch (ObjectType)
		{
		case CLOUD:
			break;
		case INVISIBLE:
			break;
		case SHIP:
			break;

		case PLANE:
			break;
		case BIRD:
			break;
		case BOX:
			BoxHandleInput();
			break;
		}

		TRACE_OUT
	}
	bool is_under_mouse()
	{
		return SDL_World ::is_under_mouse(x,y, BOX_WIDTH, BOX_HEIGHT);
	};
	bool CreateCloud(double x, double y);
	bool CreateInvisible(double x, double y);
	bool StaticObj::CreateInvisible2(double x, double y, double w, double h);
	bool CreateShip(double x, double y);
	bool CreateBox(double x, double y);
	void BoxHandleInput(void);
	dJointID MakeJoint(dBodyID body_to_joint, int *joint_number);
	dBodyID BoxGetClosest2Boxes(dBodyID *b1, dBodyID *b2);
};
