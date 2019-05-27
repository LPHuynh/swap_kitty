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
  updateFluffText();

  shop.refreshInventory(seed);
}

void Character::updateSecondaryStats()
{
  profile.stat.acc = profile.stat.per + profile.stat.dex * 2;
  profile.stat.cri = profile.stat.per + profile.stat.lrn;
  profile.maxHealth = (profile.stat.con * 2 + profile.stat.wil) * profile.level + 5000;
  profile.health = profile.maxHealth;
  profile.maxMana = (profile.stat.mag * 2 + profile.stat.wil) * profile.level + 500;
  profile.mana = profile.maxMana;
}

void Character::consumeFood(uint16_t id, bool isLogConsumedItem)
{
  int i = 0;
  bool isItemFound = false;
  for (auto& element : foodInventory)
  {
    if (element.id == id)
    {
      isItemFound = true;
      break;
    }
    i++;
  }

  if (isItemFound)
  {
    if (isLogConsumedItem)
    {
      mWorld.logging.addToMainLog("\t" + profile.name + " ate some " + foodInventory.at(id).name);
    }

    if (foodInventory.at(i).type == Food::FoodType::vermin)
    {
      profile.domesticated -= 20;
      if (profile.domesticated > 7500)
      {
        profile.domesticated -= 100;
        profile.happiness -= 100;
        mWorld.logging.addToMainLog("\t\t" + profile.name + " was forced to eat a small vermin! Yuck!");
      }
    }
    else if (foodInventory.at(i).type == Food::FoodType::fruit)
    {
      if (foodInventory.at(i).nameRaw == favouriteFruitDish.nameRaw && foodInventory.at(i).dishLevel == favouriteFruitDish.dishLevel)
      {
        profile.domesticated += 100;
        profile.happiness += 200;
        mWorld.logging.addToMainLog("\t\t" + foodInventory.at(i).name + "! Her favourite!");
      }
      else if (foodInventory.at(i).nameRaw == favouriteFruitDish.nameRaw || foodInventory.at(i).dishLevel == favouriteFruitDish.dishLevel)
      {
        profile.domesticated += 20;
        profile.happiness += 100;
        mWorld.logging.addToMainLog("\t\tThe " + foodInventory.at(i).name + " was delicious!");
      }
    }
    else if (foodInventory.at(i).type == Food::FoodType::vegatable)
    {
      if (foodInventory.at(i).nameRaw == favouriteVegatable.nameRaw)
      {
        profile.domesticated += 20;
        profile.happiness += 100;
        mWorld.logging.addToMainLog("\t\tThe " + foodInventory.at(i).name + " was delicious!");
      }
    }
    else if (foodInventory.at(i).type == Food::FoodType::flour)
    {
      profile.domesticated += 50;
    }
    else if (foodInventory.at(i).type == Food::FoodType::junkFood)
    {
      if (foodInventory.at(i).name == favouriteJunkFood.name)
      {
        profile.domesticated += 500;
        profile.happiness += 500;
        mWorld.logging.addToMainLog("\t\t" + foodInventory.at(i).name + "! Her favourite!");
      }
    }

    profile.stat = profile.stat + foodInventory.at(i).bonusStat;
    profile.stamina += foodInventory.at(i).stamina;
    profile.satiation += foodInventory.at(i).nutrient;
    profile.quench += foodInventory.at(i).quench;

    std::string text = "\t\t";
    if (foodInventory.at(i).stamina / 100 > 0)
    {
      text += "+" + std::to_string(foodInventory.at(i).stamina / 100) + "Stamina ";
    }
    if (foodInventory.at(i).nutrient > 0)
    {
      text += "+" + std::to_string(foodInventory.at(i).nutrient / 100) + "Nutrient ";
    }
    if (foodInventory.at(i).quench > 0)
    {
      text += "+" + std::to_string(foodInventory.at(i).quench / 100) + "Quench ";
    }
    mWorld.logging.addToMainLog(text);

    mWorld.freeID(id);
    foodInventory.erase(foodInventory.begin() + i);
  }
}

void Character::consumePotion(uint16_t id, bool isLogConsumedItem)
{
  int i = 0;
  bool isItemFound = false;
  for (auto& element : potionInventory)
  {
    if (element.id == id)
    {
      isItemFound = true;
      break;
    }
    i++;
  }

  if (isItemFound)
  {
    if (isLogConsumedItem)
    {
      mWorld.logging.addToMainLog("\t" + profile.name + " drank some " + potionInventory.at(id).name);
    }

    if (potionInventory.at(i).name == "Milk")
    {
      profile.cosmetic.milkDrinkingCounter += 10;
      mWorld.logging.addToMainLog("\t\tThis is good for " + profile.name + "'s growth!");
    }
    else if (potionInventory.at(i).name == "Pond Water")
    {
      profile.domesticated -= 10;
      if (profile.domesticated > 7500)
      {
        profile.domesticated -= 100;
        profile.happiness -= 100;
        mWorld.logging.addToMainLog("\t\t" + profile.name + " was forced to drick out of a nearby pond! Yuck!");
      }
    }

    profile.tempStat = profile.tempStat + potionInventory.at(i).bonusTempStat;
    profile.stamina += potionInventory.at(i).stamina;
    profile.satiation += potionInventory.at(i).nutrient;
    profile.quench += potionInventory.at(i).quench;

    std::string text = "\t\t";
    if (potionInventory.at(i).stamina / 100 > 0)
    {
      text += "+" + std::to_string(potionInventory.at(i).stamina / 100) + "Stamina ";
    }
    if (potionInventory.at(i).nutrient / 100 > 0)
    {
      text += "+" + std::to_string(potionInventory.at(i).nutrient / 100) + "Nutrient ";
    }
    if (potionInventory.at(i).quench / 100 > 0)
    {
      text += "+" + std::to_string(potionInventory.at(i).quench / 100) + "Quench ";
    }
    mWorld.logging.addToMainLog(text);

    mWorld.freeID(id);
    potionInventory.erase(potionInventory.begin() + i);
  }
}

