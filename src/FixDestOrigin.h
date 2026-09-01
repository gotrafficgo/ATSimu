#pragma once
#include "origin.h"
#include "setting.h"

class CFixDestOrigin :	public COrigin
{
public:
	CFixDestOrigin(int Located_Link_ID, int Located_Cell_ID, int Demand, int End_Link_ID);
	~CFixDestOrigin(void);
	Struct_Shortest_Path * Get_Veh_SPI(char Type);  //'T': tsp, 'D': dsp
	int End_Link_ID;
};
