// DlgGuidanceBoard.cpp : implementation file
//

#include "stdafx.h"
#include "traffic.h"
#include "DlgGuidanceBoard.h"
#include "Global.h"

// CDlgGuidanceBoard dialog

IMPLEMENT_DYNAMIC(CDlgGuidanceBoard, CDialog)

CDlgGuidanceBoard::CDlgGuidanceBoard(int Link_ID, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGuidanceBoard::IDD, pParent)
	, m_Guidance_Strategies(_T(""))
	, m_Start_Time(-1)
	, m_Received_Guidance_Number(-1)
{
	this->Link_ID= Link_ID;
}

CDlgGuidanceBoard::~CDlgGuidanceBoard()
{
}

void CDlgGuidanceBoard::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_GUIDANCE_STRATEGY, m_Guidance_Strategies);
	DDX_Text(pDX, IDC_GUIDANCE_START_TIME, m_Start_Time);
	DDX_Text(pDX, IDC_RECEIVE_GUIDANCE_NUMBER, m_Received_Guidance_Number);
}


BEGIN_MESSAGE_MAP(CDlgGuidanceBoard, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CDlgGuidanceBoard::OnBnClickedCancel)
	ON_MESSAGE(WM_UPDATEDATA, OnUpdateData)
END_MESSAGE_MAP()


// CDlgGuidanceBoard message handlers



void CDlgGuidanceBoard::RefreshData()
{
	extern bool Switch_Guidance;
	extern int Received_Guidance_Number;
	extern int Start_Time_Guidance;
	extern CCommunicator *communicator;


	///////////////////////////////////////////////////////////
	if (Switch_Guidance==false)
	{
		m_Start_Time= -1;
		m_Received_Guidance_Number=-1;
		SendMessage(WM_UPDATEDATA, FALSE);
		return;
	}

	//////////////////////////////////////////////////////////
	int Cycle_Time;
	int Last_Refresh_Time;
	int Dest_Size;
	int Dest_ID;
	float fLeft;
	float fStraight;
	float fRight;

	CString str_String="------------------";
	CString str_New_Line="\r\n";
	CString str_Star="**************";
	CString str_Space="   ";
	CString str_Link_ID;
	CString str_Cycle_Time;
	CString str_Last_Refresh_Time;
	CString str_Dest_Size;
	CString str_Dest_ID;
	CString str_fLeft;
	CString str_fStraight;
	CString str_fRight;

	m_Start_Time= Start_Time_Guidance;
	m_Received_Guidance_Number= Received_Guidance_Number;

	m_Guidance_Strategies="";

	Cycle_Time= Link_Array[Link_ID]->guidance->Current_Guidance_Info.This_Times;
	Last_Refresh_Time=  Link_Array[Link_ID]->guidance->Last_Refresh_Time;
	Dest_Size= Link_Array[Link_ID]->guidance->Current_Guidance_Info.Guidance_Dest_Size;
	str_Cycle_Time.Format("%d", (int)(Cycle_Time/GUIDANCE_CIRCLE));
	str_Last_Refresh_Time.Format("%d", Last_Refresh_Time);
	str_Link_ID.Format("%d", Link_ID);
	m_Guidance_Strategies= str_String+str_New_Line
		+ "道路号: "+ str_Link_ID	+str_New_Line
		+ "当前周期: "+str_Cycle_Time + str_New_Line
		+ "上次更新时间: "+str_Last_Refresh_Time +str_New_Line;

	for (int j=0; j<Dest_Size; j++)
	{
		Dest_ID= Link_Array[Link_ID]->guidance->Current_Guidance_Info.Guidance_Dest[j];
		fLeft= (float)Link_Array[Link_ID]->guidance->Current_Guidance_Info.Turn_Rate[j][0]/100;                //左
		fStraight= (float)Link_Array[Link_ID]->guidance->Current_Guidance_Info.Turn_Rate[j][1]/100;         //直
		fRight= (float)Link_Array[Link_ID]->guidance->Current_Guidance_Info.Turn_Rate[j][2]/100;             //右

		str_Dest_ID.Format("%d", Dest_ID);
		str_fLeft.Format("%.2f", fLeft);
		str_fStraight.Format("%.2f", fStraight);
		str_fRight.Format("%.2f", fRight);

		m_Guidance_Strategies= m_Guidance_Strategies+     str_Space+str_Star+ str_New_Line;
		m_Guidance_Strategies= m_Guidance_Strategies+     str_Space+ "目标道路: "+ str_Dest_ID + str_New_Line;
		m_Guidance_Strategies= m_Guidance_Strategies+     str_Space+ "左转率: " + str_fLeft	 +str_New_Line ;
		m_Guidance_Strategies= m_Guidance_Strategies+     str_Space+ "直行率: " + str_fStraight	 +str_New_Line ;
		m_Guidance_Strategies= m_Guidance_Strategies+     str_Space+ "右转率: " + str_fRight +str_New_Line ;
	}

	SendMessage(WM_UPDATEDATA, FALSE);

}



// void CDlgGuidanceBoard::OnBnClickedOk()
// {
// 	OnOK();
// }

void CDlgGuidanceBoard::OnBnClickedCancel()
{
	Link_Array[Link_ID]->isShowGuidanceBoard=false;
	Link_Array[Link_ID]->dlgGuidanceBoard=NULL;

	OnCancel();
}


LRESULT CDlgGuidanceBoard::OnUpdateData(WPARAM wParam, LPARAM lParam)
{
	UpdateData(wParam);
	return 0;
}