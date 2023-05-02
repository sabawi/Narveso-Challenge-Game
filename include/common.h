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
#ifndef _WCOMMON_H
#define _WCOMMON_H	1

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>

#include <ode/ode.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include <assert.h>
#include <string>
#include <cstdlib>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#ifdef USE_gfx
#include <SDL_framerate.h>
#include <SDL_gfxPrimitives.h>

#include <SDL_gfxPrimitives_font.h>
#include <SDL_imageFilter.h>
#include <SDL_rotozoom.h>
#include "ode_world.h"
#endif

#ifdef WIN32
#include <SDL_syswm.h>
#endif // WIN32


#define SPHERE_TYPE 1
#define BOX_TYPE	2
#define BALOON_TYPE 3
#define BALOON_LINK_TYPE 4
#define BASKET_TYPE 5
#define WALKER_TYPE 6
#define SHIP_TYPE 7


// GEOMs categories
const unsigned long BALOON_CATEGORY= 0x0001;
const unsigned long HARDBALL_CATEGORY= 0x0002;
const unsigned long FIXED_CATEGORY= 0x0003;
const unsigned long BASKET_CATEGORY= 0x0004;
const unsigned long BOX_CATEGORY= 0x0005;
const unsigned long WALKER_CATEGORY= 0x0006;
const unsigned long SIGN_CATEGORY= 0x0007;
const unsigned long CLOUD_CATEGORY= 0x0008;
const unsigned long SHIP_CATEGORY= 0x0009;
const unsigned long YELLOWBALL_CATEGORY= 0x000A;

const unsigned long MAX_JOINTS  = 8;


//The surfaces
// Switching to valiable window size for potability
extern int SCREEN_WIDTH ;
extern int SCREEN_HEIGHT ;
const int SCREEN_BPP = 32;

const int MAX_SPHERE_AWAKE = 3;
const int MAX_BALOON_AWAKE = 3;
const int MAX_WALKER_AWAKE = 3;

//The frame rate
const int FRAMES_PER_SECOND = 30;

//The dimensions of the spher 
const int SPHERE_WIDTH = 59;
const int SPHERE_HEIGHT = 59;

extern int BALOON_WIDTH ;
extern int BALOON_HEIGHT ;

const int LINK_LENGTH = 50;

const double BASKET_WIDTH = 80.0f;
const double BASKET_HEIGHT = 80.0f;
const double BASKET_DEPTH = 80.0f;

const int WALKER_WIDTH = 40;
const int WALKER_HEIGHT = 40;

const int SIGN_WIDTH = 60;
const int SIGN_HEIGHT = 40;

const double OBJECT_DEPTH = 10.0f;

//The dimensions of the box 
const int BOX_WIDTH = 300;
const int BOX_HEIGHT = 50;
const int BOX_DEPTH = 300;


//Total particles
const int TOTAL_PARTICLES = 20;

const double PI=3.14159265;

// Menu
const int QUIT	   = -1;
const int MAINMENU = 0;
const int GAME1    = 1;
const int GAME2    = 2;
const int GAME3    = 3;
const int GAME4    = 4;
const int GAME5    = 5;
const int OPTIONS  = 100;



struct timeval {
        long    tv_sec;         /* seconds */
        long    tv_usec;        /* and microseconds */
};


struct Stressed
{
	double value;
	dBodyID body;
	dJointID joint;
};

struct GlobalRotoZoom
{
	bool Enable;
	double Zoom;
	double RotAngle;
};

struct Blist
{
	int nBodyNum;
	int nBodyType;
	int nJoints;
	dBodyID body;
	dGeomID geom;
	dJointGroupID contacts;
    dJointID joints[MAX_JOINTS];
	bool joint_to_delete[MAX_JOINTS];
	dBodyID TmpLinks[MAX_JOINTS];
	dJointFeedback JFB[MAX_JOINTS];
	SDL_Surface *links_surface[MAX_JOINTS];
	bool bBeingMoved;
	Blist *pNext;
	Blist *pPrev;
	struct 
	{
		dReal BoxJointPos[3];
		dBodyID toBody;
		dJointID Joint;
		bool isUsed;
	} BoxJoints[3];
};

