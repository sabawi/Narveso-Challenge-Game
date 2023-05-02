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
#ifndef _SPHERE_H
#define _SPHERE_H  1

#include "ode_world.h"
#include "sdl_world.h"

//The Sphere

class Sphere : public SDL_World , public MyWorld
{
	private:
		//The offsets
		int x, y;
		int next_awake;
		
		
	public: 
		enum BALL_TYPE {REGULAR,BLUE, RED,GREEN,BLACK,YELLOW} BallType;
		BALL_TYPE Type;
		// variables
		bool awake;
		bool awake_mad;
		int frame_count;
		SDL_Surface *sphere ;
		SDL_Surface *sphere_shadow ;
		//SDL_Surface *sphere_awake_mad ;
		//SDL_Surface *sphere_awake[3];
		SDL_Surface *red ;
		SDL_Surface *green ;
		SDL_Surface *blue ;
		SDL_Surface *shimmer;
		SDL_Surface *link1;
		SDL_Surface *link2;
		SDL_Surface *link3;
		SDL_Surface *link4;
		SDL_Surface *eye_l;
		SDL_Surface *eye_r;
		SDL_Surface *eye_sleep_l;
		SDL_Surface *eye_sleep_r;
		SDL_Surface *eye_pupil;
		double dAttachCoolDown ;	
		SDL_Surface *BallArray[32];
		SDL_Surface *BallShadowArray[32];

		//The sound effects that will be used
		Mix_Chunk *scratch ;
		Mix_Chunk *high ;
		Mix_Chunk *med ;
		Mix_Chunk *low ;

		Mix_Chunk *mad_sounds[5];
		Mix_Chunk *wow ;
		Mix_Chunk *iluvit;

		Mix_Music *m_sound1;
		Mix_Music *m_sound2;
		Mix_Music *m_sound3;
		Mix_Music *m_sound4;
		Mix_Music *m_sound5;
		int suf_images;
		int next_surf;
		bool surf_inc;

		int shadow_images;
		int next_shadow;
		bool shadow_inc;
		
		double momentum_x;
		double momentum_y;

		// ODE Variables
		dWorldID world;
		dSpaceID space;
		dJointGroupID SphereContacts;
		dGeomID ground, ceiling;
		dGeomID rightwall,leftwall;			
		dBodyID body;
		dGeomID geom;
		dMass mass;
		double density;

		unsigned long  nBodyNumber ;
        PTBlist p;

		//Initializes
		Sphere();
		bool Create(double x, double y);
		bool CreateRed(double x, double y);
		bool load_sound();
		bool load_images();
		bool load_imagesHHRed();		
		void init_center(int x, int y);
		
		//Handles mouse input
		void handle_input(int nDotCount);
		
		//is sphere under the mouse
		bool is_under_mouse()
		{
			return SDL_World ::is_under_mouse(x,y, SPHERE_WIDTH, SPHERE_HEIGHT);
		};	
		//Moves the Sphere
		void move();
		void stop();

		//Shows the Sphere
		void show();

		void setposition(dReal,dReal);
		void setvelocity(float,float);
		void getvelocity(float*,float*);
		void set_force_on_sphere(float , float , float , float );
		void rotate(double);

		double volume()
		{
			return ((PI/6)*SPHERE_WIDTH * SPHERE_WIDTH * SPHERE_WIDTH);
		}
		double weight()
		{
			//std::cout<<"volume = "<<volume()<<std::endl;
			return (density * volume());
		}
		int get_next_awake() {if (next_awake==(MAX_SPHERE_AWAKE-1)) next_awake=0;else next_awake++;return next_awake;}	
		int get_next_surface() 
		{
			if(surf_inc)
				next_surf++;
			else
				next_surf--;

			if (next_surf>=suf_images) 
			{
				next_surf--;
				surf_inc = false;
			}
			else if (next_surf <= 0)
			{
				next_surf++;
				surf_inc = true;
			}
			return next_surf;
		}		

		int get_next_shadow() 
		{
			if(shadow_inc)
				next_shadow++;
			else
				next_shadow--;

			if (next_shadow>=shadow_images) 
			{
				next_shadow--;
				shadow_inc = false;
			}
			else if (next_shadow <= 0)
			{
				next_shadow++;
				shadow_inc = true;
			}
			return next_shadow;
		}
		int get_x(){ return x;}
		int get_y(){ return y;}

