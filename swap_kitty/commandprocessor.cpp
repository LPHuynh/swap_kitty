#include "commandprocessor.h"


CommandProcessor::CommandProcessor(DaemonAPI& daemonAPI, WalletAPI& walletAPI, World& world, Character& character) : mDaemonAPI(daemonAPI), mWalletAPI(walletAPI), mWorld(world), mCharacter(character)
{
  mCurrentScanHeight = 0;
  mLastTimeResyncRequest = 0;
  mIsCharacterLoaded = false;
  mWalletAddress = "";
  mTxPriority = 0;
  mMixin = 0;
  mCurrentScanHeight = 0;
  mIsBetaVersion = false;
}

CommandProcessor::~CommandProcessor()
{
}

void CommandProcessor::init(uint64_t txAmount, uint16_t txPriority, uint16_t mixin, uint64_t startingScanHeight, bool isBetaVersion)
{
  mWalletAddress = mWalletAPI.getAddress();
  mTxAmount = txAmount;
  mTxPriority = txPriority;
  mMixin = mixin;
  mCurrentScanHeight = startingScanHeight;
  mIsBetaVersion = isBetaVersion;
}

void CommandProcessor::submitCharacterCreationCommand(const std::string& characterName, uint8_t characterOption)
{
  //Specification(Param): 23 byte Character Name, 1 byte Character Option, 2 byte Clock Offset

  Command command;
  command.rulesetVersion = mWorld.currentRulesetVersion;
  command.commandCode = "NC";

  //Adjust in-game time to synchronize with localtime:
  std::time_t currentSystemTime;

  time(&currentSystemTime);
  struct tm time;
  localtime_s(&time, &currentSystemTime);
  uint16_t currentHour = time.tm_hour;
  uint16_t currentMinute = time.tm_min;
  uint16_t elapsedMinutes = currentHour * 60 + currentMinute;
  uint16_t adjustedBlock = elapsedMinutes * 4;

  uint64_t currentBlockHeight = mDaemonAPI.getBlockCount();
  uint16_t blockHeightTime = currentBlockHeight % 5760;
  if (blockHeightTime < adjustedBlock)
  {
    blockHeightTime += 5760;
  }
  uint16_t blockOffset = blockHeightTime - adjustedBlock;
  ////

  command.param = convertStringToHex(characterName) + convertInt8ToHex(characterOption) + convertIntToHex(blockOffset);

  std::string commandHex = convertCommandToHex(command);
  mWorld.logging.changeDefaultStatusMessage("Submitted Character Creation Command", "none");
  mWorld.logging.writeToFile("Submitted Command: " + commandHex);
  mWalletAPI.transfer(mWalletAddress, commandHex, mTxAmount, mTxPriority, mMixin);
}

void CommandProcessor::submitResyncGameClock(uint16_t offset)
{
  //Specification(Param): 24 byte Unused, 2 byte Offset

  Command command;
  command.rulesetVersion = mWorld.currentRulesetVersion;
  command.commandCode = "RC";
  command.param = convertIntToHex(offset);

  std::string commandHex = convertCommandToHex(command);
  mWorld.logging.writeToFile ("Submitted Command: " + commandHex);
  mWalletAPI.transfer(mWalletAddress, commandHex, mTxAmount, mTxPriority, mMixin);
}

void CommandProcessor::submitAssignScheduleCommand(uint8_t activity[24])
{
  //Specification(Param): 2 bytes unused, (1 byte Daily Activities)x24.

  Command command;
  command.rulesetVersion = mWorld.currentRulesetVersion;
  command.commandCode = "AS";
  command.param = "";

  for (int i = 0; i < 24; i++)
  {
    command.param += convertIntToHex(activity[i]);
  }

  std::string commandHex = convertCommandToHex(command);
  mWorld.logging.writeToFile("Submitted Command: " + commandHex);
  mWalletAPI.transfer(mWalletAddress, commandHex, mTxAmount, mTxPriority, mMixin);
}

