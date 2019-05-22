#include "weapon.h"


Weapon::Weapon()
{
  loadWeapon();
  loadMaterial();
  loadQuality();
  loadAbility();
  loadStatBonus();
}

Weapon::~Weapon()
{
}

void Weapon::init(World& world)
{
  mWorld = world;
}

Weapon::WeaponItem Weapon::randomizeWeapon(const std::string& seed, int64_t maxCost)
{
  WeaponType type = WeaponType(0); 
  WeaponBase base = mSword.at(0);
  WeaponMaterial material = mMaterials.at(0);
  WeaponQuality quality = mQualities.at(0);
  WeaponAbility ability = mAbilities.at(0);
  WeaponStatBonus statBonus = mStatBonuses.at(0);

  type = WeaponType(mWorld.getRandomNumber(seed, 0, 4));

  switch (type)
  {
  case WeaponType::sword: base = mSword.at(mWorld.getRandomNumber(seed, 0, 4)); break;
  case WeaponType::axe: base = mAxe.at(mWorld.getRandomNumber(seed, 0, 3)); break;
  case WeaponType::bludgeon: base = mBludgeon.at(mWorld.getRandomNumber(seed, 0, 4)); break;
  case WeaponType::stave: base = mStave.at(mWorld.getRandomNumber(seed, 0, 6)); break;
  case WeaponType::polearm: base = mPolearm.at(mWorld.getRandomNumber(seed, 0, 5)); break;
  }

  material = mMaterials.at(mWorld.getRandomNumber(seed, 0, 11));

  int16_t maxQuality = mWorld.getRandomNumber(seed, 0, 5);
  quality = mQualities.at(mWorld.getRandomNumber(seed, 0, maxQuality));

  if (mWorld.rollDie(seed, 1, 6) == 6)
  {
    ability = mAbilities.at(mWorld.getRandomNumber(seed, 1, 10));
  }
  if (mWorld.rollDie(seed, 1, 10) == 6)
  {
    statBonus = mStatBonuses.at(mWorld.getRandomNumber(seed, 1, 7));
  }

  int64_t weaponCost = base.baseCost * material.costMultplier * quality.costMultplier * ability.costMultplier * statBonus.costMultplier;

  if (weaponCost > maxCost)
  {
    ability = mAbilities.at(0);
    weaponCost = base.baseCost * material.costMultplier * quality.costMultplier * ability.costMultplier * statBonus.costMultplier;
  }
  if (weaponCost > maxCost)
  {
    statBonus = mStatBonuses.at(0);
    weaponCost = base.baseCost * material.costMultplier * quality.costMultplier * ability.costMultplier * statBonus.costMultplier;
  }
  if (weaponCost > maxCost && quality.costMultplier > 25)
  {
    quality = mQualities.at(2);
  }
  if (weaponCost > maxCost && material.costMultplier > 20)
  {
    material = mMaterials.at(5);
  }

  WeaponItem weaponItem;

  weaponItem.id = mWorld.generateID();
  weaponItem.name = quality.name + " " + statBonus.name + " " + material.name + " " + base.name + " " + ability.name;
  weaponItem.type = type;
  weaponItem.price = base.baseCost * quality.costMultplier * statBonus.costMultplier * material.costMultplier * ability.costMultplier;
  weaponItem.baseDice = mWorld.addDice({ base.bonusDice, quality.bonusDice, statBonus.bonusDice, material.bonusDice });
  weaponItem.bonusStat = mWorld.addStat({ base.bonusStat, quality.bonusStat, statBonus.bonusStat, material.bonusStat });
  weaponItem.abilityDice = ability.bonusDice;
  weaponItem.attribute = ability.attribute;

  weaponItem.id = mWorld.generateID();
  return weaponItem;
}

