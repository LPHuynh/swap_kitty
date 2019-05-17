#pragma once

#include <string>
#include "world.h"
#include "cosmetic.h"
#include "food.h"
#include "weapon.h"
#include "siphashrng.h"


class Character
{
public:
  Character();
  ~Character();

  enum NewGameOption { newWallet, loadWallet, restoreWallet };

  bool init(std::string characterName, std::string password, NewGameOption newGameOption, std::string seed, uint64_t restoreHeight, uint16_t latestRulesetVersion, World& world);

  void createNewCharacter(uint16_t rulesetVersion);

  Cosmetic cosmetic;

  World::Element primaryElement;
  World::Element secondaryElement;

  Weapon::WeaponItem equipedWeapon;

  Weapon::WeaponType favouriteWeaponType;
  Food::FoodType favouriteFoodType;
  std::string favouriteFruit;
  std::string favouriteFruitDish;
  int16_t favouriteFruitDishLevel;
  std::string favouriteVegatable;
  std::string favouriteJunkFood;

private:
  Weapon mWeapon;
  Food mFood;
  World mWorld;
  SiphashRNG mSiphashRNG;
};

