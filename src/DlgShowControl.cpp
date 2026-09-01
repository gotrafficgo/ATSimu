// DlgShowControl.cpp : implementation file
//

#include "stdafx.h"
#include "traffic.h"
#include "DlgShowControl.h"
#include "setting.h"
#include "Global.h"
#include "MainFrm.h"

// CDlgShowControl dialog

IMPLEMENT_DYNAMIC(CDlgShowControl, CDialog)

CDlgShowControl::CDlgShowControl(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgShowControl::IDD, pParent)
	, String_Box_for_Show(_T(""))
{
	ResetData();
}

CDlgShowControl::~CDlgShowControl()
{
}

void CDlgShowControl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SHOW_CONTROL, String_Box_for_Show);
}


BEGIN_MESSAGE_MAP(CDlgShowControl, CDialog)
	ON_MESSAGE(WM_UPDATEDATA, OnUpdateData)
END_MESSAGE_MAP()


// CDlgShowControl message handlers

void CDlgShowControl::ResetData()
{
	String_Box_for_Show="";
}

void CDlgShowControl::RefreshData()
{
	int i;
	extern char Current_Control_Type;
	CString str;

	ResetData();

	if (Current_Control_Type=='N')
	{
		String_Box_for_Show="";
		SendMessage(WM_UPDATEDATA, FALSE);
		return;
	}

	for (i=0;i<MAX_CROSS_NUMBER;i++)
	{
		if (Cross_Array[i]!=NULL && Cross_Array[i]->Controller!=NULL )
		{
			str.Format("Intersection%d: %d\r\n",  Cross_Array[i]->Object_ID  , Cross_Array[i]->Controller->Current_Control_Info.Cycle_Time);
			String_Box_for_Show+=str;
		}
	}
	SendMessage(WM_UPDATEDATA, FALSE);
}

void CDlgShowControl::OnClose()
{
	CMainFrame *pMainFrame= (CMainFrame *)AfxGetApp()->m_pMainWnd;   
	pMainFrame->Show_Control_View_Flag=false; 
	CDialog::OnClose();
}

LRESULT CDlgShowControl::OnUpdateData(WPARAM wParam, LPARAM lParam)
{
	UpdateData(wParam);
	return 0;
}