void Weapon::loadWeapon()
{
  //New Weapons should always be introduced at the end of their corresponding type list.

  WeaponBase prototype;

  prototype.name = "weapon";
  prototype.type = WeaponType::sword;
  prototype.baseCost = 10;
  prototype.bonusDice.face = 4;
  prototype.bonusDice.roll = 12;
  prototype.bonusDice.plus = 0;
  prototype.bonusStat = World::Stat{ 0,0,0,0,0,0,0,0,0,0 };

  WeaponBase weapon;

  //Ruleset v1: sword slot 0-4
  weapon = prototype;
  weapon.name = "Kitchen Knife";
  weapon.type = WeaponType::sword;
  weapon.baseCost = 8;
  weapon.bonusDice.face = 9;
  weapon.bonusDice.roll = 5;
  weapon.bonusStat.Dex = 500;
  weapon.bonusStat.Lrn = 1000;
  weapon.bonusStat.Acc = -500;
  weapon.bonusStat.Cri = 1500;
  mSword.push_back(weapon);

  weapon = prototype;
  weapon.name = "Dagger";
  weapon.type = WeaponType::sword;
  weapon.baseCost = 11;
  weapon.bonusDice.face = 8;
  weapon.bonusDice.roll = 6;
  weapon.bonusStat.Dex = 1500;
  weapon.bonusStat.Acc = 0;
  weapon.bonusStat.Cri = 1000;
  mSword.push_back(weapon);

  weapon = prototype;
  weapon.name = "Long Sword";
  weapon.type = WeaponType::sword;
  weapon.baseCost = 13;
  weapon.bonusDice.face = 4;
  weapon.bonusDice.roll = 14;
  weapon.bonusStat.Str = 10;
  weapon.bonusStat.Dex = 500;
  weapon.bonusStat.Acc = 500;
  weapon.bonusStat.Cri = 0;
  mSword.push_back(weapon);

  weapon = prototype;
  weapon.name = "Katana";
  weapon.type = WeaponType::sword;
  weapon.baseCost = 14;
  weapon.bonusDice.face = 6;
  weapon.bonusDice.roll = 9;
  weapon.bonusStat.Str = 5;
  weapon.bonusStat.Dex = 1500;
  weapon.bonusStat.Acc = 500;
  weapon.bonusStat.Cri = 500;
  mSword.push_back(weapon);

  weapon = prototype;
  weapon.name = "Rapier";
  weapon.type = WeaponType::sword;
  weapon.baseCost = 15;
  weapon.bonusDice.face = 2;
  weapon.bonusDice.roll = 28;
  weapon.bonusStat.Dex = 2000;
  weapon.bonusStat.Acc = 0;
  weapon.bonusStat.Cri = 1000;
  mSword.push_back(weapon);

  //Ruleset v1: axe slot 0-3
  weapon = prototype;
  weapon.name = "Hatchet";
  weapon.type = WeaponType::axe;
  weapon.baseCost = 12;
  weapon.bonusDice.face = 3;
  weapon.bonusDice.roll = 18;
  weapon.bonusStat.Str = 1500;
  weapon.bonusStat.Dex = 500;
  weapon.bonusStat.Acc = -500;
  weapon.bonusStat.Cri = 0;
  mAxe.push_back(weapon);

  weapon = prototype;
  weapon.name = "Tomahawk";
  weapon.type = WeaponType::axe;
  weapon.baseCost = 13;
  weapon.bonusDice.face = 3;
  weapon.bonusDice.roll = 18;
  weapon.bonusDice.plus = 2;
  weapon.bonusStat.Str = 500;
  weapon.bonusStat.Con = 1500;
  weapon.bonusStat.Acc = -500;
  weapon.bonusStat.Cri = 0;
  mAxe.push_back(weapon);

  weapon = prototype;
  weapon.name = "Battle Axe";
  weapon.type = WeaponType::axe;
  weapon.baseCost = 15;
  weapon.bonusDice.face = 3;
  weapon.bonusDice.roll = 14;
  weapon.bonusDice.plus = 4;
  weapon.bonusStat.Str = 2500;
  weapon.bonusStat.Acc = -1000;
  weapon.bonusStat.Cri = 0;
  mAxe.push_back(weapon);

  weapon = prototype;
  weapon.name = "Dane Axe";
  weapon.type = WeaponType::axe;
  weapon.baseCost = 16;
  weapon.bonusDice.face = 3;
  weapon.bonusDice.roll = 18;
  weapon.bonusDice.plus = 2;
  weapon.bonusStat.Str = 1500;
  weapon.bonusStat.Con = 500;
  weapon.bonusStat.Acc = -1000;
  weapon.bonusStat.Cri = 0;
  mAxe.push_back(weapon);

  //Ruleset v1: bludgeon slot 0-4
  weapon = prototype;
  weapon.name = "Club";
  weapon.type = WeaponType::bludgeon;
  weapon.baseCost = 8;
  weapon.bonusDice.face = 3;
  weapon.bonusDice.roll = 16;
  weapon.bonusDice.plus = 2;
  weapon.bonusStat.Str = 1000;
  weapon.bonusStat.Con = 1500;
  weapon.bonusStat.Acc = -1000;
  weapon.bonusStat.Cri = 0;
  mBludgeon.push_back(weapon);

  weapon = prototype;
  weapon.name = "Mace";
  weapon.type = WeaponType::bludgeon;
  weapon.baseCost = 12;
  weapon.bonusDice.face = 3;
  weapon.bonusDice.roll = 18;
  weapon.bonusDice.plus = 2;
  weapon.bonusStat.Wil = 2500;
  weapon.bonusStat.Acc = -1000;
  weapon.bonusStat.Cri = 0;
  mBludgeon.push_back(weapon);

  weapon = prototype;
  weapon.name = "Hammer";
  weapon.type = WeaponType::bludgeon;
  weapon.baseCost = 12;
  weapon.bonusDice.face = 1;
  weapon.bonusDice.roll = 32;
  weapon.bonusDice.plus = 10;
  weapon.bonusStat.Con = 500;
  weapon.bonusStat.Wil = 1500;
  weapon.bonusStat.Acc = -1000;
  weapon.bonusStat.Cri = 0;
  mBludgeon.push_back(weapon);

  weapon = prototype;
  weapon.name = "Flail";
  weapon.type = WeaponType::bludgeon;
  weapon.baseCost = 13;
  weapon.bonusDice.face = 4;
  weapon.bonusDice.roll = 14;
  weapon.bonusStat.Con = 1500;
  weapon.bonusStat.Dex = 1000;
  weapon.bonusStat.Acc = -1500;
  weapon.bonusStat.Cri = 0;
  mBludgeon.push_back(weapon);

  weapon = prototype;
  weapon.name = "Warhammer";
  weapon.type = WeaponType::bludgeon;
  weapon.baseCost = 15;
  weapon.bonusDice.face = 1;
  weapon.bonusDice.roll = 54;
  weapon.bonusStat.Str = 1500;
  weapon.bonusStat.Con = 500;
  weapon.bonusStat.Acc = -1000;
  weapon.bonusStat.Cri = 0;
  mBludgeon.push_back(weapon);

  //Ruleset v1: stave slot 0-6
  weapon = prototype;
  weapon.name = "Walking Stick";
  weapon.type = WeaponType::stave;
  weapon.baseCost = 6;
  weapon.bonusDice.face = 3;
  weapon.bonusDice.roll = 12;
  weapon.bonusStat.Wil = 3000;
  weapon.bonusStat.Acc = -1000;
  weapon.bonusStat.Cri = 0;
  mStave.push_back(weapon);

  weapon = prototype;
  weapon.name = "Twig";
  weapon.type = WeaponType::stave;
  weapon.baseCost = 7;
  weapon.bonusDice.face = 3;
  weapon.bonusDice.roll = 10;
  weapon.bonusStat.Wil = 2000;
  weapon.bonusStat.Mag = 1000;
  weapon.bonusStat.Acc = -1000;
  weapon.bonusStat.Cri = 0;
  mStave.push_back(weapon);

  weapon = prototype;
  weapon.name = "Tree Branch";
  weapon.type = WeaponType::stave;
  weapon.baseCost = 7;
  weapon.bonusDice.face = 3;
  weapon.bonusDice.roll = 12;
  weapon.bonusStat.Wil = 1500;
  weapon.bonusStat.Mag = 1500;
  weapon.bonusStat.Acc = -500;
  weapon.bonusStat.Cri = 0;
  mStave.push_back(weapon);
  
  weapon = prototype;
  weapon.name = "Broom";
  weapon.type = WeaponType::stave;
  weapon.baseCost = 8;
  weapon.bonusDice.face = 3;
  weapon.bonusDice.roll = 12;
  weapon.bonusStat.Wil = 2000;
  weapon.bonusStat.Mag = 1500;
  weapon.bonusStat.Acc = -500;
  weapon.bonusStat.Cri = 0;
  mStave.push_back(weapon);

  weapon = prototype;
  weapon.name = "Stave";
  weapon.type = WeaponType::stave;
  weapon.baseCost = 16;
  weapon.bonusDice.face = 2;
  weapon.bonusDice.roll = 18;
  weapon.bonusStat.Mag = 3500;
  weapon.bonusStat.Acc = -500;
  weapon.bonusStat.Cri = 0;
  mStave.push_back(weapon);

  weapon = prototype;
  weapon.name = "Long Stave";
  weapon.type = WeaponType::stave;
  weapon.baseCost = 18;
  weapon.bonusDice.face = 2;
  weapon.bonusDice.roll = 20;
  weapon.bonusStat.Wil = 1000;
  weapon.bonusStat.Mag = 2500;
  weapon.bonusStat.Acc = -500;
  weapon.bonusStat.Cri = 0;
  mStave.push_back(weapon);

  weapon = prototype;
  weapon.name = "Magic Stave";
  weapon.type = WeaponType::stave;
  weapon.baseCost = 20;
  weapon.bonusDice.face = 2;
  weapon.bonusDice.roll = 15;
  weapon.bonusStat.Mag = 5500;
  weapon.bonusStat.Acc = -1500;
  weapon.bonusStat.Cri = 0;
  mStave.push_back(weapon);

  //Ruleset v1: polearm slot 0-5
  weapon = prototype;
  weapon.name = "Spear";
  weapon.type = WeaponType::polearm;
  weapon.baseCost = 11;
  weapon.bonusDice.face = 2;
  weapon.bonusDice.roll = 24;
  weapon.bonusStat.Dex = 1500;
  weapon.bonusStat.Acc = 0;
  weapon.bonusStat.Cri = 1500;
  mPolearm.push_back(weapon);

  weapon = prototype;
  weapon.name = "Trident";
  weapon.type = WeaponType::polearm;
  weapon.baseCost = 12;
  weapon.bonusDice.face = 3;
  weapon.bonusDice.roll = 18;
  weapon.bonusStat.Dex = 2000;
  weapon.bonusStat.Acc = 0;
  weapon.bonusStat.Cri = 1000;
  mPolearm.push_back(weapon);

  weapon = prototype;
  weapon.name = "Harpoon";
  weapon.type = WeaponType::polearm;
  weapon.baseCost = 12;
  weapon.bonusDice.face = 2;
  weapon.bonusDice.roll = 26;
  weapon.bonusStat.Per = 1500;
  weapon.bonusStat.Lrn = 1000;
  weapon.bonusStat.Acc = 500;
  weapon.bonusStat.Cri = 1500;
  mPolearm.push_back(weapon);

  weapon = prototype;
  weapon.name = "Lance";
  weapon.type = WeaponType::polearm;
  weapon.baseCost = 13;
  weapon.bonusDice.face = 1;
  weapon.bonusDice.roll = 44;
  weapon.bonusStat.Str = 1500;
  weapon.bonusStat.Dex = 1000;
  weapon.bonusStat.Acc = 1000;
  weapon.bonusStat.Cri = 500;
  mPolearm.push_back(weapon);

  weapon = prototype;
  weapon.name = "Nginata";
  weapon.type = WeaponType::polearm;
  weapon.baseCost = 15;
  weapon.bonusDice.face = 3;
  weapon.bonusDice.roll = 20;
  weapon.bonusStat.Str = 400;
  weapon.bonusStat.Dex = 1800;
  weapon.bonusStat.Acc = 1500;
  weapon.bonusStat.Cri = 0;
  mPolearm.push_back(weapon);

  weapon = prototype;
  weapon.name = "War Scythe";
  weapon.type = WeaponType::polearm;
  weapon.baseCost = 16;
  weapon.bonusDice.face = 3;
  weapon.bonusDice.roll = 22;
  weapon.bonusStat.Str = 1400;
  weapon.bonusStat.Dex = 100;
  weapon.bonusStat.Acc = 0;
  weapon.bonusStat.Cri = 1500;
  mPolearm.push_back(weapon);
}

