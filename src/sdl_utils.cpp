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
#include "sdl_utils.h"
#include <SDL_draw.h>
#include <windows.h>
#include < time.h >
#include <ode/ode.h>

using namespace std;

#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif
 
struct timezone 
{
  int  tz_minuteswest; /* minutes W of Greenwich */
  int  tz_dsttime;     /* type of dst correction */
};
 
int gettimeofday(struct timeval *tv, struct timezone *tz)
{
	TRACE_IN
	FILETIME ft;
  unsigned __int64 tmpres = 0;
  static int tzflag;
 
  if (NULL != tv)
  {
    GetSystemTimeAsFileTime(&ft);
 
    tmpres |= ft.dwHighDateTime;
    tmpres <<= 32;
    tmpres |= ft.dwLowDateTime;
 
    /*converting file time to unix epoch*/
    tmpres /= 10;  /*convert into microseconds*/
    tmpres -= DELTA_EPOCH_IN_MICROSECS; 
    tv->tv_sec = (long)(tmpres / 1000000UL);
    tv->tv_usec = (long)(tmpres % 1000000UL);
  }
 
  if (NULL != tz)
  {
    if (!tzflag)
    {
      _tzset();
      tzflag++;
    }

	tz->tz_minuteswest =  _timezone / 60;
    tz->tz_dsttime = _daylight;
  }
 
  TRACE_OUT
  return 0;
}

void ForceTraceString (char * format, ...)
{
	//This trace function does not care if trace is on or note
	// it forces the output of a trace string
	int i=0;
	  for(i=0;i<nTrace_indent;i++) printf("|");
	  va_list args;
	  va_start (args, format);
	  
	  vprintf (format, args);
  
	  va_end (args);
	  printf("\n");
 
}
void TraceString (char * format, ...)
{
  if(bTrace)
  {
	  int i=0;
	  for(i=0;i<nTrace_indent;i++) printf("|");
	  va_list args;
	  va_start (args, format);
	  
	  vprintf (format, args);
  
	  va_end (args);
	  printf("\n");
  }
}



SDL_Cursor *init_system_cursor(const char *image[])
{
	int i, row, col;
	Uint8 data[4*32];
	Uint8 mask[4*32];
	int hot_x, hot_y;

	i = -1;
	for ( row=0; row<32; ++row ) {
		for ( col=0; col<32; ++col ) {
			if ( col % 8 ) {
				data[i] <<= 1;
				mask[i] <<= 1;
			} else {
				++i;
				data[i] = mask[i] = 0;
			}
			switch (image[4+row][col]) {
				case 'X':
					data[i] |= 0x01;
					//k[i] |= 0x01;
					break;
				case '.':
					mask[i] |= 0x01;
					break;
				case ' ':
					break;
			}
		}
	}
	sscanf(image[4+row], "%d,%d", &hot_x, &hot_y);
	return SDL_CreateCursor(data, mask, 32, 32, hot_x, hot_y);
}

void AdjustViewToWorld()
{
	TRACE_IN
	int nScrollRange = (int)((SCREEN_WIDTH+SCREEN_HEIGHT) / 8);
	int nXScrollSpeed = (int)(SCREEN_WIDTH/40) ;
	int nYScrollSpeed = (int)(SCREEN_HEIGHT/40)  ;

	if((TheWorld->event.motion.x<=SCREEN_WIDTH) && (TheWorld->event.motion.x>=SCREEN_WIDTH-nScrollRange))
	{
		//nXScrollSpeed = (int)((double)(SCREEN_WIDTH/50) * ((double)(SCREEN_WIDTH-nScrollRange) / (double)TheWorld->event.motion.x)) ;
		//cout<<"2- nXScrollSpeed = "<<nXScrollSpeed<<endl;
		TheWorld->bg_x -= nXScrollSpeed;
		if( TheWorld->bg_x < SCREEN_WIDTH-TheWorld->background->w ) 
			TheWorld->bg_x += nXScrollSpeed; //we crossed the world boundries -- back out

	}
	if((TheWorld->event.motion.x>=0) && (TheWorld->event.motion.x<=nScrollRange))
	{
		TheWorld->bg_x += nXScrollSpeed;
		if( TheWorld->bg_x >0 ) 
			TheWorld->bg_x -= nXScrollSpeed;

	}
	if ((TheWorld->event.motion.y>=0) && (TheWorld->event.motion.y<=nScrollRange))
	{
		TheWorld->bg_y -=nYScrollSpeed;
		if(TheWorld->bg_y<SCREEN_HEIGHT-TheWorld->world_height)
			TheWorld->bg_y +=nYScrollSpeed;

	}
	if((TheWorld->event.motion.y<=SCREEN_HEIGHT) && (TheWorld->event.motion.y>=SCREEN_HEIGHT-nScrollRange))
	{
		TheWorld->bg_y +=nYScrollSpeed;
		if(TheWorld->bg_y>0)
			TheWorld->bg_y -=nYScrollSpeed;
	}

	SDL_PumpEvents();
	TRACE_OUT;
}


void bg_musicFinished()
{
	TRACE_IN
	//Music is done!
	TheWorld->bg_musicPlaying = false;
	TRACE_OUT
}

double GetTime()
{
	TRACE_IN
	timeval tim;
	gettimeofday(&tim, NULL);

	TRACE_OUT
	return tim.tv_sec+(tim.tv_usec/1000000.0);	
}

char * GetTimeDiffString2(double t1, double t2, char (&str)[100])
{
	int hours,min,sec;
	hours = int((t2-t1)/(3600));
	min = int(((t2-t1) - (hours * 3600))/60);
	sec = int( (t2-t1) - (hours * 3600) - (min * 60));
	sprintf(str, "%2d Hr., %2d Min., and %d Sec.)",hours,min,sec);
	return str;
}

char * GetTimeDiffString(double t1, double t2, char (&str)[100])
{
	int hours,min,sec;
	hours = int((t2-t1)/(3600));
	min = int(((t2-t1) - (hours * 3600))/60);
	sec = int( (t2-t1) - (hours * 3600) - (min * 60));
	sprintf(str, "Time %2d-%2d-%d",hours,min,sec);
	return str;
}

