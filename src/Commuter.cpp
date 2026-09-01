#include "stdafx.h"
#include "Commuter.h"
#include "structure.h"
#include "Global.h"



/* //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
We design five types of commuters based on the resource that they choose the route and departure time
Commuter_Type, indicate the types
    predicting travel time based on  |  changing routes   |                choosing routes             |    Memory
	  experience  |   information       |    Yes   |     No     |    before departure  |   on routes      |
1          T                                                         T                    T                                            average
2          T                                           T                                  T                                            average
2          T                                           T                                  T                                            last day 
3                               T                      T                                  T
4                               T                                   T                     T
5                               T                      T                                                            T    
*/ //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*  //assign a dsp when construct the commuter
CCommuter::CCommuter(int Commuter_ID, int Commuter_Type, int Home_Location, int Work_Location, Struct_Shortest_Path * dsp, int Required_Arrival_Time)
:CDriver()
{	
	this->Chosen_Route=dsp;
	this->Home_Location= Home_Location;
	this->Work_Location= Work_Location;
	this->Required_Arrival_Time = Required_Arrival_Time;
	this->Actual_Arrival_Time= -1;

	this->Driver_Type='C';  //commuters
	this->Commuter_ID= Commuter_ID;
	this->Commuter_Type= Commuter_Type;

	for (int link_i=0; link_i<G_Link_Number; link_i++)
	{ 
		//initial travel time in memory: all are filled with fftt except [][0], because it is for prediction
		//commuters update Travel_Time_In_Memory[][] by experienced travel time in the corresponding position before entering new links.
		int fftt= Link_Array[link_i]->Length_In_FFTT;
		for (int day_i=1; day_i<MAX_SIMU_DAY_NUMBER+1; day_i++)
			Travel_Time_In_Memory[day_i][link_i]=fftt;    
	}

	for (int day_i=0; day_i<MAX_SIMU_DAY_NUMBER; day_i++)
		Departure_Time_Array[day_i]=-1;

}
*/




//calculate dsp with link+errors when it is generated, but it will make the initialization of simulation slow.
CCommuter::CCommuter(int Commuter_ID, int Commuter_Type, int Home_Location, int Work_Location, int Required_Arrival_Time)
:CDriver()
{	
	//generate the shortest path according to the fftt and perception errors(Get_Random_Number(0, 15))
	for (int link_i=0; link_i<G_Link_Number; link_i++)
	{ 
		//initial travel time in memory: all are filled with fftt except [][0], because it is for prediction
		//commuters update Travel_Time_In_Memory[][] by experienced travel time in the corresponding position before entering new links.
		int FFTT= Link_Array[link_i]->Length_In_FFTT;
		int Error_Percentage= Get_Random_Number(-10, 10)/100;  //perception errors  -10~10%
		for (int day_i=0; day_i<MAX_SIMU_DAY_NUMBER+1; day_i++)
			Travel_Time_In_Memory[day_i][link_i]=int(FFTT*(1- Error_Percentage));    
	}
	
	this->Chosen_Route=Get_Shortest_Path(Travel_Time_In_Memory[0], Home_Location, Work_Location);

	this->Home_Location= Home_Location;
	this->Work_Location= Work_Location;

	this->Required_Arrival_Time = Required_Arrival_Time;
	this->Actual_Arrival_Time= -1;

	this->Driver_Type='C';  //commuters
	this->Commuter_ID= Commuter_ID;
	this->Commuter_Type= Commuter_Type;



	for (int day_i=0; day_i<MAX_SIMU_DAY_NUMBER; day_i++)
		Departure_Time_Array[day_i]=-1;

}

CCommuter::~CCommuter(void)
{
}


bool CCommuter::Home_Or_Not()
{
	if (this->Current_Location == 'H')
		return true;
	else 
		return false;
}

