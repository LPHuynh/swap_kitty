#include "food.h"


Food::Food()
{
  loadRawFood();
}

Food::~Food()
{
}

Food::FoodItem Food::randomizeFruit(const std::string& seed)
{
  return mRawFruits.at(World::getRandomNumber(seed + "fruit__", 0, 7));
}

Food::FoodItem Food::randomizeVegatable(const std::string& seed)
{
  return mRawVegatables.at(World::getRandomNumber(seed + "vegatable__", 0, 4));
}

Food::FoodItem Food::randomizeFish(const std::string& seed, int64_t maxCost)
{
  return mRawFish.at(World::getRandomNumber(seed + "fish__", 0, 7));
}

Food::FoodItem Food::randomizeJunk(const std::string& seed)
{
  return mRawOther.at(World::getRandomNumber(seed + "junk__", 1, 6));
}

Food::FoodItem Food::randomizeCookedFood(const std::string& seed, const FoodItem& rawFood)
{
  if (rawFood.dishLevel > 0 || rawFood.type == FoodType::junk)
  {
    return rawFood;
  }

  FoodItem foodItem = rawFood;
  foodItem.dishLevel = World::getRandomNumber(seed + "cookfood__", 1, 5);

  if (foodItem.type == FoodType::fruit)
  {
    if (foodItem.dishLevel == 1)
    {
      foodItem.nameCooked = foodItem.nameRaw + " Jelly";
      foodItem.price *= 2;
      foodItem.nutrient *= 1;
      foodItem.quench += 10;
      foodItem.bonusStat.Mag += 10;
    }
    if (foodItem.dishLevel == 2)
    {
      foodItem.nameCooked = foodItem.nameRaw + " Pudding";
      foodItem.price *= 3;
      foodItem.nutrient *= 2;
      foodItem.bonusStat.Chr += 10;
      foodItem.bonusStat.Mag += 10;
    }
    if (foodItem.dishLevel == 3)
    {
      foodItem.nameCooked = foodItem.nameRaw + " Pie";
      foodItem.price *= 4;
      foodItem.nutrient *= 2;
      foodItem.stamina += 25;
      foodItem.bonusStat.Chr += 10;
      foodItem.bonusStat.Mag += 20;
    }
    if (foodItem.dishLevel == 4)
    {
      foodItem.nameCooked = foodItem.nameRaw + " Sherbert";
      foodItem.price *= 5;
      foodItem.nutrient *= 3;
      foodItem.quench += 20;
      foodItem.stamina += 50;
      foodItem.bonusStat.Chr += 10;
      foodItem.bonusStat.Mag += 20;
    }
    if (foodItem.dishLevel == 5)
    {
      foodItem.nameCooked = foodItem.nameRaw + " Ice Cream";
      foodItem.price *= 6;
      foodItem.nutrient *= 3;
      foodItem.quench += 20;
      foodItem.stamina += 100;
      foodItem.bonusStat.Chr += 20;
      foodItem.bonusStat.Mag += 30;
    }
  }
  else if (foodItem.type == FoodType::vegatable)
  {
    if (foodItem.dishLevel == 1)
    {
      foodItem.nameCooked = foodItem.nameRaw + " Salad";
      foodItem.price *= 2;
      foodItem.nutrient *= 1;
      foodItem.bonusStat.Lrn += 10;
    }
    if (foodItem.dishLevel == 2)
    {
      foodItem.nameCooked = foodItem.nameRaw + " Roll";
      foodItem.price *= 3;
      foodItem.nutrient *= 2;
      foodItem.bonusStat.Wil += 10;
      foodItem.bonusStat.Lrn += 10;
    }
    if (foodItem.dishLevel == 3)
    {
      foodItem.nameCooked = "Baked " + foodItem.nameRaw;
      foodItem.price *= 4;
      foodItem.nutrient *= 2;
      foodItem.stamina += 25;
      foodItem.bonusStat.Wil += 10;
      foodItem.bonusStat.Lrn += 20;
    }
    if (foodItem.dishLevel == 4)
    {
      foodItem.nameCooked = "Creme of " + foodItem.nameRaw;
      foodItem.price *= 5;
      foodItem.nutrient *= 3;
      foodItem.quench += 30;
      foodItem.stamina += 59;
      foodItem.bonusStat.Wil += 20;
      foodItem.bonusStat.Lrn += 20;
    }
    if (foodItem.dishLevel == 5)
    {
      foodItem.nameCooked = foodItem.nameRaw + " Ratatouille";
      foodItem.price *= 6;
      foodItem.nutrient *= 3;
      foodItem.stamina += 100;
      foodItem.bonusStat.Wil += 20;
      foodItem.bonusStat.Lrn += 30;
    }
  }
  else if (foodItem.type == FoodType::fish)
  {
    if (foodItem.dishLevel == 1)
    {
      foodItem.nameCooked = "Fried " + foodItem.nameRaw;
      foodItem.price *= 2;
      foodItem.nutrient *= 1;
      foodItem.bonusStat.Dex += 10;
    }
    if (foodItem.dishLevel == 2)
    {
      foodItem.nameCooked = foodItem.nameRaw + " Soup";
      foodItem.price *= 3;
      foodItem.nutrient *= 2;
      foodItem.quench += 30;
      foodItem.bonusStat.Per += 1;
      foodItem.bonusStat.Dex += 1;
    }
    if (foodItem.dishLevel == 3)
    {
      foodItem.nameCooked = "Grilled " + foodItem.nameRaw;
      foodItem.price *= 4;
      foodItem.nutrient *= 2;
      foodItem.stamina += 25;
      foodItem.bonusStat.Per += 10;
      foodItem.bonusStat.Dex += 20;
    }
    if (foodItem.dishLevel == 4)
    {
      foodItem.nameCooked = foodItem.nameRaw + " Tempura";
      foodItem.price *= 5;
      foodItem.nutrient *= 3;
      foodItem.stamina += 50;
      foodItem.bonusStat.Per += 20;
      foodItem.bonusStat.Dex += 20;
    }
    if (foodItem.dishLevel == 5)
    {
      foodItem.nameCooked = foodItem.nameRaw + " Sushi";
      foodItem.price *= 6;
      foodItem.nutrient *= 3;
      foodItem.stamina += 100;
      foodItem.bonusStat.Per += 20;
      foodItem.bonusStat.Dex += 30;
    }

  }
  else if (foodItem.type == FoodType::corpse)
  {
    if (foodItem.dishLevel == 1)
    {
      foodItem.nameCooked = "Roasted  " + foodItem.nameRaw;
      foodItem.price *= 2;
      foodItem.nutrient *= 1;
      foodItem.bonusStat.Str += 10;
    }
    if (foodItem.dishLevel == 2)
    {
      foodItem.nameCooked = foodItem.nameRaw + " Stew";
      foodItem.price *= 3;
      foodItem.nutrient *= 2;
      foodItem.quench += 30;
      foodItem.bonusStat.Con += 1;
      foodItem.bonusStat.Str += 1;
    }
    if (foodItem.dishLevel == 3)
    {
      foodItem.nameCooked = foodItem.nameRaw + " Cutlet";
      foodItem.price *= 4;
      foodItem.nutrient *= 2;
      foodItem.stamina += 25;
      foodItem.bonusStat.Con += 10;
      foodItem.bonusStat.Str += 20;
    }
    if (foodItem.dishLevel == 4)
    {
      foodItem.nameCooked = foodItem.nameRaw + " Burger";
      foodItem.price *= 5;
      foodItem.nutrient *= 3;
      foodItem.stamina += 50;
      foodItem.bonusStat.Con += 20;
      foodItem.bonusStat.Str += 20;
    }
    if (foodItem.dishLevel == 5)
    {
      foodItem.nameCooked = foodItem.nameRaw + " Steak";
      foodItem.price *= 6;
      foodItem.nutrient *= 3;
      foodItem.stamina += 100;
      foodItem.bonusStat.Con += 20;
      foodItem.bonusStat.Str += 30;
    }
  }
  else if (foodItem.type == FoodType::flour)
  {
    if (foodItem.dishLevel == 1)
    {
      foodItem.nameCooked = "Flat Bread" + foodItem.nameRaw;
      foodItem.price *= 2;
      foodItem.nutrient *= 2;
      foodItem.bonusStat.Str += 5;
      foodItem.bonusStat.Con += 5;
      foodItem.bonusStat.Dex += 5;
      foodItem.bonusStat.Per += 5;
      foodItem.bonusStat.Lrn += 5;
      foodItem.bonusStat.Wil += 5;
      foodItem.bonusStat.Mag += 5;
    }
    if (foodItem.dishLevel == 2)
    {
      foodItem.nameCooked = "Sandwich";
      foodItem.price *= 3;
      foodItem.nutrient *= 3;
      foodItem.quench += 30;
      foodItem.bonusStat.Str += 10;
      foodItem.bonusStat.Con += 10;
      foodItem.bonusStat.Dex += 10;
      foodItem.bonusStat.Per += 10;
      foodItem.bonusStat.Lrn += 10;
      foodItem.bonusStat.Wil += 10;
      foodItem.bonusStat.Mag += 10;
    }
    if (foodItem.dishLevel == 3)
    {
      foodItem.nameCooked = "Croissant";
      foodItem.price *= 4;
      foodItem.nutrient *= 4;
      foodItem.stamina += 25;
      foodItem.bonusStat.Str += 12;
      foodItem.bonusStat.Con += 12;
      foodItem.bonusStat.Dex += 12;
      foodItem.bonusStat.Per += 12;
      foodItem.bonusStat.Lrn += 12;
      foodItem.bonusStat.Wil += 12;
      foodItem.bonusStat.Mag += 12;
    }
    if (foodItem.dishLevel == 4)
    {
      foodItem.nameCooked = "Cinnamon Roll";
      foodItem.price *= 5;
      foodItem.nutrient *= 5;
      foodItem.stamina += 50;
      foodItem.bonusStat.Str += 15;
      foodItem.bonusStat.Con += 15;
      foodItem.bonusStat.Dex += 15;
      foodItem.bonusStat.Per += 15;
      foodItem.bonusStat.Lrn += 15;
      foodItem.bonusStat.Wil += 15;
      foodItem.bonusStat.Mag += 15;
    }
    if (foodItem.dishLevel == 5)
    {
      foodItem.nameCooked = "Melonpan";
      foodItem.price *= 6;
      foodItem.nutrient *= 6;
      foodItem.stamina += 100;
      foodItem.bonusStat.Str += 20;
      foodItem.bonusStat.Con += 20;
      foodItem.bonusStat.Dex += 20;
      foodItem.bonusStat.Per += 20;
      foodItem.bonusStat.Lrn += 20;
      foodItem.bonusStat.Wil += 20;
      foodItem.bonusStat.Mag += 20;
    }
  }

  return FoodItem();
}

