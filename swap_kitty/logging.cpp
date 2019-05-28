#include "logging.h"


Logging::Logging()
{
  statusMessage = "";
  progress = "";
  isLogTextUpdated = false;
  std::ofstream logFile;
  logFile.open("log.txt");
  logFile.close();
}

Logging::~Logging()
{
}

void Logging::addToMainLog(std::string text)
{
  mainLog.push_back(text);
  writeToFile(text);

  while (mainLog.size() > 1000)
  {
    mainLog.pop_front();
  }
  isLogTextUpdated = true;
}

void Logging::changeStatusMessage(std::string text)
{
  statusMessage = text;
}

void Logging::addToProgress(std::string text)
{
  progress += text;
  isLogTextUpdated = true;
}

void Logging::clearProgress()
{
  progress = "";
  isLogTextUpdated = true;
}

void Logging::writeToFile(std::string text)
{
  std::ofstream logFile;
  logFile.open("log.txt", std::ofstream::app);
  logFile << text << "\n";
  logFile.close();
}
