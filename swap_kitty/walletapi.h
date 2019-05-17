#pragma once

#include <string>
#include "curl_header.h"
#include "json.hpp"


class WalletAPI
{
public:
  WalletAPI();
  ~WalletAPI();
  bool init(std::string daemonHost, uint16_t daemonPort, uint16_t walletPort);

  struct Balance
  {
    uint64_t totalBalance;
    uint64_t unlockedBalance;
  };

  struct WithdrawlReceipt
  {
    bool isSuccess;
    std::string txHash;
    uint64_t txFee;
  };

  bool createWallet(std::string walletName, std::string password, std::string language);
  bool openWallet(std::string walletName, std::string password);
  bool restoreWallet(std::string walletName, std::string password, std::string seed, std::string language, uint64_t restoreHeight);
  std::string getMnemonicSeed();
  bool stopWallet();
  bool closeWallet();

  std::string getAddress();
  int64_t getBlockHeight();
  Balance getBalance();
  WithdrawlReceipt transfer(std::string walletAddress, std::string paymentID, uint64_t amount, uint16_t priority, uint16_t mixin);
  WithdrawlReceipt sweepAll(std::string walletAddress, std::string paymentID, uint16_t priority, uint16_t mixin);

private:
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  uint16_t mWalletPort;
  std::string mWalletJsonHttp;
  curl::curl_header mHeader;
};

