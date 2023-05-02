
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
#ifndef _GGAME_H
#define _GGAME_H  1

#include "ode_world.h"
#include "sdl_world.h"
using namespace std;

class ggame
{
	private:
		// vars
		int difficulty;
		double dTimedCondition; // In Seconds: to be used for timed calls to functions
		double time_1 ;
		double time_2 ;
		double rot_angel;
		char *cOption;
		Baloon *baloon;
		Sphere *sphere;
		StaticObj *box;
		Sphere red_sphere;
		Walker *walker;
		Sign *retry_sign;
		Sign *back_sign;
		Sign *game_time_sign;
		Sign *child_saved_sign;
		Sign *help_sign;
		Sign *help_screen_OK;
		Sign *help_screen_text;
		Timer fps;
		help_text Plan; 
		StaticObj Cloud;
		StaticObj Invisible1, Invisible2;
		StaticObj Ship;
		bool bEngineer_have_plan;
		char strGameTime[100];
		char strSaved[100];
		bool show_balloons;
		int CloudPos_x;
		int CloudPos_y;


		//****************************************************
		//
		//****************************************************
		bool init(int d)
		{
			TRACE_IN
			difficulty = d;

			
			int tmp_x=0, tmp_y=0;
			dTimedCondition = 0.1; // In Seconds: to be used for timed calls to functions
			time_1 =  GetTime();
			time_2 = time_1;
			rot_angel =0;
			cOption = "";

			if(!init_game_world())
				exit(-1);
			baloon = new Baloon[TheWorld->nBalloons];
			sphere = new Sphere[TheWorld->nSpheres] ;
			walker = new Walker[TheWorld->nWalkers];
			box = new StaticObj[TheWorld->nBoxes];
			retry_sign = new Sign;
			back_sign = new Sign;
			game_time_sign = new Sign;
			child_saved_sign = new Sign;
			help_sign = new Sign;
			help_screen_OK = new Sign;
			help_screen_text = new Sign;
			bEngineer_have_plan = true;

			char strGameTime[100];
			char strSaved[100];
			bool show_balloons = false;
			int CloudPos_x= TheWorld->world_width/2+400;
			int CloudPos_y= TheWorld->world_height-400;

			retry_sign->SetSignDim(100,60);
			back_sign->SetSignDim(100,60);		

			switch(difficulty)
			{
			case 1:
				cOption = "E";
				break;
			case 2:
				cOption = "M";
				break;
			case 3:
				cOption = "H";
				break;
			default:
				break;
			}


			// Set game start time
			GameResults[nCurrentGame].start_time = GetTime();

			return true;
			TRACE_OUT
		};



