#include "world.h"


World::World()
{
}

World::~World()
{
}

bool World::init(std::string daemonHost, uint16_t daemonPort, uint16_t walletPort)
{
  currentRulesetVersion = 1;

  if (!daemonAPI.init(daemonHost, daemonPort))
  {
    return false;
  }
  if (!walletAPI.init(daemonHost, daemonPort, walletPort))
  {
    return false;
  }
  return true;
}

World::Element World::randomizeElement(const std::string& seed)
{
  return Element(mSiphashRNG.getRandomNumber(seed + "element__", 0, 10));
}

World::Element World::randomizePlayerElement(const std::string& seed)
{
  return Element(mSiphashRNG.getRandomNumber(seed + "playerelement__", 1, 6));
}

int16_t World::elementAttackBonus(Element attackerElement, std::vector<Element> defenderElement)
{
  int16_t damageBonus = 0;

  for (auto & element : defenderElement) {
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
    else if (attackerElement == Element::machine && element == Element::machine)
    {
      damageBonus -= 25;
    }
    else if (attackerElement == Element::poison && element == Element::air)
    {
      damageBonus += 25;
    }
    else if (attackerElement == Element::poison && element == Element::poison)
    {
      damageBonus -= 25;
    }
    else if (attackerElement == Element::chaos && (element == Element::earth && element == Element::chaos))
    {
      damageBonus += 25;
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

  for (auto & element : die) 
  {
    combinedDice.roll += element.roll;
    combinedDice.face += element.face;
    combinedDice.plus += element.plus;
  }

  return combinedDice;
}

World::Stat World::addStat(const std::vector<Stat>& stats)
{
  Stat combinedStat;
  combinedStat.Str = 0;
  combinedStat.Con = 0;
  combinedStat.Dex = 0;
  combinedStat.Per = 0;
  combinedStat.Lrn = 0;
  combinedStat.Wil = 0;
  combinedStat.Mag = 0;
  combinedStat.Chr = 0;
  combinedStat.Acc = 0;
  combinedStat.Cri = 0;

  for (auto & element : stats)
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
