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

#include <windows.h>

using namespace std;
my_world *TheWorld=NULL;
my_fonts *MyFonts=NULL;
GameResult_t GameResults[10];
int nCurrentGame=0;
bool bTrace = false;
int nTrace_indent =0;
SDL_Surface *icon ;

#ifdef _DEBUG
bool bFullScreen = false;
#else
bool bFullScreen = true;
#endif

int SCREEN_WIDTH ;
int SCREEN_HEIGHT ;

bool init_world()
{
	TRACE_IN

	if(!dInitODE2 (0))
		return false;

	// create world
	TheWorld->world = dWorldCreate ();
	
	TheWorld->space = dHashSpaceCreate (0);
	
	TheWorld->contactgroup = dJointGroupCreate (0);

	dJointGroupEmpty (TheWorld->contactgroup);


	//InitSimParms();

	
	//dWorldSetContactSurfaceLayer (TheWorld->world,0.0001);
	
	// Init ODE objects and their link list
	TheWorld->nBodyCount = 0;
	TheWorld->pBlist = NULL;

	// Init world background
	TheWorld->bg_x=0;
	TheWorld->bg_y=0;
	
	TheWorld->MyCursor = init_system_cursor(&arrow[0]);
	SDL_SetCursor(TheWorld->MyCursor);

	if(!set_background((std::string) ".\\images\\bg1.bmp"))
	{
		TRACE_STRING("Unable to load or update background image");
		cout<<"Fatel Error : Set Background failed to load. Exiting program"<<endl;
		
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
	
void destroy_world()
{
	TRACE_IN
	// Destroy the world
	if(TheWorld!=NULL)
	{
		dJointGroupEmpty (TheWorld->contactgroup);
		dJointGroupDestroy (TheWorld->contactgroup);
		
		
		dSpaceDestroy (TheWorld->space);

		dWorldDestroy (TheWorld->world);
	}
	
	dCloseODE();
	
	TRACE_OUT
}
PTBlist AddBody(unsigned long *nbodynumber, dBodyID body, int nBodyType, dGeomID geom)
{
	TRACE_IN
	unsigned int i =0;
	PTBlist p = (PTBlist)malloc(sizeof(Blist));
	if(p==NULL) return p;

    p->body = body;
	p->geom = geom;
	p->nBodyType = nBodyType;
	p->nJoints = 0;
	p->links_surface[0]=p->links_surface[1]=p->links_surface[2]=p->links_surface[3]=NULL;
	
	TheWorld->nBodyCount++;
    *nbodynumber = p->nBodyNum = TheWorld->nBodyCount;

	p->pNext = p->pPrev = NULL;

	if(TheWorld->pBlist==NULL)
	{
		TheWorld->pBlist = p;
	}
	else
	{
		p->pNext = TheWorld->pBlist;
		TheWorld->pBlist->pPrev = p;
		TheWorld->pBlist = p;
	}
	TRACE_OUT
	return p;
}

bool DeleteBody(PTBlist &p)
{
	TRACE_IN
	PTBlist ptmp = TheWorld->pBlist;

	if(p==NULL) return false;
	while(ptmp!=NULL) 
	{
		if(ptmp == p)
		{
			if(ptmp == TheWorld->pBlist)
				TheWorld->pBlist = ptmp->pNext;

			if(ptmp->pNext!=NULL)
				ptmp->pNext->pPrev = ptmp->pPrev;
			if(ptmp->pPrev!=NULL)
				ptmp->pPrev->pNext = ptmp->pNext;

 
			free(ptmp);
			p = NULL;
			TheWorld->nBodyCount--;
			return true;	
		}
		else
			ptmp = ptmp->pNext;
	}
	TRACE_OUT
	return false;
}

void CreateLinkShadow(PTBlist pB)
{
	bTrace = true;
	TRACE_IN
	const dReal *pos;
	if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1))
	{
		if(pB->nJoints>=MAX_JOINTS) return ;
		pos = dGeomGetPosition(pB->geom);
		
		TRACE_STRING("Should see a line");
		TRACE_STRING("Body at x=%d, y=%d",(int)pos[1]+TheWorld->bg_x,(int)(SCREEN_HEIGHT - BALOON_HEIGHT/2 -pos[2]-TheWorld->bg_y));
		TRACE_STRING("Line at x=%d, y=%d",(int)pos[1]+TheWorld->bg_x,(int)(SCREEN_HEIGHT + BALOON_HEIGHT/2 -pos[2]-TheWorld->bg_y));
		

		apply_surface((int)pos[1]+TheWorld->bg_x, (int)(SCREEN_HEIGHT + BALOON_HEIGHT/2 -pos[2]-TheWorld->bg_y), pB->links_surface[0], TheWorld->screen );

		//pB->TmpLinks[pB->nJoints]=dCreateBox (space,(dReal)1.0,(dReal)30,(dReal)50);
	}
	
	TRACE_OUT
	bTrace = false;

}