bool Load_bg_music(std::string music_file)
{
	TRACE_IN
	//Load our OGG file from disk
	TRACE_STRING("Loading background music %s ...",music_file);
	TheWorld->bg_music = Mix_LoadMUS(music_file.c_str());
	if(TheWorld->bg_music == NULL) 
	{
		TRACE_STRING("Unable to load sound file: %s" ,Mix_GetError());
		TRACE_OUT
		return false;
	}
	else
	{
		
		TRACE_STRING ("..Successful");
		TRACE_OUT
		return true;
	}
	
}

bool play_bg_music()
{
	TRACE_IN
	//Play music FOREVER (setting second parameter to -1
	if(Mix_PlayMusic(TheWorld->bg_music, -1) == -1) 
	{
		TRACE_STRING("Error: Unable to play sound file: %s\n", Mix_GetError());
		return false;
	}
	else
	{
		//The music is playing!
		TheWorld->bg_musicPlaying = 1;
		return true;
	}
	TRACE_OUT
}		

void stop_bg_music()
{
	TRACE_IN
	//Release the memory allocated to our music
	Mix_HaltMusic();
	Mix_FreeMusic(TheWorld->bg_music);
	TRACE_OUT
}

bool init_audio()
{
	//Initialize SDL_mixer
	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
	{
		TRACE_OUT
		return false;    
	}
	Mix_AllocateChannels(10);        
	// my background music 
	TheWorld->bg_music = NULL;
	TheWorld->bg_musicPlaying = false;
	
	//Make sure that the bg_musicFinished() function is called when the music stops playing
	Mix_HookMusicFinished(bg_musicFinished);
	return true;
}

bool init_mySDL()
{
    
    TRACE_IN
	
	//Initialize all SDL subsystems
	if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
		TRACE_OUT
		return false;
	}

	SDL_WM_SetIcon(icon=IMG_Load( ".\\images\\narveso.png" ),NULL);
  
	const SDL_VideoInfo* pVideoInfo = SDL_GetVideoInfo();
	
	if(!pVideoInfo->hw_available && !pVideoInfo->wm_available)
	{
		TRACE_STRING("Fatal Error: SDL Init failed: No HW or Window Manager Available.");
		
		TRACE_OUT
		return false;
	}
	


	//Set up the screen
	if(bFullScreen)
	{
		SCREEN_WIDTH = pVideoInfo->current_w;
		SCREEN_HEIGHT = pVideoInfo->current_h;
		TheWorld->screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE|SDL_FULLSCREEN);
		TRACE_STRING("WIDTH = %d, HEIGHT = %d",SCREEN_WIDTH,SCREEN_HEIGHT);
	}
	else
	{	
		SCREEN_WIDTH = 1024;
		SCREEN_HEIGHT = 960;	
		TheWorld->screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
		TRACE_STRING("WIDTH = %d, HEIGHT = %d",SCREEN_WIDTH,SCREEN_HEIGHT);
	}

	//If there was an error in setting up the screen
	if( TheWorld->screen == NULL )
	{
		TRACE_OUT
		return false;
	}


	//Set the window caption
	SDL_WM_SetCaption( "Narveso Game by Al Sabawi", NULL );

	//Seed random
	srand( SDL_GetTicks() );
    

	
    //init_system_cursor( &arrow[0] );
	//If everything initialized fine

#ifdef WIN32
	if(!bFullScreen)
	{
		SDL_SysWMinfo i;
		SDL_VERSION( &i.version );
		if ( SDL_GetWMInfo ( &i) ) 
		{
			HWND hwnd = i.window;
			SetWindowPos( hwnd, HWND_TOP, 50, 50, SCREEN_WIDTH, SCREEN_HEIGHT, 0 );
		}
	}
#endif // WIN32

	TRACE_OUT
	return true;
}

bool init_SDL(void)
{
	TRACE_IN
	bool ret = false;

	TRACE_STRING("Initializing SDL.");
	  
	/* Initializes Audio and the CDROM, add SDL_INIT_VIDEO for Video */  
	if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_CDROM)< 0) 
	{
		TRACE_STRING("Could not initialize SDL:%s", SDL_GetError());
		SDL_Quit();
		ret = false;
	} 
	else 
	{
		TRACE_STRING( "Audio & CDROM initialized correctly" );

		/* Trying to read number of CD devices on system */
		TRACE_STRING( "Drives available :%d",SDL_CDNumDrives());
		for(int i=0; i < SDL_CDNumDrives(); ++i) 
		{
			TRACE_STRING("Drive %d %s\\",i,SDL_CDName(i));
		}
		ret = true;
		
	}

	if(ret)
	{
		if(TTF_Init()==-1)
		{
			TRACE_STRING("Could not initialize TTF in TTF_init() error:%s", SDL_GetError());
			ret = false;
		}
		else
		{
			TRACE_STRING("TTF initialized corectly  TTF_init() returned code:%s", SDL_GetError());
			ret = true;
		}
	}


	return ret;
	TRACE_OUT
      		
}

bool load_background(void)
{
	TRACE_IN
	if(load_files( ".\\images\\bg1.png", &TheWorld->background ) == false)
	{
		TRACE_OUT
		return false;
	}
	else
	{
		TRACE_OUT
		return true;
	}
}

bool load_files(std::string szfilename, SDL_Surface * *surface )
{
	TRACE_IN
    //Load the file
	
	TRACE_STRING("Loading file  : %s",szfilename.c_str());
	
	*surface = load_image( szfilename );
	if(*surface == NULL)
	{
		TRACE_STRING("...Failed : ");
		TRACE_OUT
		return false;
	}
	TRACE_STRING("...successful! ");
    
	if(is_file_extension(szfilename.c_str(),"bmp"))
	{
		//Set alpha  <== this function needs to move to inside load_image() for better performance
		SDL_SetAlpha( *surface, SDL_SRCALPHA | SDL_RLEACCEL, 255 );
	}
	//If everything loaded fine
	TRACE_OUT
	return true;
}


