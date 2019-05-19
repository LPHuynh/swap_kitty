#include "character.h"


Character::Character()
{
}

Character::~Character()
{
}

void Character::init(const std::string& walletAddress, World& world)
{
  money = 100000;

  primaryElement = World::Element(World::getRandomNumber(walletAddress + "playerelement__0", 1 , 6));
  secondaryElement = World::Element(World::getRandomNumber(walletAddress + "playerelement__1", 1, 6));

  cosmetic.init(primaryElement, secondaryElement, walletAddress);

  equipedWeapon = mWeapon.randomizeWeapon(walletAddress, 500000);
  favouriteWeaponType = equipedWeapon.type;
  favouriteFoodType = Food::FoodType::fish;

  Food::FoodItem fruitDish = mFood.randomizeCookedFood(walletAddress, mFood.randomizeFruit(walletAddress));

  favouriteFruit = fruitDish.nameRaw;
  favouriteFruitDish = fruitDish.nameCooked;
  favouriteFruitDishLevel = fruitDish.dishLevel;
  favouriteVegatable = mFood.randomizeVegatable(walletAddress).nameRaw;
  favouriteJunkFood = mFood.randomizeJunk(walletAddress).nameRaw;
}