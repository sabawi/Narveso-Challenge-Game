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




bool init_game3_world(int world_type)
{
	TRACE_IN

	double dBaseWorldStepSize =0.0043f;
	double dBaseWorldCFM = 0.0005f;
	double dBaseWorldERP = 0.90f;
	double dBaseWorldLinearDamping = 0.0f;
	double dBaseWorldAngDamping =0.0f;
	double dBaseWorldMaxAngSpeed = 5000.0f;

	double dBaseWorldHingedParamCFM		= 0.000001f;
	double dBaseWorldHingedParamBounce	= 0.001f;
	double dBaseWorldHingedParamStopERP	= 0.90f;
	double dBaseWorldHingedParamStopCFM	= 0.0000001f;

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

		TheWorld->nSpheres				= 16;
		TheWorld->nBalloons				= 4;
		TheWorld->nBoxes				= 0 ;
		TheWorld->nEndineers			= 0;
		TheWorld->nWalkers				= 5;
		TheWorld->nShips				= 1 ;
		TheWorld->nWinScore				= 4;
		TheWorld->balloon_lift_force	= -1.0 * TheWorld->world_gravity * TheWorld->nWinScore *  300.0f ;
		TheWorld->nWinBalloons			= 4;
		TheWorld->dWinXVelocity			= 100;
		TheWorld->EscapeVelocity		= -1000	;
		TheWorld->nJumpToShipDistance	=	450; // mm

		break;
	case 2: //medium 
		// ODE world parameters
		TheWorld->world_step_size		=	dBaseWorldStepSize+0.0003;
		TheWorld->world_gravity			=	-9800.0f; // millimeter per second
		TheWorld->world_ang_damping		=	dBaseWorldAngDamping;
		TheWorld->world_linear_damping	=	dBaseWorldLinearDamping;
		TheWorld->world_ERP				=	dBaseWorldERP;
		TheWorld->world_CFM				=	dBaseWorldCFM +0.00018; // decreese for stability 0.0005, increase for instability 0.005
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

		TheWorld->nSpheres				= 19;
		TheWorld->nBalloons				= 4;
		TheWorld->nBoxes				= 0 ;
		TheWorld->nEndineers			= 0;
		TheWorld->nWalkers				= 5;
		TheWorld->nShips				= 1 ;
		TheWorld->nWinScore				= 4;
		TheWorld->balloon_lift_force	= -1.0 * TheWorld->world_gravity * TheWorld->nWinScore *  300.0f  ;
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
		TheWorld->world_CFM				=	dBaseWorldCFM +0.00050; // decreese for stability 0.0005, increase for instability 0.005
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

		TheWorld->nSpheres				= 21;
		TheWorld->nBalloons				= 4;
		TheWorld->nBoxes				= 0 ;
		TheWorld->nEndineers			= 0;
		TheWorld->nWalkers				= 5;
		TheWorld->nShips				= 1 ;
		TheWorld->nWinScore				= 4;
		TheWorld->balloon_lift_force	= -1.0 * TheWorld->world_gravity * TheWorld->nWinScore *  300.0f ;
		TheWorld->nWinBalloons			= 4;
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

	if(!set_background((std::string) ".\\images\\bg3.bmp"))
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
	//TheWorld->ground = dCreatePlane (TheWorld->space,0,0,1,0);
	
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

void CreateCloudSpheres(Sphere1Leg *sphere, int &nSphereCount, int xOrigin, int yOrigin, int nOffset, int difficulty)
{

	int xOffset = nOffset;
	int yOffset = nOffset;

	int x0=xOrigin, y0=yOrigin + yOffset, 
		x1=xOrigin + xOffset, y1=yOrigin+yOffset, 
		x2=xOrigin, y2=yOrigin,
		x3=xOrigin + xOffset, y3=yOrigin,
		x4= x2-200, y4 = y2 -200,
		x5= x4-200, y5 = y4 -100,
		x6= x5-200, y6 = y5 -100;
	dJointID jnt =0;

	sphere[nSphereCount].Create(x0,y0);
	sphere[nSphereCount+1].Create(x1,y1);
	sphere[nSphereCount+2].Create(x2,y2);
	sphere[nSphereCount+3].Create(x3,y3);
	sphere[nSphereCount+4].Create(x4,y4);
	sphere[nSphereCount+5].Create(x5,y5);
	sphere[nSphereCount+6].Create(x6,y6);

	sphere[nSphereCount].awake = true;
	sphere[nSphereCount+1].awake = true;
	sphere[nSphereCount+2].awake = true;
	sphere[nSphereCount+3].awake = true;
	sphere[nSphereCount+4].awake = true;
	sphere[nSphereCount+5].awake = true;
	sphere[nSphereCount+6].awake = true;

	PTBlist BP0 = (PTBlist)dGeomGetData (sphere[nSphereCount].geom);
	PTBlist BP1 = (PTBlist)dGeomGetData (sphere[nSphereCount+1].geom);
	PTBlist BP2 = (PTBlist)dGeomGetData (sphere[nSphereCount+2].geom);
	PTBlist BP3 = (PTBlist)dGeomGetData (sphere[nSphereCount+3].geom);
	PTBlist BP4 = (PTBlist)dGeomGetData (sphere[nSphereCount+4].geom);
	PTBlist BP5 = (PTBlist)dGeomGetData (sphere[nSphereCount+5].geom);
	PTBlist BP6 = (PTBlist)dGeomGetData (sphere[nSphereCount+6].geom);

	//0
	if(jnt = BallAttach2Bodies(BP0, BP1))
		dJointSetBallAnchor (jnt, OBJECT_DEPTH,x0+SPHERE_WIDTH/2,y0);

	if(jnt = BallAttach2Bodies(BP0, BP3))
		dJointSetBallAnchor (jnt, OBJECT_DEPTH,x0,y0);

	//1
	if(jnt = BallAttach2Bodies(BP1, BP2))
		dJointSetBallAnchor (jnt, OBJECT_DEPTH,x1,y1);

	if(jnt = BallAttach2Bodies(BP1, BP3))
		dJointSetBallAnchor (jnt, OBJECT_DEPTH,x1,y1-SPHERE_HEIGHT);

//	HingeAttach2Bodies(BP1, BP0);
//	dJointSetHingeAnchor (BP1->joints[BP1->nJoints-1], OBJECT_DEPTH,x1-SPHERE_WIDTH/2,y1);

	//2
	if(jnt = BallAttach2Bodies(BP2, BP3))
		dJointSetBallAnchor (jnt, OBJECT_DEPTH,x2+SPHERE_WIDTH/2,y2);

	if(jnt = BallAttach2Bodies(BP2, BP0))
		dJointSetBallAnchor (jnt, OBJECT_DEPTH,x2,y2);

//	HingeAttach2Bodies(BP2, BP1);
//	dJointSetHingeAnchor (BP2->joints[BP2->nJoints-1], OBJECT_DEPTH,x2+SPHERE_WIDTH/2,y2);

	//3
	if(jnt = BallAttach2Bodies(BP3, BP0))
		dJointSetBallAnchor (jnt, OBJECT_DEPTH,x3-SPHERE_WIDTH/2,y3);

//	HingeAttach2Bodies(BP3, BP1);
//	dJointSetHingeAnchor (BP3->joints[BP3->nJoints-1], OBJECT_DEPTH,x3,y3+SPHERE_WIDTH);

	if(jnt = BallAttach2Bodies(BP3, BP2))
		dJointSetBallAnchor (jnt, OBJECT_DEPTH,x3,y3);

	// Attach to the ground
	HingeAttach2Bodies(BP2, NULL);
	HingeAttach2Bodies(BP3, NULL);

	if(jnt = BallAttach2Bodies(BP4, BP2))
		dJointSetBallAnchor (jnt, OBJECT_DEPTH,x4,y4);


	switch(difficulty)
	{
	case 1:
		nSphereCount+=5;
		break;
	case 2:
		if(jnt = HingeAttach2Bodies(BP5, BP4))
			dJointSetHingeAnchor (jnt, OBJECT_DEPTH,x5,y5);
		nSphereCount+=6;
		break;
	case 3:		
		if(jnt = HingeAttach2Bodies(BP5, BP4))
			dJointSetHingeAnchor (jnt, OBJECT_DEPTH,x5,y5);
		if(jnt = HingeAttach2Bodies(BP6, BP5))
			dJointSetHingeAnchor (jnt, OBJECT_DEPTH,x6,y6);
		nSphereCount+=7;
		break;
	default:
		break;
	}

}

void CreateSpheresBase3(Sphere *sphere, int &nSphereCount)
{
	int xOffset = 200;
	int yOffset = 200;	
	int xOrigin = 933 - xOffset;
	int yOrigin = TheWorld->world_height - 1090+SPHERE_HEIGHT/2;


	int x0=xOrigin, y0=yOrigin + yOffset, 
		x1=xOrigin + xOffset, y1=yOrigin+yOffset, 
		x2=xOrigin, y2=yOrigin,
		x3=xOrigin + xOffset, y3=yOrigin;
	dJointID jnt =0;

	sphere[nSphereCount].Create(x0,y0);
	sphere[nSphereCount+1].Create(x1,y1);
	sphere[nSphereCount+2].Create(x2,y2);
	sphere[nSphereCount+3].Create(x3,y3);

	sphere[nSphereCount].awake = true;
	sphere[nSphereCount+1].awake = true;
	sphere[nSphereCount+2].awake = true;
	sphere[nSphereCount+3].awake = true;

	PTBlist BP0 = (PTBlist)dGeomGetData (sphere[nSphereCount].geom);
	PTBlist BP1 = (PTBlist)dGeomGetData (sphere[nSphereCount+1].geom);
	PTBlist BP2 = (PTBlist)dGeomGetData (sphere[nSphereCount+2].geom);
	PTBlist BP3 = (PTBlist)dGeomGetData (sphere[nSphereCount+3].geom);

	//0
	if(jnt = HingeAttach2Bodies(BP0, BP1))
		dJointSetHingeAnchor (jnt, OBJECT_DEPTH,x0+SPHERE_WIDTH/2,y0);

	if(jnt = HingeAttach2Bodies(BP0, BP3))
		dJointSetHingeAnchor (jnt, OBJECT_DEPTH,x0,y0);

	//1
	if(jnt = HingeAttach2Bodies(BP1, BP2))
		dJointSetHingeAnchor (jnt, OBJECT_DEPTH,x1,y1);

	if(jnt = HingeAttach2Bodies(BP1, BP3))
		dJointSetHingeAnchor (jnt, OBJECT_DEPTH,x1,y1-SPHERE_HEIGHT);

//	HingeAttach2Bodies(BP1, BP0);
//	dJointSetHingeAnchor (BP1->joints[BP1->nJoints-1], OBJECT_DEPTH,x1-SPHERE_WIDTH/2,y1);

	//2
	if(jnt = HingeAttach2Bodies(BP2, BP3))
		dJointSetHingeAnchor (jnt, OBJECT_DEPTH,x2+SPHERE_WIDTH/2,y2);

	if(jnt = HingeAttach2Bodies(BP2, BP0))
		dJointSetHingeAnchor (jnt, OBJECT_DEPTH,x2,y2);

//	HingeAttach2Bodies(BP2, BP1);
//	dJointSetHingeAnchor (BP2->joints[BP2->nJoints-1], OBJECT_DEPTH,x2+SPHERE_WIDTH/2,y2);

	//3
//	if(jnt = HingeAttach2Bodies(BP3, BP0))
//		dJointSetHingeAnchor (jnt, OBJECT_DEPTH,x3-SPHERE_WIDTH/2,y3);

//	HingeAttach2Bodies(BP3, BP1);
//	dJointSetHingeAnchor (BP3->joints[BP3->nJoints-1], OBJECT_DEPTH,x3,y3+SPHERE_WIDTH);

//	if(jnt = HingeAttach2Bodies(BP3, BP2))
//		dJointSetHingeAnchor (jnt, OBJECT_DEPTH,x3,y3);

	// Attach to the ground
	HingeAttach2Bodies(BP2, NULL);
	HingeAttach2Bodies(BP3, NULL);
	nSphereCount +=4;
}
//**********************************************************************************
//==================================================================================
//   Game 3 Entry
//
//**********************************************************************************
//==================================================================================
int Game3(int difficulty , bool bSkipIntro)
{
	//bTrace = true;
	TRACE_IN
	int ret = MAINMENU;
	bool quit = false;
	int i =0, nbg =1,row=0,col=0;
	int tmp_x=0, tmp_y=0;
	int nSphereCount = 0;
	SDL_Surface *screen_flip[2] = {NULL,NULL};
	bool bZoomOut = true;
	double dZoomTime = 0; // in seconds
	double dCurrentZoom = 1.0;

	int n1LegSphereCount = 0;
	double dTimedCondition = 1; // In Seconds: to be used for timed calls to functions
	double time_1 =  GetTime();
	double time_2 = time_1;
	double rot_angel =0;
	char *cOption = "";
	switch(difficulty)
	{
	case 1:
		cOption = "E";
		n1LegSphereCount = 8;
		break;
	case 2:
		cOption = "M";
		n1LegSphereCount = 9;
		break;
	case 3:
		cOption = "H";
		n1LegSphereCount = 11;
		break;
	default:
		break;
	}
	
	// Create SDL Timer
	Timer fps;

    if(!init_game3_world(difficulty))
		exit(-1);
	

	Baloon *baloon = new Baloon[TheWorld->nBalloons];
	Sphere *sphere = new Sphere[TheWorld->nSpheres] ;
	Sphere1Leg *sphere_1leg = new Sphere1Leg[n1LegSphereCount];
	Sphere *red_sphere = new Sphere[TheWorld->nEndineers];
	Walker *walker = new Walker[TheWorld->nWalkers];
	Sign *retry_sign = new Sign;
	Sign *back_sign = new Sign;
	Sign *game_time_sign = new Sign;
	Sign *child_saved_sign = new Sign;
	Sign *help_sign = new Sign;
	Sign *help_screen_OK = new Sign;
	Sign *help_screen_text = new Sign;
	StaticObj Cloud;
	StaticObj Invisible1, Invisible2;
	StaticObj *ship = new StaticObj[TheWorld->nShips];
	StaticObj *box = new StaticObj[TheWorld->nBoxes];
	bool bEngineer_have_plan = true;
	help_text Plan; 
	double xForce=0.f;

	retry_sign->SetSignDim(100,60);
	back_sign->SetSignDim(100,60);

	strcpy(Plan.szPlanLines[0],"With two arms each,");
	strcpy(Plan.szPlanLines[1],"one on top of another");
	strcpy(Plan.szPlanLines[2],"we can stand a tower!");
	strcpy(Plan.szPlanLines[3],"Let there be equilateral triangles");
	strcpy(Plan.szPlanLines[4],".. for beauty, for stability");

	//retry_sign->SetSignDim(80,50);
	//back_sign->SetSignDim(80,50);
	//game_time_sign->SetSignDim(80,50);
	//child_saved_sign->SetSignDim(80,50);
	//help_sign->SetSignDim(80,50);
	char strGameTime[100];
	char strSaved[100];
	bool show_balloons = false;

	// Redraw the background in the view window
	//apply_surface( TheWorld->bg_x, SCREEN_HEIGHT-TheWorld->world_height-TheWorld->bg_y, TheWorld->background, TheWorld->screen ); 

	int CloudPos_x= TheWorld->world_width/2+400;
	int CloudPos_y= TheWorld->world_height-400;
	Cloud.CreateCloud(CloudPos_x,CloudPos_y);
	int nOffset = 200;
    int n1LegCount = 0;
	CreateCloudSpheres(sphere_1leg,n1LegCount , CloudPos_x+Cloud.object->w/2-nOffset, CloudPos_y+Cloud.object->h/2+SPHERE_HEIGHT/2, nOffset,difficulty);
	row =0;
	for(i=n1LegCount;i<n1LegSphereCount;i++)
	{
		double x = CloudPos_x-Cloud.object->w/2+100+SPHERE_WIDTH+10+(col*(SPHERE_WIDTH+10));
		double y = CloudPos_y+Cloud.object->h/2+SPHERE_HEIGHT/2 +(row*(SPHERE_HEIGHT+1));
		if(!sphere_1leg[i].Create(x,y))
			exit(-1);	

		// Make rows of 10 spheres 
		if(col<9)
		{ 
			col++;
		}
		else
		{
			col=0;
			row++;
		}
		dBodySetAngularVel(sphere_1leg[i].body,0.f,0.f,0.f);
		dBodySetLinearVel(sphere_1leg[i].body,0.f,0.f,0.f);
	}
	Invisible1.CreateInvisible2(933/2,(TheWorld->world_height-1094)/2, 933,TheWorld->world_height-1094 );
	Invisible2.CreateInvisible2((2574 + (723)/2),((TheWorld->world_height-1045)/2),723,TheWorld->world_height-1053);
	ship->CreateShip((2574 + (723)/2)-100,(TheWorld->world_height-1045)+20);
	
	//********************************
	//****** World Objects Initializations***********
	TheWorld->EngineerPosition[0] = OBJECT_DEPTH;
	TheWorld->EngineerPosition[1] =  1332;
	TheWorld->EngineerPosition[2] =	 TheWorld->background->h - 1201;
	//* BASKETS ******************************
	// position the boxes
	//if(nBoxCount>=4)CreateBasketsBase( basket);	
	/*
	for(i=0;i<TheWorld->nBoxes;i++)
	{
		double x = TheWorld->world_width/2+300 ;
		double y = 10+BOX_HEIGHT/2 + (i*(BOX_HEIGHT+1));
		box[i].CreateBox(x,y);
	}
	*/
		
	//*SPHERES************************
	// Position the spheres
	if(TheWorld->nSpheres>=4)CreateSpheresBase3( sphere, nSphereCount);	
	col=row=0;
	for(i=nSphereCount;i<TheWorld->nSpheres;i++)
	{
		double x = SPHERE_WIDTH +(col*(1+SPHERE_WIDTH));
		double y = TheWorld->world_height-1093+SPHERE_HEIGHT/2 + (row*(SPHERE_HEIGHT+1));
		if(!sphere[i].Create(x,y))
			exit(-1);	

		// Make rows of 10 spheres 
		if(col<5)
		{ 
			col++;
		}
		else
		{
			col=0;
			row++;
		}
	}


	// The Engineers
	/*
	for(i=0;i<TheWorld->nEndineers;i++)
	{
		red_sphere[i].CreateRed(1332+(i*100),TheWorld->background->h-1191);
	}
	*/

	//*BALLOONS**********************
	// position the Baloons 
	for(i=0;i<TheWorld->nBalloons;i++)
	{

		baloon[i].Create();		
		PTBlist pB = (PTBlist) dGeomGetData(baloon[i].geom);
		dVector3 dims;

		const dReal *RockPos = dGeomGetPosition(Invisible2.geom);
		dGeomBoxGetLengths(Invisible2.geom,dims);
		double BalloonPos_x = RockPos[1] + (dims[1]/2)-120 + (i* (baloon[i].baloon->w/3 ));
		double BalloonPos_y = RockPos[2] + dims[2]/2 + 100;		

		baloon[i].anchor_x =RockPos[1] + (dims[1]/2) -120 +(i* (baloon[i].baloon->w/3 ));
		baloon[i].anchor_y =RockPos[2] + dims[2]/2;

		baloon[i].setposition(BalloonPos_x,BalloonPos_y);		
		baloon[i].init_center(int(BalloonPos_x),int(BalloonPos_y));
		rotate_body_x(baloon[i].body, 0.0f);

		dJointID jnt = HingeAttach2Bodies(pB, NULL);
		dJointSetHingeAnchor (jnt, OBJECT_DEPTH,baloon[i].anchor_x, baloon[i].anchor_y);
		dBodySetAngularVel(baloon[i].body,0,0,0);

	}

	//*Children ***********************
	// position the Walkers 
	for(i=0;i<TheWorld->nWalkers;i++)
		walker[i].init_center(100+(rand() % (700-WALKER_WIDTH)),TheWorld->world_height -1094+WALKER_HEIGHT/2);

//************************************************************************************************
//************************************************************************************************
	// Set game start time
	GameResults[nCurrentGame].start_time = GetTime();

//************************************************************************************************
//************************************************************************************************
	// START THE BIG LOOP
	while( quit == false )
	{
		if(!bSkipIntro)
		{
			if(bZoomOut)
				bZoomOut = ZoomOut( dZoomTime, 20.0, dCurrentZoom);

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

				ship->show();
				for(i=0;i<TheWorld->nWalkers;i++)
					if(walker[i].body)
						walker[i].show(dGeomGetPosition(ship->geom));
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

		if((time_2-time_1)>dTimedCondition)
		{
            // Add a call to anything that need to be called on regular intervals 
			//...
			//const dReal *pos1 = dBodyGetPosition(sphere[5].body);
			//const dReal *pos2 = dBodyGetPosition(Ship.body);
			//double dist = sqrt( pow((pos1[1]-pos2[1]),2)+pow((pos1[2]-pos2[2]),2));
			//dBodyAddForce(sphere[5].body,0,50000 * (pos2[1]-pos1[1]),50000* (pos2[2]-pos1[2]));
			//rot_angel = rot_angel -(360/60);
			//if(rot_angel <= -360) rot_angel = 0;
			//xForce = -300000000;
			if(bZoomOut)
				dZoomTime+=1.0;
			else
				dZoomTime =0.0;

			time_1 = time_2 = GetTime();
		}
			
		//Scroll if the mouse at any enge of the view
		AdjustViewToWorld();

		//Handle The_World.events for the spheres
		for(i=0;i<TheWorld->nSpheres ;i++)
		{ 
			if(sphere[i].body)
			{
				PTBlist pB = (PTBlist) dBodyGetData(sphere[i].body);
				if(pB->nJoints==0)
					sphere[i].handle_input(TheWorld->nSpheres);
			}
		}

		for(i=0;i<n1LegSphereCount ;i++)
		{ 
			if(sphere_1leg[i].body)
			{
				PTBlist pB = (PTBlist) dBodyGetData(sphere_1leg[i].body);
				if(pB->nJoints==0)
					sphere_1leg[i].handle_input();
			}
		}

		for(i=0;i<TheWorld->nBoxes;i++)
		{
			box[i].HandleInput();	
		}

		//Handle The_World.events for the Baloons
		for(i=0;i<TheWorld->nBalloons && show_balloons;i++)
		{
			dWorldSetERP(TheWorld->world,0.9f);
			dWorldSetCFM (TheWorld->world,0.0005f);
			if(!baloon[i].is_poped)
				baloon[i].handle_input(TheWorld->nBalloons);
		}
		//If the user has Xed out the window 
		if( TheWorld->event.type == SDL_QUIT )
		{
			//Quit the program 
			quit = true;
		}

		// Redraw the background in the view window
		apply_surface( TheWorld->bg_x, SCREEN_HEIGHT-TheWorld->world_height-TheWorld->bg_y, TheWorld->background, TheWorld->screen ); 

		for(i=0;i<TheWorld->nBoxes ;i++)
		{
			box[i].show();
		}

		for(i=0;i<TheWorld->nSpheres;i++)
		{
			if(sphere[i].body)
			{
				const dReal *spos;
				spos = dBodyGetPosition(sphere[i].body);
				if(spos[2]+sphere[i].sphere->h/2<0)
				{
					GameResults[nCurrentGame].nBallsKilled++;
					sphere[i].CleanUp();
				}
				else
					sphere[i].show();
			}
		}
		for(i=0;i<n1LegSphereCount ;i++)
		{
			if(sphere_1leg[i].body)
			{
				const dReal *spos;
				spos = dBodyGetPosition(sphere_1leg[i].body);
				if(spos[2]+sphere_1leg[i].sphere->h/2<0)
				{
					sphere_1leg[i].CleanUp();	
					GameResults[nCurrentGame].nBallsKilled++;
				}
				else
				{
					sphere_1leg[i].show();
				}
				if((GameResults[nCurrentGame].nBallsSaved >= TheWorld->nWinScore) 
						&& 
						sphere_1leg[i].SphereContacts)
					dJointGroupEmpty(sphere_1leg[i].SphereContacts);
			}
		}

		Cloud.show();

		for(i=0;i<TheWorld->nShips;i++)
		{
			ship[i].show();

		}

		for(i=0;i<TheWorld->nEndineers;i++)
		{
			red_sphere[i].show();
		}

		// Show the Baloons on the screen
		for(i=0;i<TheWorld->nBalloons && show_balloons;i++)
		{
			// show Baloon bodies
			if(!baloon[i].is_poped)
				baloon[i].show();
		}
		
		// position the walkers 
		for(i=0;i<TheWorld->nWalkers;i++)
		{
			walker[i].show(dGeomGetPosition(ship->geom));
		}

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

		sprintf(strSaved,"Death Toll %d",GameResults[nCurrentGame].nBallsKilled);
		if(GameResults[nCurrentGame].nBallsKilled>0)
			child_saved_sign->place_signcolor(strSaved,SCREEN_WIDTH -300,230,36,SDL_MapRGB(TheWorld->screen->format, 0,0,255));
		else
			child_saved_sign->place_signbright(strSaved,SCREEN_WIDTH -300,230);

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

		if(red_sphere[0].is_under_mouse())
		{
			if(bEngineer_have_plan)
				help_sign->place_signcolor("I've GOT A PLAN",red_sphere[0].get_x()-80,red_sphere[0].get_y()-80,36,SDL_MapRGB(TheWorld->screen->format, 255,255,255));
			else
				help_sign->place_signcolor("I'M STUMPED!!",red_sphere[0].get_x()-80,red_sphere[0].get_y()-80,36,SDL_MapRGB(TheWorld->screen->format, 255,255,255));

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
			help_screen_text->place_signcolor(Plan.szPlanLines[0],20,80,24,SDL_MapRGB(TheWorld->screen->format, 255,255,255));
			help_screen_text->place_signcolor(Plan.szPlanLines[1],20+10,80+30,24,SDL_MapRGB(TheWorld->screen->format, 255,255,255));
			help_screen_text->place_signcolor(Plan.szPlanLines[2],20+20,80+60,24,SDL_MapRGB(TheWorld->screen->format, 255,255,255));
			help_screen_text->place_signcolor(Plan.szPlanLines[3],20+30,80+100,24,SDL_MapRGB(TheWorld->screen->format, 255,255,255));
			help_screen_text->place_signcolor(Plan.szPlanLines[4],20+40,80+130,24,SDL_MapRGB(TheWorld->screen->format, 255,255,255));

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

		// flip to the new screen
		if( SDL_Flip(TheWorld-> screen ) == -1 )
		{
			exit(-1);
		}

	  
		//Cap the frame rate
		if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
		{
			SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
		}

		
        // Float up the Baloons by giving them force opposite to gravity
		for(i=0;i<TheWorld->nBalloons;i++)
		{
			if(!baloon[i].disable_motion)
			{
				if(!baloon[i].is_poped)
					dBodyAddForce (baloon[i].body, 0, 0,TheWorld->balloon_lift_force );
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

	for(i=0;i<n1LegSphereCount;i++)
		sphere_1leg[i].CleanUp();

	for(i=0;i<TheWorld->nWalkers;i++)
		walker[i].CleanUp();

	retry_sign->CleanUp();
	back_sign->CleanUp();

	for(i=0;i<TheWorld->nBoxes;i++)
		box[i].CleanUp();

	for(i=0;i<TheWorld->nEndineers;i++)
		red_sphere[i].CleanUp();

	for(i=0;i<TheWorld->nShips;i++)
		ship[i].CleanUp();

	game_time_sign->CleanUp();

	child_saved_sign->CleanUp();

	help_sign->CleanUp();
	help_screen_OK->CleanUp();
	help_screen_text->CleanUp();

	//for(i=0;i<nBoxCount;i++)
	//	basket[i].CleanUp();

	// delete the objects
	delete [] sphere;
	delete [] sphere_1leg;
	delete [] baloon;
	delete [] walker;
	delete retry_sign;
	delete back_sign;
	delete []box;
	delete []red_sphere;
	delete []ship;
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
