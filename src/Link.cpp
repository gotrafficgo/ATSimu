
#include "stdafx.h"
#include "traffic.h"
#include "Link.h"
#include "MyObject.h"
#include "MarginalPoint.h"
#include "Cross.h"
#include "setting.h"
#include "math.h"
#include "Lane.h"
#include "ErrorLog.h"
#include "Global.h"
#include "Origin.h"
#include "Dest.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CLink::CLink()
{
}

CLink::CLink(CMyObject *start, CMyObject *end)
{
	if(start->Object_Type=='M')
		Start_Object = (CMarginalPoint*)start;
	if(start->Object_Type=='C')
		Start_Object = (CCross*)start;
	if(end->Object_Type=='M')
		End_Object = (CMarginalPoint*)end;
	if(end->Object_Type=='C')
		End_Object = (CCross*)end;

	for (int i=0;i<MAX_DETECTOR_NUMBER;i++)
		Detector_Array[i]=NULL;

	Next_Left_Link=-1;
	Next_Right_Link=-1;
	Next_Straight_Link=-1;

	Queue_Length_0_for_Control=0; 
	Queue_Length_1_for_Control=0;  
	Queue_Length_2_for_Control=0;  
	Queue_Length_0_for_Guidance=0;
	Queue_Length_1_for_Guidance=0;  
	Queue_Length_2_for_Guidance=0;  

	isShowGuidanceBoard=false;

	this->On_Link_Veh_Number=0;
	this->Sum_Of_Speed=0;

	Current_On_Link_Time=0;

	Is_Origin=false;
	Is_Dest=false;
	Limited_Speed=0;

	for (int i=0; i<MAX_ORIGIN_NUMBER;i++)
		this->Origin_Array[i]=NULL;
	Origin_Number=0;
	this->Dest=NULL;
}
CLink::~CLink()
{

}

int CLink::Get_Quad(Cpoint Start_Point, Cpoint End_Point)
{
	int x1=(int)Start_Point.x;
	int y1=(int)Start_Point.y;
	int x2=(int)End_Point.x;
	int y2=(int)End_Point.y;
	
	if(y1-y2<=0 && x1-x2>0)
		return 1;
	if(y1-y2<0 && x1-x2<=0) 
		return 2;
	if(y1-y2>=0 && x1-x2<0) 
		return 3;
	if(y1-y2>0 && x1-x2>=0) 
		return 4;
	return -1;
}

int CLink::Get_Min(int a, int b)
{
	int Min;
	if (a>b)
		Min=b;
	else
		Min=a;
	return Min;
}

void CLink::Enlarge_Link()
{
	int j;
	Cpoint new_p[2];
	Cpoint old_p[2];
	double sink,cosk,tank;
	old_p[0] = Start_Object->Object_Coodinate;
	old_p[1] = End_Object->Object_Coodinate;
	Leftest_Line_Start = old_p[0];
	Leftest_Line_End = old_p[1];
	for(j=0;j <Lane_Number;j++)
	{
		if(old_p[0].x-old_p[1].x == 0)
		{
			if(old_p[0].y-old_p[1].y >=0 )
			{
				new_p[0].x = old_p[0].x-LANE_WIDTH;
				new_p[0].y = old_p[0].y;
				new_p[1].x = old_p[1].x-LANE_WIDTH;
				new_p[1].y = old_p[1].y;
			}
			else
			{
				new_p[0].x = old_p[0].x+LANE_WIDTH;
				new_p[0].y = old_p[0].y;
				new_p[1].x = old_p[1].x+LANE_WIDTH;
				new_p[1].y = old_p[1].y;
			}
		}
		else
		{
			sink=double(old_p[0].y-old_p[1].y)/(sqrt(double(old_p[0].y-old_p[1].y)*double(old_p[0].y-old_p[1].y)+double(old_p[0].x-old_p[1].x)*double(old_p[0].x-old_p[1].x)));
			cosk=double(old_p[0].x-old_p[1].x)/(sqrt(double(old_p[0].y-old_p[1].y)*double(old_p[0].y-old_p[1].y)+double(old_p[0].x-old_p[1].x)*double(old_p[0].x-old_p[1].x)));
			tank=sink/cosk;
			if(tank>0)
			{
				new_p[0].x = old_p[0].x-LANE_WIDTH*sink;
				new_p[0].y = old_p[0].y+LANE_WIDTH*cosk;
				new_p[1].x = old_p[1].x-LANE_WIDTH*sink;
				new_p[1].y = old_p[1].y+LANE_WIDTH*cosk;
			}
			if(tank<0)
			{
				new_p[0].x = old_p[0].x-LANE_WIDTH*sink;	//	Rightest_Line_Start = new_p[0];
				new_p[0].y = old_p[0].y+LANE_WIDTH*cosk;   //  Rightest_Line_End = new_p[1];
				new_p[1].x = old_p[1].x-LANE_WIDTH*sink;
				new_p[1].y = old_p[1].y+LANE_WIDTH*cosk;
			}
			if(tank==0)
			{
				if(old_p[0].x-old_p[1].x >=0)
				{
					new_p[0].x = old_p[0].x;
					new_p[0].y = old_p[0].y+LANE_WIDTH;
					new_p[1].x = old_p[1].x;
					new_p[1].y = old_p[1].y+LANE_WIDTH;
				}
				else
				{
					new_p[0].x = old_p[0].x;
					new_p[0].y = old_p[0].y-LANE_WIDTH;
					new_p[1].x = old_p[1].x;
					new_p[1].y = old_p[1].y-LANE_WIDTH;
				}
			}
		}

		Lane_Start_Point[j] = new_p[0];
		Lane_End_Point[j] = new_p[1];
		if(j == Lane_Number-1)
		{
			Rightest_Line_Start = new_p[0];
			Rightest_Line_End = new_p[1];
		}
		Lane_Middle_Start_Point[j].x = (old_p[0].x+new_p[0].x )/2;
		Lane_Middle_Start_Point[j].y = (old_p[0].y+new_p[0].y )/2;
		Lane_Middle_End_Point[j].x = (old_p[1].x +new_p[1].x )/2;
		Lane_Middle_End_Point[j].y = (old_p[1].y +new_p[1].y )/2;
		old_p[0] = new_p[0];
		old_p[1] = new_p[1];
	}
}

void CLink::Get_Label_Coordinate()
{
	int x1,y1;
	int x2,y2;
	int x,y;

	x1= int((Leftest_Line_Start.x+ Leftest_Line_End.x)/2);
	y1= int((Leftest_Line_Start.y+ Leftest_Line_End.y)/2);

	x2= int((Rightest_Line_Start.x+ Rightest_Line_End.x)/2);
	y2= int((Rightest_Line_Start.y+ Rightest_Line_End.y)/2);

	if (x2<x1)
	{
		x=4*x2-3*x1;
		y=4*y2-3*y1;
	}
	else if (x2==x1 || y1==y2)
	{
		x=3*x2-2*x1;
		y=3*y2-2*y1;
	}
	else
	{
		x=2*x2-x1;
		y=2*y2-y1;
	}

	Label_Coordinate.x=x;
	Label_Coordinate.y=y;

}


void CLink::Add_Origin(COrigin * An_Origin)
{
	this->Origin_Array[Origin_Number]=An_Origin;
	Origin_Number++;
}


