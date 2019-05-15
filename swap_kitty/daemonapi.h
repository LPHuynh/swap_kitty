#pragma once

#include <string>
#include "curl_header.h"
#include "json.hpp"


class DaemonAPI
{
public:
  DaemonAPI();
  ~DaemonAPI();
  bool init(std::string daemonHost, uint16_t daemonPort);

  int64_t getBlockCount();
  std::string getBlockHash(uint64_t height);

private:
  std::string mDaemonJsonHttp;
  curl::curl_header mHeader;
};

