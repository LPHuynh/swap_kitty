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
  sf::Font defaultFont;
  defaultFont.loadFromFile("Lato-Regular.ttf");
  mGui.setFont(defaultFont);

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
  mIsGUILoaded = false;
  mIsCharacterCreated = false;
  mIsFullySync = false;
  mLastKnownDaemonHeight = mDaemonAPI.getBlockCount();
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

  mWalletAPI.stopWallet();
}

void App::run()
{
  mClockMain.restart();
  mClockStatus.restart();

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
  if (mClockMain.getElapsedTime().asSeconds() > 1)
  {
    mClockMain.restart();
    setWindowTitle();
    runTurns();

    if (mClockStatus.getElapsedTime().asSeconds() > 5)
    {
      mClockStatus.restart();
      mCharacter.updateStatBarText();
      mCharacter.updateStatusBarText();
      mCharacter.updateScheduleBoxText(mEvent.time.hour);
      mCharacter.updateHouseBoxText();
      mCharacter.updateCharacterSheetText();
      mGui.get<tgui::Label>("LabelStat")->setText("[HP:" + std::to_string(mCharacter.profile.health / 100) + "/" + std::to_string(mCharacter.profile.maxHealth / 100) + "]\t" + mCharacter.statBarText);
      mGui.get<tgui::Label>("LabelStatus")->setText("[MP:" + std::to_string(mCharacter.profile.mana / 100) + "/" + std::to_string(mCharacter.profile.maxMana / 100) + "]\t" + mWorld.logging.getStatusMessage() + mCharacter.statusBarText);
      mGui.get<tgui::Label>("LabelSchduleBox")->setText(mCharacter.scheduleBoxText);
      mGui.get<tgui::Label>("LabelHouseBox")->setText(mCharacter.houseBoxText);
      mGui.get<tgui::Label>("LabelCharacterSheet")->setText(mCharacter.characterSheetText);

      if (mIsFullySync)
      {
        mGui.get<tgui::Label>("LabelStatus")->setText("[MP:" + std::to_string(mCharacter.profile.mana / 100) + "/" + std::to_string(mCharacter.profile.maxMana / 100) + "]\t" + mWorld.logging.getStatusMessage() + mCharacter.statusBarText);
        mWorld.logging.popStatus();
      }
      else
      {
        mGui.get<tgui::Label>("LabelStatus")->setText("[MP:" + std::to_string(mCharacter.profile.mana / 100) + "/" + std::to_string(mCharacter.profile.maxMana / 100) + "]\t" +
        "Syncing Wallet..." + std::to_string(mWorld.currentWorldHeight) + "/" + std::to_string(mLastKnownDaemonHeight));
      }
    }

    if (mWorld.logging.isLogTextUpdated)
    {
      std::string text;

      for (auto& element : mWorld.logging.mainLog)
      {
        text += element + "\n";
      }
      std::stringstream progress;
      progress << std::setfill(' ') << std::setw(10) << std::left << mWorld.logging.progress;
      mGui.get<tgui::TextBox>("TextBoxLog")->setText(text + "\t[" + progress.str() + "]");
      mWorld.logging.isLogTextUpdated = false;
    }
  }

  loadGUI();
}

