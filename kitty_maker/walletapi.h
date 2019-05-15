#pragma once

#include <string>
#include "curl_header.h"
#include "json.hpp"


class WalletAPI
{
public:
  WalletAPI(std::string daemonHost, uint16_t daemonPort, uint16_t walletPort);
  ~WalletAPI();

  bool createWallet(std::string walletName, std::string password, std::string language);
  bool openWallet(std::string walletName, std::string password);
  bool restoreWallet(std::string walletName, std::string password, std::string seed, std::string language, uint64_t restoreHeight);
  std::string getMnemonicSeed();
  bool stopWallet();
  bool closeWallet();

  std::string getAddress();
  int64_t getBlockHeight();
  int64_t getBalance(bool isReturnUnlocked);

private:
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  uint16_t mWalletPort;
  std::string mWalletJsonHttp;
  curl::curl_header mHeader;
};

