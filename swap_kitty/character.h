#pragma once

#include "book.h"
#include "cosmetic.h"
#include "dress.h"
#include "food.h"
#include "gift.h"
#include "potion.h"
#include "weapon.h"
#include "world.h"


class Character
{
public:
  Character();
  ~Character();
  void init(const std::string& blockHash, World& world, const std::string& characterName);

  struct Profile
  {
    std::string name;
    int64_t money;
    World::Element primaryElement;
    World::Element secondaryElement;
    Cosmetic cosmetic;
    int16_t age;
    int16_t height;    //height in mm
    int16_t weight;    //weight in g
    int16_t level;
    int16_t exp;
    int16_t health;
    int16_t maxHealth;
    int16_t mana;
    int16_t maxMana;
    int16_t stamina;
    int16_t satiation;
    int16_t quench;
    int16_t toxicity;
    int16_t refinement;
    int16_t cleaniness;
    int16_t happiness;
    int16_t obidence;
  };

  struct Residence
  {
    int16_t cleaniness;
    int16_t houseLevel;
    int16_t kitchenLevel;
    int16_t libraryLevel;
    int16_t bedroomLevel;
  };

  void generateStartingStats(const std::string& blockHash);
  void generateStartingItems(const std::string& blockHash);

  Profile profile;
  World::Stat stats;
  World::Skill skills;
  Residence residence;
  
  uint16_t equipedWeapon;
  uint16_t equipedDress;
  std::vector<uint16_t> library;
  std::vector<uint16_t> playRoom;
  std::vector<std::pair<uint16_t, Weapon::WeaponItem>> weaponInventory;
  std::vector<std::pair <uint16_t, Dress::DressItem>> dressInventory;
  std::vector<std::pair <uint16_t, Food::FoodItem>> foodInventory;
  std::vector<std::pair <uint16_t, Potion::PotionItem>> potionInventory;
  std::vector<std::pair <uint16_t, Book::BookItem>> bookInventory;
  std::vector<std::pair <uint16_t, Gift::GiftItem>> GiftInventory;

  Weapon::WeaponType favouriteWeaponType;
  Food::FoodType favouriteFoodType;
  std::string favouriteFruit;
  std::string favouriteFruitDish;
  int16_t favouriteFruitDishLevel;
  std::string favouriteVegatable;
  std::string favouriteJunkFood;

  Weapon weapon;
  Dress dress;
  Food food;
  Potion potion;
  Book book;
  Gift gift;
private:
  World mWorld;
};

