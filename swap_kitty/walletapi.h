#pragma once

#include <string>
#include "curl_header.h"
#include "json.hpp"


class WalletAPI
{
public:
  WalletAPI();
  ~WalletAPI();
  bool init(const std::string& daemonHost, uint16_t daemonPort, uint16_t walletPort);

  struct Balance
  {
    uint64_t totalBalance;
    uint64_t unlockedBalance;
  };

  struct TransferReceipt
  {
    bool isSuccess;
    std::string txHash;
    uint64_t txFee;
  };

  struct PaymentID
  {
    uint64_t height;
    std::string paymentID;
  };

  bool createWallet(const std::string& walletName, const std::string& password, const std::string& language);
  bool openWallet(const std::string& walletName, const std::string& password);
  bool restoreWallet(const std::string& walletName, const std::string& password, const std::string& seed, const std::string& language, uint64_t restoreHeight);
  std::string getMnemonicSeed();
  bool stopWallet();
  bool closeWallet();

  std::string getAddress();
  int64_t getBlockHeight();
  Balance getBalance();
  std::vector<PaymentID> getIncomingPaymentID(uint64_t minHeight, uint64_t maxHeight);

  TransferReceipt transfer(const std::string& walletAddress, const std::string& paymentID, uint64_t amount, uint16_t priority, uint16_t mixin);
  TransferReceipt sweepAll(const std::string& walletAddress, const std::string& paymentID, uint16_t priority, uint16_t mixin);

private:
#ifdef _WIN32
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
#endif
  uint16_t mWalletPort;
  std::string mWalletJsonHttp;
  curl::curl_header mHeader;
};