bool CCommuter::Is_It_Time_To_Depart(int Predicted_Time, int Current_Time)
{
	if (this->Required_Arrival_Time - Predicted_Time <=Current_Time)
		return true;
	else 
		return false;

/*
	int Predicted_Arrival_Time = Predicted_Time + Current_Time;

	//if the travel time predicted by this tsp can guarantee commuters' arrival at the [Required_Arrival_Time - ARRIVAL_SCOPE, Required_Arrival_Time - ARRIVAL_SCOPE], depart!
	if (this->Required_Arrival_Time - ARRIVAL_SCOPE< Predicted_Arrival_Time     
		&& Predicted_Arrival_Time <= this->Required_Arrival_Time + ARRIVAL_SCOPE)
		return true;
	else 
		return false;
*/

}


bool CCommuter::Depart_Or_Not()
{
	extern int time_in_current_day;
	bool flag=false;
	int Travel_Time;
	bool Depart_Flag=false;


	//type 6 choose the route and departure time as the same as type3, but type 5 change have the ability of changing route on route.
	if (this->Commuter_Type==4 || this->Commuter_Type==6)   
	{
		Struct_Shortest_Path * tsp= Get_Shortest_Path('T', this->Home_Location, this->Work_Location);
		Travel_Time = tsp->Travel_Time_On_Shortest_Path;
		flag = Is_It_Time_To_Depart(Travel_Time, time_in_current_day);
		if (flag==false)
			Depart_Flag= false;
		else 
		{
			this->Chosen_Route= tsp;
			this->Predicted_Travel_Time =Travel_Time;
			Depart_Flag=true;
		}
	}
	//type 5
	else if (this->Commuter_Type==5)
	{
		Travel_Time =Get_Current_Travel_Time_On_Chosen_Route();
		flag = Is_It_Time_To_Depart(Travel_Time, time_in_current_day);
		if (flag==false)
			Depart_Flag=false;
		else 
		{
			this->Predicted_Travel_Time = Travel_Time;
			Depart_Flag=true;
		}
	}
	else //type 1,2,3
	{
		flag = Is_It_Time_To_Depart(Predicted_Travel_Time, time_in_current_day);
		Depart_Flag=flag;
	}

	return Depart_Flag;

}

//Get current travel time on chosen route based on provided information
int CCommuter::Get_Current_Travel_Time_On_Chosen_Route()
{
	int Link_ID_On_Route;
	int Travel_Time=0;

	for (int i=0; i<MAX_ROUTE_LENGTH; i++)
	{
		Link_ID_On_Route= this->Chosen_Route->Shortest_Path[i];
		if (Link_ID_On_Route!=-1)
			Travel_Time = Travel_Time + Link_Array[Link_ID_On_Route]->Current_On_Link_Time;
		else
			break;
	}

	return Travel_Time;
}


int CCommuter::Get_Start_Link_ID()
{
	return Home_Location;
}


int CCommuter::Get_End_Link_ID()
{
	return Work_Location;
}

Struct_Shortest_Path * CCommuter::Get_SP_Between_OD()
{
	return Chosen_Route;
}

char CCommuter::Get_Driver_Type()
{
	return Driver_Type;
}


char CCommuter::Get_Current_Location()
{
	return Current_Location;
}


void CCommuter::Set_After_Arrival( int Arrival_Time)
{
	this->Actual_Arrival_Time= Arrival_Time;
	this->Current_Location= 'W';
}


void CCommuter::Set_At_Day_Beginning()
{
	this->Actual_Arrival_Time=0;
	this->Predicted_Travel_Time= Choose_Route_And_Predict_Travel_Time();   //in this function, commuters' route (i.e., shortest path this time) is updated 
	this->Current_Location= 'H';
	this->Current_Link_ID= Home_Location;
}


