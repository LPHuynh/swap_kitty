#pragma once

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>
#include <vector>


class World
{
public:
  World();
  ~World();

  enum class Element { normal, fire, water, earth, air, lightning, holy, dark, machine, poison, chaos };

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

  static int16_t calculateElementAttackBonus(Element attackerElement, const std::vector<Element>& defenderElement);
  static Dice addDice(const std::vector<Dice>& die);
  static Stat addStat(const std::vector<Stat>& stats);
  static int16_t rollDie(const std::string& seed, int16_t dice, int16_t face);
  static int16_t getRandomNumber(const std::string& seed, int16_t minNumber, int16_t maxNumber);

  uint16_t currentRulesetVersion;
  uint64_t startingBlock;
  uint64_t currentScanHeight;
};