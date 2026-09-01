#pragma once
#include "setting.h"

struct Struct_Shortest_Path
{
	int End_Link_ID;
	int Shortest_Path_Dist;
	int Shortest_Path[MAX_ROUTE_LENGTH]; 
};


struct Avg_K_Q
{
	double k;
	double q;
};

struct Loc_And_Time
{
	int Location;
	int Time;
};


struct Struct_Guidance_Strategy
{
	int This_Times;                                        
	int Link_ID;
	int Guidance_Dest_Size;
	int Guidance_Dest[MAX_GUIDANCE_DESTINATION];
	int Turn_Rate[MAX_GUIDANCE_DESTINATION][MAX_TURNING_DIRECTION];
};


struct Struct_Detector
{
	int Link_ID;
	int Detector_ID;
	int Left_Veh_Number;
	int Straight_Veh_Number;
	int Right_Veh_Number;

	double Left_Speed_Mean;
	double Straight_Speed_Mean;
	double Right_Speed_Mean;
};

struct Struct_Phase 
{
	int Phase_ID;
	int Green_Percent;
	int Green_Start_Time_Percent;
};

struct Struct_Control
{
	int Cross_ID;
	int Cycle_Time; 
	Struct_Phase Phase_Info_Array[MAX_PHASE_NUMBER];
};

struct Struct_Control_Received
{
	unsigned char MessageHead;
	int Blank;
	int Cross_ID;
	int Something;
	int Cycle_Time;  

	int Blank_1;        //just for occupancy
	int Blank_2;
	int Blank_3;
	int Blank_4;

	int Something_1;     
	int Phase_Length_1;

	int Something_2;
	int Phase_Length_2;

	int Something_3;
	int Phase_Length_3;

	int Something_4;
	int Phase_Length_4;
};

struct TranStruct_Guidance
{
	Struct_Guidance_Strategy Guidance_Buffer[MAX_LINK_NUMBER];
	int Link_Number;
};

struct Struct_Lane_Update
{
	unsigned char Data_Flag;        
	int Lane_ID; 
	char Arrived_Veh_Sum;
	char Passed_Veh_Sum;
	int Current_Veh_Sum;
	char Long_Veh_Sum;
	char Mean_Speed;
	char Occupancy;
	char Queue_Length; 
	char Queue_Delay_Time;   
};

struct Struct_Controllor_Update
{
	int Current_Time[6]; 
	int Simu_Time;
	int Cycle_Number;
	unsigned char Phase_ID;  
	unsigned char Phase_Time; 
	unsigned char Effective_Green_Time; 
	unsigned char Control_Type; 
	unsigned char Is_Control_Good;
	unsigned char Is_Detector_Good;
	unsigned char Is_Loop_Good;      
};

struct Struct_Update_Data 
{
	int Area_ID;
	int Cross_ID;
	Struct_Controllor_Update controllor_update_info;
	Struct_Lane_Update lane_update_info_array[MAX_LANE_INFO];
};

struct TranStruct_MBC_Detector
{
	unsigned char MessageHead;
	unsigned char SystemMark;
	int DataLength;
	Struct_Update_Data update_data_info;
	unsigned char MessageEnd;
};

struct TranStruct_SimuTime
{
	char Head;
	int Simu_Time;
};

struct Struct_FixedOD
{
	int Origin;
	int Dest;
	int Demand;
};