#pragma once

#include "cosmetic.h"
#include "food.h"
#include "weapon.h"
#include "world.h"


class Character
{
public:
  Character();
  ~Character();
  void init(const std::string& walletAddress, World& world);

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

  int64_t money;

private:
  Weapon mWeapon;
  Food mFood;
};

