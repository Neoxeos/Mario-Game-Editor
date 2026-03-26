#include "Scene_Menu.h"
#include "Scene_Play.h"
#include "Assets.h"
#include "Game.h"
#include "Action.hpp"
#include "Components.h"
#include <SFML/Graphics/View.hpp>


Scene_Menu::Scene_Menu(Game* game)
	: Scene(game)
{
	init();
}

void Scene_Menu::init()
{
	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::D, "PLAY");
	registerAction(sf::Keyboard::Escape, "QUIT");

	m_title = "Mega Mario";
	m_menuStrings.push_back("Level 1");
	m_menuStrings.push_back("Level 2");
	m_menuStrings.push_back("Level 3");

	m_levelPaths.push_back("level1.txt");
	m_levelPaths.push_back("level2.txt");
	m_levelPaths.push_back("level3.txt");

	m_menuText.setFont(m_game->getAssets().getFont("Megaman"));
	m_menuText.setCharacterSize(64);

}

void Scene_Menu::update()
{
	m_entityManager.update();
	sRender();
}

void Scene_Menu::onEnd()
{
	m_hasEnded = true;
	m_game->quit();
}


void Scene_Menu::doAction(const Action& a)
{
	if (a.getType() == "START")
	{
		if (a.getType() == "UP")
		{
			if ( m_selectedMenuIndex > 0) { m_selectedMenuIndex--; }
			else { m_selectedMenuIndex = m_menuStrings.size() - 1; }
		}
		else if (a.getType() == "DOWN")
		{
			m_selectedMenuIndex = (m_selectedMenuIndex + 1) % m_menuStrings.size();
		}
		else if (a.getName() == "PLAY")
		{
			m_game->changeScene<Scene_Play>("play", std::make_shared<Scene_Play>(m_game, m_levelPaths[m_selectedMenuIndex]));
		}
		else if (a.getName() == "UPDATE")
		{
			update();
		}
		else if (a.getName() == "QUIT")
		{
			onEnd();
		}
	}
}

void Scene_Menu::sRender()
{
	m_game->getWindow().setView(m_game->getWindow().getDefaultView());
	m_game->getWindow().clear(sf::Color(100, 100, 255));

	m_menuText.setCharacterSize(48);
	m_menuText.setString(m_title);
	m_menuText.setFillColor(sf::Color::Black);
	m_menuText.setPosition(sf::Vector2f(10, 10));
	m_game->getWindow().draw(m_menuText);

	// menu options
	for (size_t i = 0; i < m_menuStrings.size(); i++)
	{
		m_menuText.setString(m_menuStrings[i]);
		m_menuText.setFillColor(i == m_selectedMenuIndex ? sf::Color::White : sf::Color::Black);
		m_menuText.setPosition(sf::Vector2f(10, 100 + i * 72));
		m_game->getWindow().draw(m_menuText);
	}

	// menu controls
	m_menuText.setCharacterSize(24);
	m_menuText.setFillColor(sf::Color::Black);
	m_menuText.setString("Use W/S to navigate, D to select");
	m_menuText.setPosition(sf::Vector2f(10, 690));
	m_game->getWindow().draw(m_menuText);
}
