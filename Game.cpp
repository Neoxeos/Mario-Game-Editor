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

	m_scenes["menu"] = std::make_shared<Scene_Menu>(this);
	m_scenes["play"] = std::make_shared<Scene_Play>(this, "level1");

	changeScene<Scene_Play>("play", std::make_shared<Scene_Play>(this, "level1"));
	changeScene<Scene_Menu>("menu", std::make_shared<Scene_Menu>(this));
	ImGui::SFML::Init(m_window);
}


bool Game::isRunning() const
{
	return m_running && m_window.isOpen();
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

template<typename T>
void Game::changeScene(const std::string& sceneName, std::shared_ptr<T> scene, bool endCurrentScene)
{
	if (scene)
	{
		m_scenes[sceneName] = scene;
	}
	else
	{
		if (m_scenes.find(sceneName) == m_scenes.end())
		{
			std::cerr << "Scene " << sceneName << " not found!" << std::endl;
			return;
		}
	}

	if (endCurrentScene)
	{
		m_scenes.erase(m_scenes.find(m_currentScene));
	}
}

void Game::update()
{
	sUserInput();

	for (size_t i = 0; i < m_simulationSpeed; i++)
	{
		getCurrentScene()->update();
		m_window.clear();
		getCurrentScene()->sRender();
		ImGui::SFML::Render(m_window);
		m_window.display();
	}
}

void Game::quit()
{
	m_running = false;
	ImGui::SFML::Shutdown();
	m_window.close();
}


