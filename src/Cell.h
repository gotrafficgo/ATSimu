#pragma once
#include "setting.h"

class CVeh;
class CG2Detector;
class Cpoint;

class CCell 
{	
private:
	CVeh * Veh_p;          

public:
	CCell();
	CCell(double x, double y, int ID);
	virtual ~CCell();
	void PutVehInCell(CVeh *pVeh);
	CVeh* GetVehFromCell();
	bool IsVehInCell();
	void Init_Cell();

public: 
	Cpoint* Cell_Center;
	int Cell_ID;
	int Conflict_Area_ID_Array[MAX_CONFLICT_AREA_ID_NUMBER];       //in a conflcit, -1 means not in a conflict
	int Cross_Lane_ID;                            //the cell is in this cross lane.
	int Conflict_Area_ID_Amount;       //how many conflict area this cell is in.
	CCell *Straight_Conflict_Cell;
 	CG2Detector *pG2Detector;

};
