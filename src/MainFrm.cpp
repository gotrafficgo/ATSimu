// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "traffic.h"
#include "setting.h"
#include "MainFrm.h"
#include "DlgShowData.h"
#include "ErrorLog.h"
#include "Global.h"
#include "PlotMFD.h"
#include "SimuFun.h"
#include "DlgSetting.h"
#include "DlgCarMovingModels.h"
#include "Global.h"
#include "Commuter.h"
#include "Origin.h"
#include "DlgDay2day.h"
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_SWITCH_DETECTOR, OnSwitchDetector)
	ON_COMMAND(ID_SWITCH_GUIDANCE, OnSwitchGuidance)
	ON_COMMAND(ID_SWITCH_SHOW_GUIDANCE, OnSwitchShowGuidance)
	ON_COMMAND(ID_SWITCH_POINT_CONTROL, OnSwitchPointControl)
	ON_COMMAND(ID_SWITCH_REGION_CONTROL, OnSwitchRegionControl)
	ON_COMMAND(ID_PLOT_MFD, OnPlotMFD)
	ON_COMMAND(ID_PLOT_TRAJECTORY, OnPlotTrajectory)
	ON_UPDATE_COMMAND_UI(ID_PLOT_MFD, OnUpdatePlotMFD)
	ON_UPDATE_COMMAND_UI(ID_PLOT_TRAJECTORY, OnUpdatePlotTrajectory)
	ON_COMMAND(ID_YELLOW_INTERSECTION, OnStartYellowIntersection)
	ON_UPDATE_COMMAND_UI(ID_YELLOW_INTERSECTION, OnUpdateStartYellowIntersection)
	ON_COMMAND(ID_CONFLICT, OnStartConflict)
	ON_UPDATE_COMMAND_UI(ID_CONFLICT, OnUpdateConflictBotton)
	ON_COMMAND(ID_TIME_SHORTEST_PATH, OnTimeShortestPath)
	ON_UPDATE_COMMAND_UI(ID_TIME_SHORTEST_PATH, OnUpdateTimeShortestPath)
	ON_COMMAND(ID_SETTING, OnStartSetting)
	ON_COMMAND(ID_CAR_MOVING_MODEL, OnStartCarMovingModel)
	ON_COMMAND(ID_SET_DAY_TO_DAY, OnStartSetDayToDay)
	ON_COMMAND(ID_DISPLAY_GRAPH, OnDisplayGraph)
	ON_UPDATE_COMMAND_UI(ID_DISPLAY_GRAPH, OnUpdateDisplayGraph)
	ON_UPDATE_COMMAND_UI(ID_SWITCH_GUIDANCE, OnUpdateSwitchGuidance)
	ON_UPDATE_COMMAND_UI(ID_SWITCH_SHOW_GUIDANCE, OnUpdateSwitchShowGuidance)
	ON_UPDATE_COMMAND_UI(ID_SWITCH_DETECTOR, OnUpdateSwitchDetector)
	ON_UPDATE_COMMAND_UI(ID_SWITCH_POINT_CONTROL, OnUpdateSwitchPointControl)
	ON_UPDATE_COMMAND_UI(ID_SWITCH_REGION_CONTROL, OnUpdateSwitchRegionControl)
	ON_COMMAND(ID_SHOW_DATA, OnShowData)
	ON_COMMAND(ID_CONTROL_VIEW, OnControlView)
	ON_UPDATE_COMMAND_UI(ID_SHOW_DATA, OnUpdateShowData)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_VIEW, OnUpdateControlView)
	ON_COMMAND(ID_GUIDANCE_VIEW, OnGuidanceView)
	ON_UPDATE_COMMAND_UI(ID_GUIDANCE_VIEW, OnUpdateGuidanceView)
	ON_COMMAND(ID_CG_STATEMENT, OnCGStatement)
	ON_UPDATE_COMMAND_UI(ID_CG_STATEMENT, OnUpdateCGStatement)
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	Show_Data_Flag=false;
	Show_Control_View_Flag=false;
	Show_Guidance_View_Flag=false;
	Show_GS_Statement_Flag=false;
	Show_PlotMFD_Flag=false;
	Show_Yellow_Intersection_Flag=true;
	Show_Conflict_Botton_Flag=true;
	Show_Time_Shortest_Path=false;
	Show_PlotTrajectory_Flag=false;

	st_PointControlSwitch=true;             //search "abc" for setting initial light state. for yellow at the beginning:  false  
	st_RegionControlSwitch=false;

	Switch_No_Control= false;     //search "abc" for setting initial light state. for yellow at the beginning:   true
	Switch_Point_Control= true;    //search "abc" for setting initial light state. for yellow at the beginning:      false  
	Switch_Region_Control= false;

	pDlgShowControl=NULL;
	pDlgCGStatement=NULL;
	pDlgShowGuidance=NULL;
	pDlgShowData=NULL;

	index_pause=0;
	index_run=0;
	nID=0;
	nStyle=0;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	SetMenu(NULL);    //2010.4.28  delete the menu

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnSetFocus(CWnd* pOldWnd) 
{
	CFrameWnd::OnSetFocus(pOldWnd);

	CToolBarCtrl &toolctrl=m_wndToolBar.GetToolBarCtrl();         //get the pointer of Toolbar

	int index;
	int iMage;
	index_run=toolctrl.AddBitmap(2,IDB_RUN);
	index_pause=toolctrl.AddBitmap(2,IDB_PAUSE);                                                           //load BMP
	index=m_wndToolBar.CommandToIndex(ID_RUN);                                                      //Get Index of button ID_RUN
	m_wndToolBar.GetButtonInfo(index,nID,nStyle,iMage);                                               //Get button information
	m_wndToolBar.SetButtonInfo(index,nID,nStyle,index_run);                                          //Set the button
}

