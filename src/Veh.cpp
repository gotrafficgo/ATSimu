
#include "stdafx.h"
#include "ErrorLog.h"
#include "Veh.h"
#include "Cell.h"
#include "traffic.h"
#include "Global.h"
#include "CarFollowingModels.h"
#include "setting.h"
#include <math.h>
#include "Dest.h"

CVeh::CVeh(int Veh_ID, int Veh_Type, int Start_Cell_ID, int Start_Link_ID, int End_Link_ID, Struct_Shortest_Path* Shortest_Route_Path)
{
	this->Veh_ID=Veh_ID;
	this->Start_Link_ID= Start_Link_ID;
	this->End_Link_ID=End_Link_ID;

	for (int j=0;j<MAX_ROUTE_LENGTH;j++)
		if (Shortest_Route_Path->Shortest_Path[j]!=-1)
			Current_Route_Array[j]= Shortest_Route_Path->Shortest_Path[j];
	
	Current_Link_Index = 0;       									
	Set_Dest_Lane(Start_Link_ID); 

	Veh_State = 0;  
	Driver_Type = Get_Random_Number(0,2);
	this->Veh_Type =  Veh_Type;

	Max_Speed_Percent= Set_Max_Speed_Percent();     //Set_Max_Speed() must exist prior to Set_Init_Speed()
	Set_On_Link_Max_Speed(Start_Link_ID);
	Cur_Spd = Set_Init_Speed(Start_Cell_ID);

	Last_Spd=Cur_Spd;
	
	Cell_Loc=Start_Cell_ID;
	Last_Cell_Loc=Start_Cell_ID;

	Link_Loc=Start_Link_ID;
	Lane_Loc=-1;

	Receive_Guidance=-1;
	Detected_Flag=-1;
	Cross_Move_Check_Time= -1;
	Waiting_Time=0;
	Dest_Lane_Number=-1;    
	Simutime_Passing_Detector_Middle=  -1;

	Set_Veh_Color();

//	Freshman=true;
}


CVeh::~CVeh()
{
}

void  CVeh::Set_Veh_Color()
{
//     set color by random
// 	int Ran= Get_Random_Number(0,3);
// 	if (Ran==0)
// 		return 'R';
// 	if (Ran==1)
// 		return 'K';
// 	if (Ran==2)
// 		return 'G';
// 	if (Ran==3)
// 		return 'B';
// 
// 	return 'O';

	//use uniform color
	//this->Veh_Color='G';

	switch (Veh_Type)
	{
	case 0:
		this->Veh_Color='G';
		break;
	case 1:
		this->Veh_Color='B';
		break;
	case 2:
		this->Veh_Color='K';
		break;
	}
}

double CVeh::Set_Max_Speed_Percent()
{
	double percent;
	switch (Veh_Type)
	{
	case 0:
		percent= MAX_SPEED_PERCENT_1;
		break;
	case 1:
		percent= MAX_SPEED_PERCENT_2;
		break;
	case 2:
		percent= MAX_SPEED_PERCENT_3;
		break;
	}
	return percent;

}

void CVeh::Set_On_Link_Max_Speed(int Link_ID)
{
	this->Max_Speed=int (Link_Array[Link_ID]->Limited_Speed * this->Max_Speed_Percent);
}

int CVeh::Set_Init_Speed(int Start_Cell_ID)
{
	int Init_Speed;

	if (Start_Cell_ID==0)
		Init_Speed=this->Max_Speed;
	else
		Init_Speed=this->Max_Speed;

	return Init_Speed;
}


//apply Clear_Dest_Lane_Array() before using this
void CVeh::Set_Dest_Lane()
{
	int Next_Link=-1;
	int Next_Next_Link=-1;
	int a, b, c;                 // record the number of left, straight and  right turn lane
	int i;
	int j;
	j=0;
	a=0;
	b=0;
	c=0;

	if(Current_Link_Index< MAX_ROUTE_LENGTH-2 )
	{
		Next_Link = Current_Route_Array[Current_Link_Index+1]; 
		Next_Next_Link = Current_Route_Array[Current_Link_Index+2];
	} 

	if(Next_Link < 0)   //current link is the destination link
	{
		AfxMessageBox("fault: already been destination");
		return;  
	}

	if(Next_Next_Link <0)  //there is no next-next link, then all lane is destination lane
	{
		if (Link_Array[Next_Link]->End_Object->Object_Type=='M')   //drive out of network
		{
			for(i=0;i<Link_Array[Next_Link]->Lane_Number;i++)
			{
				if(i>=MAX_LANE_NUMBER)
					AfxMessageBox("Dest_Lane  out of boundary");
				else
					Dest_Lane[i] = i;
			}
		} 
		else  //destination inside network
		{
			Dest_Lane[0]=Link_Array[Next_Link]->Lane_Number - 1;   //the far right one
		}

		return;
	}

    for(i=0;i<Link_Array[Next_Link]->Lane_Number;i++)
	{
		if(Lane_Array[Next_Link][i]->Left_Turn)     //the i th lane, able to left turn
		{
			if(Link_Array[Next_Link]->Next_Left_Link == Next_Next_Link	||Link_Array[Next_Link]->Next_UTurn_Link==Next_Next_Link) 
			{
				Dest_Lane[j] = i;
				j++;
				a++;
			}
		}
		else if(Lane_Array[Next_Link][i]->Straight_Turn)
		{
			if(Link_Array[Next_Link]->Next_Straight_Link == Next_Next_Link) 
			{
				Dest_Lane[j] = i;
				j++;
				b++;
			}
		}   
		else if(Lane_Array[Next_Link][i]->Right_Turn)
		{
			if(Link_Array[Next_Link]->Next_Right_Link == Next_Next_Link) 
			{
				Dest_Lane[j] = i;
				j++;
				c++;
			}
		}   
		else 
			AfxMessageBox("Set_Dest_Lane, no turning direction!"); 
	}

	if (a>1)
		Dest_Lane_Number=a;
	else if(b>1)
		Dest_Lane_Number=b;
	else if (c>1)
		Dest_Lane_Number=c;
	else
		Dest_Lane_Number=-1;

	if(Dest_Lane[0]==-1)
	{
		AfxMessageBox("Dest lane can not be nothing");
	}
}                    


void CVeh::Set_Dest_Lane(int Link_ID)
{	
	int i,j;
	j=0;
	int Next_Link=-1;
	if(Current_Link_Index<MAX_ROUTE_LENGTH-2)
	{
		Next_Link = Current_Route_Array[Current_Link_Index+1];
	} 
	else
	{
		AfxMessageBox("Error---CVeh::Set_Dest_Lane()~~~1");     
		return;
	}

	if(Next_Link < 0)
	{
		AfxMessageBox("Error---CVeh::Set_Dest_Lane()~~~2"); 
		return;
	}

	for(i=0;i<Link_Array[Link_ID]->Lane_Number;i++)
	{
		if(Lane_Array[Link_ID][i]->Left_Turn)
		{
			if(Link_Array[Link_ID]->Next_Left_Link == Next_Link||Link_Array[Link_ID]->Next_UTurn_Link==Next_Link) 
			{
				Dest_Lane[j] = i;
				j++;
			}
		}
		else if(Lane_Array[Link_ID][i]->Straight_Turn)
		{
			if(Link_Array[Link_ID]->Next_Straight_Link == Next_Link)
			{
				Dest_Lane[j] = i;
				j++;
			}
		}   
		else if(Lane_Array[Link_ID][i]->Right_Turn)
		{
			if(Link_Array[Link_ID]->Next_Right_Link == Next_Link)
			{
				Dest_Lane[j] = i;
				j++;
			}
		}   
	}


	int flag=0;
	for (i=0;i<Link_Array[Next_Link]->Lane_Number;i++)
	{
		if (Dest_Lane[i]!=-1 )
		{
			flag=1;
			break;
		}
	}

	if(Dest_Lane[0]==-1)
		AfxMessageBox("Dest lane can not be nothing");

}                    


void CVeh::Clear_Dest_Lane_Array()
{
	int i;
	for(i=0;i<MAX_DEST_LANE_NUMBER;i++) 
		Dest_Lane[i] = -1;
}