SDL_Surface *load_image( std::string filename ) 
{
    TRACE_IN
	//The image that's loaded
    SDL_Surface* loadedImage = NULL;
    
    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;
    
    //Load the image using SDL_image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if(loadedImage != NULL )
    {
		if(!is_file_extension(filename.c_str(),"bmp"))
			return loadedImage;

        //Create an optimized bmp image
        optimizedImage = SDL_DisplayFormat( loadedImage );
        
        //Free the old image
        SDL_FreeSurface( loadedImage );
        //If the image was optimized just fine
        if( optimizedImage != NULL )
        {
            //Map the color key
           Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF );
		   //Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 0, 0, 0 );
           //Set all pixels of color R 0, G 0xFF, B 0xFF to be transparent
           SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey );
        }
		else
		{
			TRACE_STRING("Error in IMG_Load(): %s ",IMG_GetError ());
		}

    }
	else
	{
		TRACE_STRING("Error in IMG_Load(): %s ",IMG_GetError ());
	}
    
    //Return the optimized image
	TRACE_OUT
    return optimizedImage;
}


void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
    TRACE_IN

	//Make a temporary rectangle to hold the offsets
	SDL_Rect offset;

    //Give the offsets to the rectangle
	offset.x = x;
	offset.y = y;

	if(TheWorld->rotozoom.Enable)
	{
		SDL_Rect offset2;
		SDL_Surface * tmp_surf = rotozoomSurface(source,TheWorld->rotozoom.RotAngle,TheWorld->rotozoom.Zoom,1);

		offset2.x = (Sint16)((double( x )*  TheWorld->rotozoom.Zoom ) + SCREEN_WIDTH/2  - (TheWorld->background->w * TheWorld->rotozoom.Zoom)/2);
		offset2.y = (Sint16)((double( y )*  TheWorld->rotozoom.Zoom ));// - SCREEN_WIDTH); //(TheWorld->background->h * TheWorld->rotozoom.Zoom)/2);		

		//Blit the surface
		SDL_BlitSurface( tmp_surf, NULL, destination, &offset2 );
		SDL_FreeSurface(tmp_surf);
	}
	else
	{
		//Blit the surface
		SDL_BlitSurface( source, NULL, destination, &offset );
	}
	TRACE_OUT
}

bool set_background(std::string bg_file)
{
	TRACE_IN
	
	TheWorld->background_file = bg_file;
	SDL_FreeSurface( TheWorld->background );
	if( (TheWorld->background= load_image( TheWorld->background_file )) ==NULL)
	{
		TRACE_STRING("Error: Loading background file %s" ,TheWorld->background_file);
		TRACE_OUT
		return false;
	}
	else
	{
		SDL_SetAlpha(TheWorld->background, SDL_SRCALPHA | SDL_RLEACCEL, 192 );

		TRACE_OUT
		return true;
	} 
	
}

void close_audio()
{
	//Free the sound effects 
	stop_bg_music();

	//Quit SDL_mixer 
	Mix_CloseAudio(); 

}
void SDL_clean_up()
{
	TRACE_IN

	SDL_FreeCursor(TheWorld->MyCursor);

	SDL_FreeSurface( TheWorld->background );
	SDL_FreeSurface( TheWorld->screen);
	SDL_FreeSurface(icon);
	TRACE_OUT


}

void get_victor(float x0,float y0,float x1,float y1,float *vx,float *vy)
{
	TRACE_IN
	*vx = x1 - x0;
	*vy = y1 - y0;
	TRACE_OUT
	
}
void ConvertSphereGeomToScreen(dReal gX, dReal gY, int *sX, int *sY , int r)
{
	TRACE_IN
	//*sX= (int)gX-r;
	*sX =(int)gX;
	//*sY= SCREEN_HEIGHT - r -(int)gY;
	*sY= SCREEN_HEIGHT -(int)gY;
	TRACE_OUT
}


bool is_close(int X1,int Y1,int X2,int Y2,int close_dist)
{
	
	TRACE_IN
	int dist = (int) sqrt(pow((double)(X2-X1),2)+pow((double)(Y2-Y1),2));
	if(dist<close_dist) 
	{
		TRACE_OUT
		return true;
	}
	else 
	{
		TRACE_OUT
		return false;

	}

}
bool IsWithinDistance(const dReal *pos1, const dReal *pos2, double distance)
{
	double dTestDist = sqrt(pow((pos2[1]-pos1[1]),2)+pow((pos2[2]-pos1[2]),2));

	if(dTestDist< distance)
		return true;
	else
		return false;
}
void DrawTo2Closest(dGeomID o1, int WithInDistance, dGeomID &co1, dGeomID &co2)
{
	const dReal *pos1,*pos2, *pos3;
	int x1,y1,x2,y2,x3,y3;
	int nCat1;
	PTBlist pB1 = NULL;
	co1 = co2 = 0;

	
	pB1 = (PTBlist)dGeomGetData( o1);  // Retrieve Body1 data

	if(pB1->bBeingMoved)
	{
		nCat1 = dGeomGetCategoryBits(o1);  // Category for body1
		Get2ClosestGeomUnconnected(o1,HARDBALL_CATEGORY, HARDBALL_CATEGORY, WithInDistance,co1,co2);
		if((co1 == 0) || (co2 ==0))
				return ;
		//cout<<"in DrawTo2Closest()"<<endl;
		pos1 = dGeomGetPosition(o1);
		pos2 = dGeomGetPosition(co1);	
		pos3 = dGeomGetPosition(co2);

		x1= (int)pos1[1]+TheWorld->bg_x;
		y1= SCREEN_HEIGHT  -(int)pos1[2] -TheWorld->bg_y;

		x2= (int)pos2[1]+TheWorld->bg_x;
		y2= SCREEN_HEIGHT  -(int)pos2[2]-TheWorld->bg_y;


		x3= (int)pos3[1]+TheWorld->bg_x;
		y3= SCREEN_HEIGHT  -(int)pos3[2]-TheWorld->bg_y;


		Uint32 c_white = SDL_MapRGB(TheWorld->screen->format, 255,255,255);

		//sDrawLine(x1, y1, x2, y2 , c_white );
		//sDrawLine(x1, y1, x3, y3 , c_white );
		sDrawLine2(x1, y1, x2, y2 ,  255,255,255, 50);
		sDrawLine2(x1, y1, x3, y3 ,  255,255,255, 50);
		

	}
	return ;
}



