#pragma once

#include "Cell.h"

class CConflict_Area  
{
public:
	CConflict_Area();
	virtual ~CConflict_Area();

	int Cross_ID;
	int Conflict_ID;
	int Conflict_Cell_Number;                                  //the number of cells which conflict with this cell
	int Veh_Number;
	CCell *Main_Cell;                                          //use this to distinguish different conflict
	CCell *Conflict_Cell_Array[MAX_CONFLICT_CELL_NUMBER];      //the cell conflict with Main_Cell, don't include Main_Cell itself
};

