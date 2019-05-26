#include "cosmetic.h"


Cosmetic::Cosmetic()
{
}

Cosmetic::~Cosmetic()
{
}

void Cosmetic::init(World::Element firstElement, World::Element secondElement, const std::string& seed, World& world)
{
  naturalHairColour = generateNaturalHairColour(firstElement, secondElement);
  naturalEyeColour = generateNaturalEyeColour(firstElement);
  naturalSkinTone = generateNaturalSkinTone(secondElement);
  currentHairStyle = randomizeHairStyle(seed);
  naturalTailStyle = randomizeTailStyle(seed);
  naturalEyeStyle = randomizeEyeStyle(seed);

  currentHairColour = naturalHairColour;
  currentEyeColour = naturalEyeColour;
  currentSkinTone = naturalSkinTone;

  species = "Catgirl";
  gender = "Female";
  age = 10;
  ageCounter = 0;
  weight = 26000 + world.getRandomNumber(seed, 0, 4000);
  height = 1200 + world.getRandomNumber(seed, 0, 80);
}

std::string Cosmetic::generateNaturalHairColour(World::Element firstElement, World::Element secondElement)
{
  switch (firstElement)
  {
  case World::Element::fire:
    switch (secondElement)
    {
    case World::Element::fire: return "Flaming Red";
    case World::Element::water: return "Indigo";
    case World::Element::earth: return "Fire Brick";
    case World::Element::air: return "Cotton Floss";
    case World::Element::lightning: return "Sunglow Blonde";
    case World::Element::holy: return "Cerise Red";
    case World::Element::dark: return "Blood Red";
    }
  case World::Element::water:
    switch (secondElement)
    {
    case World::Element::fire: return "Indigo";
    case World::Element::water: return "Pacific blue";
    case World::Element::earth: return "Olive Green";
    case World::Element::air: return "Icy Blue";
    case World::Element::lightning: return "Celadon Green";
    case World::Element::holy: return "Celeste Blue";
    case World::Element::dark: return "Dark Turquoise";
    }
  case World::Element::earth:
    switch (secondElement)
    {
    case World::Element::fire: return "Fire Brick";
    case World::Element::water: return "Olive Green";
    case World::Element::earth: return "Plain Brown";
    case World::Element::air: return "Antique Blue";
    case World::Element::lightning: return "Green Gold";
    case World::Element::holy: return "Goldenrod Blonde";
    case World::Element::dark: return "Burnt Umber";
    }
  case World::Element::air:
    switch (secondElement)
    {
    case World::Element::fire: return "Cotton Floss";
    case World::Element::water: return "Icy Blue";
    case World::Element::earth: return "Antique Blue";
    case World::Element::air: return "Azure Blue";
    case World::Element::lightning: return "Spring Green";
    case World::Element::holy: return "Snow White";
    case World::Element::dark: return "Pink";
    }
  case World::Element::lightning:
    switch (secondElement)
    {
    case World::Element::fire: return "Sunglow Blonde";
    case World::Element::water: return "Celadon Green";
    case World::Element::earth: return "Green Gold";
    case World::Element::air: return "Spring Green";
    case World::Element::lightning: return "Amber Blonde";
    case World::Element::holy: return "Golden White";
    case World::Element::dark: return "Burnt Ochre";
    }
  case World::Element::holy:
    switch (secondElement)
    {
    case World::Element::fire: return "Cerise Red";
    case World::Element::water: return "Celeste Blue";
    case World::Element::earth: return "Goldenrod Blonde";
    case World::Element::air: return "Snow White";
    case World::Element::lightning: return "Golden White";
    case World::Element::holy: return "Pure White";
    case World::Element::dark: return "Grey";
    }
  case World::Element::dark:
    switch (secondElement)
    {
    case World::Element::fire: return "Blood Red";
    case World::Element::water: return "Dark Turquoise";
    case World::Element::earth: return "Burnt Umber";
    case World::Element::air: return "Pink";
    case World::Element::lightning: return "Burnt Ochre";
    case World::Element::holy: return "Grey";
    case World::Element::dark: return "Dark Violet";
    }
  default: return "Dark Violet";
  }
}

std::string Cosmetic::generateNaturalEyeColour(World::Element element)
{
  switch (element) 
  {
  case World::Element::fire: return "Red";
  case World::Element::water: return "Green";
  case World::Element::earth: return "Brown";
  case World::Element::air: return "Blue";
  case World::Element::lightning: return "Amber";
  case World::Element::holy: return "White";
  case World::Element::dark: return "Black";
  default: return "Black";
  }
}

int16_t Cosmetic::generateNaturalSkinTone(World::Element element)
{
  switch (element)
  {
  case World::Element::fire: return 7500;
  case World::Element::water: return 3500;
  case World::Element::earth: return 6500;
  case World::Element::air: return 2500;
  case World::Element::lightning: return 4500;
  case World::Element::holy: return 4500;
  case World::Element::dark: return 5500;
  default: return 5000;
  }
}

std::string Cosmetic::randomizeHairStyle(const std::string& seed)
{
  return "Fluffy";
}

std::string Cosmetic::randomizeTailStyle(const std::string& seed)
{
  return "Short";
}

std::string Cosmetic::randomizeEyeStyle(const std::string& seed)
{
  return "Large, Round,";
}

std::string Cosmetic::getSkinToneDescription(int16_t skinTone)
{
  if (skinTone < 1000)
  {
    return "Ghastly White";
  }
  else if (skinTone < 2000)
  {
    return "Snow White";
  }
  else if (skinTone < 3000)
  {
    return "Pale";
  }
  else if (skinTone < 4000)
  {
    return "Fair";
  }
  else if (skinTone < 5000)
  {
    return "Medium Toned";
  }
  else if (skinTone < 7000)
  {
    return "Toned";
  }
  else if (skinTone < 8000)
  {
    return "Tanned";
  }
  else if (skinTone < 9000)
  {
    return "Dark";
  }
  else
  {
    return "Very Dark";
  }
}