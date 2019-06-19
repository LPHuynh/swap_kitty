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
  updateStatBarText();
  updateScheduleBoxText(0);
  updateHouseBoxText();
  isCharacterSheetFluff = false;
  updateCharacterSheetText();

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
      mWorld.logging.addToMainLog("\t" + profile.name + " ate " + mWorld.makeSingularNoun(foodInventory.at(i).name));
      mWorld.logging.addStatusMessage(profile.name + " is eating " + mWorld.makeSingularNoun(foodInventory.at(i).name), "eat");
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
    if (foodInventory.at(i).nutrient / 100 > 0)
    {
      text += "+" + std::to_string(foodInventory.at(i).nutrient / 100) + "Nutrient ";
    }
    if (foodInventory.at(i).quench / 100 > 0)
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
      mWorld.logging.addToMainLog("\t" + profile.name + " drank  " + potionInventory.at(i).name);
      mWorld.logging.addStatusMessage(profile.name + " is drinking " + potionInventory.at(i).name, "drink");
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

  if (profile.primaryElement == profile.secondaryElement)
  {
    switch (profile.primaryElement)
    {
    case World::Element::fire: secondaryElementText = "Brimstone"; break;
    case World::Element::water: primaryElementText = "Ice"; break;
    case World::Element::earth: secondaryElementText = "Nature"; break;
    case World::Element::air: primaryElementText = "Wind"; break;
    case World::Element::lightning: primaryElementText = "Thunder"; break;
    case World::Element::holy: primaryElementText = "Light"; break;
    case World::Element::dark: secondaryElementText = "Decay"; break;
    default: break;
    }
  }
  else if (profile.primaryElement == World::Element::holy && profile.secondaryElement == World::Element::dark)
  {
    primaryElementText = "Light";
  }
  else if (profile.primaryElement == World::Element::dark && profile.secondaryElement == World::Element::holy)
  {
    primaryElementText = "Defiance";
  }
  else;

  elementText = "She is shrouded in an aura of " + primaryElementText + " and " + secondaryElementText + ".";

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
    domesticationText = "Her hunting instincts are very strong.";
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

  fluffText = profile.name + " is a " + std::to_string(profile.cosmetic.age) + " year old " + profile.cosmetic.getGender() + " " + profile.cosmetic.getSpecies() + " "
    + "with " + profile.cosmetic.currentHairStyle + " " + profile.cosmetic.currentHairColour + " hair. "
    + "She weighs " + std::to_string(profile.cosmetic.weight / 1000) + "kg and stands " + std::to_string(profile.cosmetic.height / 10) + "cm tall. "
    + "She have a " + profile.cosmetic.naturalTailStyle + " tail. "
    + "Her eyes are " + profile.cosmetic.naturalEyeStyle + " and " + profile.cosmetic.currentEyeColour + ".\n\n"
    + "Her favourite food are " + favouriteVegatable.nameRaw + ", " + favouriteFruitDish.name + ", and " + favouriteJunkFood.nameRaw + ". "
    + activityText + weaponText + "\n\n"
    + domesticationText + "\n\n"
    + elementText + "\n";
}

void Character::updateStatBarText()
{
  statBarText = "STR:" + std::to_string((profile.stat.str + profile.tempStat.str + equipedWeapon.bonusStat.str + equipedDress.bonusStat.str) / 100) 
    + " CON:" + std::to_string((profile.stat.con + profile.tempStat.con + equipedWeapon.bonusStat.con + equipedDress.bonusStat.con) / 100)
    + " DEX:" + std::to_string((profile.stat.dex + profile.tempStat.dex + equipedWeapon.bonusStat.dex + equipedDress.bonusStat.dex) / 100)
    + " PER:" + std::to_string((profile.stat.per + profile.tempStat.per + equipedWeapon.bonusStat.per + equipedDress.bonusStat.per) / 100)
    + " LRN:" + std::to_string((profile.stat.lrn + profile.tempStat.lrn + equipedWeapon.bonusStat.lrn + equipedDress.bonusStat.lrn) / 100)
    + " WIL:" + std::to_string((profile.stat.wil + profile.tempStat.wil + equipedWeapon.bonusStat.wil + equipedDress.bonusStat.wil) / 100)
    + " MAG:" + std::to_string((profile.stat.mag + profile.tempStat.mag + equipedWeapon.bonusStat.mag + equipedDress.bonusStat.mag) / 100)
    + " CHR:" + std::to_string((profile.stat.chr + profile.tempStat.chr + equipedWeapon.bonusStat.chr + equipedDress.bonusStat.chr) / 100)
    + " ACC:" + std::to_string((profile.stat.acc + profile.tempStat.acc + equipedWeapon.bonusStat.acc + equipedDress.bonusStat.acc) / 100)
    + " CRI:" + std::to_string((profile.stat.cri + profile.tempStat.cri + equipedWeapon.bonusStat.cri + equipedDress.bonusStat.cri) / 100);    
}

