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

void Weapon::loadWeapon()
{
  //New Weapons should always be introduced at the end of this list.
  //Ruleset v1: slot 0-23

  WeaponItem prototype;

  prototype.name = "weapon";
  prototype.type = WeaponType::sword;
  prototype.baseCost = 10;
  prototype.bonusDice.face = 4;
  prototype.bonusDice.roll = 12;
  prototype.bonusDice.plus = 0;
  prototype.bonusStat.Str = 0;
  prototype.bonusStat.Con = 0;
  prototype.bonusStat.Dex = 0;
  prototype.bonusStat.Per = 0;
  prototype.bonusStat.Lrn = 0;
  prototype.bonusStat.Wil = 0;
  prototype.bonusStat.Mag = 0;
  prototype.bonusStat.Acc = 0;
  prototype.bonusStat.Cri = 0;

  WeaponItem weapon;

  weapon = prototype;
  weapon.name = "Kitchen Knife";
  weapon.type = WeaponType::sword;
  weapon.baseCost = 8;
  weapon.bonusDice.face = 9;
  weapon.bonusDice.roll = 5;
  weapon.bonusStat.Dex = 5;
  weapon.bonusStat.Lrn = 10;
  weapon.bonusStat.Acc = -5;
  weapon.bonusStat.Cri = 15;
  mWeapons.push_back(weapon);

  weapon = prototype;
  weapon.name = "Dagger";
  weapon.type = WeaponType::sword;
  weapon.baseCost = 11;
  weapon.bonusDice.face = 8;
  weapon.bonusDice.roll = 6;
  weapon.bonusStat.Dex = 15;
  weapon.bonusStat.Acc = 0;
  weapon.bonusStat.Cri = 10;
  mWeapons.push_back(weapon);

  weapon = prototype;
  weapon.name = "Long Sword";
  weapon.type = WeaponType::sword;
  weapon.baseCost = 13;
  weapon.bonusDice.face = 4;
  weapon.bonusDice.roll = 14;
  weapon.bonusStat.Str = 10;
  weapon.bonusStat.Dex = 5;
  weapon.bonusStat.Acc = 5;
  weapon.bonusStat.Cri = 0;
  mWeapons.push_back(weapon);

  weapon = prototype;
  weapon.name = "Katana";
  weapon.type = WeaponType::sword;
  weapon.baseCost = 14;
  weapon.bonusDice.face = 6;
  weapon.bonusDice.roll = 9;
  weapon.bonusStat.Str = 5;
  weapon.bonusStat.Dex = 15;
  weapon.bonusStat.Acc = 5;
  weapon.bonusStat.Cri = 5;
  mWeapons.push_back(weapon);

  weapon = prototype;
  weapon.name = "Rapier";
  weapon.type = WeaponType::sword;
  weapon.baseCost = 15;
  weapon.bonusDice.face = 2;
  weapon.bonusDice.roll = 28;
  weapon.bonusStat.Dex = 20;
  weapon.bonusStat.Acc = 0;
  weapon.bonusStat.Cri = 10;
  mWeapons.push_back(weapon);

  weapon = prototype;
  weapon.name = "Hatchet";
  weapon.type = WeaponType::axe;
  weapon.baseCost = 12;
  weapon.bonusDice.face = 3;
  weapon.bonusDice.roll = 18;
  weapon.bonusStat.Str = 15;
  weapon.bonusStat.Dex = 5;
  weapon.bonusStat.Acc = -5;
  weapon.bonusStat.Cri = 0;
  mWeapons.push_back(weapon);

  weapon = prototype;
  weapon.name = "Tomahawk";
  weapon.type = WeaponType::axe;
  weapon.baseCost = 13;
  weapon.bonusDice.face = 3;
  weapon.bonusDice.roll = 18;
  weapon.bonusDice.plus = 2;
  weapon.bonusStat.Str = 5;
  weapon.bonusStat.Con = 15;
  weapon.bonusStat.Acc = -5;
  weapon.bonusStat.Cri = 0;
  mWeapons.push_back(weapon);

  weapon = prototype;
  weapon.name = "Battle Axe";
  weapon.type = WeaponType::axe;
  weapon.baseCost = 15;
  weapon.bonusDice.face = 3;
  weapon.bonusDice.roll = 14;
  weapon.bonusDice.plus = 4;
  weapon.bonusStat.Str = 25;
  weapon.bonusStat.Acc = -10;
  weapon.bonusStat.Cri = 0;
  mWeapons.push_back(weapon);

  weapon = prototype;
  weapon.name = "Dane Axe";
  weapon.type = WeaponType::axe;
  weapon.baseCost = 16;
  weapon.bonusDice.face = 3;
  weapon.bonusDice.roll = 18;
  weapon.bonusDice.plus = 2;
  weapon.bonusStat.Str = 15;
  weapon.bonusStat.Con = 5;
  weapon.bonusStat.Acc = -10;
  weapon.bonusStat.Cri = 0;
  mWeapons.push_back(weapon);

  weapon = prototype;
  weapon.name = "Club";
  weapon.type = WeaponType::blunt;
  weapon.baseCost = 8;
  weapon.bonusDice.face = 3;
  weapon.bonusDice.roll = 16;
  weapon.bonusDice.plus = 2;
  weapon.bonusStat.Str = 10;
  weapon.bonusStat.Con = 15;
  weapon.bonusStat.Acc = -10;
  weapon.bonusStat.Cri = 0;
  mWeapons.push_back(weapon);

  weapon = prototype;
  weapon.name = "Mace";
  weapon.type = WeaponType::blunt;
  weapon.baseCost = 12;
  weapon.bonusDice.face = 3;
  weapon.bonusDice.roll = 18;
  weapon.bonusDice.plus = 2;
  weapon.bonusStat.Wil = 25;
  weapon.bonusStat.Acc = -10;
  weapon.bonusStat.Cri = 0;
  mWeapons.push_back(weapon);

  weapon = prototype;
  weapon.name = "Hammer";
  weapon.type = WeaponType::blunt;
  weapon.baseCost = 12;
  weapon.bonusDice.face = 1;
  weapon.bonusDice.roll = 32;
  weapon.bonusDice.plus = 10;
  weapon.bonusStat.Con = 5;
  weapon.bonusStat.Wil = 15;
  weapon.bonusStat.Acc = -10;
  weapon.bonusStat.Cri = 0;
  mWeapons.push_back(weapon);

  weapon = prototype;
  weapon.name = "Flail";
  weapon.type = WeaponType::blunt;
  weapon.baseCost = 13;
  weapon.bonusDice.face = 4;
  weapon.bonusDice.roll = 14;
  weapon.bonusStat.Con = 15;
  weapon.bonusStat.Dex = 10;
  weapon.bonusStat.Acc = -15;
  weapon.bonusStat.Cri = 0;
  mWeapons.push_back(weapon);

  weapon = prototype;
  weapon.name = "Warhammer";
  weapon.type = WeaponType::blunt;
  weapon.baseCost = 15;
  weapon.bonusDice.face = 1;
  weapon.bonusDice.roll = 54;
  weapon.bonusStat.Str = 15;
  weapon.bonusStat.Con = 5;
  weapon.bonusStat.Acc = -10;
  weapon.bonusStat.Cri = 0;
  mWeapons.push_back(weapon);

  weapon = prototype;
  weapon.name = "Walking Stick";
  weapon.type = WeaponType::stave;
  weapon.baseCost = 6;
  weapon.bonusDice.face = 3;
  weapon.bonusDice.roll = 12;
  weapon.bonusStat.Wil = 20;
  weapon.bonusStat.Mag = 10;
  weapon.bonusStat.Acc = -10;
  weapon.bonusStat.Cri = 0;
  mWeapons.push_back(weapon);
  
  weapon = prototype;
  weapon.name = "Stave";
  weapon.type = WeaponType::stave;
  weapon.baseCost = 16;
  weapon.bonusDice.face = 2;
  weapon.bonusDice.roll = 18;
  weapon.bonusStat.Mag = 35;
  weapon.bonusStat.Acc = -5;
  weapon.bonusStat.Cri = 0;
  mWeapons.push_back(weapon);

  weapon = prototype;
  weapon.name = "Long Stave";
  weapon.type = WeaponType::stave;
  weapon.baseCost = 18;
  weapon.bonusDice.face = 2;
  weapon.bonusDice.roll = 20;
  weapon.bonusStat.Wil = 10;
  weapon.bonusStat.Mag = 25;
  weapon.bonusStat.Acc = -5;
  weapon.bonusStat.Cri = 0;
  mWeapons.push_back(weapon);

  weapon = prototype;
  weapon.name = "Magic Stave";
  weapon.type = WeaponType::stave;
  weapon.baseCost = 20;
  weapon.bonusDice.face = 2;
  weapon.bonusDice.roll = 15;
  weapon.bonusStat.Mag = 55;
  weapon.bonusStat.Acc = -15;
  weapon.bonusStat.Cri = 0;
  mWeapons.push_back(weapon);

  weapon = prototype;
  weapon.name = "Spear";
  weapon.type = WeaponType::polearm;
  weapon.baseCost = 11;
  weapon.bonusDice.face = 2;
  weapon.bonusDice.roll = 24;
  weapon.bonusStat.Dex = 15;
  weapon.bonusStat.Acc = 0;
  weapon.bonusStat.Cri = 15;
  mWeapons.push_back(weapon);

  weapon = prototype;
  weapon.name = "Trident";
  weapon.type = WeaponType::polearm;
  weapon.baseCost = 12;
  weapon.bonusDice.face = 3;
  weapon.bonusDice.roll = 18;
  weapon.bonusStat.Dex = 20;
  weapon.bonusStat.Acc = 0;
  weapon.bonusStat.Cri = 10;
  mWeapons.push_back(weapon);

  weapon = prototype;
  weapon.name = "Harpoon";
  weapon.type = WeaponType::polearm;
  weapon.baseCost = 12;
  weapon.bonusDice.face = 2;
  weapon.bonusDice.roll = 26;
  weapon.bonusStat.Per = 15;
  weapon.bonusStat.Lrn = 10;
  weapon.bonusStat.Acc = 5;
  weapon.bonusStat.Cri = 15;
  mWeapons.push_back(weapon);

  weapon = prototype;
  weapon.name = "Lance";
  weapon.type = WeaponType::polearm;
  weapon.baseCost = 13;
  weapon.bonusDice.face = 1;
  weapon.bonusDice.roll = 44;
  weapon.bonusStat.Str = 15;
  weapon.bonusStat.Dex = 10;
  weapon.bonusStat.Acc = 10;
  weapon.bonusStat.Cri = 5;
  mWeapons.push_back(weapon);

  weapon = prototype;
  weapon.name = "Nginata";
  weapon.type = WeaponType::polearm;
  weapon.baseCost = 15;
  weapon.bonusDice.face = 3;
  weapon.bonusDice.roll = 20;
  weapon.bonusStat.Str = 4;
  weapon.bonusStat.Dex = 18;
  weapon.bonusStat.Acc = 15;
  weapon.bonusStat.Cri = 0;
  mWeapons.push_back(weapon);

  weapon = prototype;
  weapon.name = "War Scythe";
  weapon.type = WeaponType::polearm;
  weapon.baseCost = 16;
  weapon.bonusDice.face = 3;
  weapon.bonusDice.roll = 22;
  weapon.bonusStat.Str = 14;
  weapon.bonusStat.Dex = 1;
  weapon.bonusStat.Acc = 0;
  weapon.bonusStat.Cri = 15;
  mWeapons.push_back(weapon);
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
  prototype.bonusStat.Str = 0;
  prototype.bonusStat.Con = 0;
  prototype.bonusStat.Dex = 0;
  prototype.bonusStat.Per = 0;
  prototype.bonusStat.Lrn = 0;
  prototype.bonusStat.Wil = 0;
  prototype.bonusStat.Mag = 0;
  prototype.bonusStat.Acc = 0;
  prototype.bonusStat.Cri = 0;

  WeaponMaterial material;

  material = prototype;
  material.name = "Paper";
  material.artifactName = "Silly";
  material.costMultplier = 8;
  material.bonusDice.face = 0;
  material.bonusDice.roll = -2;
  material.bonusDice.plus = -2;
  material.bonusStat.Lrn = 5;
  material.bonusStat.Acc = 10;
  material.bonusStat.Cri = -10;
  mMaterials.push_back(material);

  material = prototype;
  material.name = "Wooden";
  material.artifactName = "Brambled";
  material.costMultplier = 9;
  material.bonusDice.face = 0;
  material.bonusDice.roll = -3;
  material.bonusStat.Lrn = 5;
  material.bonusStat.Acc = 10;
  material.bonusStat.Cri = 0;
  mMaterials.push_back(material);

  material = prototype;
  material.name = "Bone";
  material.artifactName = "Immortal";
  material.costMultplier = 9;
  material.bonusDice.face = 0;
  material.bonusDice.roll = -1;
  material.bonusDice.plus = 1;
  material.bonusStat.Mag = 5;
  material.bonusStat.Acc = 10;
  material.bonusStat.Cri = 10;
  mMaterials.push_back(material);

  material = prototype;
  material.name = "Coral";
  material.artifactName = "Oceanic";
  material.costMultplier = 10;
  material.bonusDice.face = 1;
  material.bonusDice.roll = -2;
  material.bonusDice.plus = -2;
  material.bonusStat.Wil = 5;
  material.bonusStat.Acc = 5;
  material.bonusStat.Cri = 10;
  mMaterials.push_back(material);

  material = prototype;
  material.name = "Bronze";
  material.artifactName = "Noble";
  material.costMultplier = 11;
  material.bonusDice.face = 0;
  material.bonusDice.roll = 1;
  material.bonusStat.Dex = 5;
  material.bonusStat.Acc = 10;
  material.bonusStat.Cri = 0;
  mMaterials.push_back(material);

  material = prototype;
  material.name = "Iron";
  material.artifactName = "Heavy";
  material.costMultplier = 12;
  material.bonusDice.face = 0;
  material.bonusDice.roll = 2;
  material.bonusDice.plus = 1;
  material.bonusStat.Str = 5;
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
  material.bonusStat.Dex = 15;
  material.bonusStat.Acc = 15;
  material.bonusStat.Cri = 15;
  mMaterials.push_back(material);

  material = prototype;
  material.name = "Steel";
  material.artifactName = "Historic";
  material.costMultplier = 15;
  material.bonusDice.face = 1;
  material.bonusDice.roll = 0;
  material.bonusDice.plus = +2;
  material.bonusStat.Con = 10;
  material.bonusStat.Acc = -5;
  material.bonusStat.Cri = 0;
  mMaterials.push_back(material);

  material = prototype;
  material.name = "Gold";
  material.artifactName = "Golden";
  material.costMultplier = 20;
  material.bonusDice.face = 2;
  material.bonusDice.roll = -4;
  material.bonusDice.plus = 2;
  material.bonusStat.Str = 10;
  material.bonusStat.Acc = -10;
  material.bonusStat.Cri = 0;
  mMaterials.push_back(material);

  material = prototype;
  material.name = "Diamond";
  material.artifactName = "Ever lasting";
  material.costMultplier = 25;
  material.bonusDice.face = 0;
  material.bonusDice.roll = 6;
  material.bonusDice.plus = 6;
  material.bonusStat.Con = 5;
  material.bonusStat.Acc = 0;
  material.bonusStat.Cri = 10;
  mMaterials.push_back(material);

  material = prototype;
  material.name = "Mithril";
  material.artifactName = "Ancient";
  material.costMultplier = 30;
  material.bonusDice.face = 0;
  material.bonusDice.roll = 14;
  material.bonusDice.plus = 2;
  material.bonusStat.Mag = 10;
  material.bonusStat.Acc = 5;
  material.bonusStat.Cri = 5;
  mMaterials.push_back(material);

  material = prototype;
  material.name = "Adamantium";
  material.artifactName = "Earthly";
  material.costMultplier = 31;
  material.bonusDice.face = 2;
  material.bonusDice.roll = 2;
  material.bonusDice.plus = 4;
  material.bonusStat.Con = 10;
  material.bonusStat.Acc = -10;
  material.bonusStat.Cri = 10;
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
  prototype.bonusStat.Str = 0;
  prototype.bonusStat.Con = 0;
  prototype.bonusStat.Dex = 0;
  prototype.bonusStat.Per = 0;
  prototype.bonusStat.Lrn = 0;
  prototype.bonusStat.Wil = 0;
  prototype.bonusStat.Mag = 0;
  prototype.bonusStat.Acc = 0;
  prototype.bonusStat.Cri = 0;

  WeaponQuality quality;

  quality = prototype;
  quality.name = "A Worned";
  quality.costMultplier = 5;
  quality.bonusDice.face = 0;
  quality.bonusDice.roll = -3;
  quality.bonusDice.plus = -2;
  quality.bonusStat.Lrn = 5;
  quality.bonusStat.Acc = -10;
  quality.bonusStat.Cri = -10;
  mQualities.push_back(quality);

  quality = prototype;
  quality.name = "An Inferior";
  quality.costMultplier = 10;
  quality.bonusDice.face = 0;
  quality.bonusDice.roll = -2;
  quality.bonusDice.plus = -1;
  quality.bonusStat.Acc = -5;
  quality.bonusStat.Cri = -5;
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
  quality.bonusStat.Wil = 5;
  quality.bonusStat.Acc = 5;
  quality.bonusStat.Cri = 5;
  mQualities.push_back(quality);

  quality = prototype;
  quality.name = "A Miracle";
  quality.costMultplier = 30;
  quality.bonusDice.face = 0;
  quality.bonusDice.roll = 1;
  quality.bonusDice.plus = 1;
  quality.bonusStat.Wil = 5;
  quality.bonusStat.Chr = 5;
  quality.bonusStat.Mag = 5;
  quality.bonusStat.Acc = 5;
  quality.bonusStat.Cri = 10;
  mQualities.push_back(quality);

  quality = prototype;
  quality.name = "The"; //Artifact Qually
  quality.costMultplier = 75;
  quality.bonusDice.face = 1;
  quality.bonusDice.roll = 3;
  quality.bonusDice.plus = 5;
  quality.bonusStat.Lrn = 5;
  quality.bonusStat.Wil = 5;
  quality.bonusStat.Chr = 5;
  quality.bonusStat.Mag = 5;
  quality.bonusStat.Acc = 10;
  quality.bonusStat.Cri = 10;
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
  prototype.bonusStat.Str = 0;
  prototype.bonusStat.Con = 0;
  prototype.bonusStat.Dex = 0;
  prototype.bonusStat.Per = 0;
  prototype.bonusStat.Lrn = 0;
  prototype.bonusStat.Wil = 0;
  prototype.bonusStat.Mag = 0;
  prototype.bonusStat.Acc = 0;
  prototype.bonusStat.Cri = 0;
  prototype.attribute = WeaponAttribute::none;

  WeaponAbility ability;

  ability = prototype;
  ability.name = ""; //No Attribute
  ability.costMultplier = 10;
  ability.bonusDice.face = 0;
  ability.bonusDice.roll = 0;
  ability.bonusDice.plus = 0;
  prototype.attribute = WeaponAttribute::none;
  mAbilities.push_back(ability);

  ability = prototype;
  ability.name = "of Fire";
  ability.costMultplier = 15;
  ability.bonusDice.face = 2;
  ability.bonusDice.roll = 6;
  ability.bonusDice.plus = 0;
  prototype.attribute = WeaponAttribute::fire;
  mAbilities.push_back(ability);

  ability = prototype;
  ability.name = "of Water";
  ability.costMultplier = 15;
  ability.bonusDice.face = 2;
  ability.bonusDice.roll = 6;
  ability.bonusDice.plus = 0;
  prototype.attribute = WeaponAttribute::water;
  mAbilities.push_back(ability);

  ability = prototype;
  ability.name = "of Earth";
  ability.costMultplier = 15;
  ability.bonusDice.face = 2;
  ability.bonusDice.roll = 6;
  ability.bonusDice.plus = 0;
  prototype.attribute = WeaponAttribute::water;
  mAbilities.push_back(ability);

  ability = prototype;
  ability.name = "of Air";
  ability.costMultplier = 15;
  ability.bonusDice.face = 2;
  ability.bonusDice.roll = 6;
  ability.bonusDice.plus = 0;
  prototype.attribute = WeaponAttribute::air;
  mAbilities.push_back(ability);

  ability = prototype;
  ability.name = "of Lightning";
  ability.costMultplier = 20;
  ability.bonusDice.face = 2;
  ability.bonusDice.roll = 8;
  ability.bonusDice.plus = 0;
  prototype.attribute = WeaponAttribute::lightning;
  mAbilities.push_back(ability);

  ability = prototype;
  ability.name = "of Technomancy";
  ability.costMultplier = 20;
  ability.bonusDice.face = 2;
  ability.bonusDice.roll = 12;
  ability.bonusDice.plus = 0;
  prototype.attribute = WeaponAttribute::machine;
  mAbilities.push_back(ability);

  ability = prototype;
  ability.name = "of Decay";
  ability.costMultplier = 20;
  ability.bonusDice.face = 4;
  ability.bonusDice.roll = 4;
  ability.bonusDice.plus = 0;
  prototype.attribute = WeaponAttribute::poison;
  mAbilities.push_back(ability);

  ability = prototype;
  ability.name = "of Sanity";
  ability.costMultplier = 20;
  ability.bonusDice.face = 1;
  ability.bonusDice.roll = 16;
  ability.bonusDice.plus = 0;
  prototype.attribute = WeaponAttribute::chaos;
  mAbilities.push_back(ability);

  ability = prototype;
  ability.name = "of Blessing";
  ability.costMultplier = 25;
  ability.bonusDice.face = 3;
  ability.bonusDice.roll = 6;
  ability.bonusDice.plus = 0;
  prototype.attribute = WeaponAttribute::holy;
  mAbilities.push_back(ability);

  ability = prototype;
  ability.name = "of Doom";
  ability.costMultplier = 25;
  ability.bonusDice.face = 3;
  ability.bonusDice.roll = 6;
  ability.bonusDice.plus = 0;
  prototype.attribute = WeaponAttribute::dark;
  mAbilities.push_back(ability);
}

