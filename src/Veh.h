#pragma once

#include "Link.h"
#include "Cross.h"
#include "setting.h"
#include "Lane.h"
#include "Cross_Lane.h"
#include "Conflict_Area.h"
#include "structure.h"

class CDriver;

class CVeh  
{
public:
	int Veh_Type; 
	int Driver_Type;
	int Current_Route_Array[MAX_ROUTE_LENGTH];	//route composed of link id, -1 means no link.
	int Current_Link_Index;                                        //index corresponding to the element in Current_Route_Array[], Current_Route_Array[Current_Link_Index] indicates current Link ID
	int Dest_Lane[MAX_LANE_NUMBER];                    //the lane that driver intend to change to
	int Veh_State;                                                      //the type of the lane vehicle is on. C: crossing; R: link
	int Return_Lane_ID;                                             //the lane id marked for coming back when passing another car.        the function of passing is not realized.
	int Start_Link_ID; 
	int End_Link_ID;
	int Receive_Guidance;                                     //-1: don't receive guidance info; 0: received but don't follow; 1: received and follow
	int Detected_Flag;
	int Cross_Move_Check_Time;                          //used for checking if the vehicles in this crossing have been processed in this simulation step
	int Waiting_Time;
	int Veh_ID;
	char Veh_Color;
	int Dest_Lane_Number;
	int Simutime_Passing_Detector_Middle;
	double Max_Speed_Percent;
	int Max_Speed;
	
	int Link_Loc;               //vehicle's current location and speed
	int Lane_Loc;	
	double Cur_Spd;
	int Cell_Loc;
	double Last_Spd;
	int Last_Cell_Loc;
	bool Freshman;    //symbol for if it has been recorded by trajectory.

	CDriver * The_Driver;

