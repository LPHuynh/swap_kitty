#pragma once

#include <string>
#include <vector>
#include "world.h"


class Job
{
public:
  Job();
  ~Job();

  enum ActivityType { domestic, entertainment, agriculture, heavyIndustry};

  struct Activity
  {
    std::string activityName;
    int16_t payment;
    ActivityType activityType;
    int16_t workLevel;
    World::Stat workStat;
    World::Skill workSkill;
    int16_t staminaUsed;
    int16_t quenchUsed;
  };

private:
  void loadActivity();

  std::vector<Activity> mActivity;
};

