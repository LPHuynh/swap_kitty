#include "event.h"


Event::Event(World& world, Character& character, DaemonAPI& daemonAPI) : mWorld(world), mCharacter(character), mDaemonAPI(daemonAPI)
{
  time = { 0,0,0,0,0,0,"","" };
  mEarning = 0;
  mCookingProgress = 0;
  mTotalActivityStatGained = { 0,0,0,0,0,0,0,0,0,0 };
  mHeatStrokeCoolDown = 0;
  mCommonColdCoolDown = 0;
}

Event::~Event()
{
}

void Event::init()
{
  //Event should be initialized after world and character have been fully populated by the character creation command
  updateTime();
  updateCheckedStat();

  //Time/Date announcement
  int timeHour = time.hour;
  std::string timeSuffix = "AM";
  std::stringstream timeMinute;
  timeMinute << std::setfill('0') << std::setw(2) << std::to_string(time.minute);

  if (timeHour > 11)
  {
    timeHour -= 12;
    timeSuffix = "PM";
  }
  if (timeHour == 0)
  {
    timeHour = 12;
  }
  time.timeString = std::to_string(timeHour) + ":" + timeMinute.str() + timeSuffix;

  std::string dayOfMonthText = std::to_string(time.day);
  std::string monthText = "";

  switch (time.day)
  {
  case 1: case 21: dayOfMonthText += "st"; break;
  case 2: case 22: dayOfMonthText += "nd"; break;
  case 3: case 23: dayOfMonthText += "rd"; break;
  default: dayOfMonthText += "th"; break;
  }

  switch (time.month)
  {
  case 1: monthText = "Spring"; break;
  case 2: monthText = "Sumner"; break;
  case 3: monthText = "Autumn"; break;
  case 4: monthText = "Winter"; break;
  }

  time.dateString = dayOfMonthText + " of " + monthText + ", Year " + std::to_string(time.year);
  mWorld.logging.addToMainLog("--::-::-- " + time.dateString + " --::-::--");

  if (time.day == 1)
  {
    if (time.month == 1)
    {
      mWorld.logging.addToMainLog("[" + time.timeString + "] " + "It's the first day of the New Year!");
    }
    else
    {
      mWorld.logging.addToMainLog("[" + time.timeString + "] " + "It's the first day of " + monthText + "!");
    }
  }

  //Initial Job
  mCharacter.currentActivity = mCharacter.dailySchedule[time.hour];

  switch (mCharacter.currentActivity.id)
  {
  case 0: mWorld.logging.addToMainLog("[" + time.timeString + "] " + mCharacter.profile.name + " began Cooking..."); break;
  case 1: mWorld.logging.addToMainLog("[" + time.timeString + "] " + mCharacter.profile.name + " began Cleaning the house..."); break;
  case 2: mWorld.logging.addToMainLog("[" + time.timeString + "] " + mCharacter.profile.name + " began Reading in the library..."); break;
  case 3: mWorld.logging.addToMainLog("[" + time.timeString + "] " + mCharacter.profile.name + " began Playing..."); break;
  case 4: mWorld.logging.addToMainLog("[" + time.timeString + "] " + mCharacter.profile.name + " began Bathing..."); break;
  case 5: mWorld.logging.addToMainLog("[" + time.timeString + "] " + mCharacter.profile.name + " began Napping..."); break;
  case 6: mWorld.logging.addToMainLog("[" + time.timeString + "] " + mCharacter.profile.name + " began Sleeping..."); break;
  case 14: mWorld.logging.addToMainLog("[" + time.timeString + "] " + mCharacter.profile.name + " began Fishing..."); break;
  case 15: mWorld.logging.addToMainLog("[" + time.timeString + "] " + mCharacter.profile.name + " began Gathering Plants..."); break;
  default: mWorld.logging.addToMainLog("[" + time.timeString + "] " + mCharacter.profile.name + " began working as a " + mCharacter.currentActivity.name + "..."); break;
  }
}

void Event::processEvent()
{
  if (time.quarterminute == 0)
  {
    if (time.minute % 6 == 0)
    {
      processTenthHourlyEvent(getBlockHash());

      if (time.minute == 0)
      {
        processHourlyEvent(getBlockHash());

        if (time.hour == 0)
        {
          processDailyEvent(getBlockHash());
        }
      }
    }
  }
  incrementTime();
}

