#include "stdafx.h"
#include "PenWeight.h"

#include <sstream>

PenWeight::PenWeight()
{
}


PenWeight::~PenWeight()
{
}

std::shared_ptr<Gdiplus::Pen> PenWeight::getPen(Gdiplus::Color color, Gdiplus::REAL width)
{
	std::stringstream ss;
	std::string key;

	// construct the key using a string stream
	ss << "("
		<< color.GetR() << ","
		<< color.GetG() << ","
		<< color.GetB() << ","
		<< ")" << ";"
		<< width;

	ss >> key;

	auto it = penMap.find(key);

	if (it != penMap.end()) // pen already exists
	{
		return it->second;
	}
	else // new pen selected
	{
		penMap[key] = std::make_shared<Gdiplus::Pen>(color, width);
		return penMap[key];
	}
}