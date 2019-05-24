#include "character.h"


Character::Character(World& world) : mWorld(world), weapon(Weapon(world)), dress(Dress(world)), food(Food(world)), potion(Potion(world)), book(Book(world)), toy(Toy(world)), shop(Shop(world, weapon, dress, food, potion, book, toy))
{
  //Warning not all member have been initiated.
  //Calling generateNewCharacter is not possible until wallet is opened
}

Character::~Character()
{
}

void Character::generateNewCharacter(const std::string& seed, const std::string& characterName)
{
  profile.name = characterName;

  generateStartingStats(seed);
  generateStartingItems(seed);
  generateFluffText();

  shop.refreshInventory(seed);
}

void Character::generateStartingStats(const std::string& seed)
{
  profile.money = 100000;
  profile.primaryElement = World::Element(mWorld.getRandomNumber(seed, 1, 6));
  profile.secondaryElement = World::Element(mWorld.getRandomNumber(seed, 1, 6));

  profile.cosmetic.init(profile.primaryElement, profile.secondaryElement, seed, mWorld);

  profile.level = 1;
  profile.exp = 0;
  profile.stamina = 10000;
  profile.satiation = 10000;
  profile.quench = 10000;
  profile.toxicity = 0;
  profile.domesticated = mWorld.getRandomNumber(seed, 0, 2500);
  profile.cleaniness = 10000;
  profile.happiness = mWorld.getRandomNumber(seed, 2500, 5000);
  profile.obidence = mWorld.getRandomNumber(seed, 0, 2500);

  stats.str = mWorld.getRandomNumber(seed, 0, 2000);
  stats.con = mWorld.getRandomNumber(seed, 0, 2000);
  stats.dex = mWorld.getRandomNumber(seed, 0, 2000);
  stats.per = mWorld.getRandomNumber(seed, 0, 2000);
  stats.lrn = mWorld.getRandomNumber(seed, 0, 2000);
  stats.wil = mWorld.getRandomNumber(seed, 0, 2000);
  stats.mag = mWorld.getRandomNumber(seed, 0, 2000);
  stats.chr = mWorld.getRandomNumber(seed, 0, 2000);
  stats.acc = stats.per + stats.dex * 2;
  stats.cri = stats.per + stats.lrn;

  profile.maxHealth = (stats.con * 2 + stats.wil) * profile.level + 5000;
  profile.health = profile.maxHealth;
  profile.maxMana = (stats.mag * 2 + stats.wil) * profile.level + 500;
  profile.mana = profile.maxMana;  

  skills.literacy = mWorld.getRandomNumber(seed, 0, 2000);
  skills.cooking = mWorld.getRandomNumber(seed, 0, 2000);
  skills.cleaning = mWorld.getRandomNumber(seed, 0, 2000);
  skills.service = mWorld.getRandomNumber(seed, 0, 2000);
  skills.music = mWorld.getRandomNumber(seed, 0, 2000);
  skills.art = mWorld.getRandomNumber(seed, 0, 2000);
  skills.tailor = mWorld.getRandomNumber(seed, 0, 2000);
  skills.stoneWorking = mWorld.getRandomNumber(seed, 0, 2000);
  skills.woodWorking = mWorld.getRandomNumber(seed, 0, 2000);
  skills.metalworking = mWorld.getRandomNumber(seed, 0, 2000);
  skills.farming = mWorld.getRandomNumber(seed, 0, 2000);
  skills.fishing = mWorld.getRandomNumber(seed, 0, 2000);
  skills.crafting = mWorld.getRandomNumber(seed, 0, 2000);
  skills.sword = mWorld.getRandomNumber(seed, 0, 2000);
  skills.axe = mWorld.getRandomNumber(seed, 0, 2000);
  skills.bludgeon = mWorld.getRandomNumber(seed, 0, 2000);
  skills.stave = mWorld.getRandomNumber(seed, 0, 2000);
  skills.polearm = mWorld.getRandomNumber(seed, 0, 2000);
  skills.evasion = mWorld.getRandomNumber(seed, 0, 2000);
  skills.fire = mWorld.getRandomNumber(seed, 0, 2000);
  skills.water = mWorld.getRandomNumber(seed, 0, 2000);
  skills.earth = mWorld.getRandomNumber(seed, 0, 2000);
  skills.air = mWorld.getRandomNumber(seed, 0, 2000);
  skills.lightning = mWorld.getRandomNumber(seed, 0, 2000);
  skills.holy = mWorld.getRandomNumber(seed, 0, 2000);
  skills.dark = mWorld.getRandomNumber(seed, 0, 2000);
  skills.machine = mWorld.getRandomNumber(seed, 0, 2000);
  skills.poison = mWorld.getRandomNumber(seed, 0, 2000);
  skills.choas = mWorld.getRandomNumber(seed, 0, 2000);

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

  uint16_t timeOfDay = (mWorld.currentWorldHeight + mWorld.localTimeOffset) % 5760;
  uint16_t currentHour = timeOfDay / 24;
  currentActivity = dailySchedule[currentHour];
  favouriteActivityType = Job::ActivityType(mWorld.getRandomNumber(seed, 0, 3));
}

