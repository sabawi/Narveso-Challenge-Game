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
#include "ode_world.h"
#include "sdl_world.h"
#include "sdl_utils.h"
#include "timer.h"
#include "sphere.h"
#include "baloon.h"
#include "box.h"
#include "basket1.h"
#include "walker.h"
#include "sign.h"
#include "static_obj.h"

#include <windows.h>

using namespace std;


bool init_game2_world(int world_type)
{
	TRACE_IN
	double dBaseWorldStepSize =0.0043f;
	double dBaseWorldCFM = 0.0001f;
	double dBaseWorldERP = 0.99f;
	double dBaseWorldLinearDamping = 0.00001f;
	double dBaseWorldAngDamping =0.01f;
	double dBaseWorldMaxAngSpeed = 50.0f;

	double dBaseWorldHingedParamCFM		= 0.0f;
	double dBaseWorldHingedParamBounce	= 0.001f;
	double dBaseWorldHingedParamStopERP	= 0.90f;
	double dBaseWorldHingedParamStopCFM	= 0.0f;

	// using metric millimeter and grams 
	switch(world_type)
	{	
	case 1: //easy
		// ODE world parameters
		TheWorld->world_step_size		=	dBaseWorldStepSize;
		TheWorld->world_gravity			=	-9800.0f; // millimeter per second
		TheWorld->world_ang_damping		=	dBaseWorldAngDamping;
		TheWorld->world_linear_damping	=	dBaseWorldLinearDamping;
		TheWorld->world_ERP				=	dBaseWorldERP;
		TheWorld->world_CFM				=	dBaseWorldCFM; // decreese for stability 0.0005, increase for instability 0.005
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

		TheWorld->nSpheres				= 30;
		TheWorld->nBalloons				= 4;
		TheWorld->nBoxes				= 0 ;
		TheWorld->nEndineers			= 1;
		TheWorld->nWalkers				= 40;
		TheWorld->nShips				= 1 ;
		TheWorld->nWinScore				= 33;
		TheWorld->balloon_lift_force	= -1.0 * TheWorld->world_gravity * TheWorld->nWinScore *  65.0f ;
		TheWorld->nWinBalloons			= 4;
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
		TheWorld->world_CFM				=	dBaseWorldCFM + 0.00005; // decreese for stability 0.0005, increase for instability 0.005
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

		TheWorld->nSpheres				= 40;
		TheWorld->nBalloons				= 4;
		TheWorld->nBoxes				= 0 ;
		TheWorld->nEndineers			= 1;
		TheWorld->nWalkers				= 40;
		TheWorld->nShips				= 1 ;
		TheWorld->nWinScore				= 33;
		TheWorld->balloon_lift_force	= -1.0 * TheWorld->world_gravity * TheWorld->nWinScore *  65.0f ;
		TheWorld->nWinBalloons			= 4;
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
		TheWorld->world_CFM				=	dBaseWorldCFM + 0.0001; // decreese for stability 0.0005, increase for instability 0.005
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

		TheWorld->nSpheres				= 45;
		TheWorld->nBalloons				= 4;
		TheWorld->nBoxes				= 0 ;
		TheWorld->nEndineers			= 1;
		TheWorld->nWalkers				= 40;
		TheWorld->nShips				= 1 ;
		TheWorld->nWinScore				= 33;
		TheWorld->balloon_lift_force	= -1.0 * TheWorld->world_gravity * TheWorld->nWinScore *  65.0f ;
		TheWorld->nWinBalloons			= 6;
		TheWorld->dWinXVelocity			= 100;
		TheWorld->EscapeVelocity		= -1000	;
		TheWorld->nJumpToShipDistance	=	450; // mm
		break;

	default:
		break;
	}
	
	TheWorld->FX.body = 0;
	TheWorld->FX.joint =0;
	TheWorld->FX.value =0;
	TheWorld->FY.body = 0;
	TheWorld->FY.joint =0;
	TheWorld->FY.value =0;
	TheWorld->FZ.body = 0;
	TheWorld->FZ.joint =0;
	TheWorld->FZ.value =0;

	TheWorld->TX.body = 0;
	TheWorld->TX.joint =0;
	TheWorld->TX.value =0;
	TheWorld->TY.body = 0;
	TheWorld->TY.joint =0;
	TheWorld->TY.value =0;
	TheWorld->TZ.body = 0;
	TheWorld->TZ.joint =0;
	TheWorld->TZ.value =0;

	if(!dInitODE2 (0))
		return false;

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

	if(!set_background((std::string) ".\\images\\g2bg.bmp"))
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
			TRACE_STRING("Failed to play background music");
	}
	else
		TRACE_STRING("Failed to load background music");
		
	TRACE_OUT
	return true;
	
}

