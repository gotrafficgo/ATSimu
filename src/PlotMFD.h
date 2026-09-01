#pragma once
#include "afxwin.h"
#include "SampleCollection.h"
#include "structure.h"


// CPlotMFD dialog

class CPlotMFD : public CDialog
{
	DECLARE_DYNAMIC(CPlotMFD)

public:
	CPlotMFD(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPlotMFD();

// Dialog Data
	enum { IDD = IDD_PLOT_MFD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	double maxX;
	double minX;
	double maxY;
	double minY;
	int width;
	int height;
	int mytop;
	int mybottom;
	int myleft;
	int myright;
	double intervalX;
	double intervalY;

public:
	afx_msg void OnPaint();
	afx_msg void OnClose();
	void PaintXYAxis(CDC *pDC);
	void PaintData(CDC *pDC);
	BOOL OnInitDialog();

	int Coordinate_Trans_X(double x);
	int Coordinate_Trans_Y(double y);
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
