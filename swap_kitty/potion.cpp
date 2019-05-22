#include "potion.h"


Potion::Potion()
{
}

Potion::~Potion()
{
}

void Potion::init(World& world)
{
  mWorld = world;
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

  potion.id = mWorld.generateID();
  return potion;
}

Potion::PotionItem Potion::generatePotion(const std::string& name)
{
  Potion::PotionItem potion;

  for (auto& element : mCommonPotions)
  {
    if (element.name == name)
    {
      potion = element;
      potion.id = mWorld.generateID();
      return potion;
    }
  }
  for (auto& element : mUncommonPotions)
  {
    if (element.name == name)
    {
      potion = element;
      potion.id = mWorld.generateID();
      return potion;
    }
  }
  for (auto& element : mRarePotions)
  {
    if (element.name == name)
    {
      potion = element;
      potion.id = mWorld.generateID();
      return potion;
    }
  }
  potion = mCommonPotions.at(0);
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
  prototype.bonusStat = World::Stat{ 0,0,0,0,0,0,0,0,0,0 };

  PotionItem potion;

  //Ruleset v1: common potion slot 0-4
  potion = prototype;
  potion.name = "Pond Water";
  potion.price = 10;
  potion.nutrient = 5;
  potion.quench = 400;
  potion.toxicity = 10;
  mCommonPotions.push_back(potion);

  potion = prototype;
  potion.name = "Water";
  potion.price = 10;
  potion.quench = 600;
  mCommonPotions.push_back(potion);

  potion = prototype;
  potion.name = "Milk";
  potion.price = 500;
  potion.nutrient = 20;
  potion.quench = 800;
  potion.stamina = 40;
  mCommonPotions.push_back(potion);

  potion = prototype;
  potion.name = "Juice";
  potion.price = 400;
  potion.nutrient = 15;
  potion.quench = 700;
  potion.stamina = 40;
  mCommonPotions.push_back(potion);

  potion = prototype;
  potion.name = "Soda";
  potion.price = 500;
  potion.nutrient = 15;
  potion.quench = 600;
  potion.stamina = 50;
  mCommonPotions.push_back(potion);

  //Ruleset v1: uncommon potion slot 0-2
  potion = prototype;
  potion.name = "Coffee";
  potion.price = 900;
  potion.nutrient = 15;
  potion.quench = 300;
  potion.stamina = 100;
  potion.toxicity = 5;
  mUncommonPotions.push_back(potion);

  potion = prototype;
  potion.name = "Beer";
  potion.price = 800;
  potion.nutrient = 25;
  potion.quench = 300;
  potion.stamina = 30;
  potion.toxicity = 20;
  mUncommonPotions.push_back(potion);

  potion = prototype;
  potion.name = "Energy Drink";
  potion.price = 1000;
  potion.nutrient = 25;
  potion.quench = 400;
  potion.stamina = 200;
  potion.toxicity = 10;
  mUncommonPotions.push_back(potion);

  //Ruleset v1: rare potion slot 0-0
  potion = prototype;
  potion.name = "Potential";
  potion.price = 5000;
  potion.nutrient = 5;
  potion.quench = 400;
  potion.stamina = 0;
  potion.toxicity = 10;
  potion.bonusStat = World::Stat{ 100,100,100,100,100,100,100,100,100,100 };
  mUncommonPotions.push_back(potion);
}
