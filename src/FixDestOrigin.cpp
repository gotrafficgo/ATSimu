#include "stdafx.h"
#include "FixDestOrigin.h"
#include "Global.h"

CFixDestOrigin::CFixDestOrigin(int Located_Link_ID, int Located_Cell_ID, int Demand, int End_Link_ID)
	:COrigin(Located_Link_ID, Located_Cell_ID, Demand)
{
	this->End_Link_ID= End_Link_ID;
}

CFixDestOrigin::~CFixDestOrigin(void)
{
}


Struct_Shortest_Path * CFixDestOrigin::Get_Veh_SPI()
{
	Struct_Shortest_Path *spi;
	spi= Get_Shortest_Path(Located_Link_ID,End_Link_ID);
	return spi;
}