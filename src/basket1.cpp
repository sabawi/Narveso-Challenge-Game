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
#include <SDL_draw.h>
#include "basket1.h"
using namespace std;


Basket1::Basket1()
{
    
	TRACE_IN
	//Initialize the offsets
	x = 0;
	y = TheWorld->world_height;
	
	//The surfaces
	basket1 = NULL;

	//inhiret the worls 
	world = TheWorld->world ;
	
	space = TheWorld->space ;
	
	BasketContacts = dJointGroupCreate (0); 
	
	ground = TheWorld->ground ;
	ceiling = TheWorld->ceiling ;
	leftwall = TheWorld->leftwall;
	rightwall = TheWorld->rightwall;
		
	body = dBodyCreate (TheWorld->world);


	dMassSetBox (&mass, (dReal)0.1f , BASKET_DEPTH, BASKET_WIDTH, BASKET_HEIGHT);	
	dBodySetMass(body,&mass);	

	geom = dCreateBox ( TheWorld->space, BASKET_DEPTH, BASKET_WIDTH, BASKET_HEIGHT);
	//dBodySetAngularVel( body, 0.0f, 0.0f, 0.0f);
	//dBodySetLinearVel(  body, 0.0f, 0.0f, 0.0f);

	//geom = dCreateSphere (TheWorld->space,(dReal)BALOON_WIDTH/2);
	//dMassSetSphere (&mass,(dReal)0.03f , BALOON_WIDTH/2);

	dGeomSetBody (geom,body);
	dGeomSetCategoryBits (geom, BASKET_CATEGORY);
	nBodyNumber = 0;
	rot_theta = 0.0f;
	//rotate_body_x(body, rot_theta);

	if((p=AddBody(&nBodyNumber,body,BASKET_TYPE,geom))==NULL)
		TRACE_STRING ("FATAL ERROR: AddBody failed. BASKET"); 
	else
	{
		p->TmpLinks[0]		= 0;		

		p->contacts = BasketContacts;
		p->bBeingMoved = false;
		p->nJoints = 0;
	
	}

	dBodySetFiniteRotationMode (body, 1);
	dBodySetFiniteRotationAxis (body, 1, 1, 1);
	//dBodySetAngularDamping (body, 0.1);
	

    dBodySetData (body, (void*)p);
	dGeomSetData (geom, (void *)p);


	
	double dTimeKeeper1 = GetTime();
	double dTimeKeeper2 = GetTime();
	//dRFromAxisAndAngle (dMat , 0, 0, 0, 0);
	//dBodySetRotation(body,dMat);
	//dBodySetAngularDamping (body, 0.5);
	

	//dJointID planeJointID = dJointCreatePlane2D( world, 0);
    //dJointAttach( planeJointID, body, 0 );
	//p=NULL;	

	load_images(); 
	//dGeomSetData (geom, (void *)p);

    TRACE_STRING("Created Basket1. Body #%d",nBodyNumber);
	TRACE_OUT
}

Basket1::~Basket1()
{
	TRACE_IN

	TRACE_OUT
}

void Basket1::setposition(double X, double Y)
{
	TRACE_IN
	dBodySetPosition (body,BASKET_DEPTH/2,X, Y);	
	TRACE_OUT
}

void Basket1::setvelocity(float xV, float yV)
{
	TRACE_IN
	//dBodySetLinearVel  (geom, 0, xV, yV);
	TRACE_OUT
}

void Basket1::getvelocity(float *xV, float *yV)
{
	TRACE_IN

	TRACE_OUT
}
void Basket1::set_force_on_basket1(float fx, float fy, float desX, float desY)
{
	TRACE_IN

	TRACE_OUT
}

void Basket1::move()
{
	TRACE_IN

	TRACE_OUT
	
}

void rotate_body_x(dBodyID body, double phi)
{

	dMatrix3 dMat;
	dQuaternion q;

	dQFromAxisAndAngle (q, 1, 0.0f, 0.0f, -phi);
	//dRFromEulerAngles (dMat, phi, 0, 0);
	dQtoR (q, dMat);
	dBodySetRotation(body,dMat);


}

void reset_YZaccess_drift(dBodyID body)
{
    const dReal     *rot = dBodyGetAngularVel (body);
    const dReal     *quat_ptr;
    dReal           quat[4],
                    quat_len;
    quat_ptr = dBodyGetQuaternion (body);
    quat[0] = quat_ptr[0];
    quat[1] = quat_ptr[1];
    quat[2] = 0; 
    quat[3] = 0; 
    quat_len = sqrt (quat[0] * quat[0] + quat[1] * quat[1]);
    quat[0] /= quat_len;
	quat[1] /= quat_len;
    
    dBodySetQuaternion (body, quat);
	
    dBodySetAngularVel (body,rot[0] ,0, 0 );

}

