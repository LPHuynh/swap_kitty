#pragma once

#include <TGUI/TGUI.hpp>
#include "character.h"
#include "commandprocessor.h"
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
    bool isBetaVersion;
    std::string signingKey;
    std::string characterName;
    std::string daemonHost;
    uint16_t daemonPort;
    uint16_t walletPort;
    uint64_t restoreHeight;
    uint64_t txAmount;
    uint16_t txPriority;
    uint16_t mixin;
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
  CommandProcessor mCommandProcessor;
  Character mCharacter;
  World mWorld;

  AppSetting mSetting;
  sf::RenderWindow mWindow;
  tgui::Gui mGui;
  sf::Clock mClock;

  void mRunTurns();
  void mLoadGraphics();
  void mDisplayNewGameSubWindow();
  void mLoadTitleScreen();
  void mLoadLoadingScreen();
  void mLoadMainScreen();
};

