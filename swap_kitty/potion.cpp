#include "potion.h"


Potion::Potion(World & world) : mWorld(world)
{
  loadPotion();
}

Potion::~Potion()
{
}


Potion::PotionItem Potion::randomizePotion(const std::string& seed, Rarity rarity)
{
  PotionItem potion;

  switch (rarity)
  {
  case Rarity::common: potion = mCommonPotions.at(mWorld.getRandomNumber(seed, 1, 4)); break;
  case Rarity::uncommon: potion = mUncommonPotions.at(mWorld.getRandomNumber(seed, 0, 2)); break;
  case Rarity::rare: potion = mRarePotions.at(mWorld.getRandomNumber(seed, 0, 0)); break;
  }

  potion.price += mWorld.getRandomNumber(seed, 0, 100);
  potion.id = mWorld.generateID();
  return potion;
}

Potion::PotionItem Potion::generatePotion(const std::string& seed, const std::string& name)
{
  Potion::PotionItem potion;

  for (auto& element : mCommonPotions)
  {
    if (element.name == name)
    {
      potion = element;
      potion.price += mWorld.getRandomNumber(seed, 0, 100);
      potion.id = mWorld.generateID();
      return potion;
    }
  }
  for (auto& element : mUncommonPotions)
  {
    if (element.name == name)
    {
      potion = element;
      potion.price += mWorld.getRandomNumber(seed, 0, 100);
      potion.id = mWorld.generateID();
      return potion;
    }
  }
  for (auto& element : mRarePotions)
  {
    if (element.name == name)
    {
      potion = element;
      potion.price += mWorld.getRandomNumber(seed, 0, 100);
      potion.id = mWorld.generateID();
      return potion;
    }
  }
  potion = mCommonPotions.at(0);
  potion.price += mWorld.getRandomNumber(seed, 0, 100);
  potion.id = mWorld.generateID();
  return potion;
}

void Potion::loadPotion()
{
  PotionItem prototype;
  prototype.id = 0;
  prototype.name = "";
  prototype.price = 0;
  prototype.nutrient = 0;
  prototype.quench = 0;
  prototype.stamina = 0;
  prototype.toxicity = 0;
  prototype.bonusTempStat = World::Stat{ 0,0,0,0,0,0,0,0,0,0 };

  PotionItem potion;

  //Ruleset v1: common potion slot 0-4
  potion = prototype;
  potion.name = "Pond Water";
  potion.price = 10;
  potion.nutrient = 50;
  potion.quench = 4000;
  potion.toxicity = 100;
  mCommonPotions.push_back(potion);

  potion = prototype;
  potion.name = "Water";
  potion.price = 10;
  potion.quench = 6000;
  mCommonPotions.push_back(potion);

  potion = prototype;
  potion.name = "Milk";
  potion.price = 500;
  potion.nutrient = 20;
  potion.quench = 8000;
  potion.stamina = 400;
  mCommonPotions.push_back(potion);

  potion = prototype;
  potion.name = "Juice";
  potion.price = 400;
  potion.nutrient = 150;
  potion.quench = 7000;
  potion.stamina = 400;
  mCommonPotions.push_back(potion);

  potion = prototype;
  potion.name = "Soda";
  potion.price = 500;
  potion.nutrient = 150;
  potion.quench = 6000;
  potion.stamina = 500;
  mCommonPotions.push_back(potion);

  //Ruleset v1: uncommon potion slot 0-2
  potion = prototype;
  potion.name = "Green Tea";
  potion.price = 500;
  potion.quench = 8000;
  potion.toxicity = -25;
  mUncommonPotions.push_back(potion);

  potion = prototype;
  potion.name = "Coffee";
  potion.price = 900;
  potion.nutrient = 150;
  potion.quench = 3000;
  potion.stamina = 1000;
  potion.toxicity = 50;
  mUncommonPotions.push_back(potion);

  potion = prototype;
  potion.name = "Energy Drink";
  potion.price = 1000;
  potion.nutrient = 250;
  potion.quench = 4000;
  potion.stamina = 2000;
  potion.toxicity = 100;
  potion.bonusTempStat.str = 1000;
  potion.bonusTempStat.wil = 1000;
  mUncommonPotions.push_back(potion);

  //Ruleset v1: rare potion slot 0-0
  potion = prototype;
  potion.name = "Potential";
  potion.price = 5000;
  potion.nutrient = 50;
  potion.quench = 2000;
  potion.stamina = 0;
  potion.toxicity = 200;
  potion.bonusTempStat = World::Stat{ 1000,1000,1000,1000,1000,1000,1000,1000,1000,1000 };
  mRarePotions.push_back(potion);
}