void Event::incrementTime()
{
  time.quarterminute++;
  if (time.quarterminute == 4)
  {
    time.quarterminute = 0;
    time.minute++;

    //Update time text every minute
    int timeHour = time.hour;
    std::string timeSuffix = "AM";
    std::stringstream timeMinute;
    timeMinute << std::setfill('0') << std::setw(2) << std::to_string(time.minute);

    if (timeHour > 11)
    {
      timeHour -= 12;
      timeSuffix = "PM";
    }
    if (timeHour == 0)
    {
      timeHour = 12;
    }
    time.timeString = std::to_string(timeHour) + ":" + timeMinute.str() + timeSuffix;

    if (time.minute == 60)
    {
      time.minute = 0;
      time.hour++;

      //Catch any minutes that rolled over to :00
      std::stringstream timeMinute;
      timeMinute << std::setfill('0') << std::setw(2) << std::to_string(time.minute);
      time.timeString = std::to_string(timeHour) + ":" + timeMinute.str() + timeSuffix;

      if (time.hour == 24)
      {
        time.hour = 0;
        time.day++;
        if (time.day == 31)
        {
          time.day = 1;
          time.month++;
          if (time.month == 4)
          {
            time.month = 1;
            time.year++;
          }
        }
      }
    }
  }
}

void Event::processDailyEvent(const std::string& seed)
{
  //Date announcement
  std::string dayOfMonthText = std::to_string(time.day);
  std::string monthText = "";

  switch (time.day)
  {
  case 1: case 21: dayOfMonthText += "st"; break;
  case 2: case 22: dayOfMonthText += "nd"; break;
  case 3: case 23: dayOfMonthText += "rd"; break;
  default: dayOfMonthText += "th"; break;
  }

  switch (time.month)
  {
  case 1: monthText = "Spring"; break;
  case 2: monthText = "Sumner"; break;
  case 3: monthText = "Autumn"; break;
  case 4: monthText = "Winter"; break;
  }

  time.dateString = dayOfMonthText + " of " + monthText + ", Year " + std::to_string(time.year);
  mWorld.logging.addToMainLog("--::-::-- " + time.dateString + " --::-::--");
  
  if (time.day == 1)
  {
    if (time.month == 1)
    {
      mWorld.logging.addToMainLog("It's the first day of the New Year!");
    }
    else
    {
      mWorld.logging.addToMainLog("It's the first day of " + monthText + "!");
    }
  }

  //Birthday announcement
  mCharacter.profile.cosmetic.ageCounter++;

  if (mCharacter.profile.cosmetic.ageCounter == 120)
  {
    mCharacter.profile.cosmetic.age++;
    mCharacter.profile.cosmetic.ageCounter = 0;
    mWorld.logging.addToMainLog(mCharacter.profile.name + " is now " + std::to_string(mCharacter.profile.cosmetic.age) + " years old!");
  }

  //Daily skill rust (~0.39% stat reduction for all stats and skills)
  mCharacter.profile.stat = mCharacter.profile.stat - mWorld.shiftStat(mCharacter.profile.stat, -8);
  mCharacter.profile.skill = mCharacter.profile.skill - mWorld.shiftSkill(mCharacter.profile.skill, -8);
  mCharacter.profile.domesticated -= mCharacter.profile.domesticated >> 8;

  //Cosmetic normalization
  mCharacter.profile.cosmetic.milkDrinkingCounter /= 2;
  if ((mCharacter.profile.cosmetic.height >> 10) < 1)
  {
    mCharacter.profile.cosmetic.height += 1 - (mCharacter.profile.cosmetic.weight >> 10);
  }
  mCharacter.profile.cosmetic.weight -= mCharacter.profile.cosmetic.weight >> 10;
  if (mCharacter.profile.cosmetic.currentSkinTone < mCharacter.profile.cosmetic.naturalSkinTone)
  {
    mCharacter.profile.cosmetic.currentSkinTone += 100;
  }
  else
  {
    mCharacter.profile.cosmetic.currentSkinTone -= 100;
  }

  //Height and weight Gain at midnight if satiated
  if (mCharacter.profile.satiation > 75000 )
  {
    if ((mCharacter.profile.cosmetic.milkDrinkingCounter > 2 && mCharacter.profile.cosmetic.age < 16) || mCharacter.profile.cosmetic.milkDrinkingCounter > 4)
    {
      mCharacter.profile.cosmetic.height++;
    }
    mCharacter.profile.cosmetic.weight += 60;
  }

  //Detoxification
  mCharacter.profile.toxicity /= 2;
  if (mCharacter.profile.primaryElement == World::Element::fire || mCharacter.profile.secondaryElement == World::Element::fire)
  {
    mCharacter.profile.toxicity /= 2;
  }

  //Daily skill gain from character's elemental
  switch (mCharacter.profile.primaryElement)
  {
  case World::Element::fire: mCharacter.profile.skill.fire += 40; break;
  case World::Element::water: mCharacter.profile.skill.water += 40; break;
  case World::Element::earth: mCharacter.profile.skill.earth += 40; break;
  case World::Element::lightning: mCharacter.profile.skill.lightning += 40; break;
  case World::Element::holy: mCharacter.profile.skill.holy += 40; break;
  case World::Element::dark: mCharacter.profile.skill.dark += 40; break;
  }
  switch (mCharacter.profile.secondaryElement)
  {
  case World::Element::fire: mCharacter.profile.skill.fire += 20; break;
  case World::Element::water: mCharacter.profile.skill.water += 20; break;
  case World::Element::earth: mCharacter.profile.skill.earth += 20; break;
  case World::Element::lightning: mCharacter.profile.skill.lightning += 20; break;
  case World::Element::holy: mCharacter.profile.skill.holy += 20; break;
  case World::Element::dark: mCharacter.profile.skill.dark += 20; break;
  }

  //Daily skill gain from character's dress
  mCharacter.profile.stat = mCharacter.profile.stat + mCharacter.equipedDress.dailyStatGain;
  mCharacter.profile.skill = mCharacter.profile.skill + mCharacter.equipedDress.dailySkillGain;

  //Refresh Shop Inventory
  mCharacter.shop.refreshInventory(seed);
}