void get_euler(const dReal * matrix,dReal &kx,dReal &ky,dReal &kz)
{       
   const dReal epsilon=0.0000001;
   if(matrix[2] < 1-epsilon && matrix[2] > -1+epsilon)
   {
      ky=-asin(matrix[2]);
      dReal c=cos(ky);
      kx= atan2(matrix[6]/c,matrix[10]/c);
      kz= atan2(matrix[1]/c,matrix[0]/c);
   } else {       
      kz=0;   
      ky=-atan2(matrix[2],0);
      kx= atan2(-matrix[9],matrix[5]);
   }       
}   

void Basket1::show()
{
	TRACE_IN
	// dReal aabb[6];
	int i,x1, x2, y1, y2;
	const dReal *pos, *pos_link2;
	//const dReal *tq,*av,*lv , *rot;
	dBodyID tmpBody = 0;
	dReal xrot,yrot,zrot;
	// dReal theta;
	//dMatrix3 dMat;
	double center[2];
	//double line1[4], line2[4], line3[4], line4[4];
	//double cx_x1,cy_y1,cx_x2,cy_y2;
	double length = 0.0;	
    //const dReal *quat_ptr; 
    //dReal quat[4], quat_len;
	double line[4];

	PTBlist pB = (PTBlist)dGeomGetData( geom);
	Uint32 color = SDL_MapRGB(TheWorld->screen->format, 255,104,238);

	//dRFromAxisAndAngle (dMat , 0, 0, 1, 90/PI);
	//dRFromEulerAngles (dMat, 0, 0, 0);
	//dBodySetRotation(body,dMat);
	//tq=dBodyGetTorque(body);
	//av=dBodyGetAngularVel(body);
	//lv=dBodyGetLinearVel(body);

	//dBodySetAngularVel  (body, 0, 0, 0);
	//dBodySetLinearVel  (body, 0, 0, 0);
	rotate_body_x(body, rot_theta);
	//dBodySetAngularDamping (body, 1.0f);
	
	reset_YZaccess_drift(body);

	pos = dBodyGetPosition ( body ); // center of basket
	init_center(int(pos[1]), (int)pos[2]); // updated x and y of the baloon
	setposition(pos[1], pos[2]);

	get_euler(dBodyGetRotation(body),xrot,yrot,zrot);

	rot_theta = xrot;
	//rot_theta+=0.1;  // For beautiful rotation for the fun of it !!!!!!

	center[0] = (double) pos[1]+TheWorld->bg_x;
	center[1] = (double) SCREEN_HEIGHT - pos[2]-TheWorld->bg_y;

	apply_surface( int(x+ BASKET_WIDTH/2) , int(y+ BASKET_HEIGHT/2), basket1 , TheWorld->screen );	

	// draw bottom
	x1 = int(center[0] - BASKET_WIDTH/2) ;
	y1 = int(center[1] + BASKET_HEIGHT/2) ;

	x2 = int(center[0] + BASKET_WIDTH/2) ;
	y2 = int(center[1] + BASKET_HEIGHT/2);
	line[0]=x1;
	line[1]=y1;
	line[2]=x2;
	line[3]=y2;

	RotateLine2 ( center, line ,rot_theta, color);

	// draw top
	x1 = int(center[0] - BASKET_WIDTH/2) ;
	y1 = int(center[1] - BASKET_HEIGHT/2) ;

	x2 = int(center[0] + BASKET_WIDTH/2) ;
	y2 = int(center[1] - BASKET_HEIGHT/2);
	line[0]=x1;
	line[1]=y1;
	line[2]=x2;
	line[3]=y2;

	RotateLine2 ( center, line ,rot_theta,color);

	// draw left
	x1 = int(center[0] - BASKET_WIDTH/2) ;
	y1 = int(center[1] - BASKET_HEIGHT/2) ;

	x2 = int(center[0] - BASKET_WIDTH/2) ;
	y2 = int(center[1] + BASKET_HEIGHT/2);
	line[0]=x1;
	line[1]=y1;
	line[2]=x2;
	line[3]=y2;

	RotateLine2 ( center, line ,rot_theta,color);

	// draw right
	x1 = int(center[0] + BASKET_WIDTH/2) ;
	y1 = int(center[1] - BASKET_HEIGHT/2) ;

	x2 = int(center[0] + BASKET_WIDTH/2) ;
	y2 = int(center[1] + BASKET_HEIGHT/2);
	line[0]=x1;
	line[1]=y1;
	line[2]=x2;
	line[3]=y2;

	RotateLine2 ( center, line ,rot_theta,color);

	x1=int(x+BASKET_WIDTH/2); // middle x of the basket
	y1=int(y+BASKET_HEIGHT/2);  // bottom y of the basket

	for(i=0;i<pB->nJoints;i++)
	{
		tmpBody=dJointGetBody (pB->joints[i], 1);

		if(tmpBody && (tmpBody!=body))
		{			
			pos_link2 = dBodyGetPosition(tmpBody); // get center of the linked body
			x2= (int) pos_link2[1]+TheWorld->bg_x;  // update it for world view

			y2= SCREEN_HEIGHT  -(int)pos_link2[2]-TheWorld->bg_y;

			//sDrawLine( x1, y1, x2, y2 , SDL_MapRGB(TheWorld->screen->format, 0,0,0) );
			sDrawLine2(x1, y1, x2, y2 ,  0,0,0, 255);
		}
	}

	dSpaceCollide (TheWorld->space,0,&nearCallback);

	TRACE_OUT
}

