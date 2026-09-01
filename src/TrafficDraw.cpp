#include "stdafx.h"
#include "TrafficDraw.h"
#include "setting.h"
#include "Global.h"
#include "Veh.h"

CTrafficDraw::CTrafficDraw(CDC *pDC)
{
	this->pDC= pDC;
	
	x=-1;
	y=-1;
	start_x=-1;
	start_y=-1;
	end_x=-1;
	end_y=-1;

	pen_for_middle_thread.CreatePen(PS_SOLID,1,MIDDLELINE_COLOR);

	brush_for_link.CreateSolidBrush(LINK_COLOR);
	brush_for_cross.CreateSolidBrush(RGB(170,170,170));
	brush_for_blank.CreateSolidBrush(BLANK_COLOR);
	pen_for_network.CreatePen(PS_SOLID,1,NETWORK_COLOR); 
	pen_for_cross.CreatePen(PS_SOLID, 1, CROSS_COLOR);
	pen_for_blank.CreatePen(PS_SOLID,1,BLANK_COLOR);

	brush_for_veh_red.CreateSolidBrush(COLOR_RED_VEH);
	brush_for_veh_white.CreateSolidBrush(COLOR_WHITE);
	brush_for_veh_black.CreateSolidBrush(COLOR_BLACK);
	brush_for_veh_green.CreateSolidBrush(COLOR_GREEN_VEH);
	brush_for_veh_blue.CreateSolidBrush(COLOR_BLUE_VEH);
	brush_for_veh_yellow.CreateSolidBrush(COLOR_YELLOW);

	pen_for_veh_red.CreatePen(PS_SOLID,1, COLOR_RED_VEH);
	pen_for_veh_white.CreatePen(PS_SOLID,1, COLOR_WHITE);
	pen_for_veh_black.CreatePen(PS_SOLID,1,COLOR_BLACK);
	pen_for_veh_green.CreatePen(PS_SOLID,1, COLOR_GREEN_VEH);
	pen_for_veh_blue.CreatePen(PS_SOLID,1, COLOR_BLUE_VEH);
	pen_for_veh_yellow.CreatePen(PS_SOLID,1,COLOR_YELLOW);

	brush_for_guidance_board.CreateSolidBrush(COLOR_GREEN);
	pen_for_guidance_board.CreatePen(PS_SOLID,1, COLOR_GREEN);

	brush_for_yellow_light.CreateSolidBrush(COLOR_YELLOW);
	brush_for_red_light.CreateSolidBrush(COLOR_RED);
	brush_for_green_light.CreateSolidBrush(COLOR_GREEN);
	pen_for_yellow_light.CreatePen(PS_SOLID,1, COLOR_YELLOW);
	pen_for_red_light.CreatePen(PS_SOLID,1,COLOR_RED);
	pen_for_green_light.CreatePen(PS_SOLID,1, COLOR_GREEN);

	fontLinkLabel.CreatePointFont(120, "Time New Roman");
	///////////////////
	for (int i=0;i<8;i++)
	{
		Cross_Poly_Array[i].x=-1;
		Cross_Poly_Array[i].y=-1;
	}
	for (int i=0;i<4;i++)
	{
		Link_Poly_Array[i].x=-1;
		Link_Poly_Array[i].y=-1;
	}
}

CTrafficDraw::~CTrafficDraw(void)
{
}

