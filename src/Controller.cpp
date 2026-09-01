// Controller.cpp: implementation of the CController class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "traffic.h"
#include "Controller.h"
#include "Global.h"
#include "ErrorLog.h"
#include "TrafficMessageBox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CController::CController(int cross_id, int phase_amount)
{
	int i;
	int j;
	Cross_ID= cross_id;
	Phase_Number= phase_amount;
	Last_Refresh_Time=0;
	Cycle_Number =0;
	Last_Update_Time=-1;

	for (i=0; i<MAX_PHASE_NUMBER; i++)
		Phase_Array[i]=NULL;

	for (i=0; i<MAX_ENTER_LINK;i++)
		for (j=0;j<MAX_DETECTOR_NUMBER;j++)
			Detector_Under_Control[i][j]=NULL;
		
	//initial Current_Control_Info
	Current_Control_Info.Cross_ID=-1;
	Current_Control_Info.Cycle_Time=-1;
	for (i=0;i<MAX_PHASE_NUMBER;i++)
	{
		Current_Control_Info.Phase_Info_Array[i].Green_Percent=-1;
		Current_Control_Info.Phase_Info_Array[i].Green_Start_Time_Percent=-1;
		Current_Control_Info.Phase_Info_Array[i].Phase_ID=-1;
	}

	//initial Init_Control_Info
	Init_Control_Info.Cross_ID=-1;
	Init_Control_Info.Cycle_Time=INIT_CONTROL_CYCLE;
	for (i=0;i< MAX_PHASE_NUMBER; i++)
	{
		Init_Control_Info.Phase_Info_Array[i].Green_Percent=ALL_GREEN;
		Init_Control_Info.Phase_Info_Array[i].Green_Start_Time_Percent=START_TIME_ALL_GREEN;
		Init_Control_Info.Phase_Info_Array[i].Phase_ID=i;
	}
	
	memset((char*)&Received_Control_Info, -1, sizeof(Received_Control_Info));
	memset((char*)&Received_Control_Info_0, -1, sizeof(Received_Control_Info_0));
//	Received_Control_Info_Sum=0;
	Clear_Tran_MBC();
}

CController::~CController()
{

}


int CController::Add_Phase(CPhase *Phase)
{
	int i;
	for (i=0;i<MAX_PHASE_NUMBER;i++)
	{
		if (Phase_Array[i]==NULL)
		{
			Phase_Array[i]=Phase;
			return 1; 
		}
	}
	return -1;  
}


int CController::Set_Phases_Cycle_Time(int Phase_ID, int New_Cycle_Time)
{
	Phase_Array[Phase_ID]->Set_Cycle_Time(New_Cycle_Time);
	Phase_Array[Phase_ID]->Set_Phase_Last_Refresh_Time();
	return 1;
}


int CController::Set_Phases_Green_Start_Time(int Phase_ID, int New_Green_Start_Time_Percent)
{
	if (Phase_Array[Phase_ID]!=NULL)
	{
		Phase_Array[Phase_ID]->Set_Green_Start_Time(New_Green_Start_Time_Percent);
		return 1;
	}
	else
		return -1;
}


int CController::Set_Phases_Green_Percent(int Phase_ID, int New_Green_Percent)
{
	if (Phase_Array[Phase_ID]!=NULL)
	{
		Phase_Array[Phase_ID]->Set_Green_Percent(New_Green_Percent);
		return 1;
	}
	else
		return -1;
}



int CController::Link_in_Phase_or_Not(int Phase_ID, int Link_ID)
{
	int j;

	for (j=0; j<LINK_IN_ONE_PHASE; j++)
	{
		if (Phase_Array[Phase_ID]->Connect_Link_Array[j]==Link_ID)
		{
			return 1;
		}		
	}
	return -1;
}


CPhase * CController::Get_Phase(int Phase_ID)
{
	return Phase_Array[Phase_ID];
}

