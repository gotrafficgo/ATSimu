// trafficDoc.cpp : implementation of the CTrafficDoc class
//

#include "stdafx.h"
#include "traffic.h"
#include "MainFrm.h"
#include "trafficDoc.h"
#include "trafficView.h"
#include "Global.h"

/////////////////////////////////////////////////////////////////////////////
// CTrafficDoc

IMPLEMENT_DYNCREATE(CTrafficDoc, CDocument)

BEGIN_MESSAGE_MAP(CTrafficDoc, CDocument)
	//{{AFX_MSG_MAP(CTrafficDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrafficDoc construction/destruction

CTrafficDoc::CTrafficDoc()
{
}

CTrafficDoc::~CTrafficDoc()
{
}


/////////////////////////////////////////////////////////////////////////////
// CTrafficDoc diagnostics

#ifdef _DEBUG
void CTrafficDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTrafficDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


BOOL CTrafficDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	Open_File_PathName=lpszPathName;	
	CMainFrame *pMain=(CMainFrame*)AfxGetApp()->m_pMainWnd;     
	CTrafficView *pView=(CTrafficView*)pMain->GetActiveView(); 
	
	pMain->Show_All_Dialog();

	pView->OnInitDraw();
	
	return false;
}

