// Line.cpp: implementation of the CLane class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "traffic.h"
#include "Lane.h"
#include "Cell.h"
#include "math.h"
#include "Global.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//Derived(int x,int y):Base1(y),Base2(x)
CLane::CLane()
{
	Left_Turn= false;
	Right_Turn= false;
	Straight_Turn= false;
	Phase=NULL;

	Lane_Cell= new CCell *[Max_Lane_Cell_Number];
}


CLane::~CLane()
{
	delete[] Lane_Cell;
}


void CLane::Set_Value(Cpoint p1, Cpoint p2) //p1: start point; p2: end point
{
    Cpoint Start_Point, End_Point;	
	double Lane_Length;
	Start_Point = p1;
	End_Point = p2;
	Lane_Length = Point_Distance(Start_Point, End_Point);
	Cell_Number = Set_Lane_Cell(Start_Point,End_Point);
	Prohibit_Change_Lane_Cell_ID = Cell_Number - (int)(PROHIBIT_CHANGE_LANE_DISTANCE / Pixel_Per_Cell);
}

//return cell_number
int CLane::Set_Lane_Cell(Cpoint End_Point, Cpoint Start_Point)
{
	double x,y;
	Cpoint Cell_Edge;  //middle point of cell edge line
	Cell_Edge.x=End_Point.x;
	Cell_Edge.y=End_Point.y;

	int Line_Length = (int)sqrt( (Start_Point.x-End_Point.x)*(Start_Point.x-End_Point.x) + (Start_Point.y-End_Point.y)*(Start_Point.y-End_Point.y) );
	int Cell_Number = (int)( Line_Length/Pixel_Per_Cell );
	double sink=fabs((Start_Point.y-End_Point.y)/Line_Length); 
	double cosk=fabs((Start_Point.x-End_Point.x)/Line_Length);

	int Quad_Number ;
	if(End_Point.x > Start_Point.x && End_Point.y > Start_Point.y)
	Quad_Number=1;  
	if(End_Point.x < Start_Point.x && End_Point.y > Start_Point.y)
	Quad_Number=2;   
	if(End_Point.x < Start_Point.x && End_Point.y < Start_Point.y)
	Quad_Number=3;    
	if(End_Point.x > Start_Point.x && End_Point.y < Start_Point.y)
	Quad_Number=4;  
	if(End_Point.y == Start_Point.y)
	Quad_Number=5;
	if(End_Point.x == Start_Point.x)
	Quad_Number=6;
	int i;

	switch(Quad_Number)
	{
		
		case 1: 
			for(i=0; i<Cell_Number; i++)
			{
				x= Cell_Edge.x-Pixel_Per_Cell*cosk/2;
				y= Cell_Edge.y-Pixel_Per_Cell*sink/2;

				Lane_Cell[i] = new CCell(x,y,i);

				Cell_Edge.x = Cell_Edge.x-Pixel_Per_Cell*cosk;
				Cell_Edge.y = Cell_Edge.y-Pixel_Per_Cell*sink;
			}
		    break;

		case 2:   
			for(i=0; i<Cell_Number; i++)  
			{	
				x= Cell_Edge.x+Pixel_Per_Cell*cosk/2;
				y = Cell_Edge.y-Pixel_Per_Cell*sink/2;
				
				Lane_Cell[i] = new CCell(x,y,i);

				Cell_Edge.x = Cell_Edge.x+Pixel_Per_Cell*cosk;
				Cell_Edge.y = Cell_Edge.y-Pixel_Per_Cell*sink;					
			}
		    break;

		case 3: 
			for(i=0; i<Cell_Number; i++)   
			{
				x = Cell_Edge.x+Pixel_Per_Cell*cosk/2;
				y = Cell_Edge.y+Pixel_Per_Cell*sink/2;
				
				Lane_Cell[i] = new CCell(x,y,i);

				Cell_Edge.x = Cell_Edge.x+Pixel_Per_Cell*cosk;
				Cell_Edge.y = Cell_Edge.y+Pixel_Per_Cell*sink;
			}
		    break;

		case 4: 
			for(i=0; i<Cell_Number; i++)  
			{
				x = Cell_Edge.x-Pixel_Per_Cell*cosk/2;
				y = Cell_Edge.y+Pixel_Per_Cell*sink/2;

				Lane_Cell[i] = new CCell(x,y,i);
				
				Cell_Edge.x = Cell_Edge.x-Pixel_Per_Cell*cosk;
				Cell_Edge.y = Cell_Edge.y+Pixel_Per_Cell*sink;
			}
		    break;

		case 5:
			if(End_Point.x >Start_Point.x)  
			{
				for(i=0; i<Cell_Number; i++) 
				{
					x = Cell_Edge.x-Pixel_Per_Cell/2;
					y = Cell_Edge.y;

					Lane_Cell[i] = new CCell(x,y,i);
					
					Cell_Edge.x = Cell_Edge.x-Pixel_Per_Cell;
				}
			}
			else
			{
				for(i=0; i<Cell_Number; i++)
				{
					x = Cell_Edge.x+Pixel_Per_Cell/2;
					y = Cell_Edge.y;

					Lane_Cell[i] = new CCell(x,y,i);
					
					Cell_Edge.x = Cell_Edge.x+Pixel_Per_Cell;
				}

			}
			break;

		case 6:  
			if(End_Point.y >Start_Point.y)  
			{
				for(i=0; i<Cell_Number; i++)   
				{
					x= Cell_Edge.x;
					y = Cell_Edge.y-Pixel_Per_Cell/2;

					Lane_Cell[i] = new CCell(x,y,i);
					
					Cell_Edge.y = Cell_Edge.y-Pixel_Per_Cell;
				}
			}
			else
			{
				for(i=0; i<Cell_Number; i++) 
				{
					x= Cell_Edge.x;
					y = Cell_Edge.y+Pixel_Per_Cell/2;

					Lane_Cell[i] = new CCell(x,y,i);
					
					Cell_Edge.y = Cell_Edge.y+Pixel_Per_Cell;
				}
			}
			break;
			
	}
	
	return Cell_Number;
}
