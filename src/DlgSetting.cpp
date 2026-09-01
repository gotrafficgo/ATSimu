// DlgSetting.cpp : implementation file
//

#include "stdafx.h"
#include "traffic.h"
#include "DlgSetting.h"
#include "Global.h"

// CDlgSetting dialog

IMPLEMENT_DYNAMIC(CDlgSetting, CDialog)

CDlgSetting::CDlgSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSetting::IDD, pParent)
	, m_Show_Value(0)
	, m_Min_Value(0)
	, m_Max_Value(0)
	, m_hour(0)
	, m_minute(0)
	, m_second(0)
	, m_Check_Slider_Demand(FALSE)
{
	Min_Value=0;
	Max_Value=60;
}

CDlgSetting::~CDlgSetting()
{
}

void CDlgSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_DEMAND, m_Slider_Demand);
	DDX_Text(pDX, IDC_SHOW_VALUE, m_Show_Value);
	DDV_MinMaxInt(pDX, m_Show_Value, Min_Value, Max_Value);
	DDX_Text(pDX, IDC_MIN_VALUE, m_Min_Value);
	DDX_Text(pDX, IDC_MAX_VALUE, m_Max_Value);
	DDX_Control(pDX, IDC_SIMU_SPEED, m_Slider_Simu_Speed);
	DDX_Text(pDX, IDC_HOUR, m_hour);
	DDV_MinMaxInt(pDX, m_hour, 0, 23);
	DDX_Text(pDX, IDC_MINUTE, m_minute);
	DDV_MinMaxInt(pDX, m_minute, 0, 59);
	DDX_Text(pDX, IDC_SECOND, m_second);
	DDV_MinMaxInt(pDX, m_second, 0, 59);
	DDX_Check(pDX, IDC_CHECK_SLIDER_DEMAND, m_Check_Slider_Demand);
}


BEGIN_MESSAGE_MAP(CDlgSetting, CDialog)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_DEMAND, &CDlgSetting::OnNMCustomdrawSliderDemand)
	ON_EN_CHANGE(IDC_SHOW_VALUE, &CDlgSetting::OnEnChangeShowValue)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SIMU_SPEED, &CDlgSetting::OnNMCustomdrawSimuSpeed)
	ON_EN_CHANGE(IDC_HOUR, &CDlgSetting::OnEnChangeHour)
	ON_BN_CLICKED(IDC_CHECK_SLIDER_DEMAND, &CDlgSetting::OnBnClickedCheckSliderDemand)
END_MESSAGE_MAP()


// CDlgSetting message handlers

BOOL CDlgSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	extern int Start_Simu_Time;
	m_hour=int(Start_Simu_Time/3600);
	m_minute=int((Start_Simu_Time%3600)/60);
	m_second=(Start_Simu_Time%3600)%60;

	m_Slider_Demand.SetRange(Min_Value,Max_Value);
	m_Slider_Demand.SetPos(Current_Demand);
	m_Slider_Demand.SetTicFreq(10);
	m_Min_Value=Min_Value;
	m_Max_Value=Max_Value;

	extern int Sleep_Time;
	m_Slider_Simu_Speed.SetRange(100, 1000);
	m_Slider_Simu_Speed.SetPos(Sleep_Time+100);

	this->m_Check_Slider_Demand=Slider_Demand_or_Not;

	UpdateData(FALSE); 
	
	return TRUE;  
}

void CDlgSetting::OnNMCustomdrawSliderDemand(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(TRUE);
	m_Show_Value=m_Slider_Demand.GetPos();
	Current_Slider_Demand=m_Show_Value;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDlgSetting::OnEnChangeShowValue()
{
	UpdateData(TRUE);
	m_Slider_Demand.SetPos(m_Show_Value);
	Current_Slider_Demand=m_Show_Value;
	UpdateData(FALSE); 
}

void CDlgSetting::OnNMCustomdrawSimuSpeed(NMHDR *pNMHDR, LRESULT *pResult)
{
	extern int Sleep_Time;
	int pos;
	UpdateData(TRUE);
	pos= m_Slider_Simu_Speed.GetPos();
	Sleep_Time=pos-100;
	UpdateData(FALSE); 
	*pResult = 0;
}

void CDlgSetting::OnEnChangeHour()
{
	UpdateData(TRUE);
	extern int Start_Simu_Time;
	Start_Simu_Time= m_hour*3600+	m_minute*60 +m_second;
	UpdateData(FALSE);
}

void CDlgSetting::OnBnClickedCheckSliderDemand()
{
	// TODO: Add your control notification handler code here
	extern bool Slider_Demand_or_Not;
	Slider_Demand_or_Not = !Slider_Demand_or_Not;	
}
