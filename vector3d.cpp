#include "vector3d.h"
#include <math.h>
Vector3d::Vector3d(float a, float b, float c)
	:x_{ a }, y_{ b }, z_{ c }
{}
float Vector3d::modul2()
{
	return (x_ * x_ + y_ * y_ + z_ * z_);
}
float Vector3d::modul()
{
	return sqrtf(modul2());
}
Vector3d Vector3d::cross(Vector3d& v)
{
	Vector3d rev(y_ * v.z_ - z_ * v.y_, z_ * v.x_ - x_ * v.z_, x_ * v.y_ - y_ * v.x_);
	return rev;
}
float Vector3d::dot(Vector3d& v)
{
	return (x_ * v.x_ + y_ * v.y_ + z_ * v.z_);
}
Vector3d Vector3d::operator-(const Vector3d& v)
{
	return Vector3d(x_ - v.x_, y_ - v.y_, z_ - v.z_);
}
Vector3d Vector3d::operator+(const Vector3d& v)
{
	return Vector3d(x_ + v.x_, y_ + v.y_, z_ + v.z_);
}
Vector3d Vector3d::operator*(float x)
{
	return Vector3d(x_ * x, y_ * x, z_ * x);
}
Vector3d Vector3d::operator/(float x)
{
	return Vector3d(x_ / x, y_ / x, z_ / x);
}
float Vector3d::mag() { return sqrtf(x_ * x_ + y_ * y_ + z_ * z_); }
void Vector3d::normalize()
{
	float length = mag();
	x_ = x_ / length;
	y_ = y_ / length;
	z_ = z_ / length;
};
