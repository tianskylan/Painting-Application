#include "stdafx.h"
#include "MyRectangle.h"


MyRectangle::MyRectangle()
{
}


MyRectangle::~MyRectangle()
{
}

void MyRectangle::draw(Gdiplus::Graphics &graphics)
{
	int topleft_x = start_point.X < end_point.X ? start_point.X : end_point.X;
	int topleft_y = start_point.Y < end_point.Y ? start_point.Y : end_point.Y;

	int bottomright_x = start_point.X > end_point.X ? start_point.X : end_point.X;
	int bottomright_y = start_point.Y > end_point.Y ? start_point.Y : end_point.Y;

	int width = bottomright_x - topleft_x;
	int height = bottomright_y - topleft_y;

	graphics.DrawRectangle(m_pen.get(),topleft_x,topleft_y,width,height);
}