int OptionsMenu(int &difficulty)
{
	int ret = MAINMENU;
	bool bQuit=false;

	Sign *option1_sign = new Sign;
	Sign *option2_sign = new Sign;
	Sign *option3_sign = new Sign;
	Sign *quit_sign = new Sign;
	Timer fps;

	option1_sign->SetSignDim(200,50);
	option2_sign->SetSignDim(200,50);
	option3_sign->SetSignDim(200,50);
	quit_sign->SetSignDim(200,50);

	while(!bQuit)
	{
		SDL_PollEvent( &TheWorld->event );
		fps.start();

		if(difficulty==1)
				option1_sign->place_signbright(">",SCREEN_WIDTH/2 -150,SCREEN_HEIGHT/2 - 200);
		if(option1_sign->is_under_mouse())
		{

			option1_sign->place_signbright("EASY",SCREEN_WIDTH/2 -100,SCREEN_HEIGHT/2 - 200);
			if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1))
			{
				difficulty = 1;
				bQuit=true;
			}
		}
		else
			option1_sign->place_signdull("EASY",SCREEN_WIDTH/2 -100,SCREEN_HEIGHT/2 - 200);
		
		if(difficulty==2)
				option2_sign->place_signbright(">",SCREEN_WIDTH/2 -150,SCREEN_HEIGHT/2 - 150);
		if(option2_sign->is_under_mouse())
		{
			option2_sign->place_signbright("MEDIUM",SCREEN_WIDTH/2 -100,SCREEN_HEIGHT/2 - 150);
			if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1))
			{
				difficulty = 2;
				bQuit=true;
			}				
			
		}
		else
			option2_sign->place_signdull("MEDIUM",SCREEN_WIDTH/2 -100,SCREEN_HEIGHT/2 -150);

		if(difficulty==3)
				option3_sign->place_signbright(">",SCREEN_WIDTH/2 -150,SCREEN_HEIGHT/2 - 100);
		if(option3_sign->is_under_mouse())
		{
			option3_sign->place_signbright("HARD",SCREEN_WIDTH/2 -100,SCREEN_HEIGHT/2 - 100);
			if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1))
			{
				difficulty = 3;
				bQuit=true;
			}				
			
		}
		else
			option3_sign->place_signdull("HARD",SCREEN_WIDTH/2 -100,SCREEN_HEIGHT/2 -100);

		if(quit_sign->is_under_mouse())
		{
			quit_sign->place_signbright("BACK",SCREEN_WIDTH/2 -100,SCREEN_HEIGHT/2 );
			if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1))
			{
				bQuit=true;
			}				
			
		}
		else
			quit_sign->place_signdull("BACK",SCREEN_WIDTH/2 -100,SCREEN_HEIGHT/2 );
				
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
	}

	delete option1_sign;
	delete option2_sign;
	delete option3_sign;
	delete quit_sign;

	return ret;

}


