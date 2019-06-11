#pragma once

#include "world.h"


class Cosmetic
{
public:
  Cosmetic();
  ~Cosmetic();
  void init(World::Element firstElement, World::Element secondElement, const std::string& seed, World& world);

  enum class Species {catgirl};
  enum class Gender {female, male};

  std::string generateNaturalHairColour(World::Element firstElement, World::Element secondElement);
  std::string generateNaturalEyeColour(World::Element element);
  int16_t generateNaturalSkinTone(World::Element element);

  std::string randomizeHairStyle(const std::string& seed);
  std::string randomizeTailStyle(const std::string& seed);
  std::string randomizeEyeStyle(const std::string& seed);

  std::string getSpecies();
  std::string getGender();
  std::string getSkinToneDescription(int16_t skinTone);

  Species species;
  Gender gender;
  uint16_t age;
  uint16_t ageCounter;
  uint16_t weight;    //weight in g
  uint16_t height;    //height in mm
  uint16_t milkDrinkingCounter;

  std::string naturalHairColour;
  std::string currentHairColour;
  std::string currentHairStyle;
  std::string naturalTailStyle;
  std::string naturalEyeColour;
  std::string currentEyeColour;
  std::string naturalEyeStyle;
  int16_t naturalSkinTone;
  int16_t currentSkinTone;
};