void CTrafficDraw::Fill_Link(int Link_ID)
{
	oldBrush= pDC->SelectObject(&brush_for_link);		
	oldPen= pDC->SelectObject(&pen_for_network);

	Link_Poly_Array[0].x= Get_New_Coordinate((int)Link_Array[Link_ID]->Rightest_Line_End.x, 'x');
	Link_Poly_Array[0].y= Get_New_Coordinate((int)Link_Array[Link_ID]->Rightest_Line_End.y, 'y');
	Link_Poly_Array[1].x= Get_New_Coordinate((int)Link_Array[Link_ID]->Rightest_Line_Start.x, 'x');
	Link_Poly_Array[1].y= Get_New_Coordinate((int)Link_Array[Link_ID]->Rightest_Line_Start.y, 'y');
	Link_Poly_Array[2].x= Get_New_Coordinate((int)Link_Array[Link_ID]->Leftest_Line_Start.x,'x');
	Link_Poly_Array[2].y= Get_New_Coordinate((int)Link_Array[Link_ID]->Leftest_Line_Start.y, 'y');
	Link_Poly_Array[3].x= Get_New_Coordinate((int)Link_Array[Link_ID]->Leftest_Line_End.x, 'x');
	Link_Poly_Array[3].y= Get_New_Coordinate((int)Link_Array[Link_ID]->Leftest_Line_End.y, 'y');
	pDC->Polygon(Link_Poly_Array, 4);
	pDC->SelectObject(&oldBrush);
	pDC->SelectObject(&oldPen);
}


void CTrafficDraw::Draw_Lane(int Link_ID)
{
	int j;
	oldPen= pDC->SelectObject(&pen_for_network);
	for(j=0;j<Link_Array[Link_ID]->Lane_Number;j++)
	{
		start_x=int(Link_Array[Link_ID]->Lane_Start_Point[j].x);
		start_y=int(Link_Array[Link_ID]->Lane_Start_Point[j].y);
		end_x=int(Link_Array[Link_ID]->Lane_End_Point[j].x);
		end_y=int(Link_Array[Link_ID]->Lane_End_Point[j].y);

		start_x= Get_New_Coordinate(start_x, 'x');
		start_y= Get_New_Coordinate(start_y, 'y');
		end_x= Get_New_Coordinate(end_x, 'x');
		end_y= Get_New_Coordinate(end_y, 'y');

		pDC->MoveTo(start_x,start_y);
		pDC->LineTo(end_x,end_y);
	}
	pDC->SelectObject(&oldPen);
}


void CTrafficDraw::Draw_Link_Label(int Link_ID)
{
	oldFont= pDC->SelectObject(&fontLinkLabel);
	oldTextColor= pDC->SetTextColor(COLOR_SAP_GREEN);
	pDC->SetBkMode(TRANSPARENT);
	int label_x= Get_New_Coordinate((int)Link_Array[Link_ID]->Label_Coordinate.x ,'x');
	int label_y= Get_New_Coordinate((int)Link_Array[Link_ID]->Label_Coordinate.y ,'y');
	pDC->TextOut(label_x, label_y ,Link_Array[Link_ID]->Object_Label);
	pDC->SetTextColor(oldTextColor);
	pDC->SelectObject(&oldFont);
}

void CTrafficDraw::Draw_Link_Middle_Line(int Link_ID)
{
	oldPen= pDC->SelectObject(&pen_for_middle_thread);
	start_x=(int)(Link_Array[Link_ID]->Leftest_Line_Start.x );
	start_y=(int)(Link_Array[Link_ID]->Leftest_Line_Start.y);
	end_x=(int)(Link_Array[Link_ID]->Leftest_Line_End.x );
	end_y=(int)(Link_Array[Link_ID]->Leftest_Line_End.y );

	start_x= Get_New_Coordinate(start_x, 'x');
	start_y= Get_New_Coordinate(start_y, 'y');
	end_x= Get_New_Coordinate(end_x, 'x');
	end_y= Get_New_Coordinate(end_y, 'y');

	pDC->MoveTo(start_x, start_y);
	pDC->LineTo(end_x,end_y);

	pDC->SelectObject(&oldPen);
}

