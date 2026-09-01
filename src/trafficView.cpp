
#include "stdafx.h"
#include "traffic.h"
#include "ErrorLog.h"
#include "trafficDoc.h"
#include "trafficView.h"
#include "point.h"
#include "Cell.h"
#include "MainFrm.h"  
#include "Communicator.h"
#include "SimuFun.h"
#include "Global.h"
#include "TrafficDraw.h"
#include "SampleCollection.h"
#include "setting.h"
#include "DlgGuidanceBoard.h"


/////////////////////////////////////////////////////////////////////////////
// CTrafficView

IMPLEMENT_DYNCREATE(CTrafficView, CScrollView)

BEGIN_MESSAGE_MAP(CTrafficView, CScrollView)
	//{{AFX_MSG_MAP(CTrafficView)
	ON_COMMAND(ID_RUN, OnRun)
	ON_WM_LBUTTONDOWN()  
	ON_WM_LBUTTONUP()  
	ON_WM_MOUSEMOVE()  
	ON_COMMAND(ID_ZOOMIN, OnZoomin)    
	ON_COMMAND(ID_ZOOMOUT, OnZoomout)  
	ON_WM_PAINT()  
	ON_UPDATE_COMMAND_UI(ID_ZOOMIN, OnUpdateZoomIn)
	ON_UPDATE_COMMAND_UI(ID_ZOOMOUT, OnUpdateZoomOut)
	ON_COMMAND(ID_MOVE, OnMove)
	ON_UPDATE_COMMAND_UI(ID_MOVE, OnUpdateMove)
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_INIT_DRAW, OnInitDraw)
//	ON_WM_DESTROY()
	ON_MESSAGE(WM_DISPVIEW, OnDispView)
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDBLCLK()	
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTrafficView construction/destruction

CTrafficView::CTrafficView()
{
	m_sizeEllipse.cx= OrigEllipseCX;
	m_sizeEllipse.cy= OrigEllipseCY;
	m_pointTopLeft.x=OrigSizeOff_X;   //10
	m_pointTopLeft.y=OrigSizeOff_Y;  //-100
	OrigSizeX=6000;                                //      3000
	OrigSizeY=-2000;                                //     -1000

	m_sizeOffset.cx=0;
	m_sizeOffset.cy=0;

	Press=0;

	last_time=0;
	this_time=0;
	
	m_pdcMemory = new CDC;  
    m_pBitmap   = new CBitmap;  
	m_bCaptured = FALSE;  
	zoom_flag=0;  
	simu_time=0;  
	st_ZoomIn=false;
	st_ZoomOut=false;
	st_Move=false;
}


CTrafficView::~CTrafficView()
{
	delete m_pdcMemory;
	delete m_pBitmap;
}

BOOL CTrafficView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTrafficView drawing

void CTrafficView::OnDraw(CDC* pDC)
{
	extern CString Open_File_PathName;
	extern bool Display_Flag;

	if (Open_File_PathName!="N")
	{
		//Draw the area which is dragged
		CPen EllipsePen(PS_SOLID,1,RGB(30,30,30));
		pDC->SelectObject(&EllipsePen);
		CPoint point(0, 0);		
		pDC->LPtoDP(&point);
		pDC->SetBrushOrg(point);
		pDC->Rectangle(CRect(m_pointTopLeft, m_sizeEllipse));

		//Refresh Dialog data
		Update_Dialog_Data();

		//Draw network
		if (Display_Flag)
			Draw_All(pDC);
	}
}



/////////////////////////////////////////////////////////////////////////////
// CTrafficView diagnostics
#ifdef _DEBUG
void CTrafficView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CTrafficView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CTrafficDoc* CTrafficView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTrafficDoc)));
	return (CTrafficDoc*)m_pDocument;
}
#endif //_DEBUG


LRESULT CTrafficView::OnDispView(WPARAM wParam, LPARAM Paint_Purpose)
{

	//stop the program at a specific time
	if (simu_time==Total_Simulation_Time)     //end time of simulation
	{
		CMainFrame *pMain= (CMainFrame *)AfxGetApp()->m_pMainWnd;  
		pMain->OnClose();
		return NULL;
	}


	//keep a constant simulation step length
	if (Constant_Simu_Length(char(Paint_Purpose)))
	{
		CClientDC dc(this);
		OnPrepareDC(&dc);
		CRect rectOld(m_pointTopLeft, m_sizeEllipse);
		dc.LPtoDP(rectOld);
		InvalidateRect(rectOld, FALSE);
		//	m_pointTopLeft = point;
		//	dc.DPtoLP(&m_pointTopLeft);
		CRect rectNew(m_pointTopLeft, m_sizeEllipse);
		dc.LPtoDP(rectNew);
		InvalidateRect(rectNew, FALSE);
		OnPaint();	
	}
	return NULL;
}


