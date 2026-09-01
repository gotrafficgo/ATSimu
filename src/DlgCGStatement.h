#if !defined(AFX_DLGCGSTATEMENT_H__1F160E10_DDDA_4CEC_B027_976C21A59AC3__INCLUDED_)
#define AFX_DLGCGSTATEMENT_H__1F160E10_DDDA_4CEC_B027_976C21A59AC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCGStatement.h : header file
//

#include "setting.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgCGStatement dialog

class CDlgCGStatement : public CDialog
{
// Construction
public:
	void RefreshData();
	LRESULT OnUpdateData(WPARAM wParam, LPARAM lParam);
	CDlgCGStatement(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCGStatement)
	enum { IDD = IDD_CG_STATEMENT };
	CString	m_Control_Statement;
	CString	m_Detector_Statement;
	CString	m_Guidance_Statement;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCGStatement)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCGStatement)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCGSTATEMENT_H__1F160E10_DDDA_4CEC_B027_976C21A59AC3__INCLUDED_)
