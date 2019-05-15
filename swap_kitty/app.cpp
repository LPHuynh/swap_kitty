#include "app.h"

#include <SFML/Graphics.hpp>


App::App(AppSetting setting)
{
  mSetting = setting;
}

App::~App()
{
}

void App::run()
{
  sf::RenderWindow window(sf::VideoMode(mSetting.windowWidth , mSetting.windowHeight), mSetting.windowTitle);
  window.setFramerateLimit(mSetting.frameRate);

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