void CTrafficView::Draw_All(CDC *pDC)
{
	int i;
	CTrafficDraw trafficDraw(pDC);

	for(i=0;i<G_Link_Number ;i++)
	{
		trafficDraw.Fill_Link(i);
		trafficDraw.Draw_Lane(i);
		trafficDraw.Draw_Link_Label(i);
		trafficDraw.Draw_Guidance_Board(i);
		trafficDraw.Draw_Link_Middle_Line(i);
	}
	for(i=0;i<G_Cross_Number ;i++)
	{
		trafficDraw.Draw_Crossing(i);
	}
	for(i=0;i<G_Link_Number ;i++)
	{
		trafficDraw.Draw_Veh_On_Link(i);
	}
	for(i=0;i<G_Cross_Number ;i++)
	{ 
 		trafficDraw.Draw_Veh_In_Cross(i);
// 		trafficDraw.Draw_Cross_Label(i);
	}
	for(i=0;i<G_Link_Number ;i++)
	{
 		trafficDraw.Draw_Traffic_Light(i);
	}
	for (i=0;i<G_Cross_Number;i++)
	{
// 		trafficDraw.Draw_Conflict_Area(i);      //show conflict area within the intersection
// 		trafficDraw.Draw_Cross_Label(i);
	}
}


void CTrafficView::OnInitDraw() 
{
	if (true==Init_Network())          //initial network
		Invalidate();
}

bool CTrafficView::Init_Network()
{
	simuFun = new CSimuFun;
	simuFun->OnInit_Object();     //Get simu information from XML file
	simuFun->Enlarge_Link();
	simuFun->Set_Object_Relation(); 
	simuFun->CCwise_Sort_Link();                  
	simuFun->Set_Next_Link();
	simuFun->Set_Cross_Edge_Point();
	simuFun->Set_Lane_Middle_Line();
	simuFun->Set_Lane_Cell_Coordinate();
	simuFun->Set_Cross_Lane_Cell_and_Enter_Link();
	simuFun->Set_Link_Label_And_GBoard();
	simuFun->Set_All_Distance_Shortest_Path();
	simuFun->Connect_Phase_And_Lane(); 
	simuFun->Set_Conflict_Area();
	simuFun->Set_Straight_Conflict();
	simuFun->Connect_Controller_and_Detector();
	simuFun->Set_OD_On_Link();
	simuFun->Set_G2Detector();
	return true;
}


void CTrafficView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	
    CSize sizeTotal(OrigSizeX, OrigSizeX); // 8-by-10.5 inches
    CSize sizePage(sizeTotal.cx / 2, sizeTotal.cy / 2);
    CSize sizeLine(sizeTotal.cx / 50, sizeTotal.cy / 50);
    SetScrollSizes(MM_LOENGLISH, sizeTotal, sizePage, sizeLine);
 
	// creates the memory device context and the bitmap
    if (m_pdcMemory->GetSafeHdc() == NULL)
	{
        CClientDC dc(this); 
        OnPrepareDC(&dc);
        CRect rectMax(0, 0, sizeTotal.cx, -sizeTotal.cy);
        dc.LPtoDP(rectMax);    //Converts logical units into device units.
        m_pdcMemory->CreateCompatibleDC(&dc);    //Creates a memory device context that is compatible with the device specified by pDC
        m_pBitmap->CreateCompatibleBitmap(&dc, rectMax.right, rectMax.bottom);        // makes bitmap same size as display window
        m_pdcMemory->SetMapMode(MM_LOENGLISH);
    }
}


void CTrafficView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rect(m_pointTopLeft, m_sizeEllipse);
	CRgn circle;
	
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.LPtoDP(rect);
	circle.CreateRectRgnIndirect(rect);
	if (circle.PtInRegion(point)) 
	{
		SetCapture();     //Capturing the mouse ensures subsequent LButtonUp message
		m_bCaptured = TRUE;
		CPoint pointTopLeft(m_pointTopLeft);
		dc.LPtoDP(&pointTopLeft);
		m_sizeOffset = point - pointTopLeft; //device coordinates
	}
}

