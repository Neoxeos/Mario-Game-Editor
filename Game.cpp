#define _USE_MATH_DEFINES

#include "Game.h"
#include "Assets.h"
#include "Scene_Play.h"	
#include "Scene_Menu.h"

#include <cmath>
#include <iostream>
#include <fstream>

Game::Game(const std::string& config)
{
	init(config);
}

void Game::init(const std::string& path)
{
	m_assets.loadFromFile(path);

	m_window.create(sf::VideoMode(1280, 768), "Game");
	m_window.setFramerateLimit(60);

	changeScene<Scene_Menu>("menu", std::make_shared<Scene_Menu>(this));
	ImGui::SFML::Init(m_window);
}

std::shared_ptr<Scene> Game::getCurrentScene()
{
	return m_scenes.at(m_scene);
}

bool Game::isRunning() const
{
	return m_running & m_window.isOpen();
}

void Game::run()
{
	while (isRunning())
	{
		update();

		// required update call to  imgui
		ImGui::SFML::Update(m_window, m_deltaClock.restart());
	}
}

void Game::sUserInput()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(event);

		if (event.type == sf::Event::Closed)
		{
			quit();
		}
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::X)
			{
				std::cout << "Screenshot saved to " << "test.png" << std::endl;
				sf::Texture texture;
				texture.create(m_window.getSize().x, m_window.getSize().y);
				texture.update(m_window);
				if (texture.copyToImage().saveToFile("test.png"))
				{
					std::cout << "Screenshot saved to " << "test.png" << std::endl;
				}
			}
		}

		if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
		{
			// if no action associated with this key, do nothing
			if (getCurrentScene()->getActionMap().find(event.key.code) == getCurrentScene()->getActionMap().end()) continue;

			// get start or end action if it was key press or release
				const std::string actionType = (event.type == sf::Event::KeyPressed) ? "START" : "END";

			// get action and send to scene
			getCurrentScene()->doAction(Action(getCurrentScene()->getActionMap().at(event.key.code), actionType));
		}
	}
}

void Game::changeScene(const std::string& sceneName, std::shared_ptr<T> scene)
{

}

void Game::setPaused(bool paused)
{
	m_paused = true;
}

/* This functions spawns in teh player in the middle of the screen */
void Game::spawnPlayer()
{
	auto entity = m_entities.addEntity("player");

	// Give this entity a Transform so it spawns at middle window with velocity specified by config and angle 0
	// get window size to spawn at middle
	float mx = m_window.getSize().x / 2.0f;
	float my = m_window.getSize().y / 2.0f;
	entity->cTransform = std::make_shared<CTransform>(Vec2f(mx, my), Vec2f(m_playerConfig.S, m_playerConfig.S), 0.0f);

	// the entity shape will have parameters accorind to config scpecs
	entity->cShape = std::make_shared<CShape>(m_playerConfig.SR, m_playerConfig.V, sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB), sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB), m_playerConfig.OT);
	entity->cCollision = std::make_shared<CCollision>(m_playerConfig.CR);
	entity->cLifespan = std::make_shared<CLifespan>(0);

	// add an input component to the player so that we can use inputs, only for player
	entity->cInput = std::make_shared<CInput>();

	// since we want this entity to be our player, set our game's player variable to be this entity
	// this goes slightly against the entitymanager paradigm, but we use the player so much it's worht it
	m_player = entity;
}

