#include "StdAfx.h"
#include "Global.h"
#include "ErrorLog.h"
#include "math.h"
#include "MainFrm.h"
#include "setting.h"


CMarginalPoint *MarginalPoint_Array[MAX_MARGINALPOINT_NUMBER];	
CCross *Cross_Array[MAX_CROSS_NUMBER];
CLink *Link_Array[MAX_LINK_NUMBER];	
CMyObject *Node_Relation[MAX_MARGINALPOINT_NUMBER+MAX_CROSS_NUMBER][MAX_MARGINALPOINT_NUMBER+MAX_CROSS_NUMBER];
CLane *Lane_Array[MAX_LINK_NUMBER][MAX_LANE_NUMBER];	
CCross_Lane  *Cross_Lane_Array[MAX_CROSS_NUMBER][MAX_CROSS_LANE_NUMBER];
CConflict_Area *Conflict_Area_Array[MAX_CROSS_NUMBER][MAX_CONFLICT_AREA_NUMBER]; 
Struct_Shortest_Path *Shortest_Path_Array[MAX_LINK_NUMBER][MAX_LINK_NUMBER];
Struct_FixedOD FixedOD_Array[MAX_FixedOD_NUMBER]; 
double Demand_Array[MAX_DEMAND_NUMBER][2];      //original demand which includes the lower and upper value; value in demand_array is veh #/min generated.

int simu_time;   
int Received_Guidance_Number=0;
double zoom_ratio=1-3/7;
CSize m_sizeEllipse;  
CPoint   m_pointTopLeft;  
int Sleep_Time=SLEEP_TIME;
int Total_Veh_Number=0;
int Out_Veh_Number=0;
int G_Link_Number=0;
int G_Cross_Number=0;
int G_MarginalPoint_Number=0;
int G_Lane_Number=0;
int G_FixedOD_Number=0;
int G_Demand_Number=0;
bool Yellow_Intersection=true;
char Car_Following_Model_Type='G';   //'G'-Gipps; 'N'-Newell; 'I'-IDM
int Start_Time_Guidance=0;
bool Switch_Guidance=false;
bool Display_Flag=true;
bool Switch_Thread= true;	
int Start_Simu_Time=0;
char Current_Control_Type='N';
int Total_Simulation_Time=3600;

int Current_Demand=0;
int Current_Slider_Demand=15;  //mean of Binomial distribution is n*p, so 0.2 means the mean is generating 2 vehs in 10 sec
bool Slider_Demand_or_Not=false;

CSimuFun *simuFun;
CSampleCollection *pSampleCollection= new CSampleCollection;
ErrorLog *err=new ErrorLog;
int Start_Time_Detector;
bool Switch_Detector=false;
bool simu_pause;
CCommunicator *communicator= new CCommunicator();

bool simu_pause_for_no_guidance= false;
bool Is_Guidance_Info_Right;


char Car_Moving_Model_Type='F';   //'F'--car-following model, 'A'--cellular autamata
CString Open_File_PathName="N";
double CA_P= DEFAULT_P;
double CA_P2= DEFAULT_P2;
int Max_Lane_Cell_Number;
int Meter_Per_Cell;
int Pixel_Per_Cell;


//the unit is veh
//Yellow_Intersection related variable. 
//Before entering intersections, vehcles estimate the veh number in the Checking_Region of the target lane of next link. 
//if veh number in this region is smaller than Entering_Boundry (veh), it enter intersections, otherwise wait in front of the intersection.
//the value of Entering_Boundry is from ENTERING_BOUNDRY_A and ENTERING_BOUNDRY_F respectively corresponding to CA model and CF model
int Entering_Boundry;    //see above comments
int Checking_Region;    //if in the "CHECKING_REGION", which begin from the beginning of link


double Point_Distance(Cpoint a1, Cpoint a2)
{
    double distance= sqrt(fabs((a1.y - a2.y)*(a1.y - a2.y) + (a1.x - a2.x)*(a1.x - a2.x)));
	return distance;
}

double Point_Distance(double x1, double y1, double x2, double y2)
{
	double distance= sqrt(fabs((y1 - y2)*(y1 - y2) + (x1 - x2)*(x1 - x2)));
	return distance;
}

