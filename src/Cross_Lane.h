#pragma once

#include "point.h"
#include "setting.h"
#include "Cell.h"

class CCross_Lane  
{
public:
	int Cross_ID; 
	int Start_Link_ID;
	int Start_Lane_ID;
	int End_Link_ID; 
	int End_Lane_ID;
	int Cell_Number;
	int Cross_Lane_ID;  
	int Turn_Direction;             //0 left turn; 1 through traffic; 2 right turn; 3 u turn
	CCell *Cross_Lane_Cell[MAX_CROSS_LANE_CELL_NUMBER]; 
	
public:
	CCross_Lane();
	virtual ~CCross_Lane();
	Cpoint Circle_Intersect_Line(Cpoint Point_on_Line , double Line_Slope , bool K_Flag, Cpoint Circle_Center, double Radius,Cpoint Point_for_Contrast,  int Flag);
	int Get_Cell_Coordinate_in_Arc(Cpoint End_Point, Cpoint Start_Point, int Cross_Lane_ID);                        //bow between two points
	int Get_Straight_and_Right_Cell_Coordinate_in_Arc(Cpoint Start_Point, Cpoint End_Point, int Cross_Lane_ID);     //to generate arc for through and right turn lane
};

