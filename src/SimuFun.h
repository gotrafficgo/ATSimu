#pragma once

//#import  "C:\\Windows\\System32\\MSXML6.dll"
#import  <MSXML6.dll>
#include "setting.h"
#include "Detector.h"
#include "Guidance.h"
#include "Cell.h"

class CPhase;

class CSimuFun  
{
public:
	CSimuFun();
	virtual ~CSimuFun();
//	bool Information_Ready;
public:
	int Get_XML_MarginalPoint(MSXML2::IXMLDOMDocumentPtr  pDOMDoc );
	int Get_XML_Cross(MSXML2::IXMLDOMDocumentPtr  pDOMDoc);
	int Get_XML_Link(MSXML2::IXMLDOMDocumentPtr  pDOMDoc );
	int Get_XML_Lane(MSXML2::IXMLDOMDocumentPtr  pDOMDoc);
	int Get_XML_Controller(MSXML2::IXMLDOMDocumentPtr  pDOMDoc); 
	int Get_XML_Demand(MSXML2::IXMLDOMDocumentPtr  pDOMDoc);
	int Get_XML_OD(MSXML2::IXMLDOMDocumentPtr  pDOMDoc);

	void Add_Cross_Lane(int Cross_ID,int Cross_Lane_ID, int Enter_Link_ID,int Enter_Lane_ID,int Next_Link_ID, int Next_Lane_ID, char Cross_Lane_Type);
	void Add_Conflict(int Cross_ID,CCell *Main_Cell,CCell *Conflict_Cell);
	bool Cross_Lane_Conflict_or_Not(char Type, int Cross_ID, int Determine_Route_ID, int Current_Straight_Route_ID);
	void Set_Conflict_Area();
	void Set_Straight_Conflict();
	int Set_Controller_on_Cross();
	void OnInit_Object();
	void Enlarge_Link();
	void Set_Object_Relation(); 
	void CCwise_Sort_Link();
	void Set_Cross_Edge_Point();
	void Set_Lane_Middle_Line();
	void Simu_Go_A_Step(); 
	void Set_Lane_Cell_Coordinate();
	void Set_Linking_Object();
	void Set_Cross_Lane_Cell_and_Enter_Link(); 
	void Set_Next_Link();   
	int Connect_Phase_And_Lane();  
	void Set_All_Distance_Shortest_Path();
	CPhase * Get_Phase_in_Lane(int Link_ID, int Cross_ID, int Lane_ID);  
	int Deal_with_Deviation_XML(int Get_Value, int Deviation);
	int Get_Turn_Direction(int Link_ID, int Lane_ID);
	void Connect_Controller_and_Detector(); 
	void Set_Controller_State(char Current_Control_Type, int Start_Time);  
	bool Enter_Link_or_Not(int Link_ID, int Cross_ID); 
	bool Straight_Route_or_Not(int Cross_ID, int Cross_Lane_ID, int Straight_Start_Link_ID, int Straight_End_Link_ID);
	void Set_Link_Label_And_GBoard();
	void Set_OD_On_Link();
	void Set_G2Detector();
	void Update_On_Link_Data();
	void Set_Commuter_On_Origin();

};


