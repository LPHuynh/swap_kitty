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
    int16_t cleanliness;
    int16_t happiness;
    int16_t obedience;
    World::Stat stat;
    World::Stat tempStat;
    World::Skill skill;
  };

  struct Residence
  {
    int16_t cleanliness;
    int16_t houseLevel;
    int16_t kitchenLevel;
    int16_t libraryLevel;
    int16_t bedroomLevel;
  };

  void generateNewCharacter(const std::string& seed, const std::string& characterName);
  void updateSecondaryStats();
  void consumeFood(uint16_t id, bool isLogConsumedItem);
  void consumePotion(uint16_t id, bool isLogConsumedItem);
  template<class T> bool destroyItem(std::vector<T>& source, uint16_t id, bool isSelling);
  template<class T> bool transferItem(std::vector<T>& source, std::vector<T>& destination, uint16_t id, bool isBuying);

  void updateFluffText();
  void updateStatBarText();
  void updateStatusBarText();
  void updateScheduleBoxText(uint8_t timeHour);
  void updateHouseBoxText();
  void updateCharacterSheetText();

  Profile profile;
  Residence residence;
  
  Weapon::WeaponItem equipedWeapon;
  Dress::DressItem equipedDress;
  std::vector<Book::BookItem> library;
  std::vector<Toy::ToyItem> toyRoom;
  std::vector<Weapon::WeaponItem> weaponInventory;
  std::vector<Dress::DressItem> dressInventory;
  std::vector<Food::FoodItem> foodInventory;
  std::vector<Potion::PotionItem> potionInventory;
  std::vector<Book::BookItem> bookInventory;
  std::vector<Toy::ToyItem>toyInventory;
  
  Job::Activity currentActivity;
  Job::Activity dailySchedule[24];

  Weapon::WeaponType favouriteWeaponType;
  Food::FoodItem favouriteFruitDish;
  Food::FoodItem favouriteVegatable;
  Food::FoodItem favouriteJunkFood;
  Job::ActivityType favouriteActivityType;

  std::string fluffText;
  std::string statBarText;
  std::string statusBarText;
  std::string scheduleBoxText;
  std::string houseBoxText;
  std::string characterSheetText;
  bool isCharacterSheetFluff;

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
};

template<typename T>
bool Character::destroyItem(std::vector<T>& source, uint16_t id, bool isSelling)
{
  if (id != 0)
  {
    return false;
  }

  int i = 0;
  bool isItemFound = false;

  for (auto& element : source)
  {
    if (element.id == id)
    {
      if (isSelling)
      {
        int64_t price = element.price >> 3;
        mWorld.logging.addToMainLog("\t" + element.name + " was sold for " + std::to_string(element.price * 0.01f));
      }

      isItemFound = true;
      break;
    }
    i++;
  }

  if (isItemFound)
  {
    mWorld.freeID(id);
    source.erase(source.begin() + i);
    return true;
  }
  return false;
}

template<typename T>
bool Character::transferItem(std::vector<T>& source, std::vector<T>& destination, uint16_t id, bool isBuying)
{
  if (id != 0)
  {
    return false;
  }

  int i = 0;
  bool isItemFound = false;

  for (auto& element : source)
  {
    if (element.id == id)
    {
      if (isBuying)
      {
        if (element.price < profile.money)
        {
          mWorld.logging.addToMainLog("\t" + element.name + " was purchased for " + std::to_string(element.price * 0.01f));
          destination.push_back(element);
          isItemFound = true;
        }
      }
      else
      {
        destination.push_back(element);
        isItemFound = true;
      }
      break;
    }
    i++;
  }

  if (isItemFound)
  {
    mWorld.freeID(id);
    source.erase(source.begin() + i);
    return true;
  }
  return false;
}