//Calculate the intersection point between two lines; the arguments are the start and end points of two lines
Cpoint Get_Intersection_Point(Cpoint Line_1_Start, Cpoint Line_1_End, Cpoint Line_2_Start, Cpoint Line_2_End)
{
	Cpoint m_point(0,0);
	double m_a1,m_a2,m_b1,m_b2,m_c1,m_c2,m_d1,m_d2;
	m_a1 = Line_1_Start.x;
	m_b1 = Line_1_Start.y;
	m_a2 = Line_1_End.x;
	m_b2 = Line_1_End.y; 
	m_c1 = Line_2_Start.x;
	m_d1 = Line_2_Start.y;
	m_c2 = Line_2_End.x;
	m_d2 = Line_2_End.y;

	if( ((m_a1==m_a2) && (m_b1==m_b2)) || ((m_c1==m_c2)&&(m_d1==m_d2)) )
	{
		AfxMessageBox("two points on a line coincide");
	}
	else if(m_a1==m_c2&&m_b1==m_d2)
	{
		m_point.x=m_a1;
		m_point.y=m_b1;
	}
	else if(m_a2==m_c1&&m_b2==m_d1)
	{
		m_point.x=m_a2;
		m_point.y=m_b2;	
	}
	else if(((m_a2-m_a1)==0&&(m_c2-m_c1)==0)||(m_b2-m_b1)/(m_a2-m_a1)==(m_d2-m_d1)/(m_c2-m_c1)) 
	{	
		AfxMessageBox("parallel lines, no intersection");
	}
	else if( (m_a2-m_a1)==0 && (m_c2-m_c1)!=0 )
	{
		double k2=(m_d2-m_d1)/(m_c2-m_c1);
		m_point.x=m_a1;
		m_point.y=k2*m_a1-k2*m_c1+m_d1;
	}
	else if( (m_c2-m_c1)==0 && (m_a2-m_a1)!=0 )
	{
		double k1=(m_b2-m_b1)/(m_a2-m_a1);
		m_point.x=m_c1;
		m_point.y=k1*m_c1-k1*m_a1+m_b1;
	}
	else
	{
		double k1=(m_b2-m_b1)/(m_a2-m_a1);
		double k2=(m_d2-m_d1)/(m_c2-m_c1);
		m_point.x=(m_d1-m_b1+m_a1*k1-m_c1*k2)/(k1-k2);
		m_point.y=k1*m_point.x-m_a1*k1+m_b1;
	}	
	return m_point;
}

Cpoint Get_Vertical_Point(Cpoint point, Cpoint start , Cpoint end )
{
	double vp_point_x;
	double vp_point_y;
	double m_c1 = point.x;
	double m_d1 = point.y;
	double m_a1 = start.x;
	double m_b1 = start.y;
	double m_a2 = end.x;
	double m_b2 = end.y;

	if((m_c1-m_a1)==0 &&m_c1==m_a2)  //point on the line
	{
		vp_point_x=m_c1;
		vp_point_y=m_d1;
	}
	else if((m_d1-m_b1)/(m_c1-m_a1)==(m_b2-m_b1)/(m_a2-m_a1))  
	{
		vp_point_x=m_c1;
		vp_point_y=m_d1;
	}
	else if((m_b2-m_b1)==0)  
	{
		vp_point_x=m_c1;
		vp_point_y=m_b1;
	}
	else if((m_a2-m_a1)==0) 
	{
		vp_point_x=m_a1;
		vp_point_y=m_d1;
	}
	else
	{
		double k2=(m_a1-m_a2)/(m_b2-m_b1);
		double c2=1;  
		int i=0;
		while(c2==m_c1||c2==m_a1||c2==m_a2) 
		{
			c2+=(double)i;
			i++;
		}
		double d2=k2*(c2-m_c1)+m_d1;  
		Cpoint pCvp1(m_a1,m_b1);
		Cpoint pCvp2(m_a2,m_b2);
		Cpoint pCvp3(m_c1,m_d1);
		Cpoint pCvp4(c2,d2);
		Cpoint pCvp_vpoint = Get_Intersection_Point(pCvp1,pCvp2,pCvp3,pCvp4);
		vp_point_x=pCvp_vpoint.x;
		vp_point_y=pCvp_vpoint.y;
	}
	Cpoint vp;
	vp.x = vp_point_x;
	vp.y = vp_point_y;
	return vp;
}


Cpoint Get_Middle_Point(Cpoint point1, Cpoint point2)
{
	Cpoint cp1= point1;
	Cpoint cp2= point2;
	Cpoint cp;
	
	cp.x= (cp1.x+cp2.x)/2;
	cp.y= (cp1.y+cp2.y)/2;

	return cp;
}


