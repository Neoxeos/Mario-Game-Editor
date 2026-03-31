#pragma once

#include "Assets.h"
#include "Vec2f.h"
#include <string>
#include <SFML/Graphics.hpp>


class Animation
{
	size_t m_framecount = 1; // total frames
	size_t m_currentFrame = 0; // current frame
	size_t m_speed = 0; // how many updates before we switch to the next frame
	std::string m_name = "none";
	sf::Sprite m_sprite;
	Vec2f m_size = {1, 1};
public:
	Animation() = default;

	Animation(const std::string& name, const sf::Texture& t)
		: Animation(name, t, 1, 0) 
	{
		m_sprite.setTexture(t);
		m_size = Vec2f(t.getSize().x, t.getSize().y);
		m_sprite.setOrigin(m_size.x / 2.0f, m_size.y / 2.0f);
		sf::IntRect rect = sf::IntRect((std::floor(m_currentFrame) * m_size.x), 0, m_size.x, m_size.y);
		m_sprite.setTextureRect(rect);
	}

	Animation(const std::string& name, const sf::Texture& t, size_t framecount, size_t speed)
		: m_name(name)
		, m_currentFrame(0)
		, m_framecount(framecount)
		, m_speed(speed)
	{
		m_sprite.setTexture(t);
		m_size = Vec2f(t.getSize().x / framecount, t.getSize().y);
		m_sprite.setOrigin(m_size.x / 2.0f, m_size.y / 2.0f);
		sf::IntRect rect = sf::IntRect((std::floor(m_currentFrame) * m_size.x), 0, m_size.x, m_size.y);
		m_sprite.setTextureRect(rect);
	}

	void update()
	{
		m_currentFrame++;
		if (m_speed == 0) return; // if speed is 0, we never switch frames
		int frame = ((int)m_currentFrame / (int)m_speed) % (int)m_framecount; 
		sf::IntRect rect = sf::IntRect(frame * m_size.x, 0, m_size.x, (int)m_size.y);

		m_sprite.setTextureRect(rect);

	}

	bool hasEnded() const
	{
		if (m_speed == 0) return false;
		if (m_currentFrame >= m_framecount * m_speed) {
			return true;
		}
		return false;
	}
	const std::string& getName() const { return m_name; }
	const Vec2f& getSize() const { return m_size; }
	sf::Sprite& getSprite() { return m_sprite; }
};