void CTrafficDraw::Draw_Crossing(int Cross_ID)
{	
	int Cross_Point_Sum=0;
	oldPen= pDC->SelectObject(&pen_for_cross);

	for(int i=0;i<MAX_CROSS_POINT_NUMBER;i++)
	{
		if(Cross_Array[Cross_ID]->Cross_Point[i].x !=-1)
		{
			if (i<MAX_CROSS_POINT_NUMBER-1)  //not the last one
			{
				x=(int)(Cross_Array[Cross_ID]->Cross_Point[i].x);
				y=(int)(Cross_Array[Cross_ID]->Cross_Point[i].y);
				end_x= (int)(Cross_Array[Cross_ID]->Cross_Point[i+1].x);
				end_y=(int)(Cross_Array[Cross_ID]->Cross_Point[i+1].y);
			}
			else
			{
				x=(int)(Cross_Array[Cross_ID]->Cross_Point[i].x);
				y=(int)(Cross_Array[Cross_ID]->Cross_Point[i].y);
				end_x= (int)(Cross_Array[Cross_ID]->Cross_Point[0].x);
				end_y=(int)(Cross_Array[Cross_ID]->Cross_Point[0].y);
			}

			x= Get_New_Coordinate(x, 'x');
			y= Get_New_Coordinate(y, 'y');
			end_x= Get_New_Coordinate(end_x, 'x');
			end_y= Get_New_Coordinate(end_y, 'y');

			pDC->MoveTo(x, y);
			pDC->LineTo(end_x,end_y);

			Cross_Poly_Array[i].x=end_x;
			Cross_Poly_Array[i].y=end_y;
			Cross_Point_Sum++;
		}
		else 
			break;
	}
	
	oldBrush= pDC->SelectObject(&brush_for_cross);
	pDC->Polygon(Cross_Poly_Array, Cross_Point_Sum);	
	Cross_Point_Sum=0;
	pDC->SelectObject(&oldBrush);
	pDC->SelectObject(oldPen);
}


void CTrafficDraw::Draw_Guidance_Board(int Link_ID)
{
	int x=0;
	int y=0;

	extern bool Switch_Guidance;
	if(Switch_Guidance==true)
	{
		if (Link_Array[Link_ID]->guidance!=NULL)
		{
			oldPen= pDC->SelectObject(&pen_for_guidance_board);
			oldBrush= pDC->SelectObject(&brush_for_guidance_board);

			x=(int)Link_Array[Link_ID]->Label_Coordinate.x;
			y=(int)Link_Array[Link_ID]->Label_Coordinate.y;

			x=Get_New_Coordinate(x,'x');
			y=Get_New_Coordinate(y,'y');

			CRect rect((int)x+int(zoom_ratio*GUIDANCE_BOARD_RADUS),
				(int)y+int(zoom_ratio*GUIDANCE_BOARD_RADUS),
				(int)x- int(zoom_ratio*GUIDANCE_BOARD_RADUS), 
				(int)y- int(zoom_ratio*GUIDANCE_BOARD_RADUS));

			Link_Array[Link_ID]->Guidance_Board=rect; 

			pDC->Ellipse(rect);

			pDC->SelectObject(&oldPen);
			pDC->SelectObject(&oldBrush);
		}
	}
}

