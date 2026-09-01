#include "stdafx.h"
#include "ErrorLog.h"
#include "traffic.h"
#include "SimuFun.h"
#include "math.h"
#include "Veh.h"
#include <new>
#include "Global.h"
#include "Communicator.h"
#include "TrafficMessageBox.h"
#include "SampleCollection.h"
#include "MainFrm.h"
#include "G2Detector.h"
#include "RanDestOrigin.h"
#include "Dest.h"
#include "FixDestOrigin.h"
#include "Commuter.h"

using namespace std;
         
CSimuFun::CSimuFun()
{
	extern char Car_Moving_Model_Type;
	extern int Entering_Boundry;
	extern int Checking_Region;

	if (Car_Moving_Model_Type=='A')
	{
		Max_Lane_Cell_Number=DEFAULT_MAX_LANE_CELL_NUMBER_A;
		Meter_Per_Cell=DEFAULT_METER_PER_CELL_A;
		Pixel_Per_Cell=DEFAULT_CELL_LENGTH_A;
		Entering_Boundry = ENTERING_BOUNDRY_A;
		Checking_Region= CHECKING_REGION_A;
	} 
	else if(Car_Moving_Model_Type=='F')
	{
		Max_Lane_Cell_Number=DEFAULT_MAX_LANE_CELL_NUMBER_F;
		Meter_Per_Cell=DEFAULT_METER_PER_CELL_F;
		Pixel_Per_Cell=DEFAULT_CELL_LENGTH_F;
		Entering_Boundry = ENTERING_BOUNDRY_F;
		Checking_Region= CHECKING_REGION_F;
	}

	Total_Simulation_Time= G_Simulated_Day_Number * G_Day_Length;


//	this->Information_Ready=false;

	int i,j;
	for(i=0;i<MAX_MARGINALPOINT_NUMBER;i++)
		MarginalPoint_Array[i]=NULL;

	for(i=0;i<MAX_CROSS_NUMBER;i++)
		Cross_Array[i]=NULL;
	
	for(i=0;i<MAX_LINK_NUMBER;i++)
		Link_Array[i]=NULL;
	
	for(i=0;i<MAX_MARGINALPOINT_NUMBER+MAX_CROSS_NUMBER;i++)
		for(j=0;j<MAX_MARGINALPOINT_NUMBER+MAX_CROSS_NUMBER;j++)
			Node_Relation[i][j]=NULL;
    
	for(i=0;i<MAX_CROSS_NUMBER;i++)
	{
		for(j=0; j<MAX_CROSS_LANE_NUMBER; j++)
		    Cross_Lane_Array[i][j] = NULL;

		for (j=0;j<MAX_CONFLICT_AREA_NUMBER;j++)
			Conflict_Area_Array[i][j]=NULL;
	}


	Record_Setting();

}

CSimuFun::~CSimuFun()
{
}




//1. Linking_Cross_or_Marginal[]; 
//2. Connected_Link[]; 
//3. Node_Relation[][]
void CSimuFun::Set_Object_Relation()
{
	int i,j,w;
	w=0; 
	CLink *p;
	for(i=0;i<G_Cross_Number;i++)
	{
		w=0;
// process column
		for(j=0;j<MAX_MARGINALPOINT_NUMBER+MAX_CROSS_NUMBER;j++)
		{
			if(Node_Relation[j][i]!=NULL)  //the i th intersection is an end point
			{
				if(j>99)     //means marginal point
				{
					Cross_Array[i]->Linking_Cross_or_Marginal[w] = (CMarginalPoint *)MarginalPoint_Array[j-100];
					Cross_Array[i]->Connected_Link[w*2] = Node_Relation[j][i]->Object_ID ;
				}
				else  //intersection
				{
					Cross_Array[i]->Linking_Cross_or_Marginal[w] = (CCross *)Cross_Array[j];   //OK
					Cross_Array[i]->Connected_Link[w*2] = Node_Relation[j][i]->Object_ID;
				}
				if(Node_Relation[i][j]!=NULL)  //two way link
				{
					Cross_Array[i]->Connected_Link[w*2+1] = Node_Relation[i][j]->Object_ID;
				}
				else   //if one way link, build a virtual link with start point i and end point j
				{ 	
					if(j>99)
					{
						p=new CLink((CCross *)Cross_Array[i], (CMarginalPoint *)MarginalPoint_Array[j-100]);
						p->Rightest_Line_Start = Cross_Array[i]->Object_Coodinate ;
						p->Rightest_Line_End = MarginalPoint_Array[j-100]->Object_Coodinate ;
						p->Leftest_Line_Start = Cross_Array[i]->Object_Coodinate ;
						p->Leftest_Line_End = MarginalPoint_Array[j-100]->Object_Coodinate ;
					}
					else
					{
						p=new CLink((CCross *)Cross_Array[i], (CCross *)Cross_Array[j]);
						p->Rightest_Line_Start = Cross_Array[i]->Object_Coodinate ;
						p->Rightest_Line_End = Cross_Array[j]->Object_Coodinate ;
						p->Leftest_Line_Start = Cross_Array[i]->Object_Coodinate ;
						p->Leftest_Line_End = Cross_Array[j]->Object_Coodinate ;
					}
					p->Lane_Number = 0;
					p->Object_ID = G_Link_Number;
					Cross_Array[i]->Connected_Link[w*2+1] = G_Link_Number;
					Link_Array[G_Link_Number] = p;
					G_Link_Number++;                 //give the virtual link a number.
					Node_Relation[i][j]=p;
				}
			w++;
			}

		}

//process raw
		for(j=0;j<MAX_MARGINALPOINT_NUMBER+MAX_CROSS_NUMBER;j++)
		{
			if(Node_Relation[i][j]!=NULL && Cross_Array[i]->Find_Connected_Link((CLink *)Node_Relation[i][j]) == 0)  //the i th intersection is start point
			{
				if(j>99)
				{
					Cross_Array[i]->Linking_Cross_or_Marginal[w] = (CMarginalPoint *)MarginalPoint_Array[j-100];
					Cross_Array[i]->Connected_Link[w*2] = Node_Relation[i][j]->Object_ID;
					//w++;
				}
				else 
				{
					Cross_Array[i]->Linking_Cross_or_Marginal[w] = (CCross *)Cross_Array[j];
					Cross_Array[i]->Connected_Link[w*2] = Node_Relation[i][j]->Object_ID;
				}

				if(Node_Relation[j][i]!=NULL) 
				{
					Cross_Array[i]->Connected_Link[w*2+1] = Node_Relation[j][i]->Object_ID;
				}
				else
				{
					//virtual link
					if(j>99)
					{
						p=new CLink((CMarginalPoint *)MarginalPoint_Array[j-100],(CCross *)Cross_Array[i]);
						p->Rightest_Line_Start = MarginalPoint_Array[j-100]->Object_Coodinate ;
						p->Rightest_Line_End = Cross_Array[i]->Object_Coodinate ;
						p->Leftest_Line_Start = MarginalPoint_Array[j-100]->Object_Coodinate ;
						p->Leftest_Line_End = Cross_Array[i]->Object_Coodinate ;
					}
					else
					{
						p=new CLink((CCross *)Cross_Array[j],(CCross *)Cross_Array[i]);
						p->Rightest_Line_Start = Cross_Array[j]->Object_Coodinate ;
						p->Rightest_Line_End = Cross_Array[i]->Object_Coodinate ;
						p->Leftest_Line_Start = Cross_Array[j]->Object_Coodinate ;
						p->Leftest_Line_End = Cross_Array[i]->Object_Coodinate ;
					}
					p->Lane_Number = 0;
					p->Object_ID = G_Link_Number;
					Cross_Array[i]->Connected_Link[w*2+1] = G_Link_Number;
					Link_Array[G_Link_Number] = p;
					G_Link_Number++;
					Node_Relation[j][i]=p;
				}
			w++;
			}
		}
		Cross_Array[i]->Connected_Segment_Number = w;  
	}

}

//Counterclockwise sort links connected intersection by 
void CSimuFun::CCwise_Sort_Link()
{
	Cpoint p0,p[5];                   //coordinate of intersections
	double SinAlpha[5];          //sin, between p[i] and x-axis
	double CosAlpha[5];        //cos, between p[i] and x-axis
	double Alpha[5];  
	double pai=3.1415926;
	int i,j,t,w;
	int Segment_Number;
	int temp_link[8];
	int q;
	for(i=0;i<G_Cross_Number;i++)
	{
		Segment_Number = Cross_Array[i]->Connected_Segment_Number; 
		p0 = Cross_Array[i]->Object_Coodinate;
		for(t=0;t<Segment_Number;t++)
			p[t]=Cross_Array[i]->Linking_Cross_or_Marginal[t]->Object_Coodinate;

		for(j=0;j< Segment_Number ;j++)
		{
			SinAlpha[j] = (p[j].y - p0.y)/sqrt((p[j].y - p0.y)*(p[j].y - p0.y) + (p[j].x - p0.x)*(p[j].x - p0.x));
			CosAlpha[j] = (p[j].x - p0.x)/sqrt((p[j].y - p0.y)*(p[j].y - p0.y) + (p[j].x - p0.x)*(p[j].x - p0.x));

			if(SinAlpha[j]>=0 && CosAlpha[j]>=0)  // [0 , pai/2]
				Alpha[j] = asin(SinAlpha[j]);
			if(SinAlpha[j]>=0 && CosAlpha[j]<0)   // (pai/2 , pai]
     			Alpha[j] = pai-asin(SinAlpha[j]);
			if(SinAlpha[j]<0 && CosAlpha[j]<=0)	// (pai , 3pai/2]
				Alpha[j]= pai+asin(-SinAlpha[j]);
			if(SinAlpha[j]<0 && CosAlpha[j]>0)	// (3pai/2 , 2pai)
				Alpha[j]=2*pai-asin(-SinAlpha[j]);
		}
		//sort Alpha[] by ascending order
		for(j=0;j<Segment_Number;j++)
		{
			for(w=0;w< Segment_Number-1;w++)
			{
				if(Alpha[w+1] < Alpha[w])
				{
				//exchange w and w+1
					double ex_w;
					int temp1,temp2;
					CMyObject *ex_p;

					//1st: w+1-> temp
					ex_w = Alpha[w+1];
					if(Cross_Array[i]->Linking_Cross_or_Marginal[w+1]->Object_Type == 'C')
						ex_p = (CCross *)Cross_Array[i]->Linking_Cross_or_Marginal[w+1];
					else
						ex_p = (CMarginalPoint *)Cross_Array[i]->Linking_Cross_or_Marginal[w+1];
					temp1 = Cross_Array[i]->Connected_Link[(w+1)*2];
					temp2 = Cross_Array[i]->Connected_Link[(w+1)*2+1];

					 //2nd: w->w+1
					Alpha[w+1] = Alpha[w];
					if(Cross_Array[i]->Linking_Cross_or_Marginal[w]->Object_Type == 'C')
						Cross_Array[i]->Linking_Cross_or_Marginal[w+1] = (CCross *)Cross_Array[i]->Linking_Cross_or_Marginal[w];
					else
						Cross_Array[i]->Linking_Cross_or_Marginal[w+1] = (CMarginalPoint *)Cross_Array[i]->Linking_Cross_or_Marginal[w];
					Cross_Array[i]->Connected_Link[(w+1)*2] = Cross_Array[i]->Connected_Link[w*2] ;
					Cross_Array[i]->Connected_Link[(w+1)*2+1] = Cross_Array[i]->Connected_Link[w*2+1] ;

					//3rd: temp->w
					Alpha[w] = ex_w;
					if(Cross_Array[i]->Linking_Cross_or_Marginal[w]->Object_Type == 'C')
						Cross_Array[i]->Linking_Cross_or_Marginal[w] = (CCross *)ex_p;
					else
						Cross_Array[i]->Linking_Cross_or_Marginal[w] = (CMarginalPoint *)ex_p;
					Cross_Array[i]->Connected_Link[w*2] = temp1;
					Cross_Array[i]->Connected_Link[w*2+1] = temp2;
				}
			}
		}

			for(q=0;q < Segment_Number;q++)
			{
				if(Link_Array[Cross_Array[i]->Connected_Link[q*2]]->End_Object == Cross_Array[i])
				{
					temp_link[2*q] = Cross_Array[i]->Connected_Link[2*q];
					temp_link[(2*q-1+Segment_Number*2)%(Segment_Number*2)] = Cross_Array[i]->Connected_Link[2*q+1];
				}
				else
				{
					temp_link[2*q] = Cross_Array[i]->Connected_Link[2*q+1];
					temp_link[(2*q-1+Segment_Number*2)%(Segment_Number*2)] = Cross_Array[i]->Connected_Link[2*q];
				}
			}

			//copy temp_link to Cross_Array[i]->Connected_Link[]
			for(q=0;q< Segment_Number*2; q++)
				Cross_Array[i]->Connected_Link[q] = temp_link[q];
	}
}