void CTrafficView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bCaptured) 
	{
		::ReleaseCapture();
		m_bCaptured = FALSE;
	}
}

void CTrafficView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bCaptured&&st_Move) 
	{
		CClientDC dc(this);
		OnPrepareDC(&dc);
		CRect rectOld(m_pointTopLeft, m_sizeEllipse);
		dc.LPtoDP(rectOld);
		m_pointTopLeft = point - m_sizeOffset;
		dc.DPtoLP(&m_pointTopLeft);
		CRect rectNew(m_pointTopLeft, m_sizeEllipse);
		dc.LPtoDP(rectNew);
		OnDispView(NULL, 'M');  //re-draw for moving
		Invalidate();
	}
}

double CTrafficView::Get_Zoom_Ratio(int zoom_flag)
{
	double ratio;
	
	if (zoom_flag>6)
	{
		AfxMessageBox("#Maximum!");
		zoom_flag=6;
	}
	if (zoom_flag<-6)
	{
		AfxMessageBox("#Minimum!");
		zoom_flag=-6;
	}
	
	ratio= 1+ zoom_flag/7.0;     //ratio is between [0 , 2]
	
	return ratio;
}

//double buffering
void CTrafficView::OnPaint() 
{	
	CPaintDC dc(this);           // device context for painting
    OnPrepareDC(&dc);       //Called by the framework before the OnDraw member function is called for screen display and before the OnPrint member function is called for each page during printing or print preview.

    CRect rectUpdate;
    dc.GetClipBox(&rectUpdate);     //Retrieves the dimensions of the tightest bounding rectangle around the current clipping boundary

    CBitmap* pOldBitmap = m_pdcMemory->SelectObject(m_pBitmap);          //load m_pBitmap to m_pdcMemory
    m_pdcMemory->SelectClipRgn(NULL);           //Selects the given region as the current clipping region for the device context
    m_pdcMemory->IntersectClipRect(&rectUpdate);       //Creates a new clipping region by forming the intersection of the current region and the rectangle specified by x1, y1, x2, and y2.
	CBrush backgroundBrush= BACKGROUND_COLOR;   //add WM_ERASEBKGND and reflection function at the same time
//  CBrush backgroundBrush((COLORREF) ::GetSysColor(COLOR_WINDOW));
    CBrush* pOldBrush = m_pdcMemory->SelectObject(&backgroundBrush);
	m_pdcMemory->PatBlt(rectUpdate.left, rectUpdate.top,rectUpdate.Width(), rectUpdate.Height(),PATCOPY);    //Creates a bit pattern on the device. 
	OnDraw(m_pdcMemory);
	dc.BitBlt(rectUpdate.left,rectUpdate.top,rectUpdate.Width(),rectUpdate.Height(),m_pdcMemory, rectUpdate.left, rectUpdate.top,SRCCOPY);   //Copies a bitmap from the source device context to this current device context.
	m_pdcMemory->SelectObject(pOldBitmap);
    m_pdcMemory->SelectObject(pOldBrush);
}


//guarantee each simulation step has the same length
//Two steps
//1. since the length of a step is usually less than 100ms, when "SIMU_STEP_LENGTH>100", sleep(SIMU_STEP_LENGTH-100). Note: it doesn't work, when a lot of veh on the network
//2. keep the accuracy by comparing the running time
bool CTrafficView::Constant_Simu_Length(char Paint_Purpose)
{
	//when dragging and zooming, it shouldnot work
	if (Paint_Purpose=='M'||Paint_Purpose=='Z')
		return true;

	if (simu_time==0)  //first step
	{
		last_time=GetTickCount();
		return true;
	}
	
	long medium_time;
	this_time= GetTickCount();
		
	if (this_time- last_time- SIMU_STEP_LENGTH < 0)    //step length of this time - one of last time < step length
	{
		while (Switch_Thread)
		{
			medium_time= GetTickCount(); 
			if (medium_time-last_time -SIMU_STEP_LENGTH >= 0)   //Once this time is large than simu length, return
			{
				last_time= medium_time;
				return true;
			}
			Sleep(1);
		}
	}
	else
	{
		last_time= this_time;
		return true;
	}
	return false;
}

void CTrafficView::OnZoomin() 
{
	extern double zoom_ratio;
	zoom_flag++;
	zoom_ratio= Get_Zoom_Ratio(zoom_flag);
	OnDispView(NULL, 'Z');              //re-paint for zooming
	Invalidate();                               //refresh
}


