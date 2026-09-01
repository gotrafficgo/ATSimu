#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDlgSetting dialog

class CDlgSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgSetting)

public:
	CDlgSetting(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSetting();

// Dialog Data
	enum { IDD = IDD_DLG_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_Slider_Demand;
	CSliderCtrl m_Slider_Simu_Speed;
	int Min_Value;
	int Max_Value;
	int m_Show_Value;
	int m_Min_Value;
	int m_Max_Value;
public:
	afx_msg void OnNMCustomdrawSliderDemand(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeShowValue();
	afx_msg void OnNMCustomdrawSimuSpeed(NMHDR *pNMHDR, LRESULT *pResult);

	BOOL OnInitDialog();
	int m_hour;
	int m_minute;
	int m_second;
	afx_msg void OnEnChangeHour();
	afx_msg void OnBnClickedCheckSliderDemand();
	BOOL m_Check_Slider_Demand;
};
