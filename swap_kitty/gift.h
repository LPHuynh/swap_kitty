#pragma once

#include "world.h"


class Gift
{
public:
  Gift();
  ~Gift();
  void init(World& world);

  struct GiftItem
  {
    uint16_t id;
    std::string name;
    int64_t price;
    bool isToy;
    bool isKitchenware;
    bool isDecoration;
    World::Stat bonusStat;
    World::Skill bonusSkill;
    int16_t health;
  };

  GiftItem randomizeGift(const std::string& seed);
  GiftItem generateGift(const std::string& name);

private:
  World mWorld;

  void loadGift();

  std::vector<GiftItem> mGifts;
};