void Event::processHourlyEvent(const std::string& seed)
{
  //Update Stats
  mCharacter.residence.cleaniness -= 100 + mCharacter.residence.houseLevel;
  mCharacter.profile.domesticated += 10;
  mWorld.shiftStat(mCharacter.profile.tempStat, -1);

  //Chance of catching vermins on the way home from agricultural jobs, heavy industry jobs, cooking, cleaning, or playing
  if (mCharacter.currentActivity.activityType == Job::ActivityType::agriculture || mCharacter.currentActivity.activityType == Job::ActivityType::heavyIndustry || mCharacter.currentActivity.id < 3)
  {
    if (mCharacter.profile.domesticated < mWorld.getRandomNumber(seed, 0, 15000 - mCharacter.profile.satiation))
    {
      Food::FoodItem caughtVermin = mCharacter.food.randomizeRawFood(seed, Food::FoodType::vermin);
      mCharacter.foodInventory.push_back(caughtVermin);
      mFoundItem.push_back(caughtVermin.name);
      mCharacter.profile.domesticated -= 10;

      switch (mCharacter.currentActivity.id)
      {
      case 0: mWorld.logging.addToMainLog("\t" + mCharacter.profile.name + " caught a " + caughtVermin.nameRaw + " while cooking..."); break;
      case 1: mWorld.logging.addToMainLog("\t" + mCharacter.profile.name + " found a " + caughtVermin.name + " while cleaning..."); break;
      case 2: mWorld.logging.addToMainLog("\t" + mCharacter.profile.name + " caught a " + caughtVermin.nameRaw + " while playing..."); break;
      default: mWorld.logging.addToMainLog("\t" + mCharacter.profile.name + " brought home a " + caughtVermin.name + " from work..."); break;
      }
    }
  }

  //Seaonal effects
  if (time.month == 2)
  {
    //Summer
    if (mCharacter.currentActivity.isOutdoorActivity && !mWorld.isNighttime)
    {
      mCharacter.profile.cosmetic.currentSkinTone += 40;
      mCharacter.profile.quench -= 40;

      if (mCharacter.equipedDress.isWinterEquip)
      {
        if (mWorld.getRandomNumber(seed, 0, 25) == 0)
        {
          mHeatStrokeCoolDown += mWorld.getRandomNumber(seed, 1, 6);
        }
      }
      else if (!mCharacter.equipedDress.isSummerEquip)
      {
        if (mWorld.getRandomNumber(seed, 0, 100) == 0)
        {
          mHeatStrokeCoolDown += mWorld.getRandomNumber(seed, 1, 4);
        }
      }
    }
    else if (mCharacter.currentActivity.isNearHeatSource)
    {
      mCharacter.profile.cosmetic.currentSkinTone += 20;

      if (mCharacter.equipedDress.isWinterEquip)
      {
        if (mWorld.getRandomNumber(seed, 0, 100) == 0)
        {
          mHeatStrokeCoolDown += mWorld.getRandomNumber(seed, 1, 2);
        }
      }
    }
  }
  else if (time.month == 4)
  {
    //Winter
    if (!mCharacter.currentActivity.isOutdoorActivity || !mCharacter.currentActivity.isNearHeatSource)
    {
      mCharacter.profile.cosmetic.currentSkinTone -= 10;
    }
    else if (mCharacter.currentActivity.isNearHeatSource)
    {
      mCharacter.profile.cosmetic.currentSkinTone += 20;
    }

    if (mCharacter.currentActivity.isOutdoorActivity)
    {
      if (mCharacter.equipedDress.isSummerEquip)
      {
        if (mWorld.getRandomNumber(seed, 0, 25) == 0)
        {
          mCommonColdCoolDown += mWorld.getRandomNumber(seed, 1, 6);
        }
      }
      else if (!mCharacter.equipedDress.isWinterEquip)
      {
        if (mWorld.getRandomNumber(seed, 0, 100) == 0)
        {
          mCommonColdCoolDown += mWorld.getRandomNumber(seed, 1, 4);
        }
      }
    }
  }
  else
  {
    //Spring and Autumn
    if (mCharacter.currentActivity.isOutdoorActivity || mCharacter.currentActivity.isNearHeatSource)
    {
      mCharacter.profile.cosmetic.currentSkinTone += 20;
    }
  }

  if (mCharacter.profile.toxicity > 100)
  {
    if (mCharacter.profile.toxicity - 100 > mWorld.getRandomNumber(seed, 0, 100))
    {
      mCommonColdCoolDown = mWorld.getRandomNumber(seed, 1, 3);
      mCharacter.profile.toxicity /= 2;
    }
  }

  //Change job
  Job::Activity nextJob;

  //Summerize Previous Job    
  std::string summary = "\t[" + mWorld.logging.progress + "]";
  mWorld.logging.clearProgress();

  if (mEarning > 0)
  {
    std::stringstream formatedEarning;
    formatedEarning << std::setfill('0') << std::setw(std::to_string(mEarning).length() + 1) << std::left << (mEarning * 0.01f);
    summary += " +" + formatedEarning.str() + "G";
    mCharacter.profile.money += mEarning;
    mEarning = 0;
  }
  for (auto& element : mFoundItem)
  {
    summary += " +(" + element + ")";
  }
  mFoundItem.clear();

  for (int i = 0; i < 11; i++)
  {
    if (mWorld.getStatByID(mTotalActivityStatGained, i) > 100)
    {
      summary += " +" + std::to_string(mWorld.getStatByID(mTotalActivityStatGained, i) / 100) + mWorld.getStatNameByID(i);
    }
  }

  mWorld.logging.addToMainLog(summary);
  mTotalActivityStatGained = { 0,0,0,0,0,0,0,0,0,0 };

  //Time announcement
  switch (time.hour)
  {
  case 6: mWorld.logging.addToMainLog("It's Morning!"); mWorld.isNighttime = false; break; //6AM
  case 20: mWorld.logging.addToMainLog("It's Nighttime!"); mWorld.isNighttime = true; break; //8PM
  }

  //Check if character is sick
  if (mCharacter.profile.stamina < 0 || mCommonColdCoolDown > 0 || mHeatStrokeCoolDown > 0)
  {
    mCharacter.profile.happiness -= 500;
    if (mWorld.isNighttime)
    {
      nextJob = mCharacter.job.getActivity("Sleep");
    }
    else
    {
      nextJob = mCharacter.job.getActivity("Nap");
    }
    if (mCharacter.profile.stamina < 0)
    {
      mWorld.logging.addToMainLog("\t" + mCharacter.profile.name + " is exhausted from the overworking and needs to rest...");
    }
    else if (mCommonColdCoolDown > 0)
    {
      mWorld.logging.addToMainLog("\t" + mCharacter.profile.name + " came down with the flu and needs to rest...");
      mCommonColdCoolDown--;
      if ((mCharacter.profile.primaryElement == World::Element::fire || mCharacter.profile.secondaryElement == World::Element::fire) && mHeatStrokeCoolDown > 0)
      {
        mCommonColdCoolDown--;
      }
    }
    else if (mHeatStrokeCoolDown > 0)
    {
      mWorld.logging.addToMainLog("\t" + mCharacter.profile.name + " collapsed from the heat and needs to rest...");
      mHeatStrokeCoolDown--;
      if ((mCharacter.profile.primaryElement == World::Element::water || mCharacter.profile.secondaryElement == World::Element::water) && mHeatStrokeCoolDown > 0)
      {
        mHeatStrokeCoolDown--;
      }
    }
  }
  else
  {
    nextJob = mCharacter.dailySchedule[time.hour];
  }

  //Start New Job
  if (mCharacter.currentActivity.id != nextJob.id)
  {
    mCharacter.currentActivity = nextJob;

    switch (mCharacter.currentActivity.id)
    {
    case 0: mWorld.logging.addToMainLog("[" + time.timeString + "] " + mCharacter.profile.name + " began Cooking..."); break;
    case 1: mWorld.logging.addToMainLog("[" + time.timeString + "] " + mCharacter.profile.name + " began Cleaning the house..."); break;
    case 2: mWorld.logging.addToMainLog("[" + time.timeString + "] " + mCharacter.profile.name + " began Reading in the library..."); break;
    case 3: mWorld.logging.addToMainLog("[" + time.timeString + "] " + mCharacter.profile.name + " began Playing..."); break;
    case 4: mWorld.logging.addToMainLog("[" + time.timeString + "] " + mCharacter.profile.name + " began Bathing..."); break;
    case 5: mWorld.logging.addToMainLog("[" + time.timeString + "] " + mCharacter.profile.name + " began Napping..."); break;
    case 6: mWorld.logging.addToMainLog("[" + time.timeString + "] " + mCharacter.profile.name + " began Sleeping..."); break;
    case 14: mWorld.logging.addToMainLog("[" + time.timeString + "] " + mCharacter.profile.name + " began Fishing..."); break;
    case 15: mWorld.logging.addToMainLog("[" + time.timeString + "] " + mCharacter.profile.name + " began Gathering Plant..."); break;
    default: mWorld.logging.addToMainLog("[" + time.timeString + "] " + mCharacter.profile.name + " began Working as a " + mCharacter.currentActivity.name + "..."); break;
    }
  }
}

