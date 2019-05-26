#include "shop.h"


Shop::Shop(World& world, Weapon& weapon, Dress& dress, Food& food, Potion& potion, Book& book, Toy& toy) : mWorld(world), mWeapon(weapon), mDress(dress), mFood(food), mPotion(potion), mBook(book), mToy(toy)
{
}

Shop::~Shop()
{
}

void Shop::refreshInventory(const std::string & seed)
{
  weaponInventory.clear();
  dressInventory.clear();
  fruitInventory.clear();
  vegatableInventory.clear();
  fishInventory.clear();
  corpseInventory.clear();
  flourInventory.clear();
  junkfoodInventory.clear();
  potionInventory.clear();
  bookInventory.clear();
  toyInventory.clear();

  for (int i = 0; i < 5; i++)
  {
    dressInventory.push_back(mDress.randomizeDress(seed));
    fruitInventory.push_back(mFood.randomizeRawFood(seed, Food::FoodType::fruit));
    vegatableInventory.push_back(mFood.randomizeRawFood(seed, Food::FoodType::vegatable));
    fishInventory.push_back(mFood.randomizeRawFood(seed, Food::FoodType::fish));
    corpseInventory.push_back(mFood.randomizeRawFood(seed, Food::FoodType::corpse));
    flourInventory.push_back(mFood.randomizeRawFood(seed, Food::FoodType::flour));
    potionInventory.push_back(mPotion.randomizePotion(seed, Potion::Rarity::common));
    potionInventory.push_back(mPotion.randomizePotion(seed, Potion::Rarity::uncommon));
    bookInventory.push_back(mBook.randomizeBook(seed));
    toyInventory.push_back(mToy.randomizeToy(seed));
  }
  for (int i = 0; i < 2; i++)
  {
    weaponInventory.push_back(mWeapon.randomizeWeapon(seed, 500000));
    weaponInventory.push_back(mWeapon.randomizeWeapon(seed, 1000000));   
    weaponInventory.push_back(mWeapon.randomizeWeapon(seed, uint64_t(-1)));
    junkfoodInventory.push_back(mFood.randomizeRawFood(seed, Food::FoodType::junkFood));

    mFood.randomizeCookedFood(seed, fruitInventory.at(i));
    mFood.randomizeCookedFood(seed, vegatableInventory.at(i));
    mFood.randomizeCookedFood(seed, fishInventory.at(i));
    mFood.randomizeCookedFood(seed, corpseInventory.at(i));
    mFood.randomizeCookedFood(seed, flourInventory.at(i));
  }
}
