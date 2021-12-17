#pragma once

#include "GEngine/GPrerequisites.h"

class GVec4
{
public:
	f32 x{}, y{}, z{}, w{};

public:
	GVec4();
	GVec4(f32 x, f32 y, f32 z, f32 w) : x(x), y(y), z(z), w(w) {}
	~GVec4();
};

