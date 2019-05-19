#include "app.h"

#include "json.hpp"


App::App()
{
  std::ifstream inFile("config.json");
  nlohmann::json jsonDatabase;
  inFile >> jsonDatabase;

  mSetting.windowHeight = 400;
  mSetting.windowWidth = 600;
  mSetting.windowTitle = "Swap Kitty";
  mSetting.lastestRulesetVersion = 1;
  mSetting.characterName = jsonDatabase["config"]["character"]["name"].get<std::string>();
  mSetting.daemonHost = jsonDatabase["config"]["daemon"]["host"].get<std::string>();
  mSetting.daemonPort = jsonDatabase["config"]["daemon"]["port"];
  mSetting.walletPort = jsonDatabase["config"]["wallet"]["port"];
  mSetting.restoreHeight = jsonDatabase["config"]["wallet"]["restoreheight"];
  mSetting.txPriority = jsonDatabase["config"]["wallet"]["txpriority"];
  mSetting.frameRate = jsonDatabase["config"]["game"]["framerate"];

  inFile.close();

  mWindow.create(sf::VideoMode(mSetting.windowWidth, mSetting.windowHeight), mSetting.windowTitle);
  mWindow.setFramerateLimit(mSetting.frameRate);
  mGui.setTarget(mWindow);

  gameState = GameState::newGameMenu;
  newGameOption = NewGameOption::waiting;

  if (!mDaemonAPI.init(mSetting.daemonHost, mSetting.daemonPort))
  {
    std::cout << "Cannot connect to Daemon.\n";
    gameState = GameState::exit;
  }
  if (!mWalletAPI.init(mSetting.daemonHost, mSetting.daemonPort, mSetting.walletPort))
  {
    std::cout << "Cannot connect to RPC Wallet.\n";
    gameState = GameState::exit;
  }

  DaemonAPI::SyncStatus syncStatus = mDaemonAPI.getSyncInfo();
  if (syncStatus.height + 5 < syncStatus.targetHeight)
  {
    std::cout << "Daemon not fully syncd.\n";
    gameState = GameState::exit;
  }

  mLoadTitleScreen();
}

App::~App()
{
  nlohmann::json jsonDatabase;

  jsonDatabase["config"]["character"]["name"] = mSetting.characterName;
  jsonDatabase["config"]["daemon"]["host"] = mSetting.daemonHost;
  jsonDatabase["config"]["daemon"]["port"] = mSetting.daemonPort;
  jsonDatabase["config"]["wallet"]["port"] = mSetting.walletPort;
  jsonDatabase["config"]["wallet"]["restoreheight"] = mSetting.restoreHeight;
  jsonDatabase["config"]["wallet"]["txpriority"] = mSetting.txPriority;
  jsonDatabase["config"]["game"]["framerate"] = mSetting.frameRate;

  std::ofstream outFile("config.json");
  outFile << std::setw(2) << jsonDatabase << std::endl;
  outFile.close();

  mWalletAPI.closeWallet();
}

void App::run()
{
  sf::Clock clock;

  while (mWindow.isOpen())
  {
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
      if (event.type == sf::Event::Closed || gameState == GameState::exit)
      {
        mWindow.close();
      }

      mGui.handleEvent(event);
    }

    if (gameState == GameState::mainGame)
    {
      //TODO: Process command, daily event, hourly event, tenthhourly event; minutely event (fluff)
    }
    else if (gameState == GameState::newGameMenu)
    {
      mDisplayNewGameSubWindow();
    }
    else if (gameState == GameState::loading)
    {
      //TODO: loading screen

      if (clock.getElapsedTime().asSeconds() > 5)
      {
        clock.restart();
        uint64_t daemonHeight = mDaemonAPI.getBlockCount();
        uint64_t walletHeight = mWalletAPI.getBlockHeight();

        if (daemonHeight > walletHeight)
        {
          std::cout << "Waiting for wallet to fully sync. Current Daemon Height: " << daemonHeight << ". Current Wallet Height: " << walletHeight;
        }
        else
        {
          //TODO: Scan for command; if no command detected create a new character
          //Else quick-process events in separate thread
          //gameState = GameState::mainGame;
        }
      }
    }

    mWindow.clear();
    mGui.draw();
    mWindow.display();
  }
}

