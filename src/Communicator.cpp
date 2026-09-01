// Communicator.cpp: implementation of the CCommunicator class.
//
//////////////////////////////////////////////////////////////////////

#include "Stdafx.h"
#include "traffic.h"
#include "Communicator.h"
#include "Global.h"
#include "ErrorLog.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommunicator::CCommunicator()
{
	Guidance_Received_Times=0;
	isNewGuidance=false;

	memset( (char*)&tran_guidance, 0, sizeof(tran_guidance) );
	memset( (char*)&tran_guidance_0, 0, sizeof(tran_guidance) );

	memset( (char*)&tran_mbc_detector, -1, sizeof(tran_mbc_detector) );
	memset( (char*)&received_control_info, -1, sizeof(received_control_info));
	memset( (char*)&Guidance_Info, 0, sizeof(Guidance_Info));

// 	static int Guidance_Port= GUIDANCE_PORT;
// 	GuidanceSock= Setup_UDP_RecvSocket(Guidance_Port);   
// 	
// 	static int Control_Port= CONTROL_PORT;
// 	ControlSock= Setup_UDP_RecvSocket(Control_Port);
// 
// 	DetectorSock= Setup_UDP_SendSocket();
// 	DetectorAddrTo= Get_Send_Addr("192.168.0.4", FLOW_DATA_PORT);
// 
// 	SimuTimeSock= Setup_UDP_SendSocket();
// 	SimuTimeAddrTo= Get_Send_Addr("192.168.0.4", SIMU_TIME_PORT);
// 
// 	theGuidanceSock= Setup_UDP_SendSocket();
// 	theGuidanceAddrTo= Get_Send_Addr("127.0.0.1", GUIDANCE_PORT);
}

CCommunicator::~CCommunicator()
{

}


SOCKET CCommunicator::Setup_UDP_SendSocket()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) 
	{
		return NULL;
	}

	if ( LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 ) 
	{
		WSACleanup( );
		return NULL; 
	}

	SOCKET sockSend= socket(AF_INET, SOCK_DGRAM, 0);
// 
// 	SOCKADDR_IN addrSend;
// 	addrSend.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
// 	addrSend.sin_family= AF_INET;
// 
// //	addrSend.sin_port=htons(FLOW_DATA_PORT); 
// 	//	addrRecv.sin_port= htons(Port);
// 
// 	///////////////////////////////////
// 	if( SOCKET_ERROR==bind(sockSend, (SOCKADDR*)&addrSend, sizeof(SOCKADDR)) )  
// 	{
// 		int Error_ID= WSAGetLastError();
// 		AfxMessageBox("#binding failed");
// 	}

	return sockSend;
}

SOCKADDR_IN CCommunicator::Get_Send_Addr(const char * IP_Addr, int Port)
{
	SOCKADDR_IN addrTo;

	addrTo.sin_addr.S_un.S_addr=inet_addr(IP_Addr);
	addrTo.sin_family=AF_INET;
	addrTo.sin_port=htons(Port); 

	return addrTo;
}


SOCKET CCommunicator::Setup_UDP_RecvSocket(int Port)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	
	wVersionRequested = MAKEWORD( 2, 2 );
	
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		return NULL;
	}
	
	if ( LOBYTE( wsaData.wVersion ) != 2 ||
        HIBYTE( wsaData.wVersion ) != 2 ) {
		WSACleanup( );
		return NULL; 
	}

	///////////////////////////////////
/*	SOCKET sockRecv= socket(AF_INET, SOCK_DGRAM, 0);*/
	SOCKET sockRecv= WSASocket(AF_INET,SOCK_DGRAM,0,NULL,0,0);
	SOCKADDR_IN addrRecv;
	addrRecv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrRecv.sin_family= AF_INET;
	addrRecv.sin_port= htons(Port);

	///////////////////////////////////
	if( SOCKET_ERROR==bind(sockRecv, (SOCKADDR*)&addrRecv, sizeof(SOCKADDR)) )  
	{
		int Error_ID= WSAGetLastError();
		AfxMessageBox("#binding failed");
	}
	
	return sockRecv;
}



