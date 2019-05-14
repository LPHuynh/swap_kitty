#pragma once

#include <string>
#include "siple.hpp"


class SiphashRNG
{
public:
  SiphashRNG();
  ~SiphashRNG();

  uint16_t rollDie(const std::string& seed, uint16_t dice, uint16_t face, uint16_t nonce);
  uint16_t getRandomNumber(const std::string& seed, uint16_t minNumber, uint16_t maxNumber, uint16_t nonce);
};