dGeomID DrawToClosest(dGeomID o1, int WithInDistance)
{
	const dReal *pos1,*pos2;
	int x1,y1,x2,y2;
	int nCat1;
	PTBlist pB1 = NULL;
	dGeomID o_closest = 0;


	pB1 = (PTBlist)dGeomGetData( o1);  // Retrieve Body1 data

	if(pB1->bBeingMoved)
	{
		nCat1 = dGeomGetCategoryBits(o1);  // Category for body1

		// If Baloon , go to Ships first 
		switch (nCat1)
		{
			
			case BALOON_CATEGORY:
			
				// I am a Balloon, my order of search: SHIP, HARDBALL, BOX
				o_closest = GetClosestGeomUnconnected(o1,SHIP_CATEGORY, WithInDistance);
				if(o_closest == 0)
					o_closest = GetClosestGeomUnconnected(o1,HARDBALL_CATEGORY, WithInDistance);
				if(o_closest == 0)
					o_closest = GetClosestGeomUnconnected(o1,BOX_CATEGORY, WithInDistance);
				break;
			case YELLOWBALL_CATEGORY:
				o_closest = GetClosestGeomUnconnected(o1,YELLOWBALL_CATEGORY, WithInDistance);
				if(o_closest == 0)
					o_closest = GetClosestGeomUnconnected(o1,HARDBALL_CATEGORY, WithInDistance);
				break;
			case HARDBALL_CATEGORY:
				// I am a HARDBALL, my order of search is HARDBALL, BOX
				o_closest = GetClosestGeomUnconnected(o1,HARDBALL_CATEGORY, WithInDistance);
				if(o_closest == 0)
					o_closest = GetClosestGeomUnconnected(o1,BOX_CATEGORY, WithInDistance);
				break;
			case BOX_CATEGORY:
				// I am a HARDBALL, my order of search is BOX, HARDBALL
				o_closest = GetClosestGeomUnconnected(o1,BOX_CATEGORY, WithInDistance);
				if(o_closest == 0)
					o_closest = GetClosestGeomUnconnected(o1,HARDBALL_CATEGORY, WithInDistance);
				break;
			default:
				break;

		}
		// if we still found nothing, return 0
		if(o_closest == 0) 
				return o_closest;

		pos1 = dGeomGetPosition(o1);
		pos2 = dGeomGetPosition(o_closest);	
		

		x1= (int)pos1[1]+TheWorld->bg_x;
		y1= SCREEN_HEIGHT -(int)pos1[2] -TheWorld->bg_y;


		x2= (int)pos2[1]+TheWorld->bg_x;
		y2= SCREEN_HEIGHT  -(int)pos2[2]-TheWorld->bg_y;

		sDrawLine2(x1, y1, x2, y2 ,  255,255,255, 50);
		
	}
	return o_closest;
}


dGeomID DrawToClosestAny(dGeomID o1,int nDrawto,int WithInDistance,dGeomID (&CloseGeom)[5] )
{
	const dReal *pos1,*pos2;
	int x1,y1,x2,y2;
	int nCat1;
	PTBlist pB1 = NULL;


	pB1 = (PTBlist)dGeomGetData( o1);  // Retrieve Body1 data

	if(pB1->bBeingMoved)
	{
		nCat1 = dGeomGetCategoryBits(o1);  // Category for body1

		GetClosest5GeomUnconnected(o1, YELLOWBALL_CATEGORY, HARDBALL_CATEGORY, WithInDistance, CloseGeom);
		for(int i=0;i<nDrawto;i++)
		{
			if(CloseGeom[i])
			{
				pos1 = dGeomGetPosition(o1);
				pos2 = dGeomGetPosition(CloseGeom[i]);	
				

				x1= (int)pos1[1]+TheWorld->bg_x;
				y1= SCREEN_HEIGHT -(int)pos1[2] -TheWorld->bg_y;


				x2= (int)pos2[1]+TheWorld->bg_x;
				y2= SCREEN_HEIGHT  -(int)pos2[2]-TheWorld->bg_y;

				sDrawLine2(x1, y1, x2, y2 ,  255,255,255, 50);

			}
		}
	}

	return 0;
}

dGeomID DrawToClosestExcluding2(dGeomID o1, int WithInDistance, dGeomID ox1, dGeomID ox2)
{
	const dReal *pos1,*pos2;
	int x1,y1,x2,y2;
	int nCat1;
	PTBlist pB1 = NULL;
	dGeomID o_closest = 0;

	
	pB1 = (PTBlist)dGeomGetData( o1);  // Retrieve Body1 data

	if(pB1->bBeingMoved)
	{
		nCat1 = dGeomGetCategoryBits(o1);  // Category for body1
		o_closest = GetClosestGeomUnconnectedExcluding2(o1,HARDBALL_CATEGORY, WithInDistance,ox1, ox2);
		if(o_closest == 0) 
				return o_closest;

		pos1 = dGeomGetPosition(o1);
		pos2 = dGeomGetPosition(o_closest);	
		

		x1= (int)pos1[1]+TheWorld->bg_x;
		y1= SCREEN_HEIGHT -(int)pos1[2] -TheWorld->bg_y;


		x2= (int)pos2[1]+TheWorld->bg_x;
		y2= SCREEN_HEIGHT -(int)pos2[2]-TheWorld->bg_y;

		sDrawLine2(x1, y1, x2, y2 ,  255,255,255, 50);
		
		
	}
	return o_closest;
}


dGeomID DrawToClosestExcluding(dGeomID o1, int WithInDistance, dGeomID ox)
{
	const dReal *pos1,*pos2;
	int x1,y1,x2,y2;
	int nCat1;
	PTBlist pB1 = NULL;
	dGeomID o_closest = 0;

	
	pB1 = (PTBlist)dGeomGetData( o1);  // Retrieve Body1 data

	if(pB1->bBeingMoved)
	{
		nCat1 = dGeomGetCategoryBits(o1);  // Category for body1
		o_closest = GetClosestGeomUnconnectedExcluding(o1,HARDBALL_CATEGORY, WithInDistance,ox);
		if(o_closest == 0) 
				return o_closest;

		pos1 = dGeomGetPosition(o1);
		pos2 = dGeomGetPosition(o_closest);	
		

		x1= (int)pos1[1]+TheWorld->bg_x;
		y1= SCREEN_HEIGHT -(int)pos1[2] -TheWorld->bg_y;


		x2= (int)pos2[1]+TheWorld->bg_x;
		y2= SCREEN_HEIGHT -(int)pos2[2]-TheWorld->bg_y;

		sDrawLine2(x1, y1, x2, y2 ,  255,255,255, 50);
		
		
	}
	return o_closest;
}