void App::runLoadingState()
{
  loadGUI();
  if (mClockMain.getElapsedTime().asSeconds() > 1)
  {
    mClockMain.restart();
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
      mIsGUILoaded = false;
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
          mIsGUILoaded = false;
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

            if (mClockMain.getElapsedTime().asSeconds() > 1)
            {
              mClockMain.restart();
              setWindowTitle();

              if (mIsCharacterCreated)
              {
                mGui.get<tgui::Button>("ButtonNewCharaCreate")->setEnabled(false);
                mGui.get<tgui::Label>("LabelBottom")->setText("New Character being generated. This process may take a few minute...");
                mGui.get<tgui::Button>("ButtonNewCharaCreate")->setText("Generating New Character...");
                if (mCommandProcessor.scanForCharacterCreationCommand())
                {
                  mCommandProcessor.processCommand();
                  mEvent.init();
                  mGui.removeAllWidgets();
                  mIsGUILoaded = false;
                  gameState = GameState::mainGame;
                }
              }
              else if (mSwapBalance.unlockedBalance > 100000000)
              {
                mGui.get<tgui::Button>("ButtonNewCharaCreate")->setEnabled(true);
                mGui.get<tgui::Button>("ButtonNewCharaCreate")->setText("Create New Character");
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

void App::createCharacter()
{
  mCommandProcessor.submitCharacterCreationCommand(mSetting.characterName, 0);
  mIsCharacterCreated = true;
}

void App::runTitleState()
{
  loadGUI();
  loadNewGameSubWindow();
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
  mIsGUILoaded = false;
  mCommandProcessor.init(mSetting.txAmount, mSetting.txPriority, mSetting.mixin, mSetting.restoreHeight, mSetting.isBetaVersion);
  mClockMain.restart();
}

void App::loadGUI()
{
  if (!mIsGUILoaded)
  {
    if (gameState == GameState::mainGame)
    {
      mGui.loadWidgetsFromFile("gui/mainscreen.gui");

      sf::Font monoFont;
      monoFont.loadFromFile("RobotoMono-Regular.ttf");
      mGui.get<tgui::Label>("LabelStat")->setInheritedFont(monoFont);
      mGui.get<tgui::Label>("LabelStatus")->setInheritedFont(monoFont);
      mGui.get<tgui::Label>("LabelHouseBox")->setInheritedFont(monoFont);
      mGui.get<tgui::Label>("LabelSchduleBox")->setInheritedFont(monoFont);
      mGui.get<tgui::Label>("LabelCharacterSheet")->setInheritedFont(monoFont);
      mGui.get<tgui::TextBox>("TextBoxLog")->setInheritedFont(monoFont);
      mGui.get<tgui::ChildWindow>("ChildWindowInteract")->setTitle(mCharacter.profile.name);

      //Base Window
      mGui.get<tgui::Button>("ButtonInteract")->connect("pressed", [&]() { mGui.get<tgui::ChildWindow>("ChildWindowInteract")->setVisible(true); });

      //Character Sub-Window
      mGui.get<tgui::Button>("ButtonFeed")->connect("pressed", &App::openFeedMenu, this);
      mGui.get<tgui::Button>("ButtonEquip")->connect("pressed", &App::openEquipMenu, this);
      mGui.get<tgui::Button>("ButtonGive")->connect("pressed", [&]() {});
      mGui.get<tgui::Button>("ButtonShop")->connect("pressed", [&]() { mGui.get<tgui::ChildWindow>("ChildWindowShop")->setVisible(true); });
      mGui.get<tgui::Button>("ButtonLibrary")->connect("pressed", [&]() {});
      mGui.get<tgui::Button>("ButtonExplore")->connect("pressed", [&]() {});
      mGui.get<tgui::Button>("ButtonSetSchedule")->connect("pressed", [&]() {});
      mGui.get<tgui::Button>("ButtonStat")->connect("pressed", &App::toggleFluffText, this);
      mGui.get<tgui::Button>("ButtonInteractCancel")->connect("pressed", [&]() { mGui.get<tgui::ChildWindow>("ChildWindowInteract")->setVisible(false); });

      //Feed Menu
      mGui.get<tgui::Button>("ButtonFeedMenuFeed")->connect("pressed", &App::feed, this);
      mGui.get<tgui::Button>("ButtonFeedMenuCancel")->connect("pressed", [&]() { mGui.get<tgui::ChildWindow>("ChildWindowFeed")->setVisible(false); });
      mGui.get<tgui::Button>("ButtonFeedAddFood")->connect("pressed", &App::addFoodToFeed, this);
      mGui.get<tgui::Button>("ButtonFeedAddPotion")->connect("pressed", &App::addPotionToFeed, this);

      //Equip Menu
      mGui.get<tgui::Button>("ButtonEquipMenuEquip")->connect("pressed", &App::equip, this);
      mGui.get<tgui::Button>("ButtonEquipMenuCancel")->connect("pressed", [&]() {  mGui.get<tgui::ChildWindow>("ChildWindowEquip")->setVisible(false); });

      //Shop Menus
      mGui.get<tgui::Button>("ButtonShopWeapon")->connect("pressed", [&]() {});
      mGui.get<tgui::Button>("ButtonShopFood")->connect("pressed", [&]() {});
      mGui.get<tgui::Button>("ButtonShopDress")->connect("pressed", [&]() {});
      mGui.get<tgui::Button>("ButtonShopBook")->connect("pressed", [&]() {});
      mGui.get<tgui::Button>("ButtonShopGift")->connect("pressed", [&]() {});
      mGui.get<tgui::Button>("ButtonShopMenuCancel")->connect("pressed", [&]() { mGui.get<tgui::ChildWindow>("ChildWindowShop")->setVisible(false); });

      mIsGUILoaded = true;
    }
    else if (gameState == GameState::loading)
    {
      mGui.loadWidgetsFromFile("gui/loadingscreen.gui");
      mGui.get<tgui::Button>("ButtonNewCharaCreate")->connect("pressed", &App::createCharacter, this);
      mIsGUILoaded = true;
    }
    else if (gameState == GameState::titleScreen)
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
      mIsGUILoaded = true;
    }
  }
}

void App::runTurns()
{
  uint64_t topHeight = mWalletAPI.getBlockHeight();

  if (mCommandProcessor.currentScanHeight < topHeight)
  {
    if (!mCommandProcessor.scanForCommands(topHeight))
    {
      //Exit game when scanned commands are critically invalid
      gameState = GameState::exit;
    }
  }

  while (mWorld.currentWorldHeight < topHeight && mClockMain.getElapsedTime().asSeconds() < 1)
  {
    mCommandProcessor.processCommand();
    mEvent.processEvent();
    mWorld.currentWorldHeight++;
  }

  if (mIsFullySync == false && mWorld.currentWorldHeight > mLastKnownDaemonHeight - 10)
  {
    mLastKnownDaemonHeight = mDaemonAPI.getBlockCount();

    if (mWorld.currentWorldHeight > mLastKnownDaemonHeight - 10)
    {
      mGui.get<tgui::Button>("ButtonFeed")->setEnabled(true);
      mGui.get<tgui::Button>("ButtonEquip")->setEnabled(true);
      mGui.get<tgui::Button>("ButtonGive")->setEnabled(true);
      mGui.get<tgui::Button>("ButtonShop")->setEnabled(true);
      mGui.get<tgui::Button>("ButtonLibrary")->setEnabled(true);
      mGui.get<tgui::Button>("ButtonExplore")->setEnabled(true);
      mGui.get<tgui::Button>("ButtonSetSchedule")->setEnabled(true);

      mIsFullySync = true;
    }
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
  statusText += "          ( Balance: " + std::to_string(mSwapBalance.unlockedBalance / pow(10, 12)) + " / " + std::to_string(mSwapBalance.totalBalance / pow(10, 12)) + "XWP ; ";
  statusText += "World Height: " + std::to_string(mWorld.currentWorldHeight) + " )";
  mWindow.setTitle(mSetting.windowTitle + statusText);
}

void App::toggleFluffText()
{
  if (mCharacter.isCharacterSheetFluff)
  {
    mCharacter.isCharacterSheetFluff = false;
    mGui.get<tgui::Button>("ButtonStat")->setText("Fluffs");
  }
  else
  {
    mCharacter.isCharacterSheetFluff = true;
    mGui.get<tgui::Button>("ButtonStat")->setText("Stats");
  }

  mCharacter.updateCharacterSheetText();
  mGui.get<tgui::Label>("LabelCharacterSheet")->setText(mCharacter.characterSheetText);
}

void App::openFeedMenu()
{
  for (int i = 0; i < 6; i++)
  {
    mFeedFoodID[i] = 0;
    mFeedPotionID[i] = 0;
  }
  mFeedFoodID[7] = 0;

  mGui.get<tgui::ChildWindow>("ChildWindowFeed")->setVisible(true);
  mGui.get<tgui::ListBox>("ListBoxFood")->removeAllItems();
  mGui.get<tgui::ListBox>("ListBoxPotion")->removeAllItems();
  mGui.get<tgui::ListBox>("ListBoxFeedSelection")->removeAllItems();

  for (auto& element : mCharacter.foodInventory)
  {
    mGui.get<tgui::ListBox>("ListBoxFood")->addItem(element.name + "(" + std::to_string(element.id) + ")", std::to_string(element.id));
  }
  for (auto& element : mCharacter.potionInventory)
  {
    mGui.get<tgui::ListBox>("ListBoxPotion")->addItem(element.name + "(" + std::to_string(element.id) + ")", std::to_string(element.id));
  }
}

void App::openEquipMenu()
{
  mGui.get<tgui::ChildWindow>("ChildWindowEquip")->setVisible(true);
  mGui.get<tgui::ListBox>("ListBoxWeapon")->removeAllItems();
  mGui.get<tgui::ListBox>("ListBoxDress")->removeAllItems();

  for (auto& element : mCharacter.weaponInventory)
  {
    mGui.get<tgui::ListBox>("ListBoxWeapon")->addItem(element.name + "(" + std::to_string(element.id) + ")", std::to_string(element.id));
  }
  for (auto& element : mCharacter.dressInventory)
  {
    mGui.get<tgui::ListBox>("ListBoxDress")->addItem(element.name + "(" + std::to_string(element.id) + ")", std::to_string(element.id));
  }
}

void App::equip()
{
  if (mSwapBalance.unlockedBalance > 100000000)
  {
    uint16_t weaponID = 0;
    uint16_t dressID = 0;

    if (mGui.get<tgui::ListBox>("ListBoxWeapon")->getSelectedItemId().toAnsiString() != "")
    {
      weaponID = std::stoi(mGui.get<tgui::ListBox>("ListBoxWeapon")->getSelectedItemId().toAnsiString());
    }

    if (mGui.get<tgui::ListBox>("ListBoxDress")->getSelectedItemId().toAnsiString() != "")
    {
      dressID = std::stoi(mGui.get<tgui::ListBox>("ListBoxDress")->getSelectedItemId().toAnsiString());
    }

    if (weaponID != 0 || dressID != 0)
    {
      mCommandProcessor.submitEquipCommand(weaponID, dressID);
      mGui.get<tgui::ChildWindow>("ChildWindowEquip")->setVisible(false);
      mWorld.logging.addToMainLog("Equipping item...");
    }
  }
  else
  {
    mWorld.logging.addToMainLog("Not enough XWP to pay transaction fees... ");
  }
}

void App::addFoodToFeed()
{
  for (int i = 0; i < 7; i++)
  {
    if (mFeedFoodID[i] == 0)
    {
      std::string itemName = mGui.get<tgui::ListBox>("ListBoxFood")->getSelectedItem();
      std::string itemID = mGui.get<tgui::ListBox>("ListBoxFood")->getSelectedItemId();
      mGui.get<tgui::ListBox>("ListBoxFood")->removeItemById(itemID);
      mGui.get<tgui::ListBox>("ListBoxFeedSelection")->addItem(itemName, itemID);
      mFeedFoodID[i] = std::stoi(itemID);
      return;
    }
  }
}

void App::addPotionToFeed()
{
  for (int i = 0; i < 6; i++)
  {
    if (mFeedPotionID[i] == 0)
    {
      std::string itemName = mGui.get<tgui::ListBox>("ListBoxPotion")->getSelectedItem();
      std::string itemID = mGui.get<tgui::ListBox>("ListBoxPotion")->getSelectedItemId();
      mGui.get<tgui::ListBox>("ListBoxPotion")->removeItemById(itemID);
      mGui.get<tgui::ListBox>("ListBoxFeedSelection")->addItem(itemName, itemID);
      mFeedPotionID[i] = std::stoi(itemID);
      return;
    }
  }
}

void App::feed()
{
  if (mSwapBalance.unlockedBalance > 100000000)
  {
    mCommandProcessor.submitFeedCommand(mFeedFoodID, mFeedPotionID);
    mGui.get<tgui::ChildWindow>("ChildWindowFeed")->setVisible(false);
    mWorld.logging.addToMainLog("Feeding " + mCharacter.profile.name + "...");
  }
  else
  {
    mWorld.logging.addToMainLog("Not enough XWP to pay transaction fees... ");
  }
}

