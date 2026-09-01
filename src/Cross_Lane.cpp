// Cross_Lane.cpp: implementation of the CCross_Lane class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "traffic.h"
#include "math.h"
#include "setting.h"
#include "Cell.h"
#include "Global.h"
#include "ErrorLog.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCross_Lane::CCross_Lane()
{
    Cell_Number = 0;
	Cross_Lane_ID= -1;
	Cross_ID=-1;
	End_Lane_ID=-1;
	End_Link_ID=-1;
	Start_Lane_ID=-1;
	Start_Link_ID=-1;
	Turn_Direction= -1;  

    int i;
	for(i=0;i<MAX_CROSS_LANE_CELL_NUMBER;i++)
		Cross_Lane_Cell[i]=NULL;
}

CCross_Lane::~CCross_Lane()
{

}

Cpoint CCross_Lane::Circle_Intersect_Line(Cpoint Point_on_Line ,	double Line_Slope , bool K_Flag,
																	 Cpoint Circle_Center,	double Radius,Cpoint Point_for_Contrast,	int Flag)
{

	double a= Circle_Center.x;
	double b= Circle_Center.y;
	double k= Line_Slope;
	double c= Point_on_Line.y-k*Point_on_Line.x;
	double R= Radius;
	Cpoint point1;
	Cpoint point2;
	double u; 
	double v; 
	double Distance1;
	double Distance2;
	double Distance; 
	switch(K_Flag)
	{
		case true:
			u= 2*(c*k-a-b*k)/(1+k*k);
			v= (a*a+b*b+c*c-2*c*b-R*R)/(1+k*k);

			point1.x=(-u+sqrt(u*u-4*v))/2;
			point1.y=k*point1.x+c;

			point2.x=(-u-sqrt(u*u-4*v))/2;
			point2.y=k*point2.x+c;
			break;

		case false:
			point1.x= a;   
			point2.x= a;

			point1.y= b+R;
			point2.y= b-R;
			break;
	}

	Distance1= Point_Distance(point1, Point_for_Contrast);  
	Distance2= Point_Distance(point2, Point_for_Contrast);
	Distance= fabs(Distance2-Distance1);
	if(Distance<0.0000000001)   //Distance1==Distance2
	{
		if(point1.x!=point2.x) 
		{
			if(Flag==1)   
			{
				return point1;
			}
			else if(Flag==-1)
			{
				return point2;
			}
			else
			{
				AfxMessageBox("Error---CCross_Lane::Circle_Intersect_Line()~~~1");
				return point1; 
			}
		}
		else 
		{
			if(Flag==1)   
			{
				if(point1.y>point2.y)
					return point1;
				else
					return point2;
			}
			else if(Flag==-1)
			{
				if(point1.y>point2.y)
					return point2;
				else
					return point1;
			}
			else
			{
				AfxMessageBox("Error---CCross_Lane::Circle_Intersect_Line()~~~2");
				return point1; 
			}
		}
	
	}
	else if(Distance2>Distance1)
	{
		return point1;
	}
	else
	{
		return point2;
	}
}

