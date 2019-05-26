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
  void changeStatusMessage(std::string text);
  void addToProgress(std::string text);
  void clearProgress();
  void writeToFile(std::string text);

  std::queue<std::string> mainLog;
  std::string statusMessage;
  std::string progress;
private:
};

