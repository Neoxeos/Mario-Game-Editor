#pragma once

#include "Action.h"
#include "EntityManager.h"

#include <memory>

class Game;

typedef std::map<int, std::string> ActionMap;

class Scene
{
	Game* m_game;
	EntityManager	m_entityManager;
	size_t m_currentFrame = 0;
	ActionMap m_actionMap;
	bool m_paused = false;
	bool m_hasEnded = false;

	virtual void onEnd() = 0;
	void setPaused(bool paused) { m_paused = paused; }
public:
	Scene();
	Scene(Game* game);
	virtual void update() = 0;
	virtual void doAction(const Action& action) = 0;
	virtual void sRender() = 0;

	void doAction(const Action& action);
	void simulate(const size_t frames);
	void registerAction(int inputKey, const std::string& actionName);

	size_t width() const;
	size_t height() const;
	size_t currentFrame() const;

	bool hasEnded() const;
	const ActionMap& getActionMap() const;
	void drawLine(const Vec2f& p1, const Vec2f& p2);
};