int m2sx(int mx)
{
	// takes x mouse position and converts it to SDL screen postion
	int sx = TheWorld->bg_x - mx;
	return sx;
}

int m2sy(int my)
{
	// takes y mouse position and return SDL screen position
	int sy = SCREEN_HEIGHT-TheWorld->world_height-TheWorld->bg_y - my;
	return sy;
}
double s2ox(int sx)
{
	// takes x mouse position and converts it to SDL screen postion
	double ox = fabs((double)sx);
	return ox;
}
double s2oy(int sy)
{
	// takes x mouse position and converts it to SDL screen postion
	double oy = (double)(TheWorld->world_height - sy);
	return oy;
}

int o2sx(double ox)
{
	int sx = (int) -ox;
	return sx;
}
int o2sy(double oy)
{

	int sy = (int)oy - TheWorld->world_height;
	return sy;
}

void RotateLine(double sx1, double sy1,double delta_x1, double delta_y1, double theta)
{
	double delta_x2, delta_y2;
	Uint32 color = SDL_MapRGB(TheWorld->screen->format, 255,255,255);

	//sDrawLine((int)sx1,(int)sy1,(int)(sx1+delta_x1),(int)(sy1-delta_y1),color);
    delta_x2=(delta_x1*cos(theta))-(delta_y1* sin(theta));
    delta_y2=(delta_x1*sin(theta))+(delta_y1* cos(theta));
    //sDrawLine((int)sx1,(int)sy1,(int)(sx1+delta_x2),(int)(sy1-delta_y2),color);
	sDrawLine2((int)sx1,(int)sy1,(int)(sx1+delta_x2),(int)(sy1-delta_y2),  255,255,255, 50);
	char str[50];
	double rot_angel = GetAngleOfLine(sx1,sy1,(sx1+delta_x2),(sy1-delta_y2));
	/*
	if(((sy1-delta_y2) - sy1)>0)
	{
		rot_angel-=270;
	}
	else if (((sx1+delta_x2) - sx1)> 0)
		rot_angel-=90;
	else if (((sx1+delta_x2) - sx1)< 0)
		rot_angel-=(270+180);
	*/
	sprintf(str,"OUT %3.2f",rot_angel);
	drawText(str,24,400,400+40,0,0,255,0,0,0);
	sprintf(str,"Diff  %3.2f",theta *180/PI - rot_angel);

	if(theta *180/PI - rot_angel > 0.0001)
		Sleep(1000);

	drawText(str,24,400,400+80,0,0,255,0,0,0);

	int x1 = int(sx1);
	int y1 = int(sy1);
	int x2 = int(sx1+delta_x2);
	int y2 = int(sy1-delta_y2);

	SDL_Surface *link1, *link2,*link3;

		load_files(".\\images\\link.png", &link1);
		double p1[] = {0,x1,y1};
				double p2[] = {0,x2,y2};

				double angle = GetAngleOfLine(double(x1), double(y1),double(x2),double(y2));
				link2 = zoomSurface(link1, 1.0, 10, 1);
				link3 = rotozoomSurface(link2, angle, 1.0, 1);

      // q2
				if((x2-x1)>=0   && (y2-y1)>=0)
				{
					//y1 -=link3->h;
					//x1 +=link3->w;
					sprintf(str,"q2");
					drawText(str,24,x2,y2,0,0,255,0,0,0);
				}
	//q3
				if((x2-x1)>=0   && (y2-y1)<=0)
				{
					y1 -=link3->h;
					//x1 -=link3->w;
					sprintf(str,"q3");
					drawText(str,24,x2,y2,0,0,255,0,0,0);
				}

		//g4
				if((x2-x1)<=0   && (y2-y1)>=0)
				{
					x1 -=link3->w;
					sprintf(str,"q4");
					drawText(str,24,x2,y2,0,0,255,0,0,0);

				}
		// q1
				if((x2-x1)<=0   && (y2-y1)<=0)
				{
					y1 -= link3->h;
					x1 -= link3->w;
					sprintf(str,"q1");
					drawText(str,24,x2,y2,0,0,255,0,0,0);
				}

				apply_surface( x1, y1, link3 , TheWorld->screen );

				sDrawLine2(x1, y1,x1+link3->w,y1,  255,255,255, 50);
				sDrawLine2(x1+link3->w, y1,x1+link3->w,y1 + link3->h,  255,255,255, 50);
				sDrawLine2(x1+link3->w, y1+ link3->h,x1,y1+ link3->h,  255,255,255, 50);
				sDrawLine2(x1, y1+ link3->h,x1,y1,  255,255,255, 50);

				SDL_FreeSurface(link2) ;
				SDL_FreeSurface(link1) ;
				link2 = NULL;

	
}

double GetAngleOfLine(double x1, double y1, double x2, double y2)
{
	double angle = 0;

	if ( x1 == x2 ) 
	{
		if ( y1 > y2 )
			return 0; 
		if ( y1 < y2 )
			return 180.0; 
	}

	if ( y1 == y2 ) 
	{
		if ( x1 < x2 )	
			return 90.0;
		if ( x1 > x2 )	
			return 270.0; 
	}

	angle = atan2( (double)(y2-y1), (double)(x1-x2) ) * (180/PI);
	if(y2<y1)
		angle +=180;

	if((y2 - y1)>0)
		angle-=270;
	else if ((x2 - x1)> 0)
		angle-=90;
	else if ((x2 - x1)< 0)
		angle-=(270+180);

	return (angle);
}

