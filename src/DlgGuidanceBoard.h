#pragma once

#include "resource.h"
#include "setting.h"
// CDlgGuidanceBoard dialog


class CDlgGuidanceBoard : public CDialog
{
	DECLARE_DYNAMIC(CDlgGuidanceBoard)

public:
	CDlgGuidanceBoard(int Link_ID, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgGuidanceBoard();

// Dialog Data
	enum { IDD = IDD_DLG_GUIDANCE_BOARD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	int Link_ID;
	LRESULT OnUpdateData(WPARAM wParam, LPARAM lParam);
	void RefreshData();
	CString m_Guidance_Strategies;
	afx_msg void OnBnClickedCancel();
	int m_Start_Time;
	int m_Received_Guidance_Number;
};
