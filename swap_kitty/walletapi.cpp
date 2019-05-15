#include "walletapi.h"

#include "curl_easy.h"
#include "curl_exception.h"
#include "windows.h"


WalletAPI::WalletAPI(std::string daemonHost, uint16_t daemonPort, uint16_t walletPort)
{
  mWalletPort = walletPort;
  mWalletJsonHttp = "http://127.0.0.1:" + std::to_string(walletPort) + "/json_rpc";
  mHeader.add("Content-Type: application/json");

  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));

  std::string startRPCWalletCommand = "swap\\swap-wallet-rpc.exe --daemon-address " + daemonHost + ":" + std::to_string(daemonPort) + " --rpc-bind-port " + std::to_string(mWalletPort) + " --disable-rpc-login --wallet-dir swap\\wallet";
  char command[1024];
  strncpy_s(command, startRPCWalletCommand.c_str(), sizeof(command));
  command[sizeof(command) - 1] = 0;

  // Start the RPC Wallet
  if (!CreateProcess(NULL, command, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
  {
    printf("CreateProcess failed (%d).\n", GetLastError());
  }
}

WalletAPI::~WalletAPI()
{
  // Close the RPC Wallet
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
}

bool WalletAPI::createWallet(std::string walletName, std::string password, std::string language)
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

bool WalletAPI::openWallet(std::string walletName, std::string password)
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

bool WalletAPI::restoreWallet(std::string walletName, std::string password, std::string seed, std::string language, uint64_t restoreHeight)
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

  return httpReponse["result"]["key"];
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

  return httpReponse["result"]["address"];
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

int64_t WalletAPI::getBalance(bool isReturnUnlocked)
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
  }

  nlohmann::json httpReponse;
  httpReponse = nlohmann::json::parse(str.str());

  if (isReturnUnlocked)
  {
    return httpReponse["result"]["unlocked_balance"];
  }

  return httpReponse["result"]["balance"];
}