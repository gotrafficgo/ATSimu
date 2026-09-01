// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__17ED8027_2548_4B02_A014_AA4253C3739D__INCLUDED_)
#define AFX_MAINFRM_H__17ED8027_2548_4B02_A014_AA4253C3739D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DlgShowData.h"
#include "DlgShowControl.h"
#include "DlgCGStatement.h"
#include "DlgShowGuidance.h"
#include "PlotMFD.h"
#include "PlotTrajectory.h"

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	//}}AFX_VIRTUAL

// Implementation
public:
	char Get_Current_Control_Type();
	virtual ~CMainFrame();
	////////////////////////
	CDlgShowData *pDlgShowData;
	CDlgShowControl *pDlgShowControl;
	CDlgShowGuidance *pDlgShowGuidance;
	CDlgCGStatement *pDlgCGStatement;
	CPlotMFD *pPlotMFD;
	CPlotTrajectory *pPlotTrajectory;

	bool Show_Data_Flag;
	bool Show_Control_View_Flag;
	bool Show_Guidance_View_Flag;
	bool Show_GS_Statement_Flag;
	bool Show_PlotMFD_Flag;
	bool Show_PlotTrajectory_Flag;
	bool Show_Yellow_Intersection_Flag;
	bool Show_Conflict_Botton_Flag;
	bool Show_Time_Shortest_Path;

	bool st_PointControlSwitch;
	bool st_RegionControlSwitch;
	bool st_NoControlSwitch;

	bool Switch_Point_Control;
	bool Switch_Region_Control;
	bool Switch_No_Control;

	/////////////////////////
	//switch of guidance
//	bool Switch_Guidance;
//	int Start_Time_Guidance;
	
	/////////////////////////
	//switch of detector
// 	bool Switch_Detector;
// 	int Start_Time_Detector;

	//change the surface of bottom
	int index_pause;
	int index_run;
	unsigned int nStyle;
	unsigned int nID;
	void Change_Button_Surface(int pressed); 
	void Show_All_Dialog();
	void Update_Dialog_Data();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd); 
	afx_msg void OnSwitchDetector();
	afx_msg void OnSwitchGuidance();
	afx_msg void OnSwitchShowGuidance();
	afx_msg void OnSwitchPointControl();
	afx_msg void OnSwitchRegionControl();
	afx_msg void OnPlotMFD();
	afx_msg void OnPlotTrajectory();
	afx_msg void OnStartYellowIntersection();
	afx_msg void OnStartConflict();
	afx_msg void OnTimeShortestPath();
	afx_msg void OnStartSetting();
	afx_msg void OnStartCarMovingModel();
	afx_msg void OnStartSetDayToDay();
	afx_msg void OnDisplayGraph();
	afx_msg void OnUpdatePlotMFD(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePlotTrajectory(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStartYellowIntersection(CCmdUI *pCmdUI);
	afx_msg void OnUpdateConflictBotton(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTimeShortestPath(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDisplayGraph(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSwitchGuidance(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSwitchShowGuidance(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSwitchDetector(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSwitchPointControl(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSwitchRegionControl(CCmdUI* pCmdUI);
	afx_msg void OnShowData();
	afx_msg void OnControlView();
	afx_msg void OnUpdateShowData(CCmdUI* pCmdUI);
	afx_msg void OnUpdateControlView(CCmdUI* pCmdUI);
	afx_msg void OnGuidanceView();
	afx_msg void OnUpdateGuidanceView(CCmdUI* pCmdUI);
	afx_msg void OnCGStatement();
	afx_msg void OnUpdateCGStatement(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__17ED8027_2548_4B02_A014_AA4253C3739D__INCLUDED_)
