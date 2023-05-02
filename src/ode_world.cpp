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
#include "ode_world.h"
#include "common.h"
#include "sdl_utils.h"
#include <SDL_draw.h>
using namespace std;
// this is called by dSpaceCollide when two objects in space are
// potentially colliding.

void UpdateBodyJointForces(PTBlist pB, int joint)
{
	if(pB->joints[joint])
		memcpy((void*)&pB->JFB[joint],(void*)dJointGetFeedback (pB->joints[joint]),sizeof(dJointFeedback));
}


void CheckAllJointsForces(bool bDisplay, double Fkill, double Tkill)
{
	PTBlist pB = TheWorld->pBlist;
	const dReal *pos1, *pos2;
	dReal dpos[3];
	//char szStr[200];
	int i,x,y;

	while(pB!=NULL)
	{
		for(i=0;i<pB->nJoints;i++)
		{				
			if(pB->joints[i] && (dJointGetType(pB->joints[i])== dJointTypeHinge))
			{
				UpdateBodyJointForces(pB, i);
				dBodyID b1 = dJointGetBody(pB->joints[i],0);
				dBodyID b2 = dJointGetBody(pB->joints[i],1);	
				
				if(b1 && b2)
				{
					PTBlist pB2 = (PTBlist) dBodyGetData(b2);
					if(bDisplay)
					{
						pos1 = dBodyGetPosition(b1);
						pos2 = dBodyGetPosition(b2);
						dpos[1] = pos1[1] + (pos2[1]-pos1[1])/4;
						dpos[2] = pos1[2] + (pos2[2]-pos1[2])/4;

						x= int( dpos[1]+TheWorld->bg_x );  
						y= SCREEN_HEIGHT - int(dpos[2])-TheWorld->bg_y;

						//sprintf(szStr,"%d",pB->nJoints);
						//drawText2(TheWorld->screen,szStr,16,x,y,SDL_MapRGB(TheWorld->screen->format, 0,0,255));
						//sprintf(szStr,"F=(%.0f,%.0f,%.0f)",pB->JFB[i].f1[0]/1000000,pB->JFB[i].f1[1]/1000000,pB->JFB[i].f1[2]/1000000);
						//drawText2(TheWorld->screen,szStr,16,x,y-17,SDL_MapRGB(TheWorld->screen->format, 0,0,255));
						//sprintf(szStr,"T=(%.0f,%.0f,%.0f)",pB->JFB[i].t1[0]/1000000,pB->JFB[i].t1[1]/1000000,pB->JFB[i].t1[2]/1000000);
						//drawText2(TheWorld->screen,szStr,16,x,y,SDL_MapRGB(TheWorld->screen->format, 0,0,255));
					}
				
					if( fabs(pB->JFB[i].f1[0]/1000000)>Fkill || fabs(pB->JFB[i].f1[1]/1000000)>Fkill || fabs(pB->JFB[i].f1[2]/1000000)>Fkill)
					{
						pB->joint_to_delete[i] = true;

						for(int n=0;n<pB2->nJoints;n++)
						{
							if(pB2->joints[n] == pB->joints[i])
								pB2->joint_to_delete[n] = true;
						}

						/*
						dJointAttach(pB->joints[i],0,0);
						dJointDestroy(pB->joints[i]);
						pB->joints[i] = 0;
						pB->nJoints--;
						pB2->nJoints--;
						
						if(pB->nJoints==1)
						{
							dJointID jnt = dBodyGetJoint(pB->body,0);
							if(jnt)
							{
								dBodyID btmp = dJointGetBody(jnt,1);
								if(btmp)
								{	
									PTBlist pbtmp = (PTBlist) dBodyGetData(btmp);							
									dJointAttach(jnt,0,0);
									dJointDestroy(jnt);
									pB->nJoints--;				
									pbtmp->nJoints--;
								}
							}
						}
						

						if(pB2->nJoints==1)
						{
							dJointID jnt = dBodyGetJoint(pB2->body,0);
							if(jnt)
							{
								dBodyID btmp = dJointGetBody(jnt,1);
								if(btmp)
								{	
									PTBlist pbtmp = (PTBlist) dBodyGetData(btmp);							
									dJointAttach(jnt,0,0);
									dJointDestroy(jnt);
									pB2->nJoints--;				
									pbtmp->nJoints--;
								}
							}
						}
						*/


					}		
					/*
					else if (fabs(pB->JFB[i].t1[0]/1000000)>Tkill || fabs(pB->JFB[i].t1[1]/1000000)>Tkill || fabs(pB->JFB[i].t1[2]/1000000)>Tkill)
					{
						pB->joint_to_delete[i] = true;
						for(int n=0;n<pB2->nJoints;n++)
						{
							if(pB2->joints[n] == pB->joints[i])
								pB2->joint_to_delete[n] = true;
						}
				
						dJointAttach(pB->joints[i],0,0);
						dJointDestroy(pB->joints[i]);
						pB->nJoints--;
						pB2->nJoints--;
						if(pB->nJoints==1)
						{
							dJointID jnt = dBodyGetJoint(pB->body,0);
							if(jnt)
							{
								dBodyID btmp = dJointGetBody(jnt,1);
								if(btmp)
								{	
									PTBlist pbtmp = (PTBlist) dBodyGetData(btmp);							
									dJointAttach(jnt,0,0);
									dJointDestroy(jnt);
									pB->nJoints--;				
									pbtmp->nJoints--;
								}
							}
						}
						
						if(pB2->nJoints==1)
						{
							dJointID jnt = dBodyGetJoint(pB2->body,0);
							if(jnt)
							{
								dBodyID btmp = dJointGetBody(jnt,1);
								if(btmp)
								{	
									PTBlist pbtmp = (PTBlist) dBodyGetData(btmp);							
									dJointAttach(jnt,0,0);
									dJointDestroy(jnt);
									pB2->nJoints--;				
									pbtmp->nJoints--;
								}
							}
						}
						


					}*/
				}

			}

		}
		pB=pB->pNext;
	}

}



