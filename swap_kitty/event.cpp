#include "event.h"


Event::Event(World& world, Character& character, DaemonAPI& daemonAPI) : mWorld(world), mCharacter(character), mDaemonAPI(daemonAPI)
{
  time = { 0,0,0,0 };
}

Event::~Event()
{
}

void Event::processEvent()
{
  updateTime();
  if (time.hour == 0)
  {
    processDailyEvent();
  }
  if (time.minute == 0)
  {
    processHourlyEvent();
  }
  if (time.minute % 10 == 0)
  {
    processTenthHourlyEvent(getBlockHash());
  }
}

void Event::updateTime()
{
  time.year = uint16_t(mWorld.currentWorldHeight / uint64_t(691200));       //There are 120 in-game days per year (30 days per season)
  time.day = uint16_t(mWorld.currentWorldHeight % uint64_t(691200) / 5760);
  time.hour = (mWorld.currentWorldHeight % 5760) / 240;
  time.minute = (mWorld.currentWorldHeight % 240) / 4;
}

void Event::processDailyEvent()
{
  //Daily skill rust (~0.39% stat reduction for all stats and skills)
  mCharacter.stat = mCharacter.stat - mWorld.shiftStat(mCharacter.stat, -8);
  mCharacter.skill = mCharacter.skill - mWorld.shiftSkill(mCharacter.skill, -8);;

  //Daily skill gain from character's elemental
  switch (mCharacter.profile.primaryElement)
  {
  case World::Element::fire: mCharacter.skill.fire += 40; break;
  case World::Element::water: mCharacter.skill.water += 40; break;
  case World::Element::earth: mCharacter.skill.earth += 40; break;
  case World::Element::lightning: mCharacter.skill.lightning += 40; break;
  case World::Element::holy: mCharacter.skill.holy += 40; break;
  case World::Element::dark: mCharacter.skill.dark += 40; break;
  }
  switch (mCharacter.profile.secondaryElement)
  {
  case World::Element::fire: mCharacter.skill.fire += 20; break;
  case World::Element::water: mCharacter.skill.water += 20; break;
  case World::Element::earth: mCharacter.skill.earth += 20; break;
  case World::Element::lightning: mCharacter.skill.lightning += 20; break;
  case World::Element::holy: mCharacter.skill.holy += 20; break;
  case World::Element::dark: mCharacter.skill.dark += 20; break;
  }

  //Daily skill gain from character's dress
  mCharacter.stat = mCharacter.stat + mCharacter.equipedDress.dailyStatGain;
  mCharacter.skill = mCharacter.skill + mCharacter.equipedDress.dailySkillGain;

  //TODO: Special Events
}

void Event::processHourlyEvent()
{
  //Print Summary of previous job
  if (mCharacter.currentActivity.id == 0)
  {
    std::cout << mCharacter.profile.name << " finished cooking.";
  }
  else if (mCharacter.currentActivity.id == 1)
  {
    std::cout << mCharacter.profile.name << " finished cleaning the house.";
  }
  else
  {
    std::cout << mCharacter.profile.name << " finished working as a " << mCharacter.currentActivity.name;
  }

  if (mCharacter.favouriteActivityType == mCharacter.currentActivity.activityType)
  {
    std::cout << " She enjoyed it!";
  }
  std::cout << "\n";
  
  //Change job
  mCharacter.currentActivity = mCharacter.dailySchedule[time.hour];
}

