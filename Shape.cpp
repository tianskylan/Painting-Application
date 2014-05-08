#include "stdafx.h"
#include "Shape.h"

Shape::Shape()
{
}


Shape::~Shape()
{
}

void Shape::setPen(std::shared_ptr<Gdiplus::Pen> pen)
{
	m_pen = pen;
}

void Shape::setStartPoint(Gdiplus::Point start)
{
	start_point = start;
}

void Shape::setEndPoint(Gdiplus::Point end)
{
	end_point = end;
}