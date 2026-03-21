#pragma once

#include "Game.h"
#include "Action.h"

class Scene
{
	Game* m_game;
	EntityManager	m_entityManager;
	int m_frame;
	std::map<int, std::string> m_actionsMap;
	bool m_paused = false;
public:
	Scene(Game& game);
	virtual void update() = 0;
	virtual void doAction(const Action& action) = 0;
	virtual void sRender() = 0;

	void doAction(Action& action);
	void simulate(int);
	void registerAction(Action& action);
};