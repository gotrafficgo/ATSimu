#pragma once

class Cpoint  //NOTE: not CPoint
{
public:
	Cpoint(double a1=-1,double a2=-1) { x = a1; y = a2; }
	virtual ~Cpoint();
public :
	double x;
	double y;
};

