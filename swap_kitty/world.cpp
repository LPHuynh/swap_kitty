#include "world.h"


World::World()
{
  currentRulesetVersion = 1;
  startingHeight = 0;
  mIDCounter = 1; //0 is used to indicate non-existing items
  localTimeOffset = 0;
}

World::~World()
{
}

int16_t World::calculateElementAttackBonus(Element attackerElement, const std::vector<Element>& defenderElement)
{
  int16_t damageBonus = 100;

  for (auto& element : defenderElement) {
    if (attackerElement == Element::fire && (element == Element::air || element == Element::poison))
    { 
      damageBonus += 25;
    }
    else if (attackerElement == Element::water && (element == Element::fire || element == Element::machine))
    {
      damageBonus += 25;
    }
    else if (attackerElement == Element::earth && (element == Element::water || element == Element::lightning))
    {
      damageBonus += 25;
    }
    else if (attackerElement == Element::air && (element == Element::earth || element == Element::chaos))
    {
      damageBonus += 25;
    }
    else if (attackerElement == Element::lightning && element == Element::water)
    {
      damageBonus += 25;
    }
    else if (attackerElement == Element::holy && element == Element::dark)
    {
      damageBonus += 25;
    }
    else if (attackerElement == Element::dark && element == Element::holy)
    {
      damageBonus += 25;
    }
    else if (attackerElement == Element::machine && element == Element::fire)
    {
      damageBonus += 25;
    }
    else if (attackerElement == Element::poison && element == Element::air)
    {
      damageBonus += 25;
    }
    else if (attackerElement == Element::poison && element == Element::poison)
    {
      damageBonus -= 15;
    }
    else if (attackerElement == Element::chaos && (element == Element::earth && element == Element::chaos))
    {
      damageBonus += 25;
    }
    if (attackerElement == element)
    {
      damageBonus -= 25;
    }
  }

  return damageBonus;
}

World::Dice World::addDice(const std::vector<Dice>& die)
{
  Dice combinedDice;
  combinedDice.roll = 0;
  combinedDice.face = 0;
  combinedDice.plus = 0;

  for (auto& element : die) 
  {
    combinedDice.roll += element.roll;
    combinedDice.face += element.face;
    combinedDice.plus += element.plus;
  }

  return combinedDice;
}

World::Stat World::addStat(const std::vector<Stat>& stats)
{
  Stat combinedStat{ 0,0,0,0,0,0,0,0,0,0 };

  for (auto& element : stats)
  {
    combinedStat.Str += element.Str;
    combinedStat.Con += element.Con;
    combinedStat.Dex += element.Dex;
    combinedStat.Per += element.Per;
    combinedStat.Lrn += element.Lrn;
    combinedStat.Wil += element.Wil;
    combinedStat.Mag += element.Mag;
    combinedStat.Chr += element.Chr;
    combinedStat.Acc += element.Acc;
    combinedStat.Cri += element.Cri;
  }

  return combinedStat;
}

World::Skill World::addSkill(const std::vector<Skill>& skills)
{
  Skill combinedSkill{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

  for (auto& element : skills)
  {
    combinedSkill.literacy = element.literacy;
    combinedSkill.cooking = element.cooking;
    combinedSkill.cleaning = element.cleaning;
    combinedSkill.service = element.service;
    combinedSkill.music = element.music;
    combinedSkill.art = element.art;
    combinedSkill.tailor = element.tailor;
    combinedSkill.stoneWorking = element.stoneWorking;
    combinedSkill.woodWorking = element.woodWorking;
    combinedSkill.metalworking = element.metalworking;
    combinedSkill.farming = element.farming;
    combinedSkill.fishing = element.fishing;
    combinedSkill.crafting = element.crafting;
    combinedSkill.sword = element.sword;
    combinedSkill.axe = element.axe;
    combinedSkill.bludgeon = element.bludgeon;
    combinedSkill.stave = element.stave;
    combinedSkill.polearm = element.polearm;
    combinedSkill.evasion = element.evasion;
    combinedSkill.fire = element.fire;
    combinedSkill.water = element.water;
    combinedSkill.earth = element.earth;
    combinedSkill.air = element.air;
    combinedSkill.lightning = element.lightning;
    combinedSkill.holy = element.holy;
    combinedSkill.dark = element.dark;
    combinedSkill.machine = element.machine;
    combinedSkill.poison = element.poison;
    combinedSkill.choas = element.choas;
  }
}

uint16_t World::rollDie(const std::string& seed, uint16_t numOfDie, uint16_t numOfFaces)
{
  int16_t result = 0;

  for (int i = 0; i < numOfDie; i++)
  {
    result += getRandomNumber(seed, 1, numOfFaces);
  }

  return result;
}

uint16_t World::getRandomNumber(std::string seed, uint16_t minNumber, uint16_t maxNumber)
{
  seed += generateNonce() + seed;

  return (hash(seed) % (maxNumber - minNumber + 1)) + minNumber;
}

uint16_t World::getNoncelessRandomNumber(std::string seed, uint16_t minNumber, uint16_t maxNumber)
{
  return (hash(seed) % (maxNumber - minNumber + 1)) + minNumber;
}

uint16_t World::generateID()
{
  uint16_t id;

  if (mRecoveredID.size() < 1000)
  {
    id = mIDCounter;
    mIDCounter++;    
  }
  else
  {
    id = mRecoveredID.front();
    mRecoveredID.pop();
  }

  return id;
}

void World::freeID(uint16_t id)
{
  mRecoveredID.push(id);
}

std::string World::generateNonce()
{
  mNonceCounter++;
  return std::to_string(mNonceCounter);
}

uint32_t World::hash(const std::string& seed)
{
  //djb2 Hash Functions 
  const char* str = seed.c_str();
  unsigned long hash = 5381;
  int c;

  while (c = *str++)
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash;
}
