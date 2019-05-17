#include "siphashrng.h"


SiphashRNG::SiphashRNG()
{
}

SiphashRNG::~SiphashRNG()
{
}

int16_t SiphashRNG::rollDie(const std::string& seed, int16_t numOfDie, int16_t numOfFaces)
{
  uint16_t result = 0;

  for (int i = 0; i < numOfDie; i++)
  {
    result += getRandomNumber(seed + "dice__" + std::to_string(i), 1, numOfFaces);
  }

  return result;
}

int16_t SiphashRNG::getRandomNumber(const std::string& seed, int16_t minNumber, int16_t maxNumber)
{
  return (siphash24(&seed, seed.length()) % (maxNumber - minNumber + 1)) + minNumber;
}