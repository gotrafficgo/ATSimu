#include "stdafx.h"
#include "SampleCollection.h"
#include "Global.h"
#include "setting.h"
#include "Commuter.h"
#include "Origin.h"

CSampleCollection::CSampleCollection(void)
{
	Reset_Departure_Arrival_Counter();
}

CSampleCollection::~CSampleCollection(void)
{

	for (int i=0; i<AKQ.GetSize()-1; i++)
	{
		delete AKQ.GetAt(i);
	}
	AKQ.RemoveAll();

	for (int i=0; i<SDAR.GetSize()-1; i++)
	{
		delete SDAR.GetAt(i);
	}
	SDAR.RemoveAll();

	for (int i=0; i<SSD.GetSize()-1; i++)
	{
		delete SSD.GetAt(i);
	}
	SSD.RemoveAll();

}

// void CSampleCollection::Collect_Veh_Data_On_Link()
// {
// 	double Speed;
// 
// 		for (int i=0; i<G_Link_Number; i++)
// 		for (int j=0; j<Link_Array[i]->Lane_Number; j++)
// 			for (int k=0; k<Link_Array[i]->Lanes[j]->Cell_Number; k++)
// 				if (true==Link_Array[i]->Lanes[j]->Lane_Cell[k]->IsVehInCell())
// 				{
// 					Link_Array[i]->On_Link_Veh_Number++;
// 					Speed= Link_Array[i]->Lanes[j]->Lane_Cell[k]->GetVehFromCell()->Cur_Spd;
// 					Link_Array[i]->Sum_Of_Speed = Link_Array[i]->Sum_Of_Speed + Speed;
// 				}
// }

