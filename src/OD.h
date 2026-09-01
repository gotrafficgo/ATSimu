#pragma once

#include "setting.h"

class CVeh;

class COD
{
public:
	COD(char Type, int Located_Link_ID, int Located_Cell_ID, bool Able_To_In_Network, bool Able_To_Out_Network);
	~COD(void);
	
public:
	char Type;    //'R' and 'M'
	int OD_ID;    //1000+Link_ID : Type 'R'  2000+Link_ID: Type 'M'
	int Located_Link_ID;
	int Located_Cell_ID;
	bool Able_To_Out_Network;
	bool Able_To_In_Network;
	int Dest_Number;  //0 means no destination
	int Dest_Array[MAX_DEST_NUMBER][2];   //NOTE: [MAX_DEST_NUMBER][1] is 1000 times of original value, see definition of OD_Array[][]
	int Time_To_Generate_Veh;

public:
	void Produce_Veh(int Veh_Type, int Enforce_Flag);
	int Is_It_Time();
	bool Is_It_Time_To_Moving_Bottleneck();
	double Get_Current_Demand();
	int Get_Random_End_Link();
	int Get_Load_Lane_ID();
	bool Able_To_In_Network_Now();
	int Get_Headway(double lambda);
};
