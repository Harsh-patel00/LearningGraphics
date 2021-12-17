#include "Helpers/Vec3.h"

Vec3 Vec3::Up()
{
	return Vec3(0, 1, 0);
}

Vec3 Vec3::Forward()
{
	return Vec3(0, 0, 1);
}

Vec3 Vec3::Right()
{
	return Vec3(1, 0, 0);
}

Vec3 Vec3::Down()
{
	return Vec3(0, -1, 0);
}

Vec3 Vec3::Backward()
{
	return Vec3(0, 0, -1);
}

Vec3 Vec3::Left()
{
	return Vec3(-1, 0, 0);
}

Vec3 Vec3::Zero()
{
	return Vec3(0, 0, 0);
}

std::ostream& operator<<(std::ostream& os, const Vec3 &v)
{
	os << "Vec3 : { " << v.x << ", " << v.y << ", " << v.z << "}";

	return os;
}
