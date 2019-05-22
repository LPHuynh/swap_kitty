#include "dress.h"


Dress::Dress(World & world) : mWorld(world)
{
  loadDress();
}

Dress::~Dress()
{
}


Dress::DressItem Dress::randomizeDress(const std::string & seed)
{
  Dress::DressItem dress = mDresses.at(mWorld.getRandomNumber(seed, 0, 0));
  dress.id = mWorld.generateID();
  return dress;
}

Dress::DressItem Dress::generateDress(const std::string& name)
{
  Dress::DressItem dress;

  for (auto& element : mDresses)
  {
    if (element.name == name)
    {
      dress = element;
      dress.id = mWorld.generateID();
      return dress;
    }
  }

  dress = mDresses.at(0);
  dress.id = mWorld.generateID();
  return dress;
}

void Dress::loadDress()
{
  DressItem prototype;
  prototype.id = 0;
  prototype.name = "";
  prototype.price = 0;
  prototype.isWinterEquip = false;
  prototype.isSummerEquip = false;
  prototype.bonusStat = World::Stat{ 0,0,0,0,0,0,0,0,0,0 };
  prototype.bonusSkill = World::Skill{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
  prototype.dailyStatGain = World::Stat{ 0,0,0,0,0,0,0,0,0,0 };
  prototype.dailySkillGain = World::Skill{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

  DressItem dress;

  //Ruleset v1: dress slot 0-0
  dress = prototype;
  dress.name = "Basic Dress";
  dress.price = 100000;
  mDresses.push_back(dress);
}
