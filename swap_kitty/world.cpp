#include "world.h"


World::World()
{
  currentRulesetVersion = 1;
  lastestRulesetVersion = 1;
  startingHeight = 0;
  currentWorldHeight = 0;
  isNighttime = false;
  localTimeOffset = 0;
  mIDCounter = 1; //0 is used to indicate non-existing items
}

World::~World()
{
}

World::Dice World::Dice::operator+(const Dice& rhs)
{
  Dice dice;

  dice.face = this->face + rhs.face;
  dice.roll = this->roll + rhs.roll;
  dice.plus = this->plus + rhs.plus;
  
  return dice;
}

World::Dice World::Dice::operator-(const Dice& rhs)
{
  Dice dice;

  dice.face = this->face - rhs.face;
  dice.roll = this->roll - rhs.roll;
  dice.plus = this->plus - rhs.plus;

  return dice;
}

World::Stat World::Stat::operator+(const Stat& rhs)
{
  Stat stat;

  stat.str = this->str + rhs.str;
  stat.con = this->con + rhs.con;
  stat.dex = this->dex + rhs.dex;
  stat.per = this->per + rhs.per;
  stat.lrn = this->lrn + rhs.lrn;
  stat.wil = this->wil + rhs.wil;
  stat.mag = this->mag + rhs.mag;
  stat.chr = this->chr + rhs.chr;
  stat.acc = this->acc + rhs.acc;
  stat.cri = this->cri + rhs.cri;

  return stat;
}

World::Skill World::Skill::operator+(const Skill& rhs)
{
  Skill skill;

  skill.literacy = this->literacy + rhs.literacy;
  skill.cooking = this->cooking + rhs.cooking;
  skill.cleaning = this->cleaning + rhs.cleaning;
  skill.service = this->service + rhs.service;
  skill.music = this->music + rhs.music;
  skill.art = this->art + rhs.art;
  skill.tailor = this->tailor + rhs.tailor;
  skill.stoneWorking = this->stoneWorking + rhs.stoneWorking;
  skill.woodWorking = this->woodWorking + rhs.woodWorking;
  skill.metalworking = this->metalworking + rhs.metalworking;
  skill.farming = this->farming + rhs.farming;
  skill.fishing = this->fishing + rhs.fishing;
  skill.crafting = this->crafting + rhs.crafting;
  skill.sword = this->sword + rhs.sword;
  skill.axe = this->axe + rhs.axe;
  skill.bludgeon = this->bludgeon + rhs.bludgeon;
  skill.stave = this->stave + rhs.stave;
  skill.polearm = this->polearm + rhs.polearm;
  skill.evasion = this->evasion + rhs.evasion;
  skill.fire = this->fire + rhs.fire;
  skill.water = this->water + rhs.water;
  skill.earth = this->earth + rhs.earth;
  skill.air = this->air + rhs.air;
  skill.lightning = this->lightning + rhs.lightning;
  skill.holy = this->holy + rhs.holy;
  skill.dark = this->dark + rhs.dark;
  skill.machine = this->machine + rhs.machine;
  skill.poison = this->poison + rhs.poison;
  skill.choas = this->choas + rhs.choas;

  return skill;
}

World::Stat World::Stat::operator-(const Stat& rhs)
{
  Stat stat;

  stat.str = this->str - rhs.str;
  stat.con = this->con - rhs.con;
  stat.dex = this->dex - rhs.dex;
  stat.per = this->per - rhs.per;
  stat.lrn = this->lrn - rhs.lrn;
  stat.wil = this->wil - rhs.wil;
  stat.mag = this->mag - rhs.mag;
  stat.chr = this->chr - rhs.chr;
  stat.acc = this->acc - rhs.acc;
  stat.cri = this->cri - rhs.cri;

  return stat;
}

