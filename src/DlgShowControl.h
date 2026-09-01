#pragma once


#include "setting.h"

// CDlgShowControl dialog

class CDlgShowControl : public CDialog
{
	DECLARE_DYNAMIC(CDlgShowControl)

public:
	CDlgShowControl(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgShowControl();

// Dialog Data
	enum { IDD = IDD_DLG_SHOW_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString String_Box_for_Show;
	void RefreshData();
	void ResetData();
	void OnClose();
	LRESULT OnUpdateData(WPARAM wParam, LPARAM lParam);
};

