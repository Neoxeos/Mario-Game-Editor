#pragma once

#include "SFML/Graphics.hpp"
#include <map>
#include <cassert>
#include <iostream>
#include <fstream>
#include "Components.h"
#include "Animation.h"

class Assets
{
	std::map<std::string, sf::Texture> m_textureMap;
	std::map<std::string, Animation> m_animationMap;
	//std::map<std::string, sf::Sound> m_sounds;
	std::map<std::string, sf::Font> m_fontMap;

	void addTexture(const std::string& textureName, const std::string& path, bool smooth = true);
	void addAnimation(const std::string& animationName, const std::string& textureName, size_t frameCount, size_t speed);
	void addFont(const std::string& fontName, const std::string& path);
	//void addSound(const std::string& soundName, const std::string& path);

	Assets() = default;
public:

	static Assets& getInstance(); // Enforce singleton pattern

	void loadFromFile(const std::string& path);

	const sf::Texture& getTexture(const std::string& textureName) const {return m_textureMap.at(textureName); }
	const Animation& getAnimation(const std::string& animationName) const {return m_animationMap.at(animationName); }
	const sf::Font& getFont(const std::string& fontName) const { return m_fontMap.at(fontName); }
	//const sf::Sound& getSound(const std::string& soundName) const { return m_sounds.at(soundName); }

	const std::map<std::string, sf::Texture>& getTextures() const { return m_textureMap;}
	const std::map<std::string, Animation>& getAnimations() const { return m_animationMap;}
};