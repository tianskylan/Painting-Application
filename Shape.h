#pragma once
#include <memory>
class Shape
{
public:
	Shape();
	~Shape();
	virtual void draw(Gdiplus::Graphics &) = 0;
	virtual void setPen(std::shared_ptr<Gdiplus::Pen>);
	virtual void setStartPoint(Gdiplus::Point);
	virtual void setEndPoint(Gdiplus::Point);
	
	Gdiplus::Point start_point;
	Gdiplus::Point end_point;

	std::shared_ptr<Gdiplus::Pen> m_pen;
};