void Event::processTenthHourlyEvent(const std::string& seed)
{
  int successRoll = 0;
  int failedRoll = 0;

  //Update Stats
  updateCheckedStat();
  mCharacter.profile.stamina -= mCharacter.currentActivity.staminaUsed;
  mCharacter.profile.satiation -= mCharacter.currentActivity.staminaUsed;
  mCharacter.profile.quench -= mCharacter.currentActivity.quenchUsed;

  if (mCharacter.currentActivity.activityType == mCharacter.favouriteActivityType || mCharacter.currentActivity.activityType == Job::ActivityType::self)
  {
    mCharacter.profile.happiness += 100;
  }
  else
  {
    mCharacter.profile.happiness -= 20;
  }

  //Check Satation/Thirst Status
  if (mCharacter.profile.satiation > 10000 || mCharacter.profile.quench > 10000)
  {
    int targetRoll = 1;

    if (mCharacter.profile.satiation > 15000 || mCharacter.profile.quench > 15000)
    {
      targetRoll = 4;
    }

    if (mWorld.rollDie(seed, 2, 6) <= targetRoll)
    {
      mWorld.logging.addToMainLog("\t" + mCharacter.profile.name + " puked.");
      mCharacter.profile.satiation -= 200;
      mCharacter.profile.quench -= 200;
      failedRoll = 100;
    }
    else if (mCharacter.profile.satiation > 1000)
    {
      mCharacter.profile.satiation -= 20;
    }
    else
    {
      mCharacter.profile.quench -= 20;
    }
  }
  if (mCharacter.profile.satiation < 0)
  {
    if (mCharacter.foodInventory.empty() || mCharacter.profile.domesticated < mWorld.getRandomNumber(seed, 0, 7500))
    {
      Food::FoodItem caughtVermin = mCharacter.food.randomizeRawFood(seed, Food::FoodType::vermin);
      mCharacter.foodInventory.push_back(caughtVermin);
      mWorld.logging.addToMainLog("\t" + mCharacter.profile.name + " caught and ate a " + caughtVermin.name + ".");
      mCharacter.consumeFood(caughtVermin.id, false);
      failedRoll = 100;
    }
    else
    {
      mWorld.logging.addToMainLog("\t" + mCharacter.profile.name + " helped herself to some " + mCharacter.foodInventory.at(0).name + ".");
      mCharacter.consumeFood(mCharacter.foodInventory.at(0).id, false);
      failedRoll++;
    }
  }
  if (mCharacter.profile.quench < 0)
  {
    uint16_t selectedID = 0;
    std::string selectedName = "";

    if (mCharacter.potionInventory.empty() || mCharacter.profile.domesticated < mWorld.getRandomNumber(seed, 0, 7500))
    {
      Potion::PotionItem water = mCharacter.potion.generatePotion(seed, "Pond Water");
      mCharacter.potionInventory.push_back(water);
      selectedID = water.id;
      selectedName = water.name;
    }
    else
    {
      for (auto& element : mCharacter.potionInventory)
      {
        if (element.quench > 250)
        {
          selectedID = element.id;
          selectedName = element.name;
        }
      }
      if (selectedID == 0)
      {
        Potion::PotionItem water = mCharacter.potion.generatePotion(seed, "Pond Water");
        mCharacter.potionInventory.push_back(water);
        selectedID = water.id;
        selectedName = water.name;
      }
    }
    mWorld.logging.addToMainLog("\t" + mCharacter.profile.name + " helped herself to some " + selectedName + ".");
    mCharacter.consumePotion(selectedID, false);
    failedRoll++;
  }

  //Obedience check
  if (mCharacter.currentActivity.activityType == Job::ActivityType::self)
  {
    mCharacter.profile.obidence += 50;
  }
  else
  {
    if (mCharacter.profile.obidence < mWorld.getRandomNumber(seed, 0, 1000 * mCharacter.currentActivity.workLevel))
    {
      mCharacter.profile.obidence += 10;
      failedRoll = 100;
    }
    else
    {
      mCharacter.profile.obidence -= 10;
    }
  }

  //Process activity
  for (int i = 0; i < 11; i++)
  {
    if (mWorld.getStatByID(mCharacter.currentActivity.workStat, i) > 0)
    {
      if (mEffectiveStat[i] > mWorld.getRandomNumber(seed, 0, mWorld.getStatByID(mCharacter.currentActivity.workStat, i) * 100))
      {
        successRoll++;
      }
      else
      {
        failedRoll++;
      }
    }
  }
  for (int i = 0; i < 30; i++)
  {
    if (mWorld.getStatByID(mCharacter.currentActivity.workStat, i) > 0)
    {
      if (mEffectiveSkill[i] > mWorld.getRandomNumber(seed, 0, mWorld.getSkillByID(mCharacter.currentActivity.workSkill, i) * 100))
      {
        successRoll++;
      }
      else
      {
        failedRoll++;
      }
    }
  }

  //Bonus success rolls
  if (mCharacter.currentActivity.activityType == Job::ActivityType::domestic)
  {
    if (mCharacter.profile.domesticated > 7500)
    {
      successRoll++;
    }
  }
  else
  {
    switch (mCharacter.currentActivity.id)
    {
    case 4: successRoll++; break; //Bath
    case 5: successRoll++; break; //Nap
    case 6: successRoll++; break; //Sleep
    case 14: if (mCharacter.profile.domesticated < 2500) { successRoll++; } break; //Fishing
    case 18: if (mCharacter.profile.domesticated < 2500) { successRoll++; } break; //Butcher
    case 19: if (mCharacter.profile.domesticated < 2500) { successRoll++; } break; //Hunting
    }
  }

  if (failedRoll == 0)
  {
    //Success
    if (mCharacter.currentActivity.id == 0)
    {
      //Cooking
      mCharacter.residence.cleaniness -= 50;
      mCookingProgress += 4;
    }
    else if (mCharacter.currentActivity.id == 1)
    {
      //Cleaning
      mCharacter.residence.cleaniness += 250;
    }
    else if (mCharacter.currentActivity.id == 2)
    {
      //Reading
      if (mCharacter.library.empty())
      {
        mCharacter.profile.skill.literacy += 10;
      }
      else
      {
        uint16_t randomSelection = mWorld.getRandomNumber(seed, 0, uint16_t(mCharacter.library.size() - 1));

        mCharacter.profile.stat = mCharacter.profile.stat + mCharacter.library.at(randomSelection).bonusStat;
        mCharacter.profile.skill = mCharacter.profile.skill + mCharacter.library.at(randomSelection).bonusSkill;
        mTotalActivityStatGained = mTotalActivityStatGained + mCharacter.library.at(randomSelection).bonusStat;

        if (mWorld.getRandomNumber(seed, 0, mCharacter.library.at(randomSelection).health) == 0)
        {
          //Book destroyed due to usage
          mCharacter.destroyItem(mCharacter.library, mCharacter.library.at(randomSelection).id, false);
        }
      }
    }
    else if (mCharacter.currentActivity.id == 3)
    {
      //Playing
      if (!mCharacter.toyRoom.empty())
      {
        uint16_t randomSelection = mWorld.getRandomNumber(seed, 0, uint16_t(mCharacter.toyRoom.size() - 1));

        mCharacter.profile.happiness += 25;
        mCharacter.profile.stat = mCharacter.profile.stat + mCharacter.toyRoom.at(randomSelection).bonusStat;
        mCharacter.profile.skill = mCharacter.profile.skill + mCharacter.toyRoom.at(randomSelection).bonusSkill;
        mTotalActivityStatGained = mTotalActivityStatGained + mCharacter.toyRoom.at(randomSelection).bonusStat;

        if (mWorld.getRandomNumber(seed, 0, mCharacter.toyRoom.at(randomSelection).health) == 0)
        {
          //Toy destroyed due to usage
          mCharacter.destroyItem(mCharacter.toyRoom, mCharacter.toyRoom.at(randomSelection).id, false);
          mCharacter.profile.happiness -= 10;
        }
      }
    }
    else if (mCharacter.currentActivity.id == 4 || mCharacter.currentActivity.id == 5 || mCharacter.currentActivity.id == 6)
    {
      //Bathing, Naping, and Sleeping
      if (mCharacter.profile.health < mCharacter.profile.maxHealth)
      {
        mCharacter.profile.health += mCharacter.profile.maxHealth / 100;
      }
      if (mCharacter.profile.mana < mCharacter.profile.maxMana)
      {
        mCharacter.profile.mana += mCharacter.profile.maxMana / 100;
      }
      if (mCharacter.currentActivity.id == 5 || mCharacter.currentActivity.id == 6)
      {
        mCharacter.profile.stamina += 200;
      }
      else
      {
        mCharacter.profile.cleaniness += 150;
        mCharacter.profile.stamina += 50;
      }

      mCharacter.profile.domesticated++;
    }
    else if (mCharacter.currentActivity.id == 14)
    {
      //Fishing
      mFishingProgress += 4;
    }
    else if (mCharacter.currentActivity.id == 15)
    {
      //Plant Gathering
      mGatheringProgress += 4;
    }
    else if (mCharacter.currentActivity.id == 15)
    {
    //Industrial Fishing
      mFishingProgress++;
    }
    else if (mCharacter.currentActivity.id == 16)
    {
    //Farming
      mGatheringProgress++;
    }
    else if (mCharacter.currentActivity.id == 17)
    {
    //Butcher
    mHuntingProgress++;
    }
    else if (mCharacter.currentActivity.id == 18)
    {
    //Hunting
    mHuntingProgress++;
    }

    mEarning += mCharacter.currentActivity.payment;
    mWorld.logging.addToProgress("*");
  }
  else if (successRoll >= failedRoll)
  {
    //Partial Success
    if (mCharacter.currentActivity.id == 0)
    {
      //Cooking
      mCharacter.residence.cleaniness -= 100;
      mCookingProgress++;
    }
    else if (mCharacter.currentActivity.id == 1)
    {
      //Cleaning
      mCharacter.residence.cleaniness += 100;
    }
    else if (mCharacter.currentActivity.id == 14)
    {
      //Fishing
      mFishingProgress++;
    }
    else if (mCharacter.currentActivity.id == 15)
    {
      //Plant Gathering
      mGatheringProgress++;
    }

    mEarning += mCharacter.currentActivity.payment / 2;
    mWorld.logging.addToProgress("+");

    //Learn from minor mistakes
    mCharacter.profile.happiness -= 50;
    mCharacter.profile.stat = mCharacter.profile.stat + mCharacter.currentActivity.workStat;
    mCharacter.profile.skill = mCharacter.profile.skill + mCharacter.currentActivity.workSkill;
    mTotalActivityStatGained = mTotalActivityStatGained + mCharacter.currentActivity.workStat;
  }
  else
  {
    //Failure
    if (mCharacter.currentActivity.id == 0)
    {
      //Cooking
      mCharacter.residence.cleaniness -= 400;
    }
    else if (mCharacter.currentActivity.id == 2)
    {
      //Reading
      if (!mCharacter.library.empty())
      {
        uint16_t randomSelection = mWorld.getRandomNumber(seed, 0, uint16_t(mCharacter.library.size() - 1));

        if (mWorld.getRandomNumber(seed, 0, mCharacter.library.at(randomSelection).health) == 0)
        {
          //Book destroyed due to usage;
          mCharacter.library.erase(mCharacter.library.begin() + randomSelection);
        }
      }
    }
    else if (mCharacter.currentActivity.id == 3)
    {
      //Playing
      if (!mCharacter.toyRoom.empty())
      {
        uint16_t randomSelection = mWorld.getRandomNumber(seed, 0, uint16_t(mCharacter.toyRoom.size() - 1));

        if (mWorld.getRandomNumber(seed, 0, mCharacter.toyRoom.at(randomSelection).health) == 0)
        {
          //Toy destroyed due to usage;
          mCharacter.toyRoom.erase(mCharacter.toyRoom.begin() + randomSelection);
        }
      }
    }

    mCharacter.profile.happiness -= 100;
    mEarning += mCharacter.currentActivity.payment / 8;
    mWorld.logging.addToProgress("-");
  }

  //Item acquirement
  if (mCookingProgress - mCharacter.residence.kitchenLevel > 20)
  {
    //Cook the first raw food in inventory
    for (auto& element : mCharacter.foodInventory)
    {
      if (element.dishLevel == 0)
      {
        mCharacter.food.randomizeCookedFood(seed, element);
        mFoundItem.push_back(element.name);
        break;
      }
    }
    mCookingProgress = 0;
  }
  if (mFishingProgress > 40)
  {
    Food::FoodItem fish = mCharacter.food.randomizeRawFood(seed, Food::FoodType::fish);
    mCharacter.foodInventory.push_back(fish);
    mFoundItem.push_back(fish.name);
    mFishingProgress = 0;
  }
  else if (mGatheringProgress > 20)
  {
    Food::FoodItem harvest;
    if (mWorld.rollDie(seed, 1, 2) == 1)
    {
      harvest = mCharacter.food.randomizeRawFood(seed, Food::FoodType::fruit);
    }
    else
    {
      harvest = mCharacter.food.randomizeRawFood(seed, Food::FoodType::vegatable);
    }
    mCharacter.foodInventory.push_back(harvest);
    mFoundItem.push_back(harvest.name);
    mGatheringProgress = 0;
  }
  else if (mHuntingProgress > 40)
  {
    Food::FoodItem corpse = mCharacter.food.randomizeRawFood(seed, Food::FoodType::corpse);
    mCharacter.foodInventory.push_back(corpse);
    mFoundItem.push_back(corpse.name);
    mHuntingProgress = 0;
  }

  //Reduce stats when over soft-maximum
  if (mCharacter.profile.health > mCharacter.profile.maxHealth)
  {
    mCharacter.profile.health -= (mCharacter.profile.maxHealth - 10000) / 2;
  }
  if (mCharacter.profile.mana > mCharacter.profile.maxMana)
  {
    mCharacter.profile.mana -= (mCharacter.profile.maxMana - 10000) / 2;
  }
  if (mCharacter.profile.stamina > 10000)
  {
    mCharacter.profile.stamina -= (mCharacter.profile.stamina - 10000) / 2;
  }
  if (mCharacter.profile.cleaniness > 10000)
  {
    mCharacter.profile.cleaniness -= (mCharacter.profile.cleaniness - 10000) / 2;
  }
  if (mCharacter.profile.happiness > 10000)
  {
    mCharacter.profile.happiness -= (mCharacter.profile.happiness - 10000) / 2;
  }
  if (mCharacter.profile.obidence > 10000)
  {
    mCharacter.profile.obidence -= (mCharacter.profile.obidence - 10000) / 2;
  }
  if (mCharacter.residence.cleaniness > 10000)
  {
    mCharacter.residence.cleaniness -= (mCharacter.profile.cleaniness - 10000) / 2;
  }
}