//in this function, commuters' route (i.e., shortest path this time) is updated inside
int CCommuter::Choose_Route_And_Predict_Travel_Time()
{
	double Predicted_Time=0;
	extern int current_day;

	if (current_day==1)   //at the beginning of the first day, commuters choose routes based on dsp
	{
		//should be:
		//Experience_Accumulation();
		//Struct_Shortest_Path * spi= Shortest_Path( this->Travel_Time_In_Memory[0][MAX_LINK_NUMBER], this->Home_Location, this->Work_Location);
		//but for save some time, we use directly: 
		//the shortest path is updated at the Constructor of CCommuter
		Predicted_Time = this->Chosen_Route->Travel_Time_On_Shortest_Path;
	}
	else
	{
		Experience_Accumulation();  //all types of commuters need to accumulate experience
		
		//only type 1 2 3 choose route and departure time here
		//type 4, 5 6 donot (see Depart_Or_Not())
		if (Commuter_Type==2 || Commuter_Type==3)
		{
			Chosen_Route= Get_Shortest_Path(Travel_Time_In_Memory[0], this->Home_Location, this->Work_Location);    //choose a new route
			Predicted_Time = Chosen_Route->Travel_Time_On_Shortest_Path;
		}

		if (Commuter_Type==1)
		{
			int link_id;
			Predicted_Time=0;
			for (int i=0; i<MAX_ROUTE_LENGTH;i++)
			{
				//only update the predicted travel time based on experience, but donot change the route, which is dsp
				link_id= Chosen_Route->Shortest_Path[i];
				if (link_id==-1)
					break;
				else
					Predicted_Time= Predicted_Time + Travel_Time_In_Memory[0][link_id];    //Travel_Time_In_Memory[0][link_id]-->the average travel time in memory on link_id
			}
		}
	}
	
	return (int)Predicted_Time;
}


//obtain Travel_Time_In_Memory[][0] for predicted travel time from Travel_Time_In_Memory[others][]
//mean of experienced travel time
//commuters predict travel time before departure on current_day;
void CCommuter::Experience_Accumulation()
{
	if (this->Commuter_Type==3)
	{
		memcpy(Travel_Time_In_Memory[0], &Travel_Time_In_Memory[current_day-1], 4*MAX_LINK_NUMBER);
		return ;
	}


	int sum=0;
	for (int link_i=0; link_i<G_Link_Number; link_i++)
	{
		for (int day_i=1; day_i<current_day; day_i++)       //from "day 1" to "current_day-1"
			sum= sum+Travel_Time_In_Memory[day_i][link_i];

		Travel_Time_In_Memory[0][link_i]=sum/(current_day-1); 
		sum=0;
	}

}


void CCommuter::Set_Experienced_Travel_Time(int Link_ID, int Experienced_Travel_Time)
{
	extern int current_day;
	Travel_Time_In_Memory[current_day][Link_ID]=Experienced_Travel_Time; 

}

//Type: 'R'--leave network regularly; 'i': irregularly
void CCommuter::Record_Travel_Experience_On_Link(int Simu_Time, int New_Link_ID)
{
	if (this->Commuter_Type==1)
		int aaaaa=0;

	int Travel_Time_On_Link_And_Intersection= Simu_Time - this->Enter_Link_Time;    //Simu_Time is the time that veh enters a new link, and Enter_Link_Time is the time that veh enter the last link
	Travel_Time_In_Memory[current_day][Current_Link_ID]=Travel_Time_On_Link_And_Intersection;

	this->Enter_Link_Time= Simu_Time;
	this->Current_Link_ID= New_Link_ID;
}


bool CCommuter::Able_To_Change_Route_On_Route()
{
	if (this->Commuter_Type==6)
		return true;
	else
		return false;
}

char CCommuter::Chosen_Veh_Color()
{
	switch (this->Commuter_Type)
	{
	case 1:
		return 'R';
	case 2:
		return 'K';
	case 3:
		return 'W';
//	case 4:
//		return 'B';
//	case 5:
//		return 'Y';
	}
	return 'X';
}