void Character::updateFluffText()
{
  std::string elementText;
  std::string primaryElementText;
  std::string secondaryElementText;
  std::string domesticationText;

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
    elementText = "She is shrouded in an aura of " + primaryElementText + " and " + secondaryElementText + ".";
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
  case Weapon::WeaponType::sword: weaponText = "never leaves the house without a good pocket knife."; break;
  case Weapon::WeaponType::axe: weaponText = "can easily find ways around locked doors and chests."; break;
  case Weapon::WeaponType::bludgeon: weaponText = "loves fixings things using percussive repair."; break;
  case Weapon::WeaponType::stave: weaponText = "loves collecting twigs and tree branch in her spare time."; break;
  case Weapon::WeaponType::polearm: weaponText = "loves poking at things with a long pointy stick."; break;
  default: weaponText = ""; break;
  }

  if (profile.domesticated < 1000)
  {
    domesticationText = "She is a salvage hunter.";

  }
  else if (profile.domesticated < 2500)
  {
    domesticationText = "Her hunting instincts are strong.";
  }
  else if (profile.domesticated < 5000)
  {
    domesticationText = "She still retained some of her hunting instincts.";
  }
  else if (profile.domesticated < 7500)
  {
    domesticationText = "She resists the urge to hunt small animals.";
  }
  else if (profile.domesticated < 9000)
  {
    domesticationText = "She have forgotting her hunting instincts.";
  }
  else
  {
    domesticationText = "She is fully domesticated.";
  }

  fluffText = profile.name + " is a " + std::to_string(profile.cosmetic.age) + " year old " + profile.cosmetic.gender + " " + profile.cosmetic.species + " "
    + "with " + profile.cosmetic.currentHairStyle + " " + profile.cosmetic.currentHairColour + " hair and a " + profile.cosmetic.getSkinToneDescription(profile.cosmetic.currentSkinTone) + " complexion.\n"
    + "She weighs " + std::to_string(profile.cosmetic.weight / 1000) + "kg and stands " + std::to_string(profile.cosmetic.height / 10) + "cm tall. "
    + "She have a " + profile.cosmetic.naturalTailStyle + " tail. "
    + "Her eyes are " + profile.cosmetic.naturalEyeStyle + " and " + profile.cosmetic.currentEyeColour + "\n\n"
    + "Her favourite food are " + favouriteVegatable.nameRaw + ", " + favouriteFruitDish.name + ", and " + favouriteJunkFood.nameRaw + ".\n"
    + activityText + weaponText + "\n\n"
    + elementText + " "
    + domesticationText + "\n";
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

  profile.stat.str = mWorld.getRandomNumber(seed, 0, 2000);
  profile.stat.con = mWorld.getRandomNumber(seed, 0, 2000);
  profile.stat.dex = mWorld.getRandomNumber(seed, 0, 2000);
  profile.stat.per = mWorld.getRandomNumber(seed, 0, 2000);
  profile.stat.lrn = mWorld.getRandomNumber(seed, 0, 2000);
  profile.stat.wil = mWorld.getRandomNumber(seed, 0, 2000);
  profile.stat.mag = mWorld.getRandomNumber(seed, 0, 2000);
  profile.stat.chr = mWorld.getRandomNumber(seed, 0, 2000);
  profile.tempStat = { 0,0,0,0,0,0,0,0,0,0 };

  profile.skill.literacy = mWorld.getRandomNumber(seed, 0, 2000);
  profile.skill.cooking = mWorld.getRandomNumber(seed, 0, 2000);
  profile.skill.cleaning = mWorld.getRandomNumber(seed, 0, 2000);
  profile.skill.service = mWorld.getRandomNumber(seed, 0, 2000);
  profile.skill.music = mWorld.getRandomNumber(seed, 0, 2000);
  profile.skill.art = mWorld.getRandomNumber(seed, 0, 2000);
  profile.skill.tailor = mWorld.getRandomNumber(seed, 0, 2000);
  profile.skill.stoneWorking = mWorld.getRandomNumber(seed, 0, 2000);
  profile.skill.woodWorking = mWorld.getRandomNumber(seed, 0, 2000);
  profile.skill.metalworking = mWorld.getRandomNumber(seed, 0, 2000);
  profile.skill.farming = mWorld.getRandomNumber(seed, 0, 2000);
  profile.skill.fishing = mWorld.getRandomNumber(seed, 0, 2000);
  profile.skill.crafting = mWorld.getRandomNumber(seed, 0, 2000);
  profile.skill.sword = mWorld.getRandomNumber(seed, 0, 2000);
  profile.skill.axe = mWorld.getRandomNumber(seed, 0, 2000);
  profile.skill.bludgeon = mWorld.getRandomNumber(seed, 0, 2000);
  profile.skill.stave = mWorld.getRandomNumber(seed, 0, 2000);
  profile.skill.polearm = mWorld.getRandomNumber(seed, 0, 2000);
  profile.skill.evasion = mWorld.getRandomNumber(seed, 0, 2000);
  profile.skill.fire = mWorld.getRandomNumber(seed, 0, 2000);
  profile.skill.water = mWorld.getRandomNumber(seed, 0, 2000);
  profile.skill.earth = mWorld.getRandomNumber(seed, 0, 2000);
  profile.skill.air = mWorld.getRandomNumber(seed, 0, 2000);
  profile.skill.lightning = mWorld.getRandomNumber(seed, 0, 2000);
  profile.skill.holy = mWorld.getRandomNumber(seed, 0, 2000);
  profile.skill.dark = mWorld.getRandomNumber(seed, 0, 2000);
  profile.skill.machine = mWorld.getRandomNumber(seed, 0, 2000);
  profile.skill.poison = mWorld.getRandomNumber(seed, 0, 2000);
  profile.skill.choas = mWorld.getRandomNumber(seed, 0, 2000);

  updateSecondaryStats();

  residence.cleaniness = 10000;
  residence.houseLevel = 1;
  residence.kitchenLevel = 1;
  residence.libraryLevel = 1;
  residence.bedroomLevel = 1;

  dailySchedule[0] = job.getActivity("Sleep");
  dailySchedule[1] = job.getActivity("Sleep");
  dailySchedule[2] = job.getActivity("Sleep");
  dailySchedule[3] = job.getActivity("Cooking");
  dailySchedule[4] = job.getActivity("Cleaning");
  dailySchedule[5] = job.getActivity("Read Book");
  dailySchedule[6] = job.getActivity("Waitress");
  dailySchedule[7] = job.getActivity("Waitress");
  dailySchedule[8] = job.getActivity("Nap");
  dailySchedule[9] = job.getActivity("Playing");
  dailySchedule[10] = job.getActivity("Read Book");
  dailySchedule[11] = job.getActivity("Fishing");
  dailySchedule[12] = job.getActivity("Fishing");
  dailySchedule[13] = job.getActivity("Nap");
  dailySchedule[14] = job.getActivity("Plant Gatherer");
  dailySchedule[15] = job.getActivity("Plant Gatherer");
  dailySchedule[16] = job.getActivity("Cooking");
  dailySchedule[17] = job.getActivity("Cleaning");
  dailySchedule[18] = job.getActivity("Playing");
  dailySchedule[19] = job.getActivity("Bath");
  dailySchedule[20] = job.getActivity("Read Book");
  dailySchedule[21] = job.getActivity("Sleep");
  dailySchedule[22] = job.getActivity("Sleep");
  dailySchedule[23] = job.getActivity("Sleep");

  uint16_t timeOfDay = (mWorld.currentWorldHeight + mWorld.localTimeOffset) % 5760;
  uint16_t currentHour = timeOfDay / 240;
  currentActivity = dailySchedule[currentHour];
  favouriteActivityType = Job::ActivityType(mWorld.getRandomNumber(seed, 1, 4));
}