void CSimuFun::Set_Lane_Middle_Line()
{
	int i,j;
	for(i=0;i<G_Link_Number;i++)
	{
		for(j=0;j<Link_Array[i]->Lane_Number;j++)
		{
			if(j==0)
			{
				Link_Array[i]->Lane_Middle_Start_Point[j] = Get_Middle_Point(Link_Array[i]->Leftest_Line_Start, Link_Array[i]->Lane_Start_Point[j]);
				Link_Array[i]->Lane_Middle_End_Point[j] = Get_Middle_Point(Link_Array[i]->Leftest_Line_End, Link_Array[i]->Lane_End_Point[j]);
			}
			else
			{
				Link_Array[i]->Lane_Middle_Start_Point[j] = Get_Middle_Point(Link_Array[i]->Lane_Start_Point[j], Link_Array[i]->Lane_Start_Point[j-1]);
				Link_Array[i]->Lane_Middle_End_Point[j] = Get_Middle_Point(Link_Array[i]->Lane_End_Point[j], Link_Array[i]->Lane_End_Point[j-1]);
			}
		}
	}
}

//1. Set the point on the intersection outline. We draw the intersection by connecting the point.
//2. meanwhile, set the coordinate in the properties of link.
void CSimuFun::Set_Cross_Edge_Point()
{
	CCross *cp;
	CLink *R[8];
	Cpoint p[8],q,temp_p,pp[8],temp_q[4],q1;
	int i,j,w,t;
	double w1,w2;
	CLink *temp_R,*temp_R_1;
	int Segment_Number;
	for(i=0;i<8;i++)
	{
		p[i].x =-1;  pp[i].x = -1;   
		p[i].y =-1;  pp[i].y = -1;
	}
	for(w=0;w<G_Cross_Number;w++)           //iterate intersections
	{
		cp=Cross_Array[w];
		Segment_Number = cp->Connected_Segment_Number ;
		for(i=0;i<Segment_Number*2;i++)   //save the link connected intersection to R[]
		{	
			//Connected_Link[]---counterclockwise, link number corresponding to node
			//cp->Connected_Link[i] corresponding crossing number.
			R[i] = Link_Array[cp->Connected_Link[i]];  
		}
		for(i=0;i<Segment_Number;i++)
		{
			p[i] = Get_Intersection_Point(R[2*i]->Rightest_Line_Start  ,   //R[0 2 4 6]
																R[2*i]->Rightest_Line_End ,
																R[2*i+1]->Rightest_Line_Start ,    //R[1 3 5 7]
																R[2*i+1]->Rightest_Line_End );  //intersection of two lines
		}
		Cpoint Temp_Vertical_Point_1;
		Cpoint Temp_Vertical_Point_2;
		for(i=0,j=0;i<Segment_Number;i++,j=j+2)
		{	
			Temp_Vertical_Point_1=Get_Vertical_Point(Cross_Array[w]->Linking_Cross_or_Marginal[i]->Object_Coodinate, p[i],  R[j]->Rightest_Line_Start );	
			w1 = Point_Distance(p[i],Temp_Vertical_Point_1 ); 

			Temp_Vertical_Point_2=Get_Vertical_Point(Cross_Array[w]->Linking_Cross_or_Marginal[i]->Object_Coodinate,
																							p[(i-1+Segment_Number)%Segment_Number], 
																							R[(j-1+Segment_Number*2)%(Segment_Number*2)]->Rightest_Line_End );	
			w2 = Point_Distance(p[(i-1+Segment_Number)%Segment_Number],Temp_Vertical_Point_2 );
			
			if(w1 <= w2)
			{

				//one link
				temp_R = R[(j-1+Segment_Number*2)%(Segment_Number*2)];

				q = Get_Vertical_Point(p[i],temp_R->Leftest_Line_Start , temp_R->Leftest_Line_End );
	
				temp_R ->Leftest_Line_Start = q;
				for(t=0; t<temp_R->Lane_Number ;t++)
				{
					q = Get_Vertical_Point(p[i],temp_R->Lane_Middle_Start_Point[t], temp_R->Lane_Middle_End_Point[t]);
					temp_R->Lane_Middle_Start_Point[t] = q;
				}
				for(t=0; t<temp_R->Lane_Number ;t++)
				{
					q = Get_Vertical_Point(p[i],temp_R->Lane_Start_Point[t] , temp_R->Lane_End_Point[t]);
					temp_R->Lane_Start_Point[t] = q;
				}
				temp_R->Rightest_Line_Start = q;
				temp_q[(i-1+Segment_Number)%Segment_Number] =q;

				
				//another link
				temp_R_1 = R[j];

				q1 = Get_Vertical_Point(p[i],temp_R_1->Leftest_Line_Start , temp_R_1->Leftest_Line_End );
				temp_R_1 ->Leftest_Line_End = q1;
				for(t=0; t<temp_R_1->Lane_Number ;t++)
				{
					q1 = Get_Vertical_Point(p[i],temp_R_1->Lane_Middle_Start_Point[t], temp_R_1->Lane_Middle_End_Point[t]);
					temp_R_1->Lane_Middle_End_Point[t] = q1;
				}
				for(t=0; t<temp_R_1->Lane_Number ;t++)
				{
					q1 = Get_Vertical_Point(p[i],temp_R_1->Lane_Start_Point[t] , temp_R_1->Lane_End_Point[t]);
					temp_R_1->Lane_End_Point[t] = q1;
				}
				temp_R_1->Rightest_Line_End = q1;
			}
			else
			{
				//one link
				temp_R= R[j];
				temp_p = p[(i-1+Segment_Number)%Segment_Number];
				q = Get_Vertical_Point(temp_p,temp_R->Leftest_Line_Start , temp_R->Leftest_Line_End );
				temp_R->Leftest_Line_End = q;
				for(t=0;t<temp_R->Lane_Number ;t++)
				{
					q = Get_Vertical_Point(temp_p,temp_R->Lane_Middle_Start_Point[t], temp_R->Lane_Middle_End_Point[t]);
					temp_R->Lane_Middle_End_Point[t] = q;
				}
				for(t=0; t<temp_R->Lane_Number ;t++)
				{
					q=Get_Vertical_Point(temp_p,temp_R->Lane_Start_Point[t], temp_R->Lane_End_Point[t]);
					temp_R->Lane_End_Point[t] = q;
				}
				temp_R->Rightest_Line_End = q;
				temp_q[(i-1+Segment_Number)%Segment_Number] = q;

				//another link
				temp_R_1= R[(j-1+Segment_Number*2)%(Segment_Number*2)];
				temp_p = p[(i-1+Segment_Number)%Segment_Number];
				q1 = Get_Vertical_Point(temp_p,temp_R_1->Leftest_Line_Start , temp_R_1->Leftest_Line_End );
				temp_R_1->Leftest_Line_Start = q1;
				for(t=0;t<temp_R_1->Lane_Number ;t++)
				{
					q1 = Get_Vertical_Point(temp_p,temp_R_1->Lane_Middle_Start_Point[t], temp_R_1->Lane_Middle_End_Point[t]);
					temp_R_1->Lane_Middle_Start_Point[t] = q1;
				}
				for(t=0; t<temp_R_1->Lane_Number ;t++)
				{
					q1=Get_Vertical_Point(temp_p,temp_R_1->Lane_Start_Point[t], temp_R_1->Lane_End_Point[t]);
					temp_R_1->Lane_Start_Point[t] = q1;
				}
				temp_R_1->Rightest_Line_Start = q1;
			}
		}
		//put corresponding points into  Cross_Array[w]->Connected_Link[]
		for(t=0;t<Segment_Number; t++)
		{
			pp[t*2] = p[t];         //pp[0--2(2s-1)],  p[0--2s]
			pp[t*2+1] = temp_q[t]; 
		}
		for(t=0;t<Segment_Number*2;t++)
		{
			cp->Cross_Point[t] = pp[t];
		}
	}
}


void CSimuFun::OnInit_Object()
{
	extern CString Open_File_PathName;
	//instantiate  DOM, and load xml file
	//	char* XML_PathName="..\\Traffic\\Sample\\Test1.4.2.xml";
	MSXML2::IXMLDOMDocumentPtr  pDOM;   
    HRESULT hr = pDOM.CreateInstance(__uuidof(MSXML2::DOMDocument30));
    if(!SUCCEEDED(hr))     
		AfxMessageBox("#Failed when loading XML file");     
	if(!pDOM->load(_variant_t(Open_File_PathName)))
		AfxMessageBox("#Failed when loading traffic network");

	G_MarginalPoint_Number = Get_XML_MarginalPoint(pDOM);
	G_Cross_Number = Get_XML_Cross(pDOM );
	G_Link_Number=Get_XML_Link(pDOM);
	G_Lane_Number=Get_XML_Lane(pDOM);
	G_Demand_Number =Get_XML_Demand(pDOM);
	G_FixedOD_Number=Get_XML_OD(pDOM);
	Set_Linking_Object();
	Get_XML_Controller(pDOM); 
}

void CSimuFun::Set_Linking_Object()
{
	///put CLink into table
	for(int i=0;i<G_Link_Number;i++)
		if(Link_Array[i]->Start_Object->Object_Type == 'M')
			if(Link_Array[i]->End_Object->Object_Type == 'M')  //M,M
				Node_Relation[100 + Link_Array[i]->Start_Object->Object_ID][100 + Link_Array[i]->End_Object->Object_ID] = Link_Array[i];
			else   //M,C
				Node_Relation[100 + Link_Array[i]->Start_Object->Object_ID][Link_Array[i]->End_Object->Object_ID] = Link_Array[i];
		else	
			if(Link_Array[i]->End_Object->Object_Type == 'M')  //C, M
				Node_Relation[Link_Array[i]->Start_Object->Object_ID][100 + Link_Array[i]->End_Object->Object_ID] = Link_Array[i];			
			else  //C, C
				Node_Relation[Link_Array[i]->Start_Object->Object_ID][Link_Array[i]->End_Object->Object_ID] = Link_Array[i];				
}

//enlarge links; get start point, end points, edge coordinate, etc. of links
void CSimuFun::Enlarge_Link()
{
	for (int i=0;i<G_Link_Number;i++)
		Link_Array[i]->Enlarge_Link();
}

int CSimuFun::Get_XML_Cross(MSXML2::IXMLDOMDocumentPtr  pDOMDoc)
{
	MSXML2::IXMLDOMDocumentPtr  pDOM=pDOMDoc;
	MSXML2::IXMLDOMNodePtr pSubNode;   
	_bstr_t  strRequestType;
	int i=0;
	CCross ** pCA;                        //pointer which points to the address of a pointer array   //* is pointer which points to structure .
	pCA=&Cross_Array[0];           //give the address of pointer array to MPA
	CCross *p;
	/////////////////////////////////////
	//	Object_ID
	//	Object_Type
	//	Object_Label
	//	Connected_Segment_Number
	//	x
	//	y
	/////////////////////////////////////
    MSXML2::IXMLDOMNodeListPtr pNodeList_Cross_ID = NULL;     ///search Object_ID in Data/Cross
    pNodeList_Cross_ID = pDOM->selectNodes("Data/Cross");

    MSXML2::IXMLDOMNodeListPtr pNodeList_Cross_Label = NULL;   ///search Cross_Label
    pNodeList_Cross_Label = pDOM->selectNodes("Data/Cross"); 

    MSXML2::IXMLDOMNodeListPtr pNodeList_Cross_LSN = NULL;   ///search Cross_Linking_Segment_Number
    pNodeList_Cross_LSN = pDOM->selectNodes("Data/Cross"); 

    MSXML2::IXMLDOMNodeListPtr pNodeList_Cross_x = NULL;      ///search Cross_x
    pNodeList_Cross_x = pDOM->selectNodes("Data/Cross"); 

    MSXML2::IXMLDOMNodeListPtr pNodeList_Cross_y = NULL;      ///search Cross_y 
    pNodeList_Cross_y = pDOM->selectNodes("Data/Cross"); 

	if(MAX_CROSS_NUMBER<pNodeList_Cross_ID->Getlength())
		AfxMessageBox("Fault, too many intersections");

	for(i=0;i<pNodeList_Cross_ID->Getlength();i++)   
	{   
		p=new CCross();

		pSubNode=pNodeList_Cross_ID->nextNode()->selectSingleNode("Object_ID");  
		strRequestType=pSubNode->Gettext(); 
		p->Object_ID = atoi(strRequestType.operator char*()  );   //atoi, string to int
		p->Object_ID= Deal_with_Deviation_XML(p->Object_ID, DEVIATION_XML);  

		pSubNode=pNodeList_Cross_Label->nextNode()->selectSingleNode("Object_Label");  
		strRequestType=pSubNode->Gettext(); 
		p->Object_Label = strRequestType.operator char*()  ;   

//		pSubNode=pNodeList_Cross_Type->nextNode()->selectSingleNode("Cross_Type");  
//		strRequestType=pSubNode->Gettext(); 
//		p->Cross_Type = atoi(strRequestType.operator char*())  ;   

		pSubNode=pNodeList_Cross_LSN->nextNode()->selectSingleNode("Connected_Segment_Number");  
		strRequestType=pSubNode->Gettext(); 
		p->Connected_Segment_Number = atoi(strRequestType.operator char*())  ;   

		pSubNode=pNodeList_Cross_x->nextNode()->selectSingleNode("x");  
		strRequestType=pSubNode->Gettext(); 
		p->Object_Coodinate.x = atof(strRequestType.operator char*()  );   //atof: string to float

		pSubNode=pNodeList_Cross_y->nextNode()->selectSingleNode("y");  
		strRequestType=pSubNode->Gettext(); 
		p->Object_Coodinate.y = atof(strRequestType.operator char*()  );  

		p->Object_Type='C';

		pCA[i] = p;
		}
	    return  pNodeList_Cross_ID->Getlength();  //return  Cross_Number
}


