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

  struct SyncStatus
  {
    uint64_t height;
    uint64_t targetHeight;
  };

  uint64_t getBlockCount();
  SyncStatus getSyncInfo();
  std::string getBlockHash(uint64_t height);

private:
  std::string mDaemonJsonHttp;
  curl::curl_header mHeader;
};

