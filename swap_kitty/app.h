#pragma once

#include <TGUI/TGUI.hpp>
#include "character.h"
#include "commandprocessor.h"
#include "daemonapi.h"
#include "event.h"
#include "walletapi.h"
#include "world.h"


class App
{
public:
  enum class GameState { titleScreen, loading, mainGame, exit };
  enum class NewGameOption { newWallet, loadWallet, restoreWallet, viewWallet, waiting };

  struct AppSetting
  {
    uint16_t windowWidth;
    uint16_t windowHeight;
    std::string windowTitle;
    uint16_t lastestRulesetVersion;
    bool isBetaVersion;
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

  GameState gameState;
  NewGameOption newGameOption;

private:
  DaemonAPI mDaemonAPI;
  WalletAPI mWalletAPI;
  CommandProcessor mCommandProcessor;
  Character mCharacter;
  Event mEvent;
  World mWorld;

  void runMainGameState();

  void runLoadingState();
  void createCharacter();

  void runTitleState();
  void loadNewGameSubWindow();
  void startGame();

  void loadGUI();
  void runTurns();
  void loadGraphics();
  void setWindowTitle();

  void toggleFluffText();

  void openFeedMenu();
  void openEquipMenu();
  void equip();
  void feed();
  void addFoodToFeed();
  void addPotionToFeed();

  AppSetting mSetting;
  sf::RenderWindow mWindow;
  sf::Event mWindowEvent;
  tgui::Gui mGui;
  sf::Clock mClockMain;
  sf::Clock mClockStatus;

  WalletAPI::Balance mSwapBalance;
  bool mIsCharacterCreated;
  bool mIsGUILoaded;
  bool mIsFullySync;
  uint64_t mLastKnownDaemonHeight;

  uint16_t mFeedFoodID[7];
  uint16_t mFeedPotionID[6];
};

