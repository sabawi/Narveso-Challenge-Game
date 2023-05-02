
#include "static_obj.h"

StaticObj::StaticObj(void)
{
	object = NULL;
	object2 =NULL;
	ObjContacts = NULL;

	rot_theta = 0.0f;
	p = NULL;;
}

StaticObj::~StaticObj(void)
{

}

bool StaticObj::CreateCloud(double x, double y)
{

	if(load_images(CLOUD))
	{
		ObjectType = CLOUD;
		geom = dCreateBox (TheWorld->space,(dReal)OBJECT_DEPTH,(dReal)object->w,(dReal)object->h);
		dGeomSetCategoryBits (geom, CLOUD_CATEGORY);
		init_center(int(x),int(y));		
		setposition(x,y);

		return true;
	}
	return false;

}
bool StaticObj::CreateInvisible(double x, double y)
{

	if(load_images(INVISIBLE))
	{
		ObjectType = INVISIBLE;
		geom = dCreateBox (TheWorld->space,(dReal)OBJECT_DEPTH,(dReal)80,(dReal)20);
		//dGeomSetCategoryBits (geom, CLOUD_CATEGORY);
		init_center(int(x),int(y));		
		setposition(x,y);

		return true;
	}
	return false;

}
bool StaticObj::CreateInvisible2(double xx, double yy, double w, double h)
{

	if(load_images(INVISIBLE))
	{
		ObjectType = INVISIBLE;
		geom = dCreateBox (TheWorld->space,(dReal)OBJECT_DEPTH,(dReal)w,(dReal)h);
		//dGeomSetCategoryBits (geom, CLOUD_CATEGORY);
		init_center(int(xx),int(yy));		
		setposition(xx,yy);
		

		return true;
	}
	return false;

}
bool StaticObj::CreateShip(double x, double y)
{

	if(load_images(SHIP))
	{
		init_x = x;
		init_y = y;
		ObjectType = SHIP;
		body = dBodyCreate (TheWorld->world);

		dMassSetBox (&mass, TheWorld->box_density, 200, object->w, object->h);	
		dBodySetMass(body,&mass);	
		geom = dCreateBox ( TheWorld->space,  200, object->w, object->h);
		rot_theta = 0.0f;
		dGeomSetBody (geom,body);
		dGeomSetCategoryBits (geom, SHIP_CATEGORY);
		init_center(int(x),int(y));		
		setposition(x,y);
		nBodyNumber =0;
		if((p=AddBody(&nBodyNumber,body,SHIP_TYPE,geom))==NULL)
			TRACE_STRING ("FATAL ERROR: AddBody failed. SHIP"); 
		else
		{
			p->TmpLinks[0]		= 0;		

			p->contacts = ObjContacts;
			p->bBeingMoved = false;
			p->nJoints = 0;
	
		}
		dBodySetLinearVel(body,0,0,0);
		dBodySetData (body, (void*)p);
		dGeomSetData (geom, (void *)p);
		//dBodyDisable(body);

		return true;
	}
	return false;

}

bool StaticObj::CreateBox(double x, double y)
{

	if(load_images(BOX))
	{
		init_x = x;
		init_y = y;
		ObjectType = BOX;
		body = dBodyCreate (TheWorld->world);
		//geom = dCreateBox (TheWorld->space,(dReal)OBJECT_DEPTH,(dReal)object->w,(dReal)object->h);
		dMassSetBox (&mass, TheWorld->box_density , BOX_DEPTH, BOX_WIDTH, BOX_HEIGHT);	
		dBodySetMass(body,&mass);	
		geom = dCreateBox ( TheWorld->space,  BOX_DEPTH, BOX_WIDTH, BOX_HEIGHT);
		rot_theta = 0.0f;
		//rotate_body_x(body,rot_theta);
		dGeomSetBody (geom,body);
		dGeomSetCategoryBits (geom, BOX_CATEGORY);
		Box_init_center(int(x),int(y));
		init_center(int(x),int(y));
		setposition(x,y);
		nBodyNumber =0;
		if((p=AddBody(&nBodyNumber,body,BOX_TYPE,geom))==NULL)
			TRACE_STRING ("FATAL ERROR: AddBody failed. SHIP"); 
		else
		{
			p->TmpLinks[0]		= 0;		

			p->contacts = ObjContacts;
			p->bBeingMoved = false;
			p->nJoints = 0;
	
		}
		dBodySetData (body, (void*)p);
		dGeomSetData (geom, (void *)p);

		//init the 3 joints positions
		setBoxJointsPositions();


		//dBodyDisable(body);

		return true;
	}
	return false;

}


