#pragma once

#include "point.h"
class CMyObject : public CObject  
{
public:
	CMyObject();
	virtual ~CMyObject();
	int Object_ID; 
	char Object_Type;            //M: marginal link;  C: crossing/intersection;   R: Link
	Cpoint Object_Coodinate;
	CString Object_Label;
};

