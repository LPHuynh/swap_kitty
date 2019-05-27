#include "app.h"

#include "json.hpp"


App::App() : mDaemonAPI(DaemonAPI()), mWalletAPI(WalletAPI()), mWorld(World()), mCharacter(Character(mWorld)), mEvent(Event(mWorld, mCharacter, mDaemonAPI)), mCommandProcessor(CommandProcessor(mDaemonAPI, mWalletAPI, mWorld, mCharacter))
{
  std::ifstream inFile("config.json");
  nlohmann::json jsonDatabase;
  inFile >> jsonDatabase;
  inFile.close();

  mSetting.windowWidth = 800;
  mSetting.windowHeight = 600;
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

  mSwapBalance = { 0,0 };  
  mWorld.lastestRulesetVersion = mSetting.lastestRulesetVersion;
  gameState = GameState::titleScreen;
  newGameOption = NewGameOption::waiting;
  isGUILoaded = false;
  mIsCharacterCreated = false;
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
  mClock.restart();

  while (mWindow.isOpen())
  {
    while (mWindow.pollEvent(mWindowEvent))
    {
      if (mWindowEvent.type == sf::Event::Closed || gameState == GameState::exit)
      {
        mWindow.close();
      }

      mGui.handleEvent(mWindowEvent);
    }

    switch (gameState)
    {
    case GameState::mainGame: runMainGameState(); break;
    case GameState::loading: runLoadingState(); break;
    case GameState::titleScreen: runTitleState(); break;
    }

    mWindow.clear();
    mGui.draw();
    mWindow.display();
  }
}

void App::runMainGameState()
{
  if (mClock.getElapsedTime().asSeconds() > 1)
  {
    setWindowTitle();
    mClock.restart();
  }

  loadMainScreenGUI();
  runTurns();
}

void App::loadMainScreenGUI()
{
}

void App::runLoadingState()
{
  loadLoadingScreenGUI();
  if (mClock.getElapsedTime().asSeconds() > 1)
  {
    mClock.restart();
    uint64_t daemonHeight = mDaemonAPI.getBlockCount();
    uint64_t walletHeight = mWalletAPI.getBlockHeight();
    setWindowTitle();

    //Scan for Character; Failing that, wait for wallet to fully sync and try again
    if (mCommandProcessor.scanForCharacterCreationCommand())
    {
      mIsCharacterCreated = true;
      mCommandProcessor.processCommand();
      mEvent.init();
      mGui.removeAllWidgets();
      gameState = GameState::mainGame;
    }
    else
    {
      if (daemonHeight > walletHeight + 10)
      {
        mGui.get<tgui::Label>("LabelWalletHeight")->setText("Syncing Wallet: " + std::to_string(walletHeight) + "/" + std::to_string(daemonHeight) + "...");
        mGui.get<tgui::ProgressBar>("ProgressBarWallet")->setValue(uint16_t((walletHeight - mSetting.restoreHeight) / (daemonHeight - mSetting.restoreHeight)));
      }
      else
      {
        mGui.get<tgui::Label>("LabelWalletHeight")->setText("Syncing Wallet: " + std::to_string(daemonHeight) + "/" + std::to_string(daemonHeight) + "...");
        mGui.get<tgui::ProgressBar>("ProgressBarWallet")->setValue(100);

        //Scan for character after fully syncing
        if (mCommandProcessor.scanForCharacterCreationCommand())
        {
          mIsCharacterCreated = true;
          mCommandProcessor.processCommand();
          mEvent.init();
          mGui.removeAllWidgets();
          gameState = GameState::mainGame;
        }
        else
        {
          mGui.get<tgui::ChildWindow>("ChildWindowNewChara")->setVisible(true);
          std::string walletAddress = mWalletAPI.getAddress();
          std::string mnemonicSeed = mWalletAPI.getMnemonicSeed();
          std::string middleTextBox = "Your Game Wallet Address:\n" + walletAddress + "\n\nMnemonic Seed:\n" + mnemonicSeed;
          mGui.get<tgui::TextBox>("TextBoxNewCharaMiddle")->setText(middleTextBox);

          //Mini loop
          while (mWindow.isOpen() && gameState == GameState::loading)
          {
            while (mWindow.pollEvent(mWindowEvent))
            {
              if (mWindowEvent.type == sf::Event::Closed || gameState == GameState::exit)
              {
                mWindow.close();
              }

              mGui.handleEvent(mWindowEvent);
            }

            if (mClock.getElapsedTime().asSeconds() > 1)
            {
              mClock.restart();
              setWindowTitle();

              if (mIsCharacterCreated)
              {
                mGui.get<tgui::Button>("ButtonNewCharaCreate")->setEnabled(false);
                mGui.get<tgui::Label>("LabelBottom")->setText("New Being generated. This process may take a few minute...");
                if (mCommandProcessor.scanForCharacterCreationCommand())
                {
                  mCommandProcessor.processCommand();
                  mEvent.init();
                  mGui.removeAllWidgets();
                  gameState = GameState::mainGame;
                }
              }
              else if (mSwapBalance.unlockedBalance > 100000000)
              {
                mGui.get<tgui::Button>("ButtonNewCharaCreate")->setEnabled(true);
              }
            }

            mWindow.clear();
            mGui.draw();
            mWindow.display();
          }
        }
      }
    }
  }
}

