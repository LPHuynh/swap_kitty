#include "food.h"


Food::Food()
{
  loadRawFood();
}

Food::~Food()
{
}

void Food::init(World& world)
{
  mWorld = world;
}

Food::FoodItem Food::randomizeRawFood(const std::string& seed, Food::FoodType foodType)
{  
  FoodItem food;

  switch (foodType)
  {
  case Food::FoodType::fruit: 
    food = mRawFruits.at(mWorld.getRandomNumber(seed, 0, 7));
    break;
  case Food::FoodType::vegatable: 
    food = mRawVegatables.at(mWorld.getRandomNumber(seed, 0, 4));
    break;
  case Food::FoodType::fish: 
    food = mRawFish.at(mWorld.getRandomNumber(seed, 0, 7));
    break;
  case Food::FoodType::corpse:
    food = mRawCorpse.at(mWorld.getRandomNumber(seed, 3, 5));
    break;
  case Food::FoodType::flour: 
    food = mRawFlour;
    break;
  case Food::FoodType::junk:
    food = mRawJunk.at(mWorld.getRandomNumber(seed, 0, 5));
    break;
  case Food::FoodType::vermin:
    food = mRawCorpse.at(mWorld.getRandomNumber(seed, 0, 2));
    break;
  }

  food.id = mWorld.generateID();
  return food;
}

