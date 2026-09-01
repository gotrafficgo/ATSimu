// DlgMessage.cpp : implementation file
//

#include "stdafx.h"
#include "traffic.h"
#include "DlgMessage.h"


// CDlgMessage dialog

IMPLEMENT_DYNAMIC(CDlgMessage, CDialog)

CDlgMessage::CDlgMessage(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMessage::IDD, pParent)
	, m_Message(_T(""))
{

}

CDlgMessage::~CDlgMessage()
{
}

void CDlgMessage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MESSAGE, m_Message);
}


BEGIN_MESSAGE_MAP(CDlgMessage, CDialog)
END_MESSAGE_MAP()


// CDlgMessage message handlers

void CDlgMessage::Init_Dialog(CString str)
{
	m_Message=str;
}
