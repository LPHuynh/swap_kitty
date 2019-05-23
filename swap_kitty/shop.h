#pragma once

#include "world.h"

#include "book.h"
#include "dress.h"
#include "food.h"
#include "potion.h"
#include "toy.h"
#include "weapon.h"


class Shop
{
public:
  Shop(World& world, Weapon& weapon, Dress& dress, Food& food, Potion& potion, Book& book, Toy& toy);
  ~Shop();

  std::vector<Weapon::WeaponItem> weaponInventory;
  std::vector<Dress::DressItem> dressInventory;
  std::vector<Food::FoodItem> fruitInventory;
  std::vector<Food::FoodItem> vegatableInventory;
  std::vector<Food::FoodItem> fishInventory;
  std::vector<Food::FoodItem> corpseInventory;
  std::vector<Food::FoodItem> flourInventory;
  std::vector<Food::FoodItem> junkfoodInventory;
  std::vector<Potion::PotionItem> potionInventory;
  std::vector<Book::BookItem> bookInventory;
  std::vector<Toy::ToyItem> toyInventory;

  void refreshInventory(const std::string& seed);

private:
  World& mWorld;
  Weapon& mWeapon;
  Dress& mDress;
  Food& mFood;
  Potion& mPotion;
  Book& mBook;
  Toy& mToy;
};

