#pragma once
#include "structure.h"

class CSampleCollection
{
public:
	CArray <Avg_K_Q*, Avg_K_Q*> AKQ;
	CArray <Struct_Departure_Arrival_Rate* , Struct_Departure_Arrival_Rate*> SDAR;   //Struct Departure Arrival Rate
	CArray <Struct_Spatial_Distribution* , Struct_Spatial_Distribution*> SSD;   //Struct Spatial Distribution

	int Departure_Commuter_Number;
	int Departure_Traveler_Number;
	int Arrival_Commuter_Number;
	int Arrival_Traveler_Number;

public:
	CSampleCollection(void);
	~CSampleCollection(void);
	
// 	void Collect_Veh_Data_On_Link();
	void Reset_Veh_Data_On_Link();
	void Update_Avg_K_Q();

	void Record_Commuter_Choices(int Day, int Link_ID, int Origin_ID, int Commuter_ID);
	void Record_MFD();
	void Empty_MFD_Record();

	void Update_Departure_Arrival_Rate();
	void One_Depart(char Type);   //'C'--commuter; 'T'--traveler
	void One_Arrival(char Type);   //'C'--commuter; 'T'--traveler
	void Reset_Departure_Arrival_Counter();
	void Record_Departure_Arrival_Rate();
	void Empty_SDAR_Record();

	void Update_Spatial_Distribution(int Day, int Day_Time);
	void Record_Spatial_Distribution();
	void Empty_SSD_Record();

};