int CCross_Lane::Get_Cell_Coordinate_in_Arc(Cpoint End_Point, Cpoint Start_Point, int Cross_Lane_ID)  
{
	Cpoint Circle_Center_of_Arc; 
	double Radius_of_Arc;
	int Point_Number_on_Chord;
	double sink;
	double cosk;
	int i;              
	double k;     
	double k1;   
	double k2;   
	double Chord_Length;
	double Radius;            
	Cpoint Middle_point; 
	bool K_Flag;                       
	Cpoint Point_Array[MAX_CROSS_LANE_CELL_NUMBER];
	Cpoint Cell_Coordinate_Array[MAX_CROSS_LANE_CELL_NUMBER];
	for (i=0; i<MAX_CROSS_LANE_CELL_NUMBER; i++)
	{
		Point_Array[i].x=-1;
		Point_Array[i].y=-1;
		Cell_Coordinate_Array[i].x=-1;
		Cell_Coordinate_Array[i].y=-1;
	}


	Chord_Length= Point_Distance(Start_Point, End_Point);
	Radius= Chord_Length*RADIUS_PERCENT_OF_DISTANCE;
	Middle_point=Get_Middle_Point(Start_Point, End_Point);
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

	switch(Quad_Number)
	{
		case 1:
			k= (End_Point.y- Start_Point.y)/(End_Point.x-Start_Point.x); 
			k1= -(1/k);    
			Circle_Center_of_Arc = Circle_Intersect_Line(Middle_point , k1 , true,  Middle_point, Radius, Middle_point, -1);
			Radius_of_Arc =Point_Distance(Start_Point, Circle_Center_of_Arc);
			Point_Number_on_Chord= (int)(Chord_Length/INTERVAL_IN_CHORD)+1;

			sink=fabs((Start_Point.y-End_Point.y)/Chord_Length); 
			cosk=fabs((Start_Point.x-End_Point.x)/Chord_Length);
			
			Point_Array[0].x=End_Point.x;
			Point_Array[0].y=End_Point.y;
			
			Cell_Coordinate_Array[0].x=End_Point.x;
			Cell_Coordinate_Array[0].y=End_Point.y;		
			
			for(i=1; i<Point_Number_on_Chord; i++)
			{
				Point_Array[i].x = Point_Array[i-1].x-INTERVAL_IN_CHORD*cosk;
				Point_Array[i].y = Point_Array[i-1].y-INTERVAL_IN_CHORD*sink;
				if(Point_Array[i].x-Circle_Center_of_Arc.x!=0)
				{
					k2=(Point_Array[i].y-Circle_Center_of_Arc.y)/(Point_Array[i].x-Circle_Center_of_Arc.x); 
					K_Flag=true;
				}
				else
				{
					k2=1;
					K_Flag=false;
				}
				Cell_Coordinate_Array[i]= Circle_Intersect_Line(Circle_Center_of_Arc , k2, K_Flag,  
															 Circle_Center_of_Arc, Radius_of_Arc,
															 Point_Array[i], 0); 
			}
			Cell_Coordinate_Array[Point_Number_on_Chord]=Start_Point;
			break;

		case 2:
			k= (End_Point.y- Start_Point.y)/(End_Point.x-Start_Point.x);
			k1= -(1/k);    
			Circle_Center_of_Arc = Circle_Intersect_Line(Middle_point , k1 , true,  Middle_point, Radius, Middle_point, -1);
			Radius_of_Arc =Point_Distance(Start_Point, Circle_Center_of_Arc);
			Point_Number_on_Chord= (int)(Chord_Length/INTERVAL_IN_CHORD)+1; 

			sink=fabs((Start_Point.y-End_Point.y)/Chord_Length);
			cosk=fabs((Start_Point.x-End_Point.x)/Chord_Length);
			
			Point_Array[0].x=End_Point.x;
			Point_Array[0].y=End_Point.y;
			
			Cell_Coordinate_Array[0].x=End_Point.x;
			Cell_Coordinate_Array[0].y=End_Point.y;		
			
			for(i=1; i<Point_Number_on_Chord; i++) 
			{
				Point_Array[i].x = Point_Array[i-1].x+INTERVAL_IN_CHORD*cosk;
				Point_Array[i].y = Point_Array[i-1].y-INTERVAL_IN_CHORD*sink;

				if(Point_Array[i].x-Circle_Center_of_Arc.x!=0)
				{
					k2=(Point_Array[i].y-Circle_Center_of_Arc.y)/(Point_Array[i].x-Circle_Center_of_Arc.x); 
					K_Flag=true;
				}
				else
				{
					k2=1; 
					K_Flag=false;
				}
				Cell_Coordinate_Array[i]= Circle_Intersect_Line(Circle_Center_of_Arc , k2, K_Flag,Circle_Center_of_Arc, Radius_of_Arc, Point_Array[i], 0); 
			}
			Cell_Coordinate_Array[Point_Number_on_Chord]=Start_Point;
			break;

		case 3:
			k= (End_Point.y- Start_Point.y)/(End_Point.x-Start_Point.x); 
			k1= -(1/k);    
			Circle_Center_of_Arc = Circle_Intersect_Line(Middle_point , k1 , true,  Middle_point, Radius,  Middle_point, 1);
			Radius_of_Arc =Point_Distance(Start_Point, Circle_Center_of_Arc);
			Point_Number_on_Chord= (int)(Chord_Length/INTERVAL_IN_CHORD)+1; 

			sink=fabs((Start_Point.y-End_Point.y)/Chord_Length);  
			cosk=fabs((Start_Point.x-End_Point.x)/Chord_Length);
			
			Point_Array[0].x=End_Point.x;
			Point_Array[0].y=End_Point.y;
			
			Cell_Coordinate_Array[0].x=End_Point.x;
			Cell_Coordinate_Array[0].y=End_Point.y;		
			
			for(i=1; i<Point_Number_on_Chord; i++) 
			{
				Point_Array[i].x = Point_Array[i-1].x+INTERVAL_IN_CHORD*cosk;
				Point_Array[i].y = Point_Array[i-1].y+INTERVAL_IN_CHORD*sink;

				if(Point_Array[i].x-Circle_Center_of_Arc.x!=0)
				{
					k2=(Point_Array[i].y-Circle_Center_of_Arc.y)/(Point_Array[i].x-Circle_Center_of_Arc.x);
					K_Flag=true;
				}
				else
				{
					k2=1; 
					K_Flag=false;
				}
				Cell_Coordinate_Array[i]= Circle_Intersect_Line(Circle_Center_of_Arc , k2, K_Flag,Circle_Center_of_Arc, Radius_of_Arc, Point_Array[i], 0); 
			}
			Cell_Coordinate_Array[Point_Number_on_Chord]=Start_Point;
			break;

		case 4:
			k= (End_Point.y- Start_Point.y)/(End_Point.x-Start_Point.x);
			k1= -(1/k);    
			Circle_Center_of_Arc = Circle_Intersect_Line(Middle_point , k1 , true,   Middle_point, Radius,  Middle_point, 1); 
			Radius_of_Arc =Point_Distance(Start_Point, Circle_Center_of_Arc);
			Point_Number_on_Chord= (int)(Chord_Length/INTERVAL_IN_CHORD)+1; 

			sink=fabs((Start_Point.y-End_Point.y)/Chord_Length); 
			cosk=fabs((Start_Point.x-End_Point.x)/Chord_Length);
			
			Point_Array[0].x=End_Point.x;
			Point_Array[0].y=End_Point.y;
			
			Cell_Coordinate_Array[0].x=End_Point.x;
			Cell_Coordinate_Array[0].y=End_Point.y;		
			
			for(i=1; i<Point_Number_on_Chord; i++) 
			{
				Point_Array[i].x = Point_Array[i-1].x-INTERVAL_IN_CHORD*cosk;
				Point_Array[i].y = Point_Array[i-1].y+INTERVAL_IN_CHORD*sink;

				if(Point_Array[i].x-Circle_Center_of_Arc.x!=0)
				{
					k2=(Point_Array[i].y-Circle_Center_of_Arc.y)/(Point_Array[i].x-Circle_Center_of_Arc.x); 
					K_Flag=true;
				}
				else
				{
					k2=1;
					K_Flag=false;
				}
				Cell_Coordinate_Array[i]= Circle_Intersect_Line(Circle_Center_of_Arc , k2, K_Flag,Circle_Center_of_Arc, Radius_of_Arc, Point_Array[i], 0);
			}
			Cell_Coordinate_Array[Point_Number_on_Chord]=Start_Point;
			break;

		case 5:
			if(Start_Point.x<End_Point.x)
			{
				Circle_Center_of_Arc.x= Middle_point.x;
				Circle_Center_of_Arc.y= Middle_point.y + Radius;
			}
			else
			{
				Circle_Center_of_Arc.x= Middle_point.x;
				Circle_Center_of_Arc.y= Middle_point.y - Radius;			
			}
			Radius_of_Arc =Point_Distance(Start_Point, Circle_Center_of_Arc);
			Point_Number_on_Chord= (int)(Chord_Length/INTERVAL_IN_CHORD)+1;

			Point_Array[0].x=End_Point.x;
			Point_Array[0].y=End_Point.y;
			
			Cell_Coordinate_Array[0].x=End_Point.x;
			Cell_Coordinate_Array[0].y=End_Point.y;		

			for(i=1; i<Point_Number_on_Chord; i++)   
			{
				if(Start_Point.x < End_Point.x)
				{
					Point_Array[i].x = Point_Array[i-1].x-INTERVAL_IN_CHORD;
				}
				else
				{
					Point_Array[i].x = Point_Array[i-1].x+INTERVAL_IN_CHORD;
				}
				Point_Array[i].y = Point_Array[i-1].y;

				if(Point_Array[i].x-Circle_Center_of_Arc.x!=0)
				{
					k2=(Point_Array[i].y-Circle_Center_of_Arc.y)/(Point_Array[i].x-Circle_Center_of_Arc.x); 
					K_Flag=true;
				}
				else 
				{
					k2=1; 
					K_Flag=false;
				}
				Cell_Coordinate_Array[i]= Circle_Intersect_Line(Circle_Center_of_Arc , k2, K_Flag,Circle_Center_of_Arc, Radius_of_Arc, Point_Array[i], 0);
			}
			Cell_Coordinate_Array[Point_Number_on_Chord]=Start_Point;
			break;

		case 6:
			if(Start_Point.y<End_Point.y)
			{
				Circle_Center_of_Arc.x= Middle_point.x - Radius;
				Circle_Center_of_Arc.y= Middle_point.y;
			}
			else 
			{
				Circle_Center_of_Arc.x= Middle_point.x + Radius;
				Circle_Center_of_Arc.y= Middle_point.y;			
			}
			Radius_of_Arc =Point_Distance(Start_Point, Circle_Center_of_Arc);
			Point_Number_on_Chord= (int)(Chord_Length/INTERVAL_IN_CHORD)+1;

			Point_Array[0].x=End_Point.x;
			Point_Array[0].y=End_Point.y;
			
			Cell_Coordinate_Array[0].x=End_Point.x;
			Cell_Coordinate_Array[0].y=End_Point.y;		

			for(i=1; i<Point_Number_on_Chord; i++) 
			{
				if(Start_Point.y < End_Point.y)
				{
					Point_Array[i].y = Point_Array[i-1].y-INTERVAL_IN_CHORD;
				}
				else
				{
					Point_Array[i].y = Point_Array[i-1].y+INTERVAL_IN_CHORD;
				}
				Point_Array[i].x = Point_Array[i-1].x;

				if(Point_Array[i].x-Circle_Center_of_Arc.x!=0)
				{
					k2=(Point_Array[i].y-Circle_Center_of_Arc.y)/(Point_Array[i].x-Circle_Center_of_Arc.x);
					K_Flag=true;
				}
				else 
				{
					k2=1;
					K_Flag=false;
				}
				Cell_Coordinate_Array[i]= Circle_Intersect_Line(Circle_Center_of_Arc , k2, K_Flag,Circle_Center_of_Arc, Radius_of_Arc,Point_Array[i], 0);
			}
			Cell_Coordinate_Array[Point_Number_on_Chord]=Start_Point;
			break;	
	}
	Point_Number_on_Chord=Point_Number_on_Chord+1;

	for(int ii=0;ii<int(Point_Number_on_Chord/2);ii++)
	{
		Cpoint a;
		a= Cell_Coordinate_Array[ii];
		Cell_Coordinate_Array[ii]= Cell_Coordinate_Array[Point_Number_on_Chord-ii-1];
		Cell_Coordinate_Array[Point_Number_on_Chord-ii-1]=a;
	}

	if (Point_Number_on_Chord>MAX_CROSS_LANE_CELL_NUMBER)
		AfxMessageBox("need enlarge MAX_CROSS_LANE_CELL_NUMBER~~~1");



	for (i=0;i<Point_Number_on_Chord;i++)
	{
		CCell *pCell=new CCell;
		pCell->Cell_Center->x=Cell_Coordinate_Array[i].x;
		pCell->Cell_Center->y=Cell_Coordinate_Array[i].y;
		pCell->Cell_ID=i;
		for (int ii=0; ii<MAX_CONFLICT_AREA_ID_NUMBER; ii++)
		{
			pCell->Conflict_Area_ID_Array[ii]=-1;
		}		
		pCell->PutVehInCell(NULL);
		Cross_Lane_Cell[i]=pCell;
		pCell->Cross_Lane_ID= Cross_Lane_ID;
		pCell=NULL;
	}

	return Point_Number_on_Chord; 

}



