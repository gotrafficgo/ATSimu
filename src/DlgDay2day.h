#pragma once


// CDlgDay2day dialog

class CDlgDay2day : public CDialog
{
	DECLARE_DYNAMIC(CDlgDay2day)

public:
	CDlgDay2day(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgDay2day();

// Dialog Data
	enum { IDD = IDD_SET_DAY_TO_DAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_commuter_number;
	int m_day_number;
	int m_day_length;
	int m_arrival_time;
	int m_commuter_type;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	BOOL OnInitDialog();
};