void CCommunicator::Receive_Data(SOCKET sockRecv, char Data_Type)
{
	int len=sizeof(SOCKADDR);
	WSABUF wsabuf;
	int len2;
	char *szChar;
	DWORD dwRead;
	DWORD dwFlag=0;
	SOCKADDR_IN addrFrom;  
	int Control_Cross_ID;
	CString str;
	extern ErrorLog *err;

	switch(Data_Type)
	{
	case 'G':
		len2= sizeof(tran_guidance_0);
		szChar= new char[len2];
		wsabuf.buf= szChar;
		wsabuf.len=len2+1;

		//blocking
		if(SOCKET_ERROR==WSARecvFrom(sockRecv,&wsabuf,1,&dwRead,&dwFlag,(SOCKADDR*)&addrFrom,&len,NULL,NULL)) 
		{
			int Error_ID= WSAGetLastError();
			AfxMessageBox("#failed in receiving guidance information!"); 
			delete[] wsabuf.buf;  
			return ;
		}
		len2= sizeof(wsabuf);
		memcpy((char*)&tran_guidance_0,wsabuf.buf,sizeof(tran_guidance_0));

		//received the first one
		if(tran_guidance_0.Guidance_Buffer[0].This_Times != tran_guidance.Guidance_Buffer[0].This_Times ) 
		{
			tran_guidance= tran_guidance_0;
			Received_Guidance_Number++;   
			isNewGuidance=true;        //if it is different from the other one, then update
		}
		//received the second one
		else
		{
			memset( (char*)&tran_guidance_0, 0, sizeof(tran_guidance_0) ); 
			isNewGuidance=false;        
			return;
		}

//		Test_for_Guidance();
		
		break;
	
	case 'R':
		//region control information
		len2=CONTROL_BUFFER_SIZE;         // sizeof(received_control_info);
		szChar= new char[len2];
		wsabuf.buf= szChar;
		wsabuf.len=len2+1; 
		
		if(SOCKET_ERROR==WSARecvFrom(sockRecv,&wsabuf,1,&dwRead,&dwFlag,(SOCKADDR*)&addrFrom,&len,NULL,NULL)) 
		{
			int Error_ID= WSAGetLastError();
			AfxMessageBox("#failed in receiving data!"); 
			delete[] wsabuf.buf;  
			return ;
		}
		memcpy((char*)&received_control_info, wsabuf.buf, sizeof(received_control_info));

//		Test_for_Control();	

		Control_Cross_ID= received_control_info.Cross_ID;

		//no information
		if (Cross_Array[Control_Cross_ID]->Controller->Received_Control_Info.Cycle_Time==-1 
			&& Cross_Array[Control_Cross_ID]->Controller->Received_Control_Info_0.Cycle_Time==-1)
		{
			Cross_Array[Control_Cross_ID]->Controller->Received_Control_Info= received_control_info;
		}
		else if //Received_Control_Info_0 
			(Cross_Array[Control_Cross_ID]->Controller->Received_Control_Info.Cycle_Time==-1 
				  && Cross_Array[Control_Cross_ID]->Controller->Received_Control_Info_0.Cycle_Time!=-1)
		{
			Cross_Array[Control_Cross_ID]->Controller->Received_Control_Info
				= Cross_Array[Control_Cross_ID]->Controller->Received_Control_Info_0;
			Cross_Array[Control_Cross_ID]->Controller->Received_Control_Info_0= received_control_info;
		}
		else if  //Received_Control_Info 
		    (Cross_Array[Control_Cross_ID]->Controller->Received_Control_Info.Cycle_Time!=-1 
			&& Cross_Array[Control_Cross_ID]->Controller->Received_Control_Info_0.Cycle_Time==-1)
		{
			Cross_Array[Control_Cross_ID]->Controller->Received_Control_Info_0= received_control_info;
		}

		break;
	} 
}