int CCross_Lane::Get_Straight_and_Right_Cell_Coordinate_in_Arc(Cpoint Start_Point, Cpoint End_Point, int Cross_Lane_ID)
{
	Cpoint Cell_Edge;  
	int i;
	Cpoint Cell_Coordinate_Array[MAX_CROSS_LANE_CELL_NUMBER];
	for (i=0; i<MAX_CROSS_LANE_CELL_NUMBER; i++)
	{
		Cell_Coordinate_Array[i].x=-1;
		Cell_Coordinate_Array[i].y=-1;
	}

	
	Cell_Edge.x=End_Point.x;
	Cell_Edge.y=End_Point.y;

	int Line_Length = (int)sqrt( (Start_Point.x-End_Point.x)*(Start_Point.x-End_Point.x) + (Start_Point.y-End_Point.y)*(Start_Point.y-End_Point.y) );
	int Cell_Number = (int)( Line_Length/CELL_LENGTH_IN_CROSS +1); 
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

	switch(Quad_Number)
	{
		
		case 1:
			
			for(i=0; i<Cell_Number; i++)
			{
				if (i==0)
				{
					Cell_Coordinate_Array[0].x = Cell_Edge.x;
					Cell_Coordinate_Array[0].y= Cell_Edge.y;		
				}
				else
				{
					Cell_Coordinate_Array[i].x = Cell_Edge.x-CELL_LENGTH_IN_CROSS*cosk/2;
					Cell_Coordinate_Array[i].y= Cell_Edge.y-CELL_LENGTH_IN_CROSS*sink/2;
					
					Cell_Edge.x = Cell_Edge.x-CELL_LENGTH_IN_CROSS*cosk;
					Cell_Edge.y = Cell_Edge.y-CELL_LENGTH_IN_CROSS*sink;
					}
			}
		    break;

		case 2: 
			for(i=0; i<Cell_Number; i++)   
			{	
				if (i==0)
				{
					Cell_Coordinate_Array[0].x = Cell_Edge.x;
					Cell_Coordinate_Array[0].y= Cell_Edge.y;		
				}
				else
				{
					Cell_Coordinate_Array[i].x= Cell_Edge.x+CELL_LENGTH_IN_CROSS*cosk/2;
					Cell_Coordinate_Array[i].y = Cell_Edge.y-CELL_LENGTH_IN_CROSS*sink/2;
					
					Cell_Edge.x = Cell_Edge.x+CELL_LENGTH_IN_CROSS*cosk;
					Cell_Edge.y = Cell_Edge.y-CELL_LENGTH_IN_CROSS*sink;					
				}
			}
		    break;

		case 3:
			for(i=0; i<Cell_Number; i++)  
			{
				if (i==0)
				{
					Cell_Coordinate_Array[0].x = Cell_Edge.x;
					Cell_Coordinate_Array[0].y= Cell_Edge.y;		
				}
				else
				{
					Cell_Coordinate_Array[i].x = Cell_Edge.x+CELL_LENGTH_IN_CROSS*cosk/2;
					Cell_Coordinate_Array[i].y = Cell_Edge.y+CELL_LENGTH_IN_CROSS*sink/2;
					
					Cell_Edge.x = Cell_Edge.x+CELL_LENGTH_IN_CROSS*cosk;
					Cell_Edge.y = Cell_Edge.y+CELL_LENGTH_IN_CROSS*sink;
				}
			}
		    break;

		case 4: 
			for(i=0; i<Cell_Number; i++)
			{
				if (i==0)
				{
					Cell_Coordinate_Array[0].x = Cell_Edge.x;
					Cell_Coordinate_Array[0].y= Cell_Edge.y;		
				}
				else
				{
					Cell_Coordinate_Array[i].x = Cell_Edge.x-CELL_LENGTH_IN_CROSS*cosk/2;
					Cell_Coordinate_Array[i].y = Cell_Edge.y+CELL_LENGTH_IN_CROSS*sink/2;
					
					Cell_Edge.x = Cell_Edge.x-CELL_LENGTH_IN_CROSS*cosk;
					Cell_Edge.y = Cell_Edge.y+CELL_LENGTH_IN_CROSS*sink;
				}
			}
		    break;

		case 5:  
			if(End_Point.x >Start_Point.x) 
			{
				
				for(i=0; i<Cell_Number; i++) 
				{
					if (i==0)
					{
						Cell_Coordinate_Array[0].x = Cell_Edge.x;
						Cell_Coordinate_Array[0].y= Cell_Edge.y;		
					}
					else
					{
						Cell_Coordinate_Array[i].x = Cell_Edge.x-CELL_LENGTH_IN_CROSS/2;
						Cell_Coordinate_Array[i].y = Cell_Edge.y;
						
						Cell_Edge.x = Cell_Edge.x-CELL_LENGTH_IN_CROSS;
					}
				}
			}
			else
			{
				for(i=0; i<Cell_Number; i++)  
				{
					if (i==0)
					{
						Cell_Coordinate_Array[0].x = Cell_Edge.x;
						Cell_Coordinate_Array[0].y= Cell_Edge.y;		
					}
					else
					{
						Cell_Coordinate_Array[i].x = Cell_Edge.x+CELL_LENGTH_IN_CROSS/2;
						Cell_Coordinate_Array[i].y = Cell_Edge.y;
						
						Cell_Edge.x = Cell_Edge.x+CELL_LENGTH_IN_CROSS;
					}
				}
			}
			break;

		case 6: 
			if(End_Point.y >Start_Point.y) 
			{
				
				for(i=0; i<Cell_Number; i++)  
				{
					if (i==0)
					{
						Cell_Coordinate_Array[0].x = Cell_Edge.x;
						Cell_Coordinate_Array[0].y= Cell_Edge.y;		
					}
					else
					{
						Cell_Coordinate_Array[i].x= Cell_Edge.x;
						Cell_Coordinate_Array[i].y = Cell_Edge.y-CELL_LENGTH_IN_CROSS/2;
						
						Cell_Edge.y = Cell_Edge.y-CELL_LENGTH_IN_CROSS;
					}
				}
			}
			else
			{
				for(i=0; i<Cell_Number; i++) 
				{
					if (i==0)
					{
						Cell_Coordinate_Array[0].x = Cell_Edge.x;
						Cell_Coordinate_Array[0].y= Cell_Edge.y;		
					}
					else
					{
						Cell_Coordinate_Array[i].x= Cell_Edge.x;
						Cell_Coordinate_Array[i].y = Cell_Edge.y+CELL_LENGTH_IN_CROSS/2;
						
						Cell_Edge.y = Cell_Edge.y+CELL_LENGTH_IN_CROSS;
					}
				}
			}
			break;
			
	}

	if (Cell_Number>MAX_CROSS_LANE_CELL_NUMBER)
	{
		AfxMessageBox("need enlarge MAX_CROSS_LANE_CELL_NUMBER~~~2");
	}


	for (i=0;i<Cell_Number;i++)
	{
		CCell *pCell=new CCell;
		pCell->Cell_Center->x=Cell_Coordinate_Array[i].x;
		pCell->Cell_Center->y=Cell_Coordinate_Array[i].y;
		pCell->Cell_ID=i;
		for (int ii=0; ii<MAX_CONFLICT_AREA_ID_NUMBER; ii++)
		{
			pCell->Conflict_Area_ID_Array[ii]=-1;
		}
		pCell->PutVehInCell(NULL);
		Cross_Lane_Cell[i]=pCell;
		pCell->Cross_Lane_ID= Cross_Lane_ID;
		pCell=NULL;
	}

	return Cell_Number;
}
