#include "stdafx.h"
#include "TrafficMessageBox.h"
#include "DlgMessage.h"

TrafficMessageBox::TrafficMessageBox(void)
{

}

TrafficMessageBox::~TrafficMessageBox(void)
{
}

void TrafficMessageBox::ShowMessage(CString str)
{
	CDlgMessage *pDlgMessage= new CDlgMessage();
	pDlgMessage->Init_Dialog(str);
	pDlgMessage->Create(IDD_MESSAGE);
	pDlgMessage->ShowWindow(SW_SHOW);
}