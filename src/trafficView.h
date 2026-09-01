// trafficView.h : interface of the CTrafficView class
//
/////////////////////////////////////////////////////////////////////////////


#if !defined(AFX_TRAFFICVIEW_H__98D6616F_B338_469E_86B4_AE50B417D562__INCLUDED_)
#define AFX_TRAFFICVIEW_H__98D6616F_B338_469E_86B4_AE50B417D562__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

 #define WM_DISPVIEW WM_USER + 11         
 
#include "TrafficDoc.h"

class CTrafficView : public CScrollView
{
protected: // create from serialization only

	DECLARE_DYNCREATE(CTrafficView)

// Attributes
public:
	CTrafficDoc* GetDocument();
// Operations
public:
	int Press;
	//zoom & shift
	CDC* m_pdcMemory;  
	CBitmap* m_pBitmap;  
    BOOL     m_bCaptured;  
    CSize    m_sizeOffset;  
	int zoom_flag;  
	int OrigSizeX;
	int OrigSizeY;
	/////////////////
	bool st_ZoomIn;
	bool st_ZoomOut;
	bool st_Move;
	////////////////////////////
	//keep a constant simu step length
	long last_time;   
	long this_time;  

///////////////////////////////

//	void OnDispView();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrafficView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// Implementation
public:
	bool Init_Network();
	virtual ~CTrafficView();
	void Draw_All(CDC* pDC);
	double Get_Zoom_Ratio(int zoom_flag);
	afx_msg void OnInitDraw();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CTrafficView(); 
private:
public:
	bool Constant_Simu_Length(char Paint_Purpose);
	void Update_Dialog_Data();
// Generated message map functions
protected:
	//{{AFX_MSG(CTrafficView)
	afx_msg void OnRun();   
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point); 
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);  
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);  
	afx_msg void OnZoomin();  
	afx_msg void OnZoomout(); 
	afx_msg void OnPaint();  
	afx_msg void OnUpdateZoomIn(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZoomOut(CCmdUI* pCmdUI);
	afx_msg void OnMove();
	afx_msg void OnUpdateMove(CCmdUI* pCmdUI);
	afx_msg LRESULT OnDispView(WPARAM wParam, LPARAM Paint_Purpose);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//	afx_msg void OnDestroy();
};

#ifndef _DEBUG  // debug version in trafficView.cpp
inline CTrafficDoc* CTrafficView::GetDocument()
   { return (CTrafficDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif 
// !defined(AFX_TRAFFICVIEW_H__98D6616F_B338_469E_86B4_AE50B417D562__INCLUDED_)
