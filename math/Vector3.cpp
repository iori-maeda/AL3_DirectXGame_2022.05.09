#include "Vector3.h"
#include<cmath>			// sqrt

// コンストラクタ
Vector3::Vector3()
	:x(0),y(0),z(0)
{}
Vector3::Vector3(float x, float y, float z) 
	:x(x),y(y),z(z)
{}
// ノルム（長さ）を求める
float Vector3::length() const
{
	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}
// 正規化する
Vector3& Vector3::normalize()
{
	float len = length();
	if (len != 0)
	{
		return *this /= len;
	}
	return *this;
}
// 内積を求める
float Vector3::dot(const Vector3& v) const
{
	return x * v.x + y * v.y + z * v.z;
}
// 外積を求める
Vector3 Vector3::cross(const Vector3& v) const
{
	Vector3 vector_cross = 
	{
		y * v.z - z * v.y,
		z * v.x - x * v.z,
		x * v.y - y * v.x
	};
	return vector_cross;
}

// 単項演算子
Vector3 Vector3::operator+() const
{
	return *this;
}
Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}
// 代入演算子
Vector3& Vector3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}
Vector3& Vector3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}
Vector3& Vector3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}
Vector3& Vector3::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}


// 2項演算子
const Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp += v2;
}
const Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp -= v2;
}
const Vector3 operator*(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp *= s;
}
const Vector3 operator*(float s, const Vector3& v)
{
	return v * s;
}
const Vector3 operator/(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp /= s;
}