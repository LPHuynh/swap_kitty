#pragma once

#include <fstream>
#include <iostream>
#include <queue>
#include <string>


class Logging
{
public:
  Logging();
  ~Logging();

  void addToMainLog(std::string text);
  void changeDefaultStatusMessage(std::string text, std::string animation);
  void addStatusMessage(std::string text, std::string animation);
  std::string getStatusMessage();
  std::string getStatusAnimation();
  void popStatus();
  void addToProgress(std::string text);
  void clearProgress();
  void writeToFile(std::string text);

  std::deque<std::string> mainLog;
  std::string progress;

  bool isLogTextUpdated;

private:
  std::string mDefaultStatus;
  std::deque<std::string> mStatusMessages;
  std::deque<std::string> mStatusAnimations;
};

