#include "character.h"


Character::Character()
{
}

Character::~Character()
{
}

void Character::init(const std::string& blockHash, World& world, const std::string& characterName)
{
  profile.name = characterName;

  mWorld = world;
  weapon.init(mWorld);
  dress.init(mWorld);
  food.init(mWorld);
  book.init(mWorld);
  gift.init(mWorld);

  generateStartingStats(blockHash);
  generateStartingItems(blockHash);
}

void Character::generateStartingStats(const std::string& blockHash)
{
  profile.money = 100000;
  profile.primaryElement = World::Element(mWorld.getRandomNumber(blockHash, 1, 6));
  profile.secondaryElement = World::Element(mWorld.getRandomNumber(blockHash, 1, 6));

  profile.cosmetic.init(profile.primaryElement, profile.secondaryElement, blockHash, mWorld);

  profile.age = 10;
  profile.height = 1200 + mWorld.getRandomNumber(blockHash, 0, 80);
  profile.weight = 26000 + mWorld.getRandomNumber(blockHash, 0, 4000);
  profile.level = 1;
  profile.exp = 0;
  profile.stamina = 10000;
  profile.satiation = 10000;
  profile.quench = 10000;
  profile.toxicity = 0;
  profile.refinement = mWorld.getRandomNumber(blockHash, 0, 2500);
  profile.cleaniness = 10000;
  profile.happiness = mWorld.getRandomNumber(blockHash, 2500, 5000);
  profile.obidence = mWorld.getRandomNumber(blockHash, 0, 2500);

  stats.Str = mWorld.getRandomNumber(blockHash, 0, 2000);
  stats.Con = mWorld.getRandomNumber(blockHash, 0, 2000);
  stats.Dex = mWorld.getRandomNumber(blockHash, 0, 2000);
  stats.Per = mWorld.getRandomNumber(blockHash, 0, 2000);
  stats.Lrn = mWorld.getRandomNumber(blockHash, 0, 2000);
  stats.Wil = mWorld.getRandomNumber(blockHash, 0, 2000);
  stats.Mag = mWorld.getRandomNumber(blockHash, 0, 2000);
  stats.Chr = mWorld.getRandomNumber(blockHash, 0, 2000);
  stats.Acc = stats.Per + stats.Dex * 2;
  stats.Cri = stats.Per + stats.Lrn;

  profile.maxHealth = (stats.Con * 2 + stats.Wil) * profile.level + 5000;
  profile.health = profile.maxHealth;
  profile.maxMana = (stats.Mag * 2 + stats.Wil) * profile.level + 500;
  profile.mana = profile.maxMana;  

  skills.literacy = mWorld.getRandomNumber(blockHash, 0, 2000);
  skills.cooking = mWorld.getRandomNumber(blockHash, 0, 2000);
  skills.cleaning = mWorld.getRandomNumber(blockHash, 0, 2000);
  skills.service = mWorld.getRandomNumber(blockHash, 0, 2000);
  skills.music = mWorld.getRandomNumber(blockHash, 0, 2000);
  skills.art = mWorld.getRandomNumber(blockHash, 0, 2000);
  skills.tailor = mWorld.getRandomNumber(blockHash, 0, 2000);
  skills.stoneWorking = mWorld.getRandomNumber(blockHash, 0, 2000);
  skills.woodWorking = mWorld.getRandomNumber(blockHash, 0, 2000);
  skills.metalworking = mWorld.getRandomNumber(blockHash, 0, 2000);
  skills.farming = mWorld.getRandomNumber(blockHash, 0, 2000);
  skills.fishing = mWorld.getRandomNumber(blockHash, 0, 2000);
  skills.crafting = mWorld.getRandomNumber(blockHash, 0, 2000);
  skills.sword = mWorld.getRandomNumber(blockHash, 0, 2000);
  skills.axe = mWorld.getRandomNumber(blockHash, 0, 2000);
  skills.bludgeon = mWorld.getRandomNumber(blockHash, 0, 2000);
  skills.stave = mWorld.getRandomNumber(blockHash, 0, 2000);
  skills.polearm = mWorld.getRandomNumber(blockHash, 0, 2000);
  skills.evasion = mWorld.getRandomNumber(blockHash, 0, 2000);
  skills.fire = mWorld.getRandomNumber(blockHash, 0, 2000);
  skills.water = mWorld.getRandomNumber(blockHash, 0, 2000);
  skills.earth = mWorld.getRandomNumber(blockHash, 0, 2000);
  skills.air = mWorld.getRandomNumber(blockHash, 0, 2000);
  skills.lightning = mWorld.getRandomNumber(blockHash, 0, 2000);
  skills.holy = mWorld.getRandomNumber(blockHash, 0, 2000);
  skills.dark = mWorld.getRandomNumber(blockHash, 0, 2000);
  skills.machine = mWorld.getRandomNumber(blockHash, 0, 2000);
  skills.poison = mWorld.getRandomNumber(blockHash, 0, 2000);
  skills.choas = mWorld.getRandomNumber(blockHash, 0, 2000);

  residence.cleaniness = 10000;
  residence.houseLevel = 1;
  residence.kitchenLevel = 1;
  residence.libraryLevel = 1;
  residence.bedroomLevel = 1;
}

void Character::generateStartingItems(const std::string& blockHash)
{
  Weapon::WeaponItem startingWeapon = weapon.randomizeWeapon(blockHash, 500000);
  weaponInventory.push_back(std::make_pair(startingWeapon.id, startingWeapon));
  equipedWeapon = weaponInventory.at(0).first;
  favouriteWeaponType = weaponInventory.at(0).second.type;

  Dress::DressItem startingDress = dress.generateDress("Basic Dress");
  dressInventory.push_back(std::make_pair(startingDress.id, startingDress));
  equipedDress = dressInventory.at(0).first;

  favouriteFoodType = Food::FoodType::fish;
  Food::FoodItem fruitDish = food.randomizeRawFood(blockHash, Food::FoodType::fruit);
  food.randomizeCookedFood(blockHash, fruitDish);
  foodInventory.push_back(std::make_pair(fruitDish.id, fruitDish));
  favouriteFruit = fruitDish.nameRaw;
  favouriteFruitDish = fruitDish.nameCooked;
  favouriteFruitDishLevel = fruitDish.dishLevel;

  for (int i = 0; i < 5; i++)
  {
    Food::FoodItem vegatable = food.randomizeRawFood(blockHash, Food::FoodType::vegatable);
    foodInventory.push_back(std::make_pair(vegatable.id, vegatable));
    favouriteVegatable = vegatable.nameRaw;
  }
  for (int i = 0; i < 5; i++)
  {
    Food::FoodItem flour = food.randomizeRawFood(blockHash, Food::FoodType::flour);
    foodInventory.push_back(std::make_pair(flour.id, flour));
  }

  Food::FoodItem junkfood = food.randomizeRawFood(blockHash, Food::FoodType::junk);
  favouriteJunkFood = junkfood.nameRaw;
  mWorld.freeID(junkfood.id);
}
