#include "Scene_Play.h"

Scene_Play::Scene_Play(Game * game, const std::string& levelPath)
	: Scene(game)
	, m_levelPath(levelPath)
{
	init(m_levelPath);
}

void Scene_Play::init(const std::string& levelPath)
{
	registerAction(sf::Keyboard::Scancode::P, "PAUSE");
	registerAction(sf::Keyboard::Scancode::Escape, "QUIT");
	registerAction(sf::Keyboard::Scancode::T, "TOGGLE_TEXTURE");
	registerAction(sf::Keyboard::Scancode::C, "TOGGLE_COLLISION");
	registerAction(sf::Keyboard::Scancode::G, "TOGGLE_GRID");

	// TODO

	loadLevel(levelPath);
}

Vec2f Scene_Play::gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity)
{
	return Vec2f(0, 0);
}

void Scene_Play::loadLevel(const std::string& filename)
{
	m_entityManager = EntityManager();
	// TODO


	spawnPlayer();
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
	m_entityManager.update();
	// TODO
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

void Scene_Play::doAction(const Action& action)
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
		{p1, sf::Color::White},
		{p2, sf::Color::White}
	};

	m_game.window().draw(line, 2, sf::Lines);
}