World::Skill World::Skill::operator-(const Skill& rhs)
{
  Skill skill;

  skill.literacy = this->literacy - rhs.literacy;
  skill.cooking = this->cooking - rhs.cooking;
  skill.cleaning = this->cleaning - rhs.cleaning;
  skill.service = this->service - rhs.service;
  skill.music = this->music - rhs.music;
  skill.art = this->art - rhs.art;
  skill.tailor = this->tailor - rhs.tailor;
  skill.stoneWorking = this->stoneWorking - rhs.stoneWorking;
  skill.woodWorking = this->woodWorking - rhs.woodWorking;
  skill.metalworking = this->metalworking - rhs.metalworking;
  skill.farming = this->farming - rhs.farming;
  skill.fishing = this->fishing - rhs.fishing;
  skill.crafting = this->crafting - rhs.crafting;
  skill.sword = this->sword - rhs.sword;
  skill.axe = this->axe - rhs.axe;
  skill.bludgeon = this->bludgeon - rhs.bludgeon;
  skill.stave = this->stave - rhs.stave;
  skill.polearm = this->polearm - rhs.polearm;
  skill.evasion = this->evasion - rhs.evasion;
  skill.fire = this->fire - rhs.fire;
  skill.water = this->water - rhs.water;
  skill.earth = this->earth - rhs.earth;
  skill.air = this->air - rhs.air;
  skill.lightning = this->lightning - rhs.lightning;
  skill.holy = this->holy - rhs.holy;
  skill.dark = this->dark - rhs.dark;
  skill.machine = this->machine - rhs.machine;
  skill.poison = this->poison - rhs.poison;
  skill.choas = this->choas - rhs.choas;

  return skill;
}

uint16_t World::getStatByID(World::Stat stat, uint8_t id)
{
  switch (id)
  {
  case 0: return stat.str; 
  case 1: return stat.con; 
  case 3: return stat.dex; 
  case 4: return stat.per; 
  case 5: return stat.lrn; 
  case 6: return stat.wil; 
  case 7: return stat.mag; 
  case 8: return stat.chr; 
  case 9: return stat.acc; 
  case 10: return stat.cri; 
  }
  return 0;
}

uint16_t World::getSkillByID(World::Skill skill, uint8_t id)
{
  switch (id)
  {
  case 0: return skill.literacy; 
  case 1: return skill.cooking; 
  case 2: return skill.cleaning; 
  case 3: return skill.service; 
  case 4: return skill.music; 
  case 5: return skill.art; 
  case 6: return skill.tailor; 
  case 7: return skill.stoneWorking; 
  case 8: return skill.woodWorking; 
  case 9: return skill.metalworking; 
  case 10: return skill.farming; 
  case 11: return skill.fishing; 
  case 12: return skill.crafting; 
  case 13: return skill.sword; 
  case 14: return skill.axe; 
  case 15: return skill.bludgeon; 
  case 16: return skill.stave; 
  case 17: return skill.polearm; 
  case 18: return skill.evasion; 
  case 19: return skill.fire; 
  case 21: return skill.water; 
  case 22: return skill.earth; 
  case 23: return skill.air; 
  case 24: return skill.lightning; 
  case 25: return skill.holy; 
  case 26: return skill.dark; 
  case 27: return skill.machine; 
  case 28: return skill.poison; 
  case 29: return skill.choas; 
  default: return 0;
  }
}

std::string World::getStatNameByID(uint8_t id)
{
  switch (id)
  {
  case 0: return "STR";
  case 1: return "CON";
  case 3: return "DEX";
  case 4: return "PER";
  case 5: return "LRN";
  case 6: return "WIL";
  case 7: return "MAG";
  case 8: return "CHR";
  case 9: return "ACC";
  case 10: return "CRI";
  default: return "";
  }
}

std::string World::getSkillNameByID(uint8_t id)
{
  switch (id)
  {
  case 0: return "Literacy";
  case 1: return "Cooking";
  case 2: return "Cleaning";
  case 3: return "Service";
  case 4: return "Music";
  case 5: return "Art";
  case 6: return "Tailor";
  case 7: return "StoneWorking";
  case 8: return "WoodWorking";
  case 9: return "Metalworking";
  case 10: return "Farming";
  case 11: return "Fishing";
  case 12: return "Crafting";
  case 13: return "Sword";
  case 14: return "Axe";
  case 15: return "Bludgeon";
  case 16: return "Stave";
  case 17: return "Polearm";
  case 18: return "Evasion";
  case 19: return "Fire";
  case 21: return "Water";
  case 22: return "Earth";
  case 23: return "Air";
  case 24: return "Lightning";
  case 25: return "Holy";
  case 26: return "Dark";
  case 27: return "Machine";
  case 28: return "Poison";
  case 29: return "Choas";
  default: return 0;
  }
}

