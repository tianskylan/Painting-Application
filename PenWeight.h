#pragma once

#include "singleton.h"
#include <memory>
#include <string>
#include <unordered_map>

class PenWeight : public Singleton<PenWeight>
{
	DECLARE_SINGLETON(PenWeight);

	

public:
	PenWeight();
	~PenWeight();

	std::shared_ptr<Gdiplus::Pen> getPen(Gdiplus::Color, Gdiplus::REAL);


	std::unordered_map<std::string, std::shared_ptr<Gdiplus::Pen>> penMap;
};

