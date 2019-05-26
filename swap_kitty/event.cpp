#include "event.h"


Event::Event(World& world, Character& character, DaemonAPI& daemonAPI) : mWorld(world), mCharacter(character), mDaemonAPI(daemonAPI)
{
  time = { 0,0,0,0 };
  mEarning = 0;
  mCookingProgress = 0;
  mTotalActivityStatGained = { 0,0,0,0,0,0,0,0,0,0 };
}

Event::~Event()
{
}

void Event::init()
{
  //Event should be initialized after world and character have been fully populated by the character creation command
  updateTime();
  updateCheckedStat();

  mCharacter.currentActivity = mCharacter.dailySchedule[time.hour];

  switch (mCharacter.currentActivity.id)
  {
  case 0: mWorld.logging.addToMainLog(mCharacter.profile.name + " began Cooking..."); break;
  case 1: mWorld.logging.addToMainLog(mCharacter.profile.name + " began Cleaning the house..."); break;
  case 2: mWorld.logging.addToMainLog(mCharacter.profile.name + " began Reading in the library..."); break;
  case 3: mWorld.logging.addToMainLog(mCharacter.profile.name + " began Playing..."); break;
  case 4: mWorld.logging.addToMainLog(mCharacter.profile.name + " began Bathing..."); break;
  case 5: mWorld.logging.addToMainLog(mCharacter.profile.name + " began Napping..."); break;
  case 6: mWorld.logging.addToMainLog(mCharacter.profile.name + " began Sleeping..."); break;
  case 14: mWorld.logging.addToMainLog(mCharacter.profile.name + " began Fishing..."); break;
  case 15: mWorld.logging.addToMainLog(mCharacter.profile.name + " began Gathering Plants..."); break;
  default: mWorld.logging.addToMainLog(mCharacter.profile.name + " began working as a " + mCharacter.currentActivity.name + "..."); break;
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
          processDailyEvent();
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
  }
  if (time.minute == 60)
  {
    time.minute = 0;
    time.hour++;
  }
  if (time.hour == 24)
  {
    time.hour = 0;
    time.day++;
    mCharacter.profile.cosmetic.ageCounter++;

    if (mCharacter.profile.cosmetic.ageCounter == 120)
    {
      mCharacter.profile.cosmetic.age++;
      mCharacter.profile.cosmetic.ageCounter = 0;
    }
  }
  if (time.day == 120)
  {
    time.day = 0;
    time.year++;
  }
}

