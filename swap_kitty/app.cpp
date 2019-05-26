#include "app.h"

#include "json.hpp"


App::App() : mDaemonAPI(DaemonAPI()), mWalletAPI(WalletAPI()), mWorld(World()), mCharacter(Character(mWorld)), mEvent(Event(mWorld, mCharacter, mDaemonAPI)), mCommandProcessor(CommandProcessor(mDaemonAPI, mWalletAPI, mWorld, mCharacter))
{
  std::ifstream inFile("config.json");
  nlohmann::json jsonDatabase;
  inFile >> jsonDatabase;
  inFile.close();

  mSetting.windowHeight = 400;
  mSetting.windowWidth = 600;
  mSetting.windowTitle = "Swap Kitty";
  mSetting.lastestRulesetVersion = 1;
  mSetting.isBetaVersion = false;
  mSetting.characterName = jsonDatabase["config"]["character"]["name"].get<std::string>();
  mSetting.daemonHost = jsonDatabase["config"]["daemon"]["host"].get<std::string>();
  mSetting.daemonPort = jsonDatabase["config"]["daemon"]["port"];
  mSetting.walletPort = jsonDatabase["config"]["wallet"]["port"];
  mSetting.restoreHeight = jsonDatabase["config"]["wallet"]["restoreheight"];
  mSetting.txAmount = jsonDatabase["config"]["wallet"]["txamount"];
  mSetting.txPriority = jsonDatabase["config"]["wallet"]["txpriority"];
  mSetting.mixin = jsonDatabase["config"]["wallet"]["mixin"];
  mSetting.frameRate = jsonDatabase["config"]["game"]["framerate"];

  mWindow.create(sf::VideoMode(mSetting.windowWidth, mSetting.windowHeight), mSetting.windowTitle);
  mWindow.setFramerateLimit(mSetting.frameRate);
  mGui.setTarget(mWindow);

  gameState = GameState::newGameMenu;
  newGameOption = NewGameOption::waiting;

  if (!mDaemonAPI.init(mSetting.daemonHost, mSetting.daemonPort))
  {
    mWorld.logging.addToMainLog("Cannot connect to Daemon.");
    gameState = GameState::exit;
  }
  if (!mWalletAPI.init(mSetting.daemonHost, mSetting.daemonPort, mSetting.walletPort))
  {
    mWorld.logging.addToMainLog("Cannot connect to RPC Wallet.");
    gameState = GameState::exit;
  }

  DaemonAPI::SyncStatus syncStatus = mDaemonAPI.getSyncInfo();
  if (syncStatus.height + 5 < syncStatus.targetHeight)
  {
    mWorld.logging.addToMainLog("Daemon not fully syncd.");
    gameState = GameState::exit;
  }

  inFile.open("blockhash.cache");
  if (inFile.is_open())
  {
    jsonDatabase.clear();
    inFile >> jsonDatabase;

    for (auto& element : jsonDatabase)
    {
      mWorld.blockhashCache[element["height"]] = element["hash"].get<std::string>();
    }
  }

  inFile.close();

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
  jsonDatabase["config"]["wallet"]["txamount"] = mSetting.txAmount;
  jsonDatabase["config"]["wallet"]["txpriority"] = mSetting.txPriority;
  jsonDatabase["config"]["wallet"]["mixin"] = mSetting.mixin;
  jsonDatabase["config"]["game"]["framerate"] = mSetting.frameRate;

  std::ofstream outFile("config.json");
  outFile << std::setw(2) << jsonDatabase << std::endl;
  jsonDatabase.clear();
  outFile.close();
  
  for (auto& element : mWorld.blockhashCache)
  {
    jsonDatabase[std::to_string(element.first)]["height"] = element.first;
    jsonDatabase[std::to_string(element.first)]["hash"] = element.second;
  }

  outFile.open("blockhash.cache");
  outFile << std::setw(2) << jsonDatabase << std::endl;
  outFile.close();

  mWalletAPI.closeWallet();
}

void App::run()
{
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
      if (mClock.getElapsedTime().asSeconds() > 1)
      {
        mClock.restart();
        uint64_t daemonHeight = mDaemonAPI.getBlockCount();
        uint64_t walletHeight = mWalletAPI.getBlockHeight();
        if (daemonHeight > walletHeight + 10)
        {
          mGui.get<tgui::Label>("LabelWalletHeight")->setText("Syncing Wallet: " + std::to_string(walletHeight) + "/" + std::to_string(daemonHeight) + "...");
        }
        else
        {
          mGui.get<tgui::Label>("LabelWalletHeight")->setText("Syncing Wallet: " + std::to_string(daemonHeight) + "/" + std::to_string(daemonHeight) + "...");

          if (mCommandProcessor.scanForCharacterCreationCommand())
          {
            mCommandProcessor.processCommand();
            mEvent.init();
            mGui.get<tgui::Label>("LabelWalletHeight")->setText("Processing Graphics and Events...");

            //Process Commands in own Thread, while displaying and loading graphics
            std::thread t1(&App::mRunTurns, this);
            std::thread t2(&App::mLoadGraphics, this);

            while (!t1.joinable() && !t2.joinable())
            {
              std::this_thread::sleep_for(std::chrono::seconds(1));
              //Display Graphics
            }            

            t1.join();
            t2.join();
            ////

            mGui.removeAllWidgets();
            mLoadMainScreen();
            gameState = GameState::mainGame;
          }
          else
          {
            //TODO show dialog explaining the game with button for character creation
            mCommandProcessor.submitCharacterCreationCommand(mSetting.characterName);
          }
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
      mWorld.logging.addToMainLog("Error Creating wallet.");
      gameState = GameState::exit;
    }
  }
  else if (newGameOption == NewGameOption::loadWallet)
  {
    if (!mWalletAPI.openWallet(characterName, password))
    {
      mWorld.logging.addToMainLog("Error opening wallet.");
      gameState = GameState::exit;
    }
  }
  else if (newGameOption == NewGameOption::restoreWallet)
  {
    if (!mWalletAPI.restoreWallet(characterName, password, seed, "English", mSetting.restoreHeight))
    {
      mWorld.logging.addToMainLog("Error restoring wallet.");
      gameState = GameState::exit;
    }
  }
  mSetting.characterName = characterName;
  gameState = GameState::loading;
  mGui.removeAllWidgets();
  mLoadLoadingScreen();
  mCommandProcessor.init(mSetting.txAmount, mSetting.txPriority, mSetting.mixin, mSetting.restoreHeight, mSetting.isBetaVersion);
  mClock.restart();
}

void App::mRunTurns()
{
  uint64_t topHeight = mDaemonAPI.getBlockCount();
  mCommandProcessor.scanForCommands();
  while (mWorld.currentWorldHeight < topHeight)
  {
    mCommandProcessor.processCommand();
    mEvent.processEvent();
    mWorld.currentWorldHeight++;
  }
}

void App::mLoadGraphics()
{
  //TODO
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
  mGui.get<tgui::EditBox>("EditBoxCharacterName")->setText(mSetting.characterName);
}

void App::mLoadLoadingScreen()
{
  mGui.loadWidgetsFromFile("gui/loadingscreen.gui");
}

void App::mLoadMainScreen()
{
}