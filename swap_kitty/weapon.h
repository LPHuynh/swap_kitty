#pragma once

#include <string>
#include <vector>
#include "world.h"

class Weapon
{
public:
  Weapon();
  ~Weapon();
private:
  enum class WeaponType { sword, axe, blunt, stave, polearm };

  struct Dice
  {
    int16_t roll;
    int16_t face;
    int16_t plus;
  };
  struct Stat
  {
    int16_t Str;
    int16_t Con;
    int16_t Dex;
    int16_t Per;
    int16_t Lrn;
    int16_t Wil;
    int16_t Mag;
    int16_t Chr;
    int16_t Acc;
    int16_t Cri;
  };
  struct WeaponItem
  {
    std::string name;
    WeaponType type;
    uint16_t baseCost;
    Dice bonusDice;
    Stat bonusStat;
  };
  struct WeaponMaterial
  {
    std::string name;
    std::string artifactName;
    uint16_t costMultplier;
    Dice bonusDice;
    Stat bonusStat;
  };
  struct WeaponQuality
  {
    std::string name;
    uint16_t costMultplier;
    Dice bonusDice;
    Stat bonusStat;
  };
  struct WeaponAbility
  {
    std::string name;
    uint16_t costMultplier;
    Dice bonusDice;
    Stat bonusStat;
    World::Attribute attribute;
  };
  struct WeaponStatBonus
  {
    std::string name;
    uint16_t costMultplier;
    Dice bonusDice;
    Stat bonusStat;
  };

  void loadWeapon();
  void loadMaterial();
  void loadQuality();
  void loadAbility();
  void loadStatBonus();

  std::vector<WeaponItem> mWeapons;
  std::vector<WeaponMaterial> mMaterials;
  std::vector<WeaponQuality> mQualities;
  std::vector<WeaponAbility> mAbilities;
  std::vector<WeaponStatBonus> mStatBonuses;
};

