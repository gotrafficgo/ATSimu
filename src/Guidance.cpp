// Guidance.cpp: implementation of the CGuidance class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "traffic.h"
#include "Guidance.h"
#include "ErrorLog.h"
#include "Global.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuidance::CGuidance(int guidance_id,int link_id)
{
	Last_Refresh_Time=INITIAL_VALUE_OF_LAST_REFRESH_TIME;
	Link_ID=link_id;
	Guidance_ID= guidance_id;
	Received_Gudiance_Times=0;
	Received_New_Guidance_Info=false;

	memset( (char*)&Current_Guidance_Info, -1, sizeof(Current_Guidance_Info));
	memset( (char*)&Received_Guidance_Info, -1, sizeof(Received_Guidance_Info));
}

CGuidance::~CGuidance()
{

}


void CGuidance:: Guidance_Run()
{
	if (Refresh_or_Not())
		if (Received_New_Guidance_Info==true && Is_Guidance_Info_Right==true)
		{
			Refresh_Guidance_Info();
			AfxMessageBox("Refreshed");
		}
		else
			simu_pause_for_no_guidance= true;  
}

void CGuidance:: Refresh_Guidance_Info()
{
	Current_Guidance_Info= Received_Guidance_Info;

	Last_Refresh_Time=simu_time;
	Received_New_Guidance_Info=false;
}

bool CGuidance::Refresh_or_Not()
{
	extern bool Switch_Guidance;
	bool Yes_or_No=false;

	if(simu_time%GUIDANCE_CIRCLE==199)   //for testing
	{
		Yes_or_No= true;
	}

// 	if (Is_Sending_to_Self==true)
// 		if(simu_time%GUIDANCE_CIRCLE==1)
// 		{
// 			Yes_or_No= true;
// 			Is_Sending_to_Self=false;
// 		}

	if (Switch_Guidance==true && simu_pause_for_no_guidance==true)
		Yes_or_No= true;

	return Yes_or_No;
}