//delete the vehicle in current cell, return this vehicle's pointer
CVeh * CVeh::Delete_Current_Link_Cell_Veh(int Link_ID,int Lane_ID, int Cell_ID)
{
	CVeh *p = NULL;
	p = Lane_Array[Link_ID][Lane_ID]->Lane_Cell[Cell_ID]->GetVehFromCell();
	if(p == NULL)
	{
		AfxMessageBox("didn't find the vehicle  \n Delete_Current_Link_Cell_Veh");
		return 0;
	}
	Lane_Array[Link_ID][Lane_ID]->Lane_Cell[Cell_ID]->PutVehInCell(NULL);

	return p;	
}



//if or not on destination lane
bool CVeh::Dest_Lane_or_Not(int Lane_ID,int Lane_Number)
{
	bool On_Dest_Lane= false;
	for(int i=0;i< Lane_Number;i++)
	{
		if(Lane_ID == Dest_Lane[i])
			On_Dest_Lane=true;
	}
	return On_Dest_Lane;
}


bool CVeh::Change_Route(int Current_Link_ID, int Next_Link)
{
	int i;
	bool flag=false;
	Struct_Shortest_Path* Shortest_Route_Path ;
	Shortest_Route_Path = ShortestPath_with_Guidance(Next_Link);   
	flag=Last_Route_Link(Shortest_Route_Path);

	if(flag==true)
	{
		for(i=0; i<MAX_ROUTE_LENGTH-i-1; i++)
			Current_Route_Array[Current_Link_Index+i+1]=Shortest_Route_Path->Shortest_Path[i]; 
		return true;	
	}
	return false;
}

bool CVeh::Change_Dest(int Current_Link_ID, int Next_Link)
{
	int i;
	int j;
	bool flag=false;
	Struct_Shortest_Path* Shortest_Route_Path ;
	int New_End_Link_ID=End_Link_ID;
	int Up_or_Down=1;
	int iteration_number=0;

	while(iteration_number<100)    //just in case, prevent from dead loop 
	{
		iteration_number++;
		Shortest_Route_Path= Get_Shortest_Path(Next_Link, New_End_Link_ID);
		flag=Last_Route_Link(Shortest_Route_Path);
		if(flag==true)
		{
			End_Link_ID= New_End_Link_ID;
			break;
		}
		else   //otherwise change End_Link_ID
		{
			if (Up_or_Down==1)
			{
				New_End_Link_ID++;
				if (Link_Array[New_End_Link_ID]==NULL)
				{
					Up_or_Down=-1;
					New_End_Link_ID=End_Link_ID;
				}
			}
			else
			{
				New_End_Link_ID--;
				if (Link_Array[New_End_Link_ID]==NULL)
					break;
			}
		}
	}
	
	if (flag==true)	//set new route, and save it to Current_Route_Array[]
	{
		j=0;
		for(i=0; i< MAX_ROUTE_LENGTH;i++)
		{
			if(this->Current_Route_Array[i]==Current_Link_ID) //find out the index of current link in Current_Route_Array[]
			{
				for(j=0; j<MAX_ROUTE_LENGTH-i-1; j++)
				{  
					this->Current_Route_Array[i+j+1]=Shortest_Route_Path->Shortest_Path[j];
				}
				Current_Link_Index=i;
				return true;				
			} 	
		}
	} 
	return false;

}

void CVeh::Drive_on_Dest_Lane(int Link_ID,  int Lane_ID,  int Cell_ID)
{
	double Big_Number=2000000.0;
	int Cur_FrontVeh_Loc=int(Big_Number);
	double Cur_FrontVeh_Spd=Big_Number;
	int Cur_Location;
	double Cur_Speed;
	int Last_FrontVeh_Loc=int(Big_Number);
	double Last_FrontVeh_Spd=0;

	CVeh * Front_pVeh=NULL;
	Front_pVeh=Get_Front_Veh_on_Link(Link_ID,Lane_ID, Cell_ID, Lane_Array[Link_ID][Lane_ID]->Cell_Number-1);

	if (Front_pVeh!=NULL)
	{
		Cur_FrontVeh_Loc=Front_pVeh->Cell_Loc;
		Cur_FrontVeh_Spd=Front_pVeh->Cur_Spd;

		Last_FrontVeh_Loc= Front_pVeh->Last_Cell_Loc;
		Last_FrontVeh_Spd= Front_pVeh->Last_Spd;
	}
	else //there is no ahead veh
	{
		Info_Light_Color_and_Time LCT;
		char Light_Color;
		int Dest;
		Dest= Dest_Lane[0];            //watch traffic light

		if ( Lane_Array[Link_ID][Dest]->Phase!=NULL)
		{
			LCT = Lane_Array[Link_ID][Dest]->Phase->Get_Phase_LCT();     
			Light_Color=LCT.Light_Color;
		}
		else 
			Light_Color='G';

		if(Light_Color == 'R')     //red light
		{
			Cur_FrontVeh_Loc= Lane_Array[Link_ID][Lane_ID]->Cell_Number;      //suppose the front veh is at the end of the link, Note the last Cell ID is (Cell_Number-1).
			Cur_FrontVeh_Spd= 0;       

			Last_FrontVeh_Loc=  Lane_Array[Link_ID][Lane_ID]->Cell_Number;      //suppose the front veh is at the end of the link, Note the last Cell ID is (Cell_Number-1).
			Last_FrontVeh_Spd= 0; 
		}
		else  //able to access intersection.
		{
			//suppose the loc and spd is very big, so that the veh can gradually slow down
			Cur_FrontVeh_Loc= int(Big_Number);
			Cur_FrontVeh_Spd= Big_Number;

			Last_FrontVeh_Loc= int(Big_Number);
			Last_FrontVeh_Spd= Big_Number;
		}
	}

	Cur_Speed= Get_Cur_Speed(Cell_ID, int(this->Cur_Spd), Last_FrontVeh_Loc, Last_FrontVeh_Spd, this->Max_Speed);
	Cur_Location=Get_Cur_Location(Cell_ID, Cur_Speed);

	//prevent from crashing
	if (Cur_Location>=Cur_FrontVeh_Loc)
	{
		Cur_Location=Cur_FrontVeh_Loc-1;
		Cur_Speed=0;
	}

	if(true==Leave_Net_Or_Not(Link_ID, Lane_ID, Cell_ID,  Cur_Location))    //finish the trip: leave the network or arrive destination
		return;

	if (true==Able_To_Enter_Cross(Link_ID, Lane_ID, Cell_ID, Cur_Location))
	{
		int Cross_ID= Link_Array[Link_ID]->End_Object->Object_ID;
		int Cross_Lane_ID= Choose_Cross_Lane(Cross_ID, Link_ID, Lane_ID);
		if (true==Have_Space_Next_Link(Cross_ID,Cross_Lane_ID))
		{
			Enter_Cross(Link_ID,Lane_ID, Cell_ID,Cross_Lane_ID);
			return; 
		}
		else
		{ 
			Cur_Speed=0;
			Cur_Location=Cell_ID;
		}
	}
	
	Veh_Run_on_Link(Link_ID, Lane_ID, Cell_ID, Link_ID, Lane_ID, Cur_Location, Cur_Speed);
}               		                                                            

int CVeh::Get_Wait_Cell_ID(int Link_ID, int Lane_ID)
{
	// 	int THE_ECL;
	//		THE_ECL =  ENFORCE_CHANGE_LINE + Link_Array[Link_ID]->The_OD->Located_Cell_ID;
	//		THE_ECL =  ENFORCE_CHANGE_LINE;
	// 	int Sum_Of_Front_Cell= Lane_Array[Link_ID][0]->Cell_Number - Cell_ID;
	int THE_FCLD;
	int Wait_Cell_ID;
	int Line_Amount_Between_Dest;
	Line_Amount_Between_Dest=Get_Lane_Number_to_Dest_Lane(Lane_ID);	

	if (Link_ID==End_Link_ID)
		THE_FCLD = PROHIBIT_CHANGE_LANE_DISTANCE + Link_Array[Link_ID]->Dest->Located_Cell_ID;
	else
		THE_FCLD = PROHIBIT_CHANGE_LANE_DISTANCE;

	//no veh can pass Wait_Cell_ID
	Wait_Cell_ID= Lane_Array[Link_ID][Lane_ID]->Cell_Number-1 - THE_FCLD - Line_Amount_Between_Dest;

	return Wait_Cell_ID;
}