void Food::randomizeCookedFood(const std::string& seed, FoodItem& cookableFood)
{
  if (cookableFood.dishLevel > 0 || cookableFood.type == FoodType::junk)
  {
    return;
  }

  cookableFood.dishLevel = mWorld.getRandomNumber(seed, 1, 5);

  if (cookableFood.type == FoodType::fruit)
  {
    if (cookableFood.dishLevel == 1)
    {
      cookableFood.nameCooked = cookableFood.nameRaw + " Jelly";
      cookableFood.price *= 2;
      cookableFood.nutrient *= 1;
      cookableFood.quench += 10;
      cookableFood.bonusStat.Mag += 10;
    }
    else if (cookableFood.dishLevel == 2)
    {
      cookableFood.nameCooked = cookableFood.nameRaw + " Pudding";
      cookableFood.price *= 3;
      cookableFood.nutrient *= 2;
      cookableFood.bonusStat.Chr += 10;
      cookableFood.bonusStat.Mag += 10;
    }
    else if (cookableFood.dishLevel == 3)
    {
      cookableFood.nameCooked = cookableFood.nameRaw + " Pie";
      cookableFood.price *= 4;
      cookableFood.nutrient *= 2;
      cookableFood.stamina += 25;
      cookableFood.bonusStat.Chr += 10;
      cookableFood.bonusStat.Mag += 20;
    }
    else if (cookableFood.dishLevel == 4)
    {
      cookableFood.nameCooked = cookableFood.nameRaw + " Sherbert";
      cookableFood.price *= 5;
      cookableFood.nutrient *= 3;
      cookableFood.quench += 20;
      cookableFood.stamina += 50;
      cookableFood.bonusStat.Chr += 10;
      cookableFood.bonusStat.Mag += 20;
    }
    else if (cookableFood.dishLevel == 5)
    {
      cookableFood.nameCooked = cookableFood.nameRaw + " Ice Cream";
      cookableFood.price *= 6;
      cookableFood.nutrient *= 3;
      cookableFood.quench += 20;
      cookableFood.stamina += 100;
      cookableFood.bonusStat.Chr += 20;
      cookableFood.bonusStat.Mag += 30;
    }
  }
  else if (cookableFood.type == FoodType::vegatable)
  {
    if (cookableFood.dishLevel == 1)
    {
      cookableFood.nameCooked = cookableFood.nameRaw + " Salad";
      cookableFood.price *= 2;
      cookableFood.nutrient *= 1;
      cookableFood.bonusStat.Lrn += 10;
    }
    else if (cookableFood.dishLevel == 2)
    {
      cookableFood.nameCooked = cookableFood.nameRaw + " Roll";
      cookableFood.price *= 3;
      cookableFood.nutrient *= 2;
      cookableFood.bonusStat.Wil += 10;
      cookableFood.bonusStat.Lrn += 10;
    }
    else if (cookableFood.dishLevel == 3)
    {
      cookableFood.nameCooked = "Baked " + cookableFood.nameRaw;
      cookableFood.price *= 4;
      cookableFood.nutrient *= 2;
      cookableFood.stamina += 25;
      cookableFood.bonusStat.Wil += 10;
      cookableFood.bonusStat.Lrn += 20;
    }
    else if (cookableFood.dishLevel == 4)
    {
      cookableFood.nameCooked = "Creme of " + cookableFood.nameRaw;
      cookableFood.price *= 5;
      cookableFood.nutrient *= 3;
      cookableFood.quench += 30;
      cookableFood.stamina += 59;
      cookableFood.bonusStat.Wil += 20;
      cookableFood.bonusStat.Lrn += 20;
    }
    else if (cookableFood.dishLevel == 5)
    {
      cookableFood.nameCooked = cookableFood.nameRaw + " Ratatouille";
      cookableFood.price *= 6;
      cookableFood.nutrient *= 3;
      cookableFood.stamina += 100;
      cookableFood.bonusStat.Wil += 20;
      cookableFood.bonusStat.Lrn += 30;
    }
  }
  else if (cookableFood.type == FoodType::fish)
  {
    if (cookableFood.dishLevel == 1)
    {
      cookableFood.nameCooked = "Fried " + cookableFood.nameRaw;
      cookableFood.price *= 2;
      cookableFood.nutrient *= 1;
      cookableFood.bonusStat.Dex += 10;
    }
    else if (cookableFood.dishLevel == 2)
    {
      cookableFood.nameCooked = cookableFood.nameRaw + " Soup";
      cookableFood.price *= 3;
      cookableFood.nutrient *= 2;
      cookableFood.quench += 30;
      cookableFood.bonusStat.Per += 1;
      cookableFood.bonusStat.Dex += 1;
    }
    else if (cookableFood.dishLevel == 3)
    {
      cookableFood.nameCooked = "Grilled " + cookableFood.nameRaw;
      cookableFood.price *= 4;
      cookableFood.nutrient *= 2;
      cookableFood.stamina += 25;
      cookableFood.bonusStat.Per += 10;
      cookableFood.bonusStat.Dex += 20;
    }
    else if (cookableFood.dishLevel == 4)
    {
      cookableFood.nameCooked = cookableFood.nameRaw + " Tempura";
      cookableFood.price *= 5;
      cookableFood.nutrient *= 3;
      cookableFood.stamina += 50;
      cookableFood.bonusStat.Per += 20;
      cookableFood.bonusStat.Dex += 20;
    }
    else if (cookableFood.dishLevel == 5)
    {
      cookableFood.nameCooked = cookableFood.nameRaw + " Sushi";
      cookableFood.price *= 6;
      cookableFood.nutrient *= 3;
      cookableFood.stamina += 100;
      cookableFood.bonusStat.Per += 20;
      cookableFood.bonusStat.Dex += 30;
    }

  }
  else if (cookableFood.type == FoodType::corpse)
  {
    if (cookableFood.dishLevel == 1)
    {
      cookableFood.nameCooked = "Roasted  " + cookableFood.nameRaw;
      cookableFood.price *= 2;
      cookableFood.nutrient *= 1;
      cookableFood.bonusStat.Str += 10;
    }
    else if (cookableFood.dishLevel == 2)
    {
      cookableFood.nameCooked = cookableFood.nameRaw + " Stew";
      cookableFood.price *= 3;
      cookableFood.nutrient *= 2;
      cookableFood.quench += 30;
      cookableFood.bonusStat.Con += 1;
      cookableFood.bonusStat.Str += 1;
    }
    else if (cookableFood.dishLevel == 3)
    {
      cookableFood.nameCooked = cookableFood.nameRaw + " Cutlet";
      cookableFood.price *= 4;
      cookableFood.nutrient *= 2;
      cookableFood.stamina += 25;
      cookableFood.bonusStat.Con += 10;
      cookableFood.bonusStat.Str += 20;
    }
    else if (cookableFood.dishLevel == 4)
    {
      cookableFood.nameCooked = cookableFood.nameRaw + " Burger";
      cookableFood.price *= 5;
      cookableFood.nutrient *= 3;
      cookableFood.stamina += 50;
      cookableFood.bonusStat.Con += 20;
      cookableFood.bonusStat.Str += 20;
    }
    else if (cookableFood.dishLevel == 5)
    {
      cookableFood.nameCooked = cookableFood.nameRaw + " Steak";
      cookableFood.price *= 6;
      cookableFood.nutrient *= 3;
      cookableFood.stamina += 100;
      cookableFood.bonusStat.Con += 20;
      cookableFood.bonusStat.Str += 30;
    }
  }
  else if (cookableFood.type == FoodType::flour)
  {
    if (cookableFood.dishLevel == 1)
    {
      cookableFood.nameCooked = "Flat Bread" + cookableFood.nameRaw;
      cookableFood.price *= 2;
      cookableFood.nutrient *= 2;
      cookableFood.bonusStat.Str += 5;
      cookableFood.bonusStat.Con += 5;
      cookableFood.bonusStat.Dex += 5;
      cookableFood.bonusStat.Per += 5;
      cookableFood.bonusStat.Lrn += 5;
      cookableFood.bonusStat.Wil += 5;
      cookableFood.bonusStat.Mag += 5;
    }
    else if (cookableFood.dishLevel == 2)
    {
      cookableFood.nameCooked = "Sandwich";
      cookableFood.price *= 3;
      cookableFood.nutrient *= 3;
      cookableFood.quench += 30;
      cookableFood.bonusStat.Str += 10;
      cookableFood.bonusStat.Con += 10;
      cookableFood.bonusStat.Dex += 10;
      cookableFood.bonusStat.Per += 10;
      cookableFood.bonusStat.Lrn += 10;
      cookableFood.bonusStat.Wil += 10;
      cookableFood.bonusStat.Mag += 10;
    }
    else if (cookableFood.dishLevel == 3)
    {
      cookableFood.nameCooked = "Croissant";
      cookableFood.price *= 4;
      cookableFood.nutrient *= 4;
      cookableFood.stamina += 25;
      cookableFood.bonusStat.Str += 12;
      cookableFood.bonusStat.Con += 12;
      cookableFood.bonusStat.Dex += 12;
      cookableFood.bonusStat.Per += 12;
      cookableFood.bonusStat.Lrn += 12;
      cookableFood.bonusStat.Wil += 12;
      cookableFood.bonusStat.Mag += 12;
    }
    else if (cookableFood.dishLevel == 4)
    {
      cookableFood.nameCooked = "Cinnamon Roll";
      cookableFood.price *= 5;
      cookableFood.nutrient *= 5;
      cookableFood.stamina += 50;
      cookableFood.bonusStat.Str += 15;
      cookableFood.bonusStat.Con += 15;
      cookableFood.bonusStat.Dex += 15;
      cookableFood.bonusStat.Per += 15;
      cookableFood.bonusStat.Lrn += 15;
      cookableFood.bonusStat.Wil += 15;
      cookableFood.bonusStat.Mag += 15;
    }
    else if (cookableFood.dishLevel == 5)
    {
      cookableFood.nameCooked = "Melonpan";
      cookableFood.price *= 6;
      cookableFood.nutrient *= 6;
      cookableFood.stamina += 100;
      cookableFood.bonusStat.Str += 20;
      cookableFood.bonusStat.Con += 20;
      cookableFood.bonusStat.Dex += 20;
      cookableFood.bonusStat.Per += 20;
      cookableFood.bonusStat.Lrn += 20;
      cookableFood.bonusStat.Wil += 20;
      cookableFood.bonusStat.Mag += 20;
    }
  }
}