void App::startGame()
{
  std::string characterName = mGui.get<tgui::EditBox>("EditBoxCharacterName")->getText();
  std::string password = mGui.get<tgui::EditBox>("EditBoxPassword")->getText();
  std::string seed = mGui.get<tgui::EditBox>("EditBoxSeed")->getText();

  if (newGameOption == NewGameOption::newWallet)
  {
    if (!mWalletAPI.createWallet(characterName, password, "English"))
    {
      std::cout << "Error Creating wallet.\n";
      std::this_thread::sleep_for(std::chrono::seconds(5));
      gameState = GameState::exit;
    }
  }
  else if (newGameOption == NewGameOption::loadWallet)
  {
    if (!mWalletAPI.openWallet(characterName, password))
    {
      std::cout << "Error opening wallet.\n";
      std::this_thread::sleep_for(std::chrono::seconds(5));
      gameState = GameState::exit;
    }
  }
  else if (newGameOption == NewGameOption::restoreWallet)
  {
    if (!mWalletAPI.restoreWallet(characterName, password, seed, "English", mSetting.restoreHeight))
    {
      std::cout << "Error restoring wallet.\n";
      std::this_thread::sleep_for(std::chrono::seconds(5));
      gameState = GameState::exit;
    }
  }
  mSetting.characterName = characterName;
  gameState = GameState::loading;
  std::this_thread::sleep_for(std::chrono::seconds(5));
  mGui.removeAllWidgets();
}

void App::mDisplayNewGameSubWindow()
{
  if (newGameOption == NewGameOption::newWallet)
  {
    mGui.get<tgui::ChildWindow>("ChildWindowNewWallet")->setVisible(true);
    mGui.get<tgui::ChildWindow>("ChildWindowNewWallet")->setTitle("New Game Wallet");
    mGui.get<tgui::Label>("LabelSeed")->setVisible(false);
    mGui.get<tgui::EditBox>("EditBoxSeed")->setVisible(false);
  }
  else if (newGameOption == NewGameOption::loadWallet)
  {
    mGui.get<tgui::ChildWindow>("ChildWindowNewWallet")->setVisible(true);
    mGui.get<tgui::ChildWindow>("ChildWindowNewWallet")->setTitle("Load Game Wallet");
    mGui.get<tgui::Label>("LabelSeed")->setVisible(false);
    mGui.get<tgui::EditBox>("EditBoxSeed")->setVisible(false);
  }
  else if (newGameOption == NewGameOption::restoreWallet)
  {
    mGui.get<tgui::ChildWindow>("ChildWindowNewWallet")->setVisible(true);
    mGui.get<tgui::ChildWindow>("ChildWindowNewWallet")->setTitle("Restore Game Wallet");
    mGui.get<tgui::Label>("LabelSeed")->setVisible(true);
    mGui.get<tgui::EditBox>("EditBoxSeed")->setVisible(true);
  }
  else
  {
    mGui.get<tgui::ChildWindow>("ChildWindowNewWallet")->setVisible(false);
  }
}

void App::mLoadTitleScreen()
{
  mGui.loadWidgetsFromFile("gui/titlescreen.gui");
  mGui.get<tgui::Button>("ButtonNewGame")->connect("pressed", [&]() { newGameOption = NewGameOption::newWallet; });
  mGui.get<tgui::Button>("ButtonLoadGame")->connect("pressed", [&]() { newGameOption = NewGameOption::loadWallet; });
  mGui.get<tgui::Button>("ButtonRestoreGame")->connect("pressed", [&]() { newGameOption = NewGameOption::restoreWallet; });
  mGui.get<tgui::Button>("ButtonExitGame")->connect("pressed", [&]() { mWindow.close(); });
  mGui.get<tgui::Button>("ButtonStartWallet")->connect("pressed", &App::startGame, this);
  mGui.get<tgui::Button>("ButtonCancelWallet")->connect("pressed", [&]() { newGameOption = NewGameOption::waiting; });
  mGui.get<tgui::EditBox>("EditBoxCharacterName")->setDefaultText(mSetting.characterName);
}