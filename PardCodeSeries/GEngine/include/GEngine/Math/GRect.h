#pragma once
#include "GEngine/GPrerequisites.h"

// Class for viewport manipulation
class GRect
{
public:
	i32 width{}, height{}, left{}, top{};

public:
	GRect() {};
	GRect(i32 width, i32 height) : width(width), height(height) {};
	GRect(i32 left, i32 top, i32 width, i32 height) : left(left), top(top), width(width), height(height) {};
	GRect(const GRect& rect) : left(rect.left), top(rect.top), width(rect.width), height(rect.height) {};
};