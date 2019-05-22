#include "gift.h"


Gift::Gift()
{
  loadGift();
}

Gift::~Gift()
{
}

void Gift::init(World& world)
{
  mWorld = world;
}

Gift::GiftItem Gift::randomizeGift(const std::string & seed)
{
  Gift::GiftItem gift = mGifts.at(mWorld.getRandomNumber(seed, 0, 9));
  gift.id = mWorld.generateID();
  return gift;
}

Gift::GiftItem Gift::generateGift(const std::string & name)
{
  Gift::GiftItem gift;

  for (auto& element : mGifts)
  {
    if (element.name == name)
    {
      gift = element;
      gift.id = mWorld.generateID();
      return gift;
    }
  }

  gift = mGifts.at(0);
  gift.id = mWorld.generateID();
  return gift;
}

void Gift::loadGift()
{
  GiftItem prototype;
  prototype.id = 0;
  prototype.name = "";
  prototype.price = 0;
  prototype.isToy = false;
  prototype.isKitchenware = false;
  prototype.isDecoration = false;
  prototype.bonusStat = World::Stat{ 0,0,0,0,0,0,0,0,0,0 };
  prototype.bonusSkill = World::Skill{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
  prototype.health = 100;

  GiftItem gift;

  //Ruleset v1: gift slot 0-9
  gift = prototype;
  gift.name = "Cattail";
  gift.price = 225;
  prototype.isToy = true;
  gift.bonusStat.Dex = 5;
  gift.health = 15;
  mGifts.push_back(gift);

  gift = prototype;
  gift.name = "Ball";
  gift.price = 3000;
  prototype.isToy = true;
  gift.bonusStat.Dex = 10;
  gift.bonusSkill.evasion = 10;
  gift.health = 50;
  mGifts.push_back(gift);

  gift = prototype;
  gift.name = "Wooden Blocks";
  gift.price = 9400;
  prototype.isToy = true;
  gift.bonusStat.Lrn = 10;
  gift.bonusSkill.woodWorking = 30;
  gift.bonusSkill.crafting = 10;
  gift.health = 70;
  mGifts.push_back(gift);

  gift = prototype;
  gift.name = "Toy Train";
  gift.price = 5250;
  prototype.isToy = true;
  gift.bonusStat.Dex = 15;
  gift.bonusStat.Per = 10;
  gift.health = 70;
  mGifts.push_back(gift);

  gift = prototype;
  gift.name = "RC Car";
  gift.price = 9250;
  prototype.isToy = true;
  gift.bonusStat.Dex = 15;
  gift.bonusStat.Per = 30;
  gift.bonusSkill.evasion = 10;
  gift.health = 50;
  mGifts.push_back(gift);

  gift = prototype;
  gift.name = "Crayon";
  gift.price = 5600;
  prototype.isToy = true;
  gift.bonusSkill.art = 60;
  gift.health = 20;
  mGifts.push_back(gift);

  gift = prototype;
  gift.name = "Recorder";
  gift.price = 8200;
  prototype.isToy = true;
  gift.bonusSkill.music = 30;
  gift.health = 80;
  mGifts.push_back(gift);

  gift = prototype;
  gift.name = "Weight Training Set";
  gift.price = 32400;
  prototype.isToy = true;
  gift.bonusStat.Str = 60;
  gift.health = 180;
  mGifts.push_back(gift);

  gift = prototype;
  gift.name = "Wack-a-Mole";
  gift.price = 17200;
  prototype.isToy = true;
  gift.bonusStat.Str = 10;
  gift.bonusSkill.bludgeon = 30;
  gift.health = 100;
  mGifts.push_back(gift);

  gift = prototype;
  gift.name = "Magnetic Fishing Game";
  gift.price = 15400;
  prototype.isToy = true;
  gift.bonusSkill.fishing = 40;
  gift.health = 120;
  mGifts.push_back(gift);
}