int CSimuFun::Get_XML_MarginalPoint(MSXML2::IXMLDOMDocumentPtr  pDOMDoc )
{
	MSXML2::IXMLDOMDocumentPtr  pDOM=pDOMDoc;
    MSXML2::IXMLDOMNodePtr pSubNode;   
	_bstr_t  strRequestType;
	int i=0;  
	CMarginalPoint ** pMPA; 
	pMPA=&MarginalPoint_Array[0];
	CMarginalPoint *p;
	/////////////////////////////////////
	//	Object_ID
	//	Object_Type
	//	Object_Label
	//	x
	//	y
	/////////////////////////////////////

    MSXML2::IXMLDOMNodeListPtr pNodeList_MarginalPoint_x = NULL; 
    pNodeList_MarginalPoint_x = pDOM->selectNodes("Data/MarginalPoint"); 
 	MSXML2::IXMLDOMNodeListPtr pNodeList_MarginalPoint_y = NULL;
  	pNodeList_MarginalPoint_y = pDOM->selectNodes("Data/MarginalPoint");    
	MSXML2::IXMLDOMNodeListPtr pNodeList_MarginalPoint_ID = NULL;
    pNodeList_MarginalPoint_ID = pDOM->selectNodes("Data/MarginalPoint");
	MSXML2::IXMLDOMNodeListPtr pNodeList_MarginalPoint_Label = NULL;
    pNodeList_MarginalPoint_Label = pDOM->selectNodes("Data/MarginalPoint");	

	if(MAX_MARGINALPOINT_NUMBER<pNodeList_MarginalPoint_ID->Getlength())
		AfxMessageBox("Fault, too many marginal points");

	int Length= pNodeList_MarginalPoint_ID->Getlength();
	for(i=0;i<Length;i++)   
	{   
		p=new CMarginalPoint();
		
		pSubNode=pNodeList_MarginalPoint_x->nextNode()->selectSingleNode("x");  
	    strRequestType=pSubNode->Gettext(); 
	    p->Object_Coodinate.x=atof(strRequestType.operator char*()  ); 

		pSubNode=pNodeList_MarginalPoint_y->nextNode()->selectSingleNode("y");  
	    strRequestType=pSubNode->Gettext();  
	    p->Object_Coodinate.y=atof(strRequestType.operator char*()  );

		pSubNode=pNodeList_MarginalPoint_ID->nextNode()->selectSingleNode("Object_ID");  
		strRequestType=pSubNode->Gettext(); 
		p->Object_ID= atoi(strRequestType.operator char*()  );
		p->Object_ID= Deal_with_Deviation_XML(p->Object_ID, DEVIATION_XML);

		pSubNode=pNodeList_MarginalPoint_Label->nextNode()->selectSingleNode("Object_Label");  
	    strRequestType=pSubNode->Gettext(); 
	    p->Object_Label = strRequestType.operator char*()  ; 

		p->Object_Type = 'M';

		pMPA[i] = p;
	} 
	return  pNodeList_MarginalPoint_ID->Getlength();  //return  MarginalPoint_Number
}

int Guidance_Number=0;

int CSimuFun::Get_XML_Link(MSXML2::IXMLDOMDocumentPtr  pDOMDoc )
{
	MSXML2::IXMLDOMDocumentPtr  pDOM=pDOMDoc;
    MSXML2::IXMLDOMNodePtr pSubNode;   
	_bstr_t  strRequestType;
	int i=0; 
	int Guidance_or_Not=0;
	int Detector_Location=-1;
	CLink ** pRA; 
	pRA=&Link_Array[0];
	CLink *p;

	CMarginalPoint ** pMPA;  
	pMPA=&MarginalPoint_Array[0];  

	CCross ** pCA;  
	pCA=&Cross_Array[0];  
	/////////////////////////////////////
	//	Object_ID
	//	Object_Type   R
	//	Object_Label
	//  Lane_Number
	//	<Link_Start>
	//		<Object_Type>
	//		<Object_ID>
	//	<Link_End>
	//		<Object_Type>
	//		<Object_ID>
	/////////////////////////////////////

	MSXML2::IXMLDOMNodeListPtr pNodeList_Object_ID = NULL; 
    pNodeList_Object_ID = pDOM->selectNodes("Data/Link");
	
	MSXML2::IXMLDOMNodeListPtr pNodeList_Object_Label = NULL;
    pNodeList_Object_Label = pDOM->selectNodes("Data/Link"); 
	
	MSXML2::IXMLDOMNodeListPtr pNodeList_Lane_Number = NULL;
    pNodeList_Lane_Number = pDOM->selectNodes("Data/Link"); 

	MSXML2::IXMLDOMNodeListPtr pNodeList_Is_Origin = NULL;
	pNodeList_Is_Origin = pDOM->selectNodes("Data/Link"); 	

	MSXML2::IXMLDOMNodeListPtr pNodeList_Is_Dest = NULL;
	pNodeList_Is_Dest = pDOM->selectNodes("Data/Link"); 	

	MSXML2::IXMLDOMNodeListPtr pNodeList_Limited_Speed = NULL;
	pNodeList_Limited_Speed = pDOM->selectNodes("Data/Link"); 	

	MSXML2::IXMLDOMNodeListPtr pNodeList_Link_Start_Object_Type = NULL;
    pNodeList_Link_Start_Object_Type = pDOM->selectNodes("Data/Link/Link_Start"); 

	MSXML2::IXMLDOMNodeListPtr pNodeList_Link_Start_Object_ID = NULL; 
    pNodeList_Link_Start_Object_ID = pDOM->selectNodes("Data/Link/Link_Start"); 
	
	MSXML2::IXMLDOMNodeListPtr pNodeList_Link_End_Object_Type = NULL;
    pNodeList_Link_End_Object_Type = pDOM->selectNodes("Data/Link/Link_End"); 

	MSXML2::IXMLDOMNodeListPtr pNodeList_Link_End_Object_ID = NULL; 
    pNodeList_Link_End_Object_ID = pDOM->selectNodes("Data/Link/Link_End"); 	

	MSXML2::IXMLDOMNodeListPtr pNodeList_Link_Guidance_or_Not = NULL;
    pNodeList_Link_Guidance_or_Not = pDOM->selectNodes("Data/Link"); 	
	
	MSXML2::IXMLDOMNodeListPtr pNodeList_Link_Detector_Location_one = NULL;
    pNodeList_Link_Detector_Location_one = pDOM->selectNodes("Data/Link/Detector_Location"); 	

	MSXML2::IXMLDOMNodeListPtr pNodeList_Link_Detector_Location_two = NULL;
    pNodeList_Link_Detector_Location_two = pDOM->selectNodes("Data/Link/Detector_Location"); 	

	MSXML2::IXMLDOMNodeListPtr pNodeList_Link_Detector_Location_three = NULL;
    pNodeList_Link_Detector_Location_three = pDOM->selectNodes("Data/Link/Detector_Location"); 	

	int Length= pNodeList_Object_ID->Getlength();
	
	if(MAX_LINK_NUMBER<Length)
		AfxMessageBox("Fault, too many marginal points");

	int Link_End_ID;
	char Link_End_Type;
	int Link_Start_ID;
	char Link_Start_Type;
	bool Is_Origin;
	bool Is_Dest;
	int Limited_Speed;

	for(i=0;i<Length;i++)   
	{   		
		//Link_Start_Object
		pSubNode=pNodeList_Link_Start_Object_ID->nextNode()->selectSingleNode("Object_ID");  
		strRequestType=pSubNode->Gettext(); 
		Link_Start_ID = atoi(strRequestType.operator char*()  );   		
		Link_Start_ID= Deal_with_Deviation_XML(Link_Start_ID, DEVIATION_XML);

		pSubNode=pNodeList_Link_Start_Object_Type->nextNode()->selectSingleNode("Object_Type");  
		strRequestType=pSubNode->Gettext();
		Link_Start_Type = *(strRequestType.operator char*() +0) ;   // get the first of char*, and transfer into char

		//Link_End_Object
		pSubNode=pNodeList_Link_End_Object_ID->nextNode()->selectSingleNode("Object_ID");  
		strRequestType=pSubNode->Gettext(); 
		Link_End_ID = atoi(strRequestType.operator char*()  );  
		Link_End_ID= Deal_with_Deviation_XML(Link_End_ID, DEVIATION_XML);

		//Is_Origin
		pSubNode=pNodeList_Is_Origin->nextNode()->selectSingleNode("Is_Origin");  
		strRequestType=pSubNode->Gettext();
		int Is_Origin_or_Not= atoi(strRequestType.operator char*());
		if(Is_Origin_or_Not==1)
			Is_Origin= true;
		else
			Is_Origin=false;

		//Is_Dest
		pSubNode=pNodeList_Is_Dest->nextNode()->selectSingleNode("Is_Dest");  
		strRequestType=pSubNode->Gettext();
		int Is_Dest_or_Not= atoi(strRequestType.operator char*());
		if(Is_Dest_or_Not==1)
			Is_Dest= true;
		else
			Is_Dest=false;

		//Limited_Speed
		pSubNode=pNodeList_Limited_Speed->nextNode()->selectSingleNode("Limited_Speed");  
		strRequestType=pSubNode->Gettext();
		Limited_Speed = atoi(strRequestType.operator char*() );  
		Limited_Speed = (int)(Limited_Speed/Pixel_Per_Cell);

		//Object_Type
		pSubNode=pNodeList_Link_End_Object_Type->nextNode()->selectSingleNode("Object_Type");  
		strRequestType=pSubNode->Gettext(); 
		Link_End_Type= *(strRequestType.operator char*() +0) ; 
		if(Link_Start_Type=='M')
		{
			if(Link_End_Type=='M')  //MM
					p=new CLink(pMPA[Link_Start_ID] , pMPA[Link_End_ID] );
			else  //MC
					p=new CLink( pMPA[Link_Start_ID] , pCA[Link_End_ID] );
		}
		else
		{
			if(Link_End_Type=='C') //CC
					p=new CLink(pCA[Link_Start_ID] , pCA[Link_End_ID] );
			else  //CM
					p=new CLink(pCA[Link_Start_ID] , pMPA[Link_End_ID] );  
		}
		pSubNode=pNodeList_Object_ID->nextNode()->selectSingleNode("Object_ID");  
		strRequestType=pSubNode->Gettext(); 
		p->Object_ID= atoi(strRequestType.operator char*()  );  
		p->Object_ID= Deal_with_Deviation_XML(p->Object_ID, DEVIATION_XML);

		p->Object_Type='R';
		p->Is_Origin=Is_Origin;
		p->Is_Dest=Is_Dest;
		p->Limited_Speed=Limited_Speed;

		pSubNode=pNodeList_Object_Label->nextNode()->selectSingleNode("Object_Label");  
		strRequestType=pSubNode->Gettext(); 
		p->Object_Label = strRequestType.operator char*()  ; 

		pSubNode=pNodeList_Lane_Number->nextNode()->selectSingleNode("Lane_Number");  
		strRequestType=pSubNode->Gettext(); 
		p->Lane_Number = atoi(strRequestType.operator char*() ) ; 

		pSubNode=pNodeList_Link_Guidance_or_Not->nextNode()->selectSingleNode("Guidance_or_Not");  
		strRequestType=pSubNode->Gettext(); 
		Guidance_or_Not = atoi(strRequestType.operator char*() ) ; 
		if (Guidance_or_Not==1)
		{
			p->guidance= new CGuidance(Guidance_Number, p->Object_ID); 
			Guidance_Number++;
		}
		else
			p->guidance= NULL;

		///////////////////////////////
		//Note: there is no detector on the link which one of its end is marginal point.
		if (p->End_Object->Object_Type=='C')
		{
			pSubNode=pNodeList_Link_Detector_Location_one->nextNode()->selectSingleNode("One");  
			strRequestType=pSubNode->Gettext();
			Detector_Location= atoi(strRequestType.operator char*());
			if (Detector_Location >=0 || Detector_Location ==DETECTOR_IN_BEGINNING) 
				p->Detector_Array[0]= new CDetector(p->Object_ID, 0,  Detector_Location);
			
			pSubNode=pNodeList_Link_Detector_Location_two->nextNode()->selectSingleNode("Two");  
			strRequestType=pSubNode->Gettext();
			Detector_Location= atoi(strRequestType.operator char*());
			if (Detector_Location >=0 || Detector_Location ==DETECTOR_IN_BEGINNING) 
				p->Detector_Array[1]= new CDetector(p->Object_ID, 1, Detector_Location);

			pSubNode=pNodeList_Link_Detector_Location_three->nextNode()->selectSingleNode("Three");  
			strRequestType=pSubNode->Gettext();
			Detector_Location= atoi(strRequestType.operator char*());
			if (Detector_Location >=0 || Detector_Location ==DETECTOR_IN_BEGINNING)
				p->Detector_Array[2]= new CDetector(p->Object_ID, 2, Detector_Location);
		}
		else
		{
			for (int m=0;m<MAX_DETECTOR_NUMBER;m++)
				p->Detector_Array[0]=NULL;
		}

		pRA[i] = p;
	} 
	return  pNodeList_Object_ID->Getlength();   //return  Link_Number
}

