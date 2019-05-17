#pragma once

#include <string>
#include <vector>
#include "world.h"
#include "siphashrng.h"


class Weapon
{
public:
  Weapon();
  ~Weapon();

  enum WeaponType { sword, axe, bludgeon, stave, polearm };

  struct WeaponItem
  {
    std::string name;
    WeaponType type;
    int64_t price;
    World::Dice baseDice;
    World::Stat bonusStat;
    World::Dice abilityDice;
    World::Element attribute;
  };

  WeaponItem randomizeWeapon(std::string seed, int64_t maxCost);

private:
  struct WeaponBase
  {
    std::string name;
    WeaponType type;
    int16_t baseCost;
    World::Dice bonusDice;
    World::Stat bonusStat;
  };
  struct WeaponMaterial
  {
    std::string name;
    std::string artifactName;
    int16_t costMultplier;
    World::Dice bonusDice;
    World::Stat bonusStat;
  };
  struct WeaponQuality
  {
    std::string name;
    int16_t costMultplier;
    World::Dice bonusDice;
    World::Stat bonusStat;
  };
  struct WeaponAbility
  {
    std::string name;
    int16_t costMultplier;
    World::Dice bonusDice;
    World::Stat bonusStat;
    World::Element attribute;
  };
  struct WeaponStatBonus
  {
    std::string name;
    int16_t costMultplier;
    World::Dice bonusDice;
    World::Stat bonusStat;
  };

  void loadWeapon();
  void loadMaterial();
  void loadQuality();
  void loadAbility();
  void loadStatBonus();

  std::vector<WeaponBase> mSword;
  std::vector<WeaponBase> mAxe;
  std::vector<WeaponBase> mBludgeon;
  std::vector<WeaponBase> mStave;
  std::vector<WeaponBase> mPolearm;
  std::vector<WeaponMaterial> mMaterials;
  std::vector<WeaponQuality> mQualities;
  std::vector<WeaponAbility> mAbilities;
  std::vector<WeaponStatBonus> mStatBonuses;

  SiphashRNG mSiphashRNG;
};

