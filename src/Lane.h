#pragma once

#include "setting.h"
#include "Phase.h"
#include "point.h"
class CCell;

class CLane  
{
public:
	CLane();
	virtual ~CLane();
	int Set_Value(Cpoint, Cpoint);	  //return cell number of this lane
	int Set_Lane_Cell(Cpoint , Cpoint );

public: 
	int Lane_ID;
	int Cell_Number;
	CCell ** Lane_Cell;        //dynamic pointer array; refer to CLane();
	bool Left_Turn;                                               //true: left turn     false: no left turn
	bool Straight_Turn;                                         //true or false
	bool Right_Turn;                                            //true or false
	int Prohibit_Change_Lane_Cell_ID;  
	CPhase *Phase;                                                //phase is on a lane; i.e., a lane contain a phase
	int Free_Flow_Travel_Time;

};
