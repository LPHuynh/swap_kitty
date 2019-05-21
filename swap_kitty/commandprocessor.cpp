#include "commandprocessor.h"


CommandProcessor::CommandProcessor()
{
  mCurrentScanHeight = 0;
  mLastTimeResyncRequest = 0;
  mIsCharacterLoaded = false;
}

CommandProcessor::~CommandProcessor()
{
}

void CommandProcessor::init(DaemonAPI& daemonAPI, WalletAPI& walletAPI, World& world, Character& character, const std::string& signingKey, uint16_t txPriority, uint16_t mixin, uint64_t startingScanHeight, bool isBetaVersion)
{
  mDaemonAPI = daemonAPI;
  mWalletAPI = walletAPI;
  mWorld = world;
  mWalletAddress = mWalletAPI.getAddress();
  mCharacter = character;
  mTxPriority = txPriority;
  mMixin = mixin;
  mSigningKey = signingKey;
  mCurrentScanHeight = startingScanHeight;
  mIsBetaVersion = isBetaVersion;

  if (mSigningKey == "")
  {
    //If signing key is not set, use the first word from the wallet's mnemonic seed
    std::istringstream iss(mWalletAPI.getMnemonicSeed());
    std::vector<std::string> results(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());
    mSigningKey = results.at(0);
  }
}

void CommandProcessor::submitCharacterCreationCommand(const std::string& characterName)
{
  //Specification(Param): 22 byte Character Name, 2 byte Clock Offset

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
  mWalletAPI.transfer(mWalletAddress, commandHex, mTxAmount, mTxPriority, mMixin);
}

void CommandProcessor::submitResyncGameClock(uint16_t offset)
{
  //Specification(Param): 22 byte Unused, 2 byte Offset

  Command command;
  command.rulesetVersion = mWorld.currentRulesetVersion;
  command.commandCode = "RC";
  command.param = convertIntToHex(offset);

  std::string commandHex = convertCommandToHex(command);
  mWalletAPI.transfer(mWalletAddress, commandHex, mTxAmount, mTxPriority, mMixin);
}

void CommandProcessor::submitAssignScheduleCommand(uint16_t activity[12])
{
  //Specification(Param): (2 byte Daily Activities)x12.

  Command command;
  command.rulesetVersion = mWorld.currentRulesetVersion;
  command.commandCode = "AS";
  command.param = "";

  for (int i = 0; i < 12; i++)
  {
    command.param += convertIntToHex(activity[i]);
  }

  std::string commandHex = convertCommandToHex(command);
  mWalletAPI.transfer(mWalletAddress, commandHex, mTxAmount, mTxPriority, mMixin);
}

void CommandProcessor::submitAssignBookCommand(uint16_t ID[12])
{
  //Specification(Param): (2 byte ID)x12

  Command command;
  command.rulesetVersion = mWorld.currentRulesetVersion;
  command.commandCode = "AB";
  command.param = "";

  for (int i = 0; i < 12; i++)
  {
    command.param += convertIntToHex(ID[i]);
  }

  std::string commandHex = convertCommandToHex(command);
  mWalletAPI.transfer(mWalletAddress, commandHex, mTxAmount, mTxPriority, mMixin);
}

void CommandProcessor::submitUseItemCommand(World::ItemAction itemAction, World::ItemType itemType[8], uint16_t ID[8], uint64_t expiryHeight)
{
  //Specification(Param): Buy: 8 byte expiry height, 1 byte Unused, (1 byte Item Type, 2 byte ID)x5
  //Specification(Param): Other: (1 byte Item Type, 2 byte ID)x8

  Command command;
  command.rulesetVersion = mWorld.currentRulesetVersion;

  uint16_t maxItem = 12;
  command.param = "";

  switch (itemAction)
  {
  case World::ItemAction::buy: command.commandCode = "BI"; command.param = convertIntToHex(expiryHeight) + "00"; maxItem = 5; break;
  case World::ItemAction::use: command.commandCode = "UI"; break;
  case World::ItemAction::sell: command.commandCode = "SI"; break;
  case World::ItemAction::discard: command.commandCode = "DI"; break;
  }

  for (int i = 0; i < maxItem; i++)
  {
    if (ID > 0)
    {
      command.param += convertIntToHex(lookupItemTable(itemType[i])) + convertIntToHex(ID[i]);
    }
  }

  std::string commandHex = convertCommandToHex(command);
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
        std::string seed = element.paymentID.substr(0, 60) + mSigningKey;
        std::string securityHash = convertIntToHex(mWorld.getRandomNumber(seed, 0, uint16_t(-1)));

        if (element.paymentID.substr(60, 4) == securityHash)
        {
          mCurrentScanHeight = element.height + 1;
          mWorld.currentWorldHeight = (element.height);
          mCommandQueue.push(std::make_pair(element.height, convertHexToCommand(element.paymentID)));
          return true;
        }
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
      if (std::stol(element.paymentID.substr(4, 4), 0, 16) >= mWorld.currentRulesetVersion)
      {
        std::string seed = element.paymentID.substr(0, 60) + mSigningKey;
        std::string securityHash = convertIntToHex(mWorld.getRandomNumber(seed, 0, uint16_t(-1)));

        if (element.paymentID.substr(60, 4) == securityHash)
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
  }
  mCurrentScanHeight++;
}

