#pragma once

#include "character.h"
#include "daemonapi.h"
#include "walletapi.h"
#include "world.h"

class CommandProcessor
{
public:
  CommandProcessor();
  ~CommandProcessor();
  void init(DaemonAPI& daemonAPI, WalletAPI& walletAPI, World& world, Character& character, const std::string& signingKey, uint16_t txPriority, uint16_t mixin, uint64_t startingScanHeight, bool isBetaVersion);

  struct Command
  {
    uint16_t rulesetVersion;
    std::string commandCode;
    std::string param;
  };

  void submitCharacterCreationCommand(const std::string& characterName);
  void submitResyncGameClock(uint16_t offset);
  void submitAssignScheduleCommand(uint16_t activity[12]);
  void submitAssignBookCommand(uint16_t ID[12]);
  void submitUseItemCommand(World::ItemAction itemAction, World::ItemType itemType[8], uint16_t ID[8]);
  bool scanForCharacterCreationCommand();
  void scanForCommands();
  bool static sortCommand(Command i, Command j);
  void processCommand();

  std::string convertCommandToHex(const CommandProcessor::Command& commandPackage);
  Command convertHexToCommand(const std::string& hexadecimalString);
  static std::string convertHexToString(const std::string& hexadecimalString);
  static std::string convertStringToHex(const std::string& textString);
  template<typename INT_T> static std::string convertIntToHex(INT_T integer);

private:
  DaemonAPI mDaemonAPI;
  WalletAPI mWalletAPI;
  Character mCharacter;
  World mWorld;

  uint8_t lookupItemTable(World::ItemType itemType);
   
  uint64_t mCurrentScanHeight;
  std::queue<std::pair<uint64_t,Command>> mCommandQueue;
  uint64_t mLastTimeResyncRequest;
  std::string mWalletAddress;
  uint64_t mTxAmount;
  uint16_t mTxPriority;
  uint16_t mMixin;
  std::string mSigningKey;
  bool mIsBetaVersion;
  bool mIsCharacterLoaded;
};