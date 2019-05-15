#pragma once

#include <string>
#include "curl_header.h"
#include "json.hpp"


class DaemonAPI
{
public:
  DaemonAPI(std::string daemonHost, uint16_t daemonPort);
  ~DaemonAPI();

  int64_t getBlockCount();
  std::string getBlockHash(int64_t height);

private:
  std::string mDaemonJsonHttp;
  curl::curl_header mHeader;
};