void Character::updateStatusBarText()
{
  statusBarText = "  ";

  if (profile.toxicity > 1000)
  {
    statusBarText += "[Intoxicated]";
  }

  if (profile.stamina < 0)
  {
    statusBarText += "[Exhausted]";
  }
  else if (profile.stamina < 1000)
  {
    statusBarText += "[Exerted]";
  }
  else if (profile.stamina < 2500)
  {
    statusBarText += "[Tired]";
  }

  if (profile.satiation < 1000)
  {
    statusBarText += "[Starving]";
  }
  else if (profile.satiation < 2500)
  {
    statusBarText += "[Hungry]";
  }
  else if (profile.satiation > 7500)
  {
    statusBarText += "[Satiated]";
  }
  else if (profile.satiation > 10000)
  {
    statusBarText += "[Stuffed]";
  }
  else;

  if (profile.quench < 1000)
  {
    statusBarText += "[Dehydrated]";
  }
  else if (profile.quench < 2500)
  {
    statusBarText += "[Thristy]";
  }
  else if (profile.quench > 10000)
  {
    statusBarText += "[Bloated]";
  }
  else;

  if (profile.cleanliness < 1000)
  {
    statusBarText += "[Filthy]";
  }
  else if (profile.cleanliness < 2500)
  {
    statusBarText += "[Dirty]";
  }
  else;

  if (profile.happiness < 1000)
  {
    statusBarText += "[Miserable]";
  }
  else if (profile.happiness < 2500)
  {
    statusBarText += "[Unhappy]";
  }
  else;
}

void Character::updateScheduleBoxText(uint8_t timeHour)
{
  scheduleBoxText = "[Schedule]\n";

  //Display 5 activity before current activity
  if (timeHour < 5)
  {
    timeHour += 24;
  }
  timeHour -= 5;
  for (int i = 0; i < 20; i++)
  {
    std::string timeSuffix = "AM";
    uint8_t displayedHour = timeHour;
    std::string displayedTime = "";

    if (timeHour > 11)
    {
      displayedHour = timeHour - 12;
      timeSuffix = "PM";
    }
    if (displayedHour == 0)
    {
      displayedHour = 12;
    }

    if (displayedHour < 10)
    {
      displayedTime += " ";
    }

    displayedTime += std::to_string(displayedHour) + ":00" + timeSuffix;

    if (i == 5)
    {
      if (currentActivity.name == dailySchedule[timeHour].name)
      {
        scheduleBoxText += " <<" + displayedTime + "\t" + currentActivity.name + ">>\n";
      }
      else
      {
        scheduleBoxText += " <<" + displayedTime + "\t" + currentActivity.name + "*>>\n";
      }
    }
    else
    {
      scheduleBoxText += "   " + displayedTime + "\t" + dailySchedule[timeHour].name + "\n";
    }

    timeHour++;
    if (timeHour == 24)
    {
      timeHour = 0;
    }
  }
}