void Weapon::loadMaterial()
{
  //New Materials should always be introduced at the end of this list.
  //Ruleset v1: slot 0-11

  WeaponMaterial prototype;

  prototype.name = "material";
  prototype.artifactName = "material";
  prototype.costMultplier = 10;
  prototype.bonusDice.face = 4;
  prototype.bonusDice.roll = 12;
  prototype.bonusDice.plus = 0;
  prototype.bonusStat = World::Stat{ 0,0,0,0,0,0,0,0,0,0 };

  WeaponMaterial material;

  material = prototype;
  material.name = "Paper";
  material.artifactName = "Silly";
  material.costMultplier = 8;
  material.bonusDice.face = 0;
  material.bonusDice.roll = -2;
  material.bonusDice.plus = -2;
  material.bonusStat.Lrn = 500;
  material.bonusStat.Acc = 1000;
  material.bonusStat.Cri = -1000;
  mMaterials.push_back(material);

  material = prototype;
  material.name = "Wooden";
  material.artifactName = "Brambled";
  material.costMultplier = 9;
  material.bonusDice.face = 0;
  material.bonusDice.roll = -3;
  material.bonusStat.Lrn = 500;
  material.bonusStat.Acc = 1000;
  material.bonusStat.Cri = 0;
  mMaterials.push_back(material);

  material = prototype;
  material.name = "Bone";
  material.artifactName = "Immortal";
  material.costMultplier = 9;
  material.bonusDice.face = 0;
  material.bonusDice.roll = -1;
  material.bonusDice.plus = 1;
  material.bonusStat.Mag = 500;
  material.bonusStat.Acc = 1000;
  material.bonusStat.Cri = 10;
  mMaterials.push_back(material);

  material = prototype;
  material.name = "Coral";
  material.artifactName = "Oceanic";
  material.costMultplier = 10;
  material.bonusDice.face = 1;
  material.bonusDice.roll = -2;
  material.bonusDice.plus = -2;
  material.bonusStat.Wil = 500;
  material.bonusStat.Acc = 500;
  material.bonusStat.Cri = 1000;
  mMaterials.push_back(material);

  material = prototype;
  material.name = "Bronze";
  material.artifactName = "Noble";
  material.costMultplier = 11;
  material.bonusDice.face = 0;
  material.bonusDice.roll = 1;
  material.bonusStat.Dex = 500;
  material.bonusStat.Acc = 1000;
  material.bonusStat.Cri = 0;
  mMaterials.push_back(material);

  material = prototype;
  material.name = "Iron";
  material.artifactName = "Heavy";
  material.costMultplier = 12;
  material.bonusDice.face = 0;
  material.bonusDice.roll = 2;
  material.bonusDice.plus = 1;
  material.bonusStat.Str = 500;
  material.bonusStat.Acc = 0;
  material.bonusStat.Cri = 0;
  mMaterials.push_back(material);

  material = prototype;
  material.name = "Glass";
  material.artifactName = "Transparent";
  material.costMultplier = 14;
  material.bonusDice.face = 0;
  material.bonusDice.roll = 4;
  material.bonusDice.plus = -4;
  material.bonusStat.Dex = 1500;
  material.bonusStat.Acc = 1500;
  material.bonusStat.Cri = 1500;
  mMaterials.push_back(material);

  material = prototype;
  material.name = "Steel";
  material.artifactName = "Historic";
  material.costMultplier = 15;
  material.bonusDice.face = 1;
  material.bonusDice.roll = 0;
  material.bonusDice.plus = +2;
  material.bonusStat.Con = 1000;
  material.bonusStat.Acc = -500;
  material.bonusStat.Cri = 0;
  mMaterials.push_back(material);

  material = prototype;
  material.name = "Gold";
  material.artifactName = "Golden";
  material.costMultplier = 21;
  material.bonusDice.face = 2;
  material.bonusDice.roll = -4;
  material.bonusDice.plus = 2;
  material.bonusStat.Str = 1000;
  material.bonusStat.Acc = -1000;
  material.bonusStat.Cri = 0;
  mMaterials.push_back(material);

  material = prototype;
  material.name = "Diamond";
  material.artifactName = "Ever lasting";
  material.costMultplier = 25;
  material.bonusDice.face = 0;
  material.bonusDice.roll = 6;
  material.bonusDice.plus = 6;
  material.bonusStat.Con = 500;
  material.bonusStat.Acc = 0;
  material.bonusStat.Cri = 1000;
  mMaterials.push_back(material);

  material = prototype;
  material.name = "Mithril";
  material.artifactName = "Ancient";
  material.costMultplier = 30;
  material.bonusDice.face = 0;
  material.bonusDice.roll = 14;
  material.bonusDice.plus = 2;
  material.bonusStat.Mag = 1000;
  material.bonusStat.Acc = 500;
  material.bonusStat.Cri = 500;
  mMaterials.push_back(material);

  material = prototype;
  material.name = "Adamantium";
  material.artifactName = "Earthly";
  material.costMultplier = 31;
  material.bonusDice.face = 2;
  material.bonusDice.roll = 2;
  material.bonusDice.plus = 4;
  material.bonusStat.Con = 1000;
  material.bonusStat.Acc = -1000;
  material.bonusStat.Cri = 1000;
  mMaterials.push_back(material);
}