void CVeh::Drive_not_on_Dest_Lane(int Link_ID, int Lane_ID, int Cell_ID)
{
	/*
	MAX    			 a   Cell_ID   b
	-------------------------------------------
   | No Merging |************|
	-------------------------------------------
	*/

	int a= Lane_Array[Link_ID][Lane_ID]->Cell_Number- (PROHIBIT_CHANGE_LANE_DISTANCE/Meter_Per_Cell);
	int b=  Lane_Array[Link_ID][Lane_ID]->Cell_Number-1- (LONG_TIME_WAIT_REGION / Meter_Per_Cell);
	if (Cell_ID>=b&&Cell_ID<a)
	{
		if (Cur_Spd==0)
			Waiting_Time++;
		else
			Waiting_Time=0;

		if (Waiting_Time > MAX_WAITING_TIME)
		{
			After_Long_Time_Waiting(Link_ID,Lane_ID);
			Waiting_Time=0;
			return;
		}
	}

	//////////////////////////////////////////////////////////
	CVeh * Front_pVeh=NULL;
	int Cur_FrontVeh_Loc;
	double Cur_FrontVeh_Spd;
	int Cur_Location;
	double Cur_Speed;
	int Wait_Cell_ID;
	int Last_FrontVeh_Loc;
	double Last_FrontVeh_Spd;

	Wait_Cell_ID= Get_Wait_Cell_ID(Link_ID, Lane_ID);
	Front_pVeh=Get_Front_Veh_on_Link(Link_ID,Lane_ID, Cell_ID, Wait_Cell_ID);

	if (Front_pVeh!=NULL)
	{
		Cur_FrontVeh_Loc=Front_pVeh->Cell_Loc;
		Cur_FrontVeh_Spd=Front_pVeh->Cur_Spd;

		Last_FrontVeh_Loc=Front_pVeh->Last_Cell_Loc;
		Last_FrontVeh_Spd=Front_pVeh->Last_Spd;
	}
	else
	{
		Cur_FrontVeh_Loc= Wait_Cell_ID;
		Cur_FrontVeh_Spd=0;

		Last_FrontVeh_Loc=Wait_Cell_ID;
		Last_FrontVeh_Spd=0;
	}

	Cur_Speed= Get_Cur_Speed(Cell_ID, int(this->Cur_Spd), Last_FrontVeh_Loc, Last_FrontVeh_Spd, this->Max_Speed);
	Cur_Location=Get_Cur_Location(Cell_ID, Cur_Speed);

////////////////
//prevent from crashing
	if (Cur_Location>=Wait_Cell_ID)
		Cur_Location=Wait_Cell_ID-1;

	if(Front_pVeh!=NULL)
		if(Cur_Location>=Front_pVeh->Cell_Loc)
			Cur_Location=Front_pVeh->Cell_Loc-1;

	int Merged_or_Not= -1;
// 	if(true==Begin_Merge_At_Some_Location())
	Merged_or_Not= Change_Lane(Link_ID, Lane_ID, Cell_ID, Cur_Location, Cur_Speed);

	if (Merged_or_Not==1)
		return;

	Veh_Run_on_Link(Link_ID, Lane_ID, Cell_ID, Link_ID, Lane_ID, Cur_Location, Cur_Speed);

}

double CVeh::Get_Cur_Speed(int Veh_Loc, double Veh_Spd, int FrontVeh_Loc, double FrontVeh_Spd, double Max_Speed)
{
	double Cur_Speed=-1;
	switch (Car_Moving_Model_Type)
	{
		case 'A':
			Cur_Speed= (double)Speed_From_VDR(Veh_Loc, (int)Veh_Spd, FrontVeh_Loc,(int)Max_Speed);		
			break;
		case 'F':
			switch (Car_Following_Model_Type)
			{
				case 'G':
					Cur_Speed= Speed_From_Gipps(Veh_Loc, Veh_Spd, FrontVeh_Loc, FrontVeh_Spd, Max_Speed);
					break;
				case 'I':
//					Cur_Speed= Speed_From_IDM(Veh_Loc, Veh_Spd, FrontVeh_Loc,FrontVeh_Spd,Max_Speed);
					break;
				case 'N':
					break;
			}			
			break;
	}
	return Cur_Speed;
}
int CVeh::Get_Cur_Location(int Cur_Loc, double Cur_Spd)
{
	int Cur_Location=-1;
	switch (Car_Moving_Model_Type)
	{
		case 'A':
			Cur_Location= Location_From_VDR(Cur_Loc, (int)Cur_Spd);
			break;
		case 'F':
			switch (Car_Following_Model_Type)
			{
				case 'G':
					Cur_Location= Location_From_Gipps(Cur_Loc, Cur_Spd);
					break;
				case 'I':
//					Cur_Location= Location_From_IDM(Cur_Loc, Cur_Spd);
					break;
				case 'N':
					break;
			}			
			break;
	}
	return Cur_Location;
}


bool CVeh::Begin_Merge_At_Some_Location(int Link_ID, int Cell_ID)      //vehs begin to change lane after passing "This_Location"
{
	int This_Location =43;
	if (Link_ID==This_Location )    //cell number of Link 43 is 231
	{
		if(Cell_ID>260)
			return true;
		else 
			return false;
	}
	else
		return true;
}

int CVeh::Change_Lane(int Link_ID, int Lane_ID, int Cell_ID, int Cur_Location, double Cur_Speed)
{
	int Lane_Changed_Or_Not=-1;    //-1 don't want to change lane; 0 wants to change, but no space; 1 succeed
	int Change_Desitnate_Lane;
	Change_Desitnate_Lane= Get_Dest_Lane(Lane_ID);

	//processing sequence: smaller number lane to bigger number lane
	//lane-changing from processed lane to unprocessed lane
	if(Change_Desitnate_Lane > Lane_ID) 
	{
		// don't do anything but moving the veh to the adjacent cell in the adjacent lane
		int Cell_ID_Before_Proc= Cell_ID;
		double Speed_Before_Proc= this->Cur_Spd;
		if (true==Decide_Change_Lane(Cell_ID))
		{
			if (Have_Space_In_Target_Lane(Link_ID, Change_Desitnate_Lane, Cell_ID_Before_Proc))
			{
				Veh_Run_on_Link(Link_ID, Lane_ID, Cell_ID, Link_ID, Change_Desitnate_Lane, Cell_ID_Before_Proc, this->Cur_Spd);
				Lane_Changed_Or_Not=1;
			}
			else    //if driver wants to change lane, but there is not enough space, then he decelerate a little (return 0).
			{
				this->Cur_Spd= Get_Random_Number(0, (int)Cur_Spd);   //random decelerate if want to change lane but can't
				Lane_Changed_Or_Not=0;
			}
		}
		else
			Lane_Changed_Or_Not= -1;
	}
	else  //bigger id lane to smaller id lane; i.e., unprocessed lane to processed lane
	{
		int Cell_ID_After_Proc= Cur_Location;
		double Speed_After_Proc= Cur_Speed;
		if (true==Decide_Change_Lane(Cell_ID))
		{
			if (Have_Space_In_Target_Lane(Link_ID, Change_Desitnate_Lane, Cell_ID_After_Proc))
			{
				Veh_Run_on_Link(Link_ID, Lane_ID, Cell_ID, Link_ID, Change_Desitnate_Lane, Cell_ID_After_Proc, Speed_After_Proc);
				Lane_Changed_Or_Not=1;
			}
			else
			{
				this->Cur_Spd= Get_Random_Number(0, (int)Cur_Spd);   //random decelerate if can't 
				Lane_Changed_Or_Not=0;
			}
		}
		else
			Lane_Changed_Or_Not= -1;
	}

	return Lane_Changed_Or_Not;
}





//make sure there exist 7 meter space to front/back vehicle
bool CVeh::Have_Space_In_Target_Lane(int Link_ID, int Lane_ID, int Cell_ID)
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

		//out of boundry
		if ( (Cell_ID - Cell_Number_In_Half_Side <0)  || (Cell_ID + Cell_Number_In_Half_Side> Lane_Array[Link_ID][Lane_ID]->Cell_Number-1) )
			return false;

		for (int i=Cell_ID - Cell_Number_In_Half_Side; i<Cell_ID + Cell_Number_In_Half_Side; i++)
		{	
			if (Lane_Array[Link_ID][Lane_ID]->Lane_Cell[i]->IsVehInCell()==true)
				return false;
		}
		return true;
	}	
}