		//Cleans up 
		void CleanUp(void);		
		~Sphere();		
};

class Sphere1Leg : public Sphere
{
	private:
		int x, y;

	public: 
		//is sphere under the mouse
		bool is_under_mouse()
		{
			return SDL_World ::is_under_mouse(x,y, SPHERE_WIDTH, SPHERE_HEIGHT);
		};	
		void init_center(int nX, int nY)
		{
			TRACE_IN

			x= TheWorld->bg_x -SPHERE_WIDTH/2 + nX;
			y= SCREEN_HEIGHT - SPHERE_HEIGHT/2 -nY-TheWorld->bg_y;

			TRACE_OUT
		};

		void CleanUp()
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


		void setposition(dReal X,dReal Y)
		{
			TRACE_IN
			if(!geom)
				return;
			dGeomSetPosition (geom,OBJECT_DEPTH,X, Y);	
			TRACE_OUT
		};

		//Handles mouse input
		void handle_input()
		{
			TRACE_IN
			if(!geom || !body)
				return;

			PTBlist thisp = (PTBlist)dGeomGetData( geom);
			if(TheWorld->MouseLocked && !thisp->bBeingMoved)
				return;
			
			if(is_under_mouse() && (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1)))
			{
				dGeomID o[5];
				o[0]=o[1]=o[2]=o[3]=o[4]=0;
				DrawToClosestAny(geom, 2, TheWorld->SearchRadius, o);
				
			}

