#pragma once

#include "world.h"


class Job
{
public:
  Job();
  ~Job();

  enum class ActivityType { self, domestic, entertainment, agriculture, heavyIndustry};

  struct Activity
  {
    std::string name;
    uint8_t id;
    bool isDayActivity;
    bool isNightActivity;
    bool isOutdoorActivity;
    bool isNearHeatSource;
    int16_t payment;
    ActivityType activityType;
    int16_t workLevel;
    World::Stat workStat;
    World::Skill workSkill;
    int16_t staminaUsed;
    int16_t quenchUsed;
  };

  Activity getActivity(const std::string& name);
  Activity getActivity(uint8_t id);

private:
  void loadActivity();

  std::vector<Activity> mActivity;
};