void CSampleCollection::Reset_Veh_Data_On_Link()
{
	for (int i=0; i<G_Link_Number; i++)
		for (int j=0; j<Link_Array[i]->Lane_Number; j++)
			for (int k=0; k<Link_Array[i]->Lanes[j]->Cell_Number; k++)
				if (true==Link_Array[i]->Lanes[j]->Lane_Cell[k]->IsVehInCell())
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
		Cell_Number_On_Line= Link_Array[i]->Length_In_Cell;
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



void CSampleCollection::Record_Commuter_Choices(int Day, int Link_ID, int Origin_ID, int Commuter_ID)
{
	if (Link_Array[Link_ID]!=NULL)
	{
		CCommuter *pCommuter = Link_Array[Link_ID]->Origin_Array[Origin_ID]->Commuter_Array[Commuter_ID]; 
		if (pCommuter!=NULL)
		{
			CString strID;
			CString strRouteChoice;
			CString strDepartureTime;
			char * FileName1;
			char * FileName2;
			
			strID.Format("%d", pCommuter->Commuter_ID);

			strRouteChoice ="RouteChoice (Commuter " + strID + ").csv";
			FileName1 = CStringToCharStar(strRouteChoice);

			strDepartureTime = "DepartureTime (Commuter " + strID + ").csv";
			FileName2= CStringToCharStar(strDepartureTime);

			//output two row, fftt and first day experience
			if (Day==1)  
			{
				for (int link_i=0; link_i<G_Link_Number; link_i++)
				{
					err->LogIntData(FileName1, Link_Array[link_i]->Length_In_FFTT);
					if (link_i!=G_Link_Number-1)
						err->LogStrData(FileName1, ",");
				}
					err->LogStrData(FileName1, "\n");
			}

			//output routes
			for (int link_i=0; link_i<G_Link_Number; link_i++)
			{
				err->LogIntData(FileName1, pCommuter->Travel_Time_In_Memory[Day][link_i]);
				if (link_i!=G_Link_Number-1)
					err->LogStrData(FileName1, ",");
			}
			err->LogStrData(FileName1, "\n");

			//output departure time
			int The_Departure_Time=pCommuter->Departure_Time_Array[Day-1];
			err->LogIntData(FileName2, The_Departure_Time);
			if (Day!=G_Simulated_Day_Number)
				err->LogStrData(FileName2, ",");

		}
	}
}


void CSampleCollection::Record_MFD()
{
	double k, q;
	CString str;
	for (int i=0; i<this->AKQ.GetSize(); i++)
	{
		k=this->AKQ.GetAt(i)->k;
		str.Format("%f", k);
		err->LogStrData("MFD.csv", str);

		err->LogStrData("MFD.csv", ",");

		q=this->AKQ.GetAt(i)->q;
		str.Format("%f", q);
		err->LogStrData("MFD.csv", str);

		err->LogStrData("MFD.csv", '\n');
	}
}


void CSampleCollection::Empty_MFD_Record()
{
	this->AKQ.RemoveAll();
}

void CSampleCollection::Empty_SDAR_Record()
{
	this->SDAR.RemoveAll();
}

void CSampleCollection::Empty_SSD_Record()
{
	this->SSD.RemoveAll();
}

void CSampleCollection::Update_Departure_Arrival_Rate()
{
	Struct_Departure_Arrival_Rate *pSDAR= new Struct_Departure_Arrival_Rate;

	pSDAR->Departure_Commuter= Departure_Commuter_Number;
	pSDAR->Departure_Traveler= Departure_Traveler_Number;
	pSDAR->Arrival_Commuter= Arrival_Commuter_Number;
	pSDAR->Arrival_Traveler= Arrival_Traveler_Number;

	this->SDAR.Add(pSDAR);
}


void CSampleCollection::One_Depart(char Type)   //'C'--commuter; 'T'--traveler
{
	if (Type=='C')
		Departure_Commuter_Number++;
	else
		Departure_Traveler_Number++;
}


void CSampleCollection::One_Arrival(char Type)   //'C'--commuter; 'T'--traveler
{
	if (Type=='C')
		Arrival_Commuter_Number++;
	else
		Arrival_Traveler_Number++;

}


void CSampleCollection::Reset_Departure_Arrival_Counter()
{
	Departure_Commuter_Number=0;
	Departure_Traveler_Number=0;
	Arrival_Commuter_Number=0;
	Arrival_Traveler_Number=0;
}


void CSampleCollection::Record_Departure_Arrival_Rate()
{
	int AC, AT, DC, DT;
	
	for (int i=0; i<this->SDAR.GetSize(); i++)
	{
		AC=this->SDAR.GetAt(i)->Arrival_Commuter;
		err->LogIntData("DepartureArrialRate.csv", AC);
		err->LogStrData("DepartureArrialRate.csv", ",");
		
		AT=this->SDAR.GetAt(i)->Arrival_Traveler;
		err->LogIntData("DepartureArrialRate.csv", AT);
		err->LogStrData("DepartureArrialRate.csv", ",");

		DC=this->SDAR.GetAt(i)->Departure_Commuter;
		err->LogIntData("DepartureArrialRate.csv", DC);
		err->LogStrData("DepartureArrialRate.csv", ",");

		DT=this->SDAR.GetAt(i)->Departure_Traveler;
		err->LogIntData("DepartureArrialRate.csv", DT);

		err->LogStrData("DepartureArrialRate.csv", "\n");

	}
}



void CSampleCollection::Update_Spatial_Distribution(int Day, int Day_Time)
{

	Struct_Spatial_Distribution *pSSD= new Struct_Spatial_Distribution;
	
	pSSD->Day= Day;
	pSSD->Day_Time=Day_Time;
	for (int i=0; i<G_Link_Number;i++)
		pSSD->Link_Veh_Number_Array[i]= Link_Array[i]->On_Link_Veh_Number;

	this->SSD.Add(pSSD);

}


void CSampleCollection::Record_Spatial_Distribution()
{
	int Day, Day_Time, Veh_Number_On_Link;
	int size=SSD.GetSize();

	for (int ssd_i=0; ssd_i<size; ssd_i++)
	{
		Day=this->SSD.GetAt(ssd_i)->Day;
		err->LogIntData("SpatialDistribution.csv", Day);
		err->LogStrData("SpatialDistribution.csv", ",");

		Day_Time= this->SSD.GetAt(ssd_i)->Day_Time;
		err->LogIntData("SpatialDistribution.csv", Day_Time);
		err->LogStrData("SpatialDistribution.csv", ",");

		for (int link_i=0; link_i<G_Link_Number;link_i++)
		{
			Veh_Number_On_Link= SSD.GetAt(ssd_i)->Link_Veh_Number_Array[link_i];
			err->LogIntData("SpatialDistribution.csv", Veh_Number_On_Link);
			if ( link_i != G_Link_Number-1)
				err->LogStrData("SpatialDistribution.csv", ",");
			else
				err->LogStrData("SpatialDistribution.csv", "\n");
		}
	}
}