			if( TheWorld->event.type == SDL_MOUSEBUTTONUP )
			{
				const dReal *mypos,*pos1;
				dBodyEnable (body);
				
				if(is_under_mouse() && thisp->bBeingMoved )
				{
					//dGeomID Connect2Geom1 =0;
					//Connect2Geom1 = DrawToClosest(geom, TheWorld->SearchRadius);
					dGeomID o[5];
					o[0]=o[1]=o[2]=o[3]=o[4]=0;
					DrawToClosestAny(geom, 2, TheWorld->SearchRadius, o);

					if(o[0] != 0)
					{
						if( (GetTime() - dAttachCoolDown)> 1)
						{
							int world_x = TheWorld->event.button.x - TheWorld->bg_x;
							int world_y = SCREEN_HEIGHT - TheWorld->event.button.y - TheWorld->bg_y;

							init_center(world_x, world_y);
							setposition(world_x, world_y);

							mypos = dGeomGetPosition(geom);					
							for(int i=0;i<2;i++)
							{						
								if(o[i])
								{
									pos1 = dGeomGetPosition(o[i]);

									// Check that none of the 2 bodies to be connected are too close to this body
									if( !IsWithinDistance(mypos,pos1,(double)SPHERE_WIDTH) )
									{
										dJointID joint=0;
										dJointFeedback *jfb = new dJointFeedback;
										dGeomSetPosition(o[i],OBJECT_DEPTH,pos1[1],pos1[2]  );
										joint=BallAttach2Bodies(thisp, (PTBlist)dGeomGetData (o[i]));

										if(joint)
											dJointSetBallAnchor (joint, OBJECT_DEPTH,mypos[1],mypos[2]);
										
										delete jfb;
									}
								}

								dAttachCoolDown = GetTime();

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
				dGeomID o[5];
				o[0]=o[1]=o[2]=o[3]=o[4]=0;
				
				if(is_under_mouse())
				{
					dBodyDisable (body);

					if(!awake)
					{
						Mix_PlayChannel( -1, high, 0 );

						awake = true;			
					}

					
					int world_x = TheWorld->event.button.x - TheWorld->bg_x;
					int world_y = SCREEN_HEIGHT - TheWorld->event.button.y - TheWorld->bg_y;
					init_center(world_x, world_y);
					setposition(world_x, world_y);					
					
					dBodySetLinearVel  (body, 0, 0, 0);

					thisp->bBeingMoved = true;
					//o1 = DrawToClosest(geom, TheWorld->SearchRadius);
					DrawToClosestAny(geom, 2, TheWorld->SearchRadius, o);
					TheWorld->MouseLocked = true;

				}
			}
			else if( TheWorld->event.type == SDL_MOUSEMOTION )
			{
				dGeomID o[5];
				o[0]=o[1]=o[2]=o[3]=o[4]=0;

				if(awake && (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1))) 
				{
					if(thisp->bBeingMoved)
					{
						dBodyDisable (body);

						int world_x = TheWorld->event.button.x - TheWorld->bg_x;
						int world_y = SCREEN_HEIGHT - TheWorld->event.button.y - TheWorld->bg_y;
						init_center(world_x, world_y);
						setposition(world_x, world_y);	

						dBodySetLinearVel  (body, 0, 0, 0);

						//o1 = DrawToClosest(geom, TheWorld->SearchRadius);
						DrawToClosestAny(geom, 2, TheWorld->SearchRadius, o);
			
					} 

				}
			}
			
			// Call default input handler
			SDL_World::handle_input();
			TRACE_OUT
		};

		void show()
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
			Uint32 color = SDL_MapRGB(TheWorld->screen->format, 238,201,0);

			if(!pB->bBeingMoved)
			{
				pos = dBodyGetPosition ( body );
				dx=pos[1];
				dy=pos[2];

				init_center(int(dx), (int)dy);
				setposition(dx,dy);

				GetRelDirectionToMouse(x,y,&rel_x,&rel_y);
			}
			else 
			{
				x =	TheWorld->event.button.x - SPHERE_WIDTH/2;			
				y = TheWorld->event.button.y - SPHERE_HEIGHT/2 ;		
			}

			if(awake || frame_count>0)
			{
				apply_surface( x, y, sphere , TheWorld->screen );
				apply_surface( x, y, sphere_shadow , TheWorld->screen );
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

			x1=x+SPHERE_WIDTH/2; 
			y1=y+SPHERE_HEIGHT/2;  


			for(i=0;i<pB->nJoints;i++)
			{
				if(pB->joint_to_delete[i])
				{
					dJointAttach(pB->joints[i],0,0);
					dJointDestroy(pB->joints[i]);
					pB->nJoints--;
					if(pB->nJoints==0)
						dJointGroupEmpty (SphereContacts);
					pB->joint_to_delete[i] = false;
				}
				else
				{
					tmpBody=dJointGetBody (pB->joints[i], 1);

					if(tmpBody && (tmpBody!=body))
					{			
						pos_link2 = dBodyGetPosition(tmpBody); // get center of the linked body
						x2= (int) pos_link2[1]+TheWorld->bg_x;  // update it for world view

						y2= SCREEN_HEIGHT  -(int)pos_link2[2]-TheWorld->bg_y;

						if(pB->joint_to_delete[i])
							sDrawLine2(x1, y1, x2, y2 , 0,0,255, 50);
						else
							//sDrawLine2(x1, y1, x2, y2 , 205,155,29, 50);
							DrawImageFromLinePoints(link1, x1, y1, x2, y2);

					}
				}
			}

			// check is cursor is close to body, if it is reset frame_count
			if(is_close(TheWorld->event.button.x,TheWorld->event.button.y,x,y,100))
				frame_count = 10;
			else if(frame_count >0)
				frame_count--;

			TRACE_OUT
		}


		bool load_images()
		{
			TRACE_IN

			if(!load_files(".\\images\\syellowball.png", &sphere))
			{
				TRACE_OUT
				return false;
			}

			if(!load_files(".\\images\\syellowball.png", &sphere_shadow))
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

			if(!load_files(".\\images\\link2.png", &link1))
			{
				TRACE_OUT
				return false;
			}
		    
			TRACE_OUT
			return true;
			
		}
		bool Create(double x, double y)
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
			Type = REGULAR;
			body = dBodyCreate (TheWorld->world);

			dMassSetSphere (&mass,TheWorld->sphere_density, SPHERE_WIDTH/2);

			dBodySetMass (body, &mass);

			geom = dCreateSphere (TheWorld->space,(dReal)SPHERE_WIDTH/2);	

			SphereContacts = dJointGroupCreate (0); 
		
			dGeomSetBody (geom,body);
			dGeomSetCategoryBits (geom, YELLOWBALL_CATEGORY);

			nBodyNumber =0;
			if(!load_images())
				return false;

			if(!load_sound())
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


};
void CreateSpheresBase(Sphere *sphere);

#endif

