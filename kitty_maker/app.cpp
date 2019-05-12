#include <SFML/Graphics.hpp>
#include "app.h"


App::App()
{
}

App::~App()
{
}

void App::run()
{
  sf::RenderWindow window(sf::VideoMode(600, 400), "Kitty Maker");
  window.setFramerateLimit(15);

  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    window.clear();
    window.display();
  }
}