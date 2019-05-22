#include "character.h"


Character::Character(World& world) : mWorld(world), weapon(Weapon(world)), dress(Dress(world)), food(Food(world)), potion(Potion(world)), book(Book(world)), toy(Toy(world))
{
  //Warning not all member have been initiated.
  //Calling generateNewCharacter is not possible until wallet is opened
}

Character::~Character()
{
}

void Character::generateNewCharacter(const std::string& blockHash, const std::string& characterName)
{
  profile.name = characterName;

  generateStartingStats(blockHash);
  generateStartingItems(blockHash);
  generateFluffText();
}

void Character::generateStartingStats(const std::string& blockHash)
{
  profile.money = 100000;
  profile.primaryElement = World::Element(mWorld.getRandomNumber(blockHash, 1, 6));
  profile.secondaryElement = World::Element(mWorld.getRandomNumber(blockHash, 1, 6));

  profile.cosmetic.init(profile.primaryElement, profile.secondaryElement, blockHash, mWorld);

  profile.level = 1;
  profile.exp = 0;
  profile.stamina = 10000;
  profile.satiation = 10000;
  profile.quench = 10000;
  profile.toxicity = 0;
  profile.domesticated = mWorld.getRandomNumber(blockHash, 0, 2500);
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

  dailySchedule[0] = job.getActivityID("Sleep");
  dailySchedule[1] = job.getActivityID("Sleep");
  dailySchedule[2] = job.getActivityID("Sleep");
  dailySchedule[3] = job.getActivityID("Sleep");
  dailySchedule[4] = job.getActivityID("Cooking");
  dailySchedule[5] = job.getActivityID("Cleaning");
  dailySchedule[6] = job.getActivityID("Nap");
  dailySchedule[7] = job.getActivityID("Playing");
  dailySchedule[8] = job.getActivityID("Nap");
  dailySchedule[9] = job.getActivityID("Playing");
  dailySchedule[10] = job.getActivityID("Read Book");
  dailySchedule[11] = job.getActivityID("Cooking");
  dailySchedule[12] = job.getActivityID("Cleaning");
  dailySchedule[13] = job.getActivityID("Nap");
  dailySchedule[14] = job.getActivityID("Read Book");
  dailySchedule[15] = job.getActivityID("Bath");
  dailySchedule[16] = job.getActivityID("Cooking");
  dailySchedule[17] = job.getActivityID("Cleaning");
  dailySchedule[18] = job.getActivityID("Playing");
  dailySchedule[19] = job.getActivityID("Bath");
  dailySchedule[20] = job.getActivityID("Read Book");
  dailySchedule[21] = job.getActivityID("Sleep");
  dailySchedule[22] = job.getActivityID("Sleep");
  dailySchedule[23] = job.getActivityID("Sleep");
  dailySchedule[24] = job.getActivityID("Sleep");

  favouriteActivityType = Job::ActivityType(mWorld.getRandomNumber(blockHash, 0, 3));
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

  for (int i = 0; i < 10; i++)
  {
    Potion::PotionItem startingPotion = potion.randomizePotion(blockHash, Potion::Rarity::common);
  }

  for (int i = 0; i < 3; i++)
  {
    Book::BookItem startingBook = book.randomizeBook(blockHash);
    library.push_back(startingBook.id);
  }

  for (int i = 0; i < 3; i++)
  {
    Toy::ToyItem startingToy = toy.randomizeToy(blockHash);
    playRoom.push_back(startingToy.id);
  }
}

void Character::generateFluffText()
{
  std::string elementText;
  std::string primaryElementText;
  std::string secondaryElementText;

  switch (profile.primaryElement)
  {
  case World::Element::fire: primaryElementText = "Fire"; break;
  case World::Element::water: primaryElementText = "Water"; break;
  case World::Element::earth: primaryElementText = "Earth"; break;
  case World::Element::air: primaryElementText = "Air"; break;
  case World::Element::lightning: primaryElementText = "Lightning"; break;
  case World::Element::holy: primaryElementText = "Holiness"; break;
  case World::Element::dark: primaryElementText = "Darkness"; break;
  default: primaryElementText = ""; break;
  }

  switch (profile.secondaryElement)
  {
  case World::Element::fire: secondaryElementText = "Fire"; break;
  case World::Element::water: secondaryElementText = "Water"; break;
  case World::Element::earth: secondaryElementText = "Earth"; break;
  case World::Element::air: secondaryElementText = "Air"; break;
  case World::Element::lightning: secondaryElementText = "Lightning"; break;
  case World::Element::holy: secondaryElementText = "Holiness"; break;
  case World::Element::dark: secondaryElementText = "Darkness"; break;
  default: secondaryElementText = ""; break;
  }

  if (primaryElementText == secondaryElementText)
  {
    elementText = "She is shrouded in an aura of pure " + primaryElementText;
  }
  else
  {
    elementText = "She is shrouded in an aura " + primaryElementText + " and " + secondaryElementText + ".\n\n";
  }

  std::string activityText;

  switch (favouriteActivityType)
  {
  case Job::ActivityType::domestic: activityText = "She finds cooking and cleaning enjoyable, and "; break;
  case Job::ActivityType::entertainment: activityText = "She finds entertainment enjoyable, and "; break;
  case Job::ActivityType::agriculture: activityText = "She finds farming and fishing enjoyable, and "; break;
  case Job::ActivityType::heavyIndustry: activityText = "She finds hard work enjoyable, and "; break;
  default: activityText = ""; break;
  }

  std::string weaponText;

  switch (favouriteWeaponType)
  {
  case Weapon::WeaponType::sword: weaponText = "never leaves the house without a good pocket knife.\n"; break;
  case Weapon::WeaponType::axe: weaponText = "easily find ways around locked doors and chests.\n"; break;
  case Weapon::WeaponType::bludgeon: weaponText = "loves fixings things using percussive repair.\n"; break;
  case Weapon::WeaponType::stave: weaponText = "loves collecting twigs and tree branch in her spare time.\n"; break;
  case Weapon::WeaponType::polearm: weaponText = "loves poking at things with a long pointy stick.\n"; break;
  default: weaponText = ""; break;
  }

  fluffText = profile.name + " is a " + std::to_string(profile.cosmetic.age) + " year old " + profile.cosmetic.gender + " " + profile.cosmetic.species + ".\n"
    + "She weighs " + std::to_string(profile.cosmetic.weight / 1000) + "kg and stands " + std::to_string(profile.cosmetic.height/10) + "cm tall.\n"
    + "She has " + profile.cosmetic.currentHairColour + " hair and a " + profile.cosmetic.getSkinToneDescription(profile.cosmetic.currentSkinTone) + " complexion.\n"
    + elementText
    + "She's a " + profile.cosmetic.species + ", so of courses she loves fish. Otherwise, she enjoys munching on " + favouriteVegatable + ", " + favouriteFruitDish + ", or " + favouriteJunkFood + ".\n"
    + activityText + weaponText;
}
