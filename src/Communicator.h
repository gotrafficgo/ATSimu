#pragma once

#include "setting.h"
#include <winsock2.h>  //Note: it is necessary to add ws2_32.lib in project->settings->link
#include "structure.h"

class CCommunicator  
{
public:
	CCommunicator();
	virtual ~CCommunicator();
	
	//for data transmission 
	TranStruct_MBC_Detector tran_mbc_detector;
	Struct_Control_Received received_control_info;
	TranStruct_Guidance tran_guidance;
	TranStruct_Guidance tran_guidance_0;     //buffer

	Struct_Guidance_Strategy Guidance_Info[MAX_LINK_NUMBER];  

	int Guidance_Received_Times;
	bool isNewGuidance;

	SOCKET GuidanceSock;
	SOCKET DetectorSock;
	SOCKET ControlSock;
	SOCKET SimuTimeSock;
	SOCKET theGuidanceSock;
	
	SOCKADDR_IN DetectorAddrTo;
	SOCKADDR_IN SimuTimeAddrTo;
	SOCKADDR_IN theGuidanceAddrTo;

	SOCKET Setup_UDP_RecvSocket(int Port);
	SOCKET Setup_UDP_SendSocket();
	SOCKADDR_IN Get_Send_Addr(const char * IP_Addr, int Port);

	void Test_for_Control();
	void Clean_Socket();
	void Receive_Data(SOCKET sockRecv, char Data_Type);
	bool Received_or_Not(int This_Times, char Info_Type);
	void Clear_Tran_MBC();
	void Test_for_Guidance();
	void Send_Simu_Time();
	void Send_Guidance_to_itself(TranStruct_Guidance the_guidance_info);

};

