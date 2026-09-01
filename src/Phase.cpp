// Phase.cpp: implementation of the CPhase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "traffic.h"
#include "Phase.h"
#include "Global.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPhase::CPhase()
{

}

CPhase::~CPhase()
{

}

CPhase::CPhase(int phase_id,   int green_percent,   int direction,   int cycle_time,    int green_start_time_percent,    int connect_link_array[])
{
	Phase_ID=phase_id;
	Green_Percent=green_percent;
	Cycle_Time=cycle_time;
	Green_Start_Time=int(cycle_time*green_start_time_percent/100);
	Direction= direction;
	Last_Color= 'N';
	Phase_Last_Refresh_Time=0;
	for (int i=0; i<LINK_IN_ONE_PHASE; i++)
		Connect_Link_Array[i]=connect_link_array[i];
}

void CPhase::Set_Green_Percent(int New_Green_Percent)
{
	Green_Percent=New_Green_Percent;
}


void CPhase::Set_Cycle_Time(int New_Cycle_Time)
{
	Cycle_Time=New_Cycle_Time;
}


void CPhase::Set_Green_Start_Time(char New_Green_Start_Time_Percent)
{
	Green_Start_Time= int (Cycle_Time * New_Green_Start_Time_Percent / 100);
}


char CPhase::Get_Current_Color()
{
	char Current_Color='0';
	int Green_Time;
	int Time_in_Phase;

	Time_in_Phase= (simu_time-Phase_Last_Refresh_Time)%Cycle_Time;	
	Green_Time= int(Green_Percent*Cycle_Time/100);

	if (Green_Start_Time + Green_Time <=Cycle_Time)
	{
		if (Time_in_Phase < Green_Start_Time)   
			Current_Color='R';
		if (Green_Start_Time <=Time_in_Phase && Time_in_Phase <Green_Start_Time+Green_Time)
			Current_Color='G';
		if (Green_Start_Time+Green_Time <=Time_in_Phase)
			Current_Color='R';
	}
	else
	{
		int Green_Time_2=Cycle_Time - Green_Start_Time;
		int Green_Time_1=Green_Time-Green_Time_2;

		if (Time_in_Phase < Green_Time_1)   
			Current_Color='G';
		if (Green_Time_1 <=Time_in_Phase && Time_in_Phase <Green_Start_Time)
			Current_Color='R';
		if (Green_Start_Time <=Time_in_Phase)
			Current_Color='G';		
	}

	return Current_Color;
}

int CPhase::Get_Left_Time()
{
	int Left_Time=-1;
	int Green_Time;
	int Current_Color;
	int Time_in_Phase;

	Time_in_Phase= (simu_time-Phase_Last_Refresh_Time)%Cycle_Time;
	Green_Time=int(Green_Percent*Cycle_Time/100);
	Current_Color=Get_Current_Color();

	if (Time_in_Phase < Green_Start_Time)    
	{
		Left_Time= Green_Start_Time- Time_in_Phase;
	}
	if (Green_Start_Time <=Time_in_Phase && Time_in_Phase <Green_Start_Time+Green_Time)
	{
		Left_Time= Green_Start_Time+Green_Time - Time_in_Phase;
	}
	if (Green_Start_Time+Green_Time <=Time_in_Phase && Time_in_Phase<Cycle_Time)   
	{
		Left_Time= Cycle_Time - Time_in_Phase;
	}
	
	return Left_Time;
}


Info_Light_Color_and_Time CPhase::Get_Phase_LCT() 
{
	extern char Current_Control_Type;
	Info_Light_Color_and_Time LCT;

	if (Current_Control_Type=='N')
	{
		LCT.Left_Time=1000;
		LCT.Light_Color='G';
		return LCT;
	}
	
	int Time_in_Phase;     
	Time_in_Phase= (simu_time-Phase_Last_Refresh_Time)%Cycle_Time;        //0---Cycle_Time-1

	LCT.Light_Color= Get_Current_Color();
	LCT.Left_Time= Get_Left_Time();
	
	return LCT;	
}

void CPhase::Set_Phase_Last_Refresh_Time()
{
	Phase_Last_Refresh_Time=simu_time;
}