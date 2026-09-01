#include "stdafx.h"
#include "OD.h"
#include "Global.h"
#include "Veh.h"

COD::COD(char Type, int Located_Link_ID, int Located_Cell_ID, bool Able_To_In_Network, bool Able_To_Out_Network)
{
	this->Type=Type;
	if(Type=='R')
		this->OD_ID=1000 + Located_Link_ID;
	else
		this->OD_ID=2000 + Located_Link_ID;
	this->Located_Link_ID=Located_Link_ID;
	this->Located_Cell_ID=Located_Cell_ID;

	this->Able_To_Out_Network= Able_To_Out_Network;
	this->Able_To_In_Network= Able_To_In_Network;

	this->Dest_Number=0;
	for (int i=0; i<MAX_DEST_NUMBER;i++)
	{
		this->Dest_Array[i][1]= -1;
		this->Dest_Array[i][2]= -1;
	}

}

COD::~COD(void)
{
}

double COD::Get_Current_Demand()
{
	extern int simu_time;
	extern int Start_Simu_Time;

	//all convert to seconds
	int x=simu_time+Start_Simu_Time;
	int xa=0;
	int xb=0;
	int ya=0;
	int yb=0;
	double y=0;

	double e=0.01;

	bool flag=false;     //true: current time falls into the given time interval
	
	int TIMES=1000;

	for (int i=0; i<G_Demand_Number-1;i++)
	{
		xa=int(3600*Demand_Array[i][0]);      //time in Demand_Array[i][0] is hour
		xb=int(3600*Demand_Array[i+1][0]);
		ya=int(TIMES*Demand_Array[i][1]);
		yb=int(TIMES*Demand_Array[i+1][1]);

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
		int Rand_Part= Get_Random_Number(-ee, ee);

		//demand
		y= Fixed_Part+Rand_Part;
		y=y/TIMES;
	}
	else
		y=0;

	return y;
}


//decide if it is the time of generating a new car by using Poission distribution
//flag: 
//-11:  not the time
//-1:    generate car regularly
//positive number: generate additional car to specific destination (Destination ID is the positive number)
int COD::Is_It_Time()
{
	int flag= -11;

	//option 1, Dynamic demand
	double Current_Demand_Percent= Get_Current_Demand();

	if (True_Or_False(Current_Demand_Percent, 1000))
	{
		flag= -1;
//		flag= -11;     //no veh generated from regular point but specific origin to destination
	}
	else
	{
		if (Dest_Number != 0)
		{
			//first, to which
			//second, generate or not
			int Which_One=Get_Random_Number(0, Dest_Number-1);     //0 -- (Dest_Number-1)
			int Dest_To_Go= Dest_Array[Which_One][0];
			double Dest_Demand_Percent=double(Dest_Array[Which_One][1])/1000;

			if (True_Or_False(Current_Demand_Percent*Dest_Demand_Percent, 1000))     //0.1 * 6 =0.6 (Max_Percent * Dest_Demand= percent)    0.6* 60=36 (36 veh per min)
				flag= Dest_To_Go;
			else
				flag= -11;
		}
		else
			flag= -11;	
	}


	//option 2, Constant demand
// 	bool flag= True_Or_False(double(Demand_From_One_Origin)/1000, 1000);
	
	//option 3 (0505)
// 	if (simu_time==10)
// 		flag=true;
// 	else 
// 		flag=false;

	return flag;
}

int COD::Get_Random_End_Link()
{
	int Sum_Of_Link= G_Link_Number;
	int i=0;

	while(true)
	{
		int ran= Get_Random_Number(0, Sum_Of_Link-1);   // random number between 0 and (SumOfLink-1)

		//the end link should be: able to go out network && not corridor
		if (true==Link_Array[ran]->The_OD->Able_To_Out_Network)
			return ran;
	
		///////////////////////////////////////////
		if(Long_Time_Iteration(&i))
			return -1;
		///////////////////////////////////////////

		Sleep(1);
	}

	return -1;
}