void ZeroJointFeedBack(dJointID joint, dJointFeedback * jfb)
{
	
	jfb->f1[0]=0.0; // force that joint applies to body 1
	jfb->f1[1]=0.0;
	jfb->f1[2]=0.0;

    jfb->t1[0]=0.0; // torque that joint applies to body 1
    jfb->t1[1]=0.0;    
	jfb->t1[2]=0.0;

    jfb->f2[0]=0.0; // force that joint applies to body 2
    jfb->f2[1]=0.0; 
	jfb->f2[2]=0.0; 


    jfb->t2[0]=0.0; // torque that joint applies to body 2
    jfb->t2[1]=0.0; 
    jfb->t2[2]=0.0; 
	
	dJointSetFeedback (joint,jfb);
}
void RecordHighestJointForces(PTBlist pB,dBodyID b2, int nJ)
{

	if(fabs(pB->JFB[nJ].f1[0]) > fabs(TheWorld->FZ.value))
	{
		TheWorld->FZ.value = pB->JFB[nJ].f1[0];
		TheWorld->FZ.body  = pB->body;
		TheWorld->FZ.joint = pB->joints[nJ];
	}
	if(fabs(pB->JFB[nJ].f1[1]) > fabs(TheWorld->FX.value))
	{
		TheWorld->FX.value = pB->JFB[nJ].f1[1];
		TheWorld->FX.body  = pB->body;
		TheWorld->FX.joint = pB->joints[nJ];
	}
	if(fabs(pB->JFB[nJ].f1[2]) > fabs(TheWorld->FY.value))
	{
		TheWorld->FY.value = pB->JFB[nJ].f1[2];
		TheWorld->FY.body  = pB->body;
		TheWorld->FY.joint = pB->joints[nJ];
	}

	if(fabs(pB->JFB[nJ].t1[0]) > fabs(TheWorld->TZ.value))
	{
		TheWorld->TZ.value = pB->JFB[nJ].t1[0];
		TheWorld->TZ.body  = pB->body;
		TheWorld->TZ.joint = pB->joints[nJ];
	}
	if(fabs(pB->JFB[nJ].t1[1]) > fabs(TheWorld->TX.value))
	{
		TheWorld->TX.value = pB->JFB[nJ].t1[1];
		TheWorld->TX.body  = pB->body;
		TheWorld->TX.joint = pB->joints[nJ];
	}
	if(fabs(pB->JFB[nJ].t1[2]) > fabs(TheWorld->TY.value))
	{
		TheWorld->TY.value = pB->JFB[nJ].t1[2];
		TheWorld->TY.body  = pB->body;
		TheWorld->TY.joint = pB->joints[nJ];
	}
	
	// The other body
	if(fabs(pB->JFB[nJ].f2[0]) > fabs(TheWorld->FZ.value))
	{
		TheWorld->FZ.value = pB->JFB[nJ].f2[0];
		TheWorld->FZ.body  = b2;
		TheWorld->FZ.joint = pB->joints[nJ];
	}
	if(fabs(pB->JFB[nJ].f2[1]) > fabs(TheWorld->FX.value))
	{
		TheWorld->FX.value = pB->JFB[nJ].f2[1];
		TheWorld->FX.body  = b2;
		TheWorld->FX.joint = pB->joints[nJ];
	}
	if(fabs(pB->JFB[nJ].f2[2]) > fabs(TheWorld->FY.value))
	{
		TheWorld->FY.value = pB->JFB[nJ].f2[2];
		TheWorld->FY.body  = b2;
		TheWorld->FY.joint = pB->joints[nJ];
	}

	if(fabs(pB->JFB[nJ].t2[0]) > fabs(TheWorld->TZ.value))
	{
		TheWorld->TZ.value = pB->JFB[nJ].t2[0];
		TheWorld->TZ.body  = b2;
		TheWorld->TZ.joint = pB->joints[nJ];
	}
	if(fabs(pB->JFB[nJ].t2[1]) > fabs(TheWorld->TX.value))
	{
		TheWorld->TX.value = pB->JFB[nJ].t1[1];
		TheWorld->TX.body  = b2;
		TheWorld->TX.joint = pB->joints[nJ];
	}
	if(fabs(pB->JFB[nJ].t2[2]) > fabs(TheWorld->TY.value))
	{
		TheWorld->TY.value = pB->JFB[nJ].t2[2];
		TheWorld->TY.body  = b2;
		TheWorld->TY.joint = pB->joints[nJ];
	}
}
dBodyID GetFurthestAttached(dBodyID MyBody, int nMinDistance, dJointID *JointID)
{
	const dReal * TestBodyPos, *MyBodyPos;
	dBodyID TestBody=0, FurthestBody = 0;
	dJointID TestJoint=0;
	int nNumJoints = 0, i =0;
	double dTestDist =0, dFurthestDist = 0,  X1=0,Y1=0,X2=0, Y2=0;
	
	nNumJoints = dBodyGetNumJoints(MyBody);

	MyBodyPos = dBodyGetPosition(MyBody);

	X1 = MyBodyPos[1];
	Y1 = MyBodyPos[2];

	for(i=0;i<nNumJoints;i++)
	{
		TestJoint =  dBodyGetJoint (MyBody, i);
		TestBody = dJointGetBody(TestJoint, 1);

		if(TestBody )
		{
			TestBodyPos = dBodyGetPosition(TestBody);

			X2 = TestBodyPos[1];
			Y2 = TestBodyPos[2];		
			dTestDist = sqrt(pow((X2-X1),2)+pow((Y2-Y1),2));
			if((dTestDist>=(double) nMinDistance) && (dTestDist>dFurthestDist))
			{
				dFurthestDist = dTestDist;
				FurthestBody = TestBody;
				*JointID = TestJoint;
			}
		}
		else
		{
			*JointID = TestJoint;
			return 0;  // Attached to static
		}
	}
	return FurthestBody;
}