typedef struct Blist *PTBlist;

typedef struct
{
	unsigned long nBodyCount;
	dWorldID world;
	dSpaceID space;
	dJointGroupID contactgroup;

		
	dGeomID ground, ceiling;
	dGeomID rightwall,leftwall;
	
	//The surfaces
	SDL_Surface *background ;
	SDL_Surface *screen ;
	std::string background_file;

	// screen attributes
	int bg_x;
	int bg_y;
	int world_width;
	int world_height;

	SDL_Event event;

	// game music
	Mix_Music *bg_music;
	bool bg_musicPlaying;
	
	SDL_Rect Rec1, Rec2;
	SDL_Cursor *MyCursor;

	bool MouseLocked;

	// ODE world parameters
	dReal world_step_size;
	dReal world_gravity;
	dReal world_ang_damping;
	dReal world_linear_damping;
	dReal world_ERP;
	dReal world_CFM;
	dReal world_max_angular_speed;
	void (*fnc_nearCallback)(void *, dGeomID , dGeomID );  
	int world_auto_disable;
	// Hinge Parameters
	dReal HingedParamCFM;
	dReal HingedParamBounce;
	dReal HingedParamStopERP;
	dReal HingedParamStopCFM ;

	// Objects parameters
	double sphere_density;
	double balloon_density;
	double box_density;
	double balloon_lift_force;	
	int nJumpToShipDistance;
	int SearchRadius;

	bool overlay_help_screen_on;
	SDL_Surface *help_screen;
	double help_flash_time;
	dReal EngineerPosition[3];
	Stressed FX;
	Stressed FY;
	Stressed FZ;
	Stressed TX;
	Stressed TY;
	Stressed TZ;
	GlobalRotoZoom rotozoom;

	// game objects;
	int nSpheres;
	int nBalloons;
	int nBoxes;
	int nEndineers;
	int nWalkers;
	int nShips;
	int nWinScore;
	int nWinBalloons;
	double dWinXVelocity;
	double EscapeVelocity;


	PTBlist pBlist ;
		
} my_world, *p_my_world;

typedef struct 
{
	int nMaxFonts;     // should be initilized to <=50 always
	TTF_Font* font[50];

} my_fonts;

typedef struct
{
	int nNumOfLines;
	char szPlanLines[10][300];

} help_text;

typedef struct 
{
	//Parallelogram
	int x1,y1,x2,y2,x3,y3,x4,y4;
} Paral;

typedef struct 
{
	bool bGameOver;
	bool bWon;
	int nGameStage;
	int nBallsSaved;
	int nBallsKilled;
	int nBallonsAttached;
	double start_time;
	double end_time;

} GameResult_t;

extern int nCurrentGame;

extern p_my_world TheWorld;
extern my_fonts *MyFonts;
extern bool bTrace;
extern bool bFullScreen;
extern int nTrace_indent;
extern SDL_Surface *icon ;
extern GameResult_t GameResults[10];


PTBlist AddBody(unsigned long *, dBodyID , int , dGeomID );
bool DeleteBody(PTBlist &);
void CreateLinkShadow(PTBlist );
void ForceTraceString (char * format, ...);
dJointID HingeAttach2Bodies(PTBlist , PTBlist );
dJointID BallAttach2Bodies(PTBlist , PTBlist );
dJointID SliderAttach2Bodies(PTBlist , PTBlist );
dJointID FixedAttach2Bodies(PTBlist , PTBlist );
dJointID PistonAttach2Bodies(PTBlist , PTBlist );
dGeomID GetClosestGeom(dGeomID , int , int );
dGeomID GetClosestGeomUnconnected(dGeomID , int , int );
dGeomID GetClosestGeomUnconnectedExcluding(dGeomID , int , int ,dGeomID );
dGeomID GetClosestGeomUnconnectedExcluding2(dGeomID , int , int ,dGeomID , dGeomID );
dGeomID Get2ClosestGeomUnconnected(dGeomID , int ,int , int , dGeomID &o1, dGeomID &o2);
dGeomID GetClosestGeomWithJointExcluding(dGeomID , int , int , dGeomID );
dBodyID GetFurthestAttached(dBodyID , int , dJointID *);
dGeomID GetClosest5GeomUnconnected(dGeomID MyGeom, int nCat1, int nCat2, int WithInDistance, dGeomID (&CloseGeom)[5]);
void ZeroJointFeedBack(dJointID , dJointFeedback * );
void destroy_world(void);
void get_victor(float x0,float y0,float x1,float y1,float *vx,float *vy);
void nearCallback (void *, dGeomID , dGeomID );
void simWorld (void);
void InitSimParms(void);
void UpdateBodyJointForces(PTBlist pB, int joint);
int Game1(int,bool);
int Game2(int,bool);
int Game3(int,bool);
int GGAME(int difficulty);
void RecordHighestJointForces(PTBlist pB,dBodyID b2, int nJ);
bool ZoomOut( double dZoomTime, double dZoomDuration, double  &dCurrentZoom);


