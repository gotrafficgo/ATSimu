#pragma once

#include "setting.h"
#include "Phase.h"
#include "Communicator.h"
#include "Detector.h"

class CController
{
private:
	CPhase *Phase_Array[MAX_PHASE_NUMBER];

public:
	int Cross_ID;
	int Phase_Number;
	Struct_Control_Received Received_Control_Info;  
	Struct_Control_Received Received_Control_Info_0;  //buffer 
	Struct_Control Current_Control_Info;
	Struct_Control Init_Control_Info;
	int Last_Refresh_Time;
	CDetector *Detector_Under_Control[MAX_ENTER_LINK][MAX_DETECTOR_NUMBER]; //one intersection is connected with at most 4 links, and each link has 3 detectors at most
	int Cycle_Number;
	Struct_Update_Data Update_Data;
	TranStruct_MBC_Detector tran_mbc_detector;
	int Start_Time;    //start time of current control type
	int Last_Update_Time; 
	
	CController(int cross_id, int phase_amount);
	virtual ~CController();
	int Add_Phase(CPhase *Phase); 
	int Set_Phases_Cycle_Time(int Phase_ID, int New_Cycle_Time);  
	int Set_Phases_Green_Start_Time(int Phase_ID, int New_Green_Start_Time_Percent);
	int Set_Phases_Green_Percent(int Phase_ID, int New_Green_Percent);
	int Link_in_Phase_or_Not(int Phase_ID, int Link_ID);
	CPhase * Get_Phase(int Phase_ID);	
	void Update_Control_Info();
	void Generate_Point_Control_Strategy();
	bool Refresh_or_Not(); 
	void Update_Phase();
	void Controller_Run();
	void Calculate_Upload_Data();
	void Save_Update_Data();
	void Calculate_Controller_Update_Data();
	void Calculate_Lane_Update_Data();
	void Send_Data(SOCKET sendSocket, SOCKADDR_IN addrTo);
	void Clear_Tran_MBC();
	bool Update_or_Not();  
	void Reset_Detector();
	int New_Phase_Start_or_Not();
	int Get_Veh_Sum(int Link_ID, int Lane_ID, int Smaller_Cell_ID, int Bigger_Cell_ID);
	int Get_Delay_Time(int Link_ID, int Lane_ID);

};