int CVeh::Choose_in_Dest_Lanes(int Link_ID, int Lane_ID, int Cell_ID)
{
	bool Is_Able_To_Choose= false;  //there is a space for lane-changing
	bool Upside=false;
	bool Downside=false;
	int Change_Dest_Lane=-1;
	
	if (Dest_Lane_Number>1)
	{ 
		//precondition, vehicle has been in destination lane
		if (Lane_ID==0)  
		{
			Is_Able_To_Choose=true;
			for (int i=1;i<Dest_Lane_Number;i++)  //if one is not destination lane, then change the value of Is_Able_To_Choose
				if (false ==Dest_Lane_or_Not(Lane_ID+i, Link_Array[Link_ID]->Lane_Number) )
				{
					Is_Able_To_Choose=false;
					Upside=true;
					Downside=false;
				}
		}
		else if (Lane_ID==Link_Array[Link_ID]->Lane_Number-1)
		{
			Is_Able_To_Choose=true;
			for (int i=Link_Array[Link_ID]->Lane_Number-1;   i>Link_Array[Link_ID]->Lane_Number-Dest_Lane_Number; i--)
				if (false==Dest_Lane_or_Not(Lane_ID-i, Link_Array[Link_ID]->Lane_Number))
				{
					Is_Able_To_Choose=false;
					Downside=true;
					Upside=false;
				}
		}
		else
		{   //do not distinguish 2 or 3
			if (true==Dest_Lane_or_Not(Lane_ID-1, Link_Array[Link_ID]->Lane_Number) )
			{
				Is_Able_To_Choose=true;
				Downside=true;
				Upside=false;
			}
			else if( true==Dest_Lane_or_Not(Lane_ID+1, Link_Array[Link_ID]->Lane_Number))
			{ 
				Is_Able_To_Choose=true;
				Upside=true;
				Downside=false;
			}
		}

		///////////////////////////////////
		if (Is_Able_To_Choose==true)     //objective condition
		{
			if(True_Or_False(0.3, 10))                      // subjective desire :based on probability
			{
				if (Upside==true && Downside==false)
					Change_Dest_Lane=  Match_Veh_Number_on_Links(Lane_ID, Lane_ID+1, Link_ID, Cell_ID);
				else if(Upside==false && Downside==true )
					Change_Dest_Lane=  Match_Veh_Number_on_Links(Lane_ID, Lane_ID-1, Link_ID, Cell_ID);
			}
			Dest_Lane_Number=-1;   
		}
	}
	//Change_Dest_Lane!=Lane_ID means decide to change lane, otherelse return -1 means don't change.
	if (Change_Dest_Lane==Lane_ID)
		Change_Dest_Lane=-1;
	
	return Change_Dest_Lane;
}


int CVeh::Match_Veh_Number_on_Links( int Line_A, int Line_B, int Link_ID, int Cell_ID)
{
	int Veh_Sum_on_Line_A=0;
	int Veh_Sum_on_Line_B=0;
	int Cell_Sum_on_Line_A= Lane_Array[Link_ID][Line_A]->Cell_Number;
	int Cell_Sum_on_Line_B= Lane_Array[Link_ID][Line_B]->Cell_Number;

	for (int i=Cell_Sum_on_Line_A-1; i>=0; i--)
	{
		if(Lane_Array[Link_ID][Line_A]->Lane_Cell[i]->GetVehFromCell()!=NULL)
			Veh_Sum_on_Line_A++;
		if(Lane_Array[Link_ID][Line_B]->Lane_Cell[i]->GetVehFromCell()!=NULL)
			Veh_Sum_on_Line_B++;
	}

	if (Veh_Sum_on_Line_A>Veh_Sum_on_Line_B)
		return Line_B;
	else
		return Line_A;

}

void CVeh::After_Long_Time_Waiting(int Link_ID,int Lane_ID)
{
	int Next_Link_ID=-1;

	//do not consider one lane with two directions
	if(Lane_Array[Link_ID][Lane_ID]->Left_Turn==1)
		Next_Link_ID= Link_Array[Link_ID]->Next_Left_Link;	
	if (Lane_Array[Link_ID][Lane_ID]->Straight_Turn==1)
		Next_Link_ID= Link_Array[Link_ID]->Next_Straight_Link;	
	if (Lane_Array[Link_ID][Lane_ID]->Right_Turn==1)
		Next_Link_ID= Link_Array[Link_ID]->Next_Right_Link;	

	if(Change_Route(Link_ID, Next_Link_ID))
	{
		Clear_Dest_Lane_Array();
		Set_Dest_Lane(Link_ID);
	}	
}


int CVeh::Follow_Guidance_or_Not(int Link_ID)
{
	int Ran_Number= Get_Random_Number(1,100);  ///1-100 random number
	int Left_Percent;
	int Straight_Percent;
	int Right_Percent;
	int Next_Link=-1;
	int The_Info=-1;
	
	Struct_Guidance_Strategy guidance_info;
	guidance_info= Link_Array[Link_ID]->guidance->Current_Guidance_Info;
	
	for (int i=0; i<guidance_info.Guidance_Dest_Size;i++)
	{
		if (End_Link_ID==guidance_info.Guidance_Dest[i] )
		{
			The_Info= i;
			break;
		}
	}

	if(The_Info==-1)
		Next_Link=-1;
	else
	{
		Left_Percent=guidance_info.Turn_Rate[The_Info][0];
		Straight_Percent= guidance_info.Turn_Rate[The_Info][1];
		Right_Percent=guidance_info.Turn_Rate[The_Info][2];
	}

	if (The_Info!=-1)
	{
		if (1<=Ran_Number&& Ran_Number<= Left_Percent)
		{
			Next_Link= Link_Array[Link_ID]->Next_Left_Link;
		}
		if (Left_Percent<Ran_Number && Ran_Number<= Left_Percent+Straight_Percent)
		{
			Next_Link= Link_Array[Link_ID]->Next_Straight_Link;
		}
		if(Left_Percent+Straight_Percent <Ran_Number && Ran_Number<= Left_Percent+Straight_Percent+Right_Percent)
		{
			Next_Link= Link_Array[Link_ID]->Next_Right_Link;
		}
	}
	else
		Next_Link=-1;
	
	return Next_Link;
}


void CVeh::Enter_Cross(int Link_ID, int Lane_ID, int Cell_ID, int Cross_Lane_ID)
{
	int Cross_ID = Link_Array[Link_ID]->End_Object->Object_ID;
	int Next_Link_ID= Cross_Lane_Array[Cross_ID][Cross_Lane_ID]->End_Link_ID;
	int Next_Lane_ID= Cross_Lane_Array[Cross_ID][Cross_Lane_ID]->End_Lane_ID;

	if (Cross_Lane_Array[Cross_ID][Cross_Lane_ID]!=NULL)
		if (Cross_Lane_Array[Cross_ID][Cross_Lane_ID]->Cross_Lane_Cell[0]!=NULL)   //there is cells in this lane
		{
			if(Cross_Lane_Array[Cross_ID][Cross_Lane_ID]->Cross_Lane_Cell[0]->IsVehInCell()==true) //if there is a vehicle in the beginning of cross lane, then vehicle can be moved to cross lane, so wait in the end of the link lane
			{   
				Cur_Spd = 0;
			}
			else
			{
				CVeh *p=NULL;
				p = Delete_Current_Link_Cell_Veh(Link_ID, Lane_ID, Cell_ID);
				Move_Veh_in_Cross(p,Cross_ID,Cross_Lane_ID, 0);   //move to Cell_ID=0
				Veh_State = 1;
			}
		}
		else  //there should be a lane, but this lane is too short to generate a cell
		{
			Enter_Link(Link_ID, Lane_ID, Cell_ID,Next_Link_ID, Next_Lane_ID);
		}
}

bool CVeh::Leave_Net_Or_Not(int Link_ID, int Lane_ID, int Start_Cell_ID,  int End_Cell_ID)
{
	if (Link_ID==End_Link_ID
		&& Start_Cell_ID < Link_Array[Link_ID]->Dest->Located_Cell_ID
		&& Link_Array[Link_ID]->Dest->Located_Cell_ID <= End_Cell_ID )
	{
		CVeh *p;
		extern int Out_Veh_Number;
		p=Delete_Current_Link_Cell_Veh(Link_ID, Lane_ID, Start_Cell_ID);	//pay attention to recording
		p=NULL;
		Out_Veh_Number++;
		return true;
	}
	else 
		return false;
}


