#include "siphashrng.h"


SiphashRNG::SiphashRNG()
{
}

SiphashRNG::~SiphashRNG()
{
}

uint16_t SiphashRNG::rollDie(const std::string& seed, uint16_t numOfDie, uint16_t numOfFaces, uint16_t nonce)
{
  std::string hashString;
  uint16_t result = 0;

  for (int i = 0; i < numOfDie; i++)
  {
    hashString = "dice__" + std::to_string(i) + seed + std::to_string(nonce);
    result += getRandomNumber(hashString, 1, numOfFaces, nonce);
  }

  return result;
}

uint16_t SiphashRNG::getRandomNumber(const std::string& seed, uint16_t minNumber, uint16_t maxNumber, uint16_t nonce)
{
  std::string hashString;
  hashString = seed + std::to_string(nonce);

  return (siphash24(&hashString, hashString.length()) % (maxNumber - minNumber + 1)) + minNumber;
}