int CSimuFun::Get_XML_Lane(MSXML2::IXMLDOMDocumentPtr  pDOMDoc)
{
	MSXML2::IXMLDOMDocumentPtr  pDOM=pDOMDoc;
	MSXML2::IXMLDOMNodePtr pSubNode;   
	_bstr_t  strRequestType;
	int i=0; 
	int w;

//	MSXML2::IXMLDOMNodeListPtr pNodeList_Lane_SN = NULL;   
//	pNodeList_Lane_SN = pDOM->selectNodes("Data/Lane"); 

    MSXML2::IXMLDOMNodeListPtr pNodeList_Link_ID = NULL;   
    pNodeList_Link_ID = pDOM->selectNodes("Data/Lane");


    MSXML2::IXMLDOMNodeListPtr pNodeList_Lane_ID = NULL; 
    pNodeList_Lane_ID = pDOM->selectNodes("Data/Lane"); 

    MSXML2::IXMLDOMNodeListPtr pNodeList_Left_Turn = NULL;   
	pNodeList_Left_Turn = pDOM->selectNodes("Data/Lane"); 

    MSXML2::IXMLDOMNodeListPtr pNodeList_Straight_Turn = NULL; 
    pNodeList_Straight_Turn = pDOM->selectNodes("Data/Lane"); 

    MSXML2::IXMLDOMNodeListPtr pNodeList_Right_Turn = NULL; 
    pNodeList_Right_Turn = pDOM->selectNodes("Data/Lane"); 
	

	for(i=0;i<pNodeList_Link_ID->Getlength();i++)   
	{   
        int Link_ID;
		pSubNode=pNodeList_Link_ID->nextNode()->selectSingleNode("Link_ID");  
		strRequestType=pSubNode->Gettext(); 
		Link_ID = atoi(strRequestType.operator char*()  ); 
		Link_ID= Deal_with_Deviation_XML(Link_ID, DEVIATION_XML);

		pSubNode=pNodeList_Lane_ID->nextNode()->selectSingleNode("Lane_ID");  
		strRequestType=pSubNode->Gettext(); 
		w = atoi(strRequestType.operator char*());
		Link_Array[Link_ID]->Lanes[w]->Lane_ID = w  ;   

		pSubNode=pNodeList_Left_Turn->nextNode()->selectSingleNode("Left_Turn");  
		strRequestType=pSubNode->Gettext(); 
		if(atoi(strRequestType.operator char*()) == 0)
			Link_Array[Link_ID]->Lanes[w]->Left_Turn = false  ;
		else
			Link_Array[Link_ID]->Lanes[w]->Left_Turn = true;
		
		pSubNode=pNodeList_Straight_Turn->nextNode()->selectSingleNode("Straight_Turn");  
		strRequestType=pSubNode->Gettext(); 
		if(atoi(strRequestType.operator char*()) == 0)
			Link_Array[Link_ID]->Lanes[w]->Straight_Turn = false  ;
		else
			Link_Array[Link_ID]->Lanes[w]->Straight_Turn = true;
		
		pSubNode=pNodeList_Right_Turn->nextNode()->selectSingleNode("Right_Turn");  
		strRequestType=pSubNode->Gettext(); 
		if(atoi(strRequestType.operator char*()) == 0)
			Link_Array[Link_ID]->Lanes[w]->Right_Turn = false  ;
		else
			Link_Array[Link_ID]->Lanes[w]->Right_Turn = true;
	}
    return  pNodeList_Link_ID->Getlength();
}

int CSimuFun::Get_XML_Demand(MSXML2::IXMLDOMDocumentPtr  pDOMDoc)
{
	MSXML2::IXMLDOMDocumentPtr  pDOM=pDOMDoc;
	MSXML2::IXMLDOMNodePtr pSubNode;   
	_bstr_t  strRequestType;
	int i=0; 
	
	MSXML2::IXMLDOMNodeListPtr pNodeList_Time = NULL;   
	pNodeList_Time = pDOM->selectNodes("Data/Demand");

	MSXML2::IXMLDOMNodeListPtr pNodeList_Value = NULL;   
	pNodeList_Value = pDOM->selectNodes("Data/Demand");

	int length=pNodeList_Time->Getlength();
	for(i=0;i<length;i++)   
	{   
		double Time;
		pSubNode=pNodeList_Time->nextNode()->selectSingleNode("Time");  
		strRequestType=pSubNode->Gettext(); 
		Time = atof(strRequestType.operator char*()); 
		Demand_Array[i][0]=Time;

		double Value;
		pSubNode=pNodeList_Value->nextNode()->selectSingleNode("Value");  
		strRequestType=pSubNode->Gettext(); 
		Value = atof(strRequestType.operator char*()); 
		Demand_Array[i][1]=Value;
	}

	return  length;
}


int CSimuFun::Get_XML_OD(MSXML2::IXMLDOMDocumentPtr  pDOMDoc)
{
	MSXML2::IXMLDOMDocumentPtr  pDOM=pDOMDoc;
	MSXML2::IXMLDOMNodePtr pSubNode;   
	_bstr_t  strRequestType;

	
	extern Struct_FixedOD FixedOD_Array[MAX_FixedOD_NUMBER];

	MSXML2::IXMLDOMNodeListPtr pNodeList_FixedOD = NULL;   
	pNodeList_FixedOD = pDOM->selectNodes("Data/FixedOD");

	MSXML2::IXMLDOMNodeListPtr pNodeList_Origin= NULL;   
	pNodeList_Origin = pDOM->selectNodes("Data/FixedOD");

	MSXML2::IXMLDOMNodeListPtr pNodeList_Destination= NULL;   
	pNodeList_Destination = pDOM->selectNodes("Data/FixedOD");

	MSXML2::IXMLDOMNodeListPtr pNodeList_Demand= NULL;   
	pNodeList_Demand = pDOM->selectNodes("Data/FixedOD");

	int length= pNodeList_FixedOD->Getlength();
	for(int i=0;i<length; i++)   
	{   
		int Origin_ID;
		pSubNode=pNodeList_Origin->nextNode()->selectSingleNode("Origin");  
		strRequestType=pSubNode->Gettext(); 
		Origin_ID = atoi(strRequestType.operator char*()); 
		
		int Dest_ID;
		pSubNode=pNodeList_Destination->nextNode()->selectSingleNode("Destination");  
		strRequestType=pSubNode->Gettext(); 
		Dest_ID = atoi(strRequestType.operator char*()); 

		int Demand;
		pSubNode=pNodeList_Demand->nextNode()->selectSingleNode("Demand");  
		strRequestType=pSubNode->Gettext(); 
		Demand = atoi(strRequestType.operator char*()); 
		
		FixedOD_Array[i].Origin=Origin_ID;
		FixedOD_Array[i].Dest=Dest_ID;
		FixedOD_Array[i].Demand=Demand;
	}

	return  length;
}

void CSimuFun::Set_Lane_Cell_Coordinate()
{
	int i,j;
	
	for(i=0;i<G_Link_Number;i++)
	{
		int Cell_Number=0;
		for(j=0;j<Link_Array[i]->Lane_Number;j++)
		{
			if(Link_Array[i]->Lanes[j] == NULL)
				AfxMessageBox("Error---CSimuFun::Set_Lane_Cell_Coordinate()");
			else
				Cell_Number= Link_Array[i]->Lanes[j]->Set_Value(Link_Array[i]->Lane_Middle_Start_Point[j], Link_Array[i]->Lane_Middle_End_Point[j]);

			if (j==0)   //taking the cell number of the first (far left) lane as the length of the link. In fact, the lengthes of lanes are the same
			{
				Link_Array[i]->Length_In_Cell=Cell_Number;

				int Free_Flow_Travel_Time= ( Cell_Number*Meter_Per_Cell) / Link_Array[i]->Limited_Speed;   //limited_speed, the unit is cell number

				int Travel_Time= Free_Flow_Travel_Time 
					                 + Waiting_Time_In_Intersections
									 + Waiting_Cycle_Number*100;	

				Link_Array[i]->Length_In_FFTT= Travel_Time;
				Link_Array[i]->Current_On_Link_Time= Travel_Time;
				
			}
		}
	}
}   


void CSimuFun::Set_Cross_Lane_Cell_and_Enter_Link()
{
	int cross_i;
	int connect_link_i;
	int Enter_Link_ID;
	int Next_Link_ID;
	int enter_link_lane_i;
	int next_link_lane_i;
	int cross_lane_id=0;

	for (cross_i=0; cross_i<G_Cross_Number; cross_i++) 
	{
		for (connect_link_i=0; connect_link_i< 2*MAX_ENTER_LINK; connect_link_i++)  //iteration entering link
		{
			Enter_Link_ID= Cross_Array[cross_i]->Connected_Link[connect_link_i];
			if (Link_Array[Enter_Link_ID]->End_Object->Object_Type=='C'
				&& Link_Array[Enter_Link_ID]->End_Object->Object_ID==Cross_Array[cross_i]->Object_ID )  //if it is entering link
			{
				for (enter_link_lane_i=0; enter_link_lane_i<Link_Array[Enter_Link_ID]->Lane_Number; enter_link_lane_i++ ) //iteration each lane entering intersection.
				{	
					/////////////////////////////////
					//left turn
					if(Link_Array[Enter_Link_ID]->Lanes[enter_link_lane_i]->Left_Turn==true )  //left turn lane
					{
						Next_Link_ID= Link_Array[Enter_Link_ID]->Next_Left_Link;
						for (next_link_lane_i=0; next_link_lane_i<Link_Array[Next_Link_ID]->Lane_Number; next_link_lane_i++ )
						{
							Add_Cross_Lane(cross_i, cross_lane_id, Enter_Link_ID,enter_link_lane_i, Next_Link_ID, next_link_lane_i,'L');
							cross_lane_id++;
						}

						//U turn
						Next_Link_ID= Link_Array[Enter_Link_ID]->Next_UTurn_Link;  
						int next_link_line= Link_Array[Next_Link_ID]->Lane_Number - 1;          //far right lane;
						Add_Cross_Lane(cross_i, cross_lane_id, Enter_Link_ID, enter_link_lane_i, Next_Link_ID, next_link_line, 'A');
						cross_lane_id++;

					}
					
					/////////////////////////////////
					//through traffic
					if(Link_Array[Enter_Link_ID]->Lanes[enter_link_lane_i]->Straight_Turn==true )  
					{
						Next_Link_ID= Link_Array[Enter_Link_ID]->Next_Straight_Link;
						//determine next_link_lane_i
						next_link_lane_i= enter_link_lane_i;                  //through traffic lane entering intersection  is able to correspond to exit intersection lane
						if (next_link_lane_i+1 >= Link_Array[Next_Link_ID]->Lane_Number )                    //the exit-intersection lane corresponding to enter-intersection link is out of range.
							next_link_lane_i= Link_Array[Next_Link_ID]->Lane_Number-1;             //choose the rightest one as the next_link_lane_i 
					
						Add_Cross_Lane(cross_i, cross_lane_id, Enter_Link_ID,enter_link_lane_i, Next_Link_ID, next_link_lane_i,'S');
						cross_lane_id++;								
					}

					///////////////////////////////////
					//right turn
					if(Link_Array[Enter_Link_ID]->Lanes[enter_link_lane_i]->Right_Turn==true)
					{
						Next_Link_ID= Link_Array[Enter_Link_ID]->Next_Right_Link;
						for (next_link_lane_i=0; next_link_lane_i<Link_Array[Next_Link_ID]->Lane_Number; next_link_lane_i++ )
						{
							Add_Cross_Lane(cross_i, cross_lane_id, Enter_Link_ID,enter_link_lane_i, Next_Link_ID, next_link_lane_i,'R');
							cross_lane_id++;
						}
					}

				}
			}
		}
		//////////////////////////
		cross_lane_id=0;
	}
	
}

