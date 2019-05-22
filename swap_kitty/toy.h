#pragma once

#include "world.h"


class Toy
{
public:
  Toy();
  ~Toy();
  void init(World& world);

  struct ToyItem
  {
    uint16_t id;
    std::string name;
    int64_t price;
    World::Stat bonusStat;
    World::Skill bonusSkill;
    int16_t health;
  };

  ToyItem randomizeToy(const std::string& seed);
  ToyItem generateToy(const std::string& name);

private:
  World mWorld;

  void loadToy();

  std::vector<ToyItem> mToys;
};

