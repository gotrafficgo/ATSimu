#pragma once

struct Add_Decrease_Speed
{
	int Add_Speed_Rate;
	int Decrease_Speed_Rate;
};

#include "Guidance.h"
#include "MyObject.h"
#include "setting.h"
#include "Detector.h"
#include "DlgGuidanceBoard.h"

class COrigin;
class CDest;
class CLane;

class CLink : public CMyObject  
{
public:
	CLink(CMyObject *, CMyObject *);
	virtual ~CLink();
	int Get_Quad(Cpoint Start_Point, Cpoint End_Point);
	int Get_Min(int a, int b);
	void Get_Label_Coordinate();
	void Enlarge_Link();
	void Add_Origin(COrigin * An_Origin);

public:
	COrigin *Origin_Array[MAX_ORIGIN_NUMBER];
	int Origin_Number;
	CDest * Dest;
	CMyObject *Start_Object; 
	CMyObject *End_Object; 
	Cpoint Lane_Middle_Start_Point[5];   //coordinate of start points of every middle line of lane
	Cpoint Lane_Middle_End_Point[5];
	Cpoint Lane_Start_Point[5];               //coordinate of start point of every lane line
	Cpoint Lane_End_Point[5]; 
	Cpoint Rightest_Line_Start;                //coordinate of start point of far right line of this link
	Cpoint Rightest_Line_End;                     
	Cpoint Leftest_Line_Start;
	Cpoint Leftest_Line_End;
	Cpoint Label_Coordinate;                 //link label
	CDetector *Detector_Array[MAX_DETECTOR_NUMBER];   //Detector_Array[0] intersection detector
	CGuidance *guidance;
	CRect Guidance_Board;                      //location of guidance board
	CDlgGuidanceBoard * dlgGuidanceBoard;       //dlg shown when double click guidance board
	bool isShowGuidanceBoard;
	int Lane_Number; 
	int Limited_Speed;      //unit is cell number
	int Next_Left_Link;            //next link id, if turn left on this link
	int Next_Straight_Link;
	int Next_Right_Link;
	int Next_UTurn_Link;                  
	int Queue_Length_0_for_Control;
	int Queue_Length_1_for_Control;  
	int Queue_Length_2_for_Control;  
	int Queue_Length_0_for_Guidance;
	int Queue_Length_1_for_Guidance;  
	int Queue_Length_2_for_Guidance;  
	int On_Link_Veh_Number;
	double Sum_Of_Speed;
	int Current_On_Link_Time;          // real on-link time for guidance information
	bool Is_Dest;
	bool Is_Origin;
	
	CLane * Lanes[MAX_LANE_NUMBER];
	int Length_In_Cell;   //cell number
	int Length_In_FFTT;  // free flow travel time on links + waiting time for intersection(=cycle time/ phase number) ; being evaluated in CSimuFun::Set_Lane_Cell_Coordinate()
	
};

