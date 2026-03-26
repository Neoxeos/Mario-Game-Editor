#pragma once

#include "Assets.h"
#include <string>
#include <SFML/Graphics.hpp>


class Animation
{
	size_t m_framecount = 1; // total frames
	size_t m_currentFrame = 0; // current frame
	size_t m_speed = 0; // how many updates before we switch to the next frame
	std::string m_name = "none";
	std::string m_textureName = "none";
	sf::IntRect m_textureRect;
public:
	Animation() = default;
	Animation(const std::string& name, const std::string& textureName)
		: Animation(name, textureName, 1, 0) {}

	Animation(const std::string& name, const std::string& textureName, size_t framecount, size_t speed)
		: m_name(name)
		, m_currentFrame(0)
		, m_textureName(textureName)
		, m_framecount(framecount)
		, m_speed(speed)
	{
		const sf::Texture& t = Assets::getInstance().getTexture(textureName);
		//m_textureRect.size = {int(t.getSize().x / framecount), int(t.getSize().y)};
	}
	void update()
	{
		m_currentFrame++;
		if (m_speed == 0) return; // if speed is 0, we never switch frames
		m_currentFrame = (m_currentFrame + 1) % m_framecount;
	}
	bool hasEnded() const
	{
		return m_currentFrame == m_framecount - 1;
	}
	const std::string& getName() const { return m_name; }
	const sf::IntRect& getRect() const { return m_textureRect; }
};