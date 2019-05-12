#pragma once

#include <string>

struct AppSetting
{
  uint16_t windowWidth;
  uint16_t windowHeight;
  std::string windowTitle;
  uint16_t frameRate;
  uint16_t rulesetVersion;
};

class App
{
public:
  App(AppSetting);
  ~App();

  void run();

private:
  AppSetting mSetting;
};

