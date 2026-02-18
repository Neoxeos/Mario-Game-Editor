#include "Assets.h"
#include <cassert>

Assets::Assets()
{

}

void Assets::loadFromFile(const std::string& path)
{
	std::ifstream file(path);
	std::string str;
	while (file.good())
	{
		file >> sr;

		if (sr == "Texture")
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

void Assets::addTexture(std::string& )