#pragma once

#include "world.h"


class Food
{
public:
  Food();
  ~Food();

  enum class FoodType { fruit, vegatable, fish, corpse, flour, junk };

  struct FoodItem
  {
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

  FoodItem randomizeFruit(const std::string& seed);
  FoodItem randomizeVegatable(const std::string& seed);
  FoodItem randomizeFish(const std::string& seed, int64_t maxCost);
  FoodItem randomizeJunk(const std::string& seed);
  FoodItem randomizeCookedFood(const std::string& seed, const FoodItem& rawFood);

private:
  std::vector<FoodItem> mRawFruits;
  std::vector<FoodItem> mRawVegatables;
  std::vector<FoodItem> mRawFish;
  std::vector<FoodItem> mRawOther;

  void loadRawFood();
};