void CController::Generate_Point_Control_Strategy()
{
	//set Current_Control_Info directly
// 	Current_Control_Info.Phase_Info_Array[0].Green_Percent=30;
// 	Current_Control_Info.Phase_Info_Array[0].Green_Start_Time_Percent=0;
// 	Current_Control_Info.Phase_Info_Array[0].Phase_ID=0;
// 
// 	Current_Control_Info.Phase_Info_Array[1].Green_Percent=20;
// 	Current_Control_Info.Phase_Info_Array[1].Green_Start_Time_Percent=30;
// 	Current_Control_Info.Phase_Info_Array[1].Phase_ID=1;
// 
// 	Current_Control_Info.Phase_Info_Array[2].Green_Percent=100;
// 	Current_Control_Info.Phase_Info_Array[2].Green_Start_Time_Percent=0;
// 	Current_Control_Info.Phase_Info_Array[2].Phase_ID=2;
// 
// 	Current_Control_Info.Phase_Info_Array[3].Green_Percent=30;
// 	Current_Control_Info.Phase_Info_Array[3].Green_Start_Time_Percent=50;
// 	Current_Control_Info.Phase_Info_Array[3].Phase_ID=3;
// 
// 	Current_Control_Info.Phase_Info_Array[4].Green_Percent=20;
// 	Current_Control_Info.Phase_Info_Array[4].Green_Start_Time_Percent=80;
// 	Current_Control_Info.Phase_Info_Array[4].Phase_ID=4;
// 
// 	Current_Control_Info.Phase_Info_Array[5].Green_Percent=100;
// 	Current_Control_Info.Phase_Info_Array[5].Green_Start_Time_Percent=0;
// 	Current_Control_Info.Phase_Info_Array[5].Phase_ID=5;
}


void CController::Update_Control_Info()
{
	extern char Current_Control_Type;

	switch (Current_Control_Type)
	{
	case 'P':    //point control
		if (simu_time==Start_Time)  //first time
		{
			Init_Control_Info.Cross_ID=Cross_ID;
			Current_Control_Info=Init_Control_Info;
		}
		else
		{
			Generate_Point_Control_Strategy();  
		}
		break;
		
	case 'R':    //regional control update
		Generate_Point_Control_Strategy();
		if (Received_Control_Info.Cycle_Time!=-1)  //means new data has been ready, update
		{
			if (Cross_ID != Received_Control_Info.Cross_ID)
				AfxMessageBox("Data doesn't match the controller ID");
			Current_Control_Info.Cycle_Time= Received_Control_Info.Cycle_Time;
			Received_Control_Info= Received_Control_Info_0;
			memset((char*)&Received_Control_Info_0, -1, sizeof(Received_Control_Info_0));
		}
		break;
	}

	Cycle_Number++;
	Last_Refresh_Time= simu_time;
}



bool CController::Refresh_or_Not()
{
	//first time of region, point control || reach cycle time 
	if (simu_time==Start_Time || (simu_time - Last_Refresh_Time == Current_Control_Info.Cycle_Time) ) 
		return true;
	else
		return false;
}


void CController::Update_Phase()
{
	int i;
	int Phase_ID;
	for (i=0; i<MAX_PHASE_NUMBER; i++)
	{
		//match or not
		if (Current_Control_Info.Phase_Info_Array[i].Phase_ID!=-1 && Phase_Array[i]==NULL
			||Current_Control_Info.Phase_Info_Array[i].Phase_ID==-1 && Phase_Array[i]!=NULL)
		{
			AfxMessageBox("phase data does not match the number");
		}
		
		if (Phase_Array[i]!=NULL)
		{		
			extern char Current_Control_Type;
			if (Current_Control_Info.Cycle_Time==0)
			{
				char a= Current_Control_Type;
			}
			Phase_ID= Current_Control_Info.Phase_Info_Array[i].Phase_ID;
			Set_Phases_Cycle_Time(Phase_ID, Current_Control_Info.Cycle_Time);  
			Set_Phases_Green_Percent(Phase_ID, Current_Control_Info.Phase_Info_Array[i].Green_Percent);
			Set_Phases_Green_Start_Time(Phase_ID,Current_Control_Info.Phase_Info_Array[i].Green_Start_Time_Percent);
		}
		else
			break;
	}
}

void CController::Send_Data(SOCKET sendSocket, SOCKADDR_IN addrTo)
{
	WSABUF wsabuf;
	DWORD dwSend;
	int len;

	len=sizeof(tran_mbc_detector);
	wsabuf.buf=(char*)&tran_mbc_detector;
	wsabuf.len = len; 
	if(SOCKET_ERROR==WSASendTo(sendSocket,&wsabuf,1,&dwSend,0, (SOCKADDR*)&addrTo,sizeof(SOCKADDR),NULL,NULL))
	{
		int Error_ID= WSAGetLastError();
		AfxMessageBox("#failed in sending data");  
	}

}

