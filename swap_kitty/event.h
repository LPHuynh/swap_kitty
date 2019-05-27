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
  void init();

  struct Time
  {
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t quarterminute;
  };

  void processEvent();

  Time time;

private:
  DaemonAPI& mDaemonAPI;
  World& mWorld;
  Character& mCharacter;

  void incrementTime();
  void processDailyEvent(const std::string& seed);
  void processHourlyEvent(const std::string& seed);
  void processTenthHourlyEvent(const std::string& seed);
  void updateTime();
  void updateCheckedStat();
  std::string getBlockHash();

  uint16_t mEffectiveStat[11];
  uint16_t mEffectiveSkill[30];
  uint64_t mEarning;
  uint16_t mCookingProgress;
  uint16_t mFishingProgress;
  uint16_t mGatheringProgress;
  uint16_t mHuntingProgress;
  
  uint16_t mHeatStrokeCoolDown;
  uint16_t mCommonColdCoolDown;
  std::vector<std::string> mFoundItem;
  World::Stat mTotalActivityStatGained;
};

