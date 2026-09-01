// PlotMFD.cpp : implementation file
//

#include "stdafx.h"
#include "traffic.h"
#include "PlotMFD.h"
#include "setting.h"
#include "MainFrm.h"
#include "SampleCollection.h"
#include "Global.h"

// CPlotMFD dialog

IMPLEMENT_DYNAMIC(CPlotMFD, CDialog)

CPlotMFD::CPlotMFD(CWnd* pParent /*=NULL*/)
	: CDialog(CPlotMFD::IDD, pParent)
{
	maxX=60;
	minX=0;
	maxY=60;
	minY=0;

	width=500;
	height=300;

	mytop=10;
	mybottom=40;
	myleft=80;
	myright=50;

	intervalX=(width-myleft-myright)/(maxX-minX);
	intervalY=(height-mybottom-mytop)/(maxY-minY);

}

CPlotMFD::~CPlotMFD()
{
}

void CPlotMFD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPlotMFD, CDialog)
	ON_WM_PAINT()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CPlotMFD message handlers


BOOL CPlotMFD::OnInitDialog() 
{
	CDialog::OnInitDialog();

	return TRUE;  
}


void CPlotMFD::OnPaint()
{
	CPaintDC dc(this);   
	CDC *pDC= &dc;  //paint in the dialog 

// 	PaintXYAxis(pDC);
// 	PaintData(pDC);

	CBitmap* m_pBitmap   = new CBitmap;  
	CBitmap* pOldBitmap = pDC->SelectObject(m_pBitmap);         
	CBrush backgroundBrush= COLOR_WHITE;   //修改背景色时，还要添加WM_ERASEBKGND消息及其响应函数
	CBrush* pOldBrush = pDC->SelectObject(&backgroundBrush);
	pDC->PatBlt(0,0,width+100,height+100, PATCOPY);
	PaintXYAxis(pDC);
	PaintData(pDC);
	dc.BitBlt(0,0,width+100,height+100,pDC, 0, 0,SRCCOPY);   //Copies a bitmap from the source device context to this current device context.
	pDC->SelectObject(pOldBitmap);
	pDC->SelectObject(pOldBrush);
}


void CPlotMFD::PaintData(CDC *pDC)
{
	extern CSampleCollection *pSampleCollection;	
	double k,q;
	CPen aPen;
	aPen.CreatePen(PS_SOLID,1, COLOR_RED);
	CPen *oldPen= pDC->SelectObject(&aPen);

	int AKQ_Size=pSampleCollection->AKQ.GetSize();
	for (int i=0; i<AKQ_Size; i++)
	{
		k=pSampleCollection->AKQ.GetAt(i)->k;
		q=pSampleCollection->AKQ.GetAt(i)->q;
		k= Coordinate_Trans_X(k);
		q= Coordinate_Trans_Y(q);
		pDC->Ellipse(int(k),int(q),int(k+3),int(q+3));	
	}

	pDC->SelectObject(&oldPen);
	CDialog::OnPaint();
}

int CPlotMFD::Coordinate_Trans_X(double x)
{
	int newX=int(myleft+(x-minX)*intervalX);
	return newX;
}

int CPlotMFD::Coordinate_Trans_Y(double y)
{
	int newY= int(height-(mybottom+(y-minY)*intervalY));
	return newY;
}

void CPlotMFD::PaintXYAxis(CDC *pDC)
{
	double bottomY=0;
	double leftX=0;

	if(minY>0)
		bottomY=minY;
	if(minX>0)
		leftX=minX;

	pDC->SetBkMode(TRANSPARENT);

	pDC->MoveTo(int(myleft),int(height-(mybottom+(bottomY-minY)*intervalY)));
	pDC->LineTo(int(width-myright),int(height-(mybottom+(bottomY-minY)*intervalY)));
	pDC->MoveTo(int(myleft+(leftX-minX)*intervalX),int(height-mybottom));
	pDC->LineTo(int(myleft+(leftX-minX)*intervalX),int(mytop));

	const int count=5;

	double spaceX=(width-myleft-myright)/count;
	double spaceY=(height-mybottom-mytop)/count;

	CString str;
	for(int i=0;i<=count;i++)
	{
		str.Format("%d",int(minX+i*(maxX-minX)/count));
		pDC->MoveTo(int(myleft+spaceX*i),int(height-(mybottom+(bottomY-minY)*intervalY)));
		pDC->LineTo(int(myleft+spaceX*i),int(height-(mybottom+(bottomY-minY)*intervalY+5)));
		pDC->TextOut(int(myleft+spaceX*i-10),	int(height-(mybottom+(bottomY-minY)*intervalY-5)),str);
	}

	for(int i=0;i<=count;i++)
	{
		str.Format("%d",int(minY+i*(maxY-minY)/count));
		pDC->MoveTo(int(myleft+(leftX-minX)*intervalX),int(height-(mybottom+spaceY*i)));
		pDC->LineTo(int(myleft+(leftX-minX*intervalX+5)),int(height-(mybottom+spaceY*i)));
		pDC->TextOut(int(myleft+(leftX-minX)*intervalX-30),	int(height-(mybottom+spaceY*i+8)),str);
	}

	pDC->TextOut(width/2 -18, height-16 , "veh/km");
	pDC->TextOut(0 , height/2-20, "veh/hr");
}


void CPlotMFD::OnClose()
{
	CMainFrame *pMainFrame= (CMainFrame *)AfxGetApp()->m_pMainWnd;   
	pMainFrame->Show_PlotMFD_Flag=false;          //可以把Show_Control_View_Flag放在这个CDialog类中，在CMainFram中判断。
	CDialog::OnClose();
}

