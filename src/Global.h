#pragma once

#include "MarginalPoint.h"  
#include "Link.h"       
#include "MyObject.h"
#include "Cross.h"      
#include "Lane.h"       
#include "Cross_Lane.h"
#include "Conflict_Area.h"
#include "structure.h"
#include "point.h"

#include "SimuFun.h"
#include "SampleCollection.h"
#include "ErrorLog.h"

double Point_Distance(Cpoint a1, Cpoint a2);
double Point_Distance(double x1, double y1, double x2, double y2);
Cpoint Get_Intersection_Point(Cpoint Line_1_Start, Cpoint Line_1_End, Cpoint Line_2_Start, Cpoint Line_2_End);  //calculate the intersection between the far right lines of  two links
Cpoint Get_Vertical_Point(Cpoint point, Cpoint start , Cpoint end );
Cpoint Get_Middle_Point(Cpoint point1, Cpoint point2);
int Get_New_Coordinate(int XorY, char Flag); 
int Get_Max_Value(int a, int b);
int Get_Max_Value(int a, int b, int c);
double Get_Min_Value(double a, double b);
int Get_Min_Value(int a, int b);
int Get_New_Mean(int Average, int Sum_without_This_Value, int Value);
double Get_New_Mean(double Average, int Sum_without_This_Value, double Value);
Struct_Shortest_Path* Shortest_Path(char Type, int Start_Link_ID, int End_Link_ID );
Struct_Shortest_Path* Get_Shortest_Path(int Travel_Time_On_Links[MAX_LINK_NUMBER], int Start_Link_ID, int End_Link_ID);
Struct_Shortest_Path * Get_Shortest_Path(char Type, int Start_Link_ID, int End_Link_ID);   //'T'-get tsp; 'D'-get dsp
bool True_Or_False(double Prob, int Max_Value);
bool Check_Load_or_Not();
bool Long_Time_Iteration(int* i);
int Get_Random_Number(int Min, int Max);
CString Get_Current_Directory();
char * CStringToCharStar(CString str);
bool Have_Space_In_Target_Lane(int Link_ID, int Lane_ID, int Cell_ID);
void Record_Setting();
extern CMarginalPoint *MarginalPoint_Array[MAX_MARGINALPOINT_NUMBER];
extern CCross *Cross_Array[MAX_CROSS_NUMBER];
extern CLink *Link_Array[MAX_LINK_NUMBER];
extern CLink *Node_Relation[MAX_MARGINALPOINT_NUMBER+MAX_CROSS_NUMBER][MAX_MARGINALPOINT_NUMBER+MAX_CROSS_NUMBER];	// table for network data; Max_Cross_Number is for intersection, the other for marginal points; filled based on user's input
extern CCross_Lane  *Cross_Lane_Array[MAX_CROSS_NUMBER][MAX_CROSS_LANE_NUMBER];				
extern CConflict_Area *Conflict_Area_Array[MAX_CROSS_NUMBER][MAX_CONFLICT_AREA_NUMBER]; 
extern Struct_FixedOD FixedOD_Array[MAX_FixedOD_NUMBER];
extern double Demand_Array[MAX_DEMAND_NUMBER][2];
extern int Network_Structure[MAX_MARGINALPOINT_NUMBER+MAX_CROSS_NUMBER][MAX_MARGINALPOINT_NUMBER+MAX_CROSS_NUMBER];

extern Struct_Shortest_Path *Distance_Shortest_Path_Array[MAX_LINK_NUMBER][MAX_LINK_NUMBER];  
extern Struct_Shortest_Path *Time_Shortest_Path_Array[MAX_LINK_NUMBER][MAX_LINK_NUMBER];

extern int simu_time;
extern int current_day;
extern int time_in_current_day;

extern int Received_Guidance_Number;
extern double zoom_ratio;  
extern CSize m_sizeEllipse;  
extern CPoint   m_pointTopLeft;  
extern int Sleep_Time;
extern int Total_Veh_Number;
extern int Out_Veh_Number;
extern int Commuter_Number_On_Network;  //commuter # on network, do not have relation with Commuter_ID
extern int Traveler_Number_On_Network;     //traveler # on network; since travelers will not come back once leave the network, it is used to label Traveler_ID

extern char Current_Control_Type;

extern int G_Link_Number;
extern int G_Cross_Number;
extern int G_MarginalPoint_Number;
extern int G_Lane_Number;
extern int G_FixedOD_Number;
extern int G_Demand_Number;

extern bool Yellow_Intersection;
extern bool Conflict_Flag;

extern int Start_Time_Guidance;
extern bool Switch_Guidance;
extern bool Display_Flag;

extern bool Switch_Thread;	

extern CSimuFun *simuFun;
extern CSampleCollection *pSampleCollection;
extern ErrorLog *err;
extern int Start_Time_Detector;
extern bool Switch_Detector;
extern bool simu_pause;
extern CCommunicator *communicator;

extern bool simu_pause_for_no_guidance;
extern bool Is_Guidance_Info_Right;

extern int Start_Simu_Time;

extern CString Open_File_PathName;

extern char Car_Following_Model_Type;
extern char Car_Moving_Model_Type;
extern double CA_P;
extern double CA_P2;
extern int Max_Lane_Cell_Number;
extern int Meter_Per_Cell;
extern int Pixel_Per_Cell;

extern int Current_Demand;   //unit is veh/min
extern int Current_Slider_Demand;   //unit is veh/min
extern bool Slider_Demand_or_Not;

extern int Entering_Boundry;     
extern int Checking_Region;

extern bool Output_RouteChoice;
extern bool Output_MFD;
extern bool Output_DepartureArrivalRate;
extern bool Output_SpatialDistribution;
extern bool Output_Trajectory;
extern bool Output_DetectorData;

extern bool Shudown_After_Program_End;

extern int G_Simulated_Day_Number;
extern int G_Commuter_Number_Per_Origin;
extern int G_Day_Length;    //duration of a day
extern int G_Required_Arrival_Time;
extern int Total_Simulation_Time;
extern int All_Commuters_Type;
extern int Waiting_Time_In_Intersections;
extern int Waiting_Cycle_Number;