void CommandProcessor::processCommand()
{
  if (mCommandQueue.front().first == mWorld.currentWorldHeight)
  {
    Command command = mCommandQueue.front().second;
    mCommandQueue.pop();

    if (command.commandCode == "NC" && !mIsCharacterLoaded)
    {
      mCharacter.init(mDaemonAPI.getBlockHash(mWorld.currentWorldHeight), mWorld, convertHexToString(command.param.substr(0, 44)));
      mWorld.localTimeOffset = std::stoi(command.param.substr(44, 4), 0, 16);
      mIsCharacterLoaded = true;
    }
    else if (command.commandCode == "RC")
    {
      if (mLastTimeResyncRequest < mWorld.currentWorldHeight + 40320)
      {
        mWorld.localTimeOffset = std::stoi(command.param.substr(44, 4), 0, 16);
      }
      else
      {
        std::cout << "Time Resync Request Denied. Command already processed recently.";
      }
    }
    else if (command.commandCode == "AS")
    {
    }
    else if (command.commandCode == "AB")
    {
    }
    else if (command.commandCode == "BI")
    {
    }
    else if (command.commandCode == "UI")
    {
    }
    else if (command.commandCode == "SI")
    {
    }
    else if (command.commandCode == "DI")
    {
    }
  }  
}

std::string CommandProcessor::convertCommandToHex(const CommandProcessor::Command& command)
{
  //Specification: Command Hex String represents 64 nibble or 32 byte in human-readable string format.
  //Reserved Placement of data (in order): 2 byte Game for Identifier, 2 byte for Ruleset Verion, 2 byte for Command Code, 24 byte for Param(Hex String), 2 for byte Security Hash.

  std::stringstream filledCommand;
  std::stringstream filledParamHex;
  filledCommand << std::setfill('0') << std::setw(4) << command.commandCode;
  filledParamHex << std::setfill('0') << std::setw(48) << command.param;

  std::string hexString;
  std::string gameHexCode;

  switch(mIsBetaVersion)
  {
    case true: gameHexCode = convertStringToHex("SB"); break;
    case false: gameHexCode = convertStringToHex("SM"); break;
  }

  std::string seed = gameHexCode + convertIntToHex(command.rulesetVersion) + filledCommand.str() + filledParamHex.str() + mSigningKey;
  std::string securityHash = convertIntToHex(mWorld.getRandomNumber(seed, 0, uint16_t(-1)));

  return gameHexCode + convertIntToHex(command.rulesetVersion) + filledCommand.str() + filledParamHex.str() + securityHash;
}

CommandProcessor::Command CommandProcessor::convertHexToCommand(const std::string& hexadecimalString)
{
  Command commandResult;

  commandResult.rulesetVersion = 0;
  commandResult.commandCode = "";
  commandResult.param = "";

  if (hexadecimalString.substr(0, 4) == convertStringToHex("SM") || (mIsBetaVersion && hexadecimalString.substr(0, 4) == convertStringToHex("SB")))
  {
    std::string seed = hexadecimalString.substr(0,60) + mSigningKey;
    std::string securityHash = convertIntToHex(mWorld.getRandomNumber(seed, 0, uint16_t(-1)));

    if (hexadecimalString.substr(60, 4) == securityHash)
    {
      commandResult.rulesetVersion = std::stoi(hexadecimalString.substr(4, 4), 0, 16);
      commandResult.commandCode = convertHexToString(hexadecimalString.substr(8, 4));
      commandResult.param = hexadecimalString.substr(12, 48);
    }
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
  static const char* const lut = "0123456789ABCDEF";
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
  case World::ItemType::gift: return 6;
  default: std::cout << "Item Lookup error\n"; return uint8_t(-1);
  }
}
