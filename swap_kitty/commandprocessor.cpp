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

void CommandProcessor::submitCharacterCreationCommand(const std::string& characterName)
{
  //Specification(Param): 24 byte Character Name, 2 byte Clock Offset

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

  command.param = convertStringToHex(characterName) + convertIntToHex(blockOffset);

  std::string commandHex = convertCommandToHex(command);
  std::cout << "Submitted Command: " << commandHex << "\n";
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
  std::cout << "Submitted Command: " << commandHex << "\n";
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
  std::cout << "Submitted Command: " << commandHex << "\n";
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
  std::cout << "Submitted Command: " << commandHex << "\n";
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
  std::cout << "Submitted Command: " << commandHex << "\n";
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
  std::cout << "Submitted Command: " << commandHex << "\n";
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
  std::cout << "Submitted Command: " << commandHex << "\n";
  mWalletAPI.transfer(mWalletAddress, commandHex, mTxAmount, mTxPriority, mMixin);
}

void CommandProcessor::submitFeedCommand(uint16_t foodID, uint16_t potionID)
{
  //Specification(Param): 22 byte Unused, 2 byte foodID, 2 byte potionID
  Command command;
  command.rulesetVersion = mWorld.currentRulesetVersion;
  command.commandCode = "FD";

  command.param = convertIntToHex(foodID) + convertIntToHex(potionID);

  std::string commandHex = convertCommandToHex(command);
  std::cout << "Submitted Command: " << commandHex << "\n";
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
  std::cout << "Submitted Command: " << commandHex << "\n";
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

void CommandProcessor::scanForCommands()
{
  std::vector<WalletAPI::PaymentID> paymentIDs;
  paymentIDs = mWalletAPI.getIncomingPaymentID(mCurrentScanHeight, mDaemonAPI.getBlockCount());

  for (auto& element : paymentIDs)
  {
    if (convertHexToString(element.paymentID.substr(0, 4)) == "SM" || (mIsBetaVersion && convertHexToString(element.paymentID.substr(0, 4)) == "SB"))
    {
      std::cout << "Recieved Command: " << element.paymentID << "\n";
      if (std::stol(element.paymentID.substr(4, 4), 0, 16) >= mWorld.currentRulesetVersion)
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
            std::cout << "Duplicate New Character Command detected; Ignoring...";
          }
        }
        else
        {
          std::cout << "Something went wrong: Commands were obtained in the incorrect order.";
        }
      }
    }
  }
  mCurrentScanHeight++;
}

