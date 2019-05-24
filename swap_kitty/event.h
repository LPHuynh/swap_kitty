#pragma once

#include "character.h"
#include "daemonapi.h"
#include "walletapi.h"
#include "world.h"


class Event
{
public:
  Event(World& world, Character& character, DaemonAPI& daemonAPI);
  ~Event();

  struct Time
  {
    uint16_t year;
    uint16_t day;
    uint16_t hour;
    uint16_t minute;
  };

  void processEvent();

  Time time;

private:
  DaemonAPI& mDaemonAPI;
  World& mWorld;
  Character& mCharacter;

  void updateTime();
  void processDailyEvent();
  void processHourlyEvent();
  void processTenthHourlyEvent(const std::string& seed);
  std::string getBlockHash();
};

