#pragma once
#include "resource.h"

// CDlgMessage dialog

class CDlgMessage : public CDialog
{
	DECLARE_DYNAMIC(CDlgMessage)

public:
	CDlgMessage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgMessage();

// Dialog Data
	enum { IDD = IDD_MESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_Message;
	void Init_Dialog(CString str);
};