void App::loadLoadingScreenGUI()
{
  if (!isGUILoaded)
  {
    mGui.loadWidgetsFromFile("gui/loadingscreen.gui");
    mGui.get<tgui::Button>("ButtonNewCharaCreate")->connect("pressed", &App::createCharacter, this);
    isGUILoaded = true;
  }
}

void App::createCharacter()
{
  mCommandProcessor.submitCharacterCreationCommand(mSetting.characterName);
  mIsCharacterCreated = true;
}

void App::runTitleState()
{
  loadTitleScreenGUI();
  loadNewGameSubWindow();
}

void App::loadTitleScreenGUI()
{
  if (!isGUILoaded)
  {
    mGui.loadWidgetsFromFile("gui/titlescreen.gui");
    mGui.get<tgui::Button>("ButtonNewGame")->connect("pressed", [&]() { newGameOption = NewGameOption::newWallet; });
    mGui.get<tgui::Button>("ButtonLoadGame")->connect("pressed", [&]() { newGameOption = NewGameOption::loadWallet; });
    mGui.get<tgui::Button>("ButtonRestoreGame")->connect("pressed", [&]() { newGameOption = NewGameOption::restoreWallet; });
    mGui.get<tgui::Button>("ButtonViewGame")->connect("pressed", [&]() { newGameOption = NewGameOption::viewWallet; });
    mGui.get<tgui::Button>("ButtonExitGame")->connect("pressed", [&]() { gameState = GameState::exit; });
    mGui.get<tgui::Button>("ButtonStartWallet")->connect("pressed", &App::startGame, this);
    mGui.get<tgui::Button>("ButtonCancelWallet")->connect("pressed", [&]() { newGameOption = NewGameOption::waiting; });
    mGui.get<tgui::EditBox>("EditBoxCharacterName")->setText(mSetting.characterName);
    mGui.get<tgui::Button>("ButtonViewGame")->setEnabled("false"); //TODO
    isGUILoaded = true;
  }
}

void App::loadNewGameSubWindow()
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
  else if (newGameOption == NewGameOption::viewWallet)
  {
    //TODO
  }
  else
  {
    mGui.get<tgui::ChildWindow>("ChildWindowNewWallet")->setVisible(false);
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
  else if (newGameOption == NewGameOption::viewWallet)
  {
  }
  mSetting.characterName = characterName;
  gameState = GameState::loading;
  mGui.removeAllWidgets();
  isGUILoaded = false;
  mCommandProcessor.init(mSetting.txAmount, mSetting.txPriority, mSetting.mixin, mSetting.restoreHeight, mSetting.isBetaVersion);
  mClock.restart();
}

void App::runTurns()
{
  uint64_t topHeight = mDaemonAPI.getBlockCount();
  if (!mCommandProcessor.scanForCommands())
  {
    //Exit game when scanned commands are critically invalid
    gameState = GameState::exit;
  }

  while (mWorld.currentWorldHeight < topHeight && mClock.getElapsedTime().asSeconds() < 1)
  {
    mCommandProcessor.processCommand();
    mEvent.processEvent();
    mWorld.currentWorldHeight++;
  }
}

void App::loadGraphics()
{
  //TODO
}

void App::setWindowTitle()
{
  mSwapBalance = mWalletAPI.getBalance();
  std::string statusText;


  statusText += "          " + mEvent.time.dateString + ", " + mEvent.time.timeString;
  statusText += "          (Balance: " + std::to_string(mSwapBalance.unlockedBalance / pow(10, 12)) + " / " + std::to_string(mSwapBalance.totalBalance / pow(10, 12)) + "XWP )";
  mWindow.setTitle(mSetting.windowTitle + statusText);
}
