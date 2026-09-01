#pragma once

#include "setting.h"
#include "Communicator.h"
class CVeh;


class CDetector  
{
public:
	int Link_ID; 
	int Detector_ID;
	int Detector_Location;     //Cell_ID
	int Cross_ID;                    //the controller connected this detector located in this crossing
	Struct_Detector detector_info;
	char Detector_Type;              //'Q': queue detector; 'C': crossing detector
	
	int Average_Delay_Time_0;   //left turn
	int Average_Delay_Time_1;   //through traffic
	int Average_Delay_Time_2;   //right turn
	
	int Veh_Sum_0;   //cumulative vehicle count
	int Veh_Sum_1;
	int Veh_Sum_2;

public:
	void Detect(double This_Speed,  int Turn_Direction);
	CDetector(int link_id, int detector_id,  int detector_location);
	virtual ~CDetector();
	double Calculate_Mean(double Mean, int Amount,double New_Value);
	void Detector_Run(CVeh *pVeh, int Turn_Direction);
	void Reset_Detector_Info();
	char Get_Detector_Type(int detector_location);
	int Get_Detector_Location_On_Link(int link_i, int lane_i);

};
