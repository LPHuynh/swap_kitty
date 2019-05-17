#include <fstream>
#include "json.hpp"

#include "app.h"
#include "world.h"


int main()
{
  std::ifstream inFile("config.json");
  nlohmann::json jsonDatabase;
  inFile >> jsonDatabase;

  App::AppSetting setting;
  setting.windowHeight = 400;
  setting.windowWidth = 600;
  setting.windowTitle = "Swap Kitty";
  setting.lastestRulesetVersion = 1;
  setting.characterName = jsonDatabase["config"]["character"].get<std::string>();
  setting.password = jsonDatabase["config"]["password"].get<std::string>();
  setting.daemonHost = jsonDatabase["config"]["daemonhost"].get<std::string>();
  setting.daemonPort = jsonDatabase["config"]["daemonport"];
  setting.walletPort = jsonDatabase["config"]["walletport"];
  setting.restoreHeight = jsonDatabase["config"]["restoreheight"];
  setting.priority = jsonDatabase["config"]["priority"];
  setting.frameRate = jsonDatabase["config"]["framerate"];

  inFile.close();

  App app;
  if (app.init(setting))
  {
    app.run();
  }

  return 0;
}