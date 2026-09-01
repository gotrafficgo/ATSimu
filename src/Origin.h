#pragma once
#include "structure.h"

class CCommuter;

class COrigin
{
public:
	COrigin(int Located_Link_ID, int Located_Cell_ID, int Demand);   //if demand=-1, then it is dynamic demand, otherwise the demand is a constant number, the value of demand
	~COrigin(void);

public:
	int Located_Link_ID;
	int Located_Cell_ID;
	int Demand;
	int Time_To_Generate_Veh;
    int End_Link_ID;   //the initial value is -1, when construct FixDestOrigin, this value is given; when construct RanDestOrigin, this value is -1.
	CCommuter * Commuter_Array[MAX_COMMUTER_NUMBER_PER_ORIGIN];
	int Commuter_Number;

public:
	void Produce_Veh();
	int Get_Current_Demand();
	int Get_Load_Lane_ID();
	int Get_Headway(double lambda);
	int Get_Veh_Type();
	virtual Struct_Shortest_Path* Get_Veh_SPI(char Type);  //'T': tsp, 'D': dsp
	bool Is_It_Time();
	bool Is_It_Time_Prob();
	bool Is_It_Time_Headway();
	bool Is_It_Time_Time();

};
