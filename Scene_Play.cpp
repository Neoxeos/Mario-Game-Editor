#include "Scene_Play.h"
#include "Physics.h"
#include "Assets.h"
#include "Game.h"
#include "Components.h"
#include "Scene_Menu.h"
#include "Action.hpp"

#include <iostream>
#include <fstream>

Scene_Play::Scene_Play(Game * game,const std::string& levelPath)
	: Scene(game)
	, m_levelPath(levelPath)
{
	init(m_levelPath);
}

void Scene_Play::init(const std::string& levelPath)
{
	registerAction(sf::Keyboard::P, "PAUSE");
	registerAction(sf::Keyboard::Escape, "QUIT");
	registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");
	registerAction(sf::Keyboard::C, "TOGGLE_COLLISION");
	registerAction(sf::Keyboard::G, "TOGGLE_GRID");

	registerAction(sf::Keyboard::Up, "JUMP");
	registerAction(sf::Keyboard::Left, "MOVE_LEFT");
	registerAction(sf::Keyboard::Right, "MOVE_RIGHT");
	registerAction(sf::Keyboard::Down, "MOVE_DOWN");
	registerAction(sf::Keyboard::Space, "SHOOT");

	loadLevel(levelPath);
}

Vec2f Scene_Play::gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity)
{
	return Vec2f(0, 0);
}

void Scene_Play::loadLevel(const std::string& filename)
{
	m_entityManager = EntityManager();
	std::ifstream f(filename);
	std::string str;

	while (f >> str) {
		if (str == "Tile") {
			std::string type; size_t x, y;
			f >> type >> x >> y;
			auto e = m_entityManager.addEntity("tile");
			e->addComponent<CTransform>(Vec2f(x, y));
			bool repeat = true;
			if (type == "Explosion" || type == "Coin") repeat = false;
			e->addComponent<CAnimation>(m_game->getAssets().getAnimation(type), repeat);
			e->getComponent<CTransform>().prevPos = e->getComponent<CTransform>().pos;
			// add a bounding box, this will now show up if we press the 'C' Key
			//e->addComponent<CBoundingBox>(m_game->getAssets().getAnimation(type).getSize());
		}
		else if (str == "Dec") {
			std::string type; size_t x, y;
			f >> type >> x >> y;
			auto e = m_entityManager.addEntity("dec");
			e->addComponent<CTransform>(Vec2f(x, y));
			e->addComponent<CAnimation>(m_game->getAssets().getAnimation(type), true);
		}
		else {
			//f >> m_playerConfig.X >> m_playerConfig.Y >> m_playerConfig.CX >> m_playerConfig.CY
			//	>> m_playerConfig.SPEED >> m_playerConfig.JUMP >> m_playerConfig.MAXSPEED >> m_playerConfig.GRAVITY
			//	>> m_playerConfig.WEAPON;
			spawnPlayer();

		}
	}
}

void Scene_Play::spawnPlayer()
{
	// check to see i plyer already exists
	if (!m_player) { m_player = m_entityManager.addEntity("player");}
	// TODO
}

void Scene_Play::spawnBullet(std::shared_ptr<Entity> entity)
{
	//todo
}

void Scene_Play::update()
{
	if (!m_paused) {
		m_entityManager.update();
		sMovement();
		sCollision();
		sLifespan();
		m_currentFrame++;
		sAnimation();
	}
	sRender();
}

void Scene_Play::sMovement()
{
	// TODO
}

void Scene_Play::sLifespan()
{
	// TODO
}

void Scene_Play::sCollision()
{
	// TODO
}

void Scene_Play::sDoAction(const Action& action)
{
	// TODO
}

void Scene_Play::sAnimation()
{
	// TODO
}

void Scene_Play::onEnd()
{
	// TODO
}

void Scene_Play::sGUI()
{
	// TODO
}

void Scene_Play::sRender()
{
	// TODO
}

void Scene_Play::drawLine(const Vec2f& p1, const Vec2f& p2)
{
	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(p1.x, p1.y)),
		sf::Vertex(sf::Vector2f(p2.x, p2.y))
	};
	m_game->getWindow().draw(line, 2, sf::Lines);
}