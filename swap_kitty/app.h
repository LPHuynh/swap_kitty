#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include "world.h"
#include "character.h"


class App
{
public:
  struct AppSetting
  {
    uint16_t windowWidth;
    uint16_t windowHeight;
    std::string windowTitle;
    uint16_t lastestRulesetVersion;
    std::string characterName;
    std::string password;
    std::string daemonHost;
    uint16_t daemonPort;
    uint16_t walletPort;
    uint64_t restoreHeight;
    uint16_t priority;
    uint16_t frameRate;
  };

  App();
  ~App();
  bool init(AppSetting setting);

  void run();

private:
  AppSetting mSetting;
  World mWorld;
  Character mCharacter;
};