void CTrafficView::OnZoomout() 
{
	extern double zoom_ratio;
	zoom_flag--;
	zoom_ratio= Get_Zoom_Ratio(zoom_flag);
	OnDispView(NULL, 'Z');  
	Invalidate(); 
}

void CTrafficView::OnMove() 
{
	st_Move=!st_Move;
}

void CTrafficView::OnUpdateMove(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(st_Move);	
}

void CTrafficView::OnUpdateZoomIn(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(st_ZoomIn);	
}

void CTrafficView::OnUpdateZoomOut(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(st_ZoomOut);		
}

BOOL CTrafficView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (nHitTest == HTCLIENT)
	{			
		HCURSOR Current_Cursor;

		if (st_Move)
			Current_Cursor= AfxGetApp()->LoadStandardCursor(IDC_SIZEALL);
		else 
			Current_Cursor= AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		
		SetCursor(Current_Cursor);     // New mouse cursor is active while mouse is captured
		
		return false;
	}
	
	return true;
}


UINT ThreadProc(LPVOID param)
{
	HWND hWnd = (HWND)param;
	extern char Current_Control_Type;
	extern int Start_Time_Guidance;
	extern bool Switch_Guidance;
	extern bool Switch_Show_Guidance;
	extern bool simu_pause_for_no_guidance;
	extern int Sleep_Time;


	//pseudo random number
	//	srand( (unsigned)time( NULL ) ); 
	srand( 8 );          

	while(Switch_Thread)
	{
		if( false==simu_pause && false==simu_pause_for_no_guidance) //Even as the simu is paused, the thread for receiving guidance information still runs until all guidance information is newest.
		{
			//send time to information center
			// 			if(Switch_Detector && Current_Control_Type!='N')
			// 				communicator->Send_Simu_Time();

			simuFun->Simu_Go_A_Step();                               //simulation go a step

			::SendMessage(hWnd, WM_DISPVIEW, 0, 0);    //draw 

			simu_time++;

// 			//pause the program at a specific time 
// 			if (simu_time==Total_Simulation_Time)
// 				simu_pause=true;

//				//keep a constant simulation step
// 			if (SIMU_STEP_LENGTH>100)
// 				::Sleep(SIMU_STEP_LENGTH-100);

			::Sleep(Sleep_Time);    

		} 
		Sleep(1);
	}
//	AfxEndThread(0);

	delete simuFun;
	delete err;
	delete communicator;
	delete pSampleCollection;

	return 1;
}


void CTrafficView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	for (int i=0; i<G_Link_Number;i++)
	{
		if (Link_Array[i]->guidance!=NULL)
		{
			if (Link_Array[i]->isShowGuidanceBoard==false)
			{
				Link_Array[i]->dlgGuidanceBoard= new CDlgGuidanceBoard(i);

				CRgn circle;
				CClientDC dc(this);
				OnPrepareDC(&dc);
				if (Switch_Guidance==true)
				{				
					dc.LPtoDP(Link_Array[i]->Guidance_Board);
					circle.CreateRectRgnIndirect(Link_Array[i]->Guidance_Board);
					if (circle.PtInRegion(point)) 
					{
						Link_Array[i]->dlgGuidanceBoard->Create(IDD_DLG_GUIDANCE_BOARD);
						Link_Array[i]->dlgGuidanceBoard->RefreshData();
						Link_Array[i]->dlgGuidanceBoard->ShowWindow(SW_SHOW);
						Link_Array[i]->isShowGuidanceBoard=true;
					}
				}
			}
		}
	}

	CScrollView::OnLButtonDblClk(nFlags, point);
}


void CTrafficView::Update_Dialog_Data()
{
	CMainFrame *pMain=(CMainFrame*)AfxGetApp()->m_pMainWnd;   

	if (pMain->Show_Data_Flag==true)
	{
		if (pMain->pDlgShowData!=NULL)
			pMain->pDlgShowData->RefreshData();		
	}

	if (pMain->Show_Control_View_Flag==true)
	{
		if(pMain->pDlgShowControl!=NULL)
			pMain->pDlgShowControl->RefreshData();
	}

	if (pMain->Show_Guidance_View_Flag==true)
	{
		if(pMain->pDlgShowGuidance!=NULL)
			pMain->pDlgShowGuidance->RefreshData();
	}

	if (pMain->Show_GS_Statement_Flag==true)
	{
		if(pMain->pDlgCGStatement!=NULL)
			pMain->pDlgCGStatement->RefreshData();
	}

	for (int i=0; i<G_Link_Number; i++)
	{
		if(Link_Array[i]->isShowGuidanceBoard==true)
		{
			if(NULL!=Link_Array[i]->dlgGuidanceBoard)
				Link_Array[i]->dlgGuidanceBoard->RefreshData();
		}
	}

	if (pMain->Show_PlotMFD_Flag==true)
	{
		if(pMain->pPlotMFD!=NULL)
		{
			pMain->pPlotMFD->OnPaint();
			pMain->pPlotMFD->Invalidate();
		}
	}
}