void Basket1::stop()
{
	TRACE_IN

	TRACE_OUT
}


void Basket1::init_center(int nX, int nY)
{

	TRACE_IN
	x= int(nX-BASKET_WIDTH/2+TheWorld->bg_x);
	y= int(SCREEN_HEIGHT - BASKET_HEIGHT/2 -nY-TheWorld->bg_y);

	TRACE_OUT
	
}
void Basket1::CleanUp()
{
	PTBlist thisp = (PTBlist)dGeomGetData( geom);	
	dJointGroupEmpty (BasketContacts);
	dJointGroupDestroy (BasketContacts);

	if(thisp!=NULL)
	{
		free(thisp);
		//cout<<"Freed Baskets"<<endl;
	}

}
bool Basket1::load_images()
{
	TRACE_IN
	load_files( ".\\images\\blue.bmp", &basket1);
	  
	//If there was a problem in loading the blue
	if( basket1 == NULL )
	{
		return false;
		cout<<"blue failed to load"<<endl;
	}

	TRACE_OUT
    return true;
	
}

void Basket1::handle_input()
{
	TRACE_IN
    PTBlist thisp = (PTBlist)dGeomGetData( geom);
	//If a key was released
	if( TheWorld->event.type == SDL_KEYDOWN )
	{
	}
	else if( TheWorld->event.type == SDL_KEYUP )
	{

	}
	else if( TheWorld->event.type == SDL_MOUSEBUTTONUP )
	{
		if(is_under_mouse())
		{
			dBodySetForce  (body, 0.0f, 0.0f, 0.0f);
			dBodySetTorque (body, 0.0f, 0.0f, 0.0f);
			//dBodySetLinearVel  (body, 0.0f, 0.0f, 0.0f);
			//dBodySetAngularVel  (body, 0.0f, 0.0f, 0.0f);
			//rotate_body_x(body, 0);
		}

	}
	else if( TheWorld->event.type == SDL_MOUSEBUTTONDOWN )
	{
			
		if(is_under_mouse())
		{

			y=int(TheWorld->event.button.y - BASKET_HEIGHT/2+ TheWorld->bg_y);
			x=int(TheWorld->event.button.x - TheWorld->bg_x);

			dGeomSetPosition (geom,(dReal)BASKET_DEPTH/2,(dReal)x, (dReal) (SCREEN_HEIGHT - BASKET_HEIGHT/2 - y));
			//cout<<"Basket Button Down"<<endl;
			dBodySetForce  (body, 0.0f, 0.0f, 0.0f);
			dBodySetTorque (body, 0.0f, 0.0f, 0.0f);
			//dBodySetLinearVel  (body, 0.0f, 0.0f, 0.0f);
			//dBodySetAngularVel  (body,0.0f, 0.0f, 0.0f);
			thisp->bBeingMoved = true;
			//DrawToClosest(geom, SEARCH_RADIUS);
		}
		else if(thisp->bBeingMoved)
		{
			thisp->bBeingMoved = false;
		}

	}
	else if( TheWorld->event.type == SDL_MOUSEMOTION && (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1)))
	{
		
		if(is_under_mouse())
		{
			//dBodyID CutBody =0;
			//dJointID jointID = 0;
			
			//disable_motion = true;
			//dBodyDisable (body);

			y=int(TheWorld->event.button.y - BASKET_HEIGHT/2+ TheWorld->bg_y);
			x=int(TheWorld->event.button.x- TheWorld->bg_x);
			//cout<<"Basket MOVE Button Down"<<endl;
			dGeomSetPosition (geom,(dReal)BASKET_DEPTH/2,(dReal)x, (dReal)(SCREEN_HEIGHT - BASKET_HEIGHT/2 - y));

			dBodySetForce  (body, 0.0f, 0.0f, 0.0f);
			dBodySetTorque (body, 0.0f, 0.0f, 0.0f);
			//dBodySetLinearVel  (body, 0.0f, 0.0f, 0.0f);
			//dBodySetAngularVel  (body, 0.0f, 0.0f, 0.0f);
			//dBodySetAngularDamping (body, 1.0);

			//rotate_body_x(body, 0);

			thisp->bBeingMoved = true;
			//DrawToClosest(geom, SEARCH_RADIUS);

			// sever the connection if the Baloon is pulled away from any object by 150 world coor units
			//if( ( (CutBody = GetFurthestAttached(body, 150, &jointID))!=0) && (jointID!=0))
			//{
				//dJointAttach(jointID,0,0);
				//if(thisp->nJoints==MAX_JOINTS)
				//{
					//Mix_PlayChannel( -1, pop, 0 );
					//dJointGroupEmpty (thisp->contacts);
					//is_poped = true;
				//}
			//}	
		}	
		else if(thisp->bBeingMoved)
		{
			thisp->bBeingMoved = false;
		}
	}
	// Call default input handler
	SDL_World::handle_input();
	TRACE_OUT
}