void CSimuFun::Add_Cross_Lane(int Cross_ID,int Cross_Lane_ID, int Enter_Link_ID,int Enter_Lane_ID, int Next_Link_ID, int Next_Lane_ID, char Cross_Lane_Type)
{
	//Cross_Lane_ID
	//Cross_ID
	//End_Lane_ID
	//End_Link_ID
	//Start_Link_ID
	//Start_Lane_ID
	//Cell_Number
	//Cross_Lane_Cell
	
	Cpoint Start_Point;
	Cpoint End_Point;
	Cross_Lane_Array[Cross_ID][Cross_Lane_ID] = new CCross_Lane;
	
	Cross_Lane_Array[Cross_ID][Cross_Lane_ID]->Cross_Lane_ID= Cross_Lane_ID;
	Cross_Lane_Array[Cross_ID][Cross_Lane_ID]->Cross_ID= Cross_ID;
	Cross_Lane_Array[Cross_ID][Cross_Lane_ID]->End_Lane_ID= Next_Lane_ID;
	Cross_Lane_Array[Cross_ID][Cross_Lane_ID]->End_Link_ID= Next_Link_ID;
	Cross_Lane_Array[Cross_ID][Cross_Lane_ID]->Start_Link_ID= Enter_Link_ID;
	Cross_Lane_Array[Cross_ID][Cross_Lane_ID]->Start_Lane_ID= Enter_Lane_ID;
	
	Cross_Array[Cross_ID]->Cross_Lane_Number++; 
	
	Start_Point = Link_Array[Enter_Link_ID]->Lane_Middle_End_Point [Enter_Lane_ID];
	End_Point = Link_Array[Next_Link_ID]->Lane_Middle_Start_Point[Next_Lane_ID];
	
	switch (Cross_Lane_Type)
	{
	case 'L':
		Cross_Lane_Array[Cross_ID][Cross_Lane_ID]->Turn_Direction=0;
		Cross_Lane_Array[Cross_ID][Cross_Lane_ID] ->Cell_Number = 
			Cross_Lane_Array[Cross_ID][Cross_Lane_ID] ->Get_Cell_Coordinate_in_Arc(End_Point,Start_Point, Cross_Lane_ID);  
		break;

	case 'S':
		Cross_Lane_Array[Cross_ID][Cross_Lane_ID]->Turn_Direction=1;
		Cross_Lane_Array[Cross_ID][Cross_Lane_ID] ->Cell_Number = 
			Cross_Lane_Array[Cross_ID][Cross_Lane_ID] ->Get_Straight_and_Right_Cell_Coordinate_in_Arc( End_Point,Start_Point, Cross_Lane_ID);
		break;

	case 'R':
		Cross_Lane_Array[Cross_ID][Cross_Lane_ID]->Turn_Direction=2;
		Cross_Lane_Array[Cross_ID][Cross_Lane_ID] ->Cell_Number = 
			Cross_Lane_Array[Cross_ID][Cross_Lane_ID] ->Get_Straight_and_Right_Cell_Coordinate_in_Arc(End_Point,Start_Point, Cross_Lane_ID);
		break;

	case 'A':   //Next_UTurn_Link
		Cross_Lane_Array[Cross_ID][Cross_Lane_ID]->Turn_Direction=2;     //u-turn has the same turn direction index.
		Cross_Lane_Array[Cross_ID][Cross_Lane_ID] ->Cell_Number = 
			Cross_Lane_Array[Cross_ID][Cross_Lane_ID] ->Get_Cell_Coordinate_in_Arc(End_Point,Start_Point, Cross_Lane_ID);
		break;
	}
}





void CSimuFun::Simu_Go_A_Step()
{
	int link_i;   
	int cell_i;  
	int lane_i;  
	int Detector_Location;
	int cross_i;
	int cross_lane_i;
	
	CVeh *theVeh;
	extern bool Switch_Detector;
	extern bool Switch_Guidance;
	extern char Current_Control_Type;
	extern CSampleCollection *pSampleCollection;


//*********************************************************************************************************************************************
//Generate vehicle && Vehicles on link run && guidance boards work
	for(link_i=0;link_i<G_Link_Number;link_i++)    //iterate every link
	{

		//at the end/beginning of a day, reset the location of commuters
		if (simu_time%G_Day_Length==0)     
			for (int origin_i=0; origin_i<Link_Array[link_i]->Origin_Number;origin_i++)
				for (int commuter_i=0; commuter_i<Link_Array[link_i]->Origin_Array[origin_i]->Commuter_Number; commuter_i++)
					Link_Array[link_i]->Origin_Array[origin_i]->Commuter_Array[commuter_i]->Set_At_Day_Beginning();


		//generate "vehicles + commuters"
		for (int origin_i=0; origin_i<Link_Array[link_i]->Origin_Number;origin_i++)
			for (int commuter_i=0; commuter_i<Link_Array[link_i]->Origin_Array[origin_i]->Commuter_Number; commuter_i++)
			{
				CCommuter* pCommuter = Link_Array[link_i]->Origin_Array[origin_i]->Commuter_Array[commuter_i];

				if (pCommuter->Current_Location == 'H')
					if (pCommuter->Home_Or_Not() && pCommuter->Depart_Or_Not())   //at home && it is time to depart
					{
						int Load_Lane_ID= Link_Array[link_i]->Origin_Array[origin_i]->Get_Load_Lane_ID();
						int Located_Cell_ID = Link_Array[link_i]->Origin_Array[origin_i]->Located_Cell_ID;
						if (Load_Lane_ID!=-1)   //can't find out an available cell for the vehcle on any lane.
							if (false == Link_Array[link_i]->Lanes[Load_Lane_ID] ->Lane_Cell[Located_Cell_ID]->IsVehInCell() )
							{
								CVeh * pVeh = new CVeh(Total_Veh_Number, 0, Located_Cell_ID , pCommuter);   //taking a vehilce
								Link_Array[link_i]->Lanes[Load_Lane_ID] ->Lane_Cell[Located_Cell_ID]->PutVehInCell(pVeh);
								pCommuter->Current_Location='R';
								pCommuter->Departure_Time_Array[current_day-1]=time_in_current_day;
								pCommuter->Enter_Link_Time= simu_time;
								Total_Veh_Number++;					
								Commuter_Number_On_Network++;
								pSampleCollection->One_Depart('C');
							}
					}
			}
	
//*********************************************************************************************************************************************
		////////////Generate veh
//   	if (link_i==130)   //0505                                                       //only generate vehs on a specific link
//		if (Link_Array[link_i]->Start_Object->Object_Type=='M')     //only generate vehs on the edge

//             //generate moving bottleneck
// 			if(true==Link_Array[link_i]->The_OD->Able_To_In_Network_Now())
// 			{
// 				int Origin_Moving_Bottleneck=47;
// 				int Dest_Moving_Bottleneck=65;
// 
// 				if (link_i != Origin_Moving_Bottleneck)
// 					Link_Array[link_i]->The_OD->Produce_Veh(0 , -1);
// 				else
// 					Link_Array[link_i]->The_OD->Produce_Veh(2, Dest_Moving_Bottleneck);        // generate moving bottleneck on link 43 at time 1000
// 			}

		for (int i=0; i<Link_Array[link_i]->Origin_Number;i++)
			Link_Array[link_i]->Origin_Array[i]->Produce_Veh();


//*********************************************************************************************************************************************
		/* //don't need this currently
		//guidance information board work
		if (Switch_Guidance)
			if(Link_Array[link_i]->guidance!=NULL)
				Link_Array[link_i]->guidance->Guidance_Run();
		*/


//*********************************************************************************************************************************************
		//vehicles run
		for(lane_i=0;lane_i<Link_Array[link_i]->Lane_Number;lane_i++)                      //iterate lane
			for(cell_i=Link_Array[link_i]->Lanes[lane_i]->Cell_Number-1;cell_i>=0;cell_i--)           //iterate cell  forward
				if(Link_Array[link_i]->Lanes[lane_i]->Lane_Cell[cell_i]->IsVehInCell()==true)
					Link_Array[link_i]->Lanes[lane_i]->Lane_Cell[cell_i]->GetVehFromCell()->Veh_On_Link (Link_Array[link_i]->End_Object->Object_ID, link_i, lane_i, cell_i);

//*********************************************************************************************************************************************
		/* //don't need this currently
		//G2Detector run
		for(lane_i=0;lane_i<Link_Array[link_i]->Lane_Number;lane_i++)                      //iterate lane
			for(cell_i=Link_Array[link_i]->Lanes[lane_i]->Cell_Number-1;cell_i>=0;cell_i--)           //iterate cell  forward
				if(Link_Array[link_i]->Lanes[lane_i]->Lane_Cell[cell_i]->pG2Detector!=NULL)
					Link_Array[link_i]->Lanes[lane_i]->Lane_Cell[cell_i]->pG2Detector->Run();
		*/
	}


//*********************************************************************************************************************************************
//detectors work (only when Switch_Detector==true && traffic control is available (not yellow))
	if (Switch_Detector && Current_Control_Type!='N')
	{
		for (link_i=0;link_i< G_Link_Number; link_i++) 
		{
			for(int detector_i=MAX_DETECTOR_NUMBER-1; detector_i>=0; detector_i--)
			{
				bool aFlag=false;
				if(Link_Array[link_i]->Detector_Array[detector_i]!=NULL)   //there exist detector on this link
 				{	
					//intersection detectors don't work under red traffic light
					if (Link_Array[link_i]->Detector_Array[detector_i]->Detector_Type=='C')
					{
						if (Link_Array[link_i]->Lanes[lane_i]->Phase!=NULL)
						{
							if(Link_Array[link_i]->Lanes[lane_i]->Phase->Get_Current_Color()=='R')
							{
								aFlag=true;
							}
						}
					} 
					//non-red light && non-intersection detectors
					if(aFlag==false)
					{
						for(lane_i=0;lane_i<Link_Array[link_i]->Lane_Number;lane_i++)
						{
							int Turn_Direction= Get_Turn_Direction(link_i, lane_i);
							Detector_Location= Link_Array[link_i]->Detector_Array[detector_i]->Get_Detector_Location_On_Link(link_i, lane_i);

							//detect the scope of DETECTOR_SCOPE
							int End_of_Detector_Location= Detector_Location - (Link_Array[link_i]->Limited_Speed+1);
							for (int the_cell_i= Detector_Location; the_cell_i>End_of_Detector_Location; the_cell_i--)
							{
								theVeh=Link_Array[link_i]->Lanes[lane_i]->Lane_Cell[the_cell_i]->GetVehFromCell();
								Link_Array[link_i]->Detector_Array[detector_i]->Detector_Run(theVeh, Turn_Direction);
							}
						}

					}
				}			
				else   //once one is NULL, no detector after it
					break;
			}
		}
	}


//*********************************************************************************************************************************************
//controllers work && vehicles in intersection run
	for(cross_i=0;cross_i<G_Cross_Number;cross_i++)       
	{
		for(cross_lane_i=MAX_CROSS_LANE_NUMBER-1; cross_lane_i>=0; cross_lane_i--)
			if(Cross_Lane_Array[cross_i][cross_lane_i]!=NULL)      
				for (cell_i=Cross_Lane_Array[cross_i][cross_lane_i]->Cell_Number-1; cell_i>=0; cell_i--)
					if(Cross_Lane_Array[cross_i][cross_lane_i]->Cross_Lane_Cell[cell_i]->IsVehInCell()==true)    //iterate forward
					{
						Cross_Lane_Array[cross_i][cross_lane_i]->Cross_Lane_Cell[cell_i]->GetVehFromCell()->Cross_Move_Check_Time= simu_time;
						Cross_Lane_Array[cross_i][cross_lane_i]->Cross_Lane_Cell[cell_i]->GetVehFromCell()->Veh_in_Cross(cross_i,cross_lane_i,cell_i);
					}
	}

//*********************************************************************************************************************************************
	//iterate every intersection and make its controller work
	for(cross_i=0;cross_i<G_Cross_Number;cross_i++)        
		if (Cross_Array[cross_i]->Controller!=NULL)  //if there exist controller
			Cross_Array[cross_i]->Controller->Controller_Run();

//*********************************************************************************************************************************************

	// if (simu_time%5==0)                //every 5 seconds, collect one data.   Note: it will influence the broadcasting of information
// {
	this->Update_On_Link_Data();   //update link information: 1. On_Link_Veh_Number; 2. Sum_Of_Speed; 3. "Current_On_Link_Time"  for information broadcasting
	pSampleCollection->Update_Avg_K_Q();
//}
	pSampleCollection->Update_Departure_Arrival_Rate();
	pSampleCollection->Reset_Departure_Arrival_Counter();
	if (time_in_current_day!=0 && time_in_current_day%300==0)   //every 300 sec
		pSampleCollection->Update_Spatial_Distribution(current_day, time_in_current_day);    //record vehicle number on links every 300 sec.
	pSampleCollection->Reset_Veh_Data_On_Link();  //reset 1. On_Link_Veh_Number=0; 2. Sum_Of_Speed=0;


//*********************************************************************************************************************************************
////////////////////////////////////////////
//clear unarrival commuter on network at the last time of the day
	if (simu_time!=0 && simu_time - (current_day-1)*G_Day_Length==G_Day_Length-1)    //3999,7999,...
	{
		int Not_Able_Finished_Commuter_Number=0;
		for (int link_i=0; link_i<G_Link_Number;link_i++)
			for(int lane_i=0;lane_i<Link_Array[link_i]->Lane_Number;lane_i++)                      //iterate lane
				for(int cell_i=Link_Array[link_i]->Lanes[lane_i]->Cell_Number-1;cell_i>=0;cell_i--)           //iterate cell  forward
				{
					CVeh* pVeh= Link_Array[link_i]->Lanes[lane_i]->Lane_Cell[cell_i]->GetVehFromCell();
					if( pVeh!=NULL)
					{					
						if ('C' == pVeh->The_Driver->Get_Driver_Type())
						{
							Not_Able_Finished_Commuter_Number++;
							pVeh->The_Driver->Set_After_Arrival(simu_time);
							pVeh->The_Driver->Record_Travel_Experience_On_Link(simu_time, -1);
						}
						pVeh->Leave_Network_From_Link(link_i, lane_i, cell_i);
					}
				}


		for(int cross_i=0;cross_i<G_Cross_Number;cross_i++)        
			for(int cross_lane_i=0; cross_lane_i<MAX_CROSS_LANE_NUMBER; cross_lane_i++)
				if(Cross_Lane_Array[cross_i][cross_lane_i]!=NULL)      
					for (int cell_i=Cross_Lane_Array[cross_i][cross_lane_i]->Cell_Number-1; cell_i>=0; cell_i--)
						if(Cross_Lane_Array[cross_i][cross_lane_i]->Cross_Lane_Cell[cell_i]->IsVehInCell()==true)    //iterate forward
						{
							CVeh* pVeh= Cross_Lane_Array[cross_i][cross_lane_i]->Cross_Lane_Cell[cell_i]->GetVehFromCell();
							if( pVeh!=NULL)
							{					
								if ('C' == pVeh->The_Driver->Get_Driver_Type())
								{
									Not_Able_Finished_Commuter_Number++;
									pVeh->The_Driver->Set_After_Arrival(simu_time);
									pVeh->The_Driver->Record_Travel_Experience_On_Link(simu_time, -1);
								}
								pVeh->Leave_Network_From_Intersection(cross_i, cross_lane_i, cell_i);
							}
						}

////////////////////////////////////////////
		//record at the end of every day
		if (Output_MFD)
		{
			pSampleCollection->Record_MFD();
			pSampleCollection->Empty_MFD_Record();

		}
		if (Output_DepartureArrivalRate)
		{
			pSampleCollection->Record_Departure_Arrival_Rate();
			pSampleCollection->Empty_SDAR_Record();
		}

		if (Output_SpatialDistribution)
		{
			pSampleCollection->Record_Spatial_Distribution();
			pSampleCollection->Empty_SSD_Record();
		}

		if (Output_RouteChoice)
		{
			pSampleCollection->Record_Commuter_Choices(current_day, 18,0,0);
			pSampleCollection->Record_Commuter_Choices(current_day, 18,0,1);
			pSampleCollection->Record_Commuter_Choices(current_day, 18,0,2);
//			pSampleCollection->Record_Commuter_Choices(current_day, 18,0,4);
//			pSampleCollection->Record_Commuter_Choices(current_day, 18,0,5);
//			pSampleCollection->Record_Commuter_Choices(current_day, 18,0,6);
		}

		err->FlushAllFiles();

////////////////////////////////////////////
//shutdown the computer after the program ends
		if (current_day==G_Simulated_Day_Number)  //end of last day
		{
			if (Shudown_After_Program_End==true)
			{
				FILE *f; 
				errno_t err  = fopen_s( &f, "c:\\windows\\system32\\shutdown.exe", "r" );
				if(err==0) system("c:\\windows\\system32\\shutdown.exe -s -t 120");  //after 60*5 sec
			}
		}

	}





/////////////////////////////////////////////	
//day end warning.
//		CString str1;
//		CString str2;
//		str1.Format("%d", current_day-1);
//		str2.Format("%d", Not_Able_Finished_Commuter_Number);
//		CString str= "Day " +str1+" ends, "+str2+" commuters can't finished trips";
//		AfxMessageBox(str);



}
 

