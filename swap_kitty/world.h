#pragma once

#include "siphashrng.h"
#include "daemonapi.h"
#include "walletapi.h"


class World
{
public:
  World();
  ~World();
  bool init(std::string daemonHost, uint16_t daemonPort, uint16_t walletPort);

  enum class Attribute { none, fire, water, earth, air, lightning, machine, poison, chaos, holy, dark };

  SiphashRNG siphashRNG;
  DaemonAPI daemonAPI;
  WalletAPI walletAPI;
};