#pragma once

#include "book.h"
#include "cosmetic.h"
#include "dress.h"
#include "food.h"
#include "job.h"
#include "potion.h"
#include "shop.h"
#include "toy.h"
#include "weapon.h"
#include "world.h"


class Character
{
public:
  Character(World& world);
  ~Character();

  void generateNewCharacter(const std::string& seed, const std::string& characterName);

  struct Profile
  {
    std::string name;
    int64_t money;
    World::Element primaryElement;
    World::Element secondaryElement;
    Cosmetic cosmetic;
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
    int16_t domesticated;
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

  Profile profile;
  World::Stat stat;
  World::Skill skill;
  Residence residence;
  
  Weapon::WeaponItem equipedWeapon;
  Dress::DressItem equipedDress;
  std::vector<Book::BookItem> library;
  std::vector<Toy::ToyItem> playRoom;
  std::vector<Weapon::WeaponItem> weaponInventory;
  std::vector<Dress::DressItem> dressInventory;
  std::vector<Food::FoodItem> foodInventory;
  std::vector<Potion::PotionItem> potionInventory;
  std::vector<Book::BookItem> bookInventory;
  std::vector<Toy::ToyItem>ToyInventory;
  
  Job::Activity currentActivity;
  Job::Activity dailySchedule[24];

  Weapon::WeaponType favouriteWeaponType;
  std::string favouriteFruit;
  std::string favouriteFruitDish;
  int16_t favouriteFruitDishLevel;
  std::string favouriteVegatable;
  std::string favouriteJunkFood;
  Job::ActivityType favouriteActivityType;

  std::string fluffText;

  Weapon weapon;
  Dress dress;
  Food food;
  Potion potion;
  Book book;
  Toy toy;
  Job job;
  Shop shop;
private:
  World& mWorld;

  void generateStartingStats(const std::string& seed);
  void generateStartingItems(const std::string& seed);
  void generateFluffText();
};

