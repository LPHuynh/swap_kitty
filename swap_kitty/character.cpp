#include "character.h"

#include <iostream>
#include <thread>
#include <chrono>


Character::Character()
{
}

Character::~Character()
{
  mWorld.walletAPI.closeWallet();
  std::this_thread::sleep_for(std::chrono::seconds(5));
}

bool Character::init(std::string characterName, std::string password, NewGameOption newGameOption, std::string seed, uint64_t restoreHeight, uint16_t latestRulesetVersion, World& world)
{
  mWorld = world;

  if (newGameOption == NewGameOption::newWallet)
  {
    if (!world.walletAPI.createWallet(characterName, password, "English"))
    {
      printf("Error restoring wallet.\n");
      return false;
    }
  }
  else if (newGameOption == NewGameOption::restoreWallet)
  {
    if (!world.walletAPI.restoreWallet(characterName, password, seed, "English", restoreHeight))
    {
      printf("Error opening wallet.\n");
      return false;
    }
  }
  else
  {
    if (!world.walletAPI.openWallet(characterName, password))
    {
      printf("Error opening wallet.\n");
      return false;
    }
  }

  uint64_t daemonHeight = world.daemonAPI.getBlockCount();
  uint64_t walletHeight = world.walletAPI.getBlockHeight();

  while ( daemonHeight > walletHeight)
  {
    std::cout << "Waiting for wallet to fully sync. Current Daemon Height: " << daemonHeight << ". Current Wallet Height: " << walletHeight;
    std::this_thread::sleep_for(std::chrono::seconds(5));
  }

  //TODO: Scan wallet for transaction for any commands.
  //      If command is detected, but no character creation command found, output warning.
  //      If no command found create a new character
  
  createNewCharacter(latestRulesetVersion);

  return true;
}

void Character::createNewCharacter(uint16_t latestRulesetVersion)
{
  mWorld.currentRulesetVersion = latestRulesetVersion;

  primaryElement = World::Element(mWorld.randomizePlayerElement(mWorld.walletAPI.getAddress() + "0"));
  secondaryElement = World::Element(mWorld.randomizePlayerElement(mWorld.walletAPI.getAddress() + "1"));

  cosmetic.init(primaryElement, secondaryElement, mWorld.walletAPI.getAddress(),mWorld);

  equipedWeapon = mWeapon.randomizeWeapon(mWorld.walletAPI.getAddress(), 500000);
  favouriteWeaponType = equipedWeapon.type;
  favouriteFoodType = Food::FoodType::fish;

  Food::FoodItem fruitDish = mFood.randomizeCookedFood(mWorld.walletAPI.getAddress(), mFood.randomizeFruit(mWorld.walletAPI.getAddress()));

  favouriteFruit = fruitDish.nameRaw;
  favouriteFruitDish = fruitDish.nameCooked;
  favouriteFruitDishLevel = fruitDish.dishLevel;
  favouriteVegatable = mFood.randomizeVegatable(mWorld.walletAPI.getAddress()).nameRaw;
  favouriteJunkFood = mFood.randomizeJunk(mWorld.walletAPI.getAddress()).nameRaw;
}