void CommandProcessor::submitAddBookCommand(uint16_t ID[13])
{
  //Specification(Param): (2 byte ID)x13

  Command command;
  command.rulesetVersion = mWorld.currentRulesetVersion;
  command.commandCode = "AB";
  command.param = "";

  for (int i = 0; i < 13; i++)
  {
    command.param += convertIntToHex(ID[i]);
  }

  std::string commandHex = convertCommandToHex(command);
  mWorld.logging.writeToFile("Submitted Command: " + commandHex);
  mWalletAPI.transfer(mWalletAddress, commandHex, mTxAmount, mTxPriority, mMixin);
}

void CommandProcessor::submitRemoveBookCommand(uint16_t ID[13])
{
  //Specification(Param): (2 byte ID)x13

  Command command;
  command.rulesetVersion = mWorld.currentRulesetVersion;
  command.commandCode = "RB";
  command.param = "";

  for (int i = 0; i < 13; i++)
  {
    command.param += convertIntToHex(ID[i]);
  }

  std::string commandHex = convertCommandToHex(command);
  mWorld.logging.writeToFile("Submitted Command: " + commandHex);
  mWalletAPI.transfer(mWalletAddress, commandHex, mTxAmount, mTxPriority, mMixin);
}

void CommandProcessor::submitManageItemCommand(World::ItemAction itemAction, World::ItemType itemType, uint16_t itemID[13])
{
  //Specification(Param): (2 byte ID)x13

  Command command;
  command.rulesetVersion = mWorld.currentRulesetVersion;

  command.param = "";

  switch (itemType)
  {
  case World::ItemType::weapon: 
    switch (itemAction)
    {
    case World::ItemAction::buy: command.commandCode = "BW"; break;
    case World::ItemAction::sell: command.commandCode = "SW"; break;
    case World::ItemAction::discard: command.commandCode = "DW"; break;
    } break;
  case World::ItemType::dress:
    switch (itemAction)
    {
    case World::ItemAction::buy: command.commandCode = "BD"; break;
    case World::ItemAction::sell: command.commandCode = "SD"; break;
    case World::ItemAction::discard: command.commandCode = "DD"; break;
    } break;
  case World::ItemType::food:
    switch (itemAction)
    {
    case World::ItemAction::buy: command.commandCode = "BF"; break;
    case World::ItemAction::sell: command.commandCode = "SF"; break;
    case World::ItemAction::discard: command.commandCode = "DF"; break;
    } break;
  case World::ItemType::potion:
    switch (itemAction)
    {
    case World::ItemAction::buy: command.commandCode = "BP"; break;
    case World::ItemAction::sell: command.commandCode = "SP"; break;
    case World::ItemAction::discard: command.commandCode = "DP"; break;
    } break;
  case World::ItemType::book:
    switch (itemAction)
    {
    case World::ItemAction::buy: command.commandCode = "BB"; break;
    case World::ItemAction::sell: command.commandCode = "SB"; break;
    case World::ItemAction::discard: command.commandCode = "DB"; break;
    } break;
  case World::ItemType::toy:
    switch (itemAction)
    {
    case World::ItemAction::buy: command.commandCode = "BT"; break;
    case World::ItemAction::sell: command.commandCode = "ST"; break;
    case World::ItemAction::discard: command.commandCode = "DT"; break;
    } break;
  }


  for (int i = 0; i < 13; i++)
  {
    if (itemID > 0)
    {
      command.param += convertIntToHex(convertIntToHex(itemID[i]));
    }
  }

  std::string commandHex = convertCommandToHex(command);
  mWorld.logging.writeToFile("Submitted Command: " + commandHex);
  mWalletAPI.transfer(mWalletAddress, commandHex, mTxAmount, mTxPriority, mMixin);
}

void CommandProcessor::submitGiftCommand(uint16_t toyID[13])
{
  //Specification(Param): (2 byte toyID)x13
  Command command;
  command.rulesetVersion = mWorld.currentRulesetVersion;
  command.commandCode = "GT";

  for (int i = 0; i < 13; i++)
  {
    if (toyID > 0)
    {
      command.param = convertIntToHex(toyID);
    }
  }

  std::string commandHex = convertCommandToHex(command);
  mWorld.logging.writeToFile("Submitted Command: " + commandHex);
  mWalletAPI.transfer(mWalletAddress, commandHex, mTxAmount, mTxPriority, mMixin);
}

