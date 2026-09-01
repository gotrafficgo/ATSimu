#include "stdafx.h"
#include "traffic.h"
#include "Cell.h"
#include "Veh.h"
#include "point.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCell::CCell()
{
	Init_Cell();
}

CCell::CCell(double x, double y, int ID)
{
	Init_Cell();
	this->Cell_Center->x=x;
	this->Cell_Center->y=y;
	this->Cell_ID=ID;
}

void CCell::Init_Cell()
{
	Veh_p=NULL;
	Straight_Conflict_Cell=NULL;
	Cross_Lane_ID=-1;
	Conflict_Area_ID_Amount=0;
	pG2Detector=NULL;

	for (int i=0; i<MAX_CONFLICT_AREA_ID_NUMBER; i++)
		Conflict_Area_ID_Array[i]=-1;

	Cell_Center= new Cpoint;
}

CCell::~CCell()
{
	
}


void CCell::PutVehInCell(CVeh *pVeh)
{
	this->Veh_p= pVeh;
}
CVeh* CCell::GetVehFromCell()
{
	if(this->Veh_p!=NULL)
		return this->Veh_p;
	else
		return NULL;
}
bool CCell::IsVehInCell()
{
	if (Veh_p==NULL)
		return false;
	else
		return true;
}