int MainMenu(int difficulty)
{
	int ret;
	bool bQuit=false;

	Sign *game1_sign = new Sign;
	Sign *game2_sign = new Sign;
	Sign *game3_sign = new Sign;
	Sign *options_sign = new Sign;
	Sign *quit_sign = new Sign;
	Timer fps;

	Load_bg_music(".\\sounds\\bg_music2.ogg");
	play_bg_music();

	game1_sign->SetSignDim(200,50);
	game2_sign->SetSignDim(200,50);
	game3_sign->SetSignDim(200,50);
	options_sign->SetSignDim(200,50);
	quit_sign->SetSignDim(200,50);

	while(!bQuit)
	{
		SDL_PollEvent( &TheWorld->event );
		fps.start();
		
		// secret key into new game
		if(TheWorld->event.type == SDL_KEYUP && TheWorld->event.key.keysym.sym == SDLK_n)
		{
			ret = 999;
			bQuit=true;
		}

		if(game1_sign->is_under_mouse())
		{
			game1_sign->place_signbright("BOOT CAMP",SCREEN_WIDTH/2 -100,SCREEN_HEIGHT/2 - 200);
			if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1))
			{
				ret = GAME1;
				bQuit=true;
			}
		}
		else
			game1_sign->place_signdull("BOOT CAMP",SCREEN_WIDTH/2 -100,SCREEN_HEIGHT/2 - 200);
		
		if(game2_sign->is_under_mouse())
		{
			game2_sign->place_signbright("BLUE SKY",SCREEN_WIDTH/2 -100,SCREEN_HEIGHT/2 - 150);
			if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1))
			{
				ret = GAME2;
				bQuit=true;
			}				
			
		}
		else
			game2_sign->place_signdull("BLUE SKY",SCREEN_WIDTH/2 -100,SCREEN_HEIGHT/2 -150);

		
		if(game3_sign->is_under_mouse())
		{
			game3_sign->place_signbright("SUICIDAL BRIDGE",SCREEN_WIDTH/2 -100,SCREEN_HEIGHT/2 - 100);
			if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1))
			{
				ret = GAME3;
				bQuit=true;
			}				
			
		}
		else
			game3_sign->place_signdull("SUICIDAL BRIDGE",SCREEN_WIDTH/2 -100,SCREEN_HEIGHT/2 -100);

		if(options_sign->is_under_mouse())
		{
			switch (difficulty)
			{
			case 1:
				options_sign->place_signbright("OPTIONS ->EASY",SCREEN_WIDTH/2 -100,SCREEN_HEIGHT/2 + 50);
				break;
			case 2:
				options_sign->place_signbright("OPTIONS ->MEDIUM",SCREEN_WIDTH/2 -100,SCREEN_HEIGHT/2 + 50);
				break;
			case 3:
				options_sign->place_signbright("OPTIONS ->HARD",SCREEN_WIDTH/2 -100,SCREEN_HEIGHT/2 + 50);
				break;
			}
			
			if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1))
			{
				ret = OPTIONS;
				bQuit=true;
			}				
			
		}
		else
		{
			switch (difficulty)
			{
			case 1:
				options_sign->place_signdull("OPTIONS ->EASY",SCREEN_WIDTH/2 -100,SCREEN_HEIGHT/2 +50);
				break;
			case 2:
				options_sign->place_signdull("OPTIONS ->MEDIUM",SCREEN_WIDTH/2 -100,SCREEN_HEIGHT/2 +50);
				break;
			case 3:
				options_sign->place_signdull("OPTIONS ->HARD",SCREEN_WIDTH/2 -100,SCREEN_HEIGHT/2 +50);
				break;
			}
		}

		if(quit_sign->is_under_mouse())
		{
			quit_sign->place_signbright("QUIT",SCREEN_WIDTH/2 -100,SCREEN_HEIGHT/2  +150);
			if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1))
			{
				ret = QUIT;
				bQuit=true;
			}				
			
		}
		else
			quit_sign->place_signdull("QUIT",SCREEN_WIDTH/2 -100,SCREEN_HEIGHT/2 +150);
				
		// Quit if hit the escape key
		if(TheWorld->event.type == SDL_KEYUP && TheWorld->event.key.keysym.sym == SDLK_ESCAPE)
		{
			ret = QUIT;
			bQuit=true;
		}

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
	}
	
	stop_bg_music();
	delete game1_sign;
	delete game2_sign;
	delete game3_sign;
	delete options_sign;
	delete quit_sign;

	return ret;

}