BOOL CTrafficView::OnEraseBkgnd(CDC* pDC)
{
	return true;   //this is necessary to prevent from flickering
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
////    A Trafic Simulation Program     /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////    Developed by        Zhengbing He, Jorge A Laval, Shoufeng Ma
////                                  Tianjin University, China
//// 		       						Georgia Institute of Technology, U.S.
////    Contact                he.zb@hotmail.com
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

//start point of this program
void CTrafficView::OnRun() 
{
	extern CString Open_File_PathName;
	extern char Current_Control_Type;
	extern bool Switch_Show_Guidance;

	if (!Check_Load_or_Not())
		return;

	Press++;
	simu_pause=false;

	CMainFrame *pMainFrame= (CMainFrame *)AfxGetApp()->m_pMainWnd;     
	pMainFrame->Change_Button_Surface(Press); 

	if(Press==1)
	{
		HWND hWnd = GetSafeHwnd();
		AfxBeginThread(ThreadProc, hWnd); 

		//  communication thread for receiving control information
		// 		HWND hWnd2= GetSafeHwnd();
		// 		AfxBeginThread(ThreadProc_to_Control, hWnd2);
		// 		
		//  communication thread for receiving guidance information
		// 		HWND hWnd3= GetSafeHwnd();
		// 		AfxBeginThread(ThreadProc_to_Guidance, hWnd3);

	}
	else if (Press%2==1)          //1,odd- run
		simu_pause=false;
	else
		simu_pause=true;
}



UINT ThreadProc_to_Guidance(LPVOID param)
{
	HWND hWnd3= (HWND)param;
	extern bool Switch_Guidance;
	extern CCommunicator *communicator;
	extern bool simu_pause_for_no_guidance;
	extern bool Is_Guidance_Info_Right;
	int i=0;
	int k=0;
	int j=0;

	while (Switch_Thread)
	{
		if (Switch_Guidance)
		{
			communicator->Receive_Data(communicator->GuidanceSock, 'G');

			if (communicator->isNewGuidance==true)
			{
				for (i=0;i<communicator->tran_guidance.Link_Number;i++)
				{
					int Link_ID= communicator->tran_guidance.Guidance_Buffer[i].Link_ID;
					if (Link_Array[Link_ID]->guidance!=NULL)
					{
						if( communicator->tran_guidance.Guidance_Buffer[i].This_Times==0
							||Link_Array[Link_ID]->guidance->Received_Gudiance_Times <= communicator->tran_guidance.Guidance_Buffer[i].This_Times)    //"="时，为了防止长时间等待而设定
						{
							Link_Array[Link_ID]->guidance->Received_Guidance_Info= communicator->tran_guidance.Guidance_Buffer[i];
							Link_Array[Link_ID]->guidance->Received_New_Guidance_Info=true;
							k++;
						}
						j++;
					}
				}
				if(k==j)
				{
					Is_Guidance_Info_Right= true;
					simu_pause_for_no_guidance=false;
					if (simu_time!=0 && simu_time%GUIDANCE_CIRCLE==0)   
					{
						for (i=0;i<communicator->tran_guidance.Link_Number;i++)
						{
							int Link_ID= communicator->tran_guidance.Guidance_Buffer[i].Link_ID;
							if (Link_Array[Link_ID]->guidance!=NULL) 
							{
								Link_Array[Link_ID]->guidance->Refresh_Guidance_Info();
							}
						}
					}
				}
				else
					Is_Guidance_Info_Right=false;
			}
		}
		Sleep(1);
	}
//	AfxEndThread(0);
	return 1;
}

UINT ThreadProc_to_Control(LPVOID param)
{
	HWND hWnd2= (HWND)param;
	extern char Current_Control_Type;
	extern CCommunicator* communicator;

	while (Switch_Thread)
	{
		if (Current_Control_Type=='R')
			communicator->Receive_Data(communicator->ControlSock, 'R');
		Sleep(1);
	}
//	AfxEndThread(0);
	return 1;
}
