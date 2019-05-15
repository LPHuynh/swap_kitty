#include "daemonapi.h"

#include "curl_easy.h"
#include "curl_exception.h"
#include "windows.h"


DaemonAPI::DaemonAPI()
{
  mDaemonJsonHttp = "http://127.0.0.1:19950/json_rpc";
  mHeader.add("Content-Type: application/json");
}

DaemonAPI::~DaemonAPI()
{
}

bool DaemonAPI::init(std::string daemonHost, uint16_t daemonPort)
{
  mDaemonJsonHttp = "http://" + daemonHost + ":" + std::to_string(daemonPort) + "/json_rpc";
  return true;
}

int64_t DaemonAPI::getBlockCount()
{
  std::ostringstream str;
  curl::curl_ios<std::ostringstream> writer(str);
  curl::curl_easy easy(writer);

  nlohmann::json httpPost;
  httpPost["id"] = "0";
  httpPost["jsonrpc"] = "2.0";
  httpPost["method"] = "get_block_count";

  easy.add(curl::curl_pair<CURLoption, std::string>(CURLOPT_URL, mDaemonJsonHttp));
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

  return httpReponse["result"]["count"];
}

std::string DaemonAPI::getBlockHash(uint64_t height)
{
  std::ostringstream str;
  curl::curl_ios<std::ostringstream> writer(str);
  curl::curl_easy easy(writer);

  nlohmann::json httpPost;
  httpPost["id"] = "0";
  httpPost["jsonrpc"] = "2.0";
  httpPost["method"] = "on_get_block_hash";
  httpPost["params"] = { height };

  easy.add(curl::curl_pair<CURLoption, std::string>(CURLOPT_URL, mDaemonJsonHttp));
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

  return httpReponse["result"];
}