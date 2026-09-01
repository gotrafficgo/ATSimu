#pragma once


// CDlgCarMovingModels dialog

class CDlgCarMovingModels : public CDialog
{
	DECLARE_DYNAMIC(CDlgCarMovingModels)

public:
	CDlgCarMovingModels(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgCarMovingModels();

// Dialog Data
	enum { IDD = IDD_CAR_MOVING_MODEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	BOOL OnInitDialog();
	char BnID_To_CFChar(int Button_ID);
	int CFChar_To_BnID(char Type);
	char BnID_To_CMChar(int Button_ID);
	int CMChar_To_BnID(char Type);
	int m_CF_Model;
	int m_Car_Moving_Model;
	double m_P;
	double m_P2;
	afx_msg void OnBnClickedOk();
	void Reset_Cell_Length(char Type);
	afx_msg void OnBnClickedCancel();
};
