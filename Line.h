#pragma once
#include "Shape.h"

class Line :
	public Shape
{
public:
	Line();
	~Line();

	void draw(Gdiplus::Graphics &) override;
};

// qiqiqiqiqiqiqiqiqiqiqiqifeng!!!!!!!