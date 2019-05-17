#pragma once

#include <string>
#include "siple.hpp"


class SiphashRNG
{
public:
  SiphashRNG();
  ~SiphashRNG();

  int16_t rollDie(const std::string& seed, int16_t dice, int16_t face);
  int16_t getRandomNumber(const std::string& seed, int16_t minNumber, int16_t maxNumber);
};