int16_t World::calculateElementAttackBonus(Element attackerElement, const std::vector<Element>& defenderElement)
{
  int16_t damageBonus = 100;

  for (auto& element : defenderElement) {
    if (attackerElement == Element::fire && (element == Element::air || element == Element::poison))
    {
      damageBonus += 25;
    }
    else if (attackerElement == Element::water && (element == Element::fire || element == Element::machine))
    {
      damageBonus += 25;
    }
    else if (attackerElement == Element::earth && (element == Element::water || element == Element::lightning))
    {
      damageBonus += 25;
    }
    else if (attackerElement == Element::air && (element == Element::earth || element == Element::chaos))
    {
      damageBonus += 25;
    }
    else if (attackerElement == Element::lightning && element == Element::water)
    {
      damageBonus += 25;
    }
    else if (attackerElement == Element::holy && element == Element::dark)
    {
      damageBonus += 25;
    }
    else if (attackerElement == Element::dark && element == Element::holy)
    {
      damageBonus += 25;
    }
    else if (attackerElement == Element::machine && element == Element::fire)
    {
      damageBonus += 25;
    }
    else if (attackerElement == Element::poison && element == Element::air)
    {
      damageBonus += 25;
    }
    else if (attackerElement == Element::poison && element == Element::poison)
    {
      damageBonus -= 15;
    }
    else if (attackerElement == Element::chaos && (element == Element::earth && element == Element::chaos))
    {
      damageBonus += 25;
    }
    if (attackerElement == element)
    {
      damageBonus -= 25;
    }
  }

  return damageBonus;
}

World::Stat World::shiftStat(const Stat& stat, int8_t bitwiseShift)
{
  Stat shiftedStat;

  if (bitwiseShift > 0)
  {
    shiftedStat.str = stat.str << bitwiseShift;
    shiftedStat.con = stat.con << bitwiseShift;
    shiftedStat.dex = stat.dex << bitwiseShift;
    shiftedStat.per = stat.per << bitwiseShift;
    shiftedStat.lrn = stat.lrn << bitwiseShift;
    shiftedStat.wil = stat.wil << bitwiseShift;
    shiftedStat.mag = stat.mag << bitwiseShift;
    shiftedStat.chr = stat.chr << bitwiseShift;
    shiftedStat.acc = stat.acc << bitwiseShift;
    shiftedStat.cri = stat.cri << bitwiseShift;
  }
  else
  {
    bitwiseShift *= -1;
    shiftedStat.str = stat.str >> bitwiseShift;
    shiftedStat.con = stat.con >> bitwiseShift;
    shiftedStat.dex = stat.dex >> bitwiseShift;
    shiftedStat.per = stat.per >> bitwiseShift;
    shiftedStat.lrn = stat.lrn >> bitwiseShift;
    shiftedStat.wil = stat.wil >> bitwiseShift;
    shiftedStat.mag = stat.mag >> bitwiseShift;
    shiftedStat.chr = stat.chr >> bitwiseShift;
    shiftedStat.acc = stat.acc >> bitwiseShift;
    shiftedStat.cri = stat.cri >> bitwiseShift;
  }

  return shiftedStat;
}

