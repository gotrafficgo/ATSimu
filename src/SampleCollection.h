#pragma once
#include "structure.h"

class CSampleCollection
{
public:
	CArray <Avg_K_Q*, Avg_K_Q*> AKQ;

public:
	CSampleCollection(void);
	~CSampleCollection(void);
	
// 	void Collect_Veh_Data_On_Link();
	void Reset_Veh_Data_On_Link();
	void Update_Avg_K_Q();
};
