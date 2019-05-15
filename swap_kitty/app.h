#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include "world.h"


class App
{
public:
  struct AppSetting
  {
    uint16_t windowWidth;
    uint16_t windowHeight;
    std::string windowTitle;
    uint16_t frameRate;
    uint16_t rulesetVersion;
    std::string characterName;
    std::string daemonHost;
    uint16_t daemonPort;
    uint16_t walletPort;
  };

  App();
  ~App();
  bool init(AppSetting setting);

  void run();

private:
  AppSetting mSetting;
  World mWorld;
};

