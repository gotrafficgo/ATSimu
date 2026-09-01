// Conflict_Area.cpp: implementation of the CConflict_Area class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "traffic.h"
#include "Conflict_Area.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConflict_Area::CConflict_Area()
{
	int i;
	Cross_ID=0;
	Conflict_ID=0;
	Main_Cell=NULL;
	Conflict_Cell_Number=0;
	Veh_Number=0;
	for (i=0;i<MAX_CONFLICT_CELL_NUMBER; i++)
		Conflict_Cell_Array[i]=NULL; 
}

CConflict_Area::~CConflict_Area()
{

}