void CTrafficDraw::Draw_Veh_On_Link(int Link_ID)
{
	int j;
	int w;
	CVeh *pVeh=NULL;

	for(j=0;j<Link_Array[Link_ID]->Lane_Number;j++)
	{
// if ( Link_ID==46 )
// {
// }
// else if(Link_ID==47  && (j==0||j==1||j==3))
// {
// }
// else
		for(w=0;w<Link_Array[Link_ID]->Lanes[j]->Cell_Number;w++)
		{
			if(Link_Array[Link_ID]->Lanes[j]->Lane_Cell[w]->IsVehInCell()==true)
			{	
				pVeh=Link_Array[Link_ID]->Lanes[j]->Lane_Cell[w]->GetVehFromCell();
				if(pVeh!=NULL)
				{
					switch (pVeh->Veh_Color)
					{
					case 'R':
						oldPen= pDC->SelectObject(&pen_for_veh_red);
						oldBrush= pDC->SelectObject(&brush_for_veh_red);
						break;
					case 'W':
						oldPen= pDC->SelectObject(&pen_for_veh_white);
						oldBrush= pDC->SelectObject(&brush_for_veh_white);
						break;
					case 'K':
						oldPen= pDC->SelectObject(&pen_for_veh_black);
						oldBrush= pDC->SelectObject(&brush_for_veh_black);
						break;
					case 'G':
						oldPen= pDC->SelectObject(&pen_for_veh_green);
						oldBrush= pDC->SelectObject(&brush_for_veh_green);
						break;
					case 'B':
						oldPen= pDC->SelectObject(&pen_for_veh_blue);
						oldBrush= pDC->SelectObject(&brush_for_veh_blue);
						break;
					case 'Y':
						oldPen= pDC->SelectObject(&pen_for_veh_yellow);
						oldBrush= pDC->SelectObject(&brush_for_veh_yellow);
						break;
					}
					x = (int)(Link_Array[Link_ID]->Lanes[j]->Lane_Cell[w]->Cell_Center->x);
					y = (int)(Link_Array[Link_ID]->Lanes[j]->Lane_Cell[w]->Cell_Center->y);

					x= Get_New_Coordinate(x, 'x');
					y= Get_New_Coordinate(y, 'y');

					pDC->Ellipse(x+int(zoom_ratio*VEH_SIZE),
						                 y+int(zoom_ratio*VEH_SIZE),
								         x- int(zoom_ratio*VEH_SIZE), 
								         y- int(zoom_ratio*VEH_SIZE) );
				}
			}
// 				else
// 				{
// 					pDC->SelectObject(&brush_for_blank);
// 					x = (int)(Link_Array[i]->Lanes[j]->Lane_Cell[w]->Cell_Center->x);
// 					y = (int)(Link_Array[i]->Lanes[j]->Lane_Cell[w]->Cell_Center->y);
// 					
// 					x= Get_New_Coordinate(x, 'x');
// 					y= Get_New_Coordinate(y, 'y');
// 					
// 					pDC->Ellipse(x+1, y+1, x-1, y-1);
// 				}
		}
	}
	pDC->SelectObject(&oldPen);
	pDC->SelectObject(&oldBrush);

}

void CTrafficDraw::Draw_Veh_In_Cross(int Cross_ID)
{
	int j;
	int w;
	CVeh *pVeh;
	for(j=0;j<MAX_CROSS_LANE_NUMBER;j++)
	{
		if(Cross_Lane_Array[Cross_ID][j]!=NULL)
		{
			for(w=0;w<Cross_Lane_Array[Cross_ID][j]->Cell_Number ;w++)		
			{
				if(Cross_Lane_Array[Cross_ID][j]->Cross_Lane_Cell[w]->IsVehInCell()==true)
				{
					x=(int)(Cross_Lane_Array[Cross_ID][j]->Cross_Lane_Cell[w]->Cell_Center->x);
					y=(int)(Cross_Lane_Array[Cross_ID][j]->Cross_Lane_Cell[w]->Cell_Center->y);

					x= Get_New_Coordinate(x, 'x');
					y= Get_New_Coordinate(y, 'y');
					
					pVeh= Cross_Lane_Array[Cross_ID][j]->Cross_Lane_Cell[w]->GetVehFromCell();
					if (pVeh!=NULL)
					{
						switch (pVeh->Veh_Color)
						{
						case 'R':
							oldPen= pDC->SelectObject(&pen_for_veh_red);
							oldBrush= pDC->SelectObject(&brush_for_veh_red);
							break;
						case 'W':
							oldPen= pDC->SelectObject(&pen_for_veh_white);
							oldBrush= pDC->SelectObject(&brush_for_veh_white);
							break;
						case 'K':
							oldPen= pDC->SelectObject(&pen_for_veh_black);
							oldBrush= pDC->SelectObject(&brush_for_veh_black);
							break;
						case 'G':
							oldPen= pDC->SelectObject(&pen_for_veh_green);
							oldBrush= pDC->SelectObject(&brush_for_veh_green);
							break;
						case 'B':
							oldPen= pDC->SelectObject(&pen_for_veh_blue);
							oldBrush= pDC->SelectObject(&brush_for_veh_blue);
							break;
						}
						pDC->Ellipse(x+int(zoom_ratio*VEH_SIZE),
											   y+int(zoom_ratio*VEH_SIZE),
											   x- int(zoom_ratio*VEH_SIZE), 
											   y- int(zoom_ratio*VEH_SIZE) );
					}

				}
			}
		}
	}
	pDC->SelectObject(&oldPen);
	pDC->SelectObject(&oldBrush);
}


