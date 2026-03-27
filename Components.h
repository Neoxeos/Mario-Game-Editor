#pragma once

#include "Animation.hpp"
#include "Assets.h"
#include "Vec2f.h"

class Component
{
public:
	bool has = false;
};

class CTransform : public Component
{
public:
	Vec2f pos = { 0.0, 0.0 };
	Vec2f prevPos = { 0.0, 0.0 };
	Vec2f scale = { 1.0, 1.0 };
	Vec2f velocity = { 0.0, 0.0 };
	float angle = 0;

	CTransform() = default;
	CTransform(const Vec2f& p)
		:pos(p)
	{
	};
	CTransform(const Vec2f& p, const Vec2f& v, const Vec2f& sc, float a)
		:pos(p), prevPos(p), velocity(v), angle(a), scale(sc)
	{
	};
};

class CShape : public Component
{
public:
	sf::CircleShape circle;

	CShape() = default;
	CShape(float radius, int points, const sf::Color& fill, const sf::Color& outline, float thickness)
		:circle(radius, points)
	{
		circle.setFillColor(fill);
		circle.setOutlineColor(outline);
		circle.setOutlineThickness(thickness);
		circle.setOrigin(radius, radius); // here we set the origin of the cirlce to be at it's center point instead of the bounding box!
	}
};

class CCollision : public Component
{
public:
	float radius = 0;
	CCollision() = default;
	CCollision(float r)
		:radius(r) {}
};

class CScore : public Component
{
public:
	CScore() = default;
	int score = 0;
	CScore(int s)
		:score(s) {}
};

class CLifespan : public Component
{
public:
	int lifespan = 0; // remaining lifespan for entity 
	int frameCreated = 0;     // total initial amount of lifespan
	CLifespan() = default;
	CLifespan(int duration, int frame)
		:lifespan(duration), frameCreated(frame) {}
};

class CInput : public Component
{
public:
	bool up = false;
	bool left = false;
	bool right = false;
	bool down = false;
	bool shoot = false;
	bool canShoot = true;
	bool canJump = true;

	CInput() = default;
};

class CBoundingBox : public Component
{
public:
	Vec2f size;
	Vec2f halfSize;
	CBoundingBox() = default;
	CBoundingBox(const Vec2f& s)
		:size(s), halfSize(s.x / 2, s.y / 2) {}
};

class CAnimation : public Component
{
public:
	Animation animation;
	bool repeat = false;
	CAnimation() = default;
	CAnimation(const Animation& animation, bool r)
		: animation(animation)
		, repeat(r) 
	{}
};

class CGravity : public Component
{
public:
	float gravity = 0;
	CGravity() = default;
	CGravity(float g) : gravity(g) {};
};

class CState : public Component
{
public:
	std::string state = "jumping";
	CState() = default;
	CState(const std::string& s) : state(s) {};
};


// needed to store class in a tuple since class needs to be default constructible to be stored in a tuple and used in the Entity class
static_assert(std::is_default_constructible_v<CTransform>);
static_assert(std::is_default_constructible_v<CLifespan>);
static_assert(std::is_default_constructible_v<CInput>);
static_assert(std::is_default_constructible_v<CBoundingBox>);
static_assert(std::is_default_constructible_v<CAnimation>);
static_assert(std::is_default_constructible_v<CGravity>);
static_assert(std::is_default_constructible_v<CState>);

static_assert(std::is_default_constructible_v<Vec2f>);
static_assert(std::is_default_constructible_v<Animation>);

static_assert(std::is_default_constructible_v<CCollision>);
static_assert(std::is_default_constructible_v<CScore>);
static_assert(std::is_default_constructible_v<CShape>);