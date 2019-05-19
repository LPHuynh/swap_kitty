#pragma once

#include "world.h"


class Cosmetic
{
public:
  Cosmetic();
  ~Cosmetic();
  void init(World::Element firstElement, World::Element secondElement, const std::string& seed);

  std::string generateNaturalHairColour(World::Element firstElement, World::Element secondElement);
  std::string generateNaturalEyeColour(World::Element element);
  int16_t generateNaturalSkinTone(World::Element element);

  std::string randomizeHairStyle(const std::string& seed);
  std::string randomizeTailStyle(const std::string& seed);
  std::string randomizeEyeStyle(const std::string& seed);

  std::string getSkinToneDescription(int16_t skinTone);

  std::string species;
  std::string gender;
  int16_t age;
  int16_t weight;
  int16_t height;

  std::string naturalHairColour;
  std::string currentHairColour;
  std::string currentHairStyle;
  std::string currentTailStyle;
  std::string naturalEyeColour;
  std::string currentEyeColour;
  std::string naturalEyeStyle;
  int16_t naturalSkinTone;
  int16_t currentSkinTone;
};

