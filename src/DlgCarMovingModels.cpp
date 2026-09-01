// DlgCarMovingModels.cpp : implementation file
//

#include "stdafx.h"
#include "traffic.h"
#include "DlgCarMovingModels.h"
#include "Global.h"


// CDlgCarMovingModels dialog

IMPLEMENT_DYNAMIC(CDlgCarMovingModels, CDialog)

CDlgCarMovingModels::CDlgCarMovingModels(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCarMovingModels::IDD, pParent)
	, m_CF_Model(0)
	, m_Car_Moving_Model(0)
	, m_P(0)
	, m_P2(0)
{

}

CDlgCarMovingModels::~CDlgCarMovingModels()
{
}

void CDlgCarMovingModels::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_GIPPS, m_CF_Model);
	DDX_Radio(pDX, IDC_RADIO_CA, m_Car_Moving_Model);
	DDX_Text(pDX, IDC_VDR_P, m_P);
	DDV_MinMaxDouble(pDX, m_P, 0, 1);
	DDX_Text(pDX, IDC_VDR_P2, m_P2);
	DDV_MinMaxDouble(pDX, m_P2, 0, 1);
}


BEGIN_MESSAGE_MAP(CDlgCarMovingModels, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgCarMovingModels::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgCarMovingModels message handlers

BOOL CDlgCarMovingModels::OnInitDialog() 
{

	CDialog::OnInitDialog();

	m_P= CA_P;
	m_P2=CA_P2;
	m_CF_Model= CFChar_To_BnID(Car_Following_Model_Type);
	m_Car_Moving_Model= CMChar_To_BnID(Car_Moving_Model_Type);

	UpdateData(FALSE); 

	return TRUE;  
}

void CDlgCarMovingModels::Reset_Cell_Length(char Type)
{
	if (Type=='A')
	{
		Max_Lane_Cell_Number=DEFAULT_MAX_LANE_CELL_NUMBER_A;
		Meter_Per_Cell=DEFAULT_METER_PER_CELL_A;
		Pixel_Per_Cell=DEFAULT_CELL_LENGTH_A;
	} 
	else if(Type=='F')
	{
		Max_Lane_Cell_Number=DEFAULT_MAX_LANE_CELL_NUMBER_F;
		Meter_Per_Cell=DEFAULT_METER_PER_CELL_F;
		Pixel_Per_Cell=DEFAULT_CELL_LENGTH_F;
	}
}

//transform symbol of car following model to button id
int CDlgCarMovingModels::CFChar_To_BnID(char Type)
{
	switch (Type)
	{
	case 'G':
		return 0;
	case 'I':
		return 1;
	case 'N':
		return 2;
	}
	return -1;
}

//transform button id to symbol of car following model 
char CDlgCarMovingModels::BnID_To_CFChar(int Button_ID)
{
	switch (Button_ID)
	{
	case 0:
		return 'G';
	case 1:
		return 'I';
	case 2:
		return 'N';
	}
	return -1;
}

//transform symbol of car following model to button id
int CDlgCarMovingModels::CMChar_To_BnID(char Type)
{
	switch (Type)
	{
	case 'A':
		return 0;
	case 'F':
		return 1;
	}
	return -1;
}

//transform button id to symbol of car following model 
char CDlgCarMovingModels::BnID_To_CMChar(int Button_ID)
{
	switch (Button_ID)
	{
	case 0:
		return 'A';
	case 1:
		return 'F';
	}
	return -1;
}
void CDlgCarMovingModels::OnBnClickedOk()
{
	UpdateData(TRUE);
	CA_P= m_P;
	CA_P2=m_P2;
	Car_Following_Model_Type= BnID_To_CFChar(m_CF_Model);
	Car_Moving_Model_Type= BnID_To_CMChar(m_Car_Moving_Model);
	Reset_Cell_Length(Car_Moving_Model_Type);
	OnOK();
}