dBodyID StaticObj::BoxGetClosest2Boxes(dBodyID *b1, dBodyID *b2)
{
	*b1 = *b2 = 0;

	double dClosestDist1 = sqrt(pow((double)(TheWorld->world_width),2)+pow((double)(TheWorld->world_height),2));
	double dClosestDist2 = dClosestDist1;

	dGeomID o1 = GetClosestGeomUnconnected(geom,BOX_CATEGORY, TheWorld->SearchRadius);
	dGeomID o2 = GetClosestGeomUnconnectedExcluding(geom,BOX_CATEGORY, TheWorld->SearchRadius,o1);
	PTBlist pB1 = (PTBlist) dBodyGetData(body);

	if(o1)
	{		
		dReal *pos1, *pos2;	
		int x1,x2,y1,y2; 
		*b1 = dGeomGetBody(o1);	
		double dTmpDist =0 ;
		int b1j1=0;
		int b2j1=0;

		PTBlist pB2 = (PTBlist) dBodyGetData(*b1);

		for (int i=0;i<3;i++)
		{
			if(!pB1->BoxJoints[i].isUsed)
			{
				for(int j=0;j<3;j++)
				{
					if(!pB2->BoxJoints[j].isUsed)
					{
						dTmpDist =	sqrt(pow(pB1->BoxJoints[i].BoxJointPos[1]-pB2->BoxJoints[j].BoxJointPos[1],2) + 
									pow(pB2->BoxJoints[i].BoxJointPos[2]-pB2->BoxJoints[j].BoxJointPos[2],2) );
						if (dTmpDist < dClosestDist1)
						{
							dClosestDist1 = dTmpDist;
							b1j1 = i;
							b2j1 = j;
						}
					}
				}
			}
		}

		pos1 = pB1->BoxJoints[b1j1].BoxJointPos;
		pos2 = pB2->BoxJoints[b2j1].BoxJointPos;	
		

		x1= (int)pos1[1]+TheWorld->bg_x;
		y1= SCREEN_HEIGHT -(int)pos1[2] -TheWorld->bg_y;


		x2= (int)pos2[1]+TheWorld->bg_x;
		y2= SCREEN_HEIGHT  -(int)pos2[2]-TheWorld->bg_y;

		sDrawLine2(x1, y1, x2, y2 ,  255,255,255, 50);			
	}
	if(o2)
	{
		dReal *pos1, *pos2;	
		int x1,x2,y1,y2;
		double dTmpDist =0 ;
		int b1j2=0;
		int b3j1=0;

		*b2 = dGeomGetBody(o2);
		PTBlist pB2 = (PTBlist) dBodyGetData(*b2);


		for (int i=0;i<3;i++)
		{
			if(!pB1->BoxJoints[i].isUsed)
			{
				for(int j=0;j<3;j++)
				{
					if(!pB2->BoxJoints[j].isUsed)
					{
						dTmpDist =	sqrt(pow(pB1->BoxJoints[i].BoxJointPos[1]-pB2->BoxJoints[j].BoxJointPos[1],2) + 
									pow(pB2->BoxJoints[i].BoxJointPos[2]-pB2->BoxJoints[j].BoxJointPos[2],2) );
						if (dTmpDist < dClosestDist2)
						{
							dClosestDist2 = dTmpDist;
							b1j2 = i;
							b3j1 = j;
						}
					}
				}
			}
		}

		pos1 = pB1->BoxJoints[b1j2].BoxJointPos;
		pos2 = pB2->BoxJoints[b3j1].BoxJointPos;
		

		x1= (int)pos1[1]+TheWorld->bg_x;
		y1= SCREEN_HEIGHT -(int)pos1[2] -TheWorld->bg_y;


		x2= (int)pos2[1]+TheWorld->bg_x;
		y2= SCREEN_HEIGHT  -(int)pos2[2]-TheWorld->bg_y;

		sDrawLine2(x1, y1, x2, y2 ,  255,255,255, 50);			
	}



	return *b1;
}

