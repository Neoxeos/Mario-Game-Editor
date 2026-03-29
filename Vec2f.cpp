#include "Vec2f.h"
#include <cmath>


Vec2f::Vec2f()
{}

Vec2f::Vec2f(float xin, float yin)
	:x(xin), y(yin)
{}

Vec2f Vec2f::operator + (const Vec2f& rhs) const
{
	return Vec2f(this->x + rhs.x, this->y + rhs.y);
}

bool Vec2f::operator == (const Vec2f& rhs) const 
{
	return(this->x == rhs.x && this->y == rhs.y);
}

bool Vec2f::operator != (const Vec2f& rhs) const
{
	return(this->x != rhs.x && this->y != rhs.y);
}

Vec2f Vec2f::operator - (const Vec2f& rhs) const
{
	return Vec2f(rhs.x - this->x, rhs.y - this->y);
}

Vec2f Vec2f::operator / (const Vec2f& rhs) const
{
	return Vec2f(this->x / rhs.x, this->y / rhs.y);
}

Vec2f Vec2f::operator * (const Vec2f& rhs) const
{
	return Vec2f(this->x * rhs.x, this->y * rhs.y);
}

Vec2f Vec2f::operator += (const Vec2f& rhs) const
{
	Vec2f toReturn = Vec2f(this->x + rhs.x, this->y + rhs.y);
	return toReturn;
}

Vec2f Vec2f::operator -= (const Vec2f& rhs) const
{
	Vec2f toReturn = Vec2f(this->x - rhs.x, this->y - rhs.y);
	return toReturn;
}

Vec2f Vec2f::operator *= (const Vec2f& rhs) const
{
	Vec2f toReturn = Vec2f(this->x * rhs.x, this->y * rhs.y);
	return toReturn;
}

Vec2f Vec2f::operator /= (const Vec2f& rhs) const
{
	Vec2f toReturn = Vec2f(this->x / rhs.x, this->y / rhs.y);
	return toReturn;
}

float Vec2f::dist(const Vec2f& rhs) const
{
	// first get difference between this and rhs
	Vec2f diff = Vec2f(this->x - rhs.x, this->y - rhs.y);
	// then get the actual distance
	return sqrtf(diff.x * diff.x + diff.y * diff.y);
}

float Vec2f::length() const
{
	return std::sqrt(x * x + y * y);
}

Vec2f Vec2f::normalize()
{
	// first get length
	float length = sqrtf(x * x + y * y);
	// then normalize
	return Vec2f(x / length, y / length);
}

void Vec2f::rotate(float theta)
{
	float rad = theta * (float)3.14159265 / 180.0f;
	float cosine = cos(rad);
	float sine = sin(rad);

	float xnew = x * cosine - y * sine;
	float ynew = x * sine + y * cosine;

	x = xnew;
	y = ynew;
}