	CArray <Loc_And_Time*, Loc_And_Time*> Trajectory;

public:
	CVeh(int Veh_ID,int Veh_Type,  int Start_Cell_ID, CDriver * The_Driver);
//	CVeh(int Veh_ID, int Veh_Type, int Start_Cell_ID, int Start_Link_ID, int End_Link_ID, Struct_Shortest_Path * spi);
	virtual ~CVeh();
	void Veh_On_Link(int Cross_id,int Link_ID, int Lane_ID, int Cell_ID);
	void Drive_on_Dest_Lane(int Link_ID, int Lane_ID, int Cell_ID);
	void Drive_not_on_Dest_Lane(int Link_ID, int Lane_ID, int Cell_ID);
	int Follow_Guidance_or_Not(int Link_ID);
	bool Change_Route(char Type, int Current_Link_ID, int Next_Link);  //'T'-tsp, 'D'-dsp
	bool Change_Dest(int Current_Link_ID, int Next_Link);     //after a long time waiting, change destination
	void Enter_Cross(int Link_ID,int Lane_ID, int Cell_ID,int Cross_Lane_ID);
	void Veh_Run_on_Link(int Start_Link_ID, int Start_Lane_ID,int Start_Cell_ID,int End_Link_ID, int End_Lane_ID, int End_Cell_ID, double New_Speed);
	CVeh* Get_Front_Veh_on_Link(int Link_ID, int Lane_ID, int Cell_ID, int Last_Cell_ID);
	int Distance_Between_Front_Veh(int Link_ID, int Lane_ID, int Cell_ID);
	Struct_Shortest_Path* ShortestPath_with_Guidance(char Type, int Next_Link);
	CVeh * Delete_Current_Link_Cell_Veh(int Link_ID, int Current_Lane_ID, int Current_Cell_Position);
	CVeh * Delete_Current_Intersection_Cell_Veh(int Cross_ID,int Cross_Lane_ID, int Cell_ID);
	CVeh *Delete_Current_Cross_Cell_Veh(int Cross_id,int Cross_Lane_ID, int Cell_ID);
	int Change_Lane(int Link_ID, int Lane_ID, int Cell_ID, int Cur_Location, double Cur_Speed);
//	bool Have_Space_In_Target_Lane(int Link_ID, int Lane_ID, int Cell_ID);
	int Get_Lane_Number_to_Dest_Lane(int Current_Lane_ID);
	int Get_Nearest_Dest_Lane_ID( int Current_Lane_ID);
	bool Decide_Change_Lane(int Cell_ID);
	int Get_Dest_Lane(int Lane_ID);
	void Veh_in_Cross(int Cross_ID, int Cross_Lane_ID,int Cell_ID);
	void Enter_Link(int Cross_id,int Cell_ID,int Cross_Lane_ID);
	void Enter_Link(int Current_Link, int Current_Lane, int Current_Cell, int Next_Link, int Next_Lane);
	void Veh_Run_in_Cross(int Cross_ID,int Start_Cross_Lane_ID,int Start_Cell_ID,int End_Cross_Lane_ID, int End_Cell_ID);
	int Move_Veh_in_Cross(CVeh *p, int Cross_id, int Cross_Lane_ID, int Move_Cell_ID); 
	int Move_Veh_on_Link(CVeh *p, int Move_Link_ID, int Move_Lane_ID, int Move_Cell_ID);      //Line_Serial_Number,Cell_Serial_Number)
	int Response_to_Guidance(int Link_ID);
	bool Last_Route_Link(Struct_Shortest_Path *spi);
	void Set_Dest_Lane();            //set destination lane
	void Set_Dest_Lane(int Link_ID);	//overload, used when generating vehicle
	bool Dest_Lane_or_Not(int Lane_Number,int Lane_ID);
	void Clear_Dest_Lane_Array();          //before entering next link, clear Dest_Lane_Array[]
	int Choose_Cross_Lane(int Cross_ID,int Current_Link_ID,int Current_Lane_ID);
	int Get_Turn_Direction(int Current_Link_ID, int Next_Link_ID);
	void After_Long_Time_Waiting(int Link_ID,int Lane_ID);      //vehicle has wait for a long time in no-destination lane
	bool Priority_or_Not(int Cross_Lane_Type,int Cross_ID, CCell* Current_Cell,  int Conflict_Area_ID);	//used in determining who go first in front of a conflict
	bool Conflict_Occupied_or_Not(int Cross_Lane_Type,int Cross_ID, int Conflict_Area_ID);
	bool Enter_Conflict_Area_or_Not(int Cross_Lane_Type, int Cross_ID, CCell * Next_Cell);    
	bool Platoon_or_Not(int Cross_ID, CCell *Current_Cell);
	int Get_Entering_Conflict_Area_ID(int Cross_Lane_Type,int Conflict_Area_Flag,int Cross_ID, CCell *Next_Cell) ;
	int Match_Veh_Number_on_Links( int Line_A, int Line_B, int Link_ID, int Cell_ID);
	int Choose_in_Dest_Lanes(int Link_ID, int Lane_ID, int End_Cell_ID);
	double Set_Max_Speed_Percent();
	int Set_Init_Speed(int Start_Cell_ID);
	bool Leave_Network_Or_Not(int Link_ID, int Lane_ID, int Start_Cell_ID,  int End_Cell_ID);
	void Leave_Network_From_Link(int Link_ID, int Lane_ID, int Cell_ID);
	void Leave_Network_From_Intersection(int Cross_ID, int Cross_Lane_ID, int Cell_ID);
	bool Able_To_Enter_Cross(int Link_ID, int Lane_ID, int Cell_ID, int Supposed_End_Cell_ID);
	void  Set_Veh_Color();
	void Update_Loc_And_Spd(CVeh *p, int New_Link_ID, int New_Lane_ID, int New_Cell_ID, double New_Speed);
	void Update_Loc_And_Spd(CVeh *p, double New_Speed);   //used in intersection
	int Get_Wait_Cell_ID(int Link_ID, int Lane_ID);
	bool Able_To_Pass_Left_Through_Conflict(int Cross_Lane_Turn_Direction, int Cross_ID, CCell *Current_Cell, CCell *Prob_Next_Cell);
	bool Able_To_Pass_Through_Through_Conflict(int Cross_ID, CCell *Next_Cell);
	bool Have_Space_Next_Link(int Front_Cross_ID, int Front_Cross_Lane_ID);
// 	int Get_Cell_Number_To_Last_Veh(int Link_ID, int Lane_ID, CVeh* pVeh);
// 	int Get_Veh_Number_On_Cross_Lane( int Front_Cross_ID, int Front_Cross_Lane_ID);
	void Collect_Trajectory(int Last_Link_ID, int Last_Lane_ID, int New_Link_ID, int New_Lane_ID, int Cell_ID);
// 	CVeh* Get_Last_Veh(int Link_ID, int Lane_ID);
	int Get_Veh_Number_In_Checking_Region(int Link_ID, int Lane_ID, int Region);
	bool Begin_Merge_At_Some_Location(int Link_ID, int Cell_ID);
	void Set_On_Link_Max_Speed(int Link_ID);
	double Get_Cur_Speed(int Veh_Loc, double Veh_Spd, int FrontVeh_Loc, double FrontVeh_Spd, double Max_Speed);
	int Get_Cur_Location(int Cur_Loc, double Cur_Spd);

};

