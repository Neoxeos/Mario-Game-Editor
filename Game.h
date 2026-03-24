#pragma once

#include "Entity.h"
#include "EntityManager.h"
#include "Scene.h"

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

struct PlayerConfig { int SR, CR, V, FR, FG, FB, OR, OG, OB, OT; float S; };
struct EnemyConfig { int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SP; float SMIN, SMAX; };
struct BulletConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L; float S; };

class Game
{
	std::map<std::string, std::shared_ptr<Scene>> m_scenes; // map of all scenes
	std::string m_scene;
	Assets m_assets = Assets::getInstance();
	sf::RenderWindow m_window; // the window we will draw to
	EntityManager m_entities; // vector of all entities
	size_t m_simulationSpeed = 1; // how many times to update the game logic per frame
	sf::Clock m_deltaClock;
	bool m_running = true;
	bool m_paused = false;



public:
	Game(const std::string& config);
	void init(const std::string& path);
	void update();
	void run();
	void quit();
	template<typename T> void changeScene(const std::string& sceneName, std::shared_ptr<T> scene, bool endCurrentScene = false);
	std::shared_ptr<Scene> getCurrentScene();
	Assets& getAssets() { return m_assets;}
	sf::RenderWindow& getWindow() { return m_window;}
	void sUserInput();
	bool isRunning() const;
};