dGeomID GetClosest5GeomUnconnected(dGeomID MyGeom, int nCat1, int nCat2, int WithInDistance, dGeomID (&CloseGeom)[5])
{
	dGeomID  TestGeom =0;
	//dGeomID CloseGeom[5];
	const dReal * TestGeomPos, *MyGeomPos;
	int nGeomsInSpace = dSpaceGetNumGeoms (TheWorld->space); 
	int i =0, j=0, k=0;
	double dTestDist =0,  X1=0,Y1=0,X2=0, Y2=0;

	// initilise the dCosestDis to the longest distance in the world = diagonal from one 
	// corner to the opposite one
	double dCosestDist[5];
	
	dCosestDist[0]=  sqrt(pow((double)(TheWorld->world_width),2)+pow((double)(TheWorld->world_height),2));
	CloseGeom[0] = 0 ;
	for(int i=0;i<4;i++)
	{
		dCosestDist[i]= dCosestDist[0];
		CloseGeom[i] = 0;
	}

	assert(MyGeom);
	MyGeomPos = dGeomGetPosition(MyGeom);
	X1 = MyGeomPos[1];
	Y1 = MyGeomPos[2];	

	for (i=0; i<nGeomsInSpace ; i++)
	{
		TestGeom = dSpaceGetGeom (TheWorld->space, i);
		if(	TestGeom!=MyGeom &&          // exclude me
			!dGeomIsSpace(TestGeom) &&   // and all spaces
			((nCat1 == dGeomGetCategoryBits(TestGeom))  ||(nCat2 == dGeomGetCategoryBits(TestGeom))) && // and ensure the right category
			(!dAreConnectedExcluding (dGeomGetBody(MyGeom), dGeomGetBody(TestGeom),  dJointTypeContact)))
		{

			TestGeomPos = dGeomGetPosition(TestGeom);
			PTBlist pB = (PTBlist) dGeomGetData(TestGeom);
			X2 = TestGeomPos[1];
			Y2 = TestGeomPos[2];
			
			dTestDist = sqrt(((X2-X1) * (X2-X1))+((Y2-Y1)* (Y2-Y1)));
#ifdef _DEBUG

			// debug code
			//int ax= (int)X2+TheWorld->bg_x;
			//int ay= SCREEN_HEIGHT  -(int)Y2 -TheWorld->bg_y - SPHERE_HEIGHT;
			//char text[50];
			//sprintf(text,"%2.0f",dTestDist);
			//drawText(text, 24,ax, ay, 255, 255, 255, 0, 0, 0);
#endif

			// end debug
			int nMyCat  = dGeomGetCategoryBits(MyGeom);

			if(dTestDist<WithInDistance)
			{		
				for(j=4;j>=0;j--)
				{
					if(dTestDist<=dCosestDist[j]) 
					{	
						if(nMyCat == HARDBALL_CATEGORY || nMyCat == YELLOWBALL_CATEGORY)
						{							
							if(pB->nJoints>0)
							{
								// switch geoms
								for(k=j+1;k<5;k++)
								{
									CloseGeom[k] = CloseGeom[k-1];												
									dCosestDist[k] = dCosestDist[k-1];
								}
								CloseGeom[j] = TestGeom;
								dCosestDist[j] = dTestDist;
#ifdef _DEBUG
								//char text2[50];
								//int ax1= (int)X1+TheWorld->bg_x;
								//int ay1= SCREEN_HEIGHT  -(int)Y1 -TheWorld->bg_y - SPHERE_HEIGHT;
								//sprintf(text2,"%2.0f",dCosestDist[0]);
								//drawText(text2, 24,ax1, ay1, 255, 255, 255, 0, 0, 0);
#endif
							}
						}
						else
						{
							// switch geoms
							for(k=j+1;k<5;k++)
							{
								CloseGeom[k] = CloseGeom[k-1];												
								dCosestDist[k] = dCosestDist[k-1];
							}
							CloseGeom[j] = TestGeom;
							dCosestDist[j] = dTestDist;
						}
					}
				}		
			}

		}
	}


	return CloseGeom[0];

}


