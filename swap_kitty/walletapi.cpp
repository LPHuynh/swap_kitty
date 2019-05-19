#include "walletapi.h"

#include <chrono>
#include <thread>
#include "curl_easy.h"
#include "curl_exception.h"
#include "windows.h"


WalletAPI::WalletAPI()
{
  mHeader.add("Content-Type: application/json");

  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));
}

WalletAPI::~WalletAPI()
{
  // Close the RPC Wallet
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
}

bool WalletAPI::init(const std::string& daemonHost, uint16_t daemonPort, uint16_t walletPort)
{
  mWalletPort = walletPort;
  mWalletJsonHttp = "http://127.0.0.1:" + std::to_string(walletPort) + "/json_rpc";

  std::string startRPCWalletCommand = "swap\\swap-wallet-rpc.exe --daemon-address " + daemonHost + ":" + std::to_string(daemonPort) + " --rpc-bind-port " + std::to_string(mWalletPort) + " --disable-rpc-login --wallet-dir swap\\wallet";
  char command[1024];
  strncpy_s(command, startRPCWalletCommand.c_str(), sizeof(command));
  command[sizeof(command) - 1] = 0;

  // Start the RPC Wallet
  if (!CreateProcess(NULL, command, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
  {
    printf("CreateProcess failed (%d).\n", GetLastError());
    return false;
  }

  std::this_thread::sleep_for(std::chrono::seconds(5)); // The wallet takes a few second before it can accept any rpc commands

  return true;
}

bool WalletAPI::createWallet(const std::string& walletName, const std::string& password, const std::string& language)
{
  std::ostringstream str;
  curl::curl_ios<std::ostringstream> writer(str);
  curl::curl_easy easy(writer);

  nlohmann::json httpPost;
  httpPost["id"] = "0";
  httpPost["jsonrpc"] = "2.0";
  httpPost["method"] = "create_wallet";
  httpPost["params"]["filename"] = walletName;
  httpPost["params"]["password"] = password;
  httpPost["params"]["language"] = language;

  easy.add(curl::curl_pair<CURLoption, std::string>(CURLOPT_URL, mWalletJsonHttp));
  easy.add(curl::curl_pair<CURLoption, curl::curl_header>(CURLOPT_HTTPHEADER, mHeader));
  easy.add<CURLOPT_SSL_VERIFYPEER>(false);
  easy.add(curl::curl_pair<CURLoption, std::string>(CURLOPT_POSTFIELDS, httpPost.dump()));

  try
  {
    easy.perform();
  }
  catch (curl::curl_easy_exception error)
  {
    curl::curlcpp_traceback errors = error.get_traceback();
    error.print_traceback();
    return false;
  }

  return true;
}

bool WalletAPI::openWallet(const std::string& walletName, const std::string& password)
{
  std::ostringstream str;
  curl::curl_ios<std::ostringstream> writer(str);
  curl::curl_easy easy(writer);

  nlohmann::json httpPost;
  httpPost["id"] = "0";
  httpPost["jsonrpc"] = "2.0";
  httpPost["method"] = "open_wallet";
  httpPost["params"]["filename"] = walletName;
  httpPost["params"]["password"] = password;

  easy.add(curl::curl_pair<CURLoption, std::string>(CURLOPT_URL, mWalletJsonHttp));
  easy.add(curl::curl_pair<CURLoption, curl::curl_header>(CURLOPT_HTTPHEADER, mHeader));
  easy.add<CURLOPT_SSL_VERIFYPEER>(false);
  easy.add(curl::curl_pair<CURLoption, std::string>(CURLOPT_POSTFIELDS, httpPost.dump()));

  try
  {
    easy.perform();
  }
  catch (curl::curl_easy_exception error)
  {
    curl::curlcpp_traceback errors = error.get_traceback();
    error.print_traceback();
    return false;
  }

  return true;
}

bool WalletAPI::restoreWallet(const std::string& walletName, const std::string& password, const std::string& seed, const std::string& language, uint64_t restoreHeight)
{
  std::ostringstream str;
  curl::curl_ios<std::ostringstream> writer(str);
  curl::curl_easy easy(writer);

  nlohmann::json httpPost;
  httpPost["id"] = "0";
  httpPost["jsonrpc"] = "2.0";
  httpPost["method"] = "restore_deterministic_wallet";
  httpPost["params"]["filename"] = walletName;
  httpPost["params"]["password"] = password;
  httpPost["params"]["seed"] = seed;
  httpPost["params"]["seed_offset"] = 0;
  httpPost["params"]["language"] = language;
  httpPost["params"]["restore_height"] = restoreHeight;

  easy.add(curl::curl_pair<CURLoption, std::string>(CURLOPT_URL, mWalletJsonHttp));
  easy.add(curl::curl_pair<CURLoption, curl::curl_header>(CURLOPT_HTTPHEADER, mHeader));
  easy.add<CURLOPT_SSL_VERIFYPEER>(false);
  easy.add(curl::curl_pair<CURLoption, std::string>(CURLOPT_POSTFIELDS, httpPost.dump()));

  try
  {
    easy.perform();
  }
  catch (curl::curl_easy_exception error)
  {
    curl::curlcpp_traceback errors = error.get_traceback();
    error.print_traceback();
    return false;
  }

  return true;
}

std::string WalletAPI::getMnemonicSeed()
{
  std::ostringstream str;
  curl::curl_ios<std::ostringstream> writer(str);
  curl::curl_easy easy(writer);

  nlohmann::json httpPost;
  httpPost["id"] = "0";
  httpPost["jsonrpc"] = "2.0";
  httpPost["method"] = "query_key";
  httpPost["params"]["key_type"] = "mnemonic";

  easy.add(curl::curl_pair<CURLoption, std::string>(CURLOPT_URL, mWalletJsonHttp));
  easy.add(curl::curl_pair<CURLoption, curl::curl_header>(CURLOPT_HTTPHEADER, mHeader));
  easy.add<CURLOPT_SSL_VERIFYPEER>(false);
  easy.add(curl::curl_pair<CURLoption, std::string>(CURLOPT_POSTFIELDS, httpPost.dump()));

  try
  {
    easy.perform();
  }
  catch (curl::curl_easy_exception error)
  {
    curl::curlcpp_traceback errors = error.get_traceback();
    error.print_traceback();
    return "";
  }

  nlohmann::json httpReponse;
  httpReponse = nlohmann::json::parse(str.str());

  return httpReponse["result"]["key"].get<std::string>();
}

bool WalletAPI::stopWallet()
{
  std::ostringstream str;
  curl::curl_ios<std::ostringstream> writer(str);
  curl::curl_easy easy(writer);

  nlohmann::json httpPost;
  httpPost["id"] = "0";
  httpPost["jsonrpc"] = "2.0";
  httpPost["method"] = "stop_wallet";

  easy.add(curl::curl_pair<CURLoption, std::string>(CURLOPT_URL, mWalletJsonHttp));
  easy.add(curl::curl_pair<CURLoption, curl::curl_header>(CURLOPT_HTTPHEADER, mHeader));
  easy.add<CURLOPT_SSL_VERIFYPEER>(false);
  easy.add(curl::curl_pair<CURLoption, std::string>(CURLOPT_POSTFIELDS, httpPost.dump()));

  try
  {
    easy.perform();
  }
  catch (curl::curl_easy_exception error)
  {
    curl::curlcpp_traceback errors = error.get_traceback();
    error.print_traceback();
    return false;
  }

  return true;
}

bool WalletAPI::closeWallet()
{
  std::ostringstream str;
  curl::curl_ios<std::ostringstream> writer(str);
  curl::curl_easy easy(writer);

  nlohmann::json httpPost;
  httpPost["id"] = "0";
  httpPost["jsonrpc"] = "2.0";
  httpPost["method"] = "close_wallet";

  easy.add(curl::curl_pair<CURLoption, std::string>(CURLOPT_URL, mWalletJsonHttp));
  easy.add(curl::curl_pair<CURLoption, curl::curl_header>(CURLOPT_HTTPHEADER, mHeader));
  easy.add<CURLOPT_SSL_VERIFYPEER>(false);
  easy.add(curl::curl_pair<CURLoption, std::string>(CURLOPT_POSTFIELDS, httpPost.dump()));

  try
  {
    easy.perform();
  }
  catch (curl::curl_easy_exception error)
  {
    curl::curlcpp_traceback errors = error.get_traceback();
    error.print_traceback();
    return false;
  }

  return true;
}

std::string WalletAPI::getAddress()
{
  std::ostringstream str;
  curl::curl_ios<std::ostringstream> writer(str);
  curl::curl_easy easy(writer);

  nlohmann::json httpPost;
  httpPost["id"] = "0";
  httpPost["jsonrpc"] = "2.0";
  httpPost["method"] = "get_address";
  httpPost["params"]["account_index"] = 0;

  easy.add(curl::curl_pair<CURLoption, std::string>(CURLOPT_URL, mWalletJsonHttp));
  easy.add(curl::curl_pair<CURLoption, curl::curl_header>(CURLOPT_HTTPHEADER, mHeader));
  easy.add<CURLOPT_SSL_VERIFYPEER>(false);
  easy.add(curl::curl_pair<CURLoption, std::string>(CURLOPT_POSTFIELDS, httpPost.dump()));

  try
  {
    easy.perform();
  }
  catch (curl::curl_easy_exception error)
  {
    curl::curlcpp_traceback errors = error.get_traceback();
    error.print_traceback();
    return "";
  }

  nlohmann::json httpReponse;
  httpReponse = nlohmann::json::parse(str.str());

  return httpReponse["result"]["address"].get<std::string>();
}

int64_t WalletAPI::getBlockHeight()
{
  std::ostringstream str;
  curl::curl_ios<std::ostringstream> writer(str);
  curl::curl_easy easy(writer);

  nlohmann::json httpPost;
  httpPost["id"] = "0";
  httpPost["jsonrpc"] = "2.0";
  httpPost["method"] = "get_height";

  easy.add(curl::curl_pair<CURLoption, std::string>(CURLOPT_URL, mWalletJsonHttp));
  easy.add(curl::curl_pair<CURLoption, curl::curl_header>(CURLOPT_HTTPHEADER, mHeader));
  easy.add<CURLOPT_SSL_VERIFYPEER>(false);
  easy.add(curl::curl_pair<CURLoption, std::string>(CURLOPT_POSTFIELDS, httpPost.dump()));

  try
  {
    easy.perform();
  }
  catch (curl::curl_easy_exception error)
  {
    curl::curlcpp_traceback errors = error.get_traceback();
    error.print_traceback();
    return 0;
  }

  nlohmann::json httpReponse;
  httpReponse = nlohmann::json::parse(str.str());
  return httpReponse["result"]["height"];
}

WalletAPI::Balance WalletAPI::getBalance()
{
  std::ostringstream str;
  curl::curl_ios<std::ostringstream> writer(str);
  curl::curl_easy easy(writer);

  nlohmann::json httpPost;
  httpPost["id"] = "0";
  httpPost["jsonrpc"] = "2.0";
  httpPost["method"] = "get_balance";

  easy.add(curl::curl_pair<CURLoption, std::string>(CURLOPT_URL, mWalletJsonHttp));
  easy.add(curl::curl_pair<CURLoption, curl::curl_header>(CURLOPT_HTTPHEADER, mHeader));
  easy.add<CURLOPT_SSL_VERIFYPEER>(false);
  easy.add(curl::curl_pair<CURLoption, std::string>(CURLOPT_POSTFIELDS, httpPost.dump()));

  try
  {
    easy.perform();
  }
  catch (curl::curl_easy_exception error)
  {
    curl::curlcpp_traceback errors = error.get_traceback();
    error.print_traceback();
    Balance balance;
    balance.totalBalance = 0;
    balance.unlockedBalance = 0;
  }

  nlohmann::json httpReponse;
  httpReponse = nlohmann::json::parse(str.str());

  Balance balance;
  balance.totalBalance = httpReponse["result"]["balance"];
  balance.unlockedBalance = httpReponse["result"]["unlocked_balance"];

  return balance;
}

WalletAPI::WithdrawlReceipt WalletAPI::transfer(const std::string& walletAddress, const std::string& paymentID, uint64_t amount, uint16_t priority, uint16_t mixin)
{
  std::ostringstream str;
  curl::curl_ios<std::ostringstream> writer(str);
  curl::curl_easy easy(writer);

  nlohmann::json destination;
  destination["address"] = walletAddress;
  destination["amount"] = amount;

  nlohmann::json httpPost;
  httpPost["id"] = "0";
  httpPost["jsonrpc"] = "2.0";
  httpPost["method"] = "transfer";
  httpPost["params"]["destinations"] = { destination };
  httpPost["params"]["payment_id"] = paymentID;
  httpPost["params"]["mixin"] = mixin;
  httpPost["params"]["priority"] = priority;
  

  easy.add(curl::curl_pair<CURLoption, std::string>(CURLOPT_URL, mWalletJsonHttp));
  easy.add(curl::curl_pair<CURLoption, curl::curl_header>(CURLOPT_HTTPHEADER, mHeader));
  easy.add<CURLOPT_SSL_VERIFYPEER>(false);
  easy.add(curl::curl_pair<CURLoption, std::string>(CURLOPT_POSTFIELDS, httpPost.dump()));

  try
  {
    easy.perform();
  }
  catch (curl::curl_easy_exception error)
  {
    curl::curlcpp_traceback errors = error.get_traceback();
    error.print_traceback();
  }

  nlohmann::json httpReponse;
  httpReponse = nlohmann::json::parse(str.str());

  WalletAPI::WithdrawlReceipt receipt;

  if (!httpReponse["result"]["tx_hash"].is_null())
  {
    receipt.txHash = httpReponse["result"]["tx_hash"].get<std::string>();
    receipt.txFee = httpReponse["result"]["fee"];
    receipt.isSuccess = true;
  }
  else
  {
    receipt.txHash = "";
    receipt.txFee = 0;
    receipt.isSuccess = false;
  }

  return receipt;
}

WalletAPI::WithdrawlReceipt WalletAPI::sweepAll(const std::string& walletAddress, const std::string& paymentID, uint16_t priority, uint16_t mixin)
{
  std::ostringstream str;
  curl::curl_ios<std::ostringstream> writer(str);
  curl::curl_easy easy(writer);

  nlohmann::json httpPost;
  httpPost["id"] = "0";
  httpPost["jsonrpc"] = "2.0";
  httpPost["method"] = "sweep_all";
  httpPost["params"]["address "] = walletAddress;
  httpPost["params"]["payment_id"] = paymentID;
  httpPost["params"]["mixin"] = mixin;
  httpPost["params"]["priority"] = priority;

  easy.add(curl::curl_pair<CURLoption, std::string>(CURLOPT_URL, mWalletJsonHttp));
  easy.add(curl::curl_pair<CURLoption, curl::curl_header>(CURLOPT_HTTPHEADER, mHeader));
  easy.add<CURLOPT_SSL_VERIFYPEER>(false);
  easy.add(curl::curl_pair<CURLoption, std::string>(CURLOPT_POSTFIELDS, httpPost.dump()));

  try
  {
    easy.perform();
  }
  catch (curl::curl_easy_exception error)
  {
    curl::curlcpp_traceback errors = error.get_traceback();
    error.print_traceback();
  }

  nlohmann::json httpReponse;
  httpReponse = nlohmann::json::parse(str.str());

  WalletAPI::WithdrawlReceipt receipt;

  if (!httpReponse["result"]["tx_hash"].is_null())
  {
    receipt.txHash = httpReponse["result"]["tx_hash"].get<std::string>();
    receipt.txFee = httpReponse["result"]["fee"];
    receipt.isSuccess = true;
  }
  else
  {
    receipt.txHash = "";
    receipt.txFee = 0;
    receipt.isSuccess = false;
  }

  return receipt;
}