void AdjustViewToWorld(void);

#define DISTANCE(P1,P2) sqrt(pow((double)(P2[1]-P1[1]),2)+pow((double)(P2[2]-P1[2]),2))

//Define Trace MACROS
#define TRACE_IN { int i=0;\
	if(bTrace) \
	{ \
		nTrace_indent++; \
		for(i=0;i<=nTrace_indent;i++) printf("..");\
		printf ( ">>%s:Func: %s:LINE %d.\n", __TIME__, __FUNCTION__, __LINE__); }\
	} 	 


#define TRACE_OUT { int i=0;\
{\
	if(bTrace) \
	{ \
		for(i=0;i<=nTrace_indent;i++)printf("..");\
		printf ( "<<%s:Func: %s:LINE %d.\n", __TIME__,__FUNCTION__,  __LINE__); }\
		nTrace_indent--;\
	}\
}
#define TRACE_STRING TraceString
#define TRACE_STRING_FORCED ForceTraceString
/* XPM */
static const char *arrow[] = {
	/* width height num_colors chars_per_pixel */
	"    32    32        3            1",
	/* colors */
	"X c #000000",
	". c #ffffff",
	"  c None",
	/* pixels */
	"         .XXXXXXXXXX.           ",
	"      .XXXXXXXXXXXXXXXXX.       ",
	"    .XXXXXXXXXXXXXXXXXXXXX.     ",
	"  .XXXXXXXXXXXXXXXXXXXXXXXXX.   ",
	".XXXXXXXXXXXXXXXXXXXXXXXXXXXXX. ",
	".XXXXXX.....XXXXXXXX......XXXXX.",
	".XXXX...XX....XXXX....XX...XXXX.",
	".XXXXX...X...XXXXXX...X...XXXXX.",
	".XXXXXXX...XXXXXXXXXX...XXXXXXX.",
	".XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX.",
	".XXXXXX....XXXXXXXXX....XXXXXXX.",
	".XXXXXX....XXXXXXXXX....XXXXXXX.",
	".XXXXXXX...XXXXXXXXX...XXXXXXXX.",
	".XXXXXXXX...XXXXXXX...XXXXXXXXX.",
	".XXXXXXXXX..XXXXXXX...XXXXXXXXX.",
	".XXXXXXXXXX...XXX...XXXXXXXXXXX.",
	".XXXXXXXXXXX.......XXXXXXXXXXXX.",
	".XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX.",
	".XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX.",
	"..XXXXXXXXXXXXXXXXXXXXXXXXXXXX..",
	"  .XXXXXXXXXXXXXXXXXXXXXXXXX.   ",
	"     .XXXXXXXXXXXXXXXXXXXX.     ",
	"      .XXXXXXXXXXXXXXXXX.       ",
	"        .XXXXXXXXXXXXXX.        ",
	"           .XXXXXXXXX.          ",
	"             .XXXXX.            ",
	"               .X.              ",
	"                .               ",
	"                                ",
	"                                ",
	"                                ",
	"                                ",
	"16,27"
};


#endif //_COMMON_H
