#include <fstream>
#include "json.hpp"

#include "app.h"


int main()
{
  std::ifstream inFile("config.json");
  nlohmann::json jsonDatabase;
  inFile >> jsonDatabase;

  App::AppSetting setting;
  setting.windowHeight = 400;
  setting.windowWidth = 600;
  setting.windowTitle = "Swap Kitty";
  setting.frameRate = jsonDatabase["config"]["framerate"];
  setting.rulesetVersion = 1;
  setting.characterName = jsonDatabase["config"]["character"].get<std::string>();
  setting.daemonHost = jsonDatabase["config"]["daemonhost"].get<std::string>();
  setting.daemonPort = jsonDatabase["config"]["daemonport"];
  setting.walletPort = jsonDatabase["config"]["walletport"];

  inFile.close();

  App app(setting);
  app.run();

  return 0;
}