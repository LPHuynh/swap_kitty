#pragma once

#include <TGUI/TGUI.hpp>
#include "character.h"
#include "daemonapi.h"
#include "walletapi.h"
#include "world.h"


class App
{
public:
  enum class GameState { newGameMenu, loading, mainGame, exit };
  enum class NewGameOption { newWallet, loadWallet, restoreWallet, waiting };

  struct AppSetting
  {
    uint16_t windowWidth;
    uint16_t windowHeight;
    std::string windowTitle;
    uint16_t lastestRulesetVersion;
    std::string characterName;
    std::string daemonHost;
    uint16_t daemonPort;
    uint16_t walletPort;
    uint64_t restoreHeight;
    uint16_t txPriority;
    uint16_t frameRate;
  };

  App();
  ~App();

  void run();
  void startGame();

  GameState gameState;
  NewGameOption newGameOption;

private:
  DaemonAPI mDaemonAPI;
  WalletAPI mWalletAPI;
  AppSetting mSetting;
  sf::RenderWindow mWindow;
  tgui::Gui mGui;
  World mWorld;
  Character mCharacter;

  void mDisplayNewGameSubWindow();
  void mLoadTitleScreen();
};

