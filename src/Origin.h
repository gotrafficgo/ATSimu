#pragma once
#include "structure.h"

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

public:
	void Produce_Veh();
	int Get_Current_Demand();
	int Get_Load_Lane_ID();
	int Get_Headway(double lambda);
	int Get_Veh_Type();
	virtual Struct_Shortest_Path* Get_Veh_SPI();
	bool Is_It_Time();
	bool Is_It_Time_Prob();
	bool Is_It_Time_Headway();
	bool Is_It_Time_Time();

};