int COD::Get_Load_Lane_ID()
{
	//type 'R' the right one
	//type 'M' random one
	int Load_Lane_ID;
	int Sum_Of_Line=Link_Array[Located_Link_ID]->Lane_Number;

	switch (Type)
	{
	case 'R':
		Load_Lane_ID = Sum_Of_Line -1; //the rightest line
		break;

	case 'M':
		Load_Lane_ID= Get_Random_Number(0, Sum_Of_Line-1);  //random line, between 0-(Sum_Of_Line-1)
		break;
	}

    return Load_Lane_ID;
}	

bool COD::Is_It_Time_To_Moving_Bottleneck()
{
	double Fixed_Part= 0.3;    //0.07
//	double Rand_Part= double(Get_Random_Number(-1*200 , 1*200))/10000;    // [-0.01 0.01]
	double Rand_Part= 0;
	double Prob_Moving_Bottleneck= Fixed_Part+Rand_Part;

		if (True_Or_False(Prob_Moving_Bottleneck, 1000))    //[0.05 0.09]
			return true;

	return false;
}

//Enforce_Flag:  -1---no enforcement now;  >=0 destination link id
void COD::Produce_Veh(int Veh_Type, int Enforce_Flag)
{
	extern int Total_Veh_Number;
	int flag=-11;

	if(Enforce_Flag==-1)
		flag=Is_It_Time();
	else
	{
		if(Is_It_Time_To_Moving_Bottleneck())	
			flag=Enforce_Flag;
		else 
			flag=-11;
	}

	if (flag== -11)   //not the time
		return;
	
	Total_Veh_Number++;
	Struct_Shortest_Path* spi;
	int End_Link_ID;
	
	if (flag== -1)  //regular
	{
		int i=0;
		do 
		{
			End_Link_ID= Get_Random_End_Link();	

			if (End_Link_ID==-1)    //cannt find a End_Link_ID after a long time interation.
				return;

			//End_Link_ID=99;   //0505
			spi= Get_Shortest_Path(Located_Link_ID,End_Link_ID);

			///////////////////////////////////////////
			if(Long_Time_Iteration(&i))
				return;
			///////////////////////////////////////////

		} while (spi->Shortest_Path[MIN_LINK_NUMBER]<0 ||End_Link_ID==Located_Link_ID);
	}
	else   //additional, to specific destination assigned by XML file, flag is the destination
	{
		End_Link_ID= flag;
		spi= Get_Shortest_Path(Located_Link_ID,End_Link_ID);		
	}

// 	if (Enforce_Flag>=0)     //generate the same moving bottleneck at both lane 1 and 2
// 	{
// 		CVeh* p;
// 		int Load_Lane_ID;
// 		p = new CVeh(Type, Total_Veh_Number, Veh_Type, Located_Cell_ID, Located_Link_ID, End_Link_ID,spi);
// 		Load_Lane_ID=1;
// 		if(false==Lane_Array[Located_Link_ID][Load_Lane_ID] ->Lane_Cell[Located_Cell_ID]->IsVehInCell())
// 			Lane_Array[Located_Link_ID][Load_Lane_ID] ->Lane_Cell[Located_Cell_ID]->PutVehInCell(p);
// 
// 		p = new CVeh(Type, Total_Veh_Number+1, Veh_Type, Located_Cell_ID, Located_Link_ID, End_Link_ID,spi);
// 		Load_Lane_ID=2;
// 		if(false==Lane_Array[Located_Link_ID][Load_Lane_ID] ->Lane_Cell[Located_Cell_ID]->IsVehInCell())
// 			Lane_Array[Located_Link_ID][Load_Lane_ID] ->Lane_Cell[Located_Cell_ID]->PutVehInCell(p);
// 	}
// 	else
// 	{
		CVeh *p = new CVeh(Type, Total_Veh_Number, Veh_Type, Located_Cell_ID, Located_Link_ID, End_Link_ID,spi);
		int Load_Lane_ID=Get_Load_Lane_ID();
		if(false==Lane_Array[Located_Link_ID][Load_Lane_ID] ->Lane_Cell[Located_Cell_ID]->IsVehInCell())
			Lane_Array[Located_Link_ID][Load_Lane_ID] ->Lane_Cell[Located_Cell_ID]->PutVehInCell(p);
// 	}
}


bool COD::Able_To_In_Network_Now()
{
	if(Able_To_In_Network==false)
		return false;

	return true;	
}

int COD::Get_Headway(double lambda)
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