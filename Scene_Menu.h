#pragma once


#include "Scene.h"
#include "EntityManager.h"
#include <memory>
#include <deque>
#include <map>



class Scene_Menu : public Scene
{

protected:

	std::string m_title;
	std::vector<std::string> m_menuStrings;
	std::vector<std::string> m_levelPaths;
	sf::Text m_menuText;
	size_t m_selectedMenuIndex = 0;

	void init();
	void update();
	void onEnd();

public:
	Scene_Menu() = default;
	Scene_Menu(Game* game = nullptr);

	void doAction(const Action& a);
	void sRender();
};