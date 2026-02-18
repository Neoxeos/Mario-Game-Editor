#include <iostream>
#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>

#include "Game.h"

int main()
{
    //// Create window
    //const int wWidth = 840;
    //const int wHeight = 700;
    //sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "First Project");

    ////ImGui ininitialization
    //ImGui::SFML::Init(window);

    //// make a shape to draw to screen
    ////sf::CircleShape circle(50);
    ////circle.setFillColor(sf::Color::Green);
    ////circle.setPosition(300.0f, 300.0f);
    //float moveSpeed = 0.01f;

    ////std::shared_ptr<sf::Shape> shape = std::make_shared<sf::CircleShape>(20);
    //std::vector<std::shared_ptr<sf::Shape>> shapes;
    //int numShapes = 1;

    //for (int x = 0; x <numShapes; x++)
    //{
    //    for (int y = 0; y < numShapes; y++)
    //    {
    //        std::shared_ptr<sf::Shape> shape = std::make_shared<sf::CircleShape>(10);
    //        shape->setFillColor(sf::Color(x*10, y*10, 100));
    //        shape->setPosition(x*20, y*20);
    //        shapes.push_back(shape);
    //    }
    //}

    //sf::Font myFont;

    //if (!myFont.loadFromFile("Fonts/arial.ttf"))
    //{
    //    std::cerr << "Could not load font!\n";
    //    exit(-1);
    //}
    //sf::Text text("Sample text", myFont, 24);
    //text.setPosition(0, wHeight - (float)text.getCharacterSize());

    //sf::Clock deltaClock;
    //while (window.isOpen())
    //{
    //    sf::Event event;
    //    while (window.pollEvent(event))
    //    {
    //        ImGui::SFML::ProcessEvent(event);
    //        if (event.type == sf::Event::Closed)
    //            window.close();
    //    }

    //    //circle.setPosition(circle.getPosition().x + circleMoveSpeed, circle.getPosition().y + circleMoveSpeed);
    //    //window.clear();
    //    for (auto& shape : shapes) {
    //        window.draw(*shape);
    //        shape->setPosition(shape->getPosition().x + moveSpeed, shape->getPosition().y + moveSpeed);
    //        std::cout << shape->getLocalBounds().height << " " << shape->getLocalBounds().left << " " << shape->getLocalBounds().top << "\n";
    //    }

    //    ImGui::SFML::Update(window, deltaClock.restart());
    //    ImGui::Begin("Window");
    //    ImGui::End();

    //    window.clear();
    //    //window.draw(circle);
    //    window.draw(text);
    //    ImGui::SFML::Render(window);
    //    window.display();
    //}

    //ImGui::SFML::Shutdown();
    //return 0;

    // Here we start our project
    Game g("config.txt");
    g.run();
}