//use for test, need to comment WSARecvFrom and its below
void CCommunicator::Test_for_Guidance()
{
	int i;
	int j;
	int Link_Sum=24;
	int This_Times=0;
	////////////////////////////
	for (i=0;i<Link_Sum;i++)
	{
		tran_guidance.Guidance_Buffer[i].This_Times=This_Times;
		tran_guidance.Guidance_Buffer[i].Link_ID= i;
		tran_guidance.Guidance_Buffer[i].Guidance_Dest_Size=0;

		for (j=0;j<MAX_GUIDANCE_DESTINATION;j++)
			tran_guidance.Guidance_Buffer[i].Guidance_Dest[j]=-1;
		
		for (j=0;j<MAX_GUIDANCE_DESTINATION;j++)
		{
			tran_guidance.Guidance_Buffer[i].Turn_Rate[j][0]=0;
			tran_guidance.Guidance_Buffer[i].Turn_Rate[j][1]=0;
			tran_guidance.Guidance_Buffer[i].Turn_Rate[j][2]=0;
		}
	}
	/////////////////////////////
	tran_guidance.Guidance_Buffer[17].Link_ID= 17;
	tran_guidance.Guidance_Buffer[17].Guidance_Dest_Size=1;
	tran_guidance.Guidance_Buffer[17].Guidance_Dest[0]=37;
	tran_guidance.Guidance_Buffer[17].Turn_Rate[0][0]=0;
	tran_guidance.Guidance_Buffer[17].Turn_Rate[0][1]=0;
	tran_guidance.Guidance_Buffer[17].Turn_Rate[0][2]=30;
}

void CCommunicator::Test_for_Control()
{
	int Control_Cross_ID;

	if (simu_time==200)
	{
		received_control_info.Cross_ID=0;
		received_control_info.Cycle_Time=30;
		Control_Cross_ID= received_control_info.Cross_ID;
		Cross_Array[Control_Cross_ID]->Controller->Received_Control_Info= received_control_info;
	}
	if (simu_time==400)
	{
		received_control_info.Cross_ID=0;
		received_control_info.Cycle_Time=100;
		Control_Cross_ID= received_control_info.Cross_ID;
		Cross_Array[Control_Cross_ID]->Controller->Received_Control_Info= received_control_info;
	}
}

void CCommunicator::Clean_Socket()
{

	closesocket(ControlSock);
	closesocket(DetectorSock);
	closesocket(GuidanceSock);
	WSACleanup();
}

bool CCommunicator::Received_or_Not(int This_Times, char Info_Type)
{
	switch(Info_Type)
	{
	case 'G' :
		if (This_Times==0 || This_Times- Guidance_Received_Times>=1)  
		{	
			Guidance_Received_Times=This_Times;
			return true;
		}
		else
			return false;

	}
	return false;
}


void CCommunicator::Send_Simu_Time()
{
	WSABUF wsabuf;
	DWORD dwSend;
	int len;
	TranStruct_SimuTime transtruct_simutime;

	transtruct_simutime.Head=0x69;
	transtruct_simutime.Simu_Time=(htonl)(simu_time);

	len=sizeof(transtruct_simutime);
	wsabuf.buf=(char*)&transtruct_simutime;
	wsabuf.len = len; 
	if(SOCKET_ERROR==WSASendTo(SimuTimeSock ,&wsabuf,1,&dwSend,0, (SOCKADDR*)&SimuTimeAddrTo ,sizeof(SOCKADDR),NULL,NULL))
	{
		int Error_ID= WSAGetLastError();
		AfxMessageBox("#failed in sending data");  
	}

	CString str;

////////////////////////////////////////////////////
// 	if (Last_Simu_Time+1!=simu_time)
// 	{
// 		CString str0, str1;
// 		str0.Format("%d", Last_Simu_Time);
// 		str1.Format("%d", simu_time);
// 
// 		AfxMessageBox("LastSimuTime: "+str0+"\n"+"CurrentSimuTime: " +str1+"\n");
// 	}
////////////////////////////////////////////////////////


//	simu_time_sum++;	
//	Last_Simu_Time=simu_time;
}


void CCommunicator::Send_Guidance_to_itself(TranStruct_Guidance the_guidance_info)
{
	WSABUF wsabuf;
	DWORD dwSend;
	int len;

	len=sizeof(the_guidance_info);
	wsabuf.buf=(char*)&the_guidance_info;
	wsabuf.len = len; 
	if(SOCKET_ERROR==WSASendTo(theGuidanceSock ,&wsabuf,1,&dwSend,0, (SOCKADDR*)&theGuidanceAddrTo ,sizeof(SOCKADDR),NULL,NULL))
	{
		int Error_ID= WSAGetLastError();
		AfxMessageBox("#failed in sending data");  
	}
}
