#include "stdafx.h"
#include "Driver.h"


CDriver::CDriver()
{

}

CDriver::~CDriver(void)
{
}


double CDriver::Set_Max_Speed_Percent()
{
/*
	double percent;
	switch (this->Driver_Type)
	{
	case 0:
		percent= MAX_SPEED_PERCENT_1;
		break;
	case 1:
		percent= MAX_SPEED_PERCENT_2;
		break;
	case 2:
		percent= MAX_SPEED_PERCENT_3;
		break;
	}
	return percent;
*/
	return MAX_SPEED_PERCENT_1;
}



int CDriver::Get_Start_Link_ID()
{
	return -1;
}
	

int CDriver::Get_End_Link_ID()
{
	return -1;
}

Struct_Shortest_Path * CDriver::Get_SP_Between_OD()
{
	return NULL;
}

char CDriver::Get_Driver_Type()
{
	return 'X';
}

char CDriver::Get_Current_Location()
{
	return 'X';
}

void CDriver::Set_After_Arrival(int Arrival_Time)
{

}

void CDriver::Set_At_Day_Beginning()
{

}


void CDriver::Record_Travel_Experience_On_Link(int Simu_Time, int New_Link_ID)
{

}

bool CDriver::Able_To_Change_Route_On_Route()
{
	return false;
}


char CDriver::Chosen_Veh_Color()
{
	return 'X';
}