dGeomID Get2ClosestGeomUnconnected(dGeomID MyGeom, int nCat1, int nCat2, int WithInDistance, dGeomID &o1, dGeomID &o2)
{
	dGeomID CloseGeom1 =0 ,CloseGeom2 = 0, TestGeom =0;
	dGeomID tmp1Geom =0 ,tmp2Geom =0;
	const dReal * TestGeomPos, *MyGeomPos;
	int nGeomsInSpace = dSpaceGetNumGeoms (TheWorld->space); 
	int i =0;
	double dTestDist =0,  X1=0,Y1=0,X2=0, Y2=0;

	o1=o2=0;
	// initilise the dCosestDis to the longest distance in the world = diagonal from one 
	// corner to the opposite one
	double dCosestDist1 =  sqrt(pow((double)(TheWorld->world_width),2)+pow((double)(TheWorld->world_height),2));
	double dCosestDist2 =  sqrt(pow((double)(TheWorld->world_width),2)+pow((double)(TheWorld->world_height),2));

	assert(MyGeom);
	MyGeomPos = dGeomGetPosition(MyGeom);
	X1 = MyGeomPos[1];
	Y1 = MyGeomPos[2];	

	for (i=0; i<nGeomsInSpace ; i++)
	{
		TestGeom = dSpaceGetGeom (TheWorld->space, i);
		if(	TestGeom!=MyGeom &&          // exclude me
			!dGeomIsSpace(TestGeom) &&   // and all spaces
			((nCat1 == dGeomGetCategoryBits(TestGeom))  ||(nCat2 == dGeomGetCategoryBits(TestGeom))) && // and ensure the right category
			(!dAreConnectedExcluding (dGeomGetBody(MyGeom), dGeomGetBody(TestGeom),  dJointTypeContact)))
		{

			TestGeomPos = dGeomGetPosition(TestGeom);
			X2 = TestGeomPos[1];
			Y2 = TestGeomPos[2];
			
			dTestDist = sqrt(((X2-X1) * (X2-X1))+((Y2-Y1)* (Y2-Y1)));
#ifdef _DEBUG

			// debug code
			int ax= (int)X2+TheWorld->bg_x;
			int ay= SCREEN_HEIGHT  -(int)Y2 -TheWorld->bg_y - SPHERE_HEIGHT;
			char text[50];
			sprintf(text,"%2.0f",dTestDist);
			drawText(text, 24,ax, ay, 255, 255, 255, 0, 0, 0);
#endif

			// end debug

			if(dTestDist<WithInDistance)
			{		
				if(dTestDist<=dCosestDist2) // closer than the 2nd closest
				{
					if(dTestDist<=dCosestDist1) // closer than the 1st closest 
					{
						// we found a new 1st closest
						int nMyCat  = dGeomGetCategoryBits(MyGeom);
						if(nMyCat == HARDBALL_CATEGORY)
						{
							PTBlist pB = (PTBlist) dGeomGetData(TestGeom);
							if(pB->nJoints>0)
							{
								// switch 1st closest to be second closest 
								//tmp2Geom = CloseGeom1;  
								CloseGeom2 = CloseGeom1;	
								//tmp1Geom = TestGeom;
								// and make the new testgeom the first closest
								CloseGeom1 = TestGeom;	
								dCosestDist2 = dCosestDist1;
								dCosestDist1 = dTestDist;
#ifdef _DEBUG
								char text2[50];

								int ax1= (int)X1+TheWorld->bg_x;
								int ay1= SCREEN_HEIGHT  -(int)Y1 -TheWorld->bg_y - SPHERE_HEIGHT;
								sprintf(text2,"%2.0f",dCosestDist1);
								drawText(text2, 24,ax1, ay1, 255, 255, 255, 0, 0, 0);
#endif

							}

						}
					}
					else
					{
						// found a new 2nd closest 
						int nMyCat  = dGeomGetCategoryBits(MyGeom);
						if(nMyCat == HARDBALL_CATEGORY)
						{
							PTBlist pB = (PTBlist) dGeomGetData(TestGeom);
							if(pB->nJoints>0)
							{
							
								//tmp2Geom = CloseGeom2;
								CloseGeom2 = TestGeom;		
								dCosestDist2 = dTestDist;

#ifdef _DEBUG
								char text2[50];

								int ax1= (int)X1+TheWorld->bg_x;
								int ay1= SCREEN_HEIGHT  -(int)Y1 -TheWorld->bg_y - SPHERE_HEIGHT;
								sprintf(text2,"%2.0f, %2.0f",dCosestDist1, dCosestDist2);
								drawText(text2, 24,ax1, ay1, 255, 255, 255, 0, 0, 0);
#endif

							}

						}
					}

				}
#ifdef _DEBUG
				else
					drawText(">dCosest", 24,ax, ay, 0, 0, 255, 0, 0, 0);
#endif			
			}
#ifdef _DEBUG
			else
				drawText(">WithIn", 24,ax, ay, 0, 255, 0, 0, 0, 0);
#endif

		}
	}

	o1 = CloseGeom1;
	o2 = CloseGeom2;


	return CloseGeom1;

}

dGeomID GetClosestGeomUnconnected(dGeomID MyGeom, int nCat, int WithInDistance)
{
	dGeomID CloseGeom =0 ,TestGeom =0;
	const dReal * TestGeomPos, *MyGeomPos;
	int nGeomsInSpace = dSpaceGetNumGeoms (TheWorld->space); 
	int i =0;
	double dTestDist =0,  X1=0,Y1=0,X2=0, Y2=0;

	// initilise the dCosestDis to the longest distance in the world = diagonal from one 
	// corner to the opposite one
	double dCosestDist = sqrt(pow((double)(TheWorld->world_width),2)+pow((double)(TheWorld->world_height),2));

	assert(MyGeom);
	
	MyGeomPos = dGeomGetPosition(MyGeom);
	X1 = MyGeomPos[1];
	Y1 = MyGeomPos[2];

	for (i=0; i<nGeomsInSpace ; i++)
	{
		TestGeom = dSpaceGetGeom (TheWorld->space, i);
		if(	TestGeom!=MyGeom &&          // exclude me
			!dGeomIsSpace(TestGeom) &&   // and all spaces
			nCat == dGeomGetCategoryBits(TestGeom) && // and ensure the right category
			(!dAreConnectedExcluding (dGeomGetBody(MyGeom), dGeomGetBody(TestGeom),  dJointTypeContact)))
		{
			TestGeomPos = dGeomGetPosition(TestGeom);
			X2 = TestGeomPos[1];
			Y2 = TestGeomPos[2];
			
			dTestDist = sqrt(pow((X2-X1),2)+pow((Y2-Y1),2));
			if(dTestDist<WithInDistance)
			{
				if(dTestDist<dCosestDist)
				{
					int nMyCat  = dGeomGetCategoryBits(MyGeom);
					if(nMyCat == HARDBALL_CATEGORY)
					{
						PTBlist pB = (PTBlist) dGeomGetData(TestGeom);
						if(pB->nJoints>0)
						{
							dCosestDist = dTestDist;
							CloseGeom = TestGeom;
						}
					}
					else
					{
						dCosestDist = dTestDist;
						CloseGeom = TestGeom;
					}
				}

			}

		}
	}
	//printf("Closest = %p\n",CloseGeom);
	return CloseGeom;

}

dGeomID GetClosestGeomUnconnectedExcluding2(dGeomID MyGeom, int nCat, int WithInDistance,dGeomID ox1, dGeomID ox2)
{
	dGeomID CloseGeom =0 ,TestGeom =0;
	const dReal * TestGeomPos, *MyGeomPos;
	int nGeomsInSpace = dSpaceGetNumGeoms (TheWorld->space); 
	int i =0;
	double dTestDist =0,  X1=0,Y1=0,X2=0, Y2=0;

	// initilise the dCosestDis to the longest distance in the world = diagonal from one 
	// corner to the opposite one
	double dCosestDist = sqrt(pow((double)(TheWorld->world_width),2)+pow((double)(TheWorld->world_height),2));

	assert(MyGeom);
	
	MyGeomPos = dGeomGetPosition(MyGeom);
	X1 = MyGeomPos[1];
	Y1 = MyGeomPos[2];

	for (i=0; i<nGeomsInSpace ; i++)
	{
		TestGeom = dSpaceGetGeom (TheWorld->space, i);

		if(	TestGeom!=ox1 && TestGeom!=ox2 &&
			TestGeom!=MyGeom &&          // exclude me
			!dGeomIsSpace(TestGeom) &&   // and all spaces
			nCat == dGeomGetCategoryBits(TestGeom) && // and ensure the right category
			(!dAreConnectedExcluding (dGeomGetBody(MyGeom), dGeomGetBody(TestGeom),  dJointTypeContact)))
		{
			TestGeomPos = dGeomGetPosition(TestGeom);
			X2 = TestGeomPos[1];
			Y2 = TestGeomPos[2];
			
			dTestDist = sqrt(pow((X2-X1),2)+pow((Y2-Y1),2));
			if(dTestDist<WithInDistance)
			{
					int nMyCat  = dGeomGetCategoryBits(MyGeom);
					if(nMyCat == HARDBALL_CATEGORY)
					{
						PTBlist pB = (PTBlist) dGeomGetData(TestGeom);
						if(pB->nJoints>0)
						{
							dCosestDist = dTestDist;
							CloseGeom = TestGeom;
						}
					}
					else
					{
						dCosestDist = dTestDist;
						CloseGeom = TestGeom;
					}

			}

		}
	}
	//printf("Closest = %p\n",CloseGeom);
	return CloseGeom;

}

