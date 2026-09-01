#pragma once

#include "Veh.h"

#define TRAJ_GAP 4
#define CELL_NUMBER 200
#define TRAJ_NUMBER 50
// CPlotTrajectory dialog

class CPlotTrajectory : public CDialog
{
	DECLARE_DYNAMIC(CPlotTrajectory)

public:
	CPlotTrajectory(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPlotTrajectory();

// Dialog Data
	enum { IDD = IDD_PLOT_TRAJECTORY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CWnd * pWnd;
	CPoint TopLeft;
	CPoint BottomRight;
	CVeh* Traj_Array[TRAJ_NUMBER];   //CVeh->Trajectory->Loc_And_Time
	double Cell_Length;
	bool Reflesh_Flag;
	int Offset;         //record the time displacement of trajectory (control the moving speed of trajectories)

public:
	afx_msg void OnPaint();
	afx_msg void OnClose();
	BOOL OnInitDialog() ;
	void PaintData(CDC *pDC);
	void PaintXYAxis(CDC *pDC);
	void Reset_Trajectory_Array();
	void Add_New_Veh(CVeh *New_Veh);
	int Get_Show_Time(int Simulation_Time);
	int Get_Show_Location(int Cell_ID);
	void Refresh_Trajectory();
};