void Weapon::loadQuality()
{
  WeaponQuality prototype;

  prototype.name = "quality";
  prototype.costMultplier = 10;
  prototype.bonusDice.face = 4;
  prototype.bonusDice.roll = 12;
  prototype.bonusDice.plus = 0;
  prototype.bonusStat = World::Stat{ 0,0,0,0,0,0,0,0,0,0 };

  WeaponQuality quality;

  quality = prototype;
  quality.name = "A Worned";
  quality.costMultplier = 5;
  quality.bonusDice.face = 0;
  quality.bonusDice.roll = -3;
  quality.bonusDice.plus = -2;
  quality.bonusStat.Lrn = 500;
  quality.bonusStat.Acc = -1000;
  quality.bonusStat.Cri = -1000;
  mQualities.push_back(quality);

  quality = prototype;
  quality.name = "An Inferior";
  quality.costMultplier = 10;
  quality.bonusDice.face = 0;
  quality.bonusDice.roll = -2;
  quality.bonusDice.plus = -1;
  quality.bonusStat.Acc = -500;
  quality.bonusStat.Cri = -500;
  mQualities.push_back(quality);

  quality = prototype;
  quality.name = "A Normal";
  quality.costMultplier = 14;
  quality.bonusDice.face = 0;
  quality.bonusDice.roll = 0;
  quality.bonusStat.Acc = 0;
  quality.bonusStat.Cri = 0;
  mQualities.push_back(quality);

  quality = prototype;
  quality.name = "A Superior";
  quality.costMultplier = 18;
  quality.bonusDice.face = 0;
  quality.bonusDice.roll = 1;
  quality.bonusDice.plus = 1;
  quality.bonusStat.Wil = 500;
  quality.bonusStat.Acc = 500;
  quality.bonusStat.Cri = 500;
  mQualities.push_back(quality);

  quality = prototype;
  quality.name = "A Miracle";
  quality.costMultplier = 30;
  quality.bonusDice.face = 0;
  quality.bonusDice.roll = 1;
  quality.bonusDice.plus = 1;
  quality.bonusStat.Wil = 500;
  quality.bonusStat.Chr = 500;
  quality.bonusStat.Mag = 500;
  quality.bonusStat.Acc = 500;
  quality.bonusStat.Cri = 1000;
  mQualities.push_back(quality);

  quality = prototype;
  quality.name = "The"; //Artifact Qually
  quality.costMultplier = 75;
  quality.bonusDice.face = 1;
  quality.bonusDice.roll = 3;
  quality.bonusDice.plus = 5;
  quality.bonusStat.Lrn = 500;
  quality.bonusStat.Wil = 500;
  quality.bonusStat.Chr = 500;
  quality.bonusStat.Mag = 500;
  quality.bonusStat.Acc = 1000;
  quality.bonusStat.Cri = 1000;
  mQualities.push_back(quality);
}