int CSimuFun::Get_XML_Controller(MSXML2::IXMLDOMDocumentPtr  pDOMDoc)
{
	MSXML2::IXMLDOMDocumentPtr  pDOM=pDOMDoc;
	MSXML2::IXMLDOMNodePtr pSubNode;   
	_bstr_t  strRequestType;

	int i;
	int j;
	int Flag;
	int Cross_ID;
	int Cycle_Time;
	int Phase_Number;
	int Direction;
	int Phase_ID;
	int Connect_Link_ID_1;
	int Connect_Link_ID_2;
	int Green_Percent;
	int Green_Start_Time_Percent;
	int Link_ID_Array[LINK_IN_ONE_PHASE];
	CPhase *Phase_Array[MAX_PHASE_NUMBER];
	Struct_Control Control_Info;

	for (i=0;i<MAX_PHASE_NUMBER;i++)
		Phase_Array[i]=NULL;

	for(i=0;i<LINK_IN_ONE_PHASE;i++)
		Link_ID_Array[i]=-1;

	///////////////////////////////////////////////////////////
	//Get data from XML file
	///////////////////////
	//	Cross_ID
	//	Cycle_Time
	//	Phase_Number
	//  Phase
	//	   |--Connect_Link_ID
	//     |    |--A
	//     |    |--B
	//	   |--Phase_ID
	//     |--Direction
	//     |--Green_Percent
	//	   |--Green_Start_Time_Percent
	///////////////////////
    MSXML2::IXMLDOMNodeListPtr pNodeList_Cross_ID = NULL;     
    pNodeList_Cross_ID = pDOM->selectNodes("Data/Controller");

    MSXML2::IXMLDOMNodeListPtr pNodeList_Cycle_Time = NULL;   
    pNodeList_Cycle_Time = pDOM->selectNodes("Data/Controller"); 

    MSXML2::IXMLDOMNodeListPtr pNodeList_Phase_Amount = NULL;   
    pNodeList_Phase_Amount = pDOM->selectNodes("Data/Controller"); 

	MSXML2::IXMLDOMNodeListPtr pNodeList_Connect_Link_ID_1 = NULL;   
    pNodeList_Connect_Link_ID_1 = pDOM->selectNodes("Data/Controller/Phase/Connect_Link_ID"); 

	MSXML2::IXMLDOMNodeListPtr pNodeList_Connect_Link_ID_2 = NULL;   
    pNodeList_Connect_Link_ID_2 = pDOM->selectNodes("Data/Controller/Phase/Connect_Link_ID"); 
	
	MSXML2::IXMLDOMNodeListPtr pNodeList_Phase_ID = NULL;   
    pNodeList_Phase_ID = pDOM->selectNodes("Data/Controller/Phase"); 
	
	MSXML2::IXMLDOMNodeListPtr pNodeList_Direction = NULL; 
    pNodeList_Direction = pDOM->selectNodes("Data/Controller/Phase"); 

    MSXML2::IXMLDOMNodeListPtr pNodeList_Green_Percent = NULL;   
    pNodeList_Green_Percent = pDOM->selectNodes("Data/Controller/Phase"); 

    MSXML2::IXMLDOMNodeListPtr pNodeList_Green_Start_Time_Percent = NULL;
    pNodeList_Green_Start_Time_Percent = pDOM->selectNodes("Data/Controller/Phase"); 

	if(G_Cross_Number < pNodeList_Cross_ID->Getlength())
	{
		AfxMessageBox("too many controller");
	}

	for(i=0;i<pNodeList_Cross_ID->Getlength();i++)   
	{
		///////////////////////
		//Cross_ID
		pSubNode=pNodeList_Cross_ID->nextNode()->selectSingleNode("Cross_ID");  
		strRequestType=pSubNode->Gettext(); 
		Cross_ID = atoi(strRequestType.operator char*());  
		Cross_ID= Deal_with_Deviation_XML(Cross_ID, DEVIATION_XML);

		///////////////////////
		//Cycle_Time
		pSubNode=pNodeList_Cycle_Time->nextNode()->selectSingleNode("Cycle_Time");  
		strRequestType=pSubNode->Gettext(); 
		Cycle_Time = atoi(strRequestType.operator char*());  

		//////////////////////
		//Phase_Number
		pSubNode=pNodeList_Phase_Amount->nextNode()->selectSingleNode("Phase_Number");  
		strRequestType=pSubNode->Gettext(); 
		Phase_Number= atoi(strRequestType.operator char*());  

		///////////////////////
		//initial Control_Info(a part)
		Control_Info.Cross_ID= Cross_ID;
		Control_Info.Cycle_Time= Cycle_Time;

		///////////////////////
		//Phase
		for (j=0; j<Phase_Number; j++)
		{
			//////////////////////
			//Connect_Link_ID_1
			pSubNode=pNodeList_Connect_Link_ID_1->nextNode()->selectSingleNode("A");  
			strRequestType=pSubNode->Gettext(); 
			Connect_Link_ID_1= atoi(strRequestType.operator char*())  ;  	
			Connect_Link_ID_1= Deal_with_Deviation_XML(Connect_Link_ID_1, DEVIATION_XML);

			//////////////////////
			//Connect_Link_ID_2
			pSubNode=pNodeList_Connect_Link_ID_2->nextNode()->selectSingleNode("B");  
			strRequestType=pSubNode->Gettext(); 
			Connect_Link_ID_2= atoi(strRequestType.operator char*())  ;  	
			Connect_Link_ID_2= Deal_with_Deviation_XML(Connect_Link_ID_2, DEVIATION_XML);

			//////////////////////
			//Phase_ID
			pSubNode=pNodeList_Phase_ID->nextNode()->selectSingleNode("Phase_ID");  
			strRequestType=pSubNode->Gettext(); 
			Phase_ID= atoi(strRequestType.operator char*())  ;  
			Phase_ID= Deal_with_Deviation_XML(Phase_ID, DEVIATION_XML);

			//////////////////////
			//Direction
			pSubNode=pNodeList_Direction->nextNode()->selectSingleNode("Direction");  
			strRequestType=pSubNode->Gettext(); 
			Direction= atoi(strRequestType.operator char*());  
			
			//////////////////////
			//Green_Percent
			pSubNode=pNodeList_Green_Percent->nextNode()->selectSingleNode("Green_Percent");  
			strRequestType=pSubNode->Gettext(); 
			Green_Percent= atoi(strRequestType.operator char*())  ;  

			//////////////////////
			//Green_Start_Time
			pSubNode=pNodeList_Green_Start_Time_Percent->nextNode()->selectSingleNode("Green_Start_Time_Percent");  
			strRequestType=pSubNode->Gettext(); 
			Green_Start_Time_Percent= atoi(strRequestType.operator char*())  ;  

			Link_ID_Array[0]=Connect_Link_ID_1;
			Link_ID_Array[1]=Connect_Link_ID_2;
			
			//Gen a phase based on XML data, but there is still no Green_Percent information
			try
			{
				CPhase *Phase= new CPhase(Phase_ID, ALL_GREEN, Direction ,INIT_CONTROL_CYCLE, START_TIME_ALL_GREEN, Link_ID_Array);   //Leaking Memory--3
				Phase_Array[j]=Phase;     
			}
			catch (bad_alloc  &a)
			{
				CString str= a.what();
				AfxMessageBox(str+"\n"+"Error---CSimuFun::Get_XML_Link( )~~~4");
				return -1;
			}
			
			Control_Info.Phase_Info_Array[j].Green_Percent= Green_Percent;
			Control_Info.Phase_Info_Array[j].Green_Start_Time_Percent= Green_Start_Time_Percent;
			Control_Info.Phase_Info_Array[j].Phase_ID= Phase_ID;
		}

		//end

		/////////////////////////////////////////////////////////
		//save it to intersection
		CController *Controller= new CController(Cross_ID, Phase_Number);
		Controller->Init_Control_Info= Control_Info;
		for (j=0;j<Phase_Number;j++)
		{
			Controller->Add_Phase(Phase_Array[j]);
		}
		Cross_Array[Cross_ID]->Controller= Controller;   

		Controller=NULL;

		//the results of initialization 
		//there is phases in Phase_Array of Controller, but no data in phase or all green, so it is the state without control
		//there is initial control information in Init_Control_Info----as initializing point control information
	}

	if (pNodeList_Cross_ID->Getlength() >0 )
	{
		Flag=1;
	} 
	else
	{
		Flag=0;
	}
	return Flag;
}



int CSimuFun::Connect_Phase_And_Lane()
{
	int cross_i;
	int link_i;
	int lane_i;
	int Link_ID;
	CPhase *Phase=NULL;
	for (cross_i=0; cross_i<MAX_CROSS_NUMBER;cross_i++) 
	{
		if (Cross_Array[cross_i]!=NULL) 
		{
			if (Cross_Array[cross_i]->Controller!=NULL) 
			{
				//iterate intersection lane
				for (link_i=0; link_i< 2*MAX_ENTER_LINK; link_i++)
				{
					Link_ID= Cross_Array[cross_i]->Connected_Link[link_i]; 
					if ('C'==Link_Array[Link_ID]->End_Object->Object_Type
						&& 
						Link_Array[Link_ID]->End_Object->Object_ID== Cross_Array[cross_i]->Object_ID )  //means entering link
					{
						for (lane_i=0; lane_i<MAX_LANE_NUMBER;lane_i++)   //iterate lane
						{
							Phase= Get_Phase_in_Lane(Link_ID, cross_i, lane_i);
							if (Phase!=NULL)
							{
								Link_Array[Link_ID]->Lanes[lane_i]->Phase= Phase;
								Phase=NULL;
							}
						}
					}
				}
			}
		}
	}

	//check if success or not
	for (link_i=0; link_i<MAX_LINK_NUMBER; link_i++)
	{
		for (lane_i=0; lane_i< MAX_LANE_NUMBER; lane_i++)
		{
			if (Link_Array[link_i]->Lanes[lane_i]->Phase!=NULL)
			{
				return 1;
			}
		}
	}
	return -1;
}



CPhase * CSimuFun::Get_Phase_in_Lane(int Link_ID, int Cross_ID, int Lane_ID)
{
	int phase_i;
	int Direction=-1;
	CPhase *Phase;
	int Link_in_Phase_Flag=-1;

	//*********************************************
	//  limitation: a lane with two turning directions is not allowed
	//*********************************************
	//Get turning directions of the lane
	if (Link_Array[Link_ID]->Lanes[Lane_ID]->Left_Turn)
	{
		Direction= 0;
	}
	if (Link_Array[Link_ID]->Lanes[Lane_ID]->Straight_Turn)
	{
		Direction= 1;
	}
	if (Link_Array[Link_ID]->Lanes[Lane_ID]->Right_Turn)
	{
		Direction= 2;
	}
	//iterate phase
	for (phase_i=0; phase_i< MAX_PHASE_NUMBER; phase_i++)
	{
		Phase= Cross_Array[Cross_ID]->Controller->Get_Phase(phase_i);
		Link_in_Phase_Flag= Cross_Array[Cross_ID]->Controller->Link_in_Phase_or_Not(phase_i, Link_ID);
		if (Phase->Direction==Direction && 1==Link_in_Phase_Flag)
		{
			return Phase;
		}
	}

	return NULL;
}



