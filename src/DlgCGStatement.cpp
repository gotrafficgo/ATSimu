// DlgCGStatement.cpp : implementation file
//

#include "stdafx.h"
#include "traffic.h"
#include "DlgCGStatement.h"
#include "MainFrm.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgCGStatement dialog


CDlgCGStatement::CDlgCGStatement(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCGStatement::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCGStatement)
	m_Control_Statement = _T("");
	m_Detector_Statement = _T("");
	m_Guidance_Statement = _T("");
	//}}AFX_DATA_INIT
}


void CDlgCGStatement::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCGStatement)
	DDX_Text(pDX, IDC_CONTROL_STATEMENT, m_Control_Statement);
	DDX_Text(pDX, IDC_DETECTOR_STATEMENT, m_Detector_Statement);
	DDX_Text(pDX, IDC_GUIDANCE_STATEMENT, m_Guidance_Statement);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCGStatement, CDialog)
	//{{AFX_MSG_MAP(CDlgCGStatement)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_MESSAGE(WM_UPDATEDATA, OnUpdateData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCGStatement message handlers
void CDlgCGStatement::RefreshData()
{
	extern char Current_Control_Type;
	extern bool Switch_Guidance;
	extern bool Switch_Detector;
	////////////////////////////
	//the state of control and guidance
	switch (Current_Control_Type)
	{
	case 'N':
		m_Control_Statement= "no control";
		break;
	case 'P':
		m_Control_Statement= "point control";
		break;
	case 'R':
		m_Control_Statement= "region control";
		break;
	}
	
	extern bool Switch_Show_Guidance;
	if (Switch_Guidance==true && Switch_Show_Guidance==true)
		m_Guidance_Statement ="on and show";
	if (Switch_Guidance==true && Switch_Show_Guidance==false)
		m_Guidance_Statement ="on an no show";
	 if (Switch_Guidance==false)
		m_Guidance_Statement ="down";

	if (Switch_Detector==true)
		m_Detector_Statement= "turn on";
	else
		m_Detector_Statement= "turn down";
	SendMessage(WM_UPDATEDATA, FALSE);
}

void CDlgCGStatement::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	CMainFrame *pMainFrame= (CMainFrame *)AfxGetApp()->m_pMainWnd;   
	pMainFrame->Show_GS_Statement_Flag=false; 
	CDialog::OnClose();
}


LRESULT CDlgCGStatement::OnUpdateData(WPARAM wParam, LPARAM lParam)
{
	UpdateData(wParam);
	return 0;
}