dJointID StaticObj::MakeJoint(dBodyID body_to_joint, int *joint_number)
{
	const dReal * bpos = dBodyGetPosition(body_to_joint);
	const dReal *mypos;
	double dClosestDist = sqrt(pow((double)(TheWorld->world_width),2)+pow((double)(TheWorld->world_height),2));
	double dTmpDist;
	dJointID joint;
	PTBlist pB1 = (PTBlist) dBodyGetData(body);

	for (int i=0;i<3;i++)
	{
		if(!pB1->BoxJoints[i].isUsed)
		{
			dTmpDist = sqrt(pow(pB1->BoxJoints[i].BoxJointPos[1]-bpos[1],2) + pow(pB1->BoxJoints[i].BoxJointPos[2]-bpos[2],2) );
			if (dTmpDist < dClosestDist)
			{
				dClosestDist = dTmpDist;
				*joint_number = i;
			}
		}
	}
	if(dClosestDist> TheWorld->SearchRadius)
	{
		dClosestDist = 0;
		joint = 0;
		
		return 0;
	}
	else
	{
		mypos = dBodyGetPosition(body);
		dBodySetPosition(body_to_joint,bpos[0],bpos[1],bpos[2]);
		dBodySetPosition(body,mypos[0],mypos[1],mypos[2]);
		joint = HingeAttach2Bodies((PTBlist)dBodyGetData (body), (PTBlist)dBodyGetData (body_to_joint));
		if(joint)
			dJointSetHingeAnchor (joint, BOX_DEPTH, pB1->BoxJoints[*joint_number].BoxJointPos[1] ,pB1->BoxJoints[*joint_number].BoxJointPos[2]);

	}

	return joint;


}

void StaticObj::BoxHandleInput()
{
	TRACE_IN
    PTBlist thisp = (PTBlist)dGeomGetData( geom);
	if(TheWorld->MouseLocked && !thisp->bBeingMoved)
		return;
	//If a key was released
	if( TheWorld->event.type == SDL_KEYDOWN )
	{
	}
	else if( TheWorld->event.type == SDL_KEYUP )
	{

	}
	else if( TheWorld->event.type == SDL_MOUSEBUTTONUP )
	{
		dBodyEnable (body);
		if(is_under_mouse()&& thisp->bBeingMoved )
		{
			dBodySetForce  (body, 0.0f, 0.0f, 0.0f);
			dBodySetTorque (body, 0.0f, 0.0f, 0.0f);
			dBodySetLinearVel  (body, 0.0f, 0.0f, 0.0f);
			dBodySetAngularVel  (body, 0.0f, 0.0f, 0.0f);
			//rotate_body_x(body, 0);
			thisp->bBeingMoved = false;
		}
		else if(thisp->bBeingMoved)
		{
			dBodyEnable (body);
			thisp->bBeingMoved = false;

		}
		TheWorld->MouseLocked = false;

	}
	else if( TheWorld->event.type == SDL_MOUSEBUTTONDOWN )
	{
			
		if(is_under_mouse())
		{
			thisp->bBeingMoved = true;
			dBodyDisable (body);
			y=int(TheWorld->event.button.y - BOX_HEIGHT/2+ TheWorld->bg_y);
			x=int(TheWorld->event.button.x - TheWorld->bg_x);

			dGeomSetPosition (geom,(dReal)OBJECT_DEPTH/2,(dReal)x, (dReal) (SCREEN_HEIGHT - BOX_HEIGHT/2 - y));
			//cout<<"Basket Button Down"<<endl;
			dBodySetForce  (body, 0.0f, 0.0f, 0.0f);
			dBodySetTorque (body, 0.0f, 0.0f, 0.0f);
			//dBodySetLinearVel  (body, 0.0f, 0.0f, 0.0f);
			//dBodySetAngularVel  (body,0.0f, 0.0f, 0.0f);
			
			//DrawToClosest(geom, SEARCH_RADIUS);
			TheWorld->MouseLocked = true;
		}
		else if(thisp->bBeingMoved)
		{
			dBodyEnable (body);
			thisp->bBeingMoved = false;

		}

	}
	else if( TheWorld->event.type == SDL_MOUSEMOTION && (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1)))
	{
		
		if(thisp->bBeingMoved)
		{
			dBodyID b1, b2;
			dBodyDisable (body);
			BoxGetClosest2Boxes(&b1,&b2);
			//dBodyID CutBody =0;
			//dJointID jointID = 0;
			
			//disable_motion = true;
			//dBodyDisable (body);

			y=int(TheWorld->event.button.y - BOX_HEIGHT/2+ TheWorld->bg_y);
			x=int(TheWorld->event.button.x- TheWorld->bg_x);
			//cout<<"Basket MOVE Button Down"<<endl;
			dGeomSetPosition (geom,(dReal)BASKET_DEPTH/2,(dReal)x, (dReal)(SCREEN_HEIGHT - BOX_HEIGHT/2 - y));

			dBodySetForce  (body, 0.0f, 0.0f, 0.0f);
			dBodySetTorque (body, 0.0f, 0.0f, 0.0f);
			//dBodySetLinearVel  (body, 0.0f, 0.0f, 0.0f);
			//dBodySetAngularVel  (body, 0.0f, 0.0f, 0.0f);
			//dBodySetAngularDamping (body, 1.0);

			//rotate_body_x(body, 0);

			//thisp->bBeingMoved = true;
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

	}
	// Call default input handler
	SDL_World::handle_input();
	TRACE_OUT
}

