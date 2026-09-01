// Detector.cpp: implementation of the CDetector class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "traffic.h"
#include "Detector.h"
#include "Veh.h"
#include "ErrorLog.h"
#include "Global.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDetector::CDetector(int link_id ,int detector_id, int detector_location)
{
	Link_ID= link_id;
	Detector_ID= detector_id;
	Detector_Location= detector_location;
	Detector_Type= Get_Detector_Type(detector_location);
	
	Veh_Sum_0=0;
	Veh_Sum_1=0;
	Veh_Sum_2=0;

	if (Detector_Type=='C')
	{
		Average_Delay_Time_0=0;
		Average_Delay_Time_1=0;
		Average_Delay_Time_2=0;
	}
	else{
		Average_Delay_Time_0=-1;
		Average_Delay_Time_1=-1;
		Average_Delay_Time_2=-1;

	}
	

	detector_info.Link_ID= Link_ID;
	detector_info.Detector_ID= Detector_ID;
	detector_info.Left_Veh_Number= 0;
	detector_info.Straight_Veh_Number= 0;
	detector_info.Right_Veh_Number= 0;
	detector_info.Left_Speed_Mean= 0;
	detector_info.Straight_Speed_Mean= 0;
	detector_info.Right_Speed_Mean=0;
	
}

CDetector::~CDetector()
{

}


char CDetector::Get_Detector_Type(int detector_location)
{
	if (detector_location==CROSS_DETECTOR_LOCATION)
		return 'C';
	else if(detector_location==QUEUE_DETECTOR_LOCATION)
		return 'Q';
	else
		return 'N';
}

double CDetector::Calculate_Mean(double Mean, int Amount,double New_Value)
{
	return (Mean*Amount + New_Value)/(Amount+1);
}

int CDetector::Get_Detector_Location_On_Link(int link_i, int lane_i)
{
	int Detector_Location_On_Link;

	if (Detector_Location==DETECTOR_IN_BEGINNING) 
		Detector_Location_On_Link= Link_Array[link_i]->Limited_Speed; 
	else
		Detector_Location_On_Link= Lane_Array[link_i][lane_i]->Cell_Number -1-  Detector_Location; 
	
	return Detector_Location_On_Link;

}


void CDetector::Detector_Run(CVeh *pVeh, int Turn_Direction)  //int This_Speed, int Turn_Direction
{
	if(pVeh!=NULL)
	{
		if (pVeh->Detected_Flag!=Detector_ID)  
		{
			pVeh->Detected_Flag=Detector_ID;
			Detect(pVeh->Cur_Spd , Turn_Direction);
			if (Detector_Type=='C')
			{
				int time;
				time= simu_time - pVeh->Simutime_Passing_Detector_Middle;
				switch (Turn_Direction)
				{
				case 0:
					Veh_Sum_0++;  
					Average_Delay_Time_0= Get_New_Mean(Average_Delay_Time_0, Veh_Sum_0, time);
					break;
				case 1:
					Veh_Sum_1++; 
					Average_Delay_Time_1= Get_New_Mean(Average_Delay_Time_1, Veh_Sum_1, time);
					break;
				case 2:
					Veh_Sum_2++; 
					Average_Delay_Time_2= Get_New_Mean(Average_Delay_Time_2, Veh_Sum_2, time);
					break;
				}
			}
			if (Detector_Type=='Q')
			{
				pVeh->Simutime_Passing_Detector_Middle=simu_time;
			}
		}
	}
}

void CDetector::Detect(double This_Speed, int Turn_Direction)
{
	switch (Turn_Direction)
	{
	case 0:
		detector_info.Left_Speed_Mean= 
			Calculate_Mean(detector_info.Left_Speed_Mean, detector_info.Left_Veh_Number, This_Speed);
		detector_info.Left_Veh_Number++ ;  
		break;

	case 1:
		detector_info.Straight_Speed_Mean= 
			Calculate_Mean(detector_info.Straight_Speed_Mean, detector_info.Left_Veh_Number, This_Speed);
		detector_info.Straight_Veh_Number++ ;  
		break;

	case 2:
		detector_info.Right_Speed_Mean= 
			Calculate_Mean(detector_info.Right_Speed_Mean, detector_info.Right_Veh_Number, This_Speed);
		detector_info.Right_Veh_Number++ ;  
		break;
	}
}


void CDetector::Reset_Detector_Info()
{
	detector_info.Link_ID= Link_ID;
	detector_info.Detector_ID= Detector_ID;
	
	detector_info.Left_Veh_Number= 0;
	detector_info.Straight_Veh_Number= 0;
	detector_info.Right_Veh_Number= 0;
	detector_info.Left_Speed_Mean= 0;
	detector_info.Straight_Speed_Mean= 0;
	detector_info.Right_Speed_Mean=0;
}
