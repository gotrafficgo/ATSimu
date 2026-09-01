#pragma once

#include "driver.h"
#include "setting.h"

class CCommuter :
	public CDriver
{
public:
	CCommuter(int Commuter_ID, int Commuter_Type, int Home_Location, int Work_Location,  int Required_Arrival_Time);
	~CCommuter(void);
	int Choose_Route_And_Predict_Travel_Time();
	bool Home_Or_Not();     //is it at home currently
	bool Depart_Or_Not();    //is it time to depart.
	void Set_Actual_Arrival_Time(int time);
	void Experience_Accumulation();
	bool Is_It_Time_To_Depart(int Predicted_Time, int Current_Time);
	int Get_Current_Travel_Time_On_Chosen_Route();   //Get current travel time on chosen route based on provided information
	int Departure_Time_Array[MAX_SIMU_DAY_NUMBER];   //save the departure times every day

public:
	Struct_Shortest_Path *Chosen_Route;
	int Home_Location;   //Start_Link_ID;
	int Work_Location;  //End_Link_ID;
	int Predicted_Travel_Time;  //
	int Required_Arrival_Time;   
	int Actual_Arrival_Time; 
	int Departure_Time;   //Departure_Tiime= Required_Arrival_Time - Predicted_Travel_Time
	int Commuter_ID;
	char Driver_Type;
	int Commuter_Type;    //type: 1,2,3,4,5
	char Current_Location;
	int Enter_Link_Time;
	int Current_Link_ID;
	int Network_In_Mind[MAX_LINK_NUMBER];

	//Travel_Time_In_Memory[0][]: travel time on links for decision; 
	//Travel_Time_In_Memory[i][]: if not fftt, actual travel time in day i
	//this travel time include the time on links and the time on intersections
	int Travel_Time_In_Memory[MAX_SIMU_DAY_NUMBER+1][MAX_LINK_NUMBER];    //see above

//////////////////////////////////////////////
//from virtual CDriver
public: 
	char Get_Driver_Type();
	int Get_Start_Link_ID();
	int Get_End_Link_ID();
	Struct_Shortest_Path* Get_SP_Between_OD();
	char Get_Current_Location();
	bool Able_To_Change_Route_On_Route();

	void Set_After_Arrival(int Arrival_Time);
	void Set_At_Day_Beginning();
	void Set_Experienced_Travel_Time(int Link_ID, int Experienced_Travel_Time);
	void Record_Travel_Experience_On_Link( int Simu_Time, int New_Link_ID);
	char Chosen_Veh_Color();

};

