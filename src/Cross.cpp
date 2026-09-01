// Cross.cpp: implementation of the CCross class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "traffic.h"
#include "Cross.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCross::CCross()
{
	int i;
	for(i=0;i<2*MAX_ENTER_LINK;i++)
	{
		Cross_Point[i].x = -1;
		Cross_Point[i].y = -1;
	}
	for(i=0;i<2*MAX_ENTER_LINK;i++)
	{
		Connected_Link[i] = -1;
	}
	for(i=0;i<MAX_ENTER_LINK;i++)
	{
		Linking_Cross_or_Marginal[i]=0;
	}

	Controller=NULL; 
	Cross_Lane_Number=0;
	Conflict_Area_Number=0;
}

CCross::~CCross()
{

}

int CCross::Find_Connected_Link(CLink *p)
{
	int i;
	for(i=0;i<8;i++)
	{
		if(Connected_Link[i] == p->Object_ID )
			return 1;
	}
	return 0;
}