dGeomID GetClosestGeomUnconnectedExcluding(dGeomID MyGeom, int nCat, int WithInDistance,dGeomID ox)
{
	dGeomID CloseGeom =0 ,TestGeom =0;
	const dReal * TestGeomPos, *MyGeomPos;
	int nGeomsInSpace = dSpaceGetNumGeoms (TheWorld->space); 
	int i =0;
	double dTestDist =0,  X1=0,Y1=0,X2=0, Y2=0;

	// initilise the dCosestDis to the longest distance in the world = diagonal from one 
	// corner to the opposite one
	double dCosestDist = sqrt(pow((double)(TheWorld->world_width),2)+pow((double)(TheWorld->world_height),2));

	assert(MyGeom);
	
	MyGeomPos = dGeomGetPosition(MyGeom);
	X1 = MyGeomPos[1];
	Y1 = MyGeomPos[2];

	for (i=0; i<nGeomsInSpace ; i++)
	{
		TestGeom = dSpaceGetGeom (TheWorld->space, i);

		if(	TestGeom!=ox &&
			TestGeom!=MyGeom &&          // exclude me
			!dGeomIsSpace(TestGeom) &&   // and all spaces
			nCat == dGeomGetCategoryBits(TestGeom) && // and ensure the right category
			(!dAreConnectedExcluding (dGeomGetBody(MyGeom), dGeomGetBody(TestGeom),  dJointTypeContact)))
		{
			TestGeomPos = dGeomGetPosition(TestGeom);
			X2 = TestGeomPos[1];
			Y2 = TestGeomPos[2];
			
			dTestDist = sqrt(pow((X2-X1),2)+pow((Y2-Y1),2));
			if(dTestDist<WithInDistance)
			{
					int nMyCat  = dGeomGetCategoryBits(MyGeom);
					if(nMyCat == HARDBALL_CATEGORY)
					{
						PTBlist pB = (PTBlist) dGeomGetData(TestGeom);
						if(pB->nJoints>0)
						{
							dCosestDist = dTestDist;
							CloseGeom = TestGeom;
						}
					}
					else
					{
						dCosestDist = dTestDist;
						CloseGeom = TestGeom;
					}

			}

		}
	}
	//printf("Closest = %p\n",CloseGeom);
	return CloseGeom;

}

dGeomID GetClosestGeom(dGeomID MyGeom, int nCat, int WithInDistance)
{
	dGeomID CloseGeom =0 ,TestGeom =0;
	const dReal * TestGeomPos, *MyGeomPos;
	int nGeomsInSpace = dSpaceGetNumGeoms (TheWorld->space); 
	int i =0;
	double dTestDist =0,  X1=0,Y1=0,X2=0, Y2=0;

	// initilise the dCosestDis to the longest distance in the world = diagonal from one 
	// corner to the opposite one
	double dCosestDist = sqrt(pow((double)(TheWorld->world_width),2)+pow((double)(TheWorld->world_height),2));;
	if(WithInDistance==-1) // -1 means search the whole world
		WithInDistance = int(dCosestDist);

	assert(MyGeom);
	
	MyGeomPos = dGeomGetPosition(MyGeom);
	X1 = MyGeomPos[1];
	Y1 = MyGeomPos[2];

	for (i=0; i<nGeomsInSpace ; i++)
	{
		TestGeom = dSpaceGetGeom (TheWorld->space, i);
		if(	TestGeom!=MyGeom &&          // exclude me
			!dGeomIsSpace(TestGeom) &&   // and all spaces
			nCat == dGeomGetCategoryBits(TestGeom)) // and ensure the right category
		{
			TestGeomPos = dGeomGetPosition(TestGeom);
			X2 = TestGeomPos[1];
			Y2 = TestGeomPos[2];

			dTestDist = sqrt(pow((X2-X1),2)+pow((Y2-Y1),2));
			if(dTestDist<WithInDistance)
			{
				if(dTestDist<dCosestDist)
				{
					dCosestDist = dTestDist;
					CloseGeom = TestGeom;
				}

			}

		}
	}
	return CloseGeom;

}

dGeomID GetClosestGeomWithJointExcluding(dGeomID MyGeom, int nCat, int WithInDistance, dGeomID exclude)
{
	dGeomID CloseGeom =0 ,TestGeom =0;
	const dReal * TestGeomPos, *MyGeomPos;
	int nGeomsInSpace = dSpaceGetNumGeoms (TheWorld->space); 
	int i =0;
	double dTestDist =0,  X1=0,Y1=0,X2=0, Y2=0;

	// initilise the dCosestDis to the longest distance in the world = diagonal from one 
	// corner to the opposite one
	double dCosestDist = sqrt(pow((double)(TheWorld->world_width),2)+pow((double)(TheWorld->world_height),2));

	assert(MyGeom);
	
	MyGeomPos = dGeomGetPosition(MyGeom);
	X1 = MyGeomPos[1];
	Y1 = MyGeomPos[2];

	for (i=0; i<nGeomsInSpace ; i++)
	{
		TestGeom = dSpaceGetGeom (TheWorld->space, i);
		if(	TestGeom!=MyGeom &&          // exclude me
			!dGeomIsSpace(TestGeom) &&   // and all spaces
			nCat == dGeomGetCategoryBits(TestGeom) &&  // and ensure the right category
			TestGeom != exclude)

		{
			TestGeomPos = dGeomGetPosition(TestGeom);
			
			PTBlist Bp = (PTBlist)dGeomGetData(TestGeom);

			if(Bp->nJoints>1)
			{
				
				X2 = TestGeomPos[1];
				Y2 = TestGeomPos[2];

				dTestDist = sqrt(pow((X2-X1),2)+pow((Y2-Y1),2));
				if(dTestDist<WithInDistance)
				{
					if(dTestDist<dCosestDist)
					{
						dCosestDist = dTestDist;
						CloseGeom = TestGeom;
					}

				}
			}

		}
	}
	return CloseGeom;

}

