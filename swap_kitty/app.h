#pragma once

#include <string>


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

  App(AppSetting);
  ~App();

  void run();

private:
  AppSetting mSetting;
};