World::Skill World::shiftSkill(const Skill& skill, int8_t bitwiseShift)
{
  Skill shiftedSkill;

  if (bitwiseShift > 0)
  {
    shiftedSkill.literacy = skill.literacy << bitwiseShift;
    shiftedSkill.cooking = skill.cooking << bitwiseShift;
    shiftedSkill.cleaning = skill.cleaning << bitwiseShift;
    shiftedSkill.service = skill.service << bitwiseShift;
    shiftedSkill.music = skill.music << bitwiseShift;
    shiftedSkill.art = skill.art << bitwiseShift;
    shiftedSkill.tailor = skill.tailor << bitwiseShift;
    shiftedSkill.stoneWorking = skill.stoneWorking << bitwiseShift;
    shiftedSkill.woodWorking = skill.woodWorking << bitwiseShift;
    shiftedSkill.metalworking = skill.metalworking << bitwiseShift;
    shiftedSkill.farming = skill.farming << bitwiseShift;
    shiftedSkill.fishing = skill.fishing << bitwiseShift;
    shiftedSkill.crafting = skill.crafting << bitwiseShift;
    shiftedSkill.sword = skill.sword << bitwiseShift;
    shiftedSkill.axe = skill.axe << bitwiseShift;
    shiftedSkill.bludgeon = skill.bludgeon << bitwiseShift;
    shiftedSkill.stave = skill.stave << bitwiseShift;
    shiftedSkill.polearm = skill.polearm << bitwiseShift;
    shiftedSkill.evasion = skill.evasion << bitwiseShift;
    shiftedSkill.fire = skill.fire << bitwiseShift;
    shiftedSkill.water = skill.water << bitwiseShift;
    shiftedSkill.earth = skill.earth << bitwiseShift;
    shiftedSkill.air = skill.air << bitwiseShift;
    shiftedSkill.lightning = skill.lightning << bitwiseShift;
    shiftedSkill.holy = skill.holy << bitwiseShift;
    shiftedSkill.dark = skill.dark << bitwiseShift;
    shiftedSkill.machine = skill.machine << bitwiseShift;
    shiftedSkill.poison = skill.poison << bitwiseShift;
    shiftedSkill.choas = skill.choas << bitwiseShift;
  }
  else
  {
    bitwiseShift *= -1;
    shiftedSkill.literacy = skill.literacy >> bitwiseShift;
    shiftedSkill.cooking = skill.cooking >> bitwiseShift;
    shiftedSkill.cleaning = skill.cleaning >> bitwiseShift;
    shiftedSkill.service = skill.service >> bitwiseShift;
    shiftedSkill.music = skill.music >> bitwiseShift;
    shiftedSkill.art = skill.art >> bitwiseShift;
    shiftedSkill.tailor = skill.tailor >> bitwiseShift;
    shiftedSkill.stoneWorking = skill.stoneWorking >> bitwiseShift;
    shiftedSkill.woodWorking = skill.woodWorking >> bitwiseShift;
    shiftedSkill.metalworking = skill.metalworking >> bitwiseShift;
    shiftedSkill.farming = skill.farming >> bitwiseShift;
    shiftedSkill.fishing = skill.fishing >> bitwiseShift;
    shiftedSkill.crafting = skill.crafting >> bitwiseShift;
    shiftedSkill.sword = skill.sword >> bitwiseShift;
    shiftedSkill.axe = skill.axe >> bitwiseShift;
    shiftedSkill.bludgeon = skill.bludgeon >> bitwiseShift;
    shiftedSkill.stave = skill.stave >> bitwiseShift;
    shiftedSkill.polearm = skill.polearm >> bitwiseShift;
    shiftedSkill.evasion = skill.evasion >> bitwiseShift;
    shiftedSkill.fire = skill.fire >> bitwiseShift;
    shiftedSkill.water = skill.water >> bitwiseShift;
    shiftedSkill.earth = skill.earth >> bitwiseShift;
    shiftedSkill.air = skill.air >> bitwiseShift;
    shiftedSkill.lightning = skill.lightning >> bitwiseShift;
    shiftedSkill.holy = skill.holy >> bitwiseShift;
    shiftedSkill.dark = skill.dark >> bitwiseShift;
    shiftedSkill.machine = skill.machine >> bitwiseShift;
    shiftedSkill.poison = skill.poison >> bitwiseShift;
    shiftedSkill.choas = skill.choas >> bitwiseShift;
  }

  return shiftedSkill;
}

uint16_t World::rollDie(const std::string& seed, uint16_t numOfDie, uint16_t numOfFaces)
{
  int16_t result = 0;

  for (int i = 0; i < numOfDie; i++)
  {
    result += getRandomNumber(seed, 1, numOfFaces);
  }

  return result;
}

uint16_t World::getRandomNumber(std::string seed, uint16_t minNumber, uint16_t maxNumber)
{
  seed += generateNonce() + seed;
  return (generateHash(seed) % (maxNumber - minNumber + 1)) + minNumber;
}

uint16_t World::getNoncelessRandomNumber(std::string seed, uint16_t minNumber, uint16_t maxNumber)
{
  return (generateHash(seed) % (maxNumber - minNumber + 1)) + minNumber;
}

uint16_t World::generateID()
{
  uint16_t id;

  if (mRecoveredID.size() < 1000)
  {
    id = mIDCounter;
    mIDCounter++;    
  }
  else
  {
    id = mRecoveredID.front();
    mRecoveredID.pop();
  }

  return id;
}

void World::freeID(uint16_t id)
{
  mRecoveredID.push(id);
}

std::string World::generateNonce()
{
  mNonceCounter++;
  return std::to_string(mNonceCounter);
}

uint32_t World::generateHash(const std::string& seed)
{
  //djb2 Hash Functions 
  const char* str = seed.c_str();
  unsigned long hash = 5381;
  int c;

  while (c = *str++)
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash;
}