//**************************************************************
// Rotate a line around a center point by an angle theta 
// IN: ref Array of 4 doubles for (x1,y1) to (x2,y2) line
//    center point (cx,cy)
//    Rotation angle theta in Rad
//    pass in 32bit color if you want to add a draw line call
//***************************************************************************
void RotateLine2 (double center[2], double (&line)[4],double theta, Uint32 color)
{
	double length = 0.0;
	double cx_x1,cy_y1,cx_x2,cy_y2;

	cx_x1 = line[0] - center[0];
	cy_y1 = line[1] - center[1];
	cx_x2 = line[2] - center[0];
	cy_y2 = line[3] - center[1];

    line[0]= center[0] - ((int)(cx_x1*cos(theta)))-((int)(cy_y1* sin(theta)));
    line[1]= center[1] - ((int)(cx_x1*sin(theta)))+((int)(cy_y1* cos(theta))); 

    line[2]= center[0] - ((int)(cx_x2*cos(theta)))-((int)(cy_y2* sin(theta)));
    line[3]= center[1] - ((int)(cx_x2*sin(theta)))+((int)(cy_y2* cos(theta))); 
}

// void sDrawLine(int x1, int y1, int x2, int y2, Uint32 color )
// Draw a clipped line in within a box
// Line Cliping using Liang-Barsky algorithm 
// IN : start and end of line from x1,y1 to x2,y2
//      32bit color 
//****************************************************************

void sDrawLine(int x1, int y1, int x2, int y2, Uint32 color )
{
	// using Liang-Barsky line clipping algorithm 
	int i;
	int 	wxmin =1, wymin=1 , wxmax=SCREEN_WIDTH-1 , wymax = SCREEN_HEIGHT-1;
	float u1 = 0.0  , u2 = 1.0 ;

	int  	p1 , q1 , p2 , q2 , p3 , q3 , p4 ,q4 ;
	float r1 , r2 , r3 , r4 ;
	int 	x11 , y11 , x22 , y22 ;
	
	for(i=-2;i<2;i++)
	{
	p1 = -(x2 - x1 ); q1 = x1 - wxmin ;
	p2 = ( x2 - x1 ) ; q2 = wxmax - x1 ;
	p3 = - ( y2 - y1 ) ; q3 = y1 - wymin ;
	p4 = ( y2 - y1  ) ; q4 = wymax - y1 ;
	if( ( ( p1 == 0.0 ) && ( q1 < 0.0 ) ) ||
		( ( p2 == 0.0 ) && ( q2 < 0.0 ) ) ||
		( ( p3 == 0.0 ) && ( q3 < 0.0 ) ) ||
		( ( p4 == 0.0 ) && ( q4 < 0.0 ) ) )
    {
	  //Line is rejected Do nothing

    }
	else
	{
		if( p1 != 0.0 )
		{
			r1 =(float) q1 /p1 ;
			if( p1 < 0 )
				u1 = max(r1 , u1 );
			else
				u2 = min(r1 , u2 );
		}
		if( p2 != 0.0 )
		{
			r2 = (float ) q2 /p2 ;
			if( p2 < 0 )
				u1 = max(r2 , u1 );
			else
				u2 = min(r2 , u2 );

		}
		if( p3 != 0.0 )
		{
			r3 = (float )q3 /p3 ;
			if( p3 < 0 )
				u1 = max(r3 , u1 );
			else
				u2 = min(r3 , u2 );
		}
		if( p4 != 0.0 )
		{
			r4 = (float )q4 /p4 ;
			if( p4 < 0 )
				u1 = max(r4 , u1 );
			else
				u2 = min(r4 , u2 );
		}

		if( u1 > u2 )
		{
			//printf("line rejected\n");
		}
		else
		{
			x11 = x1 + u1 * ( x2 - x1 ) ;
			y11 = y1 + u1 * ( y2 - y1 ) ;

			x22 = x1 + u2 * ( x2  - x1 );
			y22 = y1 + u2 * ( y2 - y1 );

			Draw_Line(TheWorld->screen,x11,y11,x22,y22,color);
		}
	}
		x1=x1+i; y1=y1+i; x2=x2+i; y2=y2+i;
	}
}

void sDrawLine2(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	// using Liang-Barsky line clipping algorithm 
	int i;
	int 	wxmin =1, wymin=1 , wxmax=SCREEN_WIDTH-1 , wymax = SCREEN_HEIGHT-1;
	float u1 = 0.0  , u2 = 1.0 ;
	
	//Draw_Rect(TheWorld->screen,wxmin,wymin,wxmax,wymax,color);

	int  	p1 , q1 , p2 , q2 , p3 , q3 , p4 ,q4 ;
	float r1 , r2 , r3 , r4 ;
	int 	x11 , y11 , x22 , y22 ;
	
	for(i=-2;i<2;i++)
	{
	p1 = -(x2 - x1 ); q1 = x1 - wxmin ;
	p2 = ( x2 - x1 ) ; q2 = wxmax - x1 ;
	p3 = - ( y2 - y1 ) ; q3 = y1 - wymin ;
	p4 = ( y2 - y1  ) ; q4 = wymax - y1 ;
	if( ( ( p1 == 0.0 ) && ( q1 < 0.0 ) ) ||
		( ( p2 == 0.0 ) && ( q2 < 0.0 ) ) ||
		( ( p3 == 0.0 ) && ( q3 < 0.0 ) ) ||
		( ( p4 == 0.0 ) && ( q4 < 0.0 ) ) )
    {
	  //Line is rejected Do nothing

    }
	else
	{
		if( p1 != 0.0 )
		{
			r1 =(float) q1 /p1 ;
			if( p1 < 0 )
				u1 = max(r1 , u1 );
			else
				u2 = min(r1 , u2 );
		}
		if( p2 != 0.0 )
		{
			r2 = (float ) q2 /p2 ;
			if( p2 < 0 )
				u1 = max(r2 , u1 );
			else
				u2 = min(r2 , u2 );

		}
		if( p3 != 0.0 )
		{
			r3 = (float )q3 /p3 ;
			if( p3 < 0 )
				u1 = max(r3 , u1 );
			else
				u2 = min(r3 , u2 );
		}
		if( p4 != 0.0 )
		{
			r4 = (float )q4 /p4 ;
			if( p4 < 0 )
				u1 = max(r4 , u1 );
			else
				u2 = min(r4 , u2 );
		}

		if( u1 > u2 )
		{
			//printf("line rejected\n");
		}
		else
		{
			x11 = x1 + u1 * ( x2 - x1 ) ;
			y11 = y1 + u1 * ( y2 - y1 ) ;

			x22 = x1 + u2 * ( x2  - x1 );
			y22 = y1 + u2 * ( y2 - y1 );

			//Draw_Line(TheWorld->screen,x11,y11,x22,y22,color);
			lineRGBA(TheWorld->screen,x11,y11,x22,y22, r, g, b, a); 
		}
	}
		x1=x1+i; y1=y1+i; x2=x2+i; y2=y2+i;
	}
}