void CMainFrame::Change_Button_Surface(int pressed)   
{
	int index;
	index=m_wndToolBar.CommandToIndex(ID_RUN);         //Get Index of button ID_RUN
	pressed= pressed%2;
	switch (pressed)
	{
		case 0:
			m_wndToolBar.SetButtonInfo(index,nID,nStyle,index_run);   //Set the button
			break;
		case 1:
			m_wndToolBar.SetButtonInfo(index,nID,nStyle,index_pause);
			break;
	}
}


void CMainFrame::OnSwitchDetector() 
{
	if (!Check_Load_or_Not())
		return;

//	st_DetectorSwitch=!st_DetectorSwitch;

	extern int Start_Time_Detector;
	extern bool Switch_Detector;

	if (Switch_Detector==false)   //start to provide traffic flow data collected by detectors
	{
		Start_Time_Detector= simu_time;
		Switch_Detector=true;
	} 
	else  //stop
	{
		Start_Time_Detector= 0;
		Switch_Detector=false;
	}
}

void CMainFrame::OnSwitchGuidance() 
{
	if (!Check_Load_or_Not())
		return;

//	st_GuidanceSwitch=!st_GuidanceSwitch;

	extern int Start_Time_Detector;
	
	if (Switch_Guidance==false)   //start to guide
	{
//		Start_Time_Guidance= simu_time+1;  
		Start_Time_Guidance= Start_Time_Detector;
		Switch_Guidance=true;
	} 
	else  //stop
	{
		Start_Time_Guidance= 0;
		Switch_Guidance=false;
	}	
	Invalidate();
}

bool Switch_Show_Guidance=false;
void CMainFrame::OnSwitchShowGuidance()
{
	if (!Check_Load_or_Not())
		return;

	Switch_Show_Guidance=!Switch_Show_Guidance;
}

void CMainFrame::OnSwitchPointControl() 
{
	if (!Check_Load_or_Not())
		return;

	///////////////////////////////
	st_RegionControlSwitch=false;
	st_PointControlSwitch=!st_PointControlSwitch;
	///////////////////////////////

	Switch_Point_Control=!Switch_Point_Control;

	extern CSimuFun *simuFun;
	extern char Current_Control_Type;

	if (Switch_Point_Control==true)  //point control
	{
		Switch_Region_Control=false;
		Switch_No_Control= false;
		Current_Control_Type= Get_Current_Control_Type();
//		simuFun->Set_Controller_State(Current_Control_Type, simu_time+1);  
	} 
	else  //point control: grow out of nothing
	{
		Switch_Region_Control=false;
		Switch_No_Control= true;
		Current_Control_Type= Get_Current_Control_Type();
//		simuFun->Set_Controller_State(Current_Control_Type,simu_time+1);
	}
}

void CMainFrame::OnSwitchRegionControl() 
{
	if (!Check_Load_or_Not())
		return;

	///////////////////////////////
	st_RegionControlSwitch=!st_RegionControlSwitch;
	///////////////////////////////

	Switch_Region_Control=!Switch_Region_Control;

	extern CSimuFun *simuFun;
	extern char Current_Control_Type;

	//refer to OnSwitchPointControl()
	if (Switch_Region_Control==true)  
	{
		Switch_Point_Control=false;
		Switch_No_Control= false;
		Current_Control_Type= Get_Current_Control_Type();
		simuFun->Set_Controller_State(Current_Control_Type, simu_time+1);

	} 
	else  ////region control: out of nothing: only to no control but not point control
	{
		Switch_Point_Control=false;
		Switch_No_Control= true;
		Current_Control_Type= Get_Current_Control_Type();
		simuFun->Set_Controller_State(Current_Control_Type, simu_time+1);

	}	
}

