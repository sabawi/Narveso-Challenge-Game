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

#include "box.h"
using namespace std;

//extern  my_world *TheWorld;


Box::Box()
{
    
	TRACE_IN
	//Initialize the offsets
	x = 400;
	y = SCREEN_HEIGHT - BOX_HEIGHT;
	
	o_x=0;
	o_y=0;

	//The surfaces
	box = NULL;

	//inhiret the worls 
	world = TheWorld->world ;
	
	space = TheWorld->space ;
	
	contactgroup = TheWorld->contactgroup ;
	
	ground = TheWorld->ground ;
	ceiling = TheWorld->ceiling ;
	leftwall = TheWorld->leftwall;
	rightwall = TheWorld->rightwall;
		
	BoxContacts = dJointGroupCreate (0); 

	body = dBodyCreate (world);
		
	//dMassSetBox (&mass,0.1,1.0,BOX_WIDTH,BOX_HEIGHT);
	//dBodySetMass (body,&mass);
    nBodyNumber =0;
	geom = dCreateBox (space,(dReal)100.0,(dReal)BOX_WIDTH,(dReal)BOX_HEIGHT);
	dGeomSetCategoryBits (geom, BOX_CATEGORY);

	p=NULL;	
	dGeomSetBody (geom,body);
	//dBodyDisable (body);
	
	//setvelocity(0,0);

	if((p=AddBody(&nBodyNumber,body,BOX_TYPE,geom))==NULL)
		TRACE_STRING("FATAL ERROR: AddBody failed. BOX"); 
	else
	{
		p->contacts = BoxContacts;
		p->bBeingMoved = false;
		p->nJoints = 0;
	}
	
	
	load_images(); 
	dGeomSetData (geom, (void *)p);

    TRACE_STRING("Created box. Body #%d",nBodyNumber);
	TRACE_OUT
}

Box::~Box()
{
	TRACE_IN

	TRACE_OUT
}

void Box::setposition(float X, float Y)
{
	TRACE_IN

	dGeomSetPosition (geom,OBJECT_DEPTH,X, Y);	
	
	x= (int) X-BOX_WIDTH/2+TheWorld->bg_x;
	y= (int) SCREEN_HEIGHT - (int) Y - BOX_HEIGHT/2-TheWorld->bg_y;
	TRACE_OUT
}

void Box::setvelocity(float xV, float yV)
{
	TRACE_IN
	//dBodySetLinearVel  (geom, 0, xV, yV);
	TRACE_OUT
}

void Box::getvelocity(float *xV, float *yV)
{
	TRACE_IN
	//const dReal *pos = dBodyGetLinearVel(body);
	//*xV = pos[1];
	//*yV = pos[2];
	TRACE_OUT
}
void Box::set_force_on_box(float fx, float fy, float desX, float desY)
{
	TRACE_IN
	//dBodyAddForce(body,0, 10.0,10.0);
	//cout<<"adding force "<<endl;
	TRACE_OUT
}

void Box::move()
{
	TRACE_IN

	TRACE_OUT
	
}

void Box::show()
{
	TRACE_IN
	const dReal *pos;
	
	pos = dGeomGetPosition ( geom );

	//x= (int) pos[1]-BOX_WIDTH/2+TheWorld->bg_x;
	x= o_x+ TheWorld->bg_x;
	//y= (int) SCREEN_HEIGHT - (int) pos[2] - BOX_HEIGHT/2-TheWorld->bg_y;
	y= o_y - TheWorld->bg_y;
	//cout<<"Geom: X="<<pos[1]<<" Y="<<pos[2]<<endl;
	
	apply_surface( x, y, box , TheWorld->screen );
	TRACE_OUT
}

void Box::stop()
{
	TRACE_IN

	TRACE_OUT
}
bool Box::is_under_mouse(int X, int Y)
{
	TRACE_IN
	int topY, botY, leftX, rightX;
   
 
	leftX = x ;
	rightX = x + BOX_WIDTH;
	topY = y;
	botY = y+BOX_HEIGHT;

	if ( (X>=leftX && X<=rightX) && (Y<=botY && Y>= topY) )
	{
		return true;
		TRACE_OUT
	}
	else
	{ 
		return false;
		TRACE_OUT
	}
}

void Box::init_center(int nX, int nY)
{

	TRACE_IN
	setposition((float) nX, (float)nY);
	TRACE_OUT
	
}

bool Box::load_images()
{
	TRACE_IN
	load_files( ".\\images\\box1.bmp", &box);
	//If there was a problem in loading the spheret
	if( box == NULL )
	{
		TRACE_OUT
		return false;
	}
	TRACE_OUT
    return true;
	
}
