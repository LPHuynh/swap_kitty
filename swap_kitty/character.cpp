#include "character.h"


Character::Character()
{
}

Character::~Character()
{
}

void Character::init(const std::string& blockHash, World& world, const std::string& characterName)
{
  name = characterName;
  money = 100000;

  primaryElement = World::Element(world.getRandomNumber(blockHash, 1 , 6));
  secondaryElement = World::Element(world.getRandomNumber(blockHash, 1, 6));

  cosmetic.init(primaryElement, secondaryElement, blockHash, world);
  weaponInventory.push_back(std::make_pair(world.generateID(), weapon.randomizeWeapon(blockHash, 500000)));
  equipedWeapon = weaponInventory.at(0).first;
  favouriteWeaponType = weaponInventory.at(0).second.type;
  favouriteFoodType = Food::FoodType::fish;

  Food::FoodItem fruitDish = food.randomizeRawFood(blockHash, Food::FoodType::fruit);
  food.randomizeCookedFood(blockHash, fruitDish);
  favouriteFruit = fruitDish.nameRaw;
  favouriteFruitDish = fruitDish.nameCooked;
  favouriteFruitDishLevel = fruitDish.dishLevel;
  favouriteVegatable = food.randomizeRawFood(blockHash, Food::FoodType::vegatable).nameRaw;
  favouriteJunkFood = food.randomizeRawFood(blockHash, Food::FoodType::junk).nameRaw;
}