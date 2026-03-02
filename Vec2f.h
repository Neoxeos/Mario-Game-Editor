#pragma once

class Vec2f
{
public:
	float x = 0;
	float y = 0;

	Vec2f();
	Vec2f(float xin, float yin);

	bool operator == (const Vec2f& rhs) const;
	bool operator != (const Vec2f& rhs) const;

	Vec2f operator + (const Vec2f& rhs) const;
	Vec2f operator - (const Vec2f& rhs) const;
	Vec2f operator / (const Vec2f& rhs) const;
	Vec2f operator * (const Vec2f& rhs) const;

	Vec2f operator += (const Vec2f& rhs) const;
	Vec2f operator -= (const Vec2f& rhs) const;
	Vec2f operator *= (const Vec2f& rhs) const;
	Vec2f operator /= (const Vec2f& rhs) const;

	float dist(const Vec2f& rhs) const;
	Vec2f normalize();
};