Food::FoodItem Food::generateRawFood(const std::string & name)
{
  Food::FoodItem food;

  for (auto& element : mRawFruits)
  {
    if (element.nameRaw == name)
    {
      food = element;
      food.id = mWorld.generateID();
      return food;
    }
  }
  for (auto& element : mRawVegatables)
  {
    if (element.nameRaw == name)
    {
      food = element;
      food.id = mWorld.generateID();
      return food;
    }
  }
  for (auto& element : mRawFish)
  {
    if (element.nameRaw == name)
    {
      food = element;
      food.id = mWorld.generateID();
      return food;
    }
  }
  for (auto& element : mRawCorpse)
  {
    if (element.nameRaw == name)
    {
      food = element;
      food.id = mWorld.generateID();
      return food;
    }
  }
  if (mRawFlour.nameRaw == name)
  {
    food = mRawFlour;
    food.id = mWorld.generateID();
    return food;
  }
  for (auto& element : mRawJunk)
  {
    if (element.nameRaw == name)
    {
      food = element;
      food.id = mWorld.generateID();
      return food;
    }
  }

  food = mRawFruits.at(0);
  food.id = mWorld.generateID();
  return food;
}

void Food::loadRawFood()
{
  FoodItem prototype;

  prototype.id = 0;
  prototype.nameRaw = "";
  prototype.nameCooked = "";
  prototype.type = FoodType::fruit;
  prototype.dishLevel = 0;
  prototype.price = 100;
  prototype.nutrient = 100;
  prototype.quench = 100;
  prototype.stamina = 0;
  prototype.bonusStat = World::Stat{ 0,0,0,0,0,0,0,0,0,0 };

  FoodItem rawFood;

  //Ruleset v1: fruit slot 0-7
  rawFood = prototype;
  rawFood.nameRaw = "Apple";
  rawFood.nameCooked = rawFood.nameRaw;
  rawFood.type = FoodType::fruit;
  rawFood.price = 200;
  rawFood.nutrient = 200;
  rawFood.quench = 0;
  rawFood.bonusStat.Con = 10;
  rawFood.bonusStat.Mag = 20;
  mRawFruits.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Cherry";
  rawFood.nameCooked = rawFood.nameRaw;
  rawFood.type = FoodType::fruit;
  rawFood.price = 200;
  rawFood.nutrient = 190;
  rawFood.quench = 10;
  rawFood.bonusStat.Per = 10;
  rawFood.bonusStat.Mag = 10;
  rawFood.bonusStat.Chr = 10;
  mRawFruits.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Grape";
  rawFood.nameCooked = rawFood.nameRaw;
  rawFood.type = FoodType::fruit;
  rawFood.price = 200;
  rawFood.nutrient = 180;
  rawFood.quench = 20;
  rawFood.bonusStat.Per = 10;
  rawFood.bonusStat.Mag = 10;
  rawFood.bonusStat.Chr = 10;
  mRawFruits.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Lemon";
  rawFood.nameCooked = rawFood.nameRaw;
  rawFood.type = FoodType::fruit;
  rawFood.price = 200;
  rawFood.nutrient = 150;
  rawFood.quench = 50;
  rawFood.bonusStat.Wil = 10;
  rawFood.bonusStat.Mag = 10;
  rawFood.bonusStat.Chr = 10;
  mRawFruits.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Strawberry";
  rawFood.nameCooked = rawFood.nameRaw;
  rawFood.type = FoodType::fruit;
  rawFood.price = 200;
  rawFood.nutrient = 180;
  rawFood.quench = 20;
  rawFood.bonusStat.Per = 10;
  rawFood.bonusStat.Mag = 10;
  rawFood.bonusStat.Chr = 10;
  mRawFruits.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Orange";
  rawFood.nameCooked = rawFood.nameRaw;
  rawFood.type = FoodType::fruit;
  rawFood.price = 200;
  rawFood.nutrient = 170;
  rawFood.quench = 30;
  rawFood.bonusStat.Per = 10;
  rawFood.bonusStat.Mag = 10;
  rawFood.bonusStat.Chr = 10;
  mRawFruits.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Pineapple";
  rawFood.nameCooked = rawFood.nameRaw;
  rawFood.type = FoodType::fruit;
  rawFood.price = 200;
  rawFood.nutrient = 170;
  rawFood.quench = 30;
  rawFood.bonusStat.Per = 10;
  rawFood.bonusStat.Mag = 10;
  rawFood.bonusStat.Chr = 10;
  mRawFruits.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Melon";
  rawFood.nameCooked = rawFood.nameRaw;
  rawFood.type = FoodType::fruit;
  rawFood.price = 200;
  rawFood.nutrient = 50;
  rawFood.quench = 150;
  rawFood.bonusStat.Per = 10;
  rawFood.bonusStat.Mag = 10;
  rawFood.bonusStat.Chr = 10;
  mRawFruits.push_back(rawFood);

  //Ruleset v1: vegatable slot 0-4
  rawFood = prototype;
  rawFood.nameRaw = "Cucumber";
  rawFood.nameCooked = rawFood.nameRaw;
  rawFood.type = FoodType::vegatable;
  rawFood.price = 100;
  rawFood.nutrient = 20;
  rawFood.quench = 80;
  rawFood.bonusStat.Lrn = 10;
  rawFood.bonusStat.Wil = 10;
  mRawVegatables.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Mushroom";
  rawFood.nameCooked = rawFood.nameRaw;
  rawFood.type = FoodType::vegatable;
  rawFood.price = 100;
  rawFood.nutrient = 100;
  rawFood.quench = 0;
  rawFood.bonusStat.Lrn = 10;
  rawFood.bonusStat.Wil = 10;
  mRawVegatables.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Tomato";
  rawFood.nameCooked = rawFood.nameRaw;
  rawFood.type = FoodType::vegatable;
  rawFood.price = 100;
  rawFood.nutrient = 90;
  rawFood.quench = 10;
  rawFood.bonusStat.Lrn = 10;
  rawFood.bonusStat.Wil = 10;
  mRawVegatables.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Potato";
  rawFood.nameCooked = rawFood.nameRaw;
  rawFood.type = FoodType::vegatable;
  rawFood.price = 100;
  rawFood.nutrient = 100;
  rawFood.quench = 0;
  rawFood.bonusStat.Lrn = 10;
  rawFood.bonusStat.Wil = 10;
  mRawVegatables.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Zucchini";
  rawFood.nameCooked = rawFood.nameRaw;
  rawFood.type = FoodType::vegatable;
  rawFood.price = 100;
  rawFood.nutrient = 100;
  rawFood.quench = 0;
  rawFood.bonusStat.Lrn = 10;
  rawFood.bonusStat.Wil = 10;
  mRawVegatables.push_back(rawFood);

  //Ruleset v1: fish slot 0-7
  rawFood = prototype;
  rawFood.nameRaw = "Gold Fish";
  rawFood.nameCooked = "Raw " + rawFood.nameRaw;
  rawFood.type = FoodType::fish;
  rawFood.price = 150;
  rawFood.nutrient = 150;
  rawFood.quench = 0;
  rawFood.bonusStat.Dex = 10;
  rawFood.bonusStat.Per = 15;
  mRawFish.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Jellyfish";
  rawFood.nameCooked = "Raw " + rawFood.nameRaw;
  rawFood.type = FoodType::fish;
  rawFood.price = 200;
  rawFood.nutrient = 200;
  rawFood.quench = 0;
  rawFood.bonusStat.Dex = 10;
  rawFood.bonusStat.Per = 20;
  mRawFish.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Carp";
  rawFood.nameCooked = "Raw " + rawFood.nameRaw;
  rawFood.type = FoodType::fish;
  rawFood.price = 210;
  rawFood.nutrient = 210;
  rawFood.quench = 0;
  rawFood.bonusStat.Dex = 20;
  rawFood.bonusStat.Per = 10;
  mRawFish.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Trout";
  rawFood.nameCooked = "Raw " + rawFood.nameRaw;
  rawFood.type = FoodType::fish;
  rawFood.price = 230;
  rawFood.nutrient = 230;
  rawFood.quench = 0;
  rawFood.bonusStat.Dex = 20;
  rawFood.bonusStat.Per = 15;
  mRawFish.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Cod";
  rawFood.nameCooked = "Raw " + rawFood.nameRaw;
  rawFood.type = FoodType::fish;
  rawFood.price = 240;
  rawFood.nutrient = 240;
  rawFood.quench = 0;
  rawFood.bonusStat.Dex = 20;
  rawFood.bonusStat.Per = 15;
  mRawFish.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Eel";
  rawFood.nameCooked = "Raw " + rawFood.nameRaw;
  rawFood.type = FoodType::fish;
  rawFood.price = 270;
  rawFood.nutrient = 270;
  rawFood.quench = 0;
  rawFood.bonusStat.Dex = 20;
  rawFood.bonusStat.Per = 15;
  mRawFish.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Salmon";
  rawFood.nameCooked = "Raw " + rawFood.nameRaw;
  rawFood.type = FoodType::fish;
  rawFood.price = 310;
  rawFood.nutrient = 310;
  rawFood.quench = 0;
  rawFood.bonusStat.Dex = 25;
  rawFood.bonusStat.Per = 15;
  mRawFish.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Tuna";
  rawFood.nameCooked = "Raw " + rawFood.nameRaw;
  rawFood.type = FoodType::fish;
  rawFood.price = 350;
  rawFood.nutrient = 350;
  rawFood.quench = 0;
  rawFood.bonusStat.Dex = 30;
  rawFood.bonusStat.Per = 15;
  mRawFish.push_back(rawFood);

  //Ruleset v1: corpse slot 0-5
  rawFood = prototype;
  rawFood.nameRaw = "Mouse";
  rawFood.nameCooked = "Dead " + rawFood.nameRaw;
  rawFood.type = FoodType::corpse;
  rawFood.price = 10;
  rawFood.nutrient = 100;
  rawFood.quench = 0;
  rawFood.bonusStat.Lrn = 10;
  rawFood.bonusStat.Wil = 10;
  mRawCorpse.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Bird";
  rawFood.nameCooked = "Dead " + rawFood.nameRaw;
  rawFood.type = FoodType::corpse;
  rawFood.price = 10;
  rawFood.nutrient = 100;
  rawFood.quench = 0;
  rawFood.bonusStat.Lrn = 10;
  rawFood.bonusStat.Wil = 10;
  mRawCorpse.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Snake";
  rawFood.nameCooked = "Dead " + rawFood.nameRaw;
  rawFood.type = FoodType::corpse;
  rawFood.price = 10;
  rawFood.nutrient = 100;
  rawFood.quench = 0;
  rawFood.bonusStat.Lrn = 10;
  rawFood.bonusStat.Wil = 10;
  mRawCorpse.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Chicken";
  rawFood.nameCooked = "Raw " + rawFood.nameRaw;
  rawFood.type = FoodType::corpse;
  rawFood.price = 240;
  rawFood.nutrient = 240;
  rawFood.quench = 0;
  rawFood.bonusStat.Str = 20;
  rawFood.bonusStat.Con = 15;
  mRawCorpse.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Pork";
  rawFood.nameCooked = "Raw " + rawFood.nameRaw;
  rawFood.type = FoodType::corpse;
  rawFood.price = 350;
  rawFood.nutrient = 350;
  rawFood.quench = 0;
  rawFood.bonusStat.Str = 30;
  rawFood.bonusStat.Con = 15;
  mRawFish.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Beef";
  rawFood.nameCooked = "Raw " + rawFood.nameRaw;
  rawFood.type = FoodType::corpse;
  rawFood.price = 400;
  rawFood.nutrient = 400;
  rawFood.quench = 0;
  rawFood.bonusStat.Str = 40;
  rawFood.bonusStat.Con = 20;
  mRawFish.push_back(rawFood);

  //Flour
  rawFood = prototype;
  rawFood.nameRaw = "Flour";
  rawFood.nameCooked = rawFood.nameRaw;
  rawFood.type = FoodType::flour;
  rawFood.price = 100;
  rawFood.nutrient = 75;
  rawFood.quench = 0;
  mRawFlour = rawFood;

  //Ruleset v1: junk slot 0-5
  rawFood = prototype;
  rawFood.nameRaw = "Potato Chip";
  rawFood.nameCooked = rawFood.nameRaw;
  rawFood.type = FoodType::junk;
  rawFood.dishLevel = 99;
  rawFood.price = 5000;
  rawFood.nutrient = 500;
  rawFood.quench = 0;
  mRawJunk.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Pizza";
  rawFood.nameCooked = rawFood.nameRaw;
  rawFood.type = FoodType::junk;
  rawFood.dishLevel = 99;
  rawFood.price = 5500;
  rawFood.nutrient = 500;
  rawFood.quench = 0;
  mRawJunk.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Chocolate";
  rawFood.nameCooked = rawFood.nameRaw;
  rawFood.type = FoodType::junk;
  rawFood.dishLevel = 99;
  rawFood.price = 7500;
  rawFood.nutrient = 500;
  rawFood.quench = 0;
  rawFood.stamina = 300;
  mRawJunk.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Popcorn";
  rawFood.nameCooked = rawFood.nameRaw;
  rawFood.type = FoodType::junk;
  rawFood.dishLevel = 99;
  rawFood.price = 1000;
  rawFood.nutrient = 200;
  rawFood.quench = 0;
  mRawJunk.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Pop Tarts";
  rawFood.nameCooked = rawFood.nameRaw;
  rawFood.type = FoodType::junk;
  rawFood.dishLevel = 99;
  rawFood.price = 1500;
  rawFood.nutrient = 200;
  rawFood.quench = 0;
  mRawJunk.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Instant Ramen";
  rawFood.nameCooked = rawFood.nameRaw;
  rawFood.type = FoodType::junk;
  rawFood.dishLevel = 99;
  rawFood.price = 2000;
  rawFood.nutrient = 500;
  rawFood.quench = 0;
  mRawJunk.push_back(rawFood);
}