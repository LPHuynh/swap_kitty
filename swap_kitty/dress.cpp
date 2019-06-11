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
  Dress::DressItem dress = mDresses.at(mWorld.getRandomNumber(seed, 0, 10));

  if (mWorld.rollDie(seed, 1, 3) > 1)
  {
    uint16_t qualitySelection = mWorld.getRandomNumber(seed, 0, 15);
    dress.price *= 2;
    switch (qualitySelection)
    {
    case 0: dress.name = "Beutiful " + dress.baseName; dress.bonusStat.chr += 4000; dress.price *= 4; break;
    case 1: dress.name = "Pretty " + dress.baseName; dress.bonusStat.chr += 2000; dress.price *= 2;; break;
    case 2: dress.name = "Cute " + dress.baseName; dress.bonusStat.chr += 1000; break;
    case 3: dress.name = "Brutish " + dress.baseName; dress.bonusStat.str += 1000; break;
    case 4: dress.name = "Light Weight " + dress.baseName; dress.bonusStat.dex += 1000; break;
    case 5: dress.name = "Silky " + dress.baseName; dress.bonusStat.per += 1000; break;
    case 6: dress.name = "Comfy  " + dress.baseName; dress.bonusStat.lrn += 1000; break;
    case 7: dress.name = "Tight " + dress.baseName; dress.bonusStat.wil += 1000; break;
    case 8: dress.name = "Magical " + dress.baseName; dress.bonusStat.mag += 1000; break;
    case 9: dress.name = "Stuffy " + dress.baseName; dress.bonusSkill.fire += 1000; dress.isWinterEquip = true; break;
    case 10: dress.name = "Cool " + dress.baseName; dress.bonusSkill.water += 1000; dress.isSummerEquip = true; break;
    case 11: dress.name = "Plain " + dress.baseName; dress.bonusSkill.earth += 1000; break;
    case 12: dress.name = "Frilly " + dress.baseName; dress.bonusSkill.air += 1000; break;
    case 13: dress.name = "Bright " + dress.baseName; dress.bonusSkill.lightning += 1000; break;
    case 14: dress.name = "Fluffy " + dress.baseName; dress.bonusSkill.holy += 1000; break;
    case 15: dress.name = "Cuddly " + dress.baseName; dress.bonusSkill.dark += 1000; break;
    }

    if (dress.isWinterEquip && dress.isSummerEquip)
    {
      dress.isWinterEquip = false;
      dress.isSummerEquip = false;
    }
  }

  dress.price += mWorld.getRandomNumber(seed, 0, 50000);
  dress.id = mWorld.generateID();
  return dress;
}

Dress::DressItem Dress::generateDress(const std::string& name)
{
  Dress::DressItem dress;

  for (auto& element : mDresses)
  {
    if (element.baseName == name)
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
  prototype.baseName = "";
  prototype.name = "";
  prototype.price = 0;
  prototype.isWinterEquip = false;
  prototype.isSummerEquip = false;
  prototype.bonusStat = World::Stat{ 0,0,0,0,0,0,0,0,0,0 };
  prototype.bonusSkill = World::Skill{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
  prototype.dailyStatGain = World::Stat{ 0,0,0,0,0,0,0,0,0,0 };
  prototype.dailySkillGain = World::Skill{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

  DressItem dress;

  //Ruleset v1: dress slot 0-10
  dress = prototype;
  dress.baseName = "Basic Dress";
  dress.name = dress.baseName;
  dress.price = 100000;
  mDresses.push_back(dress);

  dress = prototype;
  dress.baseName = "Maid Dress";
  dress.name = dress.baseName;
  dress.price = 150000;
  dress.dailySkillGain.cleaning = 200;
  mDresses.push_back(dress);

  dress = prototype;
  dress.baseName = "Apron Dress";
  dress.name = dress.baseName;
  dress.price = 150000;
  dress.dailySkillGain.cooking = 200;
  mDresses.push_back(dress);

  dress = prototype;
  dress.baseName = "Witch's Gown";
  dress.name = "A " + dress.baseName;
  dress.price = 150000;
  dress.dailyStatGain.mag = 100;
  mDresses.push_back(dress);

  dress = prototype;
  dress.baseName = "Jump Suit";
  dress.name = dress.baseName;
  dress.price = 150000;
  dress.dailyStatGain.con = 100;
  mDresses.push_back(dress);

  dress = prototype;
  dress.baseName = "Victorian Dress";
  dress.name = dress.baseName;
  dress.price = 150000;
  dress.dailyStatGain.chr = 100;
  mDresses.push_back(dress);

  dress = prototype;
  dress.baseName = "Hunting Dress";
  dress.name = dress.baseName;
  dress.price = 150000;
  dress.dailyStatGain.per = 100;
  mDresses.push_back(dress);

  dress = prototype;
  dress.baseName = "Ninja Suit";
  dress.name = dress.baseName;
  dress.price = 150000;
  prototype.isSummerEquip = true;
  dress.dailyStatGain.dex = 100;
  mDresses.push_back(dress);

  dress = prototype;
  dress.baseName = "Summer Dress";
  dress.name = dress.baseName;
  dress.price = 150000;
  prototype.isSummerEquip = true;
  dress.dailyStatGain.dex = 100;
  mDresses.push_back(dress);

  dress = prototype;
  dress.baseName = "Fur Coat";
  dress.name = dress.baseName;
  prototype.isWinterEquip = true;
  dress.price = 200000;
  dress.dailySkillGain.farming = 200;
  mDresses.push_back(dress);

  dress = prototype;
  dress.baseName = "Santa Costume";
  dress.name = dress.baseName;
  prototype.isWinterEquip = true;
  dress.price = 200000;
  dress.dailySkillGain.crafting = 200;
  mDresses.push_back(dress);
}