void Weapon::loadStatBonus()
{
  //New Ability should always be introduced at the end of this list.
  //Ruleset v1: slot 0-7

  WeaponStatBonus prototype;

  prototype.name = "statbonus";
  prototype.costMultplier = 10;
  prototype.bonusDice.face = 0;
  prototype.bonusDice.roll = 0;
  prototype.bonusDice.plus = 1;
  prototype.bonusStat.Str = 0;
  prototype.bonusStat.Con = 0;
  prototype.bonusStat.Dex = 0;
  prototype.bonusStat.Per = 0;
  prototype.bonusStat.Lrn = 0;
  prototype.bonusStat.Wil = 0;
  prototype.bonusStat.Mag = 0;
  prototype.bonusStat.Acc = 0;
  prototype.bonusStat.Cri = 0;

  WeaponStatBonus statBonus;

  statBonus = prototype;
  statBonus.name = ""; //No Extra Stats
  statBonus.costMultplier = 10;
  mStatBonuses.push_back(statBonus);

  statBonus = prototype;
  statBonus.name = "Crushing";
  statBonus.costMultplier = 12;
  statBonus.bonusStat.Str = 20;
  mStatBonuses.push_back(statBonus);

  statBonus = prototype;
  statBonus.name = "Mighty";
  statBonus.costMultplier = 12;
  statBonus.bonusStat.Con = 20;
  mStatBonuses.push_back(statBonus);

  statBonus = prototype;
  statBonus.name = "Dancing";
  statBonus.costMultplier = 12;
  statBonus.bonusStat.Dex = 20;
  mStatBonuses.push_back(statBonus);

  statBonus = prototype;
  statBonus.name = "Conspicuous";
  statBonus.costMultplier = 12;
  statBonus.bonusStat.Per = 20;
  mStatBonuses.push_back(statBonus);

  statBonus = prototype;
  statBonus.name = "Trustworthy";
  statBonus.costMultplier = 12;
  statBonus.bonusStat.Lrn = 20;
  mStatBonuses.push_back(statBonus);

  statBonus = prototype;
  statBonus.name = "Glowing";
  statBonus.costMultplier = 12;
  statBonus.bonusStat.Wil = 20;
  mStatBonuses.push_back(statBonus);

  statBonus = prototype;
  statBonus.name = "Magical";
  statBonus.costMultplier = 12;
  statBonus.bonusStat.Mag = 20;
  mStatBonuses.push_back(statBonus);
}