bool CController::Update_or_Not()
{
	int Result= New_Phase_Start_or_Not();
	if (Result!=-1 && Last_Update_Time-simu_time!=0)
	{
		Last_Update_Time= simu_time;
		return true;
	}
	else
	{
		return false;
	}
/*
	//upload data by cycle
	simu_time - Last_Refresh_Time == Current_Control_Info.Cycle_Time

	////////////////////////
	int Time_in_This_Cycle= simu_time-Last_Refresh_Time;
	int Time_Red_to_Green;

	for (int i=0; i<MAX_PHASE_NUMBER; i++)
	{
		if (Phase_Array[i]!=NULL)
		{
			Time_Red_to_Green= Phase_Array[i]->Green_Start_Time;     
			if (Phase_Array[i]->Green_Percent!=100 )
				if (Time_Red_to_Green==Time_in_This_Cycle)
					return true;
		}
	}
	return false;
*/	
}

int CController::New_Phase_Start_or_Not()
{
	int Time_Red_to_Green;
	int Time_in_This_Cycle= simu_time-Last_Refresh_Time;
	int Phase_ID;

	for (int i=0; i<MAX_PHASE_NUMBER; i++)
	{
		if (Phase_Array[i]!=NULL)
		{
			Time_Red_to_Green= Phase_Array[i]->Green_Start_Time;   //red-->green
			if (Phase_Array[i]->Green_Percent!=100 )                  
				if (Time_Red_to_Green==Time_in_This_Cycle)
				{
					Phase_ID=i;
					return i;
				}
		}
	}
	return -1;	
}

void CController::Controller_Run()
{
	extern char Current_Control_Type;
	extern CCommunicator *communicator;
	extern bool Switch_Detector;

	switch (Current_Control_Type)
	{
	case 'N':  //no control 
		break;
		
	case 'P':                                  //point control 
		if (Refresh_or_Not())
		{
			Update_Control_Info();            //generate point control strategy 
			Update_Phase(); 
		}
		break;
		
	case 'R':                                  //regional control
		if (Refresh_or_Not())            
		{
			Update_Control_Info();  
			if (Current_Control_Info.Cycle_Time==-1)
			{
				AfxMessageBox("#No regional control data");
				return;
			}
			Update_Phase();  
		}
		break;
	}


	if (Switch_Detector && Current_Control_Type!='N')
	{
		if(true==Update_or_Not())       
		{
			Save_Update_Data();
			Send_Data(communicator->DetectorSock, communicator->DetectorAddrTo );            //send traffic flow data from detector
			Clear_Tran_MBC();                                                                //clear tran_traffic_flow in communicator
			Reset_Detector();
		}
	}
}


void CController::Clear_Tran_MBC()
{
	memset( (char*)&tran_mbc_detector, -1, sizeof(tran_mbc_detector) );
	memset( (char*)&Update_Data, -1, sizeof(Update_Data) );
}
void CController::Reset_Detector()
{
	for (int i=0;i<MAX_ENTER_LINK;i++)
	{
		for (int j=0; j<MAX_DETECTOR_NUMBER;j++)
		{
			if(Detector_Under_Control[i][j]!=NULL)
			{
				Detector_Under_Control[i][j]->Reset_Detector_Info();
			}
		}
	}
}

void CController::Calculate_Upload_Data()
{

}


void CController::Save_Update_Data()
{

	extern CCommunicator *communicator;

	Update_Data.Area_ID= 0;
	Update_Data.Cross_ID= htonl(Cross_ID);

	Calculate_Controller_Update_Data();
	Calculate_Lane_Update_Data();
	
	tran_mbc_detector.MessageHead=0x68;	
	tran_mbc_detector.SystemMark=0;
	tran_mbc_detector.DataLength= htonl( sizeof(Update_Data) );
	tran_mbc_detector.update_data_info= Update_Data;
	tran_mbc_detector.MessageEnd=0x18;	
}