void CTrafficDraw::Draw_Cross_Label(int Cross_ID)
{
		CString strLabel;
		if (Cross_Array[Cross_ID]!=NULL && Cross_Array[Cross_ID]->Controller!=NULL)
		{	
			pDC->SetTextColor(COLOR_RED);
			pDC->SetBkMode(TRANSPARENT);
			CString str;
			str.Format("%d", Cross_Array[Cross_ID]->Controller->Current_Control_Info.Cycle_Time );
			pDC->TextOut(Get_New_Coordinate((int)Cross_Array[Cross_ID]->Object_Coodinate.x, 'x'), Get_New_Coordinate((int)Cross_Array[Cross_ID]->Object_Coodinate.y, 'y'), str);
		}

		if (Cross_Array[Cross_ID]!=NULL && Cross_Array[Cross_ID]->Controller!=NULL)
		{	
			CString str;	
			str.Format("C%d-%d     ", Cross_Array[Cross_ID]->Object_ID, Cross_Array[Cross_ID]->Controller->Current_Control_Info.Cycle_Time );
			strLabel=strLabel+ str;
		}
		pDC->SetTextColor(COLOR_WHITE);
		pDC->SetBkMode(TRANSPARENT);
		//	pDC->TextOut(Get_New_Coordinate(1, 'x'), Get_New_Coordinate(1, 'y'), String_Control);	
		pDC->TextOut(0, 0, strLabel);	

}


void CTrafficDraw::Draw_Traffic_Light(int Link_ID)
{
	int j;
	extern char Current_Control_Type;

	if (Link_Array[Link_ID]->End_Object->Object_Type!='M')
	{	
		for(j=0;j<Link_Array[Link_ID]->Lane_Number;j++)
		{
			int Last_Cell= Link_Array[Link_ID]->Lanes[j]->Cell_Number - 1;
			x = (int)(Link_Array[Link_ID]->Lanes[j]->Lane_Cell[Last_Cell]->Cell_Center->x);
			y = (int)(Link_Array[Link_ID]->Lanes[j]->Lane_Cell[Last_Cell]->Cell_Center->y);		
			x= Get_New_Coordinate(x, 'x');
			y= Get_New_Coordinate(y, 'y');

			if (Current_Control_Type=='N')   //no control: yellow
			{
				if (simu_time%4==0 ||simu_time%4==1)
				{
					oldPen= pDC->SelectObject(&pen_for_yellow_light);
					oldBrush= pDC->SelectObject(&brush_for_yellow_light);
				}
				else
				{
					oldPen= pDC->SelectObject(&pen_for_blank);
					oldBrush= pDC->SelectObject(&brush_for_blank);
				}
			}
			else
				if (Link_Array[Link_ID]->Lanes[j]->Phase!=NULL )   //control
				{
					char Current_Color = Link_Array[Link_ID]->Lanes[j]->Phase->Get_Current_Color();
					if (Current_Color=='R' )
					{
						oldPen= pDC->SelectObject(&pen_for_red_light);
						oldBrush= pDC->SelectObject(&brush_for_red_light);
					}
					if (Current_Color=='Y')
					{
						oldPen= pDC->SelectObject(&pen_for_yellow_light);
						oldBrush= pDC->SelectObject(&brush_for_yellow_light);
					}
					if (Current_Color=='G')
					{
						oldPen= pDC->SelectObject(&pen_for_green_light);
						oldBrush= pDC->SelectObject(&brush_for_green_light);
					}
				}
				pDC->Ellipse(x+int(zoom_ratio*LIGHT_SIZE),
										y+int(zoom_ratio*LIGHT_SIZE),
								    	x- int(zoom_ratio*LIGHT_SIZE), 
					                    y- int(zoom_ratio*LIGHT_SIZE) );
		}
	}
	pDC->SelectObject(&oldPen);
	pDC->SelectObject(&oldBrush);
}


