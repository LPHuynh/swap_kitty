#include "logging.h"


Logging::Logging()
{
  std::string statusMessage = "";
  std::string progress = "";
  std::ofstream logFile;
  logFile.open("log.txt");
  logFile.close();
}

Logging::~Logging()
{
}

void Logging::addToMainLog(std::string text)
{
  mainLog.push(text);
  writeToFile(text);

  while (mainLog.size() > 1000)
  {
    mainLog.pop();
  }
}

void Logging::changeStatusMessage(std::string text)
{
  statusMessage = text;
}

void Logging::addToProgress(std::string text)
{
  progress += text;
}

void Logging::clearProgress()
{
  progress = "";
}

void Logging::writeToFile(std::string text)
{
  std::ofstream logFile;
  logFile.open("log.txt", std::ofstream::app);
  logFile << text << "\n";
  logFile.close();
}
