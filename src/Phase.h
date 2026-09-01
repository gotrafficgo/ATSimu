#pragma once

struct Info_Light_Color_and_Time
{
	char Light_Color;
	int Left_Time;
};

#include "setting.h"

class CPhase  
{
friend class CController;
private:
	int Green_Percent;        // green time/cycle
	int Cycle_Time;
	int Green_Start_Time;   //start time of green light
	int Phase_ID;                //ID in a controller
	int Phase_Last_Refresh_Time;
	void Set_Green_Percent(int New_Green_Percent);
	void Set_Cycle_Time(int New_Cycle_Time);
	void Set_Green_Start_Time(char New_Start_Color);
	void Set_Phase_Last_Refresh_Time();

public:
	int Connect_Link_Array[LINK_IN_ONE_PHASE]; 
	int Direction;   
	CPhase();
	CPhase(int phase_id, int green_percent, int direction, int cycle_time, int green_start_Time, int connect_link_array[]);
	virtual ~CPhase();
	char Get_Current_Color();
	int Get_Left_Time();
	Info_Light_Color_and_Time Get_Phase_LCT();
	char Last_Color;
};

