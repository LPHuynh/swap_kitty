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
    case 0: dress.baseName = "A Beutiful " + dress.name; dress.bonusStat.chr += 4000; dress.price *= 4; break;
    case 1: dress.baseName = "A Pretty " + dress.name; dress.bonusStat.chr += 2000; dress.price *= 2;; break;
    case 2: dress.baseName = "A Cute " + dress.name; dress.bonusStat.chr += 1000; break;
    case 3: dress.baseName = "A Brutish " + dress.name; dress.bonusStat.str += 1000; break;
    case 4: dress.baseName = "A Light Weight " + dress.name; dress.bonusStat.dex += 1000; break;
    case 5: dress.baseName = "A Silky " + dress.name; dress.bonusStat.per += 1000; break;
    case 6: dress.baseName = "A Comfy  " + dress.name; dress.bonusStat.lrn += 1000; break;
    case 7: dress.baseName = "A Tight " + dress.name; dress.bonusStat.wil += 1000; break;
    case 8: dress.baseName = "A Magical " + dress.name; dress.bonusStat.mag += 1000; break;
    case 9: dress.baseName = "A Stuffy " + dress.name; dress.bonusSkill.fire += 1000; dress.isWinterEquip = true; break;
    case 10: dress.baseName = "A Cool " + dress.name; dress.bonusSkill.water += 1000; dress.isSummerEquip = true; break;
    case 11: dress.baseName = "A Plain " + dress.name; dress.bonusSkill.earth += 1000; break;
    case 12: dress.baseName = "A Frilly " + dress.name; dress.bonusSkill.air += 1000; break;
    case 13: dress.baseName = "A Bright " + dress.name; dress.bonusSkill.lightning += 1000; break;
    case 14: dress.baseName = "A Fluffy " + dress.name; dress.bonusSkill.holy += 1000; break;
    case 15: dress.baseName = "A Cuddly " + dress.name; dress.bonusSkill.dark += 1000; break;
    }

    if (dress.isWinterEquip && dress.isSummerEquip)
    {
      dress.isWinterEquip = false;
      dress.isSummerEquip = false;
    }
  }

  dress.price += mWorld.getRandomNumber(seed, 0, 100000);
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
  dress.name = "A " + dress.baseName;
  dress.price = 100000;
  mDresses.push_back(dress);

  dress = prototype;
  dress.baseName = "Maid Dress";
  dress.name = "A " + dress.baseName;
  dress.price = 150000;
  dress.dailySkillGain.cleaning = 200;
  mDresses.push_back(dress);

  dress = prototype;
  dress.baseName = "Apron Dress";
  dress.name = "An " + dress.baseName;
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
  dress.name = "A " + dress.baseName;
  dress.price = 150000;
  dress.dailyStatGain.con = 100;
  mDresses.push_back(dress);

  dress = prototype;
  dress.baseName = "Victorian Dress";
  dress.name = "A " + dress.baseName;
  dress.price = 150000;
  dress.dailyStatGain.chr = 100;
  mDresses.push_back(dress);

  dress = prototype;
  dress.baseName = "Hunting Dress";
  dress.name = "A " + dress.baseName;
  dress.price = 150000;
  dress.dailyStatGain.per = 100;
  mDresses.push_back(dress);

  dress = prototype;
  dress.baseName = "Ninja Suit";
  dress.name = "A " + dress.baseName;
  dress.price = 150000;
  prototype.isSummerEquip = true;
  dress.dailyStatGain.dex = 100;
  mDresses.push_back(dress);

  dress = prototype;
  dress.baseName = "Summer Dress";
  dress.name = "A " + dress.baseName;
  dress.price = 150000;
  prototype.isSummerEquip = true;
  dress.dailyStatGain.dex = 100;
  mDresses.push_back(dress);

  dress = prototype;
  dress.baseName = "Fur Coat";
  prototype.isWinterEquip = true;
  dress.name = "A " + dress.baseName;
  dress.price = 200000;
  dress.dailySkillGain.farming = 200;
  mDresses.push_back(dress);

  dress = prototype;
  dress.baseName = "Santa Costume";
  prototype.isWinterEquip = true;
  dress.name = "A " + dress.baseName;
  dress.price = 200000;
  dress.dailySkillGain.crafting = 200;
  mDresses.push_back(dress);
}
