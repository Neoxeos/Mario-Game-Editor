#include <iostream>
#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>

#include "Game.h"

int main()
{
    Game g("assets.txt");
    g.run();
}