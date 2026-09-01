#if !defined(AFX_DLGSHOWDATA_H__72BD32A8_62F8_4E71_8FB3_8680C129B3AE__INCLUDED_)
#define AFX_DLGSHOWDATA_H__72BD32A8_62F8_4E71_8FB3_8680C129B3AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "setting.h"

// DlgShowData.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgShowData dialog

class CDlgShowData : public CDialog
{
// Construction
public:
	void RefreshData();
	CString ConvertTime(int Time);
	LRESULT OnUpdateData(WPARAM wParam, LPARAM lParam);
	CDlgShowData(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgShowData)
	enum { IDD = IDD_DLG_SHOW_DATA };
	int		m_simu_time;
	int		m_veh_number;
	int		m_total_veh_number;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgShowData)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgShowData)
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CString m_simu_time_2;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSHOWDATA_H__72BD32A8_62F8_4E71_8FB3_8680C129B3AE__INCLUDED_)
