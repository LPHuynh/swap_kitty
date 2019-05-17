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

  DaemonAPI::SyncStatus syncStatus = mWorld.daemonAPI.getSyncInfo();
  if (syncStatus.height + 5 < syncStatus.targetHeight)
  {
    printf("Daemon not fully syncd.");
    return false;
  }

  //TODO: Add option to Start New Character, Load Character, Restore Character
  Character::NewGameOption newGameOption = Character::NewGameOption::newWallet;
  std::string seed = "";

  if (!mCharacter.init(setting.characterName, setting.password, newGameOption, seed, setting.restoreHeight, setting.lastestRulesetVersion, mWorld))
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