void Character::generateStartingItems(const std::string& seed)
{
  Weapon::WeaponItem startingWeapon = weapon.randomizeWeapon(seed, 500000);
  weaponInventory.push_back(startingWeapon);
  equipedWeapon = weaponInventory.at(0).id;
  favouriteWeaponType = weaponInventory.at(0).type;

  Dress::DressItem startingDress = dress.generateDress("Basic Dress");
  dressInventory.push_back(startingDress);
  equipedDress = dressInventory.at(0).id;

  Food::FoodItem fruitDish = food.randomizeRawFood(seed, Food::FoodType::fruit);
  food.randomizeCookedFood(seed, fruitDish);
  foodInventory.push_back(fruitDish);
  favouriteFruit = fruitDish.nameRaw;
  favouriteFruitDish = fruitDish.nameCooked;
  favouriteFruitDishLevel = fruitDish.dishLevel;

  for (int i = 0; i < 5; i++)
  {
    Food::FoodItem vegatable = food.randomizeRawFood(seed, Food::FoodType::vegatable);
    foodInventory.push_back( vegatable);
    favouriteVegatable = vegatable.nameRaw;
  }
  for (int i = 0; i < 5; i++)
  {
    Food::FoodItem flour = food.randomizeRawFood(seed, Food::FoodType::flour);
    foodInventory.push_back(flour);
  }

  Food::FoodItem junkfood = food.randomizeRawFood(seed, Food::FoodType::junkFood);
  favouriteJunkFood = junkfood.nameRaw;
  mWorld.freeID(junkfood.id);

  for (int i = 0; i < 10; i++)
  {
    Potion::PotionItem startingPotion = potion.randomizePotion(seed, Potion::Rarity::common);
  }

  for (int i = 0; i < 3; i++)
  {
    Book::BookItem startingBook = book.randomizeBook(seed);
    library.push_back(startingBook.id);
  }

  for (int i = 0; i < 3; i++)
  {
    Toy::ToyItem startingToy = toy.randomizeToy(seed);
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
    elementText = "She is shrouded in an aura of " + primaryElementText + " and " + secondaryElementText + ".\n\n";
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
  case Weapon::WeaponType::axe: weaponText = "can easily find ways around locked doors and chests.\n"; break;
  case Weapon::WeaponType::bludgeon: weaponText = "loves fixings things using percussive repair.\n"; break;
  case Weapon::WeaponType::stave: weaponText = "loves collecting twigs and tree branch in her spare time.\n"; break;
  case Weapon::WeaponType::polearm: weaponText = "loves poking at things with a long pointy stick.\n"; break;
  default: weaponText = ""; break;
  }

  fluffText = profile.name + " is a " + std::to_string(profile.cosmetic.age) + " year old " + profile.cosmetic.gender + " " + profile.cosmetic.species + " "
    + "with " + profile.cosmetic.currentHairColour + " hair and a " + profile.cosmetic.getSkinToneDescription(profile.cosmetic.currentSkinTone) + " complexion.\n"
    + "She weighs " + std::to_string(profile.cosmetic.weight / 1000) + "kg and stands " + std::to_string(profile.cosmetic.height / 10) + "cm tall.\n\n"
    + "Her favourite food are " + favouriteVegatable + ", " + favouriteFruitDish + ", and " + favouriteJunkFood + ".\n"
    + activityText + weaponText + "\n"
    + elementText;
}