void Event::processDailyEvent()
{
  //Daily skill rust (~0.39% stat reduction for all stats and skills)
  mCharacter.stat = mCharacter.stat - mWorld.shiftStat(mCharacter.stat, -8);
  mCharacter.skill = mCharacter.skill - mWorld.shiftSkill(mCharacter.skill, -8);
  mCharacter.profile.domesticated -= mCharacter.profile.domesticated >> 8;
  mCharacter.profile.cosmetic.weight -= mCharacter.profile.cosmetic.weight >> 10;

  //Height and weight Gain/Loss at midnight if satiated
  if (mCharacter.profile.satiation > 75000 )
  {
    mCharacter.profile.cosmetic.height += 1;
    mCharacter.profile.cosmetic.weight += 60;
  }

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

void Event::processHourlyEvent(const std::string& seed)
{
  //Update Stats
  mCharacter.residence.cleaniness -= 100 + mCharacter.residence.houseLevel;
  mCharacter.profile.domesticated += 10;

  //Change job
  if (mCharacter.currentActivity.id != mCharacter.dailySchedule[time.hour].id)
  {
    //Summerize Previous Job
    
    std::string summary = "[" + mWorld.logging.progress + "]";
    mWorld.logging.clearProgress();

    if (mEarning > 0)
    {
      std::ostringstream formatedEarning;
      formatedEarning << (mEarning * 0.01f);
      summary += " +" + formatedEarning.str() + "G";
      mCharacter.profile.money += mEarning;
      mEarning = 0;
    }
    for (auto& element : mFoundItem)
    {
      summary += " +(" + element + ")";
    }
    mFoundItem.clear();

    //Display stat gained
    for (int i = 0; i < 11; i++)
    {
      if (mWorld.getStatByID(mTotalActivityStatGained, i) > 100)
      {
        summary += " +" + std::to_string(mWorld.getStatByID(mTotalActivityStatGained, i) / 100) + mWorld.getStatNameByID(i);
      }
    }

    mWorld.logging.addToMainLog(summary);
    mTotalActivityStatGained = { 0,0,0,0,0,0,0,0,0,0 };

    //Chance of catching vermins on the way home from agricultural jobs, heavy industry jobs, cooking, cleaning, or playing
    if (mCharacter.currentActivity.activityType == Job::ActivityType::agriculture || mCharacter.currentActivity.activityType == Job::ActivityType::heavyIndustry || mCharacter.currentActivity.id < 3)
    {
      if (mCharacter.profile.domesticated < mWorld.getRandomNumber(seed, 0, 15000 - mCharacter.profile.satiation))
      {
        Food::FoodItem caughtVermin = mCharacter.food.randomizeRawFood(seed, Food::FoodType::vermin);
        mCharacter.foodInventory.push_back(caughtVermin);
        mCharacter.profile.domesticated -= 10;
        mWorld.logging.addToMainLog(mCharacter.profile.name + " brought home a " + caughtVermin.nameCooked + "...");
      }
    }

    //Start New Job
    mCharacter.currentActivity = mCharacter.dailySchedule[time.hour];

    switch (mCharacter.currentActivity.id)
    {
    case 0: mWorld.logging.addToMainLog(mCharacter.profile.name + " began Cooking..."); break;
    case 1: mWorld.logging.addToMainLog(mCharacter.profile.name + " began Cleaning the house..."); break;
    case 2: mWorld.logging.addToMainLog(mCharacter.profile.name + " began Reading in the library..."); break;
    case 3: mWorld.logging.addToMainLog(mCharacter.profile.name + " began Playing..."); break;
    case 4: mWorld.logging.addToMainLog(mCharacter.profile.name + " began Bathing..."); break;
    case 5: mWorld.logging.addToMainLog(mCharacter.profile.name + " began Napping..."); break;
    case 6: mWorld.logging.addToMainLog(mCharacter.profile.name + " began Sleeping..."); break;
    case 14: mWorld.logging.addToMainLog(mCharacter.profile.name + " began Fishing..."); break;
    case 15: mWorld.logging.addToMainLog(mCharacter.profile.name + " began Gathering Plant..."); break;
    default: mWorld.logging.addToMainLog(mCharacter.profile.name + " began Working as a " + mCharacter.currentActivity.name + "..."); break;
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
      mWorld.logging.addToMainLog(mCharacter.profile.name + " puked.");
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
    if (mCharacter.foodInventory.empty() || mCharacter.profile.domesticated < mWorld.getRandomNumber(seed, 0, 10000))
    {
      Food::FoodItem caughtVermin = mCharacter.food.randomizeRawFood(seed, Food::FoodType::vermin);
      //TODO process eating caught vermin
      mCharacter.profile.domesticated -= 20;
      mCharacter.profile.happiness -= 100;
      mWorld.logging.addToMainLog(mCharacter.profile.name + " caught and ate a " + caughtVermin.nameCooked + ".");
      failedRoll = 100;
    }
    else
    {
      //TODO: process eating food
      mWorld.logging.addToMainLog(mCharacter.profile.name + " helped herself to some " + mCharacter.foodInventory.at(0).nameCooked + ".");
      failedRoll++;
    }
  }
  if (mCharacter.profile.quench < 0)
  {
    if (mCharacter.potionInventory.empty() || mCharacter.profile.domesticated < mWorld.getRandomNumber(seed, 0, 10000))
    {
      Potion::PotionItem water = mCharacter.potion.generatePotion(seed, "Pond Water");
      mCharacter.profile.domesticated -= 10;
    }
    else
    {
      uint16_t id = 0;
      for (auto& element : mCharacter.potionInventory)
      {
        if (element.quench > 250)
        {
          id = element.id;
        }
      }
      if (id == 0)
      {
        Potion::PotionItem water = mCharacter.potion.generatePotion(seed, "Pond Water");
        mCharacter.profile.domesticated -= 10;
      }
      //TODO: process drinking potion
    }   

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

  mCharacter.profile.stamina -= mCharacter.currentActivity.staminaUsed;
  mCharacter.profile.satiation -= mCharacter.currentActivity.staminaUsed;
  mCharacter.profile.quench -= mCharacter.currentActivity.staminaUsed;

  if (failedRoll == 0)
  {
    //Success
    if (mCharacter.currentActivity.id == 0)
    {
      //Cooking
      mCharacter.residence.cleaniness -= 50;
      mCookingProgress += 2;
      if (mCookingProgress - mCharacter.residence.kitchenLevel > 10)
      {
        //Cook the first raw food in inventory
        for (auto& element : mCharacter.foodInventory)
        {
          if (element.dishLevel == 0)
          {
            mCharacter.food.randomizeCookedFood(seed, element);
            mFoundItem.push_back(element.nameCooked);
            break;
          }
        }
        mCookingProgress = 0;
      }
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
        mCharacter.skill.literacy += 10;
      }
      else
      {
        uint16_t randomSelection = mWorld.getRandomNumber(seed, 0, uint16_t(mCharacter.library.size() - 1));

        mCharacter.stat = mCharacter.stat + mCharacter.library.at(randomSelection).bonusStat;
        mCharacter.skill = mCharacter.skill + mCharacter.library.at(randomSelection).bonusSkill;
        mTotalActivityStatGained = mTotalActivityStatGained + mCharacter.library.at(randomSelection).bonusStat;

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

        mCharacter.profile.happiness += 25;
        mCharacter.stat = mCharacter.stat + mCharacter.toyRoom.at(randomSelection).bonusStat;
        mCharacter.skill = mCharacter.skill + mCharacter.toyRoom.at(randomSelection).bonusSkill;
        mTotalActivityStatGained = mTotalActivityStatGained + mCharacter.toyRoom.at(randomSelection).bonusStat;

        if (mWorld.getRandomNumber(seed, 0, mCharacter.toyRoom.at(randomSelection).health) == 0)
        {
          //Toy destroyed due to usage;
          mCharacter.profile.happiness -= 10;
          mCharacter.toyRoom.erase(mCharacter.toyRoom.begin() + randomSelection);
        }
      }
    }
    else if (mCharacter.currentActivity.id == 4 || mCharacter.currentActivity.id == 5 || mCharacter.currentActivity.id == 6)
    {
      //Bathing, Naping, and Sleeping
      if (mCharacter.stat.wil < 1500)
      {
        mCharacter.stat = mCharacter.stat + mCharacter.currentActivity.workStat;
        mCharacter.skill = mCharacter.skill + mCharacter.currentActivity.workSkill;
        mTotalActivityStatGained = mTotalActivityStatGained + mCharacter.currentActivity.workStat;
      }
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
      mFishingProgress += 2;

      if (mFishingProgress > 20)
      {
        Food::FoodItem fish = mCharacter.food.randomizeRawFood(seed, Food::FoodType::fish);
        mCharacter.foodInventory.push_back(fish);
        mFoundItem.push_back(fish.nameCooked);
        mFishingProgress = 0;
      }
    }
    else if (mCharacter.currentActivity.id == 15)
    {
      //Plant Gathering
      mGatheringProgress += 2;

      if (mGatheringProgress > 10)
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
        mFoundItem.push_back(harvest.nameCooked);
        mGatheringProgress = 0;
      }
    }

    mEarning += mCharacter.currentActivity.payment;
    mWorld.logging.addToProgress("*");
  }
  else if (successRoll > failedRoll)
  {
    //Partial Success
    if (mCharacter.currentActivity.id == 0)
    {
      //Cooking
      mCharacter.residence.cleaniness -= 50;
      mCookingProgress++;
    }
    else if (mCharacter.currentActivity.id == 1)
    {
      //Cleaning
      mCharacter.residence.cleaniness += 100;
      if (mCharacter.residence.cleaniness > 10000)
      {
        mCharacter.residence.cleaniness = 10000;
      }
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
    mCharacter.stat = mCharacter.stat + mCharacter.currentActivity.workStat;
    mCharacter.skill = mCharacter.skill + mCharacter.currentActivity.workSkill;
    mTotalActivityStatGained = mTotalActivityStatGained + mCharacter.currentActivity.workStat;
  }
  else
  {
    //Failure
    if (mCharacter.currentActivity.id == 0)
    {
      //Cooking
      mCharacter.residence.cleaniness -= 200;
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

  //Reduce to max stat if over
  if (mCharacter.profile.health > mCharacter.profile.maxHealth)
  {
    mCharacter.profile.health = mCharacter.profile.maxHealth;
  }
  if (mCharacter.profile.mana > mCharacter.profile.maxMana)
  {
    mCharacter.profile.mana = mCharacter.profile.maxMana;
  }
  if (mCharacter.profile.stamina > 10000)
  {
    mCharacter.profile.cleaniness = 10000;
  }
  if (mCharacter.profile.cleaniness > 10000)
  {
    mCharacter.profile.cleaniness = 10000;
  }
  if (mCharacter.profile.happiness > 10000)
  {
    mCharacter.profile.happiness = 10000;
  }
  if (mCharacter.profile.obidence > 10000)
  {
    mCharacter.profile.obidence = 10000;
  }
}

void Event::updateTime()
{
  uint64_t offsetedHeight = mWorld.currentWorldHeight - mWorld.localTimeOffset;
  time.year = uint8_t(offsetedHeight / uint64_t(691200));       //There are 120 in-game days per year (30 days per season)
  time.day = uint8_t(offsetedHeight % uint64_t(691200) / 5760 + 1);
  time.hour = uint8_t((offsetedHeight % 5760) / 240);
  time.minute = uint8_t((offsetedHeight % 240) / 4);
  time.quarterminute = uint8_t((offsetedHeight % 4));
}

void Event::updateCheckedStat()
{

  for (int i = 0; i < 11; i++)
  {
    mEffectiveStat[i] = mWorld.getStatByID(mCharacter.stat, i) + mWorld.getStatByID(mCharacter.equipedWeapon.bonusStat, i) + mWorld.getStatByID(mCharacter.equipedDress.bonusStat, i);
  }

  for (int i = 0; i < 30; i++)
  {
    mEffectiveSkill[i] = mWorld.getSkillByID(mCharacter.skill, i) + mWorld.getSkillByID(mCharacter.equipedWeapon.bonusSkill, i) + mWorld.getSkillByID(mCharacter.equipedDress.bonusSkill, i);
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