//Transform the coordinate of map into the one of screen after dragging
int Get_New_Coordinate(int XorY, char Flag)  //Flag---x y
{
	int zoom_ratio1=1;     
	if (Flag=='x') 
	{
		int x=XorY;
		x= int(x*zoom_ratio);       //zoom
		x= x+ m_pointTopLeft.x + SIZEOFF_X;  	//Transform to the coordinate relative to "rectangle"
		return x;
	}
	if (Flag=='y')
	{
		int y= XorY;
		y= int(y*zoom_ratio);
		y= y+ m_sizeEllipse.cy;		//reverse
		y= y+ m_pointTopLeft.y+ SIZEOFF_Y;
		return y;
	}
	return -1;
}


int Get_Max_Value(int a, int b)
{
	if (a>b)
		return a;
	else
		return b;
}

double Get_Min_Value(double a, double b)
{
	if(a>=b)
		return b;
	else
		return a;
}

int Get_Min_Value(int a, int b)
{
	if(a>=b)
		return b;
	else
		return a;
}

int Get_Max_Value(int a, int b, int c)
{
	int max;

	max=a;

	if(max<b)
		max=b;

	if(max<c)
		max=c;

	return max;
}

int Get_New_Mean(int Average, int Sum_without_This_Value, int Value)
{
	int theAverage=0;
	theAverage= (Average * Sum_without_This_Value+Value)/(Sum_without_This_Value+1);

	return theAverage;
}

double Get_New_Mean(double Average, int Sum_without_This_Value, double Value)
{
	double theAverage=0;
	theAverage= (Average * Sum_without_This_Value+Value)/(Sum_without_This_Value+1);

	return theAverage;
}

//Type: P
Struct_Shortest_Path* Shortest_Path(char Type, int Start_Link_ID, int End_Link_ID)
{
	int Max_Number=100000000;
	int min;
	int u;
	int i;
	int j;
	int dist[MAX_ROUTE_LENGTH];
	int s[MAX_ROUTE_LENGTH];
	int path[MAX_ROUTE_LENGTH];
	int way[MAX_ROUTE_LENGTH];
	int way_1[MAX_ROUTE_LENGTH];    //length of Shortest_Path
	int Link_ID;
	int temp;
	int Start_Point = Link_Array[Start_Link_ID]->End_Object->Object_ID;
	int End_Point = Link_Array[End_Link_ID]->Start_Object->Object_ID;
	for(i=0; i<MAX_ROUTE_LENGTH; i++)
	{
		way[i]=-1;
		way_1[i]=-1;
	}
	for(i=0; i<MAX_ROUTE_LENGTH; i++)
	{
		if(Node_Relation[Start_Point][i] == NULL)
			dist[i] = Max_Number;
		else
		{
			Link_ID = Node_Relation[Start_Point][i]->Object_ID;
			switch (Type)
			{
			case 'D':
				dist[i] = Lane_Array[Link_ID][0]->Cell_Number;
				break;
			case 'T':
				dist[i]= Link_Array[Link_ID]->Current_On_Link_Time;
				break;
			}			
		}
		s[i]= 0;
		if(i!=Start_Point && dist[i]< Max_Number)
			path[i]=Start_Point;
		else
			path[i]=-1;
	}
	s[Start_Point]=1;
	for(i=0; i<MAX_ROUTE_LENGTH-1; i++)
	{
		min =Max_Number;
		u=Start_Point;
		for(j=0; j<MAX_ROUTE_LENGTH; j++)
			if(!s[j] && dist[j]<min)
			{
				u=j;
				min=dist[j];
			}
			s[u]=1;
			for(int w=0; w<MAX_ROUTE_LENGTH; w++)
			{
				/////////////////added
				if(Node_Relation[u][w] == NULL)
					temp = Max_Number;
				else
				{
					Link_ID = Node_Relation[u][w]->Object_ID;
					switch (Type)
					{
					case 'D':
						temp = Lane_Array[Link_ID][0]->Cell_Number;
						break;
					case 'T':
						temp= Link_Array[Link_ID]->Current_On_Link_Time;
						break;
					}
				}
				///////////////////added end

				if(!s[w] && dist[u]+temp<dist[w])
				{
					dist[w]= dist[u]+temp;
					path[w]= u;
				}			
			}
	}
	way[0]= End_Point;
	int k=End_Point;
	
	for(j=1; j<MAX_ROUTE_LENGTH; j++)
		if(k>=0)  
		{
			way[j]=path[k];   
			k=way[j];   
		}

	//////////added
	int temp_Link;
	int temp_vector=0;
	way_1[temp_vector]=Start_Link_ID;
	temp_vector++;
	for(i=1;i<MAX_ROUTE_LENGTH;i++)
	{
		if(way[i]!=-1)
		{
			temp_Link = Node_Relation[way[i]][way[i-1]]->Object_ID;
			way_1[temp_vector] = temp_Link;
			temp_vector++;
		}
		else
			break;
	}
	way_1[temp_vector]=End_Link_ID;

	Struct_Shortest_Path* spi=new Struct_Shortest_Path;
	spi->Shortest_Path_Dist =dist[End_Point];
	for (int i=0; i<MAX_ROUTE_LENGTH; i++)
		spi->Shortest_Path[i]= way_1[i];

	////add end

	//change the order of shortest path
	i=0;
	j=0;
	int w=0;
	int t=0;
	int temp1;
	while(spi->Shortest_Path [j]!=-1)
		j++;
	j--;
	t=(j-1)/2;
	for(w=0;w<t;w++)
	{
		temp1=spi->Shortest_Path [i+w+1];
		spi->Shortest_Path [i+w+1]=spi->Shortest_Path [j-w-1];
		spi->Shortest_Path [j-w-1]=temp1;
	}
	
	spi->End_Link_ID=End_Link_ID;

	return spi;
}