//==================================================================================
//   main()
//
//==================================================================================
int main(int argc, char *argv[])
{
	TRACE_IN
	int ret = 0;
	int nNext = 0;
	bool bQuit = false;
	int difficulty = 1;
	bool bSkipIntro = false;
	
	if((TheWorld = (my_world *) malloc(sizeof(my_world)))==NULL)
	{
		cout<<"ERROR: malloc() failed to allocate TheWorld"<<endl;
		return false;
	}
	ZeroMemory(TheWorld,sizeof(my_world));

	if((MyFonts = (my_fonts *) malloc(sizeof(my_fonts)))==NULL)
	{
		cout<<"ERROR: malloc() failed to allocate fonts"<<endl;
		return false;
	}
	ZeroMemory(MyFonts,sizeof(my_fonts));

	// initialise SDL subsystem
	if(!init_SDL())
		return false;

	// Load font array
	MyFonts->nMaxFonts = 30;	
	for(int i=0;i<MyFonts->nMaxFonts;i++)
	{
		MyFonts->font[i] = TTF_OpenFontIndex(".\\images\\Xoxoxa.ttf",31+i , 0);
		if(MyFonts->font[i] == NULL)
		{
			TRACE_STRING("Could not Load font from file at TTF_OpenFontIndex() error:%s", SDL_GetError());
			return false;
		}
	}
	
	ZeroMemory(TheWorld,sizeof(my_world));
	// initialise my SDL stuff
	if(!init_mySDL())
		return false;
	//nNext = GAME1;
	while(!bQuit)
	{				
		//clear screen
		SDL_FillRect(TheWorld->screen, NULL, SDL_MapRGB(TheWorld->screen->format, 0, 0, 0)); 
		init_audio();

		switch (nNext)
		{
			case MAINMENU:
				// Show Menu
				nNext = MainMenu(difficulty);
				break;
			case GAME1:
				nCurrentGame=GAME1;
				ZeroMemory(&GameResults[GAME1],sizeof(GameResult_t));	
				nNext = Game1(difficulty, bSkipIntro);
				break;
			case GAME2:
				nCurrentGame=GAME2;
				ZeroMemory(&GameResults[GAME2],sizeof(GameResult_t));	
				nNext = Game2(difficulty,bSkipIntro);
				break;
			case GAME3:
				nCurrentGame=GAME3;
				ZeroMemory(&GameResults[GAME3],sizeof(GameResult_t));	
				nNext = Game3(difficulty,bSkipIntro);
				break;
			case 999:
				nCurrentGame=9;
				ZeroMemory(&GameResults[9],sizeof(GameResult_t));	
				nNext = GGAME(difficulty);				
				break;
			case OPTIONS:
				nNext = OptionsMenu(difficulty);
				break;
			case QUIT: 
				bQuit = true;
				break;
			default:
				break;
		}		

		if(nNext == nCurrentGame)
			bSkipIntro = true;
		else
			bSkipIntro = false;
				
		close_audio();

	}
	//Quit SDL		
	SDL_clean_up();

	for(int i=0;i<MyFonts->nMaxFonts;i++)
		TTF_CloseFont(MyFonts->font[i]);

	if(MyFonts!=NULL)
		free(MyFonts);

	TTF_Quit();
	SDL_Quit();		

	// Free the main gloabl memmory 
	if(TheWorld!=NULL)
		free(TheWorld);

	TRACE_OUT
	return 0;
}
