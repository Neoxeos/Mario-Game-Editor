#pragma once

#include "Scene.h"
#include "EntityManager.h"

class Scene_Play : public Scene
{
	struct PlayerConfig
	{
		float X, Y, CX, CY, SPEED, MAXSEPEED, JUMP, GRAVITY;
		std::string WEAPON;
	};

protected:
	std::shared_ptr<Entity> m_player;
	std::string m_levelPath;
	PlayerConfig m_playerConfig;
	bool m_drawTextures = true;
	bool m_drawCollisions = false;
	bool m_drawGrid = false;
	const Vec2f m_gridSize = { 64, 64 };
	//EntityManager m_entityManager;
	//Game m_game;

	void init(const std::string& levelPath);
	void loadLevel(const std::string& filename);
	void update();
	void onEnd();
	Vec2f gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity);
	void spawnBullet(std::shared_ptr<Entity> entity);
	void sMovement();
	void sLifespan();
	void sCollision();
	void doAction(const Action& action);
	void sGUI();
	void sRender();
	void drawLine(const Vec2f& p1, const Vec2f& p2);
	void sAnimation();
	void spawnPlayer();

public:
	Scene_Play() = default;
	Scene_Play(Game* game) : Scene(game) {}
	Scene_Play(Game* game,const std::string& levelPath);
};