/*
* This functions correctly spawns enemy inside game boundary 
*/
void Game::spawnEnemy()
{
	auto entity = m_entities.addEntity("enemy");
	float angle = 0.0f;

	// set up position for enemy
	float ex = m_enemyConfig.SR + (rand() / (float(RAND_MAX) / (m_window.getSize().x - 2*m_enemyConfig.SR))); // random number between radius and maxwindow_size-radius
	float ey = m_enemyConfig.SR + (rand() / (float(RAND_MAX) / (m_window.getSize().y - 2*m_enemyConfig.SR)));

	// set up velocity
	float vx = m_enemyConfig.SMIN + (static_cast <float> (rand()) / static_cast <float> (RAND_MAX/(m_enemyConfig.SMAX - m_enemyConfig.SMIN)));
	float vy = m_enemyConfig.SMIN + (static_cast <float> (rand()) / static_cast <float> (RAND_MAX/(m_enemyConfig.SMAX - m_enemyConfig.SMIN)));
	if (rand() % 2 == 0) // 50% chance to be negative 
	{
		vx = -vx;
		vy = -vy;
	}

	// set vertices
	int enemyVertices = m_enemyConfig.VMIN + (rand() / (RAND_MAX / (m_enemyConfig.VMAX - m_enemyConfig.VMIN)));
	
	// set color
	int enemyRedColor = 1 + rand() % 255;
	int enemyGreenColor = 1 + rand() % 255;
	int enemyBlueColor = 1 + rand() % 255;

	// construct enemy
	entity->cTransform = std::make_shared<CTransform>(Vec2f(ex, ey), Vec2f(vx, vy), angle);
	entity->cShape = std::make_shared<CShape>(m_enemyConfig.SR, enemyVertices, sf::Color(enemyRedColor, enemyGreenColor, enemyBlueColor), sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB), m_enemyConfig.OT);
	entity->cCollision = std::make_shared<CCollision>(m_enemyConfig.CR);
	entity->cLifespan = std::make_shared<CLifespan>(0);

	// set score component
	entity->cScore = std::make_shared<CScore> (enemyVertices * 100);

	// record when the most recent enemy was spawned
	m_lastEnemySpawnTime = m_currentFrame;
}

/* This function spawns in small enemies after we have destroyed the main one*/
void Game::spawnSmallEnemies(std::shared_ptr<Entity> e)
{
	// TODO spawn small enemies at the location of the input enemy e

	// whene we create the smaller enemy we have to read the values of the orignal enemy
	// - spawn a number of small enemies equal to the vertices of the original enemy
	// - set each small enemy to the same color as the original, half the size
	// - samll enemies are worth double points of the original enemy
	for (int i = 0; i < e->cShape->circle.getPointCount(); i++)
	{
		auto enemy = m_entities.addEntity("small enemy");

		// get angle
		float angle = M_PI * (((360 * i) / e->cShape->circle.getPointCount()) + e->cTransform->angle) / 180;

		// get position
		float ex = e->cTransform->pos.x + (e->cShape->circle.getRadius() * cosf(angle));
		float ey = e->cTransform->pos.y + (e->cShape->circle.getRadius() * sinf(angle));

		// get velocity
		float speed = sqrtf(e->cTransform->velocity.x * e->cTransform->velocity.x + e->cTransform->velocity.y * e->cTransform->velocity.y);
		float vx = speed * cosf(angle);
		float vy = speed * sinf(angle);

		enemy->cTransform = std::make_shared<CTransform>(Vec2f(ex, ey), Vec2f(vx, vy), 0.0f);
		enemy->cShape = std::make_shared<CShape>(e->cShape->circle.getRadius() / 2, e->cShape->circle.getPointCount(), e->cShape->circle.getFillColor() , e->cShape->circle.getOutlineColor(), e->cShape->circle.getOutlineThickness());
		enemy->cCollision = std::make_shared<CCollision>(e->cShape->circle.getRadius() / 2);
		enemy->cLifespan = std::make_shared<CLifespan>(m_enemyConfig.L);
		enemy->cScore = std::make_shared<CScore>(e->cShape->circle.getPointCount() * 200);
	}
	


}

/* spawn bullet from player character to where we are clicking */
void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2f& target)
{
	auto bullet = m_entities.addEntity("bullet");

	Vec2f D = entity->cTransform->pos - target;
	Vec2f DNormal = D.normalize();
	float angle = atan2f(D.y, D.x);

	bullet->cTransform = std::make_shared<CTransform>(Vec2f(entity->cTransform->pos.x, entity->cTransform->pos.y), Vec2f(DNormal.x * m_bulletConfig.S, DNormal.y * m_bulletConfig.S), angle);
	bullet->cShape = std::make_shared<CShape>(m_bulletConfig.SR, m_bulletConfig.V , sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB), sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB), m_bulletConfig.OT);
	bullet->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);
	bullet->cLifespan = std::make_shared <CLifespan>(m_bulletConfig.L);
}

/* spawns in special weapon */
void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
	// TODO implement
}