char CMainFrame::Get_Current_Control_Type()
{
	if (Switch_Point_Control==false && Switch_Region_Control==false)
	{
		return 'N';
	}
	else if (Switch_No_Control==false && Switch_Point_Control==true && Switch_Region_Control==false)
	{
		return 'P';
	}
	else if (Switch_No_Control==false && Switch_Point_Control==false && Switch_Region_Control==true)
	{
		return 'R';
	}
	else
	{
		AfxMessageBox("Control strategy wrong");
		return 'o';
	}
}

void CMainFrame::OnDisplayGraph()
{
	Display_Flag=!Display_Flag;
	Invalidate();
}

void CMainFrame::OnUpdateDisplayGraph(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(Display_Flag);
}

void CMainFrame::OnUpdateSwitchPointControl(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(st_PointControlSwitch);	
}

void CMainFrame::OnUpdateSwitchRegionControl(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(st_RegionControlSwitch);	
}

void CMainFrame::OnUpdateSwitchGuidance(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(Switch_Guidance);
}

void CMainFrame::OnUpdateSwitchShowGuidance(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(Switch_Show_Guidance);
}

void CMainFrame::OnUpdateSwitchDetector(CCmdUI* pCmdUI) 
{
	extern bool Switch_Detector;
	pCmdUI->SetCheck(Switch_Detector);	
}


void CMainFrame::OnControlView() 
{
	if (Show_Control_View_Flag==true)
		if(pDlgShowControl!=NULL)
		{
			pDlgShowControl->EndDialog(0);
			Show_Control_View_Flag=false;
			return;
		}
	pDlgShowControl= new CDlgShowControl();
	pDlgShowControl->Create(IDD_DLG_SHOW_CONTROL);
	pDlgShowControl->RefreshData();
	pDlgShowControl->ShowWindow(SW_SHOW);
	Show_Control_View_Flag=true;
	
}

void CMainFrame::OnShowData() 
{
	if (Show_Data_Flag==true)
		if(pDlgShowData!=NULL)
		{
			pDlgShowData->EndDialog(0);
			Show_Data_Flag=false;
			return;
		}
	pDlgShowData=new CDlgShowData();
	pDlgShowData->Create(IDD_DLG_SHOW_DATA);
	pDlgShowData->RefreshData();
	pDlgShowData->ShowWindow(SW_SHOW);
	Show_Data_Flag=true;
}

void CMainFrame::OnGuidanceView() 
{
	if (Show_Guidance_View_Flag==true)

		if(pDlgShowGuidance!=NULL)
		{
			pDlgShowGuidance->EndDialog(0);
			Show_Guidance_View_Flag=false;
			return;
		}
	pDlgShowGuidance= new CDlgShowGuidance();
	pDlgShowGuidance->Create(IDD_DLG_SHOW_GUIDANCE);
	pDlgShowGuidance->RefreshData();
	pDlgShowGuidance->ShowWindow(SW_SHOW);
	Show_Guidance_View_Flag=true;	
	
}

void CMainFrame::OnCGStatement() 
{
	if (Show_GS_Statement_Flag==true)
		if(pDlgCGStatement!=NULL)
		{
			pDlgCGStatement->EndDialog(0);
			Show_GS_Statement_Flag=false;
			return;
		}

	pDlgCGStatement= new CDlgCGStatement();
	pDlgCGStatement->Create(IDD_CG_STATEMENT);
	pDlgCGStatement->RefreshData();
	pDlgCGStatement->ShowWindow(SW_SHOW);
	Show_GS_Statement_Flag=true;
}

void CMainFrame::OnUpdateShowData(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(Show_Data_Flag);	
}

void CMainFrame::OnUpdateControlView(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(Show_Control_View_Flag);	
}

void CMainFrame::OnUpdateGuidanceView(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(Show_Guidance_View_Flag);
}

void CMainFrame::OnUpdateCGStatement(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(Show_GS_Statement_Flag);	
}

void CMainFrame::OnUpdatePlotMFD(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(Show_PlotMFD_Flag);
}

void CMainFrame::OnUpdatePlotTrajectory(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(Show_PlotTrajectory_Flag);
}

void CMainFrame::OnUpdateStartYellowIntersection(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(Show_Yellow_Intersection_Flag);
}

void CMainFrame::OnUpdateConflictBotton(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(Show_Conflict_Botton_Flag);
}

void CMainFrame::OnUpdateTimeShortestPath(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(Show_Time_Shortest_Path);
}