dJointID FixedAttach2Bodies(PTBlist pB1, PTBlist pB2)
{
	dBodyID b1=pB1->body , b2 ;
	dJointID c2 =0;
	
	if(pB2==NULL)
	{
		if(pB1->nJoints>=MAX_JOINTS)
			return c2;
		// attach to static 
		c2= dJointCreateFixed (TheWorld->world, pB1->contacts);
		dJointAttach (c2,b1, 0);
		
		pB1->joints[pB1->nJoints]= c2;
		pB1->joint_to_delete[pB1->nJoints] = false;
		ZeroJointFeedBack(c2,&pB1->JFB[pB1->nJoints]);
		pB1->nJoints++;
		return c2;
	}
	else
		b2 = pB2->body;

	if(pB1->nJoints>=MAX_JOINTS|| pB2->nJoints>=MAX_JOINTS)
	{
		return c2;
	}
	if(dAreConnectedExcluding (b1, b2,  dJointTypeContact)==0)
	{
		dJointID c2= dJointCreateFixed (TheWorld->world, pB1->contacts);

		dJointAttach (c2,b1, b2);

		pB1->joints[pB1->nJoints]=pB2->joints[pB2->nJoints]= c2;
		pB1->joint_to_delete[pB1->nJoints] = pB2->joint_to_delete[pB2->nJoints] = false;
		ZeroJointFeedBack(c2,&pB1->JFB[pB1->nJoints]);
		ZeroJointFeedBack(c2,&pB2->JFB[pB2->nJoints]);
		pB1->nJoints++;
		pB2->nJoints++;
		//cout<<"Joining 2 bodies by Fixed-- numb of joints = "<<pB1->nJoints<<endl;
		return c2;
	}
	return c2;

}
dJointID HingeAttach2Bodies(PTBlist pB1, PTBlist pB2)
{
	dBodyID b1=pB1->body, b2;
	dJointID c2 =0;

	if(pB2==NULL)
	{
		if(pB1->nJoints>=MAX_JOINTS)
			return c2;

		// attach to static 
		c2= dJointCreateHinge (TheWorld->world, pB1->contacts);

		dJointAttach (c2,b1, 0);
		dJointSetHingeAxis (c2,1,0,0);
		
		pB1->joints[pB1->nJoints]= c2;
		pB1->joint_to_delete[pB1->nJoints] = false;
		ZeroJointFeedBack(c2,&pB1->JFB[pB1->nJoints]);
		pB1->nJoints++;
		return c2;
	}
	else
		b2 = pB2->body;

	if(pB1->nJoints>=MAX_JOINTS|| pB2->nJoints>=MAX_JOINTS)
	{
		return c2;
	}
	if(dAreConnectedExcluding (b1, b2,  dJointTypeContact)==0)
	{
		const dReal *pos1, *pos2;
		pos1 = dBodyGetPosition(b1);
		dBodySetPosition(b1,pos1[0],pos1[1],pos1[2]);
		pos2 = dBodyGetPosition(b2);
		dBodySetPosition(b2,pos2[0],pos2[1],pos2[2]);

		c2= dJointCreateHinge (TheWorld->world, pB1->contacts);
		pB1->joint_to_delete[pB1->nJoints] = pB2->joint_to_delete[pB2->nJoints] = false;
		dJointSetHingeAxis (c2, 1, 0, 0);

		dJointSetHingeParam (c2, dParamCFM, TheWorld->HingedParamCFM);
		dJointSetHingeParam (c2, dParamBounce, TheWorld->HingedParamBounce);
		dJointSetHingeParam (c2, dParamStopERP, TheWorld->HingedParamStopERP);
		dJointSetHingeParam (c2, dParamStopCFM, TheWorld->HingedParamStopCFM);
	
		dJointAttach (c2,b1, b2);
		
		//dJointSetHingeAxis (c2,1,0,0);

		//pB1->joints[pB1->nJoints]=pB2->joints[pB2->nJoints]= c2;
		pB1->joints[pB1->nJoints]= c2;
		ZeroJointFeedBack(c2,&pB1->JFB[pB1->nJoints]);
		//ZeroJointFeedBack(c2,&pB2->JFB[pB2->nJoints]);
		pB1->nJoints++;
		//pB2->nJoints++;
		//cout<<"Joining 2 bodies by Hinge-- numb of joints = "<<pB1->nJoints<<endl;
		return c2;
	}
	return c2;

}