bool CVeh::Able_To_Enter_Cross(int Link_ID, int Lane_ID, int Cell_ID, int Supposed_End_Cell_ID)
{
	int Next_Link=Current_Route_Array[Current_Link_Index+1];  

	if(Next_Link<0)	  // last link, don't need anything about intersection
		return false;

 	Info_Light_Color_and_Time LCT;
	int Cross_Lane_ID=-1;
	int Dest;
	int Light_Color;
	
 	Dest= Dest_Lane[0];            //watch traffic light, take the first lane in the dest lane array as the object

	if(Lane_Array[Link_ID][Dest]->Phase!=NULL)
	{
		LCT = Lane_Array[Link_ID][Dest]->Phase->Get_Phase_LCT();     //watch light---phase
		Light_Color= LCT.Light_Color;
	}
	else
		Light_Color='G';

	if(Light_Color == 'G')   //Green light
	{
		if(Supposed_End_Cell_ID > Lane_Array[Link_ID][Lane_ID]->Cell_Number-1)	//if able to enter crossing
			return true;
	}
	return false;
}

bool CVeh::Have_Space_Next_Link(int Front_Cross_ID, int Front_Cross_Lane_ID)
{
	if (Yellow_Intersection==false)     //if donot use yellow intersection, vehs can enter the intersection directly.
		return true;

	int Cell_Number_To_Last_Veh=-1;
	int Next_Link_ID=-1;
	int Next_Lane_ID=-1;
	bool flag_end=false;
	bool flag_whole=false;
	CVeh * pVeh=NULL;
	extern int Entering_Boundry;
	extern int Checking_Region;

	Next_Link_ID=Cross_Lane_Array[Front_Cross_ID][Front_Cross_Lane_ID]->End_Link_ID;
	Next_Lane_ID=Cross_Lane_Array[Front_Cross_ID][Front_Cross_Lane_ID]->End_Lane_ID;

// 	int Total_Number= Lane_Array[Next_Link_ID][Next_Lane_ID]->Cell_Number;
// 	pVeh=Get_Last_Veh(Next_Link_ID, Next_Lane_ID);
// 	Cell_Number_To_Last_Veh=Get_Cell_Number_To_Last_Veh(Next_Link_ID, Next_Lane_ID, pVeh);

	int Veh_Number_In_Checking_Region = Get_Veh_Number_In_Checking_Region(Next_Link_ID, Next_Lane_ID, Checking_Region);
	if (Veh_Number_In_Checking_Region < Entering_Boundry)
		return true;
	else
		return false;


// 	if (Cell_Number_To_Last_Veh>=ALLOW_ENTER_LINK)
// 		return true;
// 	else
// 		if (pVeh->Cur_Spd>=0.999)
// 			return true;
// 		else
// 			return false;
}


int CVeh::Get_Veh_Number_In_Checking_Region(int Link_ID, int Lane_ID, int Region)
{
	int Veh_Number_In_Checking_Region=0;

	for (int i=0; i<Region; i++)
	{
		if (true==Lane_Array[Link_ID][Lane_ID]->Lane_Cell[i]->IsVehInCell())
			Veh_Number_In_Checking_Region++;
	}

	return Veh_Number_In_Checking_Region;
}

//Get cell number from the end of the lane to last vehicle,
// int CVeh::Get_Cell_Number_To_Last_Veh(int Link_ID, int Lane_ID, CVeh* pLastVeh)
// {
// 	int Total_Number= Lane_Array[Link_ID][Lane_ID]->Cell_Number;
// 	if (pLastVeh==NULL)
// 		return Total_Number;
// 	else
// 	{
// 		int Sum=0;
// 		Sum=pLastVeh->Cell_Loc;
// 		return Sum;
// 	}
// }
// 
// CVeh* CVeh::Get_Last_Veh(int Link_ID, int Lane_ID)
// {
// 	CVeh* pVeh=NULL;
// 	int Total_Number= Lane_Array[Link_ID][Lane_ID]->Cell_Number;
// 
// 	for (int i=0; i<Total_Number; i++)
// 		if (true==Lane_Array[Link_ID][Lane_ID]->Lane_Cell[i]->IsVehInCell())
// 			return Lane_Array[Link_ID][Lane_ID]->Lane_Cell[i]->GetVehFromCell();
// 
// 	return pVeh;
// }


// int CVeh::Get_Veh_Number_On_Cross_Lane(int Cross_ID, int Cross_Lane_ID)
// {
// 	int Sum=0;
// 	int Total_Number= Cross_Lane_Array[Cross_ID][Cross_Lane_ID]->Cell_Number;
// 	for (int i=0; i<Total_Number; i++ )
// 	{
// 		if(true==Cross_Lane_Array[Cross_ID][Cross_Lane_ID]->Cross_Lane_Cell[i]->IsVehInCell())
// 			Sum++;
// 	}
// 	return Sum;
// }


void CVeh::Veh_Run_on_Link(int Start_Link_ID, int Start_Lane_ID,int Start_Cell_ID, int End_Link_ID, int End_Lane_ID, int End_Cell_ID, double New_Speed)
{
	CVeh *p=NULL;

	p = Delete_Current_Link_Cell_Veh(Start_Link_ID, Start_Lane_ID, Start_Cell_ID);
	Move_Veh_on_Link(p, End_Link_ID, End_Lane_ID, End_Cell_ID);		
	Collect_Trajectory(Start_Link_ID,Start_Lane_ID,End_Link_ID,End_Lane_ID, End_Cell_ID);
	Update_Loc_And_Spd(p, End_Link_ID, End_Lane_ID, End_Cell_ID, New_Speed);
}

void CVeh::Update_Loc_And_Spd(CVeh *p, int New_Link_ID, int New_Lane_ID, int New_Cell_ID, double New_Speed)
{
	p->Link_Loc= New_Link_ID;
	p->Lane_Loc= New_Lane_ID;
	
	p->Last_Cell_Loc= p->Cell_Loc;
	p->Last_Spd= p->Cur_Spd;

	p->Cell_Loc= New_Cell_ID;
	p->Cur_Spd= New_Speed;
}

//used in Intersection
void CVeh::Update_Loc_And_Spd(CVeh *p, double New_Speed)
{
	p->Last_Spd= p->Cur_Spd;
	p->Cur_Spd= New_Speed;
}



void CVeh::Veh_On_Link(int Cross_id,int Link_ID, int Lane_ID, int Cell_ID)
{
	if(Link_ID<0 ||Link_ID>=MAX_LINK_NUMBER)
	{
		AfxMessageBox("Error---CVeh::Veh_On_Link()");
		return;
	}

	extern bool Switch_Show_Guidance;
	if(Switch_Show_Guidance==true)
		if(Link_Array[Link_ID]->guidance!=NULL)
			if(Cell_ID>=GUIDANCE_LOCATION && Receive_Guidance==-1) 
				Receive_Guidance=Response_to_Guidance(Link_ID);

	bool Flag=Dest_Lane_or_Not(Lane_ID,Link_Array[Link_ID]->Lane_Number);

	if(Flag)
		Drive_on_Dest_Lane(Link_ID, Lane_ID, Cell_ID);
	else
		Drive_not_on_Dest_Lane(Link_ID, Lane_ID, Cell_ID);
}


CVeh *CVeh::Get_Front_Veh_on_Link(int Link_ID, int Lane_ID, int Cell_ID, int Last_Cell_ID)
{
	int i;
	for(i=Cell_ID+1;i<=Last_Cell_ID;i++)   
		  if(Lane_Array[Link_ID][Lane_ID]->Lane_Cell[i]->IsVehInCell()==true)
			   return Lane_Array[Link_ID][Lane_ID]->Lane_Cell[i]->GetVehFromCell();

	return NULL;   
}

//if there is no front vehicle, return -1
int CVeh::Distance_Between_Front_Veh(int Link_ID, int Lane_ID, int Cell_ID)
{
	int i,j;
	j=0;
	for(i=Cell_ID+1; i<Lane_Array[Link_ID][Lane_ID]->Cell_Number; i++)
	{
		if(Lane_Array[Link_ID][Lane_ID]->Lane_Cell[i]->IsVehInCell()==false)
			j++;
		else
			break;
	}
	return j;
}