void CTrafficDraw::Draw_Conflict_Area(int Cross_ID)
{
	int conflict_x;
	int conflict_y;
	int j;

	CPen pen_for_conflict_cell(PS_SOLID, 5,COLOR_GREEN);
	oldPen= pDC->SelectObject(&pen_for_conflict_cell);
	for (j=0;j<Cross_Array[Cross_ID]->Conflict_Area_Number;j++)
	{
		for(int k=0;k<Conflict_Area_Array[Cross_ID][j]->Conflict_Cell_Number;k++)
		{
			int cross_lane_id=Conflict_Area_Array[Cross_ID][j]->Conflict_Cell_Array[k]->Cross_Lane_ID;
			{
				conflict_x= int(Conflict_Area_Array[Cross_ID][j]->Conflict_Cell_Array[k]->Cell_Center->x);
				conflict_y= int(Conflict_Area_Array[Cross_ID][j]->Conflict_Cell_Array[k]->Cell_Center->y);
				conflict_x= Get_New_Coordinate(conflict_x, 'x');
				conflict_y= Get_New_Coordinate(conflict_y, 'y' );
				pDC->Ellipse(conflict_x+1, conflict_y+1, conflict_x-1, conflict_y-1);
			}
		}
	}

	//draw Main_Cell
	CPen pen_for_conflict_main(PS_SOLID, 5,COLOR_RED);
	oldPen= pDC->SelectObject(&pen_for_conflict_main);
	for (j=0;j<Cross_Array[Cross_ID]->Conflict_Area_Number;j++)
	{
		int cross_lane_id= Conflict_Area_Array[Cross_ID][j]->Main_Cell->Cross_Lane_ID;
		{
			conflict_x= int(Conflict_Area_Array[Cross_ID][j]->Main_Cell->Cell_Center->x);
			conflict_y= int(Conflict_Area_Array[Cross_ID][j]->Main_Cell->Cell_Center->y);
			conflict_x= Get_New_Coordinate(conflict_x, 'x');
			conflict_y= Get_New_Coordinate(conflict_y, 'y' );
			pDC->Ellipse(conflict_x+1, conflict_y+1, conflict_x-1, conflict_y-1);
		}
	}

	//Draw straight conflict cell
	CPen pen_for_straight_conflict(PS_SOLID, 5, COLOR_SAP_GREEN);
	oldPen= pDC->SelectObject(&pen_for_straight_conflict);
	for (j=0; j<Cross_Array[Cross_ID]->Cross_Lane_Number;j++)
	{
		for (int i=0; i<Cross_Lane_Array[Cross_ID][j]->Cell_Number; i++)
		{
			if(Cross_Lane_Array[Cross_ID][j]->Cross_Lane_Cell[i]->Straight_Conflict_Cell!=NULL)
			{
				conflict_x=int(Cross_Lane_Array[Cross_ID][j]->Cross_Lane_Cell[i]->Straight_Conflict_Cell->Cell_Center->x);
				conflict_y=int(Cross_Lane_Array[Cross_ID][j]->Cross_Lane_Cell[i]->Straight_Conflict_Cell->Cell_Center->y);
				conflict_x= Get_New_Coordinate(conflict_x, 'x');
				conflict_y= Get_New_Coordinate(conflict_y, 'y' );
				pDC->Ellipse(conflict_x+1, conflict_y+1, conflict_x-1, conflict_y-1);
			}
		}	
	}
}


