#pragma once

#include "world.h"


class Food
{
public:
  Food();
  ~Food();
  void init(World& world);

  enum class FoodType { fruit, vegatable, fish, corpse, flour, junk };

  struct FoodItem
  {
    uint16_t id;
    std::string nameRaw;
    std::string nameCooked;
    FoodType type;
    int16_t dishLevel;
    int64_t price;
    int16_t nutrient;
    int16_t quench;
    int16_t stamina;
    World::Stat bonusStat;
  };

  FoodItem randomizeRawFood(const std::string& seed, Food::FoodType foodType);
  void randomizeCookedFood(const std::string& seed, FoodItem& rawFood);

private:
  World mWorld;

  void loadRawFood();

  std::vector<FoodItem> mRawFruits;
  std::vector<FoodItem> mRawVegatables;
  std::vector<FoodItem> mRawFish;
  std::vector<FoodItem> mRawCorpse;
  FoodItem mRawFlour;
  std::vector<FoodItem> mRawJunk;
};