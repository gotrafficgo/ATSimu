#pragma once
#include "origin.h"

class CRanDestOrigin :	public COrigin
{
public:
	CRanDestOrigin(int Located_Link_ID, int Located_Cell_ID, int Demand);
	~CRanDestOrigin(void);

	int Get_Random_End_Link();
	Struct_Shortest_Path * Get_Veh_SPI();
};