void Character::generateStartingItems(const std::string& seed)
{
  Weapon::WeaponItem startingWeapon = weapon.randomizeWeapon(seed, 500000);
  weaponInventory.push_back(startingWeapon);
  equipedWeapon = weaponInventory.at(0);
  favouriteWeaponType = weaponInventory.at(0).type;

  Dress::DressItem startingDress = dress.randomizeDress(seed);
  dressInventory.push_back(startingDress);
  equipedDress = dressInventory.at(0);

  Food::FoodItem fruitDish = food.randomizeRawFood(seed, Food::FoodType::fruit);
  food.randomizeCookedFood(seed, fruitDish);
  favouriteFruitDish = fruitDish;
  foodInventory.push_back(fruitDish);

  for (int i = 0; i < 5; i++)
  {
    Food::FoodItem vegatable = food.randomizeRawFood(seed, Food::FoodType::vegatable);
    favouriteVegatable = vegatable;
    foodInventory.push_back( vegatable);
  }
  for (int i = 0; i < 5; i++)
  {
    Food::FoodItem flour = food.randomizeRawFood(seed, Food::FoodType::flour);
    foodInventory.push_back(flour);
  }

  Food::FoodItem junkfood = food.randomizeRawFood(seed, Food::FoodType::junkFood);
  favouriteJunkFood = junkfood;
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
    toyRoom.push_back(startingToy);
  }
}