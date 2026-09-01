// traffic.h : main header file for the TRAFFIC application
//

#if !defined(AFX_TRAFFIC_H__733A1BE0_C6A2_442B_A429_AE79481D068E__INCLUDED_)
#define AFX_TRAFFIC_H__733A1BE0_C6A2_442B_A429_AE79481D068E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTrafficApp:
// See traffic.cpp for the implementation of this class
//

class CTrafficApp : public CWinApp
{
public:
	CTrafficApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrafficApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTrafficApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAFFIC_H__733A1BE0_C6A2_442B_A429_AE79481D068E__INCLUDED_)
