#pragma once

#include "drawView.h"
#include "Shape.h"
#include <memory>
class ShapeMaker
{
public:
	ShapeMaker();
	~ShapeMaker();
	static std::shared_ptr<Shape> makeShape(int);
};

