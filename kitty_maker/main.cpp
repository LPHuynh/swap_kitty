#include "app.h"


int main()
{
  AppSetting setting;
  setting.windowHeight = 400;
  setting.windowWidth = 600;
  setting.windowTitle = "Kitty Maker";
  setting.frameRate = 15;
  setting.rulesetVersion = 1;

  App app(setting);
  app.run();

  return 0;
}