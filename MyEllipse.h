#pragma once
#include "Shape.h"
class MyEllipse :
	public Shape
{
public:
	MyEllipse();
	~MyEllipse();

	void draw(Gdiplus::Graphics &) override;
};