void CommandProcessor::submitFeedCommand(uint16_t foodID[7], uint16_t potionID[6])
{
  //Specification(Param): (2 byte foodID)x7, (2 byte potionID)x6
  Command command;
  command.rulesetVersion = mWorld.currentRulesetVersion;
  command.commandCode = "FD";

  command.param = "";
  
  for (int i = 0; i < 7; i++)
  {
    command.param += convertIntToHex(foodID);
  }
  for (int i = 0; i < 6; i++)
  {
    command.param += convertIntToHex(potionID);
  }

  std::string commandHex = convertCommandToHex(command);
  mWorld.logging.writeToFile("Submitted Command: " + commandHex);
  mWalletAPI.transfer(mWalletAddress, commandHex, mTxAmount, mTxPriority, mMixin);
}

void CommandProcessor::submitEquipCommand(uint16_t weaponID, uint16_t dressID)
{
  //Specification(Param): 22 byte Unused, 2 byte weaponID, 2 byte dressID
  Command command;
  command.rulesetVersion = mWorld.currentRulesetVersion;
  command.commandCode = "EQ";

  command.param = convertIntToHex(weaponID) + convertIntToHex(dressID);

  std::string commandHex = convertCommandToHex(command);
  mWorld.logging.writeToFile("Submitted Command: " + commandHex);
  mWalletAPI.transfer(mWalletAddress, commandHex, mTxAmount, mTxPriority, mMixin);
}

bool CommandProcessor::scanForCharacterCreationCommand()
{
  Command command;
  command.commandCode = "";
  command.rulesetVersion = 0;
  command.param = "";

  std::vector<WalletAPI::PaymentID> paymentIDs;
  paymentIDs = mWalletAPI.getIncomingPaymentID(mCurrentScanHeight, mDaemonAPI.getBlockCount());

  for (auto& element : paymentIDs)
  {
    if (convertHexToString(element.paymentID.substr(0, 4)) == "SM" || (mIsBetaVersion && convertHexToString(element.paymentID.substr(0, 4)) == "SB"))
    {
      if (convertHexToString(element.paymentID.substr(8, 4)) == "NC")
      {
        mCurrentScanHeight = element.height + 1;
        mWorld.currentWorldHeight = (element.height);
        mCommandQueue.push(std::make_pair(element.height, convertHexToCommand(element.paymentID)));
        return true;
      }
    }
  }
  return false;
}

bool CommandProcessor::scanForCommands()
{
  std::vector<WalletAPI::PaymentID> paymentIDs;
  paymentIDs = mWalletAPI.getIncomingPaymentID(mCurrentScanHeight, mDaemonAPI.getBlockCount());

  for (auto& element : paymentIDs)
  {
    if (convertHexToString(element.paymentID.substr(0, 4)) == "SM" || (mIsBetaVersion && convertHexToString(element.paymentID.substr(0, 4)) == "SB"))
    {
      mWorld.logging.writeToFile("Recieved Command: " + element.paymentID);
      uint16_t ruleset = std::stoi(element.paymentID.substr(4, 4), 0, 16);

      if (ruleset >= mWorld.currentRulesetVersion)
      {
        if (ruleset <= mWorld.lastestRulesetVersion)
        {
          if (element.height >= mCurrentScanHeight)
          {
            if (element.paymentID.substr(8, 4) != "NC")
            {
              mCurrentScanHeight = element.height;
              mCommandQueue.push(std::make_pair(element.height, convertHexToCommand(element.paymentID)));
            }
            else
            {
              mWorld.logging.addToMainLog("Duplicate New Character Command detected; Ignoring...");
            }
          }
          else
          {
            mWorld.logging.addToMainLog("Error: Commands were obtained in the incorrect order.");
            return false;
          }
        }
        else
        {
          mWorld.logging.addToMainLog("Error: Commands with a Ruleset Version that is higher than supported version was detected. Please upgrade to the latest version.");
          return false;
        }
      }
      else
      {
        mWorld.logging.addToMainLog("Warning: Commands with a Ruleset Version that is lower than the current Ruleset was detected. Ignoring...");
      }
    }
  }
  mCurrentScanHeight++;
  return true;
}

bool CommandProcessor::sortCommand(Command i, Command j)
{
  return (i.param.substr(0, 64) < j.param.substr(0, 64));
}