//if or not show these dialog with the starting of program.
void CMainFrame::Show_All_Dialog()
{
	OnShowData();
//	OnCGStatement();
//	OnControlView();
//	OnGuidanceView(); 
}

void CMainFrame::OnPlotMFD()
{	
	if (!Check_Load_or_Not())
		return;

	Show_PlotMFD_Flag=!Show_PlotMFD_Flag;
	pPlotMFD= new CPlotMFD();			
	pPlotMFD->Create(IDD_MFD);
	pPlotMFD->ShowWindow(SW_SHOW);

}

void CMainFrame::OnPlotTrajectory()
{
	if (!Check_Load_or_Not())
		return;

	Show_PlotTrajectory_Flag=!Show_PlotTrajectory_Flag;
	pPlotTrajectory= new CPlotTrajectory();
	pPlotTrajectory->Create(IDD_PLOT_TRAJECTORY);
	pPlotTrajectory->ShowWindow(SW_SHOW);
}

void CMainFrame::OnStartSetting()
{
	if (!Check_Load_or_Not())
		return;

	CDlgSetting *pDlgSetting=new CDlgSetting();
	pDlgSetting->Create(IDD_DLG_SETTING);
	pDlgSetting->ShowWindow(SW_SHOW);
}

void CMainFrame::OnStartCarMovingModel()
{
	if (Open_File_PathName!="N")
	{
		AfxMessageBox("The setting can't be changed after the network is loaded");
		return;
	}

	CDlgCarMovingModels *pDlgCarMovingModels=new CDlgCarMovingModels();
	pDlgCarMovingModels->Create(IDD_CAR_MOVING_MODEL);
	pDlgCarMovingModels->ShowWindow(SW_SHOW);
}


void CMainFrame::OnStartSetDayToDay()
{
	
	if (Open_File_PathName!="N")
	{
		AfxMessageBox("The setting can't be changed after the network is loaded");
		return;
	}
	

	CDlgDay2day *pDlgDay2day=new CDlgDay2day();
	pDlgDay2day->Create(IDD_SET_DAY_TO_DAY);
	pDlgDay2day->ShowWindow(SW_SHOW);
}


void CMainFrame::OnStartYellowIntersection()
{
	if (!Check_Load_or_Not())
		return;

	Show_Yellow_Intersection_Flag=!Show_Yellow_Intersection_Flag;
	Yellow_Intersection=!Yellow_Intersection;
}

void CMainFrame::OnStartConflict()
{
	if (!Check_Load_or_Not())
		return;

	Show_Conflict_Botton_Flag=!Show_Conflict_Botton_Flag;
	Conflict_Flag=!Conflict_Flag;
}

void CMainFrame::OnTimeShortestPath()
{
	Show_Time_Shortest_Path=!Show_Time_Shortest_Path;

/*
	//when time shortest path is closed, information is not ready, but if shortest path is chosen, which doesn't mean information is ready
	extern CSimuFun *simuFun;
	if (Show_Time_Shortest_Path==false)
		simuFun->Information_Ready=false;
*/
}




void CMainFrame::OnClose()
{
	if (simu_time!=Total_Simulation_Time-1)
	{
		//record MFD, if one wants to record MFD, uncomment it	
		if (Output_MFD)
		{
			pSampleCollection->Record_MFD();
			pSampleCollection->Empty_MFD_Record();
		}

		if (Output_DepartureArrivalRate)
		{
			pSampleCollection->Record_Departure_Arrival_Rate();
			pSampleCollection->Empty_MFD_Record();
		}
		
		if (Output_SpatialDistribution)
		{
			pSampleCollection->Record_Spatial_Distribution();
			pSampleCollection->Empty_SSD_Record();
		}

		//record evolution of one commuter's route choices
		//NOTE: the first line (day 0) is the fftt of links
		if (Output_RouteChoice)
		{
			pSampleCollection->Record_Commuter_Choices(current_day, 18,0,0);
			pSampleCollection->Record_Commuter_Choices(current_day, 18,0,1);
			pSampleCollection->Record_Commuter_Choices(current_day, 18,0,2);
//			pSampleCollection->Record_Commuter_Choices(current_day, 18,0,3);
//			pSampleCollection->Record_Commuter_Choices(current_day, 18,0,5);
//			pSampleCollection->Record_Commuter_Choices(current_day, 18,0,6);
		}

//		err->FlushAllFiles();

	}

	Switch_Thread=false;	
	Sleep(1000);

	//close
	err->CloseAllFiles();
	pSampleCollection=NULL;
	communicator->Clean_Socket();
	CFrameWnd::OnClose();

}




