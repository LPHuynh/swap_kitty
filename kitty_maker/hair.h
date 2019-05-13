#pragma once

#include <string>
#include "world.h"


class Hair
{
public:
  Hair();
  ~Hair();

  std::string getNaturalHairColour(World::Attribute primaryAttribute, World::Attribute secondaryAttribute);
};