void MatMult (const dReal * pos, const dReal * R, dReal (&res)[4])
{
	int i=0,j=0,k=0;
	dReal mat1[4][3], mat2[4];

	mat1[0][0]=R[0];
	mat1[1][0]=R[4];
	mat1[2][0]=R[8];
	mat1[3][0]=0;

	mat1[0][1]=R[1];
	mat1[1][1]=R[5];
	mat1[2][1]=R[9];
	mat1[3][1]=0;

	mat1[0][2]=R[2];
	mat1[1][2]=R[6];
	mat1[2][2]=R[10];
	mat1[3][2]=0;
  
	mat2[0]=pos[0];
	mat2[1]=pos[1];
	mat2[2]=pos[2];
	mat2[3]=1;
        
	for(i=0;i<4;i++)
    {
		for(j=0;j<3;j++)
        {
            res[i] += mat1[i][j]* mat2[j];
            
        }
        printf("[%d]\n",res[i]);
    }
 
}
void RotateBox (const dReal * center, double w, double h, const dReal * R, dReal (&res)[4][4])
{
	int i=0,j=0,k=0;
	double mat1[4][3];
	double corner[4][4];

	mat1[0][0]=R[0];
	mat1[1][0]=R[4];
	mat1[2][0]=R[8];
	mat1[3][0]=0;

	mat1[0][1]=R[1];
	mat1[1][1]=R[5];
	mat1[2][1]=R[9];
	mat1[3][1]=0;

	mat1[0][2]=R[2];
	mat1[1][2]=R[6];
	mat1[2][2]=R[10];
	mat1[3][2]=0;
    
	corner[0][0] = center[0];
	corner[0][1] = center[1]-(w/2);
	corner[0][2] = center[2]+(h/2);
	corner[0][3] = 1;
      
	corner[1][0] = center[0];
	corner[1][1] = center[1]+(w/2);
	corner[1][2] = center[2]+(h/2);
	corner[1][3] = 1;

	corner[2][0] = center[0];
	corner[2][1] = center[1]+(w/2);
	corner[2][2] = center[2]-(h/2);
	corner[2][3] = 1;

	corner[3][0] = center[0];
	corner[3][1] = center[1]-(w/2);
	corner[3][2] = center[2]-(h/2);
	corner[3][3] = 1;

	for (k=0;k<4;k++)
	{
		MatMult(corner[k],R,res[k]);
	}
 
}

bool is_file_extension(const char *filename, const char * extension)
{
    // This function works with '.' in the extension and without it

    const char* peek = &filename [strlen (filename) - 1]; // start from last char and go backward
	const char* ext = extension;

	if(*ext == '.')
		ext++;

    while (peek >= filename)
    {
        if (*peek == '.')
        {
			if(strcmp(ext, peek+1)==0)				
				return true;
			else
				break;
        }
        peek--;
    }

    return false;
}

bool IntersectLines(	double Ax, double Ay,
						double Bx, double By,
						double Cx, double Cy,
						double Dx, double Dy,
						double *X, double *Y) 
{

  double  distAB, theCos, theSin, newX, ABpos ;

  //  Fail if either line is undefined.
  if (Ax==Bx && Ay==By || Cx==Dx && Cy==Dy) 
	  return false;

  //  (1) Translate the system so that point A is on the origin.
  Bx-=Ax; By-=Ay;
  Cx-=Ax; Cy-=Ay;
  Dx-=Ax; Dy-=Ay;

  //  Discover the length of segment A-B.
  distAB=sqrt((Bx*Bx+By*By));

  //  (2) Rotate the system so that point B is on the positive X axis.
  theCos=Bx/distAB;
  theSin=By/distAB;
  newX=(Cx*theCos+Cy*theSin);
  Cy  =(Cy*theCos-Cx*theSin); Cx=newX;
  newX=(Dx*theCos+Dy*theSin);
  Dy  =(Dy*theCos-Dx*theSin); Dx=newX;

  //  Fail if the lines are parallel.
  if (Cy==Dy) 
	  return false;

  //  (3) Discover the position of the intersection point along line A-B.
  ABpos=(Dx+(Cx-Dx)*Dy/(Dy-Cy));

  //  (4) Apply the discovered position to line A-B in the original coordinate system.
  *X=(Ax+ABpos*theCos);
  *Y=(Ay+ABpos*theSin);

  //  Success.
  return true; 

} 

void drawText(char* string,
              int size,
              int x, int y,
              int fR, int fG, int fB,
              int bR, int bG, int bB)
{
   int font_index = size - 24;

   SDL_Color foregroundColor = { fR, fG, fB };
   SDL_Color backgroundColor = { bR, bG, bB };

   TTF_SetFontStyle(MyFonts->font[font_index], TTF_STYLE_NORMAL);

   SDL_Surface* textSurface = TTF_RenderText_Blended(MyFonts->font[font_index], string, foregroundColor);

   SDL_Rect textLocation = { x, y, 0, 0 };

   SDL_BlitSurface(textSurface, NULL, TheWorld->screen, &textLocation);

   SDL_FreeSurface(textSurface);

}

SDL_Color translate_color(Uint32 int_color)                           

      //Change from an "int color" to an SDL_Color
{
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        SDL_Color color={(int_color & 0x00ff0000)/0x10000,(int_color &
0x0000ff00)/0x100,(int_color & 0x000000ff),0};
    #else
        SDL_Color color={(int_color & 0x000000ff),(int_color &
0x0000ff00)/0x100,(int_color & 0x00ff0000)/0x10000,0};    
    #endif
    return color;
}



