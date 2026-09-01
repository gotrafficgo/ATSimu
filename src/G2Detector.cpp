#include "stdafx.h"
#include "G2Detector.h"
#include "setting.h"
#include "Global.h"
#include "Veh.h"

CG2Detector::~CG2Detector(void)
{

}

CG2Detector::CG2Detector(int Link_ID, int Lane_ID, int Cell_ID)
{
	this->Located_Link_ID=Link_ID;
	this->Located_Lane_ID=Lane_ID;
	this->Located_Cell_ID=Cell_ID;
	
	/*
	---------------------------------------------
     Scan region <---  detector |
	---------------------------------------------
	*/
	for (int i=0; i<SCAN_REGION; i++)
		Scan_Region[i]=Lane_Array[Located_Link_ID][Located_Lane_ID]->Lane_Cell[Located_Cell_ID - i];
	
	for (int i=0; i<SCAN_REGION;i++)
	{
		Veh_In_Scan_Last_Time[i]=NULL;
		Veh_In_Scan_This_Time[i]=NULL;
		Passed_Veh[i]=NULL;
	}

	Record_Frequency=NCURVE_RECORD_FREQ;    
	Arrival_Count=0;
	Mean_Speed=0;
	Veh_Number=0;

	CString strLink;
	CString strLane;
	CString strCell;
	strLink.Format("%d", Located_Link_ID);
	strLane.Format("%d", Located_Lane_ID);
	strCell.Format("%d", Located_Cell_ID);
	Detector_ID=strLink+"."+strLane+"."+strCell;

	//convert from CString to char*
	CString str;
	str="G2Detector(" + Detector_ID + ").csv";
	int Length= str.GetLength()+1;
	Data_File_Name=new char[Length];
	strncpy_s(Data_File_Name, Length, str, Length);
}


void CG2Detector::Collect_Data()
{
	for (int i=0; i<SCAN_REGION; i++)
	{
		if (Passed_Veh[i]!=NULL)
		{
			double This_Speed= Passed_Veh[i]->Last_Spd;     //at this time, veh has run out of the detected region, here we collect the last speed when veh head out of the region
			Mean_Speed= Get_New_Mean(Mean_Speed,Arrival_Count, This_Speed);
			Arrival_Count++;
		}
		if (Scan_Region[i]->IsVehInCell()==true)
			Veh_Number++;
	}
}

bool CG2Detector::Record_Data()
{
	if (simu_time!= 0 && simu_time%Record_Frequency==0)
	{
		extern ErrorLog* err;
		err->LogIntData(Data_File_Name, simu_time);  
		err->LogStrData(Data_File_Name, ",");
		err->LogDoubleData(Data_File_Name, Mean_Speed);  
		err->LogStrData(Data_File_Name, ",");
		err->LogIntData(Data_File_Name, Arrival_Count); 
		err->LogStrData(Data_File_Name, ",");
		err->LogIntData(Data_File_Name, Veh_Number); 
		err->LogStrData(Data_File_Name,"\n");
		return true;
	}
	return false;
}

void CG2Detector::Run()
{
	Set_Veh_In_Scan_This_Time();
	Set_Passed_Veh();
	for (int i=0; i<SCAN_REGION;i++)
		Veh_In_Scan_Last_Time[i]=Veh_In_Scan_This_Time[i];
	Collect_Data();

	if(true==Record_Data())
		Clear_Data();

	Clear_Memory();

}

void CG2Detector::Set_Veh_In_Scan_This_Time()
{
	for (int i=0; i<SCAN_REGION;i++)
		if(true==Scan_Region[i]->IsVehInCell())
			Veh_In_Scan_This_Time[i]= Scan_Region[i]->GetVehFromCell();
}

void CG2Detector::Set_Passed_Veh()
{
	CVeh* pVeh_Last_Time=NULL;
	CVeh* pVeh_This_Time=NULL;
	int k=0;

	for (int i=0; i<SCAN_REGION; i++)
	{
		//check if the veh in last time array is stiil in the this time array
		pVeh_Last_Time=Veh_In_Scan_Last_Time[i];
		if (pVeh_Last_Time!=NULL)
		{
			int m=0;
			for (int j=0; j<SCAN_REGION; j++)
			{
				pVeh_This_Time=Veh_In_Scan_This_Time[j];
				if( pVeh_This_Time!=NULL)
					if (pVeh_Last_Time->Veh_ID==pVeh_This_Time->Veh_ID)
						break;
				m++;
			}

			if (m==SCAN_REGION)     //means "pVeh_Last_Time" is not in this time array after a iteration from the begin to the end, which means, the veh passed the detector
			{
				Passed_Veh[k]=pVeh_Last_Time;
				k++;
			}
		}
	}
}

void CG2Detector::Clear_Memory()
{
	for(int i=0; i<SCAN_REGION; i++)
	{
		Passed_Veh[i]=NULL;
		Veh_In_Scan_This_Time[i]=NULL;          //clear this time data
	}
}

void CG2Detector::Clear_Data()
{
	Mean_Speed=0;
	Arrival_Count=0;
	Veh_Number=0;
}