dJointID BallAttach2Bodies(PTBlist pB1, PTBlist pB2)
{
	dBodyID b1=pB1->body, b2;
	dJointID c2 =0;

	if(pB2==NULL)
	{
		if(pB1->nJoints>=MAX_JOINTS)
			return c2;

		// attach to static 
		c2= dJointCreateBall (TheWorld->world, pB1->contacts);

		dJointAttach (c2,b1, 0);
		
		pB1->joints[pB1->nJoints]= c2;
		pB1->joint_to_delete[pB1->nJoints] = false;
		//ZeroJointFeedBack(c2,&pB1->JFB[pB1->nJoints]);
		pB1->nJoints++;
		return c2;
	}
	else
		b2 = pB2->body;

	if(pB1->nJoints>=MAX_JOINTS|| pB2->nJoints>=MAX_JOINTS)
	{
		return c2;
	}
	if(dAreConnectedExcluding (b1, b2,  dJointTypeContact)==0)
	{
		const dReal *pos1, *pos2;
		pos1 = dBodyGetPosition(b1);
		//dBodySetPosition(b1,pos1[0],pos1[1],pos1[2]);
		pos2 = dBodyGetPosition(b2);
		//dBodySetPosition(b2,pos2[0],pos2[1],pos2[2]);

		c2= dJointCreateBall (TheWorld->world, pB1->contacts);
		pB1->joint_to_delete[pB1->nJoints] = pB2->joint_to_delete[pB2->nJoints] = false;

		dJointAttach (c2,b1, b2);

		//pB1->joints[pB1->nJoints]=pB2->joints[pB2->nJoints]= c2;
		pB1->joints[pB1->nJoints]= c2;
		//ZeroJointFeedBack(c2,&pB1->JFB[pB1->nJoints]);
		//ZeroJointFeedBack(c2,&pB2->JFB[pB2->nJoints]);
		pB1->nJoints++;
		//pB2->nJoints++;
		//cout<<"Joining 2 bodies by Hinge-- numb of joints = "<<pB1->nJoints<<endl;
		return c2;
	}
	return c2;

}


dJointID SliderAttach2Bodies(PTBlist pB1, PTBlist pB2)
{
	dBodyID b1=pB1->body , b2 = pB2->body;
	dJointID c2 =0;

	if(pB1->nJoints>=MAX_JOINTS || pB2->nJoints>=MAX_JOINTS)
	{
		return c2;
	}
	if(dAreConnectedExcluding (b1, b2,  dJointTypeContact)==0)
	{
		c2= dJointCreateSlider (TheWorld->world, pB1->contacts);
		pB1->joint_to_delete[pB1->nJoints] = pB2->joint_to_delete[pB2->nJoints] = false;
		//dJointSetSliderAxis (c2, 0, 0,1 );
		dJointSetHingeParam (c2, dParamCFM, TheWorld->HingedParamCFM);
		dJointSetHingeParam (c2, dParamBounce, TheWorld->HingedParamBounce);
		dJointSetHingeParam (c2, dParamStopERP, TheWorld->HingedParamStopERP);
		dJointSetHingeParam (c2, dParamStopCFM, TheWorld->HingedParamStopCFM);
		dJointAttach (c2,b1, b2);
		pB1->joints[pB1->nJoints]=pB2->joints[pB2->nJoints]=c2;
		ZeroJointFeedBack(c2,&pB1->JFB[pB1->nJoints]);
		ZeroJointFeedBack(c2,&pB2->JFB[pB2->nJoints]);
		pB1->nJoints++;
		pB2->nJoints++;
		//cout<<"Joining 2 bodies by a Slider -- numb of joints = "<<pB1->nJoints<<endl;
		return c2;
	}
	return c2;

}
dJointID PistonAttach2Bodies(PTBlist pB1, PTBlist pB2)
{
	dBodyID b1=pB1->body , b2 = pB2->body;
	dJointID c2 =0;

	if(pB1->nJoints>=MAX_JOINTS || pB2->nJoints>=MAX_JOINTS)
	{
		return c2;
	}
	if(dAreConnectedExcluding (b1, b2,  dJointTypeContact)==0)
	{
		c2= dJointCreatePiston (TheWorld->world, pB1->contacts);
		pB1->joint_to_delete[pB1->nJoints] = pB2->joint_to_delete[pB2->nJoints] = false;
		//dJointSetSliderAxis (c2, 0, 0, 1);
		dJointSetHingeParam (c2, dParamCFM, TheWorld->HingedParamCFM);
		dJointSetHingeParam (c2, dParamBounce, TheWorld->HingedParamBounce);
		dJointSetHingeParam (c2, dParamStopERP, TheWorld->HingedParamStopERP);
		dJointSetHingeParam (c2, dParamStopCFM, TheWorld->HingedParamStopCFM);
		dJointAttach (c2,b1, b2);
		pB1->joints[pB1->nJoints]=pB2->joints[pB2->nJoints]=c2;
		//ZeroJointFeedBack(c2,&pB1->JFB[pB1->nJoints]);
		//ZeroJointFeedBack(c2,&pB2->JFB[pB2->nJoints]);
		pB1->nJoints++;
		pB2->nJoints++;
		//cout<<"Joining 2 bodies by a Piston -- numb of joints = "<<pB1->nJoints<<endl;
		return c2;
	}
	return c2;
}

// this is called by dSpaceCollide when two objects in space are
// potentially colliding.

