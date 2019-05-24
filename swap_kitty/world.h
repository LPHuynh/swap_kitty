#pragma once

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <mutex>
#include <queue>
#include <sstream>
#include <string>
#include <thread>
#include <vector>


class World
{
public:
  World();
  ~World();

  enum class Element { normal, fire, water, earth, air, lightning, holy, dark, machine, poison, chaos };
  enum class ItemType { weapon, dress, food, potion, book, toy };
  enum class ItemAction { buy, sell, discard };

  struct Dice
  {
    int16_t roll;
    int16_t face;
    int16_t plus;
  };
  struct Stat
  {
    int16_t str;
    int16_t con;
    int16_t dex;
    int16_t per;
    int16_t lrn;
    int16_t wil;
    int16_t mag;
    int16_t chr;
    int16_t acc;
    int16_t cri;
  };
  struct Skill
  {
    int16_t literacy;
    int16_t cooking;
    int16_t cleaning;
    int16_t service;
    int16_t music;
    int16_t art;
    int16_t tailor;
    int16_t stoneWorking;
    int16_t woodWorking;
    int16_t metalworking;
    int16_t farming;
    int16_t fishing;
    int16_t crafting;
    int16_t sword;
    int16_t axe;
    int16_t bludgeon;
    int16_t stave;
    int16_t polearm;
    int16_t evasion;
    int16_t fire;
    int16_t water;
    int16_t earth;
    int16_t air;
    int16_t lightning;
    int16_t holy;
    int16_t dark;
    int16_t machine;
    int16_t poison;
    int16_t choas;
  };

  int16_t calculateElementAttackBonus(Element attackerElement, const std::vector<Element>& defenderElement);
  Dice addDice(const std::vector<Dice>& die);
  Stat addStat(const std::vector<Stat>& stats);
  Skill addSkill(const std::vector<Skill>& skills);
  uint16_t rollDie(const std::string& seed, uint16_t dice, uint16_t face);
  uint16_t getRandomNumber(std::string seed, uint16_t minNumber, uint16_t maxNumber);
  uint16_t getNoncelessRandomNumber(std::string seed, uint16_t minNumber, uint16_t maxNumber);
  uint16_t generateID();
  void freeID(uint16_t id);
  std::string generateNonce();

  uint16_t currentRulesetVersion;
  uint64_t startingHeight;
  uint64_t currentWorldHeight;
  uint16_t localTimeOffset;
private:
  uint32_t generateHash(const std::string& seed);

  uint16_t mIDCounter;
  uint16_t mNonceCounter;
  std::queue<uint16_t> mRecoveredID;
};