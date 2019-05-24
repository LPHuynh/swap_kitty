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

  stat.str = mWorld.getRandomNumber(seed, 0, 2000);
  stat.con = mWorld.getRandomNumber(seed, 0, 2000);
  stat.dex = mWorld.getRandomNumber(seed, 0, 2000);
  stat.per = mWorld.getRandomNumber(seed, 0, 2000);
  stat.lrn = mWorld.getRandomNumber(seed, 0, 2000);
  stat.wil = mWorld.getRandomNumber(seed, 0, 2000);
  stat.mag = mWorld.getRandomNumber(seed, 0, 2000);
  stat.chr = mWorld.getRandomNumber(seed, 0, 2000);
  stat.acc = stat.per + stat.dex * 2;
  stat.cri = stat.per + stat.lrn;

  profile.maxHealth = (stat.con * 2 + stat.wil) * profile.level + 5000;
  profile.health = profile.maxHealth;
  profile.maxMana = (stat.mag * 2 + stat.wil) * profile.level + 500;
  profile.mana = profile.maxMana;  

  skill.literacy = mWorld.getRandomNumber(seed, 0, 2000);
  skill.cooking = mWorld.getRandomNumber(seed, 0, 2000);
  skill.cleaning = mWorld.getRandomNumber(seed, 0, 2000);
  skill.service = mWorld.getRandomNumber(seed, 0, 2000);
  skill.music = mWorld.getRandomNumber(seed, 0, 2000);
  skill.art = mWorld.getRandomNumber(seed, 0, 2000);
  skill.tailor = mWorld.getRandomNumber(seed, 0, 2000);
  skill.stoneWorking = mWorld.getRandomNumber(seed, 0, 2000);
  skill.woodWorking = mWorld.getRandomNumber(seed, 0, 2000);
  skill.metalworking = mWorld.getRandomNumber(seed, 0, 2000);
  skill.farming = mWorld.getRandomNumber(seed, 0, 2000);
  skill.fishing = mWorld.getRandomNumber(seed, 0, 2000);
  skill.crafting = mWorld.getRandomNumber(seed, 0, 2000);
  skill.sword = mWorld.getRandomNumber(seed, 0, 2000);
  skill.axe = mWorld.getRandomNumber(seed, 0, 2000);
  skill.bludgeon = mWorld.getRandomNumber(seed, 0, 2000);
  skill.stave = mWorld.getRandomNumber(seed, 0, 2000);
  skill.polearm = mWorld.getRandomNumber(seed, 0, 2000);
  skill.evasion = mWorld.getRandomNumber(seed, 0, 2000);
  skill.fire = mWorld.getRandomNumber(seed, 0, 2000);
  skill.water = mWorld.getRandomNumber(seed, 0, 2000);
  skill.earth = mWorld.getRandomNumber(seed, 0, 2000);
  skill.air = mWorld.getRandomNumber(seed, 0, 2000);
  skill.lightning = mWorld.getRandomNumber(seed, 0, 2000);
  skill.holy = mWorld.getRandomNumber(seed, 0, 2000);
  skill.dark = mWorld.getRandomNumber(seed, 0, 2000);
  skill.machine = mWorld.getRandomNumber(seed, 0, 2000);
  skill.poison = mWorld.getRandomNumber(seed, 0, 2000);
  skill.choas = mWorld.getRandomNumber(seed, 0, 2000);

  residence.cleaniness = 10000;
  residence.houseLevel = 1;
  residence.kitchenLevel = 1;
  residence.libraryLevel = 1;
  residence.bedroomLevel = 1;

  dailySchedule[0] = job.getActivity("Sleep");
  dailySchedule[1] = job.getActivity("Sleep");
  dailySchedule[2] = job.getActivity("Sleep");
  dailySchedule[3] = job.getActivity("Sleep");
  dailySchedule[4] = job.getActivity("Cooking");
  dailySchedule[5] = job.getActivity("Cleaning");
  dailySchedule[6] = job.getActivity("Nap");
  dailySchedule[7] = job.getActivity("Playing");
  dailySchedule[8] = job.getActivity("Nap");
  dailySchedule[9] = job.getActivity("Playing");
  dailySchedule[10] = job.getActivity("Read Book");
  dailySchedule[11] = job.getActivity("Cooking");
  dailySchedule[12] = job.getActivity("Cleaning");
  dailySchedule[13] = job.getActivity("Nap");
  dailySchedule[14] = job.getActivity("Read Book");
  dailySchedule[15] = job.getActivity("Bath");
  dailySchedule[16] = job.getActivity("Cooking");
  dailySchedule[17] = job.getActivity("Cleaning");
  dailySchedule[18] = job.getActivity("Playing");
  dailySchedule[19] = job.getActivity("Bath");
  dailySchedule[20] = job.getActivity("Read Book");
  dailySchedule[21] = job.getActivity("Sleep");
  dailySchedule[22] = job.getActivity("Sleep");
  dailySchedule[23] = job.getActivity("Sleep");
  dailySchedule[24] = job.getActivity("Sleep");

  uint16_t timeOfDay = (mWorld.currentWorldHeight + mWorld.localTimeOffset) % 5760;
  uint16_t currentHour = timeOfDay / (24 * 4);
  currentActivity = dailySchedule[currentHour];
  favouriteActivityType = Job::ActivityType(mWorld.getRandomNumber(seed, 1, 4));
}

void Character::generateStartingItems(const std::string& seed)
{
  Weapon::WeaponItem startingWeapon = weapon.randomizeWeapon(seed, 500000);
  weaponInventory.push_back(startingWeapon);
  equipedWeapon = weaponInventory.at(0);
  favouriteWeaponType = weaponInventory.at(0).type;

  Dress::DressItem startingDress = dress.generateDress("Basic Dress");
  dressInventory.push_back(startingDress);
  equipedDress = dressInventory.at(0);

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
    library.push_back(startingBook);
  }

  for (int i = 0; i < 3; i++)
  {
    Toy::ToyItem startingToy = toy.randomizeToy(seed);
    playRoom.push_back(startingToy);
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
