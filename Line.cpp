#include "stdafx.h"
#include "Line.h"


Line::Line()
{
}


Line::~Line()
{
}

void Line::draw(Gdiplus::Graphics &graphics)
{
	graphics.DrawLine(m_pen.get(),start_point,end_point);
}