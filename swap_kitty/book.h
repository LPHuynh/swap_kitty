#pragma once

#include "world.h"
class Book
{
public:
  Book();
  ~Book();
  void init(World& world);

  struct BookItem
  {
    uint16_t id;
    std::string name;
    std::string nameRaw;
    int64_t price;
    World::Stat bonusStat;
    World::Skill bonusSkill;
    int16_t health;
  };

  BookItem randomizeBook(const std::string& seed);
  BookItem generateBook(const std::string& seed, const std::string & name);
  void applyWearLevelPrefix(const std::string& seed, Book::BookItem& book);

private:
  World mWorld;

  void loadBook();

  std::vector<BookItem> mBooks;
};