//==================================================================================
//   Game 2 Entry
//
//==================================================================================
int Game2(int difficulty, bool bSkipIntro)
{
	//bTrace = true;
	TRACE_IN
	int ret = MAINMENU;
	bool quit = false;
	int i =0, nbg =1,row=0,col=0;
	int tmp_x=0, tmp_y=0;
	//const int nSphereCount = 30,
	//	nBoxCount =0, 
	//	nBaloonCount=6, 
	//	nWalkerCount = 40;
	bool bZoomOut = true;
	double dZoomTime = 0; // in seconds
	double dCurrentZoom = 1.0;

	double dTimedCondition = 1.0; // In Seconds: to be used for timed calls to functions
	double time_1 =  GetTime();
	double time_2 = time_1;
	double rot_angel =0;
	char *cOption = "";
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
	

	Timer fps;


    if(!init_game2_world(difficulty))
		exit(-1);
	Baloon *baloon = new Baloon[TheWorld->nBalloons];
	Sphere *sphere = new Sphere[TheWorld->nSpheres] ;
	Sphere red_sphere;
	Walker *walker = new Walker[TheWorld->nWalkers];
	Sign *retry_sign = new Sign;
	Sign *back_sign = new Sign;
	Sign *game_time_sign = new Sign;
	Sign *child_saved_sign = new Sign;
	Sign *help_sign = new Sign;
	Sign *help_screen_OK = new Sign;
	Sign *help_screen_text = new Sign;

	//SDL_Surface *fog_suf = NULL;
	//SDL_Surface *fog_suf2 = NULL;

	//int fog_scroll = 0;
	//fog_suf = load_image(".\\images\\fog.png");

	//fog_suf2 = rotozoomSurface(fog_suf,0,TheWorld->background->h/fog_suf->h,1);
    //Free the old image
    //SDL_FreeSurface( fog_suf );
    //If the image was optimized just fine
	//fog_suf2 = fog_suf;
    //if( fog_suf2 != NULL )
    {
		   //Map the color key
           //Uint32 colorkey = SDL_MapRGB( fog_suf2->format, 0, 0, 0 );
		   //Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 0, 0, 0 );
           //Set all pixels of color R 0, G 0xFF, B 0xFF to be transparent
           //SDL_SetColorKey( fog_suf2, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey );
		   //SDL_SetAlpha( fog_suf2, SDL_SRCALPHA | SDL_RLEACCEL, 255 );
	}
	
    //Create an optimized bmp image
    //fog_suf = SDL_DisplayFormatAlpha( fog_suf2 );


	StaticObj Cloud;
	StaticObj Invisible1, Invisible2;
	StaticObj Ship;
	//StaticObj *Box = new StaticObj[TheWorld->nBoxes];
	bool bEngineer_have_plan = true;
	help_text Plan; 
	retry_sign->SetSignDim(100,60);
	back_sign->SetSignDim(100,60);

	strcpy(Plan.szPlanLines[0],"It is written:");
	strcpy(Plan.szPlanLines[1],"'Group of littles may");
	strcpy(Plan.szPlanLines[2],"survive if they exceed");
	strcpy(Plan.szPlanLines[3],"two to power five.'");
	strcpy(Plan.szPlanLines[4],"Wiser is wider at the base, I say.");

	char strGameTime[100];
	char strSaved[100];
	bool show_balloons = false;

	// Redraw the background in the view window
	apply_surface( TheWorld->bg_x, SCREEN_HEIGHT-TheWorld->world_height-TheWorld->bg_y, TheWorld->background, TheWorld->screen ); 

	int CloudPos_x= TheWorld->world_width/2+400;
	int CloudPos_y= TheWorld->world_height-400;
	Cloud.CreateCloud(CloudPos_x,CloudPos_y);

	Invisible1.CreateInvisible(TheWorld->background->w-300,500);
	Invisible2.CreateInvisible(TheWorld->background->w-300,300);

	Ship.CreateShip(TheWorld->world_width/2+600,CloudPos_y + Cloud.object->h/2 +50 );

	//********************************
	//****** World Objects ***********
	TheWorld->EngineerPosition[0] = OBJECT_DEPTH;
	TheWorld->EngineerPosition[1] =  3100;
	TheWorld->EngineerPosition[2] =	 TheWorld->world_height - 2223;
	//* BASKETS ******************************

	// position the boxes
	//if(nBoxCount>=4)CreateBasketsBase( basket);	
//	for(i=0;i<TheWorld->nBoxes;i++)
//	{
//		double x = TheWorld->world_width/2+500 ;
//		double y = BOX_HEIGHT/2 + (i*(BOX_HEIGHT));
//		Box[i].CreateBox(x,y);
//	}
		
	//*SPHERES************************

	// Position the spheres
	if(TheWorld->nSpheres>=4)CreateSpheresBase( sphere);	
	for(i=4;i<TheWorld->nSpheres;i++)
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

//************************************************************************************************
//************************************************************************************************
	// Set game start time
	GameResults[nCurrentGame].start_time = GetTime();

	//*******************
	//*******************
	// START THE BIG LOOP
	while( quit == false )
	{
		if(!bSkipIntro)
		{
			if(bZoomOut)
				bZoomOut = ZoomOut( dZoomTime, 30.0, dCurrentZoom);
		}
		else
			TheWorld->rotozoom.Enable = bZoomOut = false;

		//Start the frame timer 
		fps.start();

		SDL_PollEvent( &TheWorld->event );

		// add up children saved
		GameResults[nCurrentGame].nBallsSaved =0;
		for(i=0;i<TheWorld->nWalkers;i++)
		{
			if(walker[i].on_the_ship)
				GameResults[nCurrentGame].nBallsSaved++;
		}
		
		if(GameResults[nCurrentGame].nBallsSaved>=TheWorld->nWinScore)
		{
			show_balloons = true;
		}

		// Check game result
		if(GameResults[nCurrentGame].bGameOver && (GetTime() >GameResults[nCurrentGame].end_time + 1.0) )
		{
			// game is ending -- start the ending sceen

			char strTime[255];
			int inc =5, count=0;
			GameResults[nCurrentGame].end_time = GetTime();
			sprintf(strSaved,"%d Children Saved out of %d",GameResults[nCurrentGame].nBallsSaved,TheWorld->nWalkers);
			sprintf(strTime,"It took you %s",GetTimeDiffString2(GameResults[nCurrentGame].start_time,GetTime(),strGameTime));

			while(!quit && GetTime()<GameResults[nCurrentGame].end_time +20)
			{
				SDL_PollEvent( &TheWorld->event );
				apply_surface( TheWorld->bg_x, SCREEN_HEIGHT-TheWorld->world_height-TheWorld->bg_y, TheWorld->background, TheWorld->screen ); 
				
				child_saved_sign->place_signcolor(strSaved,SCREEN_WIDTH/4,SCREEN_HEIGHT/2-200,45,SDL_MapRGB(TheWorld->screen->format, 0,255,0));
				child_saved_sign->place_signcolor(strTime,SCREEN_WIDTH/4,SCREEN_HEIGHT/2-100,45,SDL_MapRGB(TheWorld->screen->format, 0,255,0));
				child_saved_sign->place_signcolor("Survival Journey Continues...",SCREEN_WIDTH/4+100,SCREEN_HEIGHT/2 ,45,SDL_MapRGB(TheWorld->screen->format, 0,255,0));
				drawText(cOption,24,SCREEN_WIDTH -40,SCREEN_HEIGHT - 140,100, 100, 100,0,0,0);
				if(retry_sign->is_under_mouse())
				{
					retry_sign->place_signbright("RETRY",SCREEN_WIDTH -110,SCREEN_HEIGHT - 100);
					if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1))
					{
						ret = nCurrentGame;
						quit = true;
					}
				}
				else
					retry_sign->place_signdull("RETRY",SCREEN_WIDTH -110,SCREEN_HEIGHT - 100);
				
				if(back_sign->is_under_mouse())
				{
					back_sign->place_signbright("MENU",SCREEN_WIDTH -110,SCREEN_HEIGHT - 50);
					if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1))
					{
						ret = MAINMENU;
						quit = true;
					}				
					
				}
				else
					back_sign->place_signdull("MENU",SCREEN_WIDTH -110,SCREEN_HEIGHT -50);
				Ship.show();
				for(i=0;i<TheWorld->nWalkers;i++)
					if(walker[i].body)
						walker[i].show(dGeomGetPosition(Ship.geom));
				for(i=0;i<TheWorld->nBalloons;i++)
				{
					// show Baloon bodies
					if(!baloon[i].is_poped)
						baloon[i].show();
				}
				// Redraw the background in the view window
				//Scroll background 
				TheWorld->bg_x -= inc; 
				//If the background has gone too far 
				if( TheWorld->bg_x  <= - TheWorld->background->w +SCREEN_WIDTH) 
				{ 
					//Reset the offset 
					TheWorld->bg_x = SCREEN_WIDTH ; 
					inc = 5;
					count++;

					if(count ==2) break;
	
				} 
				// flip to the new screen
				if( SDL_Flip(TheWorld->screen ) == -1 )
				{
					exit(-1);
				}
	  
			}

			quit = true;
			continue;
		}

		// Put 
		if((time_2-time_1)>dTimedCondition)
		{
            // Add a call to anything that need to be called on regular intervals 
			//...
			//const dReal *pos1 = dBodyGetPosition(sphere[5].body);
			//const dReal *pos2 = dBodyGetPosition(Ship.body);
			//double dist = sqrt( pow((pos1[1]-pos2[1]),2)+pow((pos1[2]-pos2[2]),2));
			//dBodyAddForce(sphere[5].body,0,50000 * (pos2[1]-pos1[1]),50000* (pos2[2]-pos1[2]));
			rot_angel = rot_angel -(360/60);
			if(rot_angel <= - 360) rot_angel = 0;

			if(bZoomOut)
				dZoomTime+=1.0;
			else
				dZoomTime =0.0;
			time_1 = time_2 = GetTime();
		}
			
		//Scroll if the mouse at any enge of the view
		AdjustViewToWorld();

		//While there's events to handle
		//while( SDL_PollEvent( &TheWorld->event ) )
		{        
			//Handle The_World.events for the spheres
			for(i=0;i<TheWorld->nSpheres;i++)
			{
				PTBlist pB = (PTBlist) dBodyGetData(sphere[i].body);
				if(pB->nJoints==0)
					sphere[i].handle_input(TheWorld->nSpheres);
			}

//			for(i=0;i<TheWorld->nBoxes;i++)
//				Box[i].HandleInput();	

			//Handle The_World.events for the Baloons
			for(i=0;i<TheWorld->nBalloons;i++)
			{
				if(!baloon[i].is_poped)
					baloon[i].handle_input(TheWorld->nBalloons);
			}

			//If the user has Xed out the window 
			if( TheWorld->event.type == SDL_QUIT )
			{
				//Quit the program 
				quit = true;
			}

		}
		
		// Redraw the background in the view window		
		//fog_scroll-=4;
		//if(fog_scroll <= -fog_suf2->w)
		//	fog_scroll=0;
		

