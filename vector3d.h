#ifndef __VECTOR3D_H__
#define __VECTOR3D_H__
class Vector3d
{
public:
	float x_, y_, z_;
	Vector3d(float x = 0, float y = 0, float z = 0);
	float modul2();
	float modul();
	Vector3d cross(Vector3d& v);
	float dot(Vector3d& v);
	Vector3d operator-(const Vector3d& v);
	Vector3d operator+(const Vector3d& v);
	Vector3d operator*(float x);
	Vector3d operator/(float x);
	float mag();
	void normalize();
};
#endif