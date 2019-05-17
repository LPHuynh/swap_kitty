#pragma once

#include <string>
#include "world.h"
#include "siphashrng.h"


class Cosmetic
{
public:
  Cosmetic();
  ~Cosmetic();
  void init(World::Element firstElement, World::Element secondElement, std::string seed, World& world);

  std::string generateNaturalHairColour(World::Element firstElement, World::Element secondElement);
  std::string generateNaturalEyeColour(World::Element element);
  int16_t generateNaturalSkinTone(World::Element element);

  std::string randomizeHairStyle(std::string seed);
  std::string randomizeTailStyle(std::string seed);
  std::string randomizeEyeStyle(std::string seed);

  std::string getSkinToneDescription(int16_t skinTone);

  std::string species;
  std::string gender;
  std::string naturalHairColour;
  std::string currentHairColour;
  std::string currentHairStyle;
  std::string currentTailStyle;
  std::string naturalEyeColour;
  std::string currentEyeColour;
  std::string naturalEyeStyle;
  int16_t naturalSkinTone;
  int16_t currentSkinTone;

private:
  World mWorld;
  SiphashRNG mSiphashRNG;
};