void CController::Calculate_Controller_Update_Data()
{
	extern char Current_Control_Type;
	unsigned char Char_New_Start_Phase_ID= -1;
	int Int_New_Start_Phase_ID=-1;
	int New_Phase_Time=0;

	Int_New_Start_Phase_ID= New_Phase_Start_or_Not();

	Char_New_Start_Phase_ID= (char)Int_New_Start_Phase_ID;
	if (Int_New_Start_Phase_ID==-1)
		AfxMessageBox("Error---CController::Calculate_Controller_Update_Data()");
	else
		New_Phase_Time= (int)(Phase_Array[Int_New_Start_Phase_ID]->Green_Percent* Current_Control_Info.Cycle_Time/100);

	Update_Data.controllor_update_info.Current_Time[0]=       htonl(2008);
	Update_Data.controllor_update_info.Current_Time[1]=       htonl(8);
	Update_Data.controllor_update_info.Current_Time[2]=       htonl(27);
	Update_Data.controllor_update_info.Current_Time[3]=       htonl(1);
	Update_Data.controllor_update_info.Current_Time[4]=       htonl(1);
	Update_Data.controllor_update_info.Current_Time[5]=       htonl(simu_time%60);
	Update_Data.controllor_update_info.Simu_Time=             htonl(simu_time);
	Update_Data.controllor_update_info.Cycle_Number=          htonl(Cycle_Number);
	Update_Data.controllor_update_info.Phase_ID=              Char_New_Start_Phase_ID;
	Update_Data.controllor_update_info.Phase_Time=            New_Phase_Time;
	Update_Data.controllor_update_info.Effective_Green_Time=  0;
	Update_Data.controllor_update_info.Control_Type=          Current_Control_Type;
	Update_Data.controllor_update_info.Is_Control_Good=       0;
	Update_Data.controllor_update_info.Is_Detector_Good=      0;
	Update_Data.controllor_update_info.Is_Loop_Good=          0;

}

int CController::Get_Veh_Sum(int Link_ID, int Lane_ID, int Smaller_Cell_ID, int Bigger_Cell_ID)
{
	int theSum=0;

	for (int i=Bigger_Cell_ID; i> Smaller_Cell_ID; i--)
	{
		if( Link_Array[Link_ID]->Lanes[Lane_ID]->Lane_Cell[i]->IsVehInCell()==true)
		{
			theSum++;
		}
	}
	return theSum;
}

int CController::Get_Delay_Time(int Link_ID, int Lane_ID)
{
	int Average_Delay_Time=0;
	
	switch (Lane_ID)
	{
	case 0:
		Average_Delay_Time = Link_Array[Link_ID]->Detector_Array[0]->Average_Delay_Time_0;

		Link_Array[Link_ID]->Detector_Array[0]->Average_Delay_Time_0=0;	
		Link_Array[Link_ID]->Detector_Array[0]->Veh_Sum_0=0;
		break;

	case 1:
		Average_Delay_Time = Link_Array[Link_ID]->Detector_Array[0]->Average_Delay_Time_1;

		Link_Array[Link_ID]->Detector_Array[0]->Average_Delay_Time_1=0;
		Link_Array[Link_ID]->Detector_Array[0]->Veh_Sum_1=0;
		break;

	case 2:
		Average_Delay_Time = Link_Array[Link_ID]->Detector_Array[0]->Average_Delay_Time_2;

		Link_Array[Link_ID]->Detector_Array[0]->Average_Delay_Time_2=0;
		Link_Array[Link_ID]->Detector_Array[0]->Veh_Sum_2=0;
		break;

	}

	return Average_Delay_Time;

}

