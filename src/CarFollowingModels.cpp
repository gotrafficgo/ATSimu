#pragma once
#include "StdAfx.h"
#include "setting.h"
#include <math.h>
#include "Global.h"

int Location_From_Gipps(int Cur_Loc, double Cur_Spd)
{
	//Eular method is used, and h=1;
	int Next_Loc=int(Cur_Loc+Cur_Spd);
	return Next_Loc;
}

double Speed_From_Gipps(int Veh_Loc, double Veh_Spd, int FrontVeh_Loc, double FrontVeh_Spd, double Max_Speed)
{
	///////////////////////
	//parameters
	int tau=1;
	double Vmax = Max_Speed;   //free flow speed
	double A=1.7;               //Acceleration: 1.7m/s^2            
	double B = 3.0;             //the estimated breaking parameter: 3.0m/s^2                 
	double B0 = 2.8;           //the actual braking parameter: 2.8m/s^2                    
	double THETA =0.5;       // theta is safety margin parameter, gipps sets theta equals one-half of tao   
	double S0 = VEHICLE_LENGTH;     //here is 7m       //veh length of the (first) veh : 6.5m

	double v=Veh_Spd;
	double vleader= FrontVeh_Spd;
	int s= FrontVeh_Loc-Veh_Loc;
	double x= B*B*(tau/2 + THETA)*(tau/2 + THETA) + B*(2*(s - S0) - tau*v + vleader*vleader/B0);
	if (x<0) x=0;
	double y=pow(x, 0.5);
	//Vfree= v + 2.5*a*tau*(1 - v/Vmax)*(0.025 + v/Vmax)^0.5;
    //Vcong= -B*(tau/2 + THETA) + (B^2*(tau/2 + THETA)^2 + B*(2*(s - S0) - tau*v + vleader^2/B0))^0.5;
	double Vfree= v + 2.5*A*tau*(1 - v/Vmax)*pow((0.025 + v/Vmax) , 0.5);
	double Vcong= -B*(tau/2 + THETA)+y;   //1.73

	Vfree=ceil(Vfree);
	Vcong=floor(Vcong);

	double VV = Get_Min_Value(Vfree, Vcong);

	if (VV<0) 
		VV=0;

	return VV;
}



int Speed_From_VDR(int Veh_Loc, int Veh_Spd, int Last_FrontVeh_Loc, int Max_Speed)
{
	////////////////////////////////////////////////////////
	// 	Step 0: determination of the randomization parameter p(v)
	// 		For standing cars (v=0) one has p(v=0)=p2 .
	// 		For moving cars ,(v>0) the randomization is chosen as p(v)=p.
	double pv;
	double p2=CA_P2;
	double p=CA_P;
	if (Veh_Spd==0)
		pv=p2;
	else
		pv=p;
	
	////////////////////////////////////////////////////////
	// Step 1: acceleration
	// All cars that have not already reached the maximal velocity vmax acceleration by one unit: v -> v+1
	if (Veh_Spd<Max_Speed)
		Veh_Spd++;

	////////////////////////////////////////////////////////
	// 	Step 2: safety distance
	// 		If a car has d empty cells in front of it and is its velocity v (after step 1) larger then d, then it reduces the velocity to d:
	// 		v -> min{d,v}
	int d= Last_FrontVeh_Loc - Veh_Loc - 1;
	Veh_Spd= Get_Min_Value(d, Veh_Spd);

	// 	Step 3: randomization
	// 		With probability p, the velocity is reduced by one unit (if v after step 2):v -> v-1
	if(Veh_Spd!=0)
		if (True_Or_False(pv, 100))
			Veh_Spd--;

	// 	Step 4: driving
	// 		After steps 1-3 the new velocity vn for each car n has been determined forward by vn cells: xn -> xn+vn.

	return Veh_Spd;
}

int Location_From_VDR(int Cur_Loc, int Cur_Spd)
{
	int This_Time_Location =Cur_Spd + Cur_Loc;
	return This_Time_Location;
}










// int Location_From_IDM(int Cur_Loc, double Cur_Spd)
// {
// 	//Eular method is used, and h=1;
// 	int Next_Loc=int(Cur_Loc+Cur_Spd);
// 	return Next_Loc;
// }
// 
// double Speed_From_IDM(int Veh_Loc, double Veh_Spd, int FrontVeh_Loc, double FrontVeh_Spd, double Max_Speed)
// {
// 	double Vmax = Max_Speed;   //free flow speed
// 	double A=1.0;                         //Acceleration: 1.7m/s^2
// 	double B = 3.0;                       //the estimated breaking parameter: 3.0m/s^2
// 	double T = 1.5;                       //time headway---1.5(s)
// 	double s0 = 2;                        //minimum gap---2(m)
// 
// 	double dv= Veh_Spd- FrontVeh_Spd;
// 	double v=Veh_Spd;
// 	int s=FrontVeh_Loc-Veh_Loc;
// 
// 	double S = s0 + v*T + v*dv/(2* pow((A*B),0.5) );
// 	double Acceleration = A*(1 - pow((v/Vmax), 4) - pow((S/s), 2));
// 
// 	double Next_Speed= Veh_Spd+Acceleration;
// 
// 	if(Next_Speed<0)
// 		Next_Speed=0;
// 
// 	return Next_Speed;
// }

// 
// int Location_From_Newell(int CurX, int CurLeaderX, double Max_Speed)    //t-tao or t-1 is current time, t is next step
// {
// 	double u= Max_Speed;    //FREE_FLOW_SPEED
// 	double w= WAVE_SPEED;
// 	double k=MAX_DENSITY;
// 
// //	double tao= 1;
// 	double delta= 1.0;
// 
// 	double Xfree= CurX + u;
// 	double Xcong= CurLeaderX- delta;
// 
// 	int XX = int(Get_Min_Value(Xfree, Xcong));
// 
// 	return XX;
// }
// 
// 
// 
// double Speed_From_Newell(double CurLeaderV, double Max_Speed)
// {
// 	double Vfree= FREE_FLOW_SPEED;
// 	double Vcong= CurLeaderV;
// 	double VV= Get_Min_Value(Vfree, Vcong);
// 
// 	return VV;
// }
