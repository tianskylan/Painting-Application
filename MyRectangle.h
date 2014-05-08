#pragma once
#include "Shape.h"
class MyRectangle :
	public Shape
{
public:
	MyRectangle();
	~MyRectangle();

	void draw(Gdiplus::Graphics &);
};

