// PlotTrajectory.cpp : implementation file
//

#include "stdafx.h"
#include "traffic.h"
#include "PlotTrajectory.h"
#include "setting.h"
#include "MainFrm.h"
#include "Global.h"
// CPlotTrajectory dialog


IMPLEMENT_DYNAMIC(CPlotTrajectory, CDialog)

CPlotTrajectory::CPlotTrajectory(CWnd* pParent /*=NULL*/)
	: CDialog(CPlotTrajectory::IDD, pParent)
{
	Reset_Trajectory_Array();
}

CPlotTrajectory::~CPlotTrajectory()
{
}

void CPlotTrajectory::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPlotTrajectory, CDialog)
	ON_WM_PAINT()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CPlotTrajectory message handlers


BOOL CPlotTrajectory::OnInitDialog() 
{
	CDialog::OnInitDialog();
	pWnd = GetDlgItem(IDC_CANVAS);                 //IDC_STATIC1 specified in the dialog editor
	CRect   m_rect;   
	pWnd->GetClientRect(&m_rect); 
	TopLeft= m_rect.TopLeft();
	BottomRight= m_rect.BottomRight();

	Cell_Length= BottomRight.y/CELL_NUMBER;

	Reflesh_Flag=false;
	Offset=0;
	return TRUE;  
}
void CPlotTrajectory::OnPaint()
{
	pWnd = GetDlgItem(IDC_CANVAS);  
	CPaintDC dc(pWnd);
	CDC * pDC = pWnd->GetDC();

	CBitmap* m_pBitmap   = new CBitmap;  
	CBitmap* pOldBitmap = pDC->SelectObject(m_pBitmap);         
	CBrush backgroundBrush= COLOR_WHITE;   
	CBrush* pOldBrush = pDC->SelectObject(&backgroundBrush);
	pDC->PatBlt(0,0,BottomRight.x,BottomRight.y, PATCOPY);
	Refresh_Trajectory();
 	PaintXYAxis(pDC);
	PaintData(pDC);
	dc.BitBlt(0,0,BottomRight.x,BottomRight.y,pDC, 0, 0,SRCCOPY);   //Copies a bitmap from the source device context to this current device context.
	pDC->SelectObject(pOldBitmap);
	pDC->SelectObject(pOldBrush);
}

void CPlotTrajectory::PaintData(CDC *pDC)
{
	CPen aPen;
	aPen.CreatePen(PS_SOLID,1, COLOR_RED);
	CPen *oldPen= pDC->SelectObject(&aPen);

	int Start_X;
	int Start_Y;
	int End_X;
	int End_Y;

	for (int i=0;i<TRAJ_NUMBER;i++)
		if(Traj_Array[i]!=NULL)
			if(Traj_Array[i]->Trajectory.GetSize()>=2)      //the number of points of trajectory is bigger than 2
			for (int j=0; j<Traj_Array[i]->Trajectory.GetSize()-1;j++)
			{
				Start_X=Traj_Array[i]->Trajectory.GetAt(j)->Time;
				Start_Y=Traj_Array[i]->Trajectory.GetAt(j)->Location;
				End_X=Traj_Array[i]->Trajectory.GetAt(j+1)->Time;
				End_Y=Traj_Array[i]->Trajectory.GetAt(j+1)->Location;

				Start_X=Get_Show_Time(Start_X);
				Start_Y=Get_Show_Location(Start_Y);
				End_X=Get_Show_Time(End_X);
				End_Y=Get_Show_Location(End_Y);

				pDC->MoveTo(Start_X,Start_Y);
				pDC->LineTo(End_X,End_Y);
			}

	pDC->SelectObject(&oldPen);
	CDialog::OnPaint();
}

void CPlotTrajectory::PaintXYAxis(CDC *pDC)
{
	pDC->SetBkMode(TRANSPARENT);

	pDC->MoveTo(TopLeft);
	pDC->LineTo(TopLeft.x,BottomRight.y);
	pDC->MoveTo(TopLeft.x,BottomRight.y);
	pDC->LineTo(BottomRight);
}


void CPlotTrajectory::OnClose()
{
	CMainFrame *pMainFrame= (CMainFrame *)AfxGetApp()->m_pMainWnd;   
	pMainFrame->Show_PlotTrajectory_Flag=false;        
	CDialog::OnClose();
}


void CPlotTrajectory::Reset_Trajectory_Array()
{
	for(int i=0; i<TRAJ_NUMBER;i++)
		Traj_Array[i]=NULL;
}

//only allow to add veh, and there is no deletion except it is pushed out the array
void CPlotTrajectory::Add_New_Veh(CVeh *New_Veh)
{
	//move everyone forward
	for (int i=TRAJ_NUMBER; i>0; i--)     //note: it is "i>0"
		Traj_Array[i]=Traj_Array[i-1];

	Traj_Array[0]=New_Veh;
}

int CPlotTrajectory::Get_Show_Time(int Simulation_Time)
{
	int Time_Span=int(BottomRight.x/TRAJ_GAP);
	int Show_Time= Simulation_Time%Time_Span;   //Time_Span
	Show_Time=Show_Time+Offset;
	return Show_Time;
}

int CPlotTrajectory::Get_Show_Location(int Cell_ID)
{
	int Show_Location=BottomRight.y-int(Cell_Length*Cell_ID);
	return Show_Location;
}

void CPlotTrajectory::Refresh_Trajectory()
{
	if (Reflesh_Flag==true)
		Offset++;
	if (Offset>BottomRight.x)
		Offset=0;
	Reflesh_Flag=false;
}

