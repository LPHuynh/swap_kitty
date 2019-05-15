#include "app.h"


App::App()
{
}

App::~App()
{
}

bool App::init(AppSetting setting)
{
  mSetting = setting;
  if (!mWorld.init(setting.daemonHost, setting.daemonPort, setting.walletPort))
  {
    return false;
  }
  return true;
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