Struct_Shortest_Path * Get_Shortest_Path(int Start_Link_ID, int End_Link_ID)
{
	Struct_Shortest_Path *spi;
	extern CSimuFun *simuFun;
	CMainFrame *pMainFrame= (CMainFrame *)AfxGetApp()->m_pMainWnd;   

	if(simuFun->Information_Ready==true && pMainFrame->Show_Time_Shortest_Path==true)     //information has been ready &&time sp is chosen
		spi= Shortest_Path('T', Start_Link_ID, End_Link_ID);
	else
		spi = Shortest_Path_Array[Start_Link_ID][End_Link_ID];
	
	return spi;
}

bool Check_Load_or_Not()
{
	if (Open_File_PathName=="N")
	{
		AfxMessageBox("Please load a network first!");
		return false;
	}
	else
		return true;
}


bool Long_Time_Iteration(int* i)
{
	if(*i>= MAX_ITERATION_NUMBER)
		return true;
	else
	{
		*i=*i+1;
		return false;
	}
}

int Get_Random_Number(int Min, int Max)   //say, Min=10, Max=20
{
	int Ran= rand()%(Max-Min+1);   //then: 0-10
	Ran=Ran+Min;
	return Ran;
}


bool True_Or_False(double Prob, int Max_Value)
{
	int Ran_Number= Get_Random_Number(1, Max_Value);   ///get random number between 1-Max_Value

	if (Ran_Number<=int(Prob*Max_Value))
		return true;
	else
		return false;
}

CString Get_Current_Directory()
{
// 	//the directory of workspace
// 	char pBuf[MAX_PATH];  
// 	GetCurrentDirectory(MAX_PATH,pBuf); 
// 	strcat(pBuf,"\\");
// 	CString strtemp=pBuf;

	//the directory of exe
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);     
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;    
	CString strtemp=szFilePath;

	return strtemp;

}



//make sure there exist 7 meter space to front/back vehicle
bool Have_Space_In_Target_Lane(int Link_ID, int Lane_ID, int Cell_ID)
{
	if(Meter_Per_Cell>=VEHICLE_LENGTH)
	{
		if (Lane_Array[Link_ID][Lane_ID]->Lane_Cell[Cell_ID]->IsVehInCell()==false)
			return true;
		else 
			return false;
	}
	else 
	{
		int Cell_Number_In_Half_Side= VEHICLE_LENGTH;

		if ( (Cell_ID - Cell_Number_In_Half_Side >=0)  && (Cell_ID + Cell_Number_In_Half_Side <Lane_Array[Link_ID][Lane_ID]->Cell_Number-1) )   //intermediate of link
		{
			for (int i=Cell_ID - Cell_Number_In_Half_Side; i<Cell_ID + Cell_Number_In_Half_Side; i++)
				if (Lane_Array[Link_ID][Lane_ID]->Lane_Cell[i]->IsVehInCell()==true)
					return false;
			return true;
		}
		else if (Cell_ID-Cell_Number_In_Half_Side<0)   //the beginning of links
		{
			for (int i=0; i<Cell_Number_In_Half_Side; i++)
				if (Lane_Array[Link_ID][Lane_ID]->Lane_Cell[i]->IsVehInCell()==true)
					return false;
			return true;
		} 
		else //others
			return false;
	}
}