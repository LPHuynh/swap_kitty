#include "hair.h"


Hair::Hair()
{
}

Hair::~Hair()
{
}

std::string Hair::getNaturalHairColour(World::Attribute primaryAttribute, World::Attribute secondaryAttribute)
{
  if (primaryAttribute == secondaryAttribute)
  {
    if (primaryAttribute == World::Attribute::fire)
    { return "Flaming Red";
    }
    else if (primaryAttribute == World::Attribute::water)
    { return "Pacific blue";
    }
    else if (primaryAttribute == World::Attribute::earth)
    { return "Plain Brown";
    }
    else if (primaryAttribute == World::Attribute::air)
    { return "Azure Blue";
    }
    else if (primaryAttribute == World::Attribute::lightning)
    { return "Amber Blonde";
    }
    else if (primaryAttribute == World::Attribute::holy)
    { return "Snow White";
    }
    else if (primaryAttribute == World::Attribute::dark)
    { return "Dark Violet";
    }
  }

  if (primaryAttribute == World::Attribute::fire || secondaryAttribute == World::Attribute::fire)
  {
    if (primaryAttribute == World::Attribute::water || secondaryAttribute == World::Attribute::water)
    { return "Indigo";
    }
    if (primaryAttribute == World::Attribute::earth || secondaryAttribute == World::Attribute::earth)
    { return "Fire Brick";
    }
    if (primaryAttribute == World::Attribute::air || secondaryAttribute == World::Attribute::air)
    { return "Cotton Floss";
    }
    if (primaryAttribute == World::Attribute::lightning || secondaryAttribute == World::Attribute::lightning)
    { return "Sunglow Blonde";
    }
    if (primaryAttribute == World::Attribute::holy || secondaryAttribute == World::Attribute::holy)
    { return "Cerise Red";
    }
    if (primaryAttribute == World::Attribute::dark || secondaryAttribute == World::Attribute::dark)
    { return "Blood Red";
    }
  }

  if (primaryAttribute == World::Attribute::water || secondaryAttribute == World::Attribute::water)
  {
    if (primaryAttribute == World::Attribute::earth || secondaryAttribute == World::Attribute::earth)
    { return "Olive Green";
    }
    if (primaryAttribute == World::Attribute::air || secondaryAttribute == World::Attribute::air)
    { return "Icy Blue";
    }
    if (primaryAttribute == World::Attribute::lightning || secondaryAttribute == World::Attribute::lightning)
    { return "Celadon Green";
    }
    if (primaryAttribute == World::Attribute::holy || secondaryAttribute == World::Attribute::holy)
    { return "Celeste Blue";
    }
    if (primaryAttribute == World::Attribute::dark || secondaryAttribute == World::Attribute::dark)
    { return "Dark Turquoise";
    }
  }

  if (primaryAttribute == World::Attribute::earth || secondaryAttribute == World::Attribute::earth)
  {
    if (primaryAttribute == World::Attribute::air || secondaryAttribute == World::Attribute::air)
    { return "Antique Blue";
    }
    if (primaryAttribute == World::Attribute::lightning || secondaryAttribute == World::Attribute::lightning)
    { return "Green Gold";
    }
    if (primaryAttribute == World::Attribute::holy || secondaryAttribute == World::Attribute::holy)
    { return "Goldenrod Blonde";
    }
    if (primaryAttribute == World::Attribute::dark || secondaryAttribute == World::Attribute::dark)
    { return "Burnt Umber";
    }
  }

  if (primaryAttribute == World::Attribute::air || secondaryAttribute == World::Attribute::air)
  {
    if (primaryAttribute == World::Attribute::lightning || secondaryAttribute == World::Attribute::lightning)
    { return "Spring Green";
    }
    if (primaryAttribute == World::Attribute::holy || secondaryAttribute == World::Attribute::holy)
    { return "Golden White";
    }
    if (primaryAttribute == World::Attribute::dark || secondaryAttribute == World::Attribute::dark)
    { return "Pink";
    }
  }

  if (primaryAttribute == World::Attribute::lightning || secondaryAttribute == World::Attribute::lightning)
  {
    if (primaryAttribute == World::Attribute::holy || secondaryAttribute == World::Attribute::holy)
    { return "Golden White";
    }
    if (primaryAttribute == World::Attribute::dark || secondaryAttribute == World::Attribute::dark)
    { return "Burnt Ochre";
    }
  }

  return "Grey"; //Holy+Dark and fallback for any unexecpted combination
}