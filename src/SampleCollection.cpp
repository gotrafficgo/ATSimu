#include "stdafx.h"
#include "SampleCollection.h"
#include "Global.h"
#include "setting.h"

CSampleCollection::CSampleCollection(void)
{
}

CSampleCollection::~CSampleCollection(void)
{
	for (int i=0; i<AKQ.GetSize()-1; i++)
	{
		delete AKQ.GetAt(i);
	}
	AKQ.RemoveAll();
}

// void CSampleCollection::Collect_Veh_Data_On_Link()
// {
// 	double Speed;
// 
// 		for (int i=0; i<G_Link_Number; i++)
// 		for (int j=0; j<Link_Array[i]->Lane_Number; j++)
// 			for (int k=0; k<Lane_Array[i][j]->Cell_Number; k++)
// 				if (true==Lane_Array[i][j]->Lane_Cell[k]->IsVehInCell())
// 				{
// 					Link_Array[i]->On_Link_Veh_Number++;
// 					Speed= Lane_Array[i][j]->Lane_Cell[k]->GetVehFromCell()->Cur_Spd;
// 					Link_Array[i]->Sum_Of_Speed = Link_Array[i]->Sum_Of_Speed + Speed;
// 				}
// }

void CSampleCollection::Reset_Veh_Data_On_Link()
{
	for (int i=0; i<G_Link_Number; i++)
		for (int j=0; j<Link_Array[i]->Lane_Number; j++)
			for (int k=0; k<Lane_Array[i][j]->Cell_Number; k++)
				if (true==Lane_Array[i][j]->Lane_Cell[k]->IsVehInCell())
				{
					Link_Array[i]->On_Link_Veh_Number=0;
					Link_Array[i]->Sum_Of_Speed=0;
				}
}


void CSampleCollection::Update_Avg_K_Q()
{
	double K_On_A_Link=0;
	double Q_On_A_Link=0;
	double Sum_Of_K=0;
	double Sum_Of_Q=0;
	double Total_Length_A_Link=0;
	double Sum_Of_Speed=0;
	int Veh_Number=0;
	int Lane_Number=0;
	int Cell_Number_On_Line=0;

	for (int i=0; i<G_Link_Number; i++)
	{
		Lane_Number= Link_Array[i]->Lane_Number;
		Cell_Number_On_Line= Lane_Array[i][0]->Cell_Number;
		Veh_Number= Link_Array[i]->On_Link_Veh_Number;

		//////////////////////////////////////////////////////
		//Sum_Of_K
		Total_Length_A_Link=double(Lane_Number*Cell_Number_On_Line*Meter_Per_Cell)/1000;     //Unit Conversion: m => km
		K_On_A_Link= Veh_Number/Total_Length_A_Link;
		K_On_A_Link= K_On_A_Link/Link_Array[i]->Lane_Number;
		Sum_Of_K +=K_On_A_Link;

		/////////////////////////////////////////////////////
		//Sum_Of_Q
		Sum_Of_Speed=Link_Array[i]->Sum_Of_Speed*3600/1000;      //Unit Conversion: m/s => km/hr
		Q_On_A_Link=Sum_Of_Speed/Total_Length_A_Link;
		Q_On_A_Link= Q_On_A_Link/Link_Array[i]->Lane_Number;
		Sum_Of_Q+=Q_On_A_Link;
	}

	Avg_K_Q * KnQ= new Avg_K_Q;
	KnQ->k= Sum_Of_K/G_Link_Number;
	KnQ->q=Sum_Of_Q/G_Link_Number;
	AKQ.Add(KnQ);
}
