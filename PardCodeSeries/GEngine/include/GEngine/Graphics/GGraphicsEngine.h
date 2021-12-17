#pragma once
#include "GEngine/Math/GVec4.h"

class GGraphicsEngine
{
public:
	GGraphicsEngine();
	~GGraphicsEngine();

public:
	void Clear(const GVec4 &color);
};

