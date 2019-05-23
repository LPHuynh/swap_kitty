#pragma once

#include "world.h"

class Dress
{
public:
  Dress(World& world);
  ~Dress();

  struct DressItem
  {
    uint16_t id;
    std::string baseName;
    std::string name;
    int64_t price;
    bool isWinterEquip;
    bool isSummerEquip;
    World::Stat bonusStat;
    World::Skill bonusSkill;
    World::Stat dailyStatGain;
    World::Skill dailySkillGain;
  };

  DressItem randomizeDress(const std::string& seed);
  DressItem generateDress(const std::string& name);

private:
  World& mWorld;

  void loadDress();

  std::vector<DressItem> mDresses;
};