//deal with the deviation between this program and XML; the sequence of this program is beginning with 0, but xml is with 1
int CSimuFun::Deal_with_Deviation_XML(int Get_Value, int Deviation)
{
	if (Get_Value==-1)
		return Get_Value;
	else
		return (Get_Value - Deviation);
}

int CSimuFun::Get_Turn_Direction(int Link_ID, int Lane_ID)
{
	if (Link_Array[Link_ID]->Lanes[Lane_ID]->Left_Turn==true)
		return 0;
	
	if (Link_Array[Link_ID]->Lanes[Lane_ID]->Straight_Turn==true)
		return 1;
	
	if (Link_Array[Link_ID]->Lanes[Lane_ID]->Right_Turn==true)
		return 2;
	
	return -1;
}

void CSimuFun::Connect_Controller_and_Detector()
{
	int cross_i;
	int Link_Link_i;
	int Detector_on_Link_i;
	int Link_Link_ID;
	int k=0;

	for (cross_i=0;cross_i<G_Cross_Number; cross_i++) 
	{
		k=0;
		for (Link_Link_i=0;Link_Link_i<2*MAX_ENTER_LINK;Link_Link_i++) 
		{
			Link_Link_ID= Cross_Array[cross_i]->Connected_Link[Link_Link_i];
			
			if (cross_i==Link_Array[Link_Link_ID]->End_Object->Object_ID   //determine the link id which is toward the intersection: current intersection==the end intersection of this link 
				&&
				'C'==Link_Array[Link_Link_ID]->End_Object->Object_Type)
			{
				for (Detector_on_Link_i=0; Detector_on_Link_i<MAX_DETECTOR_NUMBER; Detector_on_Link_i++)  
				{
					if (Link_Array[Link_Link_ID]->Detector_Array[Detector_on_Link_i]!=NULL) 
					{
						Cross_Array[cross_i]->Controller->Detector_Under_Control[k][Detector_on_Link_i]
							= Link_Array[Link_Link_ID]->Detector_Array[Detector_on_Link_i] ; 
						
						Link_Array[Link_Link_ID]->Detector_Array[Detector_on_Link_i]->Cross_ID= cross_i;		
					}	
				}
				k++;
			}
		}
	}
}



void CSimuFun::Set_Controller_State(char Current_Control_Type, int Start_Time)
{
	int i;
	for (i=0 ; i<G_Cross_Number;i++)
		if (Cross_Array[i]!=NULL) 
			Cross_Array[i]->Controller->Start_Time= Start_Time;   
}

void CSimuFun::Set_Straight_Conflict()
{
	int cross_i;
	int connect_link_i;
	int cross_lane_i;
	int cross_lane_j;
	int cell_i;
	int cell_j;
	int Straight_Start_Link_ID;
	int Straight_End_Link_ID;
	CCell *Cell_A=NULL;
	CCell *Cell_B=NULL;
	double Distance=0;

	for (cross_i=0; cross_i< G_Cross_Number; cross_i++)   
	{                                                                                                                 //1a
		for (connect_link_i=0; connect_link_i< 2*MAX_ENTER_LINK; connect_link_i++)   //iterate entering link
		{                                                                                                             //2a
			Straight_Start_Link_ID= Cross_Array[cross_i]->Connected_Link[connect_link_i];   //straight link or not, focus on Enter_Link_ID
			if (true==Enter_Link_or_Not(Straight_Start_Link_ID, cross_i))
			{                                                                                                           //11a
				Straight_End_Link_ID= Link_Array[Straight_Start_Link_ID]->Next_Straight_Link;  //find out the go-straight link
				for (cross_lane_i=0; cross_lane_i<Cross_Array[cross_i]->Cross_Lane_Number; cross_lane_i++)
				{                                                                                                         //4a
					//*************************************
					//judge: if it is the straight lane in straight link
					if ( true==Straight_Route_or_Not(cross_i, cross_lane_i, Straight_Start_Link_ID, Straight_End_Link_ID) )  
					{                                                                                                 //6a
						//**************************************
						//iterate every lane
						for (cross_lane_j=0; cross_lane_j<Cross_Array[cross_i]->Cross_Lane_Number; cross_lane_j++)
						{                                                                                         //8a
							if (true==Cross_Lane_Conflict_or_Not('S', cross_i,cross_lane_j,cross_lane_i) )  
							{ 		
								for (cell_i=Cross_Lane_Array[cross_i][cross_lane_i]->Cell_Number-1; cell_i>=0;cell_i--)
								{                                                                                             //7a
									for(cell_j=0;cell_j<Cross_Lane_Array[cross_i][cross_lane_j]->Cell_Number;cell_j++)
									{                                                                             //10a
										//*************************************
										Cell_A=Cross_Lane_Array[cross_i][cross_lane_i]->Cross_Lane_Cell[cell_i];
										Cell_B=Cross_Lane_Array[cross_i][cross_lane_j]->Cross_Lane_Cell[cell_j];
										Distance= Point_Distance(Cell_A->Cell_Center->x, 
																						Cell_A->Cell_Center->y,
																						Cell_B->Cell_Center->x, 
																						Cell_B->Cell_Center->y);
										//*************************************

										if (Distance <= TROUGH_CONFLICT_SCOPE) 
										{
											Cell_A->Straight_Conflict_Cell=Cell_B;
											Cell_B->Straight_Conflict_Cell=Cell_A;
											break;
										} 
									}//10b
								}//9b
							}//8b
						}//7b
					}//6b
				}//4b			
			}//11b
		}//2b
	}//1b

}

void CSimuFun::Set_Conflict_Area()
{
	/*
		use the cell of straight lane where conflict happens distinguish conflict point.
		iteration: every through traffic lane (Straight turn), from front to back
		iteration: every cell on throught traffic lane, from front to back
		iteration: every left turning lane
		iteration: every cell on the left turning lane, from back to front
		calculate distance between two cells
		if it is smaller than the conflicting distance
			then there is a conflict
	*/
	int Straight_Start_Link_ID;
	int Straight_End_Link_ID;
	CCell *Main_Cell=NULL;
	CCell *Conflict_Cell=NULL;
	double Distance=0;

	int cross_i;
	int cell_i;
	int cell_j;
	int cross_lane_i;
	int cross_lane_j;
	int connect_link_i;
	int conflict_i=0;
	
	for (cross_i=0; cross_i< G_Cross_Number; cross_i++)   //iterate intersection
	{                                                                                                                 //1a
		for (connect_link_i=0; connect_link_i< 2*MAX_ENTER_LINK; connect_link_i++)   //iterate entering link
		{                                                                                                             //2a
			Straight_Start_Link_ID= Cross_Array[cross_i]->Connected_Link[connect_link_i];   //find out the go-straight link
			if (true==Enter_Link_or_Not(Straight_Start_Link_ID, cross_i))
			{                                                                                                           //11a
				Straight_End_Link_ID= Link_Array[Straight_Start_Link_ID]->Next_Straight_Link;
				for (cross_lane_i=0; cross_lane_i<Cross_Array[cross_i]->Cross_Lane_Number; cross_lane_i++)
				{                                                                                                         //4a
					//*************************************
					if ( true==Straight_Route_or_Not(cross_i, cross_lane_i, Straight_Start_Link_ID, Straight_End_Link_ID) )  
					{                                                                                                 //6a
						//**************************************
						for (cross_lane_j=0; cross_lane_j<Cross_Array[cross_i]->Cross_Lane_Number; cross_lane_j++)
						{                                                                                         //8a
							if (true==Cross_Lane_Conflict_or_Not('L', cross_i,cross_lane_j,cross_lane_i) )  
							{ 		
								for (cell_i=Cross_Lane_Array[cross_i][cross_lane_i]->Cell_Number-1; cell_i>=0;cell_i--)
								{                                                                                             //7a
									for(cell_j=0;cell_j<Cross_Lane_Array[cross_i][cross_lane_j]->Cell_Number;cell_j++)
									{                                                                             //10a
										//*************************************
										Main_Cell=Cross_Lane_Array[cross_i][cross_lane_i]->Cross_Lane_Cell[cell_i];
										Conflict_Cell=Cross_Lane_Array[cross_i][cross_lane_j]->Cross_Lane_Cell[cell_j];
										Distance= Point_Distance(Main_Cell->Cell_Center->x, 
																						Main_Cell->Cell_Center->y,
																						Conflict_Cell->Cell_Center->x, 
																						Conflict_Cell->Cell_Center->y);
										//*************************************

										if (Distance <= LEFT_CONFLICT_SCOPE)
										{
											Add_Conflict(cross_i, Main_Cell, Conflict_Cell);
										} 
									}//10b
								}//9b
							}//8b
						}//7b
					}//6b
				}//4b			
			}//11b
		}//2b
	}//1b
}

bool CSimuFun::Cross_Lane_Conflict_or_Not(char Type, int Cross_ID, int Determine_Route_ID, int Current_Straight_Route_ID)
{
	int Determine_Turn_Direction;
	Determine_Turn_Direction= Cross_Lane_Array[Cross_ID][Determine_Route_ID]->Turn_Direction;
	switch (Type)
	{
	case 'S':
		if(Determine_Turn_Direction!=1)
			return false;
		break;
	case 'L':
		if(Determine_Turn_Direction!=0)
			return false;
		break;
	}

	int Straight_Route_Start_Link_ID;
	int Straight_Route_End_Link_ID;
	int Determine_Route_Start_Link_ID;
	int Determine_Route_End_Link_ID;

	Straight_Route_Start_Link_ID= Cross_Lane_Array[Cross_ID][Current_Straight_Route_ID]->Start_Link_ID;
	Straight_Route_End_Link_ID= Cross_Lane_Array[Cross_ID][Current_Straight_Route_ID]->End_Link_ID;
	Determine_Route_Start_Link_ID=  Cross_Lane_Array[Cross_ID][Determine_Route_ID]->Start_Link_ID;
	Determine_Route_End_Link_ID= Cross_Lane_Array[Cross_ID][Determine_Route_ID]->End_Link_ID;

	int a=-1;
	int b=-1;
	int Cross_Link_Amount;
	Cross_Link_Amount= 2*Cross_Array[Cross_ID]->Connected_Segment_Number;

	// left and straight (which have the same a and b)
	for (int i=0;i<Cross_Link_Amount;i++)
	{
		if(Cross_Array[Cross_ID]->Connected_Link[i]==Straight_Route_Start_Link_ID)
		{	
			//for 1st pair
			if (i==Cross_Link_Amount-1)
				a= Cross_Array[Cross_ID]->Connected_Link[0];   //Straight_Route_Start_Link_ID+1 on couterclockwise
			else
				a= Cross_Array[Cross_ID]->Connected_Link[i+1]; 

			//for 2nd pair
			if (i==Cross_Link_Amount-2)
				b= Cross_Array[Cross_ID]->Connected_Link[0];
			else
				b= Cross_Array[Cross_ID]->Connected_Link[i+2];	
			
			break;
		}
	}

	switch (Type)
	{
	case 'S':
		if(Determine_Route_End_Link_ID==a)
			return true;
		break;
	case 'L':
		if (Determine_Route_End_Link_ID==a || Determine_Route_Start_Link_ID==b )   
			return true;
		break;
	}

	return false;
}

void CSimuFun::Add_Conflict(int Cross_ID, CCell *Main_Cell, CCell *Conflict_Cell)
{
	int i;
	int Insert_Conflict_Cell_Flag;          //the place inserting new Conflict_Cell
	int Insert_Conflict_Area_ID_Flag;       //the place inserting Conflict_Area_ID into Conflict_Cell

	//if there is the conflict taking Main_Cell as its center
	for (i=0; i<Cross_Array[Cross_ID]->Conflict_Area_Number; i++)  //iterate in all conflict area of this intersection
	{
		if (Conflict_Area_Array[Cross_ID][i]->Main_Cell== Main_Cell )
		{
			Insert_Conflict_Area_ID_Flag= Conflict_Cell->Conflict_Area_ID_Amount;
			Conflict_Cell->Conflict_Area_ID_Array[Insert_Conflict_Area_ID_Flag]= Conflict_Area_Array[Cross_ID][i]->Conflict_ID;
			Conflict_Cell->Conflict_Area_ID_Amount++;
			
			Insert_Conflict_Cell_Flag= Conflict_Area_Array[Cross_ID][i]->Conflict_Cell_Number;
			Conflict_Area_Array[Cross_ID][i]->Conflict_Cell_Array[Insert_Conflict_Cell_Flag]= Conflict_Cell;
			Conflict_Area_Array[Cross_ID][i]->Conflict_Cell_Number++;
			return;
		}
	}
	
	if (Cross_Array[Cross_ID]->Conflict_Area_Number>MAX_CONFLICT_AREA_NUMBER)
	{
		AfxMessageBox("generate too many conflicts"); 
	}
	
	//if no, then create; the place is Current_General_Position
	CConflict_Area *CA= new CConflict_Area;
	CA->Cross_ID= Cross_ID;
	CA->Conflict_ID=Cross_Array[Cross_ID]->Conflict_Area_Number;
	CA->Main_Cell= Main_Cell;
	CA->Conflict_Cell_Array[0]=Conflict_Cell;
	CA->Conflict_Cell_Number=1;
	
	Conflict_Area_Array[Cross_ID][CA->Conflict_ID]=CA;   //save to conflict array
	Cross_Array[Cross_ID]->Conflict_Area_Number++;	

	Insert_Conflict_Area_ID_Flag= Conflict_Cell->Conflict_Area_ID_Amount;
	Conflict_Cell->Conflict_Area_ID_Array[Insert_Conflict_Area_ID_Flag]= Conflict_Area_Array[Cross_ID][i]->Conflict_ID;
	Conflict_Cell->Conflict_Area_ID_Amount++;
}


