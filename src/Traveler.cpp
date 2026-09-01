#include "stdafx.h"
#include "Traveler.h"
#include "Global.h"


/*    //assign a dsp when construct the traveler
CTraveler::CTraveler(int Traveler_ID, int Start_Link_ID, int End_Link_ID, Struct_Shortest_Path *dsp)
:CDriver()
{
	this->DSP_Between_OD= dsp;
	Max_Speed_Percent= Set_Max_Speed_Percent();     //Driver_Type must exist prior to Set_Max_Speed_Percent()

	this->Start_Link_ID = Start_Link_ID;
	this->End_Link_ID = End_Link_ID;
	this->Driver_Type='T';   //Traveler
	this->Traveler_ID= Traveler_ID;
}
*/


//calculate dsp with link+errors when it is generated, but it will make the simulation slower as the generation number of travelers.
CTraveler::CTraveler(int Traveler_ID, int Start_Link_ID, int End_Link_ID)
:CDriver()
{
	int Network_In_Mind[MAX_LINK_NUMBER];

	//generate the shortest path according to the fftt and perception errors(Get_Random_Number(-10, 10))
	for (int link_i=0; link_i<G_Link_Number; link_i++)
	{ 
		//initial travel time in memory: all are filled with fftt except [][0], because it is for prediction
		//commuters update Travel_Time_In_Memory[][] by experienced travel time in the corresponding position before entering new links.
		int FFTT= Link_Array[link_i]->Length_In_FFTT;
		int Error_Percentage= Get_Random_Number(-10, 10)/100;  //perception errors  -10~10%
		Network_In_Mind[link_i]=int(FFTT*(1- Error_Percentage));    
	}
	
	this->DSP_Between_OD=Get_Shortest_Path(Network_In_Mind, Start_Link_ID, End_Link_ID);

	Max_Speed_Percent= Set_Max_Speed_Percent();     //Driver_Type must exist prior to Set_Max_Speed_Percent()

	this->Start_Link_ID = Start_Link_ID;
	this->End_Link_ID = End_Link_ID;
	this->Driver_Type='T';   //Traveler
	this->Traveler_ID= Traveler_ID;
}

CTraveler::~CTraveler(void)
{
}


int CTraveler::Get_Start_Link_ID()
{
	return Start_Link_ID;
}
	
int CTraveler::Get_End_Link_ID()
{
	return End_Link_ID;
}

Struct_Shortest_Path * CTraveler::Get_SP_Between_OD()
{
	return DSP_Between_OD;
}

char CTraveler::Get_Driver_Type()
{
	return Driver_Type;
}

char CTraveler::Get_Current_Location()
{
	return Current_Location;
}

void CTraveler::Set_After_Arrival(int Arrival_Time)
{
}

void CTraveler::Set_At_Day_Beginning()
{
}


void CTraveler::Record_Travel_Experience_On_Link( int Simu_Time, int New_Link_ID)
{
}

bool CTraveler::Able_To_Change_Route_On_Route()
{
	return false;
}


char CTraveler::Chosen_Veh_Color()
{
	return 'G';
}