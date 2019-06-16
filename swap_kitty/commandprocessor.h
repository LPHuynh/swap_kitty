#pragma once

#include "character.h"
#include "daemonapi.h"
#include "walletapi.h"
#include "world.h"

class CommandProcessor
{
public:
  CommandProcessor(DaemonAPI& daemonAPI, WalletAPI& walletAPI, World& world, Character& character);
  ~CommandProcessor();
  void init(uint64_t txAmount, uint16_t txPriority, uint16_t mixin, uint64_t startingScanHeight, bool isBetaVersion);

  struct Command
  {
    uint16_t rulesetVersion;
    std::string commandCode;
    std::string param;
  };

  void submitCharacterCreationCommand(const std::string& characterName, uint8_t characterOption);
  void submitResyncGameClock(uint16_t offset);
  void submitAssignScheduleCommand(uint8_t activity[24]);
  void submitAddBookCommand(uint16_t ID[13]);
  void submitRemoveBookCommand(uint16_t ID[13]);
  void submitManageItemCommand(World::ItemAction itemAction, World::ItemType itemType, uint16_t ID[13]);
  void submitGiftCommand(uint16_t toyID[13]);
  void submitFeedCommand(uint16_t foodID[7], uint16_t potionID[6]);
  void submitEquipCommand(uint16_t weaponID, uint16_t dressID);
  bool scanForCharacterCreationCommand();
  bool scanForCommands();
  bool static sortCommand(Command i, Command j);
  void processCommand();

  std::string convertCommandToHex(const CommandProcessor::Command& commandPackage);
  Command convertHexToCommand(const std::string& hexadecimalString);
  static std::string convertHexToString(const std::string& hexadecimalString);
  static std::string convertStringToHex(const std::string& textString);
  std::string convertInt8ToHex(uint8_t integer);
  template<typename INT_T> std::string convertIntToHex(INT_T integer);

  std::string getBlockHash();

private:
  DaemonAPI& mDaemonAPI;
  WalletAPI& mWalletAPI;
  Character& mCharacter;
  World& mWorld;

  uint8_t lookupItemTable(World::ItemType itemType);
   
  uint64_t mCurrentScanHeight;
  std::queue<std::pair<uint64_t,Command>> mCommandQueue;
  uint64_t mLastTimeResyncRequest;
  std::string mWalletAddress;
  uint64_t mTxAmount;
  uint16_t mTxPriority;
  uint16_t mMixin;
  bool mIsBetaVersion;
  bool mIsCharacterLoaded;
};

template<typename INT_T>
std::string CommandProcessor::convertIntToHex(INT_T integer)
{
  if (sizeof(INT_T) == 1)
  {
    // See CommandProcessor::convertInt8ToHex
    mWorld.logging.writeToFile("Error: unexpected hex conversion");
    return "";
  }
  else
  {
    std::stringstream stream;
    stream << std::setfill('0') << std::setw(sizeof(INT_T) * 2) << std::hex << integer;
    return stream.str();
  }
}