		//****************************************************
		//
		//****************************************************
		bool init_game_world(void )
		{
			TRACE_IN

			double dBaseWorldStepSize =0.0043f;
			double dBaseWorldCFM = 0.0002f;
			double dBaseWorldERP = 0.99f;
			double dBaseWorldLinearDamping = 0.000001f;
			double dBaseWorldAngDamping =0.000001f;
			double dBaseWorldMaxAngSpeed = 50.0f;

			double dBaseWorldHingedParamCFM		= 0.0f;
			double dBaseWorldHingedParamBounce	= 0.001f;
			double dBaseWorldHingedParamStopERP	= 0.99f;
			double dBaseWorldHingedParamStopCFM	= 0.0f;

			switch(difficulty)
			{	
			case 1: //easy
				// ODE world parameters
				TheWorld->world_step_size		=	dBaseWorldStepSize;
				TheWorld->world_gravity			=	-9800.0f; // millimeter per second
				TheWorld->world_ang_damping		=	dBaseWorldAngDamping;
				TheWorld->world_linear_damping	=	dBaseWorldLinearDamping;
				TheWorld->world_ERP				=	dBaseWorldERP;
				TheWorld->world_CFM				=	dBaseWorldCFM ; // decreese for stability 0.0005, increase for instability 0.005
				TheWorld->world_max_angular_speed=	dBaseWorldMaxAngSpeed;
				TheWorld->world_auto_disable	=	0;
				TheWorld->fnc_nearCallback		=	nearCallback;

				// Basic Objects parameters
				TheWorld->sphere_density		=	0.001f; // water density = 0.001 grams/mm3 (millimeter cube) = 1 grams/cm3
				TheWorld->balloon_density		=	0.0005f;
				TheWorld->box_density			=	0.001f;

				// Hinge Parameters
				TheWorld->HingedParamCFM		= dBaseWorldHingedParamCFM;
				TheWorld->HingedParamBounce		= dBaseWorldHingedParamBounce;
				TheWorld->HingedParamStopERP	= dBaseWorldHingedParamStopERP;
				TheWorld->HingedParamStopCFM	= dBaseWorldHingedParamStopCFM;

				TheWorld->SearchRadius			= 300;

				TheWorld->nSpheres				= 15;
				TheWorld->nBalloons				= 6;
				TheWorld->nBoxes				= 20 ;
				TheWorld->nEndineers			= 0;
				TheWorld->nWalkers				= 50;
				TheWorld->nShips				= 1 ;
				TheWorld->nWinScore				= 42;
				TheWorld->balloon_lift_force	= -1.0 * TheWorld->world_gravity * TheWorld->nWinScore *  65.0f ;
				TheWorld->nWinBalloons			= 6;
				TheWorld->dWinXVelocity			= 100;
				TheWorld->EscapeVelocity		= -1000	;
				TheWorld->nJumpToShipDistance	=	450; // mm

				break;
			case 2: //medium 
				// ODE world parameters
				TheWorld->world_step_size		=	dBaseWorldStepSize;
				TheWorld->world_gravity			=	-9800.0f; // millimeter per second
				TheWorld->world_ang_damping		=	dBaseWorldAngDamping;
				TheWorld->world_linear_damping	=	dBaseWorldLinearDamping;
				TheWorld->world_ERP				=	dBaseWorldERP;
				TheWorld->world_CFM				=	dBaseWorldCFM * 3; //0.0015f; // decreese for stability 0.0005, increase for instability 0.005
				TheWorld->world_max_angular_speed=	dBaseWorldMaxAngSpeed;
				TheWorld->world_auto_disable	=	0;
				TheWorld->fnc_nearCallback		=	nearCallback;

				// Basic Objects parameters
				TheWorld->sphere_density		=	0.001f; // water density = 0.001 grams/mm3 (millimeter cube) = 1 grams/cm3
				TheWorld->balloon_density		=	0.0005f;
				TheWorld->box_density			=	0.001f;

				// Hinge Parameters
				TheWorld->HingedParamCFM		= dBaseWorldHingedParamCFM;
				TheWorld->HingedParamBounce		= dBaseWorldHingedParamBounce;
				TheWorld->HingedParamStopERP	= dBaseWorldHingedParamStopERP;
				TheWorld->HingedParamStopCFM	= dBaseWorldHingedParamStopCFM;

				TheWorld->SearchRadius			= 300;

				TheWorld->nSpheres				= 20;
				TheWorld->nBalloons				= 6;
				TheWorld->nBoxes				= 0 ;
				TheWorld->nEndineers			= 1;
				TheWorld->nWalkers				= 50;
				TheWorld->nShips				= 1 ;
				TheWorld->nWinScore				= 42;
				TheWorld->balloon_lift_force	= -1.0 * TheWorld->world_gravity * TheWorld->nWinScore *  60.0f ;
				TheWorld->nWinBalloons			= 6;
				TheWorld->dWinXVelocity			= 100;
				TheWorld->EscapeVelocity		= -1000	;
				TheWorld->nJumpToShipDistance	=	450; // mm
				break;

			case 3: //hard
				// ODE world parameters
				TheWorld->world_step_size		=	dBaseWorldStepSize;
				TheWorld->world_gravity			=	-9800.0f; // millimeter per second
				TheWorld->world_ang_damping		=	dBaseWorldAngDamping;
				TheWorld->world_linear_damping	=	dBaseWorldLinearDamping;
				TheWorld->world_ERP				=	dBaseWorldERP;
				TheWorld->world_CFM				=	dBaseWorldCFM * 4; //0.0026f; // decreese for stability 0.0005, increase for instability 0.005
				TheWorld->world_max_angular_speed=	dBaseWorldMaxAngSpeed;
				TheWorld->world_auto_disable	=	0;
				TheWorld->fnc_nearCallback		=	nearCallback;

				// Basic Objects parameters
				TheWorld->sphere_density		=	0.001f; // water density = 0.001 grams/mm3 (millimeter cube) = 1 grams/cm3
				TheWorld->balloon_density		=	0.0005f;
				TheWorld->box_density			=	0.001f;

				// Hinge Parameters
				TheWorld->HingedParamCFM		= dBaseWorldHingedParamCFM;
				TheWorld->HingedParamBounce		= dBaseWorldHingedParamBounce;
				TheWorld->HingedParamStopERP	= dBaseWorldHingedParamStopERP;
				TheWorld->HingedParamStopCFM	= dBaseWorldHingedParamStopCFM;

				TheWorld->SearchRadius			= 300;

				TheWorld->nSpheres				= 25;
				TheWorld->nBalloons				= 6;
				TheWorld->nBoxes				= 0 ;
				TheWorld->nEndineers			= 1;
				TheWorld->nWalkers				= 50;
				TheWorld->nShips				= 1 ;
				TheWorld->nWinScore				= 42;
				TheWorld->balloon_lift_force	= -1.0 * TheWorld->world_gravity * TheWorld->nWinScore *  60.0f ;
				TheWorld->nWinBalloons			= 6;
				TheWorld->dWinXVelocity			= 100;
				TheWorld->EscapeVelocity		= -1000;
				TheWorld->nJumpToShipDistance	=	450; // mm
				break;

			default:
				break;
			}
			if(!dInitODE2 (0))
				return false;

			TheWorld->overlay_help_screen_on = false;
			TheWorld->help_screen = NULL;
			TheWorld->help_flash_time = -1;

			// create world
			TheWorld->world = dWorldCreate ();
			
			TheWorld->space = dHashSpaceCreate (0);
			
			TheWorld->contactgroup = dJointGroupCreate (0);

			dJointGroupEmpty (TheWorld->contactgroup);

			InitSimParms();
			
			//dWorldSetContactSurfaceLayer (TheWorld->world,0.0001);
			
			// Init ODE objects and their link list
			TheWorld->nBodyCount = 0;
			TheWorld->pBlist = NULL;

			// Init world background
			TheWorld->bg_x=0;
			TheWorld->bg_y=0;
			TheWorld->MouseLocked = false;
			
			TheWorld->MyCursor = init_system_cursor(&arrow[0]);
			SDL_SetCursor(TheWorld->MyCursor);

			if(!set_background((std::string) ".\\images\\bg4.bmp"))
			{
				TRACE_STRING("Unable to load or update background image");
				cout<<"Fatel Error : Set Background failed to load. Exiting program"<<endl;
				return false;
				
			}
			
			//Set the world's limits from the background image
			TheWorld->world_height = TheWorld->background->h;
			TheWorld->world_width = TheWorld->background->w;
			TRACE_STRING("World h=%d, w=%d",TheWorld->world_height,TheWorld->world_width);

			//set initial view 
			TheWorld->bg_x = -TheWorld->background->w/2 + (SCREEN_WIDTH/2);

			// Set the hard ODE world limits by planes on all 4 sides
			
			// Bottom
			TheWorld->ground = dCreatePlane (TheWorld->space,0,0,1,0);
			
			// Top
			TheWorld->ceiling = dCreatePlane (TheWorld->space,0,0,-1,-TheWorld->world_height);
			
			// Left
			TheWorld->leftwall = dCreatePlane (TheWorld->space,0,1,0,0);
			
			// Right
			TheWorld->rightwall = dCreatePlane (TheWorld->space,0,-1,0,-TheWorld->world_width);

			// Back
			TheWorld->rightwall = dCreatePlane (TheWorld->space,1,0,0,500);

			//front
			TheWorld->rightwall = dCreatePlane (TheWorld->space,-1,0,0,-500);



			//load background music 
			if(Load_bg_music(".\\sounds\\bg_music.ogg"))
			{
				if(!play_bg_music())
				{
					TRACE_STRING("Failed to play background music");
					return false;
				}
			}
			else
			{
				TRACE_STRING("Failed to load background music");
				return false;
			}
				
			TRACE_OUT
			return true;
		}
		