void CommandProcessor::processCommand()
{
  if (mCommandQueue.empty())
  {
    return;
  }

  if (mCommandQueue.front().first == mWorld.currentWorldHeight)
  {
    std::deque<Command> commands;
    if (!mCommandQueue.empty())
    {
      while (mCommandQueue.front().first == mWorld.currentWorldHeight)
      {
        commands.push_back(mCommandQueue.front().second);
        mCommandQueue.pop();

        if (mCommandQueue.empty())
        {
          break;
        }
      }
    }
    //if more than 1 commands are on the same block, process them in alphabetical order
    std::sort(commands.begin(), commands.end(), sortCommand);

    while (!commands.empty())
    {
      if (commands.front().commandCode == "NC" && !mIsCharacterLoaded)
      {
        mWorld.localTimeOffset = std::stoi(commands.front().param.substr(48, 4), 0, 16);
        mCharacter.generateNewCharacter(getBlockHash(), convertHexToString(commands.front().param.substr(0, 46)));

        uint8_t characterOption = std::stoi(commands.front().param.substr(46, 2), 0, 16);

        if (characterOption < 128)
        {
          mCharacter.profile.cosmetic.gender = Cosmetic::Gender::female;
        }
        else
        {
          mCharacter.profile.cosmetic.gender = Cosmetic::Gender::male;
          characterOption -= 128;
        }

        switch (characterOption)
        {
        case 0: mCharacter.profile.cosmetic.species = Cosmetic::Species::catgirl; break;
        default: mCharacter.profile.cosmetic.species = Cosmetic::Species::catgirl; break;
        }
        
        mIsCharacterLoaded = true;
        mWorld.logging.addToMainLog("..::New Character Created::..\n" + mCharacter.fluffText);
      }
      else if (commands.front().commandCode == "RC")
      {
        mWorld.logging.addToMainLog("..::Adjusted In-Game Clock::..");
        if (mLastTimeResyncRequest < mWorld.currentWorldHeight + 40320)
        {
          mWorld.localTimeOffset = std::stoi(commands.front().param.substr(44, 4), 0, 16);
        }
        else
        {
          mWorld.logging.addToMainLog("Time Resync Request Denied. Command processed too recently.");
        }
      }
      else if (commands.front().commandCode == "AS")
      {
        mWorld.logging.addToMainLog("..::Made Changes to Daily Schedule::..");
        Job::Activity activity;

        for (int i = 0; i < 24; i++)
        {
          activity = mCharacter.job.getActivity(std::stoi(commands.front().param.substr(4 + i * 2, 2), 0, 16));

          if (activity.isDayActivity && (i > 5 || i < 20) || activity.isNightActivity && (i < 6 || i > 19))
          {
            mCharacter.dailySchedule[i] = activity;
          }
          else
          {
            mWorld.logging.addToMainLog("Error. Invalid schedule item deteced. " + activity.name + " could not be set at " + std::to_string(i) + "h");
          }
        }
      }
      else if (commands.front().commandCode == "AB")
      {
        mWorld.logging.addToMainLog("..::Added Some Books to Library::..");
        for (int i = 0; i < 13; i++)
        {
          mCharacter.transferItem(mCharacter.bookInventory, mCharacter.library, std::stoi(commands.front().param.substr(i * 4, 4), 0, 16), false);
        }
      }
      else if (commands.front().commandCode == "RB")
      {
        mWorld.logging.addToMainLog("..::Removed Some Books from Library::..");
        for (int i = 0; i < 13; i++)
        {
          mCharacter.transferItem(mCharacter.library, mCharacter.bookInventory, std::stoi(commands.front().param.substr(i * 4, 4), 0, 16), false);
        }
      }
      else if (commands.front().commandCode == "BW")
      {
        mWorld.logging.addToMainLog("..::Bought Some Weapons::..");
        for (int i = 0; i < 13; i++)
        {
          mCharacter.transferItem(mCharacter.shop.weaponInventory, mCharacter.weaponInventory, std::stoi(commands.front().param.substr(i * 4, 4), 0, 16), true);
        }
      }
      else if (commands.front().commandCode == "SW")
      {
        mWorld.logging.addToMainLog("..::Sold Some Weapon::..");
        for (int i = 0; i < 13; i++)
        {
          mCharacter.destroyItem(mCharacter.weaponInventory, std::stoi(commands.front().param.substr(i * 4, 4), 0, 16), true);
        }
      }
      else if (commands.front().commandCode == "DW")
      {
        mWorld.logging.addToMainLog("..::Discarded Some Weapon::..");
        for (int i = 0; i < 13; i++)
        {
          mCharacter.destroyItem(mCharacter.weaponInventory, std::stoi(commands.front().param.substr(i * 4, 4), 0, 16), false);
        }
      }
      else if (commands.front().commandCode == "BD")
      {
        mWorld.logging.addToMainLog("..::Bought Some Dress::..");
        for (int i = 0; i < 13; i++)
        {
          mCharacter.transferItem(mCharacter.shop.dressInventory, mCharacter.dressInventory, std::stoi(commands.front().param.substr(i * 4, 4), 0, 16), true);
        }
      }
      else if (commands.front().commandCode == "SD")
      {
        mWorld.logging.addToMainLog("..::Sold Some Dress::..");
        for (int i = 0; i < 13; i++)
        {
          mCharacter.destroyItem(mCharacter.dressInventory, std::stoi(commands.front().param.substr(i * 4, 4), 0, 16), true);
        }
      }
      else if (commands.front().commandCode == "DD")
      {
        mWorld.logging.addToMainLog("..::Discarded Some Dress::..");
        for (int i = 0; i < 13; i++)
        {
          mCharacter.destroyItem(mCharacter.dressInventory, std::stoi(commands.front().param.substr(i * 4, 4), 0, 16), false);
        }
      }
      else if (commands.front().commandCode == "BF")
      {
        mWorld.logging.addToMainLog("..::Bought Some Food::..");
        for (int i = 0; i < 13; i++)
        {
          int id = std::stoi(commands.front().param.substr(i * 4, 4), 0, 16);
          mCharacter.transferItem(mCharacter.shop.fruitInventory, mCharacter.foodInventory, id, true);
          mCharacter.transferItem(mCharacter.shop.vegatableInventory, mCharacter.foodInventory, id, true);
          mCharacter.transferItem(mCharacter.shop.fishInventory, mCharacter.foodInventory, id, true);
          mCharacter.transferItem(mCharacter.shop.corpseInventory, mCharacter.foodInventory, id, true);
          mCharacter.transferItem(mCharacter.shop.flourInventory, mCharacter.foodInventory, id, true);
          mCharacter.transferItem(mCharacter.shop.junkfoodInventory, mCharacter.foodInventory, id, true);
        }
      }
      else if (commands.front().commandCode == "SF")
      {
        mWorld.logging.addToMainLog("..::Sold Some Food::..");
        for (int i = 0; i < 13; i++)
        {
          mCharacter.destroyItem(mCharacter.foodInventory, std::stoi(commands.front().param.substr(i * 4, 4), 0, 16), true);
        }
      }
      else if (commands.front().commandCode == "DF")
      {
        mWorld.logging.addToMainLog("..::Discarded Some Food::..");
        for (int i = 0; i < 13; i++)
        {
          mCharacter.destroyItem(mCharacter.foodInventory, std::stoi(commands.front().param.substr(i * 4, 4), 0, 16), false);
        }
      }
      else if (commands.front().commandCode == "BP")
      {
        mWorld.logging.addToMainLog("..::Bought Some Potions::..");
        for (int i = 0; i < 13; i++)
        {
          mCharacter.transferItem(mCharacter.shop.potionInventory, mCharacter.potionInventory, std::stoi(commands.front().param.substr(i * 4, 4), 0, 16), true);
        }
      }
      else if (commands.front().commandCode == "SP")
      {
        mWorld.logging.addToMainLog("..::Sold Some Potion::..");
        for (int i = 0; i < 13; i++)
        {
          mCharacter.destroyItem(mCharacter.potionInventory, std::stoi(commands.front().param.substr(i * 4, 4), 0, 16), true);
        }
      }
      else if (commands.front().commandCode == "DP")
      {
        mWorld.logging.addToMainLog("..::Discarded Some Potion::..");
        for (int i = 0; i < 13; i++)
        {
          mCharacter.destroyItem(mCharacter.potionInventory, std::stoi(commands.front().param.substr(i * 4, 4), 0, 16), false);
        }
      }
      else if (commands.front().commandCode == "BB")
      {
        mWorld.logging.addToMainLog("..::Bought Some Books::..");
        for (int i = 0; i < 13; i++)
        {
          mCharacter.transferItem(mCharacter.shop.bookInventory, mCharacter.bookInventory, std::stoi(commands.front().param.substr(i * 4, 4), 0, 16), true);
        }
      }
      else if (commands.front().commandCode == "SB")
      {
        mWorld.logging.addToMainLog("..::Sold Some Book::..");
        for (int i = 0; i < 13; i++)
        {
          mCharacter.destroyItem(mCharacter.bookInventory, std::stoi(commands.front().param.substr(i * 4, 4), 0, 16), true);
        }
      }
      else if (commands.front().commandCode == "DB")
      {
        mWorld.logging.addToMainLog("..::Discarded Some Books::..");
        for (int i = 0; i < 13; i++)
        {
          mCharacter.destroyItem(mCharacter.bookInventory, std::stoi(commands.front().param.substr(i * 4, 4), 0, 16), false);
        }
      }
      else if (commands.front().commandCode == "BT")
      {
        mWorld.logging.addToMainLog("..::Bought Some Toys::..");
        for (int i = 0; i < 13; i++)
        {
          mCharacter.transferItem(mCharacter.shop.toyInventory, mCharacter.toyInventory, std::stoi(commands.front().param.substr(i * 4, 4), 0, 16), true);
        }
      }
      else if (commands.front().commandCode == "ST")
      {
        mWorld.logging.addToMainLog("..::Sold Some Toys::..");
        for (int i = 0; i < 13; i++)
        {
          mCharacter.destroyItem(mCharacter.toyInventory, std::stoi(commands.front().param.substr(i * 4, 4), 0, 16), true);
        }
      }
      else if (commands.front().commandCode == "DT")
      {
        mWorld.logging.addToMainLog("..::Discarded Some Toys::..");
        for (int i = 0; i < 13; i++)
        {
          mCharacter.destroyItem(mCharacter.toyInventory, std::stoi(commands.front().param.substr(i * 4, 4), 0, 16), false);
        }
      }
      else if (commands.front().commandCode == "GT")
      {
        mWorld.logging.addToMainLog("..::Gave Some Toys to " + mCharacter.profile.name +"::..");
        for (int i = 0; i < 13; i++)
        {
          if (mCharacter.transferItem(mCharacter.toyInventory, mCharacter.toyRoom, std::stoi(commands.front().param.substr(i * 4, 4), 0, 16), false))
          {
            mCharacter.profile.happiness += int16_t(mCharacter.toyRoom.back().price);
            mWorld.logging.addToMainLog("\t" + mCharacter.profile.name + " loves her new " + mCharacter.toyRoom.back().name);
          }
        }
      }
      else if (commands.front().commandCode == "FD")
      {
        mWorld.logging.addToMainLog("..::Feed Some Food/Drink to " + mCharacter.profile.name + "::..");
        for (int i = 0; i < 13; i++)
        {
          if (i < 8)
          {
            mCharacter.consumeFood(std::stoi(commands.front().param.substr(i * 4, 4), 0, 16), true);
          }
          else
          {
            mCharacter.consumePotion(std::stoi(commands.front().param.substr(i * 4, 4), 0, 16), true);
          }
        }
      }
      else if (commands.front().commandCode == "EQ")
      {
        mWorld.logging.addToMainLog("..::Equipped Some Weapon/Clothing on " + mCharacter.profile.name + "::..");
        uint16_t weaponID = std::stoi(commands.front().param.substr(44, 4), 0, 16);
        uint16_t dressID = std::stoi(commands.front().param.substr(48, 4), 0, 16);

        if (weaponID != 0)
        {
          for (auto& element : mCharacter.weaponInventory)
          {
            if (element.id = weaponID)
            {
              mCharacter.equipedWeapon = element;
              mWorld.logging.addToMainLog("\t" + mCharacter.profile.name + " equipped " + mCharacter.equipedWeapon.name);
            }
          }
        }
        if (dressID != 0)
        {
          for (auto& element : mCharacter.weaponInventory)
          {
            if (element.id = dressID)
            {
              mCharacter.equipedWeapon = element;
              mWorld.logging.addToMainLog("\t" + mCharacter.profile.name + " equipped " + mCharacter.equipedDress.name);
            }
          }
        }
      }
      mWorld.currentRulesetVersion = commands.front().rulesetVersion;
      commands.pop_front();
    }
  }
}