//SDL_FillRect(TheWorld->screen, NULL, SDL_MapRGB(TheWorld->screen->format, 0, 0, 0)); 
		apply_surface( TheWorld->bg_x, SCREEN_HEIGHT-TheWorld->world_height-TheWorld->bg_y, TheWorld->background, TheWorld->screen ); 
//		apply_surface( TheWorld->bg_x +fog_scroll, SCREEN_HEIGHT-TheWorld->world_height-TheWorld->bg_y, fog_suf, TheWorld->screen );
//		apply_surface( fog_suf->w +  TheWorld->bg_x + fog_scroll, SCREEN_HEIGHT-TheWorld->world_height-TheWorld->bg_y, fog_suf, TheWorld->screen ); 


//		for(i=0;i<TheWorld->nBoxes ;i++)
//			Box[i].show();

		//Show the sphers on  screen
		for(i=0;i<TheWorld->nSpheres;i++)
		{
			sphere[i].show();
		}

		Cloud.show();
		Ship.show();
		red_sphere.show();

		/*
		{		
		char str[50];
		sprintf(str,"IN %3.2f",rot_angel);
		drawText(str,24,400,400,0,0,255,0,0,0);
		RotateLine(400, 400,0, 100, rot_angel*PI/180 );

		}
	
		double center[]={400,400};
		double line[]={500,500,500,600};
		RotateLine2 ( center, line ,rot_angel,color);
		double line2[]={300,300,300,600};
		RotateLine2 ( center, line2 ,rot_angel,color);
		double line3[]={300,600,500,600};
		RotateLine2 ( center, line3 ,rot_angel,color);
		double line4[]={600,500,600,300};
		RotateLine2 ( center, line4 ,rot_angel,color);
		double shaft1[]={400,420,500,420};
		RotateLine2 ( center, shaft1 ,rot_angel,color);
		double shaft2[]={400,380,500,380};
		RotateLine2 ( center, shaft2 ,rot_angel,color);
		double shaft3[]={400,380,400,420};
		RotateLine2 ( center, shaft3 ,rot_angel,color);
		*/
		
		if(TheWorld->FX.body)
		{
			const dReal *pos;
			char str[50];
			int x1,y1;
			pos = dBodyGetPosition(TheWorld->FX.body);
			x1= int(TheWorld->bg_x -SPHERE_WIDTH/2 + pos[1]);
			y1= int(SCREEN_HEIGHT - SPHERE_HEIGHT/2 -pos[2]-TheWorld->bg_y);
			sprintf(str,"Highest Fx");
			drawText(str,24,x1+30,y1+30,255,0,0,0,0,0);
		}
		if(TheWorld->FY.body)
		{
			const dReal *pos;
			char str[50];
			int x1,y1;
			pos = dBodyGetPosition(TheWorld->FY.body);
			x1= int(TheWorld->bg_x -SPHERE_WIDTH/2 + pos[1]);
			y1= int(SCREEN_HEIGHT - SPHERE_HEIGHT/2 -pos[2]-TheWorld->bg_y);
			sprintf(str,"Highest Fy");
			drawText(str,24,x1+30,y1,255,0,0,0,0,0);
		}

		if(TheWorld->FZ.body)
		{
			const dReal *pos;
			char str[50];
			int x1,y1;
			pos = dBodyGetPosition(TheWorld->FZ.body);
			x1= int(TheWorld->bg_x -SPHERE_WIDTH/2 + pos[1]);
			y1= int(SCREEN_HEIGHT - SPHERE_HEIGHT/2 -pos[2]-TheWorld->bg_y);
			sprintf(str,"Highest Fz");
			drawText(str,24,x1+30,y1-30,255,0,0,0,0,0);
		}


		if(TheWorld->TX.body)
		{
			const dReal *pos;
			char str[50];
			int x1,y1;
			pos = dBodyGetPosition(TheWorld->TX.body);
			x1= int(TheWorld->bg_x -SPHERE_WIDTH/2 + pos[1]);
			y1= int(SCREEN_HEIGHT - SPHERE_HEIGHT/2 -pos[2]-TheWorld->bg_y);
			sprintf(str,"Highest Tx");
			drawText(str,24,x1-30,y1+30,255,100,0,0,0,0);
		}
		if(TheWorld->TY.body)
		{
			const dReal *pos;
			char str[50];
			int x1,y1;
			pos = dBodyGetPosition(TheWorld->TY.body);
			x1= int(TheWorld->bg_x -SPHERE_WIDTH/2 + pos[1]);
			y1= int(SCREEN_HEIGHT - SPHERE_HEIGHT/2 -pos[2]-TheWorld->bg_y);
			sprintf(str,"Highest Ty");
			drawText(str,24,x1-30,y1,255,100,0,0,0,0);
		}

		if(TheWorld->TZ.body)
		{
			const dReal *pos;
			char str[50];
			int x1,y1;
			pos = dBodyGetPosition(TheWorld->TZ.body);
			x1= int(TheWorld->bg_x -SPHERE_WIDTH/2 + pos[1]);
			y1= int(SCREEN_HEIGHT - SPHERE_HEIGHT/2 -pos[2]-TheWorld->bg_y);
			sprintf(str,"Highest Tz");
			drawText(str,24,x1-30,y1-30,255,100,0,0,0,0);
		}



		// Show the Baloons on the screen
		for(i=0;i<TheWorld->nBalloons && show_balloons;i++)
		{
			dWorldSetERP(TheWorld->world,0.9f);
			dWorldSetCFM (TheWorld->world,0.0005f);
			// show Baloon bodies
			if(!baloon[i].is_poped)
				baloon[i].show();
		}
		
		// position the walkers 
		for(i=0;i<TheWorld->nWalkers;i++)
			walker[i].show(dGeomGetPosition(Ship.geom));
		
		game_time_sign->place_signbright(
									GetTimeDiffString(
											GameResults[nCurrentGame].start_time,GetTime(),strGameTime),
									SCREEN_WIDTH -300,
									20);
		sprintf(strSaved,"Child Loaded %d",GameResults[nCurrentGame].nBallsSaved);
		if(GameResults[nCurrentGame].nBallsSaved>=TheWorld->nWinScore)
		{
			child_saved_sign->place_signcolor(strSaved,SCREEN_WIDTH -350,90,48,SDL_MapRGB(TheWorld->screen->format, 0,255,0));
		}
		else
			child_saved_sign->place_signbright(strSaved,SCREEN_WIDTH -300,90);

		child_saved_sign->place_signbright("Child Saved 0",SCREEN_WIDTH -300,160);		

		drawText(cOption,24,SCREEN_WIDTH -40,SCREEN_HEIGHT - 140,100, 100, 100,0,0,0);
		if(retry_sign->is_under_mouse())
		{
			retry_sign->place_signbright("RETRY",SCREEN_WIDTH -110,SCREEN_HEIGHT - 100);
			if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1))
			{
				ret = nCurrentGame;
				quit = true;
			}
		}
		else
			retry_sign->place_signdull("RETRY",SCREEN_WIDTH -110,SCREEN_HEIGHT - 100);
		
		if(back_sign->is_under_mouse())
		{
			back_sign->place_signbright("MENU",SCREEN_WIDTH -110,SCREEN_HEIGHT - 50);
			if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1))
			{
				ret = MAINMENU;
				quit = true;
			}				
			
		}
		else
			back_sign->place_signdull("MENU",SCREEN_WIDTH -110,SCREEN_HEIGHT -50);

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


			//CheckAllJointsForces(false,300.f,2000.f);
		// flip to the new screen
		if( SDL_Flip(TheWorld-> screen ) == -1 )
		{
			exit(-1);
		}
	  
		//Cap the frame rate
		if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
		{
			//SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
		}

	
		
        // Float up the Baloons by giving them force opposite to gravity
		for(i=0;i<TheWorld->nBalloons;i++)
		{
			if(!baloon[i].disable_motion)
			{
				if(!baloon[i].is_poped)
					dBodyAddForce (baloon[i].body, 0, 0, TheWorld->balloon_lift_force );
			}
		}


		// Capture the end time of the cycle
		time_2 = GetTime();


		// Run a world ODE simulation cycle
		simWorld();	


	}
	
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
//	delete []Box;
	delete game_time_sign;
	delete child_saved_sign;
	delete help_sign;
	delete help_screen_OK;
	delete help_screen_text;

    // We have exited -- Destry the world and all in it
	destroy_world();

	
	TRACE_OUT
	return ret;

}