void Weapon::loadAbility()
{
  //New Ability should always be introduced at the end of this list.
  //Ruleset v1: slot 0-10

  WeaponAbility prototype;

  prototype.name = "ability";
  prototype.costMultplier = 10;
  prototype.bonusDice.face = 2;
  prototype.bonusDice.roll = 6;
  prototype.bonusDice.plus = 0;
  prototype.bonusStat = World::Stat{ 0,0,0,0,0,0,0,0,0,0 };
  prototype.attribute = World::Element::normal;

  WeaponAbility ability;

  ability = prototype;
  ability.name = ""; //No Attribute
  ability.costMultplier = 10;
  ability.bonusDice.face = 0;
  ability.bonusDice.roll = 0;
  ability.bonusDice.plus = 0;
  prototype.attribute = World::Element::normal;
  mAbilities.push_back(ability);

  ability = prototype;
  ability.name = "of Fire";
  ability.costMultplier = 15;
  ability.bonusDice.face = 2;
  ability.bonusDice.roll = 6;
  ability.bonusDice.plus = 0;
  prototype.attribute = World::Element::fire;
  mAbilities.push_back(ability);

  ability = prototype;
  ability.name = "of Water";
  ability.costMultplier = 15;
  ability.bonusDice.face = 2;
  ability.bonusDice.roll = 6;
  ability.bonusDice.plus = 0;
  prototype.attribute = World::Element::water;
  mAbilities.push_back(ability);

  ability = prototype;
  ability.name = "of Earth";
  ability.costMultplier = 15;
  ability.bonusDice.face = 2;
  ability.bonusDice.roll = 6;
  ability.bonusDice.plus = 0;
  prototype.attribute = World::Element::water;
  mAbilities.push_back(ability);

  ability = prototype;
  ability.name = "of Air";
  ability.costMultplier = 15;
  ability.bonusDice.face = 2;
  ability.bonusDice.roll = 6;
  ability.bonusDice.plus = 0;
  prototype.attribute = World::Element::air;
  mAbilities.push_back(ability);

  ability = prototype;
  ability.name = "of Lightning";
  ability.costMultplier = 20;
  ability.bonusDice.face = 2;
  ability.bonusDice.roll = 8;
  ability.bonusDice.plus = 0;
  prototype.attribute = World::Element::lightning;
  mAbilities.push_back(ability);

  ability = prototype;
  ability.name = "of Technomancy";
  ability.costMultplier = 20;
  ability.bonusDice.face = 2;
  ability.bonusDice.roll = 12;
  ability.bonusDice.plus = 0;
  prototype.attribute = World::Element::machine;
  mAbilities.push_back(ability);

  ability = prototype;
  ability.name = "of Decay";
  ability.costMultplier = 20;
  ability.bonusDice.face = 4;
  ability.bonusDice.roll = 4;
  ability.bonusDice.plus = 0;
  prototype.attribute = World::Element::poison;
  mAbilities.push_back(ability);

  ability = prototype;
  ability.name = "of Sanity";
  ability.costMultplier = 20;
  ability.bonusDice.face = 1;
  ability.bonusDice.roll = 16;
  ability.bonusDice.plus = 0;
  prototype.attribute = World::Element::chaos;
  mAbilities.push_back(ability);

  ability = prototype;
  ability.name = "of Blessing";
  ability.costMultplier = 25;
  ability.bonusDice.face = 3;
  ability.bonusDice.roll = 6;
  ability.bonusDice.plus = 0;
  prototype.attribute = World::Element::holy;
  mAbilities.push_back(ability);

  ability = prototype;
  ability.name = "of Doom";
  ability.costMultplier = 25;
  ability.bonusDice.face = 3;
  ability.bonusDice.roll = 6;
  ability.bonusDice.plus = 0;
  prototype.attribute = World::Element::dark;
  mAbilities.push_back(ability);
}

