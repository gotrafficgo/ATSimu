// DlgDay2day.cpp : implementation file
//

#include "stdafx.h"
#include "traffic.h"
#include "DlgDay2day.h"
#include "Global.h"

// CDlgDay2day dialog

IMPLEMENT_DYNAMIC(CDlgDay2day, CDialog)

CDlgDay2day::CDlgDay2day(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDay2day::IDD, pParent)
	, m_commuter_number(0)
	, m_day_number(0)
	, m_day_length(0)
	, m_arrival_time(0)
	, m_commuter_type(0)
{

}

CDlgDay2day::~CDlgDay2day()
{
}

void CDlgDay2day::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_COMMUTER_NUMBER, m_commuter_number);
	DDX_Text(pDX, IDC_DAY_NUMBER, m_day_number);
	DDX_Text(pDX, IDC_DAY_LENGTH, m_day_length);
	DDX_Text(pDX, IDC_ARRIVAL_TIME, m_arrival_time);
	DDX_Text(pDX, IDC_COMMUTER_TYPE, m_commuter_type);
}


BEGIN_MESSAGE_MAP(CDlgDay2day, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgDay2day::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgDay2day::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgDay2day message handlers

void CDlgDay2day::OnBnClickedOk()
{
	UpdateData(TRUE);

	G_Commuter_Number_Per_Origin=m_commuter_number;
	G_Day_Length= m_day_length;
	G_Required_Arrival_Time= m_arrival_time;
	All_Commuters_Type= m_commuter_type;
	G_Simulated_Day_Number= m_day_number;

	OnOK();
}

void CDlgDay2day::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}


BOOL CDlgDay2day::OnInitDialog() 
{

	CDialog::OnInitDialog();

	m_commuter_number= G_Commuter_Number_Per_Origin;
	m_day_length=G_Day_Length;
	m_arrival_time=G_Required_Arrival_Time;
	m_commuter_type= All_Commuters_Type;
	m_day_number=G_Simulated_Day_Number;

	UpdateData(FALSE); 

	return TRUE;  
}