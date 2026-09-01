#include "stdafx.h"
#include "RanDestOrigin.h"
#include "setting.h"
#include "Global.h"

CRanDestOrigin::CRanDestOrigin(int Located_Link_ID, int Located_Cell_ID, int Demand)
		:COrigin(Located_Link_ID, Located_Cell_ID, Demand)
{
	
}

CRanDestOrigin::~CRanDestOrigin(void)
{
}


Struct_Shortest_Path * CRanDestOrigin::Get_Veh_SPI(char Type)  //'T': tsp, 'D': dsp
{
	int End_Link_ID;
	int i=0;
	Struct_Shortest_Path *spi;
    do 
	{
		End_Link_ID= Get_Random_End_Link();	

		if (End_Link_ID==-1)    //cannot find a End_Link_ID after a long time iteration.
			return NULL;

		//End_Link_ID=99;   //0505
		spi= Get_Shortest_Path('D', Located_Link_ID,End_Link_ID);

		///////////////////////////////////////////
		if(Long_Time_Iteration(&i))
			return NULL;
		///////////////////////////////////////////

	} while (spi->Shortest_Path[MIN_LINK_NUMBER]<0 ||End_Link_ID==Located_Link_ID);

	return spi;

}



int CRanDestOrigin::Get_Random_End_Link()
{
	int Sum_Of_Link= G_Link_Number;
	int i=0;

	while(true)
	{
		int ran= Get_Random_Number(0, Sum_Of_Link-1);   // random number between 0 and (SumOfLink-1)

		if (Link_Array[ran]->Dest != NULL)
			return ran;

		///////////////////////////////////////////
		if(Long_Time_Iteration(&i))
			return -1;
		///////////////////////////////////////////

		Sleep(1);
	}

	return -1;
}