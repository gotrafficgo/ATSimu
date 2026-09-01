// DlgShowData.cpp : implementation file
//

#include "stdafx.h"
#include "traffic.h"
#include "DlgShowData.h"
#include "MainFrm.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgShowData dialog


CDlgShowData::CDlgShowData(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgShowData::IDD, pParent)
	, m_simu_time_2(_T(""))
{
	//{{AFX_DATA_INIT(CDlgShowData)
	m_simu_time = 0;
	m_simu_time_2="0";
	m_veh_number = 0;
	m_total_veh_number = 0;
	//}}AFX_DATA_INIT
}


void CDlgShowData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgShowData)
	DDX_Text(pDX, IDC_SIMU_TIME, m_simu_time);
	DDX_Text(pDX, IDC_CURRENT_VEH_NUMBER, m_veh_number);
	DDX_Text(pDX, IDC_TOTAL_VEH_NUMBER, m_total_veh_number);
	DDX_Text(pDX, IDC_SIMU_TIME_2, m_simu_time_2);
	//}}AFX_DATA_MAP
	
}


BEGIN_MESSAGE_MAP(CDlgShowData, CDialog)
	//{{AFX_MSG_MAP(CDlgShowData)
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_MESSAGE(WM_UPDATEDATA, OnUpdateData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgShowData message handlers

int CDlgShowData::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CDlgShowData::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgShowData::RefreshData()
{
	extern int Total_Veh_Number;
	extern int Out_Veh_Number;
	extern char Current_Control_Type;

	/////////////////////////////
	extern int simu_time;
	extern int Start_Simu_Time;
	m_simu_time= simu_time;
	m_simu_time_2=ConvertTime(simu_time+Start_Simu_Time);
	m_total_veh_number= Total_Veh_Number;
	m_veh_number= Total_Veh_Number- Out_Veh_Number;

	SendMessage(WM_UPDATEDATA, FALSE);
}

CString CDlgShowData::ConvertTime(int Time)
{
	CString strTime;
	CString str;

	int hor=int(Time/3600);
	int min=int((Time%3600)/60);
	int sec=(Time%3600)%60;

	str.Format("%d", hor);
	strTime=str+":";

	str.Format("%d", min);
	strTime=strTime+str+":";

	str.Format("%d",sec);
	strTime=strTime+str;

	return strTime;
}


void CDlgShowData::OnClose(void)
{
	CMainFrame *pMainFrame= (CMainFrame *)AfxGetApp()->m_pMainWnd;   
	pMainFrame->Show_Data_Flag=false; 

	CDialog::OnClose();
}



LRESULT CDlgShowData::OnUpdateData(WPARAM wParam, LPARAM lParam)
{
	UpdateData(wParam);
	return 0;
}