void Food::loadRawFood()
{
  FoodItem prototype;

  prototype.nameRaw = "";
  prototype.nameCooked = "";
  prototype.type = FoodType::fruit;
  prototype.dishLevel = 0;
  prototype.price = 100;
  prototype.nutrient = 100;
  prototype.quench = 100;
  prototype.stamina = 0;
  prototype.bonusStat.Str = 0;
  prototype.bonusStat.Con = 0;
  prototype.bonusStat.Dex = 0;
  prototype.bonusStat.Per = 0;
  prototype.bonusStat.Lrn = 0;
  prototype.bonusStat.Wil = 0;
  prototype.bonusStat.Mag = 0;
  prototype.bonusStat.Acc = 0;
  prototype.bonusStat.Cri = 0;

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

  //Ruleset v1: other slot 0-6
  //Flour is used in baked goods
  rawFood = prototype;
  rawFood.nameRaw = "Flour";
  rawFood.nameCooked = rawFood.nameRaw;
  rawFood.type = FoodType::flour;
  rawFood.price = 100;
  rawFood.nutrient = 75;
  rawFood.quench = 0;
  mRawOther.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Potato Chip";
  rawFood.nameCooked = rawFood.nameRaw;
  rawFood.type = FoodType::junk;
  rawFood.dishLevel = 99;
  rawFood.price = 5000;
  rawFood.nutrient = 500;
  rawFood.quench = 0;
  mRawOther.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Pizza";
  rawFood.nameCooked = rawFood.nameRaw;
  rawFood.type = FoodType::junk;
  rawFood.dishLevel = 99;
  rawFood.price = 5500;
  rawFood.nutrient = 500;
  rawFood.quench = 0;
  mRawOther.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Chocolate";
  rawFood.nameCooked = rawFood.nameRaw;
  rawFood.type = FoodType::junk;
  rawFood.dishLevel = 99;
  rawFood.price = 7500;
  rawFood.nutrient = 500;
  rawFood.quench = 0;
  rawFood.stamina = 300;
  mRawOther.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Popcorn";
  rawFood.nameCooked = rawFood.nameRaw;
  rawFood.type = FoodType::junk;
  rawFood.dishLevel = 99;
  rawFood.price = 1000;
  rawFood.nutrient = 200;
  rawFood.quench = 0;
  mRawOther.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Pop Tarts";
  rawFood.nameCooked = rawFood.nameRaw;
  rawFood.type = FoodType::junk;
  rawFood.dishLevel = 99;
  rawFood.price = 1500;
  rawFood.nutrient = 200;
  rawFood.quench = 0;
  mRawOther.push_back(rawFood);

  rawFood = prototype;
  rawFood.nameRaw = "Instant Ramen";
  rawFood.nameCooked = rawFood.nameRaw;
  rawFood.type = FoodType::junk;
  rawFood.dishLevel = 99;
  rawFood.price = 2000;
  rawFood.nutrient = 500;
  rawFood.quench = 0;
  mRawOther.push_back(rawFood);
}