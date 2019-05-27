#include "food.h"


Food::Food(World & world) : mWorld(world)
{
  loadRawFood();
}

Food::~Food()
{
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
  case Food::FoodType::junkFood:
    food = mRawJunkFood.at(mWorld.getRandomNumber(seed, 0, 5));
    break;
  case Food::FoodType::vermin:
    food = mRawCorpse.at(mWorld.getRandomNumber(seed, 0, 2));
    break;
  }

  food.price += mWorld.getRandomNumber(seed, 0, 200);
  food.id = mWorld.generateID();
  return food;
}

void Food::randomizeCookedFood(const std::string& seed, FoodItem& cookableFood)
{
  if (cookableFood.dishLevel > 0 || cookableFood.type == FoodType::junkFood)
  {
    return;
  }

  cookableFood.price += mWorld.getRandomNumber(seed, 0, 200);
  cookableFood.dishLevel = mWorld.getRandomNumber(seed, 1, 5);

  if (cookableFood.type == FoodType::fruit)
  {
    if (cookableFood.dishLevel == 1)
    {
      cookableFood.name = cookableFood.nameRaw + " Jelly";
      cookableFood.price *= 2;
      cookableFood.nutrient *= 1;
      cookableFood.quench += 1000;
      cookableFood.bonusStat.mag += 10;
    }
    else if (cookableFood.dishLevel == 2)
    {
      cookableFood.name = cookableFood.nameRaw + " Pudding";
      cookableFood.price *= 3;
      cookableFood.nutrient *= 2;
      cookableFood.bonusStat.chr += 10;
      cookableFood.bonusStat.mag += 10;
    }
    else if (cookableFood.dishLevel == 3)
    {
      cookableFood.name = cookableFood.nameRaw + " Pie";
      cookableFood.price *= 4;
      cookableFood.nutrient *= 2;
      cookableFood.stamina += 2500;
      cookableFood.bonusStat.chr += 10;
      cookableFood.bonusStat.mag += 20;
    }
    else if (cookableFood.dishLevel == 4)
    {
      cookableFood.name = cookableFood.nameRaw + " Sherbert";
      cookableFood.price *= 5;
      cookableFood.nutrient *= 3;
      cookableFood.quench += 2000;
      cookableFood.stamina += 5000;
      cookableFood.bonusStat.chr += 10;
      cookableFood.bonusStat.mag += 20;
    }
    else if (cookableFood.dishLevel == 5)
    {
      cookableFood.name = cookableFood.nameRaw + " Ice Cream";
      cookableFood.price *= 6;
      cookableFood.nutrient *= 3;
      cookableFood.quench += 2000;
      cookableFood.stamina += 10000;
      cookableFood.bonusStat.chr += 20;
      cookableFood.bonusStat.mag += 30;
    }
  }
  else if (cookableFood.type == FoodType::vegatable)
  {
    if (cookableFood.dishLevel == 1)
    {
      cookableFood.name = cookableFood.nameRaw + " Salad";
      cookableFood.price *= 2;
      cookableFood.nutrient *= 1;
      cookableFood.bonusStat.lrn += 10;
    }
    else if (cookableFood.dishLevel == 2)
    {
      cookableFood.name = cookableFood.nameRaw + " Roll";
      cookableFood.price *= 3;
      cookableFood.nutrient *= 2;
      cookableFood.bonusStat.wil += 10;
      cookableFood.bonusStat.lrn += 10;
    }
    else if (cookableFood.dishLevel == 3)
    {
      cookableFood.name = "Baked " + cookableFood.nameRaw;
      cookableFood.price *= 4;
      cookableFood.nutrient *= 2;
      cookableFood.stamina += 2500;
      cookableFood.bonusStat.wil += 10;
      cookableFood.bonusStat.lrn += 20;
    }
    else if (cookableFood.dishLevel == 4)
    {
      cookableFood.name = "Cream of " + cookableFood.nameRaw + " Soup";
      cookableFood.price *= 5;
      cookableFood.nutrient *= 3;
      cookableFood.quench += 3000;
      cookableFood.stamina += 5000;
      cookableFood.bonusStat.wil += 20;
      cookableFood.bonusStat.lrn += 20;
    }
    else if (cookableFood.dishLevel == 5)
    {
      cookableFood.name = cookableFood.nameRaw + " Ratatouille";
      cookableFood.price *= 6;
      cookableFood.nutrient *= 3;
      cookableFood.stamina += 10000;
      cookableFood.bonusStat.wil += 20;
      cookableFood.bonusStat.lrn += 30;
    }
  }
  else if (cookableFood.type == FoodType::fish)
  {
    if (cookableFood.dishLevel == 1)
    {
      cookableFood.name = "Fried " + cookableFood.nameRaw;
      cookableFood.price *= 2;
      cookableFood.nutrient *= 1;
      cookableFood.bonusStat.dex += 10;
    }
    else if (cookableFood.dishLevel == 2)
    {
      cookableFood.name = cookableFood.nameRaw + " Soup";
      cookableFood.price *= 3;
      cookableFood.nutrient *= 2;
      cookableFood.quench += 3000;
      cookableFood.bonusStat.per += 1;
      cookableFood.bonusStat.dex += 1;
    }
    else if (cookableFood.dishLevel == 3)
    {
      cookableFood.name = "Grilled " + cookableFood.nameRaw;
      cookableFood.price *= 4;
      cookableFood.nutrient *= 2;
      cookableFood.stamina += 2500;
      cookableFood.bonusStat.per += 10;
      cookableFood.bonusStat.dex += 20;
    }
    else if (cookableFood.dishLevel == 4)
    {
      cookableFood.name = cookableFood.nameRaw + " Tempura";
      cookableFood.price *= 5;
      cookableFood.nutrient *= 3;
      cookableFood.stamina += 50;
      cookableFood.bonusStat.per += 20;
      cookableFood.bonusStat.dex += 20;
    }
    else if (cookableFood.dishLevel == 5)
    {
      cookableFood.name = cookableFood.nameRaw + " Sushi";
      cookableFood.price *= 6;
      cookableFood.nutrient *= 3;
      cookableFood.stamina += 10000;
      cookableFood.bonusStat.per += 20;
      cookableFood.bonusStat.dex += 30;
    }

  }
  else if (cookableFood.type == FoodType::corpse)
  {
    if (cookableFood.dishLevel == 1)
    {
      cookableFood.name = "Roasted  " + cookableFood.nameRaw;
      cookableFood.price *= 2;
      cookableFood.nutrient *= 1;
      cookableFood.bonusStat.str += 10;
    }
    else if (cookableFood.dishLevel == 2)
    {
      cookableFood.name = cookableFood.nameRaw + " Stew";
      cookableFood.price *= 3;
      cookableFood.nutrient *= 2;
      cookableFood.quench += 3000;
      cookableFood.bonusStat.con += 1;
      cookableFood.bonusStat.str += 1;
    }
    else if (cookableFood.dishLevel == 3)
    {
      cookableFood.name = cookableFood.nameRaw + " Cutlet";
      cookableFood.price *= 4;
      cookableFood.nutrient *= 2;
      cookableFood.stamina += 2500;
      cookableFood.bonusStat.con += 10;
      cookableFood.bonusStat.str += 20;
    }
    else if (cookableFood.dishLevel == 4)
    {
      cookableFood.name = cookableFood.nameRaw + " Burger";
      cookableFood.price *= 5;
      cookableFood.nutrient *= 3;
      cookableFood.stamina += 5000;
      cookableFood.bonusStat.con += 20;
      cookableFood.bonusStat.str += 20;
    }
    else if (cookableFood.dishLevel == 5)
    {
      cookableFood.name = cookableFood.nameRaw + " Steak";
      cookableFood.price *= 6;
      cookableFood.nutrient *= 3;
      cookableFood.stamina += 10000;
      cookableFood.bonusStat.con += 20;
      cookableFood.bonusStat.str += 30;
    }
  }
  else if (cookableFood.type == FoodType::flour)
  {
    if (cookableFood.dishLevel == 1)
    {
      cookableFood.name = "Flat Bread";
      cookableFood.price *= 2;
      cookableFood.nutrient *= 2;
      cookableFood.bonusStat.str += 5;
      cookableFood.bonusStat.con += 5;
      cookableFood.bonusStat.dex += 5;
      cookableFood.bonusStat.per += 5;
      cookableFood.bonusStat.lrn += 5;
      cookableFood.bonusStat.wil += 5;
      cookableFood.bonusStat.mag += 5;
    }
    else if (cookableFood.dishLevel == 2)
    {
      cookableFood.name = "Sandwich";
      cookableFood.price *= 3;
      cookableFood.nutrient *= 3;
      cookableFood.bonusStat.str += 10;
      cookableFood.bonusStat.con += 10;
      cookableFood.bonusStat.dex += 10;
      cookableFood.bonusStat.per += 10;
      cookableFood.bonusStat.lrn += 10;
      cookableFood.bonusStat.wil += 10;
      cookableFood.bonusStat.mag += 10;
    }
    else if (cookableFood.dishLevel == 3)
    {
      cookableFood.name = "Croissant";
      cookableFood.price *= 4;
      cookableFood.nutrient *= 4;
      cookableFood.stamina += 2500;
      cookableFood.bonusStat.str += 12;
      cookableFood.bonusStat.con += 12;
      cookableFood.bonusStat.dex += 12;
      cookableFood.bonusStat.per += 12;
      cookableFood.bonusStat.lrn += 12;
      cookableFood.bonusStat.wil += 12;
      cookableFood.bonusStat.mag += 12;
    }
    else if (cookableFood.dishLevel == 4)
    {
      cookableFood.name = "Cinnamon Roll";
      cookableFood.price *= 5;
      cookableFood.nutrient *= 5;
      cookableFood.stamina += 5000;
      cookableFood.bonusStat.str += 15;
      cookableFood.bonusStat.con += 15;
      cookableFood.bonusStat.dex += 15;
      cookableFood.bonusStat.per += 15;
      cookableFood.bonusStat.lrn += 15;
      cookableFood.bonusStat.wil += 15;
      cookableFood.bonusStat.mag += 15;
    }
    else if (cookableFood.dishLevel == 5)
    {
      cookableFood.name = "Melonpan";
      cookableFood.price *= 6;
      cookableFood.nutrient *= 6;
      cookableFood.stamina += 10000;
      cookableFood.bonusStat.str += 20;
      cookableFood.bonusStat.con += 20;
      cookableFood.bonusStat.dex += 20;
      cookableFood.bonusStat.per += 20;
      cookableFood.bonusStat.lrn += 20;
      cookableFood.bonusStat.wil += 20;
      cookableFood.bonusStat.mag += 20;
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
  for (auto& element : mRawJunkFood)
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
  prototype.name = "";
  prototype.type = FoodType::fruit;
  prototype.dishLevel = 0;
  prototype.price = 100;
  prototype.nutrient = 1000;
  prototype.quench = 1000;
  prototype.stamina = 0;
  prototype.bonusStat = World::Stat{ 0,0,0,0,0,0,0,0,0,0 };

  FoodItem rawFood;

  //Ruleset v1: fruit slot 0-7
  rawFood = prototype;
  rawFood.nameRaw = "Apple";
  rawFood.name = rawFood.nameRaw;
  rawFood.type = FoodType::fruit;
  rawFood.price = 200;
  rawFood.nutrient = 2000;
  rawFood.quench = 0;
  rawFood.bonusStat.con = 10;
  rawFood.bonusStat.mag = 20;
  mRawFruits.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Cherry";
  rawFood.name = rawFood.nameRaw;
  rawFood.type = FoodType::fruit;
  rawFood.price = 200;
  rawFood.nutrient = 1900;
  rawFood.quench = 100;
  rawFood.bonusStat.per = 10;
  rawFood.bonusStat.mag = 10;
  rawFood.bonusStat.chr = 10;
  mRawFruits.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Grape";
  rawFood.name = rawFood.nameRaw;
  rawFood.type = FoodType::fruit;
  rawFood.price = 200;
  rawFood.nutrient = 1800;
  rawFood.quench = 200;
  rawFood.bonusStat.per = 10;
  rawFood.bonusStat.mag = 10;
  rawFood.bonusStat.chr = 10;
  mRawFruits.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Lemon";
  rawFood.name = rawFood.nameRaw;
  rawFood.type = FoodType::fruit;
  rawFood.price = 200;
  rawFood.nutrient = 1500;
  rawFood.quench = 500;
  rawFood.bonusStat.wil = 10;
  rawFood.bonusStat.mag = 10;
  rawFood.bonusStat.chr = 10;
  mRawFruits.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Strawberry";
  rawFood.name = rawFood.nameRaw;
  rawFood.type = FoodType::fruit;
  rawFood.price = 200;
  rawFood.nutrient = 1800;
  rawFood.quench = 200;
  rawFood.bonusStat.per = 10;
  rawFood.bonusStat.mag = 10;
  rawFood.bonusStat.chr = 10;
  mRawFruits.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Orange";
  rawFood.name = rawFood.nameRaw;
  rawFood.type = FoodType::fruit;
  rawFood.price = 200;
  rawFood.nutrient = 1700;
  rawFood.quench = 300;
  rawFood.bonusStat.per = 10;
  rawFood.bonusStat.mag = 10;
  rawFood.bonusStat.chr = 10;
  mRawFruits.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Pineapple";
  rawFood.name = rawFood.nameRaw;
  rawFood.type = FoodType::fruit;
  rawFood.price = 200;
  rawFood.nutrient = 1700;
  rawFood.quench = 300;
  rawFood.bonusStat.per = 10;
  rawFood.bonusStat.mag = 10;
  rawFood.bonusStat.chr = 10;
  mRawFruits.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Melon";
  rawFood.name = rawFood.nameRaw;
  rawFood.type = FoodType::fruit;
  rawFood.price = 200;
  rawFood.nutrient = 500;
  rawFood.quench = 1500;
  rawFood.bonusStat.per = 10;
  rawFood.bonusStat.mag = 10;
  rawFood.bonusStat.chr = 10;
  mRawFruits.push_back(rawFood);

  //Ruleset v1: vegatable slot 0-4
  rawFood = prototype;
  rawFood.nameRaw = "Cucumber";
  rawFood.name = rawFood.nameRaw;
  rawFood.type = FoodType::vegatable;
  rawFood.price = 100;
  rawFood.nutrient = 200;
  rawFood.quench = 800;
  rawFood.bonusStat.lrn = 10;
  rawFood.bonusStat.wil = 10;
  mRawVegatables.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Mushroom";
  rawFood.name = rawFood.nameRaw;
  rawFood.type = FoodType::vegatable;
  rawFood.price = 100;
  rawFood.nutrient = 1000;
  rawFood.quench = 0;
  rawFood.bonusStat.lrn = 10;
  rawFood.bonusStat.wil = 10;
  mRawVegatables.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Tomato";
  rawFood.name = rawFood.nameRaw;
  rawFood.type = FoodType::vegatable;
  rawFood.price = 100;
  rawFood.nutrient = 900;
  rawFood.quench = 100;
  rawFood.bonusStat.lrn = 10;
  rawFood.bonusStat.wil = 10;
  mRawVegatables.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Potato";
  rawFood.name = rawFood.nameRaw;
  rawFood.type = FoodType::vegatable;
  rawFood.price = 100;
  rawFood.nutrient = 1000;
  rawFood.quench = 0;
  rawFood.bonusStat.lrn = 10;
  rawFood.bonusStat.wil = 10;
  mRawVegatables.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Zucchini";
  rawFood.name = rawFood.nameRaw;
  rawFood.type = FoodType::vegatable;
  rawFood.price = 100;
  rawFood.nutrient = 1000;
  rawFood.quench = 0;
  rawFood.bonusStat.lrn = 10;
  rawFood.bonusStat.wil = 10;
  mRawVegatables.push_back(rawFood);

  //Ruleset v1: fish slot 0-7
  rawFood = prototype;
  rawFood.nameRaw = "Gold Fish";
  rawFood.name = "Raw " + rawFood.nameRaw;
  rawFood.type = FoodType::fish;
  rawFood.price = 150;
  rawFood.nutrient = 1500;
  rawFood.quench = 0;
  rawFood.bonusStat.dex = 10;
  rawFood.bonusStat.per = 15;
  mRawFish.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Jellyfish";
  rawFood.name = "Raw " + rawFood.nameRaw;
  rawFood.type = FoodType::fish;
  rawFood.price = 200;
  rawFood.nutrient = 2000;
  rawFood.quench = 0;
  rawFood.bonusStat.dex = 10;
  rawFood.bonusStat.per = 20;
  mRawFish.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Carp";
  rawFood.name = "Raw " + rawFood.nameRaw;
  rawFood.type = FoodType::fish;
  rawFood.price = 210;
  rawFood.nutrient = 2100;
  rawFood.quench = 0;
  rawFood.bonusStat.dex = 20;
  rawFood.bonusStat.per = 10;
  mRawFish.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Trout";
  rawFood.name = "Raw " + rawFood.nameRaw;
  rawFood.type = FoodType::fish;
  rawFood.price = 230;
  rawFood.nutrient = 2300;
  rawFood.quench = 0;
  rawFood.bonusStat.dex = 20;
  rawFood.bonusStat.per = 15;
  mRawFish.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Cod";
  rawFood.name = "Raw " + rawFood.nameRaw;
  rawFood.type = FoodType::fish;
  rawFood.price = 240;
  rawFood.nutrient = 2400;
  rawFood.quench = 0;
  rawFood.bonusStat.dex = 20;
  rawFood.bonusStat.per = 15;
  mRawFish.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Eel";
  rawFood.name = "Raw " + rawFood.nameRaw;
  rawFood.type = FoodType::fish;
  rawFood.price = 270;
  rawFood.nutrient = 2700;
  rawFood.quench = 0;
  rawFood.bonusStat.dex = 20;
  rawFood.bonusStat.per = 15;
  mRawFish.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Salmon";
  rawFood.name = "Raw " + rawFood.nameRaw;
  rawFood.type = FoodType::fish;
  rawFood.price = 310;
  rawFood.nutrient = 3100;
  rawFood.quench = 0;
  rawFood.bonusStat.dex = 25;
  rawFood.bonusStat.per = 15;
  mRawFish.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Tuna";
  rawFood.name = "Raw " + rawFood.nameRaw;
  rawFood.type = FoodType::fish;
  rawFood.price = 350;
  rawFood.nutrient = 3500;
  rawFood.quench = 0;
  rawFood.bonusStat.dex = 30;
  rawFood.bonusStat.per = 15;
  mRawFish.push_back(rawFood);

  //Ruleset v1: corpse slot 0-5
  rawFood = prototype;
  rawFood.nameRaw = "Mouse";
  rawFood.name = "Dead " + rawFood.nameRaw;
  rawFood.type = FoodType::corpse;
  rawFood.price = 10;
  rawFood.nutrient = 1000;
  rawFood.quench = 0;
  rawFood.bonusStat.lrn = 10;
  rawFood.bonusStat.wil = 10;
  mRawCorpse.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Bird";
  rawFood.name = "Dead " + rawFood.nameRaw;
  rawFood.type = FoodType::corpse;
  rawFood.price = 10;
  rawFood.nutrient = 1000;
  rawFood.quench = 0;
  rawFood.bonusStat.lrn = 10;
  rawFood.bonusStat.wil = 10;
  mRawCorpse.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Snake";
  rawFood.name = "Dead " + rawFood.nameRaw;
  rawFood.type = FoodType::corpse;
  rawFood.price = 10;
  rawFood.nutrient = 1000;
  rawFood.quench = 0;
  rawFood.bonusStat.lrn = 10;
  rawFood.bonusStat.wil = 10;
  mRawCorpse.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Chicken";
  rawFood.name = "Raw " + rawFood.nameRaw;
  rawFood.type = FoodType::corpse;
  rawFood.price = 240;
  rawFood.nutrient = 2400;
  rawFood.quench = 0;
  rawFood.bonusStat.str = 20;
  rawFood.bonusStat.con = 15;
  mRawCorpse.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Pork";
  rawFood.name = "Raw " + rawFood.nameRaw;
  rawFood.type = FoodType::corpse;
  rawFood.price = 350;
  rawFood.nutrient = 3500;
  rawFood.quench = 0;
  rawFood.bonusStat.str = 30;
  rawFood.bonusStat.con = 15;
  mRawCorpse.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Beef";
  rawFood.name = "Raw " + rawFood.nameRaw;
  rawFood.type = FoodType::corpse;
  rawFood.price = 400;
  rawFood.nutrient = 4000;
  rawFood.quench = 0;
  rawFood.bonusStat.str = 40;
  rawFood.bonusStat.con = 20;
  mRawCorpse.push_back(rawFood);

  //Flour
  rawFood = prototype;
  rawFood.nameRaw = "Flour";
  rawFood.name = rawFood.nameRaw;
  rawFood.type = FoodType::flour;
  rawFood.price = 100;
  rawFood.nutrient = 750;
  rawFood.quench = 0;
  mRawFlour = rawFood;

  //Ruleset v1: junk food slot 0-5
  rawFood = prototype;
  rawFood.nameRaw = "Potato Chip";
  rawFood.name = rawFood.nameRaw;
  rawFood.type = FoodType::junkFood;
  rawFood.dishLevel = 99;
  rawFood.price = 5000;
  rawFood.nutrient = 5000;
  rawFood.quench = 0;
  mRawJunkFood.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Pizza";
  rawFood.name = rawFood.nameRaw;
  rawFood.type = FoodType::junkFood;
  rawFood.dishLevel = 99;
  rawFood.price = 5500;
  rawFood.nutrient = 5000;
  rawFood.quench = 0;
  mRawJunkFood.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Chocolate";
  rawFood.name = rawFood.nameRaw;
  rawFood.type = FoodType::junkFood;
  rawFood.dishLevel = 99;
  rawFood.price = 7500;
  rawFood.nutrient = 5000;
  rawFood.quench = 0;
  rawFood.stamina = 300;
  mRawJunkFood.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Popcorn";
  rawFood.name = rawFood.nameRaw;
  rawFood.type = FoodType::junkFood;
  rawFood.dishLevel = 99;
  rawFood.price = 1000;
  rawFood.nutrient = 2000;
  rawFood.quench = 0;
  mRawJunkFood.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Pop Tarts";
  rawFood.name = rawFood.nameRaw;
  rawFood.type = FoodType::junkFood;
  rawFood.dishLevel = 99;
  rawFood.price = 1500;
  rawFood.nutrient = 2000;
  rawFood.quench = 0;
  mRawJunkFood.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Instant Ramen";
  rawFood.name = rawFood.nameRaw;
  rawFood.type = FoodType::junkFood;
  rawFood.dishLevel = 99;
  rawFood.price = 2000;
  rawFood.nutrient = 5000;
  rawFood.quench = 0;
  mRawJunkFood.push_back(rawFood);
}