//implement Delete_Current_Cell_Veh() first
int CVeh::Move_Veh_on_Link(CVeh *p,    int Move_Link_ID,   int Move_Lane_ID,  int Move_Cell_ID) 
{
	if(Move_Lane_ID <0 || Move_Lane_ID > Link_Array[Move_Link_ID]->Lane_Number)
	{
		AfxMessageBox("Fault, Lane_ID"); 
		return 0;
	}
	if(Lane_Array[Move_Link_ID][Move_Lane_ID]->Lane_Cell[Move_Cell_ID]->IsVehInCell()==true)
	{
 		AfxMessageBox("Destination place is occupied");
		return 0;
	}

	Lane_Array[Move_Link_ID][Move_Lane_ID]->Lane_Cell[Move_Cell_ID]->PutVehInCell(p);

/////////////////////////////////////////////////////////////////////////////////
///////////cancel the comments, when one wants to collect the trajectory data.
/*if(simu_time>1000 && simu_time<1200)*/
	if (Move_Link_ID==45 &&Move_Lane_ID==1 &&p!=NULL)
	{
		//*************************
		extern ErrorLog* err;

		//veh id
		err->LogIntData("trajectory.csv", p->Veh_ID);  
		err->LogStrData("trajectory.csv", ",");

		//simu time
		err->LogIntData("trajectory.csv", simu_time);
		err->LogStrData("trajectory.csv", ",");

		//cell id
		err->LogIntData("trajectory.csv", Move_Cell_ID);
				
		//
		err->LogStrData("trajectory.csv","\n");

		//*************************
	}

	return 1;
}


int CVeh::Get_Lane_Number_to_Dest_Lane(int Current_Lane_ID)
{
	int Lane_Number;
	int Nearest_Dest_Lane_ID;
	Nearest_Dest_Lane_ID= Get_Nearest_Dest_Lane_ID(Current_Lane_ID);
	
	Lane_Number= abs(Nearest_Dest_Lane_ID- Current_Lane_ID) -1;
		
	return Lane_Number;
}


//get the nearest lane in destination lane array
int CVeh::Get_Nearest_Dest_Lane_ID( int Current_Lane_ID)
{
	 int Nearest_Dest_Lane_ID = Dest_Lane[0];
	 for(int i=1; i<MAX_LANE_NUMBER;i++)
	 {
	 	if( Dest_Lane[i]!=-1)
	 		if( abs(Dest_Lane[i]-Current_Lane_ID) <= abs(Nearest_Dest_Lane_ID-Current_Lane_ID) )
	 			Nearest_Dest_Lane_ID = Dest_Lane[i];
	}
	 return Nearest_Dest_Lane_ID;	
}


bool CVeh::Decide_Change_Lane(int Cell_ID)
{
	if (Cell_ID*Meter_Per_Cell<=VEHICLE_LENGTH)    //can't change lanes at the beginning of a lane; o/w, errors may occur
		return false;

	bool flag=True_Or_False(PROB_OF_CHANGE_LANE, 100);
	return flag;
}

int CVeh::Get_Dest_Lane(int Lane_ID)
{
	int Change_Dest_Lane=-1;
	int Nearest_Dest_Lane_ID = Get_Nearest_Dest_Lane_ID(Lane_ID);
	if (Nearest_Dest_Lane_ID > Lane_ID)
		Change_Dest_Lane= Lane_ID+1;
	else
		Change_Dest_Lane= Lane_ID-1;

	return Change_Dest_Lane;
}


void CVeh::Veh_in_Cross(int Cross_ID,  int Cross_Lane_ID,  int Cell_ID)
{
	CCell *Current_Cell= Cross_Lane_Array[Cross_ID][Cross_Lane_ID]->Cross_Lane_Cell[Cell_ID];
	CCell *Prob_Next_Cell= Cross_Lane_Array[Cross_ID][Cross_Lane_ID]->Cross_Lane_Cell[Cell_ID+SPEED_IN_CROSS];
	int Cross_Lane_Turn_Direction=Cross_Lane_Array[Cross_ID][Cross_Lane_ID]->Turn_Direction ;
	int Cell_Amount= Cross_Lane_Array[Cross_ID][Cross_Lane_ID]->Cell_Number;	

	if (Prob_Next_Cell==NULL)
	{
//
// for debugging, let a vehicle be stuck in one lane
// 		if(Cross_Lane_Turn_Direction==1&&Cross_Lane_Array[Cross_ID][Cross_Lane_ID]->End_Link_ID==6)  
// 		{
// 			Cur_Spd=0;
// 			return;
// 		}
		Enter_Link(Cross_ID, Cell_ID,Cross_Lane_ID);
		Detected_Flag=-1;  
		return;	
	}

	// there is vehicles in front
	if (Prob_Next_Cell->IsVehInCell()==true)
	{
		Cur_Spd=0;
		return ;
	}

	//no conflict in right turn direction
	if (Cross_Lane_Turn_Direction==2)
	{
		Veh_Run_in_Cross(Cross_ID,Cross_Lane_ID, Cell_ID, Cross_Lane_ID, int(Cell_ID+SPEED_IN_CROSS));	
		return;
	}

	//through-through and through-left turn
	bool Left_Enter=Able_To_Pass_Left_Through_Conflict(Cross_Lane_Turn_Direction, Cross_ID, Current_Cell, Prob_Next_Cell);
 	bool Straight_Enter=Able_To_Pass_Through_Through_Conflict(Cross_ID, Prob_Next_Cell);
	
	if (Left_Enter==true&&Straight_Enter==true)
		Veh_Run_in_Cross(Cross_ID,Cross_Lane_ID, Cell_ID, Cross_Lane_ID, int(Cell_ID+SPEED_IN_CROSS));		
	else
		Cur_Spd=0;
}



bool CVeh::Able_To_Pass_Left_Through_Conflict(int Cross_Lane_Turn_Direction, int Cross_ID, CCell *Current_Cell, CCell *Prob_Next_Cell)
{
	if (Enter_Conflict_Area_or_Not(Cross_Lane_Turn_Direction, Cross_ID, Prob_Next_Cell)==false)  
		return true;

	bool Enter_Conflict_Area_Flag=false;
	int Conflict_Area_Flag=0;
	extern bool Switch_Thread;
	int Conflict_Area_ID;
	while (Switch_Thread)
	{
		//check different Conflict_Area_ID each time based on Conflict_Area_Flog
		Conflict_Area_ID= Get_Entering_Conflict_Area_ID(Cross_Lane_Turn_Direction, Conflict_Area_Flag, Cross_ID, Prob_Next_Cell); //Get the conflict area id that is entering

		if (Conflict_Area_ID==-1)       //already checked all conflict area, then break the loop
			break;

		if(Conflict_Occupied_or_Not(Cross_Lane_Turn_Direction, Cross_ID, Conflict_Area_ID)==true)	    //there is a vehicle inside the conflict area
		{			
			Enter_Conflict_Area_Flag=false;                   //vehicle stops
			break;                                            //if condition could be satisfied, then break
		}
		else  //no vehicle in conflicts area
		{
			if (false==Priority_or_Not(Cross_Lane_Turn_Direction,Cross_ID,  Current_Cell, Conflict_Area_ID)) //there is a waiting vehicle which has a priority to enter conflict
			{
				Enter_Conflict_Area_Flag=false;
				break;
			} 
			else
			{
				Enter_Conflict_Area_Flag=true;
			}
		}	
		Conflict_Area_Flag++;
	}
	
	return Enter_Conflict_Area_Flag;
}

bool CVeh::Able_To_Pass_Through_Through_Conflict(int Cross_ID, CCell *Next_Cell)
{
	 if(Next_Cell->Straight_Conflict_Cell==NULL)    //means it has not the straight conflict problem
		 return true;

	 if(Next_Cell->Straight_Conflict_Cell->IsVehInCell()==true)
		 return false;
	 else
		return true;   //set priority between two straightly running vehs here if want 

}

void CVeh::Enter_Link(int Cross_id,	  int Cell_ID,  int Cross_Lane_ID)
{
	CVeh *p=NULL;
	int next_link;
	int next_line;

	next_link = Cross_Lane_Array[Cross_id][Cross_Lane_ID]->End_Link_ID ; 
	next_line = Cross_Lane_Array[Cross_id][Cross_Lane_ID]->End_Lane_ID ;
	if (Lane_Array[next_link][next_line]->Lane_Cell[0]->IsVehInCell()==false)
	{
		//move vehicle to the j th cell in the lane of next link.
		p = Delete_Current_Cross_Cell_Veh(Cross_id,Cross_Lane_ID, Cell_ID);
		Move_Veh_on_Link(p,next_link, next_line, 0);
		
		Clear_Dest_Lane_Array();
		Set_Dest_Lane();
		Set_On_Link_Max_Speed(next_link);

		Veh_State=0;  //enter link, then change the state into 0
		Current_Link_Index++;
		Receive_Guidance=-1;
		Cur_Spd=((double)CELL_LENGTH_IN_CROSS)/Pixel_Per_Cell;
	} 
	else
	{
		//there is a vehicle stuck in the cell of next link, so this vehicle have to stop and wait
		Cur_Spd = 0;
	}

	p=NULL;
}


