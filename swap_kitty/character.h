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
  void init(const std::string& blockHash, World& world, const std::string& characterName);

  std::string name;
  int64_t money;

  Cosmetic cosmetic;

  World::Element primaryElement;
  World::Element secondaryElement;

  uint16_t equipedWeapon;
  //Dress::DressItem equipedDress;
  std::vector<std::pair<uint16_t, Weapon::WeaponItem>> weaponInventory;
  //std::vector<std::pair <uint16_t, Dress::DressItem>> dressInventory;
  std::vector<std::pair <uint16_t, Food::FoodItem>> foodInventory;
  //std::vector<std::pair <uint16_t, Potion::PotionItem>> potionInventory;
  //std::vector<std::pair <uint16_t, Book::BookItem>> bookInventory;
  //std::vector<std::pair <uint16_t, Toy::ToyItem>> ToyInventory;
  //std::vector<std::pair <uint16_t, Gift::GiftItem>> GiftInventory;

  Weapon::WeaponType favouriteWeaponType;
  Food::FoodType favouriteFoodType;
  std::string favouriteFruit;
  std::string favouriteFruitDish;
  int16_t favouriteFruitDishLevel;
  std::string favouriteVegatable;
  std::string favouriteJunkFood;

  Weapon weapon;
  Food food;
};

