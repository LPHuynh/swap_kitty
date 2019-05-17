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

  enum Element { none, fire, water, earth, air, lightning, holy, dark, machine, poison, chaos };

  struct Dice
  {
    int16_t roll;
    int16_t face;
    int16_t plus;
  };
  struct Stat
  {
    int16_t Str;
    int16_t Con;
    int16_t Dex;
    int16_t Per;
    int16_t Lrn;
    int16_t Wil;
    int16_t Mag;
    int16_t Chr;
    int16_t Acc;
    int16_t Cri;
  };

  World::Element randomizeElement(const std::string& seed);
  World::Element randomizePlayerElement(const std::string& seed);

  int16_t elementAttackBonus(Element attackerElement, std::vector<Element> defenderElement);

  static Dice addDice(const std::vector<Dice>& die);
  static Stat addStat(const std::vector<Stat>& stats);

  DaemonAPI daemonAPI;
  WalletAPI walletAPI;

  uint16_t currentRulesetVersion;

private:
  SiphashRNG mSiphashRNG;
};