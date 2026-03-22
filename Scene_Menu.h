#pragma once


#include "Scene.h"



class Scene_Menu : public Scene
{
public:
	Scene_Menu() = default;
	Scene_Menu(Game& game)
		: Scene(game)
	{
		init();
	}

	void init() {}
};