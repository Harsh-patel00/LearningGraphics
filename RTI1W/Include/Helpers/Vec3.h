#pragma once

#include <vector>
#include <iostream>

class Vec3
{
public:
	float x{}, y{}, z{};
	float r{}, g{}, b{};	
	 
public:
	Vec3(){}
	Vec3(float x, float y, float z) : x(x), y(y), z(z), r(x), g(y), b(z) {}
	Vec3(float val) : x(val), y(val), z(val), r(val), g(val), b(val) {}

public:
	Vec3 Up(); // Y-axis is pointing towards ceiling/sky
	Vec3 Forward(); // Z-axis is towards you.
	Vec3 Right(); // X-axis is pointing towards your right

	Vec3 Down(); // Y-axis is pointing towards floor/earth
	Vec3 Backward(); // Z-axis is into the screen.
	Vec3 Left(); // X-axis is pointing towards your left

	static Vec3 Zero();

private:
	friend std::ostream& operator <<(std::ostream& os, const Vec3 &v);

};

typedef Vec3 Color;
typedef Vec3 Position;
typedef Vec3 Direction;