bool CommandProcessor::sortCommand(Command i, Command j)
{
  return (i.param.substr(60, 4) < j.param.substr(60, 4));
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
    //if more than 1 commands are on the same block, process them in alphabetical order of their security hash
    std::sort(commands.begin(), commands.end(), sortCommand);

    while (!commands.empty())
    {
      if (commands.front().commandCode == "NC" && !mIsCharacterLoaded)
      {
        //New Character
        mWorld.localTimeOffset = std::stoi(commands.front().param.substr(48, 4), 0, 16);
        mCharacter.generateNewCharacter(getBlockHash(), convertHexToString(commands.front().param.substr(0, 48)));
        mIsCharacterLoaded = true;
        std::cout << "\n..::Created New Character::..\n" << mCharacter.fluffText << "\n";
      }
      else if (commands.front().commandCode == "RC")
      {
        //Resync Clock
        if (mLastTimeResyncRequest < mWorld.currentWorldHeight + 40320)
        {
          mWorld.localTimeOffset = std::stoi(commands.front().param.substr(44, 4), 0, 16);
          std::cout << "\n..::Adjusted In-Game Clock::..\n";
        }
        else
        {
          std::cout << "Time Resync Request Denied. Command already processed recently.";
        }
      }
      else if (commands.front().commandCode == "AS")
      {
        //Assign Schedule
        for (int i = 0; i < 24; i++)
        {
          mCharacter.dailySchedule[i] = mCharacter.job.getActivity(std::stoi(commands.front().param.substr(4 + i * 2, 2), 0, 16));
          std::cout << "\n..::Made Changes to Daily Schedule::..\n";
        }
      }
      else if (commands.front().commandCode == "AB")
      {
        //Add Books to Library
        std::vector<Book::BookItem> newBookInventory;

        for (int i = 0; i < 13; i++)
        {
          for (auto& element : mCharacter.bookInventory)
          {
            if (element.id == std::stoi(commands.front().param.substr(i * 4, 4), 0, 16))
            {
              mCharacter.library.push_back(element);
            }
            else
            {
              newBookInventory.push_back(element);
            }
          }
          
          mCharacter.bookInventory = newBookInventory;

          std::cout << "\n..::Added Some Books to Library::..\n";
        }
      }
      else if (commands.front().commandCode == "RB")
      {
        //Remove Books from Library
        std::vector<Book::BookItem> newLibraryInventory;

        for (int i = 0; i < 13; i++)
        {
          for (auto& element : mCharacter.bookInventory)
          {
            if (element.id == std::stoi(commands.front().param.substr(i * 4, 4), 0, 16))
            {
              mCharacter.bookInventory.push_back(element);
            }
            else
            {
              newLibraryInventory.push_back(element);
            }
          }

          mCharacter.library = newLibraryInventory;

          std::cout << "\n..::Removed Some Books from Library::..\n";
        }
      }
      else if (commands.front().commandCode == "BW")
      {
        std::cout << "\n..::Bought Some Weapons::..\n";
      }
      else if (commands.front().commandCode == "SW")
      {
        std::cout << "\n..::Sold Some Weapon::..\n";
      }
      else if (commands.front().commandCode == "DW")
      {
        std::cout << "\n..::Discarded Some Weapon::..\n";
      }
      else if (commands.front().commandCode == "BD")
      {
        std::cout << "\n..::Bought Some Dress::..\n";
      }
      else if (commands.front().commandCode == "SD")
      {
        std::cout << "\n..::Sold Some Dress::..\n";
      }
      else if (commands.front().commandCode == "DD")
      {
        std::cout << "\n..::Discarded Some Dress::..\n";
      }
      else if (commands.front().commandCode == "BF")
      {
        std::cout << "\n..::Bought Some Food::..\n";
      }
      else if (commands.front().commandCode == "SF")
      {
        std::cout << "\n..::Sold Some Food::..\n";
      }
      else if (commands.front().commandCode == "DF")
      {
        std::cout << "\n..::Discarded Some Food::..\n";
      }
      else if (commands.front().commandCode == "BP")
      {
        std::cout << "\n..::Bought Some Potions::..\n";
      }
      else if (commands.front().commandCode == "SP")
      {
        std::cout << "\n..::Sold Some Potion::..\n";
      }
      else if (commands.front().commandCode == "DP")
      {
        std::cout << "\n..::Discarded Some Potion::..\n";
      }
      else if (commands.front().commandCode == "BB")
      {
        std::cout << "\n..::Bought Some Books::..\n";
      }
      else if (commands.front().commandCode == "SB")
      {
        std::cout << "\n..::Sold Some Book::..\n";
      }
      else if (commands.front().commandCode == "DB")
      {
      std::cout << "\n..::Discarded Some Books::..\n";
      }
      else if (commands.front().commandCode == "BT")
      {
        std::cout << "\n..::Bought Some Toys::..\n";
      }
      else if (commands.front().commandCode == "ST")
      {
        std::cout << "\n..::Sold Some Toys::..\n";
      }
      else if (commands.front().commandCode == "DT")
      {
        std::cout << "\n..::Discarded Some Toys::..\n";
      }

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

template<typename INT_T>
std::string CommandProcessor::convertIntToHex(INT_T integer)
{
  std::stringstream stream;
  stream << std::setfill('0') << std::setw(sizeof(INT_T)*2) << std::hex << integer;
  return stream.str();
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
  default: std::cout << "Item Lookup error\n"; return uint8_t(-1);
  }
}
