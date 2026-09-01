#pragma once

#include "setting.h"
#include "Communicator.h"

class CGuidance  
{
	private:
		int Guidance_ID;
		int Link_ID;
	public:
		int Received_Gudiance_Times;
		int Last_Refresh_Time;
		bool Received_New_Guidance_Info;
		Struct_Guidance_Strategy Received_Guidance_Info;
		Struct_Guidance_Strategy Current_Guidance_Info;

	public:
		CGuidance(int guidance_id,int link_id);
		virtual ~CGuidance();
		void Guidance_Run();
		void Refresh_Guidance_Info();
		bool Refresh_or_Not();

};