void drawText2(char* string,int size,int x, int y,Uint32 Color)
{
	SDL_Surface* textSurface;
	TTF_Font* font;
	SDL_Rect textLocation = { x, y, 0, 0 };
	SDL_Color fg;
	int font_index = size - 24;

	SDL_Color bg = { 0,0,0 };
	font = MyFonts->font[font_index];
	if(font)
	{
		fg = translate_color(Color) ;

		TTF_SetFontStyle(font, TTF_STYLE_NORMAL);

		textSurface = TTF_RenderText_Blended(font, string, fg);
		

		SDL_BlitSurface(textSurface, NULL, TheWorld->screen, &textLocation);

		SDL_FreeSurface(textSurface);

	}
}

void DrawImageFromLinePoints(SDL_Surface *image, int x1, int y1, int x2, int y2)
{
				// ***************************************************
				// **  Draw the links 
				// ***************************************************
				int x1a=x1, y1a=y1;
				SDL_Surface *img2,*img3;

				//sDrawLine( x1, y1, x2, y2 , c_gray );
				double p1[] = {0,x1,y1};
				double p2[] = {0,x2,y2};
				int nHalfHt = image->h/2;
				int nHalfWidht = image->w/2;

				double angle = GetAngleOfLine(double(x1), double(y1),double(x2),double(y2));
				img2 = zoomSurface(image, 1.0, DISTANCE(p1,p2)/image->w, 1);
				img3 = rotozoomSurface(img2, angle, 1.0, 1);
		
				/*
				char str[50];	
				sprintf(str,"unknown");
				bool unknown=true;
				*/
				// Going Clockwise 
				//q1	O1
				//		OO
				if((x2-x1)>=0   && (y2-y1)<=0)
				{
					y1a -=img3->h-nHalfHt;
					x1a -=nHalfWidht;
					//x1a -=link3->w;
					/*
					sprintf(str,"q1");
					drawText(str,24,x1-50,y1-50,0,0,255,0,0,0);
					sDrawLine2(x1, y1, x1-50+30,y1-50+30 , 0,102,102, 50);
					sDrawLine2(x2, y2, x1-50+30,y1-50+30 , 0,102,102, 50);
					unknown=false;
					*/
					
				}
				// q2   OO
				//		O2
				else if((x2-x1)>=0   && (y2-y1)>=0)
				{
					//y1a -=link3->h;
					x1a -=nHalfWidht;
					y1a -=nHalfHt;
					/*
					sprintf(str,"q2");
					drawText(str,24,x1+50,y1-50,0,0,255,0,0,0);
					sDrawLine2(x1, y1, x1+50+30,y1-50+30 , 0,102,102, 50);
					sDrawLine2(x2, y2, x1+50+30,y1-50+30 , 0,102,102, 50);
					unknown=false;
					*/
				}


				//g3	OO
				//		3O
				else if((x2-x1)<0   && (y2-y1)>=0)
				{
					x1a -=img3->w-nHalfWidht;
					y1a -=nHalfHt;
					/*
					sprintf(str,"q3");
					drawText(str,24,x1-50,y1+50,0,0,255,0,0,0);
					sDrawLine2(x1, y1, x1-50+30,y1+50+30 , 0,102,102, 50);
					sDrawLine2(x2, y2, x1-50+30,y1+50+30 , 0,102,102, 50);
					unknown=false;
					*/

				}
				// q4	4O
				//		OO
				else if((x2-x1)<0   && (y2-y1)<=0)
				{

					y1a -= img3->h-nHalfHt;
					x1a -= img3->w-nHalfWidht;
					/*
					sprintf(str,"q4");
					drawText(str,24,x1+50,y1+50,0,0,255,0,0,0);
					sDrawLine2(x1, y1, x1+50+30,y1+50+30 , 0,102,102, 50);
					sDrawLine2(x2, y2, x1+50+30,y1+50+30 , 0,102,102, 50);
					unknown=false;
					*/
				}

				/*
				if(unknown)
				{
					drawText(str,24,x1+100,y1+100,0,0,255,0,0,0);
					sDrawLine2(x1, y1, x1+100+30,y1+100+30 , 0,102,102, 50);
					sDrawLine2(x2, y2, x1+100+30,y1+100+30 , 0,102,102, 50);
				}
				*/

				apply_surface( x1a, y1a, img3 , TheWorld->screen );
//sDrawLine2(x1, y1, x2, y2 , 0,102,102, 50);
				SDL_FreeSurface(img3) ;
				SDL_FreeSurface(img2) ;
				img3 = img2 = NULL;

}


bool ZoomOut( double dZoomTime, double dZoomDuration, double  &dCurrentZoom)
{
	bool bZoomOut = true;
	//TheWorld->bg_y = - TheWorld->background->h + SCREEN_HEIGHT;
	if((dCurrentZoom * TheWorld->background->w <= SCREEN_WIDTH) &&
	   (dCurrentZoom * TheWorld->background->h <= SCREEN_HEIGHT))
	{
		if(dZoomTime>dZoomDuration)
		{
			bZoomOut = false;
			dCurrentZoom = 1.0;
			TheWorld->rotozoom.Enable = false;
			TheWorld->rotozoom.RotAngle = 0;
			TheWorld->rotozoom.Zoom = dCurrentZoom ;
			TheWorld->bg_y = 0;
		}
		else
		{
			TheWorld->bg_x = 0;
			TheWorld->bg_y = - TheWorld->background->h + SCREEN_HEIGHT;
		}
		
	}
	else
	{
		TheWorld->rotozoom.Enable = true;
		TheWorld->rotozoom.RotAngle = 0;
		dCurrentZoom *=0.99;
		TheWorld->rotozoom.Zoom = dCurrentZoom ;
		TheWorld->bg_x = 0;
		TheWorld->bg_y = - TheWorld->background->h + SCREEN_HEIGHT;
		SDL_FillRect(TheWorld->screen, NULL, SDL_MapRGB(TheWorld->screen->format, 0, 0, 0)); 
	}
	return bZoomOut;

}