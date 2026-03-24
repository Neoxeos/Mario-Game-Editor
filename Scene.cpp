#include "Scene.h"
#include "Game.h"

Scene::Scene()
	: m_game(nullptr)
{
}

Scene::Scene(Game* game)
	: m_game(game)
{
}

void Scene::setPaused(bool paused)
{
	m_paused = paused;
}

size_t Scene::width() const
{
	return m_game->getWindow().getSize().x;
}

size_t Scene::height() const
{
	return m_game->getWindow().getSize().y;
}

size_t Scene::currentFrame() const
{
	return m_currentFrame;
}

bool Scene::hasEnded() const
{
	return m_hasEnded;
}

const ActionMap& Scene::getActionMap() const
{
	return m_actionMap;
}

void Scene::registerAction(int inputKey, const std::string& actionName)
{
	m_actionMap[inputKey] = actionName;
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