std::string CommandProcessor::convertCommandToHex(const CommandProcessor::Command& command)
{
  //Specification: Command Hex String represents 64 nibble or 32 byte in human-readable string format.
  //Reserved Placement of data (in order): 2 byte Game for Identifier, 2 byte for Ruleset Verion, 2 byte for Command Code, 26 byte for Param(Hex String)

  std::stringstream filledCommand;
  std::stringstream filledParamHex;
  filledCommand << std::setfill('0') << std::setw(4) << convertStringToHex(command.commandCode);
  filledParamHex << std::setfill('0') << std::setw(52) << command.param;

  std::string hexString;
  std::string gameHexCode;

  switch(mIsBetaVersion)
  {
    case true: gameHexCode = convertStringToHex("SB"); break;
    case false: gameHexCode = convertStringToHex("SM"); break;
  }

  return gameHexCode + convertIntToHex(command.rulesetVersion) + filledCommand.str() + filledParamHex.str();
}

CommandProcessor::Command CommandProcessor::convertHexToCommand(const std::string& hexadecimalString)
{
  Command commandResult;

  commandResult.rulesetVersion = 0;
  commandResult.commandCode = "";
  commandResult.param = "";

  if (convertHexToString(hexadecimalString.substr(0, 4)) == "SM" || (mIsBetaVersion && convertHexToString(hexadecimalString.substr(0, 4)) == "SB"))
  {
    commandResult.rulesetVersion = std::stoi(hexadecimalString.substr(4, 4), 0, 16);
    commandResult.commandCode = convertHexToString(hexadecimalString.substr(8, 4));
    commandResult.param = hexadecimalString.substr(12, 52);
  }

  return commandResult;
}

