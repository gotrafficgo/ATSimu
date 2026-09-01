#pragma once
#include "setting.h"
struct Struct_Shortest_Path;

class CDriver
{
public:
	CDriver();
	~CDriver(void);
	double Set_Max_Speed_Percent();
	double Max_Speed_Percent;
	char Current_Location;

public:
	//getting
	virtual char Get_Driver_Type();
	virtual char Get_Current_Location();
	virtual int Get_Start_Link_ID();
	virtual int Get_End_Link_ID();
	virtual Struct_Shortest_Path * Get_SP_Between_OD();
	virtual bool Able_To_Change_Route_On_Route();
	
	//setting
	virtual void Set_After_Arrival(int Arrival_Time);
	virtual void Set_At_Day_Beginning();
	virtual void Record_Travel_Experience_On_Link( int Simu_Time, int New_Link_ID);
	virtual char Chosen_Veh_Color();

};