		//****************************************************
		//
		//****************************************************
		void ShowAllObjects(void)
		{
			int i;
			for(i=0;i<TheWorld->nBalloons;i++)
				baloon[i].show();

			for(i=0;i<TheWorld->nSpheres;i++)
				sphere[i].show();

			for(i=0;i<TheWorld->nWalkers;i++)
			{
				walker[i].show(dGeomGetPosition(Ship.geom));
				if(walker[i].on_the_ship)
					GameResults[nCurrentGame].nBallsSaved++;
			}

			for(i=0;i<TheWorld->nBoxes;i++)
				box[i].show();

			Cloud.show();
			Ship.show();
			red_sphere.show();

			// If we have an overlay help screen, show it
			if(TheWorld->overlay_help_screen_on)
			{
				apply_surface(0, 0, TheWorld->help_screen, TheWorld->screen );
				help_screen_OK->place_signdull("OK", TheWorld->help_screen->w-100,TheWorld->help_screen->h-150 );
				help_screen_text->place_signbright(Plan.szPlanLines[0],20,80);
				help_screen_text->place_signbright(Plan.szPlanLines[1],20+10,80+40);
				help_screen_text->place_signbright(Plan.szPlanLines[2],20+20,80+75);
				help_screen_text->place_signbright(Plan.szPlanLines[3],20+30,80+110);
				help_screen_text->place_signbright(Plan.szPlanLines[4],20+40,80+320);

				if(help_screen_OK->is_under_mouse())
				{
					help_screen_OK->place_signbright("OK",TheWorld->help_screen->w-100,TheWorld->help_screen->h-150);
					if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1))
					{
						TheWorld->overlay_help_screen_on = false;
						TheWorld->help_screen;
					}
				}

			}

		}

		//****************************************************
		//
		//****************************************************
		int handle_input()
		{
			int ret =1;
			int i =0;

			// check if user want to quit
			if( (TheWorld->event.type == SDL_QUIT ) ||
				(TheWorld->event.type == SDL_KEYUP && TheWorld->event.key.keysym.sym == SDLK_ESCAPE))
			{ 
				//Quit the game 
				ret = -1; 
			} 

			for(i=0;i<TheWorld->nSpheres;i++)
			{
				PTBlist pB = (PTBlist) dBodyGetData(sphere[i].body);
				if(pB->nJoints==0)
					sphere[i].handle_input(TheWorld->nSpheres);
			}

			for(i=0;i<TheWorld->nBoxes;i++)
				box[i].HandleInput();	

			//Handle The_World.events for the Baloons
			for(i=0;i<TheWorld->nBalloons;i++)
			{
				if(!baloon[i].is_poped)
					baloon[i].handle_input(TheWorld->nBalloons);
			}

			if(red_sphere.is_under_mouse())
			{
				if(bEngineer_have_plan)
					help_sign->place_signcolor("I've GOT A PLAN",red_sphere.get_x()-80,red_sphere.get_y()-80,36,SDL_MapRGB(TheWorld->screen->format, 255,255,255));
				else
					help_sign->place_signcolor("I'M STUMPED!!",red_sphere.get_x()-80,red_sphere.get_y()-80,36,SDL_MapRGB(TheWorld->screen->format, 255,255,255));

				if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1) && !TheWorld->overlay_help_screen_on)
				{
					
					TheWorld->help_screen = load_image( ".\\images\\eng_pad.png" );
					if(TheWorld->help_screen)
					{
						TheWorld->overlay_help_screen_on = true;
						TheWorld->help_flash_time = 500; // milliseconds 
					}

				}
			}

			drawText(cOption,24,SCREEN_WIDTH -40,SCREEN_HEIGHT - 140,100, 100, 100,0,0,0);
			if(retry_sign->is_under_mouse())
			{
				retry_sign->place_signbright("RETRY",SCREEN_WIDTH -110,SCREEN_HEIGHT - 100);
				if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1))
				{
					ret = -1;

				}
			}
			else
				retry_sign->place_signdull("RETRY",SCREEN_WIDTH -110,SCREEN_HEIGHT - 100);
			
			if(back_sign->is_under_mouse())
			{
				back_sign->place_signbright("MENU",SCREEN_WIDTH -110,SCREEN_HEIGHT - 50);
				if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1))
				{
					ret = -1;
				}				
				
			}
			else
				back_sign->place_signdull("MENU",SCREEN_WIDTH -110,SCREEN_HEIGHT -50);
			return ret;

		};

	public: 
		//****************************************************
		//
		//****************************************************
		ggame()
		{
			TRACE_IN

			Baloon *baloon = NULL;
			Sphere *sphere = NULL;
			Sphere red_sphere;
			StaticObj *box = NULL;
			Walker *walker = NULL;;
			Sign *retry_sign = NULL;
			Sign *back_sign = NULL;
			Sign *game_time_sign = NULL;;
			Sign *child_saved_sign = NULL;;
			Sign *help_sign = NULL;;
			Sign *help_screen_OK = NULL;;
			Sign *help_screen_text = NULL;;	
		
			TRACE_OUT
		};


		//****************************************************
		//
		//****************************************************
		~ggame()
		{
			TRACE_IN
			int i =0;

			for(i=0;i<TheWorld->nBalloons;i++)
				baloon[i].CleanUp();

			for(i=0;i<TheWorld->nSpheres;i++)
				sphere[i].CleanUp();

			for(i=0;i<TheWorld->nWalkers;i++)
				walker[i].CleanUp();

			// delete the objects
			delete [] sphere;
			delete [] baloon;
			delete [] walker;
			delete retry_sign;
			delete back_sign;
			delete [] box;
			delete game_time_sign;
			delete child_saved_sign;
			delete help_sign;
			delete help_screen_OK;
			delete help_screen_text;

			// We have exited -- Destry the world and all in it
			destroy_world();

			TRACE_OUT			
		};


		//****************************************************
		//
		//****************************************************
		int run(int d)
		{
			int ret = MAINMENU;
			bool quit = false;
			int i =0, nbg =1,row=0,col=0;

			init(d);
			strcpy(Plan.szPlanLines[0],"It is written:");
			strcpy(Plan.szPlanLines[1],"'Group of littles may");
			strcpy(Plan.szPlanLines[2],"survive if they exceed");
			strcpy(Plan.szPlanLines[3],"two to power five.'");
			strcpy(Plan.szPlanLines[4],"Wiser is wider at the base, I say.");


			// Create and position objects
			// -- Cloud
			int CloudPos_x= TheWorld->world_width/2+400;
			int CloudPos_y= TheWorld->world_height-400;
			Cloud.CreateCloud(CloudPos_x,CloudPos_y);

			// -- invisible objects
			Invisible1.CreateInvisible(TheWorld->background->w-300,500);
			Invisible2.CreateInvisible(TheWorld->background->w-300,300);

			// -- Ship
			Ship.CreateShip(TheWorld->world_width/2+600,TheWorld->world_height-356);

			// -- Spheres
			for(i=0;i<TheWorld->nSpheres;i++)
			{
				double x = 100+(col*SPHERE_WIDTH);
				double y = SPHERE_HEIGHT/2 +(row*(SPHERE_HEIGHT+1));
				sphere[i].Create(x,y);	
				// Make rows of 10 spheres 
				if(col<10)
				{ 
					col++;
				}
				else
				{
					col=0;
					row++;
				}
			}

			for(i=0;i<TheWorld->nBoxes;i++)
			{
				double x = TheWorld->world_width/2+300 ;
				double y = 10+BOX_HEIGHT/2 + (i*(BOX_HEIGHT+1));
				box[i].CreateBox(x,y);
			}

			// -- Engineer
			TheWorld->EngineerPosition[0] = OBJECT_DEPTH;
			TheWorld->EngineerPosition[1] =  3100;
			TheWorld->EngineerPosition[2] =	 TheWorld->world_height - 2223;
			red_sphere.CreateRed(TheWorld->EngineerPosition[1],TheWorld->EngineerPosition[2]);

			//*BALLOONS**********************
			// position the Baloons 
			for(i=0;i<TheWorld->nBalloons;i++)
			{

				baloon[i].Create();		
				PTBlist pB = (PTBlist) dGeomGetData(baloon[i].geom);

				const dReal *CloudPos = dGeomGetPosition(Cloud.geom);
				double BalloonPos_x = CloudPos[1] - (Cloud.object->w/3)+ (i* (baloon[i].baloon->w +5));
				double BalloonPos_y = CloudPos[2] + Cloud.object->h/3 ;		

				baloon[i].anchor_x =CloudPos[1] - (Cloud.object->w/3)+ (i* (baloon[i].baloon->w +5));
				baloon[i].anchor_y =CloudPos[2] + Cloud.object->h/3;

				baloon[i].setposition(BalloonPos_x,BalloonPos_y);		
				baloon[i].init_center(int(BalloonPos_x),int(BalloonPos_y));
				rotate_body_x(baloon[i].body, 0.0f);

				dJointID jnt = HingeAttach2Bodies(pB, NULL);
				dJointSetHingeAnchor (jnt, OBJECT_DEPTH,baloon[i].anchor_x, baloon[i].anchor_y);
				dBodySetAngularVel(baloon[i].body,0,0,0);

			}

			//*WALKERS***********************
			// position the Walkers 
			for(i=0;i<TheWorld->nWalkers;i++)
				walker[i].init_center(100+(rand() % (TheWorld->world_width-WALKER_WIDTH)),WALKER_HEIGHT/2);

			// End object creation section

			// START THE BIG LOOP
			while( quit == false )
			{
				//Start the frame timer 
				fps.start();

				// Get SDL events
				SDL_PollEvent( &TheWorld->event );





				
				if((time_2-time_1)>dTimedCondition)
				{
					// Add a trigger to be pulled HERE

					// Reset trigger time
					time_1 = time_2 = GetTime();
				}

				//Scroll if the mouse at any enge of the view
				AdjustViewToWorld();


				// clear screen
				//SDL_FillRect(TheWorld->screen, NULL, SDL_MapRGB(TheWorld->screen->format, 0, 0, 0)); 
				apply_surface( TheWorld->bg_x, SCREEN_HEIGHT-TheWorld->world_height-TheWorld->bg_y, TheWorld->background, TheWorld->screen );
				// add up children saved
				GameResults[nCurrentGame].nBallsSaved =0;
				
				// display objects
				ShowAllObjects();

				// proccess all user input here
				if(handle_input() == -1)
					quit = true;

				if(GameResults[nCurrentGame].nBallsSaved>TheWorld->nWinScore)
					show_balloons = true;

				// flip to the new screen
				if( SDL_Flip(TheWorld-> screen ) == -1 )
				{
					exit(-1);
				}
			  
				//Cap the frame rate
//				if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
//				{
//					SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
//				}

				// Capture the end time of the cycle
				time_2 = GetTime();

				// Run a world ODE simulation cycle
				simWorld();	
			}

			return ret;
		}

};



#endif
