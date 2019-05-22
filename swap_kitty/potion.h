#pragma once

#include "world.h"


class Potion
{
public:
  Potion(World& world);
  ~Potion();

  enum class Rarity { common, uncommon, rare };
  
  struct PotionItem
  {
    uint16_t id;
    std::string name;
    int64_t price;
    int16_t nutrient;
    int16_t quench;
    int16_t stamina;
    int16_t toxicity;
    World::Stat bonusStat;
  };

  PotionItem randomizePotion(const std::string& seed, Rarity rarity);
  PotionItem generatePotion(const std::string& name);

private:
  World& mWorld;

  void loadPotion();

  std::vector<PotionItem> mCommonPotions;
  std::vector<PotionItem> mUncommonPotions;
  std::vector<PotionItem> mRarePotions;

};