void nearCallback (void *data, dGeomID o1, dGeomID o2)
{
	TRACE_IN
	int nCat1, nCat2;
	int nContactsMade = 0;
	const int nMaxContacts=100;
	dContact contact[nMaxContacts];
	//dContactGeom contact_geom[nMaxContacts];
	//const dReal *bpos;
	bool bBounce = true;
	PTBlist pB1 = NULL, pB2=NULL;

	// Sanity check that these OLE objects exist
	if(!o1 || !o2)
		return;

	//assert(o1);
	//assert(o2);

	if( (pB1 = (PTBlist)dGeomGetData( o1)) && (pB1->bBeingMoved ))
		return;
	if( (pB2 = (PTBlist)dGeomGetData( o2)) && (pB2->bBeingMoved ))
		return;

	if (dGeomIsSpace(o1) || dGeomIsSpace(o2))
	{
		TRACE_STRING("testing space %p %p\n", o1,o2);
    		// colliding a space with something
		dSpaceCollide2(o1,o2,data,&nearCallback);
    		// Note we do not want to test intersections within a space,
    		// only between spaces.
		TRACE_OUT
		return;
	}

	nCat1 = dGeomGetCategoryBits(o1);  // Category for body1
	nCat2 = dGeomGetCategoryBits(o2);  // Category for body2

	// Check for collisions and return contacts 
	nContactsMade = dCollide (o1,o2,nMaxContacts,&(contact[0].geom),sizeof(dContact));
	
	if (nContactsMade > 0) 
	{
		// Collision contacts were found, process them based on body type

		//pB1 = (PTBlist)dGeomGetData( o1);  // Retrieve Body1 data
		//pB2 = (PTBlist)dGeomGetData (o2);  // Retrieve Body2 data	
		
		if ( !dGeomIsSpace(o1) &&  dGeomGetBody(o1))
		{
			dBodySetAngularVel  (dGeomGetBody(o1), 0.0f, 0.0f, 0.0f);
			dBodySetTorque (dGeomGetBody(o1), 0.0f, 0.0f, 0.0f);
		}

		// Process each of the contacts made in this collision
		for (int i=0; i<nContactsMade; i++) 
		{
			// Set the flags to allow us to control the properties of the this contact surface
			contact[i].surface.mode = dContactBounce | dContactSoftCFM | dContactSoftERP ;

			// DEFAULT VALUES SET BELOW: Change them if needed for each specific collision bodies
			contact[i].surface.bounce	  = 0.4f;	// 0: Not Bouncy, 1:Max bounceness
			contact[i].surface.bounce_vel = 0.09f;	// Minimum bounce velocity, don't bounce if less
			contact[i].surface.soft_cfm   = 0.00001f;// 0: Very hard body, 1: Max softness
			contact[i].surface.soft_erp   = 0.99f;	// Error reduction parameter (ERP) also for softness
			contact[i].surface.mu = 10.0f;
			//contact[i].surface.mu2 = 0.0f;

			
			if((nCat1 == HARDBALL_CATEGORY) || (nCat2 == HARDBALL_CATEGORY))
			{
				contact[i].surface.bounce	= 0.9f;	// hardballs are bouncy
				contact[i].surface.soft_cfm = 0.00001f;	 // Hardballs as hard
				contact[i].surface.bounce_vel = 0.f; // no limit on hardballs bounce velocity
				contact[i].surface.soft_erp = 0.9f; 
				contact[i].surface.mu = 0.f;  // No friction for hardballs
			}
			if((nCat1 == YELLOWBALL_CATEGORY) || (nCat2 == YELLOWBALL_CATEGORY))
			{
				contact[i].surface.bounce	= 0.9f;	// hardballs are bouncy
				contact[i].surface.soft_cfm = 0.00001f;	 // Hardballs as hard
				contact[i].surface.bounce_vel = 0.f; // no limit on hardballs bounce velocity
				contact[i].surface.soft_erp = 0.9f; 
				contact[i].surface.mu = 0.f;  // No friction for hardballs
			}
			if((nCat1 == BALOON_CATEGORY) || (nCat2 == BALOON_CATEGORY))
			{
				// If either body is a BALOON, make the collision very soft and not bouncy
				contact[i].surface.bounce	= 0.01f;	
				contact[i].surface.soft_cfm = 0.0001f;	  
				contact[i].surface.soft_erp = 0.8f;
			}	
			if(nCat1 == BOX_CATEGORY || (nCat2 == BOX_CATEGORY))
			{
				contact[i].surface.bounce	= 0.9f;	
				contact[i].surface.soft_cfm = 0.00001f;	
				contact[i].surface.bounce_vel = 0.f;
				contact[i].surface.soft_erp = 0.95f;
				contact[i].surface.mu = 100000.f; // high friction for boxes 
			}		
			if(nCat1 == SHIP_CATEGORY || (nCat2 == SHIP_CATEGORY))
			{
				contact[i].surface.bounce	= 0.0001f;	
				contact[i].surface.soft_cfm = 0.0001f;	  
				contact[i].surface.soft_erp = 0.8f;
				contact[i].surface.mu = 1.f; // high friction for boxes 

			}
			if(nCat1 == CLOUD_CATEGORY || (nCat2 == CLOUD_CATEGORY))
			{
				contact[i].surface.bounce	= 0.0001f;	
				contact[i].surface.soft_cfm = 0.0001f;	  
				contact[i].surface.soft_erp = 0.8f;
				contact[i].surface.mu = 1000000.0f; // very high friction for clouds

			}			

			// Bounce off one another and go your marry way
			dJointID c = dJointCreateContact (TheWorld->world,TheWorld->contactgroup,&contact[0]);
			dJointAttach (c,dGeomGetBody(o1), dGeomGetBody(o2));
		}
	}
	TRACE_OUT
}
 
MyWorld::MyWorld()
{
	TRACE_IN
	world=TheWorld->world;
	space=TheWorld->space;
	contactgroup=TheWorld->contactgroup;

		
		
	ground=TheWorld->ground;
	ceiling=TheWorld->ceiling;
	 
	rightwall=TheWorld->rightwall;
	leftwall=TheWorld->leftwall;
		
	TRACE_OUT		
}

MyWorld::~MyWorld()
{
	TRACE_IN

	TRACE_OUT
}

void simWorld (void)
{
    TRACE_IN
	// find collisions and add contact joints
	//dSpaceCollide (TheWorld->space,0,&nearCallback);
    dSpaceCollide (TheWorld->space,0,TheWorld->fnc_nearCallback);
	// step the simulation
	// Larger Step = fast simumation but less accurate
	dWorldQuickStep (TheWorld->world,TheWorld->world_step_size);  
        
	// remove all contact joints
	dJointGroupEmpty (TheWorld->contactgroup);

	TRACE_OUT
}

void InitSimParms(void)
{
	TRACE_IN

	dWorldSetGravity (TheWorld->world,0,0,TheWorld->world_gravity);

	dWorldSetLinearDamping(TheWorld->world, TheWorld->world_linear_damping);

	dWorldSetAngularDamping (TheWorld->world, TheWorld->world_ang_damping);

	dWorldSetMaxAngularSpeed (TheWorld->world, TheWorld->world_max_angular_speed);
	
	dWorldSetERP(TheWorld->world,TheWorld->world_ERP);
	
	dWorldSetCFM (TheWorld->world,TheWorld->world_CFM	);

	// Auto Disable flag optimizes simulation. -> it disable static objects and 
	// that leaves them out of the simulation matrix 
	dWorldSetAutoDisableFlag (TheWorld->world, TheWorld->world_auto_disable); // Set to 1 

	TRACE_OUT
}


 

