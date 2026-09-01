#pragma once
#include "setting.h"
#include "Veh.h"
#include "Cell.h"


class CG2Detector
{
private:
	int Located_Link_ID;
	int Located_Lane_ID;
	int Located_Cell_ID;

	int Record_Frequency;            //How many secs does it record veh count.
	int Arrival_Count;
	double Mean_Speed;
	int Veh_Number;
	CString Detector_ID;
	char* Data_File_Name;

	CCell* Scan_Region[SCAN_REGION];
	CVeh* Veh_In_Scan_Last_Time[SCAN_REGION];      //save the vehs which moved into scan region.
	CVeh* Veh_In_Scan_This_Time[SCAN_REGION];
	CVeh* Passed_Veh[SCAN_REGION];

private:
	void Collect_Data();
	bool Record_Data();
	void Set_Veh_In_Scan_This_Time();
	void Set_Passed_Veh();
	void Clear_Memory();
	void Clear_Data();

public:
	CG2Detector(int Link_ID, int Lane_ID, int Cell_ID);
	~CG2Detector(void);
	void Run();

};
