#pragma once

#include <string>
#include "world.h"


class Eyes
{
public:
  Eyes();
  ~Eyes();

  std::string getNaturalEyeColour(World::Attribute primaryAttribute);
};