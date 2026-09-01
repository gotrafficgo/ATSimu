#pragma once

class CTrafficDraw
{
public:
	/////////////////////
	int x,y;
	int start_x;
	int start_y;
	int end_x;
	int end_y;
	/////////////////////
	CDC *pDC;
	CPen pen_for_network; 
	CPen pen_for_cross;
	CPen pen_for_middle_thread;
	CBrush brush_for_link;
	CBrush brush_for_cross;
	CBrush brush_for_blank;
	CPen pen_for_blank;

	CBrush brush_for_veh_red;
	CBrush brush_for_veh_white;
	CBrush brush_for_veh_black;
	CBrush brush_for_veh_green;
	CBrush brush_for_veh_blue;
	CPen pen_for_veh_red;
	CPen pen_for_veh_white;
	CPen pen_for_veh_black;
	CPen pen_for_veh_green;
	CPen pen_for_veh_blue;

	CBrush brush_for_guidance_board;
	CPen pen_for_guidance_board;

	CBrush brush_for_yellow_light;
	CBrush brush_for_red_light;
	CBrush brush_for_green_light;
	CPen pen_for_yellow_light;
	CPen pen_for_green_light;
	CPen pen_for_red_light;

	CPoint Link_Poly_Array[4];
	CPoint Cross_Poly_Array[8];
	////////////////
	CPen *oldPen;
	CBrush *oldBrush;
	COLORREF oldTextColor;
	///////////////
	CFont fontLinkLabel;
	CFont *oldFont;
	
public:
	CTrafficDraw(CDC *pDC);
	~CTrafficDraw(void);
	void Fill_Link(int Link_ID);
	void Draw_Lane(int Link_ID);
	void Draw_Link_Label(int Link_ID);
	void Draw_Link_Middle_Line(int Link_ID);
	void Draw_Crossing(int Cross_ID);
	void Draw_Veh_On_Link(int Link_ID);
	void Draw_Veh_In_Cross(int Cross_ID);
	void Draw_Cross_Label(int Cross_ID);
	void Draw_Traffic_Light(int Link_ID);
	void Draw_Conflict_Area(int Cross_ID);
	void Draw_Guidance_Board(int Link_ID);
};
