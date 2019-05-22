#include "job.h"


Job::Job()
{
  loadActivity();
}

Job::~Job()
{
}

uint8_t Job::getActivityID(const std::string& name)
{
  for (auto& element : mActivity)
  {
    if (element.name == name)
    {
      return element.id;
    }
  }
  return 0;
}

void Job::loadActivity()
{
  Activity prototype;

  prototype.name =  "";
  prototype.isDayActivity = true;
  prototype.isNightActivity = false;
  prototype.payment = 0;
  prototype.activityType = ActivityType::domestic;
  prototype.workLevel = 0;
  prototype.workStat = World::Stat{ 0,0,0,0,0,0,0,0,0,0 };
  prototype.workSkill = World::Skill{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
  prototype.staminaUsed = 0;
  prototype.quenchUsed = 0;

  Activity activity;

  // Ruleset v1: activity slot 0 - 23
  activity = prototype;
  activity.name = "Cooking";
  activity.isNightActivity = true;
  activity.payment = 0;
  activity.activityType = ActivityType::domestic;
  activity.workLevel = 1;
  activity.workStat.Dex = 40;
  activity.workStat.Lrn = 40;
  activity.workSkill.cooking = 40;
  activity.workSkill.sword = 40;
  prototype.staminaUsed = 10;
  prototype.quenchUsed = 5;
  mActivity.push_back(activity);

  activity = prototype;
  activity.name = "Cleaning";
  activity.isNightActivity = true;
  activity.payment = 1;
  activity.activityType = ActivityType::domestic;
  activity.workLevel = 1;
  activity.workStat.Dex = 40;
  activity.workStat.Lrn = 40;
  activity.workSkill.cleaning = 40;
  activity.workSkill.stave = 40;
  activity.staminaUsed = 10;
  activity.quenchUsed = 5;
  mActivity.push_back(activity);

  activity = prototype;
  activity.name = "Read Book";
  activity.isNightActivity = true;
  activity.payment = 0;
  activity.activityType = ActivityType::domestic;
  activity.workLevel = 0;
  activity.workStat.Wil = 40;
  activity.workStat.Lrn = 40;
  activity.workSkill.literacy = 60;
  activity.staminaUsed = 5;
  activity.quenchUsed = 5;
  mActivity.push_back(activity);

  activity = prototype;
  activity.name = "Play";
  activity.isNightActivity = true;
  activity.payment = 0;
  activity.activityType = ActivityType::domestic;
  activity.workLevel = 0;
  activity.workStat.Con = 40;
  activity.workStat.Dex = 40;
  activity.workSkill.art = 30;
  activity.workSkill.music = 30;
  activity.staminaUsed = 10;
  activity.quenchUsed = 10;
  mActivity.push_back(activity);

  activity = prototype;
  activity.name = "Bath";
  activity.isNightActivity = true;
  activity.payment = 0;
  activity.activityType = ActivityType::domestic;
  activity.workLevel = 0;
  activity.workStat.Wil = 0;
  activity.staminaUsed = 5;
  activity.quenchUsed = 5;
  mActivity.push_back(activity);

  activity = prototype;
  activity.name = "Nap";
  activity.payment = 0;
  activity.activityType = ActivityType::domestic;
  activity.workLevel = 0;
  activity.workStat.Wil = 60;
  activity.staminaUsed = 5;
  activity.quenchUsed = 5;
  mActivity.push_back(activity);

  activity = prototype;
  activity.name = "Sleep";
  activity.isNightActivity = true;
  activity.payment = 0;
  activity.activityType = ActivityType::domestic;
  activity.workLevel = 0;
  activity.workStat.Wil = 40;
  activity.staminaUsed = 5;
  activity.quenchUsed = 5;
  mActivity.push_back(activity);

  activity = prototype;
  activity.name = "Waitress";
  activity.payment = 120;
  activity.workLevel = 2;
  activity.activityType = ActivityType::domestic;
  activity.workStat.Chr = 60;
  activity.workStat.Lrn = 40;
  activity.workSkill.cleaning = 40;
  activity.workSkill.service = 60;
  activity.staminaUsed = 15;
  activity.quenchUsed = 10;
  mActivity.push_back(activity);

  activity = prototype;
  activity.name = "Bar Tending";
  activity.isDayActivity = false;
  activity.isNightActivity = true;
  activity.payment = 140;
  activity.workLevel = 2;
  activity.activityType = ActivityType::domestic;
  activity.workStat.Chr = 40;
  activity.workStat.Lrn = 70;
  activity.workSkill.cleaning = 30;
  activity.workSkill.service = 60;
  activity.staminaUsed = 10;
  activity.quenchUsed = 5;
  mActivity.push_back(activity);

  activity = prototype;
  activity.name = "Maid";
  activity.payment = 200;
  activity.workLevel = 3;
  activity.activityType = ActivityType::domestic;
  activity.workStat.Chr = 50;
  activity.workStat.Lrn = 80;
  activity.workSkill.cleaning = 80;
  activity.workSkill.service = 70;
  activity.staminaUsed = 15;
  activity.quenchUsed = 10;
  mActivity.push_back(activity);

  activity = prototype;
  activity.name = "Street Artist";
  activity.payment = 80;
  activity.workLevel = 1;
  activity.activityType = ActivityType::entertainment;
  activity.workStat.Chr = 40;
  activity.workStat.Lrn = 40;
  activity.workSkill.art = 60;
  activity.staminaUsed = 10;
  activity.quenchUsed = 15;
  mActivity.push_back(activity);

  activity = prototype;
  activity.name = "Dancer";
  activity.isNightActivity = true;
  activity.payment = 160;
  activity.workLevel = 2;
  activity.activityType = ActivityType::entertainment;
  activity.workStat.Chr = 60;
  activity.workStat.Dex = 70;
  activity.workSkill.art = 70;
  activity.workSkill.music = 60;
  activity.staminaUsed = 15;
  activity.quenchUsed = 15;
  mActivity.push_back(activity);

  activity = prototype;
  activity.name = "Magician";
  activity.isNightActivity = true;
  activity.payment = 190;
  activity.workLevel = 2;
  activity.activityType = ActivityType::entertainment;
  activity.workStat.Chr = 50;
  activity.workStat.Mag = 70;
  activity.workSkill.stave = 50;
  activity.workSkill.art = 70;
  activity.staminaUsed = 20;
  activity.quenchUsed = 15;
  mActivity.push_back(activity);

  activity = prototype;
  activity.name = "Singer";
  activity.isNightActivity = true;
  activity.payment = 250;
  activity.workLevel = 3;
  activity.activityType = ActivityType::entertainment;
  activity.workStat.Chr = 90;
  activity.workStat.Dex = 70;
  activity.workSkill.art = 80;
  activity.workSkill.music = 90;
  activity.staminaUsed = 20;
  activity.quenchUsed = 15;
  mActivity.push_back(activity);

  activity = prototype;
  activity.name = "Fishing";
  activity.payment = 0;
  activity.workLevel = 1;
  activity.activityType = ActivityType::agriculture;
  activity.workStat.Per = 40;
  activity.workStat.Dex = 40;
  activity.workSkill.fishing = 80;
  activity.workSkill.water = 40;
  activity.staminaUsed = 15;
  activity.quenchUsed = 25;
  mActivity.push_back(activity);

  activity = prototype;
  activity.name = "Gathering";
  activity.payment = 0;
  activity.workLevel = 1;
  activity.activityType = ActivityType::agriculture;
  activity.workStat.Per = 40;
  activity.workStat.Lrn = 40;
  activity.workSkill.farming = 40;
  activity.workSkill.earth = 20;
  activity.staminaUsed = 15;
  activity.quenchUsed = 20;
  mActivity.push_back(activity);

  activity = prototype;
  activity.name = "Industrial Fishing";
  activity.payment = 150;
  activity.workLevel = 2;
  activity.activityType = ActivityType::agriculture;
  activity.workStat.Str = 50;
  activity.workStat.Dex = 70;
  activity.workSkill.fishing = 60;
  activity.workSkill.water = 20;
  activity.staminaUsed = 20;
  activity.quenchUsed = 25;
  mActivity.push_back(activity);

  activity = prototype;
  activity.name = "Farm";
  activity.payment = 150;
  activity.workLevel = 2;
  activity.activityType = ActivityType::agriculture;
  activity.workStat.Str = 50;
  activity.workStat.Con = 70;
  activity.workSkill.farming = 60;
  activity.workSkill.earth = 20;
  activity.staminaUsed = 25;
  activity.quenchUsed = 25;
  mActivity.push_back(activity);

  activity = prototype;
  activity.name = "Butcher";
  activity.payment = 140;
  activity.workLevel = 2;
  activity.activityType = ActivityType::agriculture;
  activity.workStat.Lrn = 50;
  activity.workStat.Wil = 70;
  activity.workSkill.sword = 60;
  activity.workSkill.choas = 20;
  activity.staminaUsed = 20;
  activity.quenchUsed = 15;
  mActivity.push_back(activity);

  activity = prototype;
  activity.name = "Hunting";
  activity.payment = 200;
  activity.workLevel = 3;
  activity.activityType = ActivityType::agriculture;
  activity.workStat.Per = 90;
  activity.workStat.Wil = 90;
  activity.workSkill.choas = 40;
  activity.workSkill.evasion = 80;
  activity.staminaUsed = 20;
  activity.quenchUsed = 15;
  mActivity.push_back(activity);

  activity = prototype;
  activity.name = "Wood Cutting";
  activity.payment = 170;
  activity.workLevel = 1;
  activity.activityType = ActivityType::heavyIndustry;
  activity.workStat.Str = 60;
  activity.workStat.Con = 60;
  activity.workSkill.axe = 70;
  activity.workSkill.woodWorking = 70;
  activity.staminaUsed = 15;
  activity.quenchUsed = 15;
  mActivity.push_back(activity);

  activity = prototype;
  activity.name = "Mining";
  activity.payment = 270;
  activity.workLevel = 1;
  activity.activityType = ActivityType::heavyIndustry;
  activity.workStat.Str = 60;
  activity.workStat.Con = 80;
  activity.workSkill.earth = 70;
  activity.workSkill.stoneWorking = 70;
  activity.staminaUsed = 30;
  activity.quenchUsed = 20;
  mActivity.push_back(activity);

  activity = prototype;
  activity.name = "Black Smithing";
  activity.payment = 210;
  activity.workLevel = 2;
  activity.activityType = ActivityType::heavyIndustry;
  activity.workStat.Str = 70;
  activity.workStat.Con = 60;
  activity.workSkill.fire = 60;
  activity.workSkill.metalworking = 70;
  activity.staminaUsed = 25;
  activity.quenchUsed = 25;
  mActivity.push_back(activity);

  activity = prototype;
  activity.name = "Ship Wright";
  activity.payment = 220;
  activity.workLevel = 2;
  activity.activityType = ActivityType::heavyIndustry;
  activity.workStat.Str = 50;
  activity.workStat.Dex = 60;
  activity.workSkill.bludgeon = 70;
  activity.workSkill.woodWorking = 70;
  activity.staminaUsed = 25;
  activity.quenchUsed = 25;
  mActivity.push_back(activity);

  activity = prototype;
  activity.name = "Iron Smelting";
  activity.payment = 280;
  activity.workLevel = 3;
  activity.activityType = ActivityType::heavyIndustry;
  activity.workStat.Str = 50;
  activity.workStat.Dex = 50;
  activity.workSkill.fire = 50;
  activity.workSkill.metalworking = 80;
  activity.staminaUsed = 25;
  activity.quenchUsed = 25;
  mActivity.push_back(activity);

  uint8_t i = 0;
  for (auto& element : mActivity)
  {
    element.id = i;
    i++;
  }
}
