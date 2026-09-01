#include "stdafx.h"
#include "Origin.h"
#include "Global.h"
#include "Veh.h"
#include <math.h>
#include "Traveler.h"

COrigin::COrigin(int Located_Link_ID, int Located_Cell_ID, int Demand)
{
	this->Located_Link_ID=Located_Link_ID;
	this->Located_Cell_ID=Located_Cell_ID;
	this->Demand= Demand;
	this->Time_To_Generate_Veh=0;
    End_Link_ID=-1;

	extern int G_Commuter_Number_Per_Origin;
	Commuter_Number=G_Commuter_Number_Per_Origin;

	for (int i=0; i<MAX_COMMUTER_NUMBER_PER_ORIGIN; i++)
		Commuter_Array[i]=NULL;
}

COrigin::~COrigin(void)
{
}


//0. tell Link type
//1. Randomly choose
//2. If occupied, randomly choose another
//if all lane are occupied, return -1;
int COrigin::Get_Load_Lane_ID()
{
	int i=0;
	int Load_Lane_ID;
	int Sum_Of_Lane=Link_Array[Located_Link_ID]->Lane_Number;
	if (Located_Cell_ID!=0)
		Load_Lane_ID = Sum_Of_Lane -1; //the far right lane
	else
	{
		while (i<10)
		{
			Load_Lane_ID= Get_Random_Number(0, Sum_Of_Lane - 1);      //random line, between 0-(Sum_Of_Line-1)
			if(false==Link_Array[Located_Link_ID]->Lanes[Load_Lane_ID] ->Lane_Cell[Located_Cell_ID]->IsVehInCell())
				break;
			else
				i++;
		}
	}

	if (i==10)
		Load_Lane_ID= -1;

	return Load_Lane_ID;
}


void COrigin::Produce_Veh()
{
	if (Is_It_Time()==false)
		return;

	int Load_Lane_ID= Get_Load_Lane_ID();
	if (Load_Lane_ID==-1)
		return;

	bool Able_Flag = Have_Space_In_Target_Lane(this->Located_Link_ID, Load_Lane_ID, this->Located_Cell_ID);
	if (Able_Flag)
	{
		extern int Total_Veh_Number;
		Struct_Shortest_Path* spi= Get_Veh_SPI('D'); //shortest path is just for making sure there are routes between OD
		int End_Link_ID= spi->End_Link_ID;
		int Veh_Type= Get_Veh_Type();
		
		//CTraveler *pTraveler= new CTraveler(Traveler_Number_On_Network, Located_Link_ID, End_Link_ID, spi);
		CTraveler *pTraveler= new CTraveler(Traveler_Number_On_Network, Located_Link_ID, End_Link_ID);
		CVeh *p = new CVeh(Traveler_Number_On_Network, Veh_Type, Located_Cell_ID, pTraveler);
		
		Link_Array[Located_Link_ID]->Lanes[Load_Lane_ID] ->Lane_Cell[Located_Cell_ID]->PutVehInCell(p);
		Traveler_Number_On_Network++;
		Total_Veh_Number++;
		pSampleCollection->One_Depart('T');
	}
}



int COrigin::Get_Veh_Type()
{
	return 0;
}

bool COrigin:: Is_It_Time()
{
	bool flag;

// flag=Is_It_Time_Time();          //generate vehs by fix time
	flag= Is_It_Time_Prob();        //generate vehs by given demand in XML file
//	flag=Is_It_Time_Headway();   //generate vehs by poisson distribution

	return flag;
}

bool COrigin::Is_It_Time_Prob()
{
	extern int Current_Demand;
	
	if(!Slider_Demand_or_Not)
		Current_Demand= Get_Current_Demand();   
	else 
		Current_Demand= Current_Slider_Demand;

	bool flag= True_Or_False((double)Current_Demand/60, 1000);   // the unit of demand is veh/min, here we convert it to probability in binomial distribution here by over 60
	return flag;
}

bool COrigin::Is_It_Time_Headway()
{
	bool flag=false;
	extern int Current_Demand;
	
	if (simu_time>=this->Time_To_Generate_Veh)
	{
		Current_Demand	= Get_Current_Demand();
		double lambda= (double)Current_Demand/60;
		int head_way= Get_Headway(lambda);
		Time_To_Generate_Veh= simu_time+head_way;
	}

	return flag;
}

bool COrigin::Is_It_Time_Time()
{
	bool flag;
	if (simu_time==10)
		flag=true;
	else 
		flag=false;
	return flag;
}

int COrigin::Get_Current_Demand()
{
	if (Demand !=-1 )
		return Demand;

	extern int simu_time;
	extern int Start_Simu_Time;

	//all convert to seconds
	int x=simu_time+Start_Simu_Time;
	int xa=0;
	int xb=0;
	int ya=0;
	int yb=0;
	double y=0;

	double e=0;

	bool flag=false;     //true: current time falls into the given time interval

	int TIMES=1000;

	for (int i=0; i<G_Demand_Number-1;i++)
	{
		xa=int(G_Day_Length*(Demand_Array[i][0]));      //time in Demand_Array[i][0] is in day
		xb=int(G_Day_Length*Demand_Array[i+1][0]);
		ya=int(TIMES* Demand_Array[i][1]);   //the unit of demand is veh/min
		yb=int(TIMES* Demand_Array[i+1][1]);

		if (x>=xa&&x<xb)
		{
			flag=true;
			break;
		}
	}


	if (flag==true)    //two ends are a and b
	{
		//fixed part of demand
		double Fixed_Part=(x-xa)*(yb-ya)/(xb-xa)+ya;

		//random part of demand
		int ee= int(TIMES*e);
		int Rand_Part= 0;   //Get_Random_Number(-ee, ee);

		//demand
		y= Fixed_Part+Rand_Part;
		y=y/TIMES;
	}
	else
		y=0;

	return int(y);
}



Struct_Shortest_Path* COrigin::Get_Veh_SPI(char Type)  //'T': tsp, 'D': dsp
{
	return NULL;
}


int COrigin::Get_Headway(double lambda)
{
	if (lambda<0.000001)
	{
		return 1000000;
	}

	int Int_Headway=0;
	int Max_Number=10000;
	double ran= Get_Random_Number(0,Max_Number);
	ran=ran/Max_Number;
	double headway= -(1/lambda)*log(ran);

	Int_Headway= int(headway+0.5);
	if(Int_Headway==0)
		Int_Headway++;

	return Int_Headway;
}