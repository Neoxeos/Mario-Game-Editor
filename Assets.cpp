
#include "Assets.h"
#include "Animation.hpp"
#include <cassert>

Assets& Assets::getInstance()
{
	static Assets instance;
	return instance;
}

void Assets::addTexture(const std::string& textureName, const std::string& path, bool smooth)
{
	getInstance().m_textureMap[textureName] = sf::Texture();
	
	if (!getInstance().m_textureMap[textureName].loadFromFile(path))
	{
		std::cerr << "Failed to load texture: " << path << std::endl;
		getInstance().m_textureMap.erase(textureName);
	} 
	else
	{
		getInstance().m_textureMap[textureName].setSmooth(smooth);
		std::cout << "Loaded texture: " << path << std::endl;
	}
}

const sf::Texture& Assets::getTexture(const std::string& textureName) const {
	assert(getInstance().m_textureMap.find(textureName) != getInstance().m_textureMap.end());
	return getInstance().m_textureMap.at(textureName);
}


void Assets::addAnimation(const std::string& animationName, const std::string& textureName, size_t frameCount, size_t speed)
{
	getInstance().m_animationMap[animationName] = Animation(animationName, textureName, frameCount, speed);
}

const Animation& Assets::getAnimation(const std::string& animationName) const {
	assert(getInstance().m_animationMap.find(animationName) != getInstance().m_animationMap.end());
	return getInstance().m_animationMap.at(animationName);
}

void Assets::addFont(const std::string& fontName, const std::string& path)
{
	getInstance().m_fontMap[fontName] = sf::Font();
	if (!getInstance().m_fontMap[fontName].loadFromFile(path))
	{
		std::cerr << "Failed to load font: " << path << std::endl;
		getInstance().m_fontMap.erase(fontName);
	} 
	else
	{
		std::cout << "Loaded font: " << path << std::endl;
	}
}

const sf::Font& Assets::getFont(const std::string& fontName) const {
	assert(getInstance().m_fontMap.find(fontName) != getInstance().m_fontMap.end());
	return getInstance().m_fontMap.at(fontName);
}

//const sf::Sound& getSound(const std::string& soundName) const { return m_sounds.at(soundName); }

void Assets::loadFromFile(const std::string& path)
{
	std::ifstream file(path);
	std::string str;
	while (file.good())
	{
		file >> str;

		if (str == "Texture")
		{
			std::string name, path;
			file >> name >> path;
			addTexture(name, path);
		}
		else if (str == "Animation")
		{
			std::string name, texture;
			size_t frames, speed;
			file >> name >> texture >> frames >> speed;
			addAnimation(name, texture, frames, speed);
		}
		else if (str == "Font")
		{
			std::string name, path;
			file >> name >> path;
			addFont(name, path);
		}
		else
		{
			std::cerr << "Unknown Asset type: " << str << std::endl;
		}
	}
}
