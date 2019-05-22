#include "toy.h"


Toy::Toy()
{
  loadToy();
}

Toy::~Toy()
{
}

void Toy::init(World& world)
{
  mWorld = world;
}

Toy::ToyItem Toy::randomizeToy(const std::string & seed)
{
  Toy::ToyItem toy = mToys.at(mWorld.getRandomNumber(seed, 0, 9));
  toy.id = mWorld.generateID();
  return toy;
}

Toy::ToyItem Toy::generateToy(const std::string & name)
{
  Toy::ToyItem toy;

  for (auto& element : mToys)
  {
    if (element.name == name)
    {
      toy = element;
      toy.id = mWorld.generateID();
      return toy;
    }
  }

  toy = mToys.at(0);
  toy.id = mWorld.generateID();
  return toy;
}

void Toy::loadToy()
{
  ToyItem prototype;
  prototype.id = 0;
  prototype.name = "";
  prototype.price = 0;
  prototype.bonusStat = World::Stat{ 0,0,0,0,0,0,0,0,0,0 };
  prototype.bonusSkill = World::Skill{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
  prototype.health = 100;

  ToyItem toy;

  //Ruleset v1: toy slot 0-9
  toy = prototype;
  toy.name = "Cattail";
  toy.price = 225;
  toy.bonusStat.Dex = 5;
  toy.health = 15;
  mToys.push_back(toy);

  toy = prototype;
  toy.name = "Ball";
  toy.price = 3000;
  toy.bonusStat.Dex = 10;
  toy.bonusSkill.evasion = 10;
  toy.health = 50;
  mToys.push_back(toy);

  toy = prototype;
  toy.name = "Wooden Blocks";
  toy.price = 9400;
  toy.bonusStat.Lrn = 10;
  toy.bonusSkill.woodWorking = 30;
  toy.bonusSkill.crafting = 10;
  toy.health = 70;
  mToys.push_back(toy);

  toy = prototype;
  toy.name = "Toy Train";
  toy.price = 5250;
  toy.bonusStat.Dex = 15;
  toy.bonusStat.Per = 10;
  toy.health = 70;
  mToys.push_back(toy);

  toy = prototype;
  toy.name = "RC Car";
  toy.price = 9250;
  toy.bonusStat.Dex = 15;
  toy.bonusStat.Per = 30;
  toy.bonusSkill.evasion = 10;
  toy.health = 50;
  mToys.push_back(toy);

  toy = prototype;
  toy.name = "Crayon";
  toy.price = 5600;
  toy.bonusSkill.art = 60;
  toy.health = 20;
  mToys.push_back(toy);

  toy = prototype;
  toy.name = "Recorder";
  toy.price = 8200;
  toy.bonusSkill.music = 30;
  toy.health = 80;
  mToys.push_back(toy);

  toy = prototype;
  toy.name = "Weight Training Set";
  toy.price = 32400;
  toy.bonusStat.Str = 60;
  toy.health = 180;
  mToys.push_back(toy);

  toy = prototype;
  toy.name = "Wack-a-Mole";
  toy.price = 17200;
  toy.bonusStat.Str = 10;
  toy.bonusSkill.bludgeon = 30;
  toy.health = 100;
  mToys.push_back(toy);

  toy = prototype;
  toy.name = "Magnetic Fishing Game";
  toy.price = 15400;
  toy.bonusSkill.fishing = 40;
  toy.health = 120;
  mToys.push_back(toy);
}