void Character::updateHouseBoxText()
{
  std::string cleanslinessText;

  if (residence.cleanliness < 1000)
  {
    cleanslinessText = "Filthy";
  }
  else if (residence.cleanliness < 2500)
  {
    cleanslinessText = "Dirty";
  }
  else if (residence.cleanliness < 5000)
  {
    cleanslinessText = "Messy";
  }
  else if (residence.cleanliness < 7500)
  {
    cleanslinessText = "Tidy";
  }
  else if (residence.cleanliness < 10000)
  {
    cleanslinessText = "Clean";
  }
  else
  {
    cleanslinessText = "Spotless";
  }

  houseBoxText = 
    "Money: " + std::to_string(profile.money / 100) + "G\n" +
    "House: " + cleanslinessText;
}

void Character::updateCharacterSheetText()
{
  std::stringstream ss;

  ss << "Equipment:\n";
  ss << "  " + mWorld.makeSingularNoun(equipedWeapon.name) + "\n";
  ss << "  " + mWorld.makeSingularNoun(equipedDress.name) + "\n\n";

  characterSheetText = "";
  if (isCharacterSheetFluff)
  {
    updateFluffText();
    characterSheetText = ss.str() + fluffText;
  }
  else
  {
    ss << std::left << std::setw(15) << "Level: " + std::to_string(profile.level);
    ss << "Exp: " + std::to_string(profile.exp) + "\n";
    ss << std::left << std::setw(15) << "Health: " + std::to_string(profile.health / 100) + "/" + std::to_string(profile.maxHealth / 100);
    ss << "Mana: " << std::to_string(profile.mana / 100) + "/" + std::to_string(profile.maxMana / 100) + "\n\n";

    ss << std::left << std::setw(15) << "sword:     " + std::to_string((profile.skill.sword + equipedWeapon.bonusSkill.sword + equipedDress.bonusSkill.sword) / 100);
    ss << "literacy:  " + std::to_string((profile.skill.literacy + equipedWeapon.bonusSkill.literacy + equipedDress.bonusSkill.literacy) / 100) + "\n";
    ss << std::left << std::setw(15) << "axe:       " + std::to_string((profile.skill.axe + equipedWeapon.bonusSkill.axe + equipedDress.bonusSkill.axe) / 100);
    ss << "cooking:   " + std::to_string((profile.skill.cooking + equipedWeapon.bonusSkill.cooking + equipedDress.bonusSkill.cooking) / 100) + "\n";
    ss << std::left << std::setw(15) << "bludgeon:  " + std::to_string((profile.skill.bludgeon + equipedWeapon.bonusSkill.bludgeon + equipedDress.bonusSkill.bludgeon) / 100);
    ss << "cleaning:  " + std::to_string((profile.skill.cleaning + equipedWeapon.bonusSkill.cleaning + equipedDress.bonusSkill.cleaning) / 100) + "\n";
    ss << std::left << std::setw(15) << "stave:     " + std::to_string((profile.skill.stave + equipedWeapon.bonusSkill.stave + equipedDress.bonusSkill.stave) / 100);
    ss << "service:   " + std::to_string((profile.skill.service + equipedWeapon.bonusSkill.service + equipedDress.bonusSkill.service) / 100) + "\n";
    ss << std::left << std::setw(15) << "polearm:   " + std::to_string((profile.skill.polearm + equipedWeapon.bonusSkill.polearm + equipedDress.bonusSkill.polearm) / 100);
    ss << "music:     " + std::to_string((profile.skill.music + equipedWeapon.bonusSkill.music + equipedDress.bonusSkill.music) / 100) + "\n";
    ss << std::left << std::setw(15) << "evasion:   " + std::to_string((profile.skill.evasion + equipedWeapon.bonusSkill.evasion + equipedDress.bonusSkill.evasion) / 100);
    ss << "art:       " + std::to_string((profile.skill.art + equipedWeapon.bonusSkill.art + equipedDress.bonusSkill.art) / 100) + "\n";   
    ss << std::left << std::setw(15) << "fire:      " + std::to_string((profile.skill.fire + equipedWeapon.bonusSkill.fire + equipedDress.bonusSkill.fire) / 100);
    ss << "tailor:    " + std::to_string((profile.skill.tailor + equipedWeapon.bonusSkill.tailor + equipedDress.bonusSkill.tailor) / 100) + "\n";
    ss << std::left << std::setw(15) << "water:     " + std::to_string((profile.skill.water + equipedWeapon.bonusSkill.water + equipedDress.bonusSkill.water) / 100);
    ss << "stonework: " + std::to_string((profile.skill.stoneWorking + equipedWeapon.bonusSkill.stoneWorking + equipedDress.bonusSkill.stoneWorking) / 100) + "\n";
    ss << std::left << std::setw(15) << "earth:     " + std::to_string((profile.skill.earth + equipedWeapon.bonusSkill.earth + equipedDress.bonusSkill.earth) / 100);
    ss << "woodwork:  " + std::to_string((profile.skill.woodWorking + equipedWeapon.bonusSkill.woodWorking + equipedDress.bonusSkill.woodWorking) / 100) + "\n";
    ss << std::left << std::setw(15) << "air:       " + std::to_string((profile.skill.air + equipedWeapon.bonusSkill.air + equipedDress.bonusSkill.air) / 100);
    ss << "metalwork: " + std::to_string((profile.skill.metalworking + equipedWeapon.bonusSkill.metalworking + equipedDress.bonusSkill.metalworking) / 100) + "\n";
    ss << std::left << std::setw(15) << "lightning: " + std::to_string((profile.skill.lightning + equipedWeapon.bonusSkill.lightning + equipedDress.bonusSkill.lightning) / 100);
    ss << "farming:   " + std::to_string((profile.skill.farming + equipedWeapon.bonusSkill.farming + equipedDress.bonusSkill.farming) / 100) + "\n";
    ss << std::left << std::setw(15) << "holy:      " + std::to_string((profile.skill.holy + equipedWeapon.bonusSkill.holy + equipedDress.bonusSkill.holy) / 100);
    ss << "fishing:   " + std::to_string((profile.skill.fishing + equipedWeapon.bonusSkill.fishing + equipedDress.bonusSkill.fishing) / 100) + "\n";
    ss << std::left << std::setw(15) << "dark:      " + std::to_string((profile.skill.dark + equipedWeapon.bonusSkill.dark + equipedDress.bonusSkill.dark) / 100);
    ss << "crafting:  " + std::to_string((profile.skill.crafting + equipedWeapon.bonusSkill.crafting + equipedDress.bonusSkill.crafting) / 100) + "\n";
    ss << "machine:   " + std::to_string((profile.skill.machine + equipedWeapon.bonusSkill.machine + equipedDress.bonusSkill.machine) / 100) + "\n";
    ss << "poison:    " + std::to_string((profile.skill.poison + equipedWeapon.bonusSkill.poison + equipedDress.bonusSkill.poison) / 100) + "\n";
    ss << "chaos:     " + std::to_string((profile.skill.chaos + equipedWeapon.bonusSkill.chaos + equipedDress.bonusSkill.chaos) / 100);

    characterSheetText = ss.str();
  }
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
  profile.cleanliness = 10000;
  profile.happiness = mWorld.getRandomNumber(seed, 2500, 5000);
  profile.obedience = mWorld.getRandomNumber(seed, 0, 2500);

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
  profile.skill.chaos = mWorld.getRandomNumber(seed, 0, 2000);

  updateSecondaryStats();

  residence.cleanliness = 10000;
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
  dailySchedule[6] = job.getActivity("Waitress");
  dailySchedule[7] = job.getActivity("Waitress");
  dailySchedule[8] = job.getActivity("Playing");
  dailySchedule[9] = job.getActivity("Nap");
  dailySchedule[10] = job.getActivity("Read Book");
  dailySchedule[11] = job.getActivity("Fishing");
  dailySchedule[12] = job.getActivity("Fishing");
  dailySchedule[13] = job.getActivity("Nap");
  dailySchedule[14] = job.getActivity("Plant Gathering");
  dailySchedule[15] = job.getActivity("Plant Gathering");
  dailySchedule[16] = job.getActivity("Nap");
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
  foodInventory.push_back(junkfood);

  for (int i = 0; i < 10; i++)
  {
    Potion::PotionItem startingPotion = potion.randomizePotion(seed, Potion::Rarity::common);
    potionInventory.push_back(startingPotion);
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