void Event::processTenthHourlyEvent(const std::string& seed)
{
  //Process activity
  World::Stat effectiveStats = mCharacter.stat + mCharacter.equipedWeapon.bonusStat + mCharacter.equipedDress.bonusStat;
  World::Skill effectiveSkills = mCharacter.skill + mCharacter.equipedWeapon.bonusSkill + mCharacter.equipedDress.bonusSkill;

  if (mCharacter.favouriteActivityType == mCharacter.currentActivity.activityType)
  {
    mCharacter.profile.happiness++;
  }

  std::vector <std::pair<uint16_t, uint16_t>> statCheck;

  statCheck.push_back(std::make_pair(mCharacter.stat.str, mCharacter.currentActivity.workStat.str));
  statCheck.push_back(std::make_pair(mCharacter.stat.con, mCharacter.currentActivity.workStat.con));
  statCheck.push_back(std::make_pair(mCharacter.stat.dex, mCharacter.currentActivity.workStat.dex));
  statCheck.push_back(std::make_pair(mCharacter.stat.per, mCharacter.currentActivity.workStat.per));
  statCheck.push_back(std::make_pair(mCharacter.stat.lrn, mCharacter.currentActivity.workStat.lrn));
  statCheck.push_back(std::make_pair(mCharacter.stat.wil, mCharacter.currentActivity.workStat.wil));
  statCheck.push_back(std::make_pair(mCharacter.stat.mag, mCharacter.currentActivity.workStat.mag));
  statCheck.push_back(std::make_pair(mCharacter.stat.chr, mCharacter.currentActivity.workStat.chr));

  statCheck.push_back(std::make_pair(mCharacter.skill.literacy, mCharacter.currentActivity.workSkill.literacy));
  statCheck.push_back(std::make_pair(mCharacter.skill.cooking, mCharacter.currentActivity.workSkill.cooking));
  statCheck.push_back(std::make_pair(mCharacter.skill.cleaning, mCharacter.currentActivity.workSkill.cleaning));
  statCheck.push_back(std::make_pair(mCharacter.skill.service, mCharacter.currentActivity.workSkill.service));
  statCheck.push_back(std::make_pair(mCharacter.skill.music, mCharacter.currentActivity.workSkill.music));
  statCheck.push_back(std::make_pair(mCharacter.skill.art, mCharacter.currentActivity.workSkill.art));
  statCheck.push_back(std::make_pair(mCharacter.skill.tailor, mCharacter.currentActivity.workSkill.tailor));
  statCheck.push_back(std::make_pair(mCharacter.skill.stoneWorking, mCharacter.currentActivity.workSkill.stoneWorking));
  statCheck.push_back(std::make_pair(mCharacter.skill.woodWorking, mCharacter.currentActivity.workSkill.woodWorking));
  statCheck.push_back(std::make_pair(mCharacter.skill.metalworking, mCharacter.currentActivity.workSkill.metalworking));
  statCheck.push_back(std::make_pair(mCharacter.skill.farming, mCharacter.currentActivity.workSkill.farming));
  statCheck.push_back(std::make_pair(mCharacter.skill.fishing, mCharacter.currentActivity.workSkill.fishing));
  statCheck.push_back(std::make_pair(mCharacter.skill.crafting, mCharacter.currentActivity.workSkill.crafting));
  statCheck.push_back(std::make_pair(mCharacter.skill.sword, mCharacter.currentActivity.workSkill.sword));
  statCheck.push_back(std::make_pair(mCharacter.skill.axe, mCharacter.currentActivity.workSkill.axe));
  statCheck.push_back(std::make_pair(mCharacter.skill.bludgeon, mCharacter.currentActivity.workSkill.bludgeon));
  statCheck.push_back(std::make_pair(mCharacter.skill.stave, mCharacter.currentActivity.workSkill.stave));
  statCheck.push_back(std::make_pair(mCharacter.skill.polearm, mCharacter.currentActivity.workSkill.polearm));
  statCheck.push_back(std::make_pair(mCharacter.skill.evasion, mCharacter.currentActivity.workSkill.evasion));
  statCheck.push_back(std::make_pair(mCharacter.skill.fire, mCharacter.currentActivity.workSkill.fire));
  statCheck.push_back(std::make_pair(mCharacter.skill.water, mCharacter.currentActivity.workSkill.water));
  statCheck.push_back(std::make_pair(mCharacter.skill.earth, mCharacter.currentActivity.workSkill.earth));
  statCheck.push_back(std::make_pair(mCharacter.skill.air, mCharacter.currentActivity.workSkill.air));
  statCheck.push_back(std::make_pair(mCharacter.skill.lightning, mCharacter.currentActivity.workSkill.lightning));
  statCheck.push_back(std::make_pair(mCharacter.skill.holy, mCharacter.currentActivity.workSkill.holy));
  statCheck.push_back(std::make_pair(mCharacter.skill.dark, mCharacter.currentActivity.workSkill.dark));
  statCheck.push_back(std::make_pair(mCharacter.skill.machine, mCharacter.currentActivity.workSkill.machine));
  statCheck.push_back(std::make_pair(mCharacter.skill.poison, mCharacter.currentActivity.workSkill.poison));
  statCheck.push_back(std::make_pair(mCharacter.skill.choas, mCharacter.currentActivity.workSkill.choas));

  int successRoll = 0;
  int failedRoll = 0;
  World::Stat statUp { 0,0,0,0,0,0,0,0,0,0 };
  World::Skill skillUp { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

  for (auto& element : statCheck)
  {
    if (element.second > 0)
    {
      if (element.first > mWorld.getRandomNumber(seed, 0, element.second))
      {
        successRoll++;
      }
      else
      {
        failedRoll++;
      }
    }
  }

  if (successRoll > failedRoll)
  {
    mCharacter.stat = mCharacter.stat + mCharacter.currentActivity.workStat;
  }
}

std::string Event::getBlockHash()
{
  return mDaemonAPI.getBlockHash(mWorld.currentWorldHeight - 5);
}