void CController::Calculate_Lane_Update_Data()
{
	int i=0;
	int j;
	int Queue_Length_0_for_Control;
	int Queue_Length_1_for_Control;  
	int Queue_Length_2_for_Control;  
	int Queue_Length_0_for_Guidance; 
	int Queue_Length_1_for_Guidance;  
	int Queue_Length_2_for_Guidance; 
	int Queue_Delay_Time_0;
	int Queue_Delay_Time_1;
	int Queue_Delay_Time_2;
	int a,b,c,d,e,f;
	char hh,ii,jj;
	int Link_ID;
	int Phase_Length;

	for (j=0; j<MAX_ENTER_LINK;j++)
	{
		Link_ID= Detector_Under_Control[j][0]->Link_ID;   
		Phase_Length=Update_Data.controllor_update_info.Phase_Time;

		a=   Link_Array[Link_ID]->Queue_Length_0_for_Control ; 
		b=   Link_Array[Link_ID]->Queue_Length_1_for_Control;  
		c=   Link_Array[Link_ID]->Queue_Length_2_for_Control;  
		d=   Link_Array[Link_ID]->Queue_Length_0_for_Guidance;  
		e=   Link_Array[Link_ID]->Queue_Length_1_for_Guidance;  
		f=   Link_Array[Link_ID]->Queue_Length_2_for_Guidance;  

		hh=   (char)((A_MIN* Detector_Under_Control[j][1]->detector_info.Left_Veh_Number)/Phase_Length);
		ii=   (char)((A_MIN* Detector_Under_Control[j][1]->detector_info.Straight_Veh_Number)/Phase_Length);
		jj=   (char)((A_MIN* Detector_Under_Control[j][1]->detector_info.Right_Veh_Number)/Phase_Length);


		if (Link_Array[Link_ID]->Lane_Number==4) 
		{
			Queue_Length_0_for_Control= Get_Veh_Sum(Link_ID, 0, Link_Array[Link_ID]->Lanes[0]->Cell_Number-1-29, Link_Array[Link_ID]->Lanes[0]->Cell_Number-1);
			Queue_Length_1_for_Control= Get_Veh_Sum(Link_ID, 1, Link_Array[Link_ID]->Lanes[1]->Cell_Number-1-29, Link_Array[Link_ID]->Lanes[1]->Cell_Number-1)
									                  + Get_Veh_Sum(Link_ID, 2, Link_Array[Link_ID]->Lanes[2]->Cell_Number-1-29, Link_Array[Link_ID]->Lanes[2]->Cell_Number-1);
			Queue_Length_2_for_Control= Get_Veh_Sum(Link_ID, 3, Link_Array[Link_ID]->Lanes[3]->Cell_Number-1-29, Link_Array[Link_ID]->Lanes[3]->Cell_Number-1);

			Queue_Length_0_for_Guidance=  Get_Veh_Sum(Link_ID, 0, 0, Link_Array[Link_ID]->Lanes[0]->Cell_Number-1);  
			Queue_Length_1_for_Guidance=  Get_Veh_Sum(Link_ID, 1, 0, Link_Array[Link_ID]->Lanes[1]->Cell_Number-1)
								                         +  Get_Veh_Sum(Link_ID, 2, 0, Link_Array[Link_ID]->Lanes[2]->Cell_Number-1);
			Queue_Length_2_for_Guidance=  Get_Veh_Sum(Link_ID, 3, 0, Link_Array[Link_ID]->Lanes[3]->Cell_Number-1);    

			int Delay_Time_1= Get_Delay_Time(Link_ID, 1);
			int Delay_Time_2= Get_Delay_Time(Link_ID, 2);
			Queue_Delay_Time_0= Get_Delay_Time(Link_ID, 0);
			Queue_Delay_Time_1= Get_Max_Value (Delay_Time_1,Delay_Time_2);
			Queue_Delay_Time_2= Get_Delay_Time(Link_ID, 3);

		}
		else
		{
			Queue_Length_0_for_Control= Get_Veh_Sum(Link_ID, 0, Link_Array[Link_ID]->Lanes[0]->Cell_Number-1-29, Link_Array[Link_ID]->Lanes[0]->Cell_Number-1);
			Queue_Length_1_for_Control= Get_Veh_Sum(Link_ID, 1, Link_Array[Link_ID]->Lanes[1]->Cell_Number-1-29, Link_Array[Link_ID]->Lanes[1]->Cell_Number-1);
			Queue_Length_2_for_Control= Get_Veh_Sum(Link_ID, 2, Link_Array[Link_ID]->Lanes[2]->Cell_Number-1-29, Link_Array[Link_ID]->Lanes[2]->Cell_Number-1);			

			Queue_Length_0_for_Guidance=  Get_Veh_Sum(Link_ID, 0, 0, Link_Array[Link_ID]->Lanes[0]->Cell_Number-1);  
			Queue_Length_1_for_Guidance=  Get_Veh_Sum(Link_ID, 1, 0, Link_Array[Link_ID]->Lanes[1]->Cell_Number-1);
			Queue_Length_2_for_Guidance=  Get_Veh_Sum(Link_ID, 2, 0, Link_Array[Link_ID]->Lanes[2]->Cell_Number-1);    

			Queue_Delay_Time_0= Get_Delay_Time(Link_ID, 0);
			Queue_Delay_Time_1= Get_Delay_Time(Link_ID, 1);
			Queue_Delay_Time_2= Get_Delay_Time(Link_ID, 2);
		}

		///////////////////////////////////////////////
		//left turn 
		Update_Data.lane_update_info_array[i].Data_Flag=                 0xff;
		Update_Data.lane_update_info_array[i].Lane_ID=                   htonl(Link_ID*10+0);  
		Update_Data.lane_update_info_array[i].Arrived_Veh_Sum=       char(Detector_Under_Control[j][1]->detector_info.Left_Veh_Number);
		Update_Data.lane_update_info_array[i].Passed_Veh_Sum=        char(Detector_Under_Control[j][0]->detector_info.Left_Veh_Number);
		Update_Data.lane_update_info_array[i].Current_Veh_Sum=       htonl(Phase_Length*Queue_Length_0_for_Guidance);
		Update_Data.lane_update_info_array[i].Long_Veh_Sum=          hh;
		Update_Data.lane_update_info_array[i].Mean_Speed=                char(Pixel_Per_Cell*Detector_Under_Control[j][0]->detector_info.Left_Speed_Mean);
		Update_Data.lane_update_info_array[i].Occupancy=          char(100*Update_Data.lane_update_info_array[i].Passed_Veh_Sum/Update_Data.controllor_update_info.Phase_Time);
		Update_Data.lane_update_info_array[i].Queue_Delay_Time=          Queue_Delay_Time_0;
		Update_Data.lane_update_info_array[i].Queue_Length =             char(Queue_Length_0_for_Control);

		//through traffic
		Update_Data.lane_update_info_array[i+1].Data_Flag=               0xff;
		Update_Data.lane_update_info_array[i+1].Lane_ID=                 htonl(Link_ID*10+1);
		Update_Data.lane_update_info_array[i+1].Arrived_Veh_Sum=     char(Detector_Under_Control[j][1]->detector_info.Straight_Veh_Number);
		Update_Data.lane_update_info_array[i+1].Passed_Veh_Sum=      char(Detector_Under_Control[j][0]->detector_info.Straight_Veh_Number);
		Update_Data.lane_update_info_array[i+1].Current_Veh_Sum=     htonl(Phase_Length*Queue_Length_1_for_Guidance);
		Update_Data.lane_update_info_array[i+1].Long_Veh_Sum=        ii;
		Update_Data.lane_update_info_array[i+1].Mean_Speed=              char(Pixel_Per_Cell*Detector_Under_Control[j][0]->detector_info.Straight_Speed_Mean);
		Update_Data.lane_update_info_array[i+1].Occupancy=        char(100*Update_Data.lane_update_info_array[i+1].Passed_Veh_Sum/Update_Data.controllor_update_info.Phase_Time);
		Update_Data.lane_update_info_array[i+1].Queue_Delay_Time=        Queue_Delay_Time_1;
		Update_Data.lane_update_info_array[i+1].Queue_Length=            char(Queue_Length_1_for_Control);

		//right turn
		Update_Data.lane_update_info_array[i+2].Data_Flag=               0xff;
		Update_Data.lane_update_info_array[i+2].Lane_ID=                 htonl(Link_ID*10+2);
		Update_Data.lane_update_info_array[i+2].Arrived_Veh_Sum=     char(Detector_Under_Control[j][1]->detector_info.Right_Veh_Number);
		Update_Data.lane_update_info_array[i+2].Passed_Veh_Sum=      char(Detector_Under_Control[j][0]->detector_info.Right_Veh_Number);
		Update_Data.lane_update_info_array[i+2].Current_Veh_Sum=     htonl(Phase_Length*Queue_Length_2_for_Guidance);
		Update_Data.lane_update_info_array[i+2].Long_Veh_Sum=        jj;
		Update_Data.lane_update_info_array[i+2].Mean_Speed=              char(Pixel_Per_Cell*Detector_Under_Control[j][0]->detector_info.Right_Speed_Mean);
		Update_Data.lane_update_info_array[i+2].Occupancy=        char(100*Update_Data.lane_update_info_array[i+2].Passed_Veh_Sum/Update_Data.controllor_update_info.Phase_Time);
		Update_Data.lane_update_info_array[i+2].Queue_Delay_Time=        Queue_Delay_Time_2;
		Update_Data.lane_update_info_array[i+2].Queue_Length=            char(Queue_Length_2_for_Control);

		i=i+3;
	}
}

