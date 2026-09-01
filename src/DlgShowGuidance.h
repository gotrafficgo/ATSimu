#pragma once

#include "setting.h"

// CDlgShowGuidance dialog

class CDlgShowGuidance : public CDialog
{
	DECLARE_DYNAMIC(CDlgShowGuidance)

public:
	CDlgShowGuidance(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgShowGuidance();

// Dialog Data
	enum { IDD = IDD_DLG_SHOW_GUIDANCE };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_Start_Time;
	int m_Received_Guidance_Number;
	CString m_Guidance_Strategies;

	void RefreshData();
	void OnClose();
	LRESULT OnUpdateData(WPARAM wParam, LPARAM lParam);
};
