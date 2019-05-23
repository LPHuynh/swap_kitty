#include "book.h"


Book::Book(World& world) : mWorld(world)
{
  loadBook();
}

Book::~Book()
{
}

Book::BookItem Book::randomizeBook(const std::string& seed)
{
  Book::BookItem book = mBooks.at(mWorld.getRandomNumber(seed, 0, 15));
  book.health = mWorld.getRandomNumber(seed, 1, 100);
  book.price -= (100 - book.health) * 50;
  book.price += mWorld.getRandomNumber(seed, 1, 10000);

  applyWearLevelPrefix(seed, book);

  book.id = mWorld.generateID();
  return book;
}

Book::BookItem Book::generateBook(const std::string& seed, const std::string & name)
{
  Book::BookItem book;
  book.health = mWorld.getRandomNumber(seed, 1, 100);
  book.price -= (100 - book.health) * 50;

  applyWearLevelPrefix(seed, book);

  for (auto& element : mBooks)
  {
    if (element.nameRaw == name)
    {
      book = element;
      book.id = mWorld.generateID();
      return book;
    }
  }

  book = mBooks.at(0);
  book.id = mWorld.generateID();
  return book;
}

void Book::applyWearLevelPrefix(const std::string& seed, Book::BookItem& book)
{
  if (book.health < 20)
  {
    book.name = "A Tattered " + book.nameRaw;
  }
  else if (book.health < 40)
  {
    book.name = "A Worned " + book.nameRaw;
  }
  else if (book.health < 60)
  {
    book.name = "A Used " + book.nameRaw;
  }
  else if (book.health < 80)
  {
    book.name = "A Fine  " + book.nameRaw;
  }
  else
  {
    book.name = "A New  " + book.nameRaw;
  }
}

void Book::loadBook()
{
  BookItem prototype;
  prototype.id = 0;
  prototype.name = "";
  prototype.nameRaw = "";
  prototype.price = 10000;
  prototype.bonusStat = World::Stat{ 0,0,0,0,0,0,0,0,0,0 };
  prototype.bonusSkill = World::Skill{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
  prototype.bonusSkill.literacy = 1;
  prototype.health = 100;

  BookItem book;

  //Ruleset v1: book slot 0-15
  book = prototype;
  book.nameRaw = "Enclopedia of Everything";
  book.name = book.nameRaw;
  book.bonusStat.lrn = 10;
  mBooks.push_back(book);

  book = prototype;
  book.nameRaw = "Book of Zen";
  book.name = book.nameRaw;
  book.bonusStat.wil = 10;
  mBooks.push_back(book);

  book = prototype;
  book.nameRaw = "Magic Book";
  book.name = book.nameRaw;
  book.bonusStat.mag = 10;
  mBooks.push_back(book);

  book = prototype;
  book.nameRaw = "Makeup Guide";
  book.name = book.nameRaw;
  book.bonusStat.chr = 10;
  mBooks.push_back(book);

  book = prototype;
  book.nameRaw = "Dictionary";
  book.name = book.nameRaw;
  book.bonusSkill.literacy = 20;
  mBooks.push_back(book);

  book = prototype;
  book.nameRaw = "Classical Literature";
  book.name = book.nameRaw;
  book.bonusSkill.literacy = 20;
  mBooks.push_back(book);

  book = prototype;
  book.nameRaw = "Cook Book";
  book.name = book.nameRaw;
  book.bonusSkill.cooking = 10;
  mBooks.push_back(book);

  book = prototype;
  book.nameRaw = "Music Book";
  book.name = book.nameRaw;
  book.bonusSkill.music = 10;
  mBooks.push_back(book);

  book = prototype;
  book.nameRaw = "Art Book";
  book.name = book.nameRaw;
  book.bonusSkill.art = 10;
  mBooks.push_back(book);

  book = prototype;
  book.nameRaw = "Fashion Book";
  book.name = book.nameRaw;
  book.bonusSkill.tailor = 10;
  mBooks.push_back(book);

  book = prototype;
  book.nameRaw = "Stone Identification Guide";
  book.name = book.nameRaw;
  book.bonusSkill.stoneWorking = 10;
  mBooks.push_back(book);

  book = prototype;
  book.nameRaw = "Woodcrafting Guide";
  book.name = book.nameRaw;
  book.bonusSkill.woodWorking = 10;
  mBooks.push_back(book);

  book = prototype;
  book.nameRaw = "Metalcrafting Guide";
  book.name = book.nameRaw;
  book.bonusSkill.metalworking = 10;
  mBooks.push_back(book);

  book = prototype;
  book.nameRaw = "Farming Guide";
  book.name = book.nameRaw;
  book.bonusSkill.farming = 10;
  mBooks.push_back(book);

  book = prototype;
  book.nameRaw = "Fishing Guide";
  book.name = book.nameRaw;
  book.bonusSkill.fishing = 10;
  mBooks.push_back(book);

  book = prototype;
  book.nameRaw = "Origami Guide";
  book.name = book.nameRaw;
  book.bonusSkill.crafting = 10;
  mBooks.push_back(book);
}
