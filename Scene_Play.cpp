#include "Scene_Play.h"

Scene_Play::Scene_Play(Game * game, const std::string& levelPath)
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

void Scene::drawLine(const Vec2f& p1, const Vec2f& p2)
{
	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(p1.x, p1.y)),
		sf::Vertex(sf::Vector2f(p2.x, p2.y))
	};
	m_game->getWindow().draw(line, 2, sf::Lines);
}