bool CSimuFun::Enter_Link_or_Not(int Link_ID, int Cross_ID)
{
	if (Link_Array[Link_ID]->End_Object->Object_ID==Cross_Array[Cross_ID]->Object_ID
		&& Link_Array[Link_ID]->End_Object->Object_Type== Cross_Array[Cross_ID]->Object_Type )
		return true;
	else
		return false;
}


bool CSimuFun::Straight_Route_or_Not(int Cross_ID, int Cross_Lane_ID, int Straight_Start_Link_ID, int Straight_End_Link_ID)
{
	if (Cross_Lane_Array[Cross_ID][Cross_Lane_ID]->Start_Link_ID==Straight_Start_Link_ID
		&&Cross_Lane_Array[Cross_ID][Cross_Lane_ID]->End_Link_ID==Straight_End_Link_ID)
		return true;
	else
		return false;
}


void CSimuFun::Set_All_Distance_Shortest_Path()
{
	for (int i=0; i<G_Link_Number;i++)
		for (int j=0; j<G_Link_Number; j++)
		{
			if (i==j)
				Distance_Shortest_Path_Array[i][j]=NULL;
			Distance_Shortest_Path_Array[i][j]= Shortest_Path('D', i, j);
		}
}


void CSimuFun::Set_Next_Link()
{
	int i;
	int j;
	
	for (i=0;i<G_Cross_Number;i++)
	{
		for (j=0;j<2*MAX_ENTER_LINK;j++)
		{
			if (Cross_Array[i]->Connected_Link[j]!=-1)
			{
				int Link_ID=Cross_Array[i]->Connected_Link[j];
				if (Link_Array[Link_ID]->End_Object->Object_ID==Cross_Array[i]->Object_ID
					&&Link_Array[Link_ID]->End_Object->Object_Type==Cross_Array[i]->Object_Type) 
				{
					/////////////////////////
					if (j+1 >= 2*MAX_ENTER_LINK)
						Link_Array[Link_ID]->Next_Right_Link= Cross_Array[i]->Connected_Link[j+1-2*MAX_ENTER_LINK];				
					else
						Link_Array[Link_ID]->Next_Right_Link= Cross_Array[i]->Connected_Link[j+1];
					/////////////////////////
					if(j+3 >= 2*MAX_ENTER_LINK)
						Link_Array[Link_ID]->Next_Straight_Link= Cross_Array[i]->Connected_Link[j+3-2*MAX_ENTER_LINK];
					else
						Link_Array[Link_ID]->Next_Straight_Link= Cross_Array[i]->Connected_Link[j+3];
					//////////////////////////
					if(j+5 >= 2*MAX_ENTER_LINK)
						Link_Array[Link_ID]->Next_Left_Link= Cross_Array[i]->Connected_Link[j+5-2*MAX_ENTER_LINK];
					else
						Link_Array[Link_ID]->Next_Left_Link= Cross_Array[i]->Connected_Link[j+5];
					///////////////////////////
					if(j+7 >= 2*MAX_ENTER_LINK)
						Link_Array[Link_ID]->Next_UTurn_Link= Cross_Array[i]->Connected_Link[j+7-2*MAX_ENTER_LINK];
					else
						Link_Array[Link_ID]->Next_UTurn_Link= Cross_Array[i]->Connected_Link[j+7];
				}
			}
		}
	}
}

void CSimuFun::Set_Link_Label_And_GBoard()
{
	for (int i=0; i<G_Link_Number;i++)
	{
		if (Link_Array[i]!=NULL )
			Link_Array[i]->Get_Label_Coordinate();
	}
}

void CSimuFun::Set_OD_On_Link()
{
	int Located_Cell_ID;
	//////////////////////////////////////////
	//setup the regular OD
	for (int i=0; i<G_Link_Number;i++)
	{
		CLink *The_Link= Link_Array[i];
		if (The_Link!=NULL )
		{
			if (The_Link->Start_Object->Object_Type=='M')
			{
				Located_Cell_ID=0;
				CRanDestOrigin *An_Origin= new CRanDestOrigin(The_Link->Object_ID, Located_Cell_ID, -1);
				The_Link->Add_Origin(An_Origin);
			}
			else if(The_Link->End_Object->Object_Type=='M')
				The_Link->Dest=new CDest(The_Link->Object_ID , Link_Array[i]->Length_In_Cell-1 );
			else
			{
				int Rightest_Lane_ID=The_Link->Lane_Number - 1;
				int Number_Of_Cell= Link_Array[The_Link->Object_ID]->Lanes[Rightest_Lane_ID]->Cell_Number;
				Located_Cell_ID= int(Number_Of_Cell/2);                     //middle of the link
				CRanDestOrigin *An_Origin= new CRanDestOrigin(The_Link->Object_ID, Located_Cell_ID, -1);
				The_Link->Add_Origin(An_Origin);

				The_Link->Dest=new CDest(The_Link->Object_ID, Located_Cell_ID);
			}	
		}
	}

	/////////////////////////////////
	//setup the OD assigned using XML
	for (int i=0; i<G_FixedOD_Number; i++)
	{
		int Origin_Link_ID= FixedOD_Array[i].Origin;
		int Dest_Link_ID=	FixedOD_Array[i].Dest;
		int Demand= FixedOD_Array[i].Demand;

		//Origin
		if (Link_Array[Origin_Link_ID]->Start_Object->Object_Type=='M')
			Located_Cell_ID=0;
		else
		{
			int Rightest_Lane_ID=Link_Array[Origin_Link_ID]->Lane_Number - 1;
			int Number_Of_Cell= Link_Array[Origin_Link_ID]->Lanes[Rightest_Lane_ID]->Cell_Number;
			Located_Cell_ID= int(Number_Of_Cell/2);           
		}
		CFixDestOrigin *An_Origin= new CFixDestOrigin(Origin_Link_ID, Located_Cell_ID, Demand, Dest_Link_ID);
		Link_Array[Origin_Link_ID]->Add_Origin(An_Origin);
		
		//Dest
		if (Link_Array[Dest_Link_ID]->Dest==NULL)
		{
			if (Link_Array[Dest_Link_ID]->End_Object->Object_Type=='M')
				Located_Cell_ID= Link_Array[Dest_Link_ID]->Length_In_Cell-1;
			else
			{
				int Rightest_Lane_ID=Link_Array[Origin_Link_ID]->Lane_Number - 1;
				int Number_Of_Cell= Link_Array[Origin_Link_ID]->Lanes[Rightest_Lane_ID]->Cell_Number;
				Located_Cell_ID= int(Number_Of_Cell/2);           
			}

			CDest *An_Dest= new CDest(Dest_Link_ID, Located_Cell_ID);
			Link_Array[Dest_Link_ID]->Dest=An_Dest;
		}

	}
}

void CSimuFun::Set_G2Detector()
{
	for(int link_i=0;link_i<G_Link_Number;link_i++)    //iterate every link
		for(int lane_i=0;lane_i<Link_Array[link_i]->Lane_Number;lane_i++)                      //iterate lane
			for(int cell_i=Link_Array[link_i]->Lanes[lane_i]->Cell_Number;cell_i>=0;cell_i--)           //iterate cell  forward
			{
				if (
					(link_i==47 && lane_i==2 && cell_i==95) 
 					|| (link_i==47 && lane_i==2 && cell_i==45) 
					)
				{
					CG2Detector *pG2Detector= new CG2Detector(link_i, lane_i, cell_i);
					Link_Array[link_i]->Lanes[lane_i]->Lane_Cell[cell_i]->pG2Detector = pG2Detector;
				}
			}
}


void CSimuFun::Update_On_Link_Data()
{
	double Speed;
	extern CSimuFun *simuFun;
	for (int i=0; i<G_Link_Number; i++)
	{
		int On_Link_Veh_Number=0;
		double Sum_Of_Speed=0;
		double Avg_Speed= 0;

		////////////////////////////////////////////////////////
		//sum of speed
		for (int j=0; j<Link_Array[i]->Lane_Number; j++)
		{
			for (int k=0; k<Link_Array[i]->Lanes[j]->Cell_Number; k++)
				if (true==Link_Array[i]->Lanes[j]->Lane_Cell[k]->IsVehInCell())
				{
					On_Link_Veh_Number++;
					Speed= Link_Array[i]->Lanes[j]->Lane_Cell[k]->GetVehFromCell()->Cur_Spd;
					Sum_Of_Speed =Sum_Of_Speed + Speed;
				}
		}

		Link_Array[i]->On_Link_Veh_Number=On_Link_Veh_Number;
		Link_Array[i]->Sum_Of_Speed= Sum_Of_Speed;
		
		/////////////////////////////////////////////////////
		CMainFrame *pMainFrame= (CMainFrame *)AfxGetApp()->m_pMainWnd;   
		if (pMainFrame->Show_Time_Shortest_Path)
			if (simu_time%BROADCASTING_FREQUENCY==0)    //updating time is coming and 
			{
				//no veh
				if (On_Link_Veh_Number<=FREE_FLOW_VEH_NUMBER)
				{
					Avg_Speed=Link_Array[i]->Limited_Speed;
					Link_Array[i]->Current_On_Link_Time=int(Link_Array[i]->Length_In_Cell*Pixel_Per_Cell/Avg_Speed);	
				}
				else if (Sum_Of_Speed==0)   // can't move, equal to a big number
				{
					Link_Array[i]->Current_On_Link_Time=100000000;	   
				}
				else
				{
					Avg_Speed= Sum_Of_Speed/On_Link_Veh_Number;
					Link_Array[i]->Current_On_Link_Time=int(Link_Array[i]->Length_In_Cell*Pixel_Per_Cell/Avg_Speed);	
				}
			}
	}

/*
	// due to the broadcasting time, set information ready, otherwise false
	if (simu_time%BROADCASTING_FREQUENCY==0)
		if(false==simuFun->Information_Ready)
			this->Information_Ready=true;
	else
		simuFun->Information_Ready=false;
*/
}



void CSimuFun::Set_Commuter_On_Origin()
{
	int End_Link_ID = -1;
	int Commuter_Number=0;

	for (int link_i=0; link_i<G_Link_Number;link_i++)
	{
		CLink *The_Link= Link_Array[link_i];
		if (The_Link!=NULL )
			if(The_Link->Is_Origin==true)
				for (int origin_i=0; origin_i<The_Link->Origin_Number; origin_i++)
				{ 
					COrigin * pOrigin= The_Link->Origin_Array[origin_i];
					for (int commuter_i=0; commuter_i< pOrigin->Commuter_Number; commuter_i++)
					{
						Struct_Shortest_Path *spi;

						////////////////////////////////////////////////////////////////////////////////////////////
						// generate special commuters travel from 18 to 78
						if (link_i==18 && origin_i==0 &&commuter_i<3)   //no type-3,4,5
						{
							End_Link_ID= 78;
						
							int Type= commuter_i +1;  
							CCommuter *  pCommuter = new CCommuter(Commuter_Number, Type, link_i, End_Link_ID, G_Required_Arrival_Time);		//link_i is pOrigin->Located_Link_ID
							
							pOrigin->Commuter_Array[commuter_i]= pCommuter;
							Commuter_Number++;
						}
						else
						{
							// at the beginning the destination of commuters are randomly assigned as the work place, and the home and work place will be fixed in the simulation
							if (pOrigin->End_Link_ID==-1)    //CRanDestOrigin
							{
								//if intend to get an end link randomly, need to use Get_Veh_SPI() to get spi, and then get the end link from spi, because Get_Veh_SPI is able to check the link number on route.
								spi= pOrigin->Get_Veh_SPI('D');   //shortest path is just for making sure there are routes between OD
								End_Link_ID	= spi->End_Link_ID;
							}
							else    //CFixDestOrigin
							{
								End_Link_ID = pOrigin->End_Link_ID;
							}
						
							//create a commuter
							CCommuter *  pCommuter = new CCommuter(Commuter_Number, All_Commuters_Type, link_i, End_Link_ID, G_Required_Arrival_Time);		//link_i is pOrigin->Located_Link_ID
							pOrigin->Commuter_Array[commuter_i]= pCommuter;
							Commuter_Number++;
						}
						////////////////////////////////////////////////////////////////////////////////////////////
					}
				}
	}

}