void CVeh::Enter_Link(int Current_Link,  int Current_Lane,   int Current_Cell,  int Next_Link,	  int Next_Lane)
{
	CVeh *p=NULL;
	int Next_Cell;
	Next_Cell= 0;

	if (Lane_Array[Next_Link][Next_Lane]->Lane_Cell[0]->IsVehInCell()==false)
	{
		Veh_Run_on_Link(Current_Link, Current_Lane, Current_Cell, Next_Link, Next_Lane, Next_Cell, 1);

		Clear_Dest_Lane_Array();
		Set_Dest_Lane();
		Set_On_Link_Max_Speed(Next_Link);

		Veh_State=0; 
		Current_Link_Index++;
		Receive_Guidance=-1;
		Cur_Spd=((double)CELL_LENGTH_IN_CROSS)/Pixel_Per_Cell;    //take the speed in intersection as the initial speed on the link.  speed in intersection is 1 cell of intersection
	} 
	else
	{
		Cur_Spd = 0;
	}
	
	p=NULL;
}


void CVeh:: Veh_Run_in_Cross(int Cross_ID,	int Start_Cross_Lane_ID,int Start_Cell_ID,int End_Cross_Lane_ID, int End_Cell_ID)
{
	CVeh *p=NULL;
	p = Delete_Current_Cross_Cell_Veh(Cross_ID, Start_Cross_Lane_ID, Start_Cell_ID);
	Move_Veh_in_Cross(p,Cross_ID,End_Cross_Lane_ID, End_Cell_ID);		
}


CVeh * CVeh::Delete_Current_Cross_Cell_Veh(int Cross_id,int Cross_Lane_ID,int Cell_ID)
{
	CVeh *p = NULL;
	p = Cross_Lane_Array[Cross_id][Cross_Lane_ID]->Cross_Lane_Cell[Cell_ID]->GetVehFromCell();
	if(p == NULL)
	{
		AfxMessageBox("Fault, the one intended to delete does not exist.");
		return 0;
	}
	Cross_Lane_Array[Cross_id][Cross_Lane_ID]->Cross_Lane_Cell[Cell_ID]->PutVehInCell(NULL);

	return p;	
}


int CVeh::Move_Veh_in_Cross(CVeh *p, int Cross_id,  int Cross_Lane_ID,  int Move_Cell_ID) 
{
	if(Cross_Lane_Array[Cross_id][Cross_Lane_ID]->Cross_Lane_Cell[Move_Cell_ID]->IsVehInCell()==true)
	{
		AfxMessageBox("Fault, the cell that the vehicle wants to move into is occupied");
		return 0;
	}

	Update_Loc_And_Spd(p, SPEED_IN_CROSS);
	Cross_Lane_Array[Cross_id][Cross_Lane_ID]->Cross_Lane_Cell[Move_Cell_ID]->PutVehInCell(p);
	return 1;
}			 


int CVeh::Response_to_Guidance(int Link_ID)			 
{
	int Follow_or_Not=0; 
	int Guidance_Link; 

	Guidance_Link= Follow_Guidance_or_Not(Link_ID);
	if (Guidance_Link >=0 )  
	{
		if (Change_Route(Link_ID, Guidance_Link))
		{
			Clear_Dest_Lane_Array();
			Set_Dest_Lane(Link_ID);
			Follow_or_Not=1;
		} 
	}
	else
		Follow_or_Not=0;

	return Follow_or_Not;
}




bool CVeh::Last_Route_Link(Struct_Shortest_Path *spi)
{
	int Link_ID;
	for(int i=0; i<MAX_ROUTE_LENGTH; i++)    
	{  
		Link_ID=spi->Shortest_Path[i];
		if (this->End_Link_ID==Link_ID)
			return true;
	}
	return false;
}


bool CVeh::Enter_Conflict_Area_or_Not(int Cross_Lane_Type, int Cross_ID, CCell * Next_Cell)
{
	switch (Cross_Lane_Type)
	{
	case 0:
		//if next cell is in a conflict area.
		if (Next_Cell->Conflict_Area_ID_Amount!=0)
			return true;
		break;

	case 1:
		for (int i=0; i<MAX_CONFLICT_AREA_NUMBER;i++ )
			if(Conflict_Area_Array[Cross_ID][i]!=NULL)
				if (Conflict_Area_Array[Cross_ID][i]->Main_Cell==Next_Cell)
					return true;
		break;

	case 2:
		AfxMessageBox("Fault, there is a right turning vehicle in conflict area"); 
		break;
	}
	
	return false;
}

bool CVeh::Conflict_Occupied_or_Not(int Cross_Lane_Type,int Cross_ID, int Conflict_Area_ID)
{
	switch (Cross_Lane_Type)
	{
	case 0:
		if (Conflict_Area_Array[Cross_ID][Conflict_Area_ID]->Main_Cell->IsVehInCell()==true)
			return true;		
		break;

	case 1:
		for (int i=0;i<Conflict_Area_Array[Cross_ID][Conflict_Area_ID]->Conflict_Cell_Number; i++)
			if(Conflict_Area_Array[Cross_ID][Conflict_Area_ID]->Conflict_Cell_Array[i]->IsVehInCell()==true)
				return true;
		break;

	case 2:
		AfxMessageBox("Fault, there is a right turning vehicle in conflict area");
		break;
	}
	return false; 
}




bool CVeh::Priority_or_Not(int Cross_Lane_Type,int Cross_ID, CCell* Current_Cell, int Conflict_Area_ID)
{
	//if veh has already been conflict_area, it has the priority
	for (int i=0; i<Current_Cell->Conflict_Area_ID_Amount;i++)
		if (Current_Cell->Conflict_Area_ID_Array[i]==Conflict_Area_ID)
			return true;

	int i;
	CCell *Other_Waiting_Cell=NULL;
	CCell *Other_Conflict_Cell=NULL;
	int Cell_Amount= Cross_Lane_Array[Cross_ID][Current_Cell->Cross_Lane_ID]->Cell_Number;

	switch (Cross_Lane_Type)
	{
	case 0:    //is focusing on left turning lane
		Other_Conflict_Cell= Conflict_Area_Array[Cross_ID][Conflict_Area_ID]->Main_Cell;
		if (Other_Conflict_Cell->Cell_ID- SPEED_IN_CROSS >=Cell_Amount)  //9+1>=10
			Other_Waiting_Cell= NULL;
		else
			Other_Waiting_Cell= Cross_Lane_Array[Cross_ID][Other_Conflict_Cell->Cross_Lane_ID]->Cross_Lane_Cell[Other_Conflict_Cell->Cell_ID- SPEED_IN_CROSS];
		
		if (Other_Waiting_Cell!=NULL)
			if (Other_Waiting_Cell->IsVehInCell()==true)  //if there is a vehicle in Other_Waiting_Cell
				if(Other_Waiting_Cell->GetVehFromCell()->Cross_Move_Check_Time==simu_time-1)    //this time, vehicle in through traffic is still not processed(has been processed and stop in front of Conflict_Area, which means can't enter conflict this time)

					//check if current left turning vehicle has priority: in platoon, it would have priority, return true. else, left turning vehicle never has priority.
					 //Current vehicle (left turning vehicle) in a platoon
					if (true==Platoon_or_Not(Cross_ID, Current_Cell))  
						return true;
					else
						return false;               //non-through traffic || not in platoon, no priority

		return true;		 //if no Other_Waiting_Cell, then in conflict area there must be no other vehicle has priority
		break;

	case 1:     //is focusing on through traffic lane
		//consider the cell in Conflict_Cell_Array
		for (i=0; i<Conflict_Area_Array[Cross_ID][Conflict_Area_ID]->Conflict_Cell_Number; i++)
		{
			Other_Conflict_Cell= Conflict_Area_Array[Cross_ID][Conflict_Area_ID]->Conflict_Cell_Array[i];
			Other_Waiting_Cell= Cross_Lane_Array[Cross_ID][Other_Conflict_Cell->Cross_Lane_ID]->Cross_Lane_Cell[Other_Conflict_Cell->Cell_ID- SPEED_IN_CROSS];
			if (Other_Waiting_Cell!=NULL)
				if (Other_Waiting_Cell->IsVehInCell()==true)
					if(Other_Waiting_Cell->GetVehFromCell()->Cross_Move_Check_Time==simu_time-1)
						//as if there is a platoon in Other_Waithing_Cell, then the through traffic vehicle doesn't have priority
						//Other_Waiting_Cell platoon
						if (true==Platoon_or_Not(Cross_ID, Other_Waiting_Cell))  
							return false;   	
		}
		
		return true; 
		break;

	case 2:
		AfxMessageBox("Fault, there is a right turning vehicle in conflict area");
		break;
	}

	return false;
}