void Weapon::loadStatBonus()
{
  //New StatBonus Ability should always be introduced at the end of this list.
  //Ruleset v1: slot 0-7

  WeaponStatBonus prototype;

  prototype.name = "statbonus";
  prototype.costMultplier = 10;
  prototype.bonusDice.face = 0;
  prototype.bonusDice.roll = 0;
  prototype.bonusDice.plus = 1;
  prototype.bonusStat = World::Stat{ 0,0,0,0,0,0,0,0,0,0 };

  WeaponStatBonus statBonus;

  statBonus = prototype;
  statBonus.name = ""; //No Extra Stats
  statBonus.costMultplier = 10;
  mStatBonuses.push_back(statBonus);

  statBonus = prototype;
  statBonus.name = "Crushing";
  statBonus.costMultplier = 12;
  statBonus.bonusStat.Str = 2000;
  mStatBonuses.push_back(statBonus);

  statBonus = prototype;
  statBonus.name = "Mighty";
  statBonus.costMultplier = 12;
  statBonus.bonusStat.Con = 2000;
  mStatBonuses.push_back(statBonus);

  statBonus = prototype;
  statBonus.name = "Dancing";
  statBonus.costMultplier = 12;
  statBonus.bonusStat.Dex = 2000;
  mStatBonuses.push_back(statBonus);

  statBonus = prototype;
  statBonus.name = "Conspicuous";
  statBonus.costMultplier = 12;
  statBonus.bonusStat.Per = 2000;
  mStatBonuses.push_back(statBonus);

  statBonus = prototype;
  statBonus.name = "Trustworthy";
  statBonus.costMultplier = 12;
  statBonus.bonusStat.Lrn = 2000;
  mStatBonuses.push_back(statBonus);

  statBonus = prototype;
  statBonus.name = "Glowing";
  statBonus.costMultplier = 12;
  statBonus.bonusStat.Wil = 2000;
  mStatBonuses.push_back(statBonus);

  statBonus = prototype;
  statBonus.name = "Magical";
  statBonus.costMultplier = 12;
  statBonus.bonusStat.Mag = 2000;
  mStatBonuses.push_back(statBonus);
}