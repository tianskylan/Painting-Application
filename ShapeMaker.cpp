#include "stdafx.h"
#include "ShapeMaker.h"

#include "Line.h"
#include "MyEllipse.h"
#include "MyRectangle.h"


ShapeMaker::ShapeMaker()
{
}


ShapeMaker::~ShapeMaker()
{
}

/*input : current shape indicator
	*cur == 0 : Line
	*cur == 1 : Ellipse
	*cur == 2 : Rectangle
*/
std::shared_ptr<Shape> ShapeMaker::makeShape(int cur)
{
	if (cur == 0)
	{
		return std::make_shared<Line>();
	}
	else if (cur == 1)
	{
		return std::make_shared<MyEllipse>();
	}
	else if (cur == 2)
	{
		return std::make_shared<MyRectangle>();
	}

	return nullptr;
}