bool CVeh::Platoon_or_Not(int Cross_ID,CCell *Current_Cell)
{
	int Cell_ID= Current_Cell->Cell_ID;
	int Cross_Lane_ID= Current_Cell->Cross_Lane_ID;
	int Cell_Amount= Cross_Lane_Array[Cross_ID][Cross_Lane_ID]->Cell_Number;

	if (Cell_ID==Cell_Amount-2&&Cell_ID==Cell_Amount-1) 
	{
		return true;
	}

	//the second type platoon, there is a vehicle in the front cell of current cell, no moving
	if (Cross_Lane_Array[Cross_ID][Cross_Lane_ID]->Cross_Lane_Cell[Cell_ID+SPEED_IN_CROSS]->IsVehInCell()==true
		&&
		Cross_Lane_Array[Cross_ID][Cross_Lane_ID]->Cross_Lane_Cell[Cell_ID+SPEED_IN_CROSS]->GetVehFromCell()->Cur_Spd==0)
		return true;		

	//the first type platoon, there is a vehicle in the front cell of current cell, moving one step
	if(Cross_Lane_Array[Cross_ID][Cross_Lane_ID]->Cross_Lane_Cell[Cell_ID+2*SPEED_IN_CROSS]->IsVehInCell()==true
		&&
		Cross_Lane_Array[Cross_ID][Cross_Lane_ID]->Cross_Lane_Cell[Cell_ID+2*SPEED_IN_CROSS]->GetVehFromCell()->Cur_Spd >0 )
		return true;  

	return false;

}


int CVeh::Get_Entering_Conflict_Area_ID(int Cross_Lane_Type,int Conflict_Area_Flag,int Cross_ID, CCell *Next_Cell)
{
	int Entering_Conflict_Area_ID=-1;
	switch (Cross_Lane_Type)
	{
	case 0:
		Entering_Conflict_Area_ID=Next_Cell->Conflict_Area_ID_Array[Conflict_Area_Flag];   //Cell_ID=0 has been processed in Veh_in_Cross, then Cell_ID-SPEED_IN_CROSS can't be smaller than 0
		break;

	case 1:
		Entering_Conflict_Area_ID=-1;
		if (Conflict_Area_Flag==0)
			for (int i=0; i<MAX_CONFLICT_AREA_NUMBER;i++ )
				if(Conflict_Area_Array[Cross_ID][i]!=NULL)
					if (Conflict_Area_Array[Cross_ID][i]->Main_Cell==Next_Cell)
					{
						Entering_Conflict_Area_ID= Conflict_Area_Array[Cross_ID][i]->Conflict_ID;
						break;
					}
		break;

	case 2:
		AfxMessageBox("Fault, there is a right turning vehicle in conflict area");  

		break;
	}

	return Entering_Conflict_Area_ID;
}

int CVeh::Choose_Cross_Lane(int Cross_ID, int Current_Link_ID, int Current_Lane_ID)
{
	//first, turning direction
	//through
	//right turning, left turning
		//start lane id
		//end lane id
	int i;
	int Turn_Direction;
	int Cross_Line_End_ID;
	int Cross_Lane_ID=-1;
	int Next_Link_ID=-1;
	
	Next_Link_ID=Current_Route_Array[Current_Link_Index+1];  
	Turn_Direction= Get_Turn_Direction(Current_Link_ID, Next_Link_ID);

	if (Turn_Direction==1)
	{
		for (i=0; i<MAX_CROSS_LANE_NUMBER; i++)
			if (Cross_Lane_Array[Cross_ID][i]!=NULL)
				if (Cross_Lane_Array[Cross_ID][i]->Start_Link_ID== Current_Link_ID	&&Cross_Lane_Array[Cross_ID][i]->Start_Lane_ID== Current_Lane_ID )  //this lane is connected with the lane in crossing
					if (Cross_Lane_Array[Cross_ID][i]->End_Link_ID== Next_Link_ID)
					{
						Cross_Lane_ID= Cross_Lane_Array[Cross_ID][i]->Cross_Lane_ID;  
						break;
					}
	}
	else
	{
		if (Turn_Direction==3)   //U turn
		{
			Cross_Line_End_ID= Link_Array[Next_Link_ID]->Lane_Number-1;     //the far right lane
		}
		else 
		{
			//choose a loading lane randomly, when Turn_Direction ==0 or 2
			Cross_Line_End_ID= Get_Random_Number(0, Link_Array[Next_Link_ID]->Lane_Number-1);
		}

		for (i=0; i<MAX_CROSS_LANE_NUMBER; i++)
			if (Cross_Lane_Array[Cross_ID][i]!=NULL) 
				if (Cross_Lane_Array[Cross_ID][i]->Start_Link_ID== Current_Link_ID&&Cross_Lane_Array[Cross_ID][i]->Start_Lane_ID== Current_Lane_ID )
					if (Cross_Lane_Array[Cross_ID][i]->End_Link_ID== Next_Link_ID&&Cross_Lane_Array[Cross_ID][i]->End_Lane_ID== Cross_Line_End_ID)
					{
						Cross_Lane_ID= Cross_Lane_Array[Cross_ID][i]->Cross_Lane_ID;
						break;
					}	
	}

	return Cross_Lane_ID;
}

int CVeh::Get_Turn_Direction(int Current_Link_ID, int Next_Link_ID)
{
	int Turn_Direction=-1;

	if(Link_Array[Current_Link_ID]->Next_Left_Link==Next_Link_ID)
		Turn_Direction=0;
	if(Link_Array[Current_Link_ID]->Next_Straight_Link== Next_Link_ID )
		Turn_Direction=1;
	if(Link_Array[Current_Link_ID]->Next_Right_Link== Next_Link_ID)
		Turn_Direction=2;
	if(Link_Array[Current_Link_ID]->Next_UTurn_Link== Next_Link_ID)
		Turn_Direction=3;
	
	return Turn_Direction;
}


//////////////////////////////////////////////
Struct_Shortest_Path* CVeh::ShortestPath_with_Guidance(int Next_Link)
{
	//choose a new shortest path
	Struct_Shortest_Path *spi= Get_Shortest_Path(Next_Link,End_Link_ID);

	int Link_ID=-1;
	int End_Of_Route=100000000;
	bool flag=false;    //the existence of flag is to avoid: in Shortest_Pant[], 115,-1,-1,-1,-1,-1,109
	for(int i=0; i< MAX_ROUTE_LENGTH;i++)
	{
		Link_ID= spi->Shortest_Path[i];
		if (Link_ID!=-1)
			if (flag==false && Link_Array[Link_ID]->End_Object->Object_Type=='M')    //only when Object_Type=='M', the destination will be changed.
			{ 
				End_Of_Route=i;
				this->End_Link_ID=Link_ID;        //Change the destination of this veh
				flag=true;
			}

		if (i>End_Of_Route)
			spi->Shortest_Path[i]=-1;
	}
	return spi;
}


void CVeh::Collect_Trajectory(int Last_Link_ID, int Last_Lane_ID, int New_Link_ID, int New_Lane_ID, int Cell_ID)
{
// 	Loc_And_Time* LAT= new Loc_And_Time();
// 	if (Last_Link_ID!=New_Link_ID || Last_Lane_ID!=New_Lane_ID)
// 	{
// 		this->Trajectory.RemoveAll();
// 		this->Freshman=true;
// 	}
// 	LAT->Location=Cell_ID;
// 	LAT->Time=simu_time;
// 	Trajectory.Add(LAT);
}