std::string CommandProcessor::convertHexToString(const std::string& hexadecimalString)
{
  std::string textString;

  for (uint8_t i = 0; i < hexadecimalString.length(); i += 2)
  {
    std::string byte = hexadecimalString.substr(i, 2);
    if (byte != "00")
    {
      char chr = (char)(int)strtol(byte.c_str(), 0, 16);
      textString.push_back(chr);
    }
  }

  return textString;
}

std::string CommandProcessor::convertStringToHex(const std::string& textString)
{
  static const char* const lut = "0123456789abcdef";
  size_t len = textString.length();

  std::string output;
  output.reserve(2 * len);
  for (size_t i = 0; i < len; ++i)
  {
    const unsigned char c = textString[i];
    output.push_back(lut[c >> 4]);
    output.push_back(lut[c & 15]);
  }
  return output;
}

std::string CommandProcessor::convertInt8ToHex(uint8_t integer)
{
  //workaround for uint8_t being treated as a char
  std::stringstream stream;
  stream << std::setfill('0') << std::setw(2) << std::hex << uint16_t(integer);
  return stream.str();
}

std::string CommandProcessor::getBlockHash()
{
  std::map<uint64_t, std::string>::iterator it = mWorld.blockhashCache.find(mWorld.currentWorldHeight - 5);
  if (it != mWorld.blockhashCache.end())
  {
    return it->second;
  }
  else
  {
    std::string blockHash = mDaemonAPI.getBlockHash(mWorld.currentWorldHeight - 5);
    mWorld.blockhashCache[mWorld.currentWorldHeight - 5] = blockHash;
    return blockHash;
  }
}

uint8_t CommandProcessor::lookupItemTable(World::ItemType itemType)
{
  switch (itemType)
  {
  case World::ItemType::weapon: return 0;
  case World::ItemType::dress: return 1;
  case World::ItemType::food: return 2; 
  case World::ItemType::potion: return 3;
  case World::ItemType::book: return 4;
  case World::ItemType::toy: return 5;
  default: mWorld.logging.addToMainLog("Item Lookup error"); return uint8_t(-1);
  }
}