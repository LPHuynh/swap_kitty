#include "eyes.h"


Eyes::Eyes()
{
}

Eyes::~Eyes()
{
}

std::string Eyes::getNaturalEyeColour(World::Attribute primaryAttribute)
{
  if (primaryAttribute == World::Attribute::fire)
  { return "Red";
  }
  else if (primaryAttribute == World::Attribute::water)
  { return "Green";
  }
  else if (primaryAttribute == World::Attribute::earth)
  { return "Brown";
  }
  else if (primaryAttribute == World::Attribute::air)
  { return "Blue";
  }
  else if (primaryAttribute == World::Attribute::lightning)
  { return "Amber";
  }
  else if (primaryAttribute == World::Attribute::holy)
  { return "White";
  }
  else //Dark and fallback for any unexecpted attribute
  { return "Black";
  }
}