void Event::updateTime()
{
  uint64_t offsetedHeight = mWorld.currentWorldHeight - mWorld.localTimeOffset;
  time.year = uint8_t(offsetedHeight / uint64_t(691200) + 1);       //There are 120 in-game days per year (30 days per season)
  time.month = uint8_t(offsetedHeight % uint64_t(691200) / 172800 + 1);
  time.day = uint8_t(offsetedHeight % uint64_t(172800) / 5760 + 1);
  time.hour = uint8_t((offsetedHeight % 5760) / 240);
  time.minute = uint8_t((offsetedHeight % 240) / 4);
  time.quarterminute = uint8_t((offsetedHeight % 4));
}

void Event::updateCheckedStat()
{

  for (int i = 0; i < 11; i++)
  {
    mEffectiveStat[i] = mWorld.getStatByID(mCharacter.profile.stat, i) + mWorld.getStatByID(mCharacter.profile.tempStat, i) + mWorld.getStatByID(mCharacter.equipedWeapon.bonusStat, i) + mWorld.getStatByID(mCharacter.equipedDress.bonusStat, i);
  }

  for (int i = 0; i < 30; i++)
  {
    mEffectiveSkill[i] = mWorld.getSkillByID(mCharacter.profile.skill, i) + mWorld.getSkillByID(mCharacter.equipedWeapon.bonusSkill, i) + mWorld.getSkillByID(mCharacter.equipedDress.bonusSkill, i);
  }
}

std::string Event::getBlockHash()
{
  std::map<uint64_t, std::string>::iterator it = mWorld.blockhashCache.find(mWorld.currentWorldHeight - 5);
  if (it != mWorld.blockhashCache.end())
  {
    return it->second;
  }
  else
  {
    std::string blockHash = mDaemonAPI.getBlockHash(mWorld.currentWorldHeight - 5);
    mWorld.blockhashCache[mWorld.currentWorldHeight - 5] = blockHash;
    return blockHash;
  }
}
