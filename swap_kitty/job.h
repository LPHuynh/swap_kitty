#pragma once

#include "world.h"


class Job
{
public:
  Job();
  ~Job();

  enum class ActivityType { domestic, entertainment, agriculture, heavyIndustry};

  struct Activity
  {
    std::string name;
    uint8_t id;
    bool isDayActivity;
    bool isNightActivity;
    int16_t payment;
    ActivityType activityType;
    int16_t workLevel;
    World::Stat workStat;
    World::Skill workSkill;
    int16_t staminaUsed;
    int16_t quenchUsed;
  };

  uint8_t getActivityID(const std::string& name);

private:
  void loadActivity();

  std::vector<Activity> mActivity;
};

