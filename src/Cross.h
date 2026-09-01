#pragma once


#include "MyObject.h"
#include "Link.h"
#include "point.h"
#include "Cross.h"
#include "MarginalPoint.h"
#include "Controller.h"


class CCross : public CMyObject  
{
public:
	int Connected_Segment_Number;             
	CMyObject *Linking_Cross_or_Marginal[4];    // counterclockwise, the intersection and marginal points connecting this intersection
	int Connected_Link[2*MAX_ENTER_LINK];       //counterclockwise, link number corresponding to Linking_Cross_or_Marginal[]
	Cpoint Cross_Point[MAX_CROSS_POINT_NUMBER];                  //points on the edge of intersection
	int Cross_Lane_Number;                 //the lane number in this intersection
	CController *Controller;
	int Conflict_Area_Number;             //the number of conflict area in this intersection

public:
	CCross();                  
	virtual ~CCross();
	int Find_Connected_Link(CLink *p);  //search link *p, if p is in Connected_Link[], return 0--false
};
