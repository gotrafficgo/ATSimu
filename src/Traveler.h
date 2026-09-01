#pragma once
#include "driver.h"

class CTraveler :
	public CDriver
{
public:
	CTraveler(int Traveler_ID, int Start_Link_ID, int End_Link_ID);
	~CTraveler(void);

	int Traveler_ID;
	int Start_Link_ID;
	int End_Link_ID;
	Struct_Shortest_Path *DSP_Between_OD;
	char Driver_Type;

//////////////////////////////////////////////
//from virtual CDriver
public:
	int Get_Start_Link_ID();
	int Get_End_Link_ID();
	Struct_Shortest_Path * Get_SP_Between_OD();
	char Get_Driver_Type();
	char Get_Current_Location();
	bool Able_To_Change_Route_On_Route();

	void Set_After_Arrival(int Arrival_Time);
	void Set_At_Day_Beginning();
	void Record_Travel_Experience_On_Link( int Simu_Time, int New_Link_ID);
	char Chosen_Veh_Color();
};
