#pragma once

#include <string>
#include <vector>
#include "world.h"
#include "siphashrng.h"


class Food
{
public:
  Food();
  ~Food();

  enum FoodType { fruit, vegatable, fish, corpse, flour, junk };

  struct FoodItem
  {
    std::string nameRaw;
    std::string nameCooked;
    FoodType type;
    int16_t dishLevel;
    int64_t price;
    int16_t nutrient;
    int16_t quench;
    World::Stat bonusStat;
  };

  FoodItem randomizeFruit(std::string seed);
  FoodItem randomizeVegatable(std::string seed);
  FoodItem randomizeFish(std::string seed, int64_t maxCost);
  FoodItem randomizeJunk(std::string seed);

  FoodItem randomizeCookedFood(const std::string& seed, const FoodItem& rawFood);

private:
  std::vector<FoodItem> mRawFruits;
  std::vector<FoodItem> mRawVegatables;
  std::vector<FoodItem> mRawFish;
  std::vector<FoodItem> mRawOther;

  void loadRawFood();

  SiphashRNG mSiphashRNG;
};