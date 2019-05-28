#include "logging.h"


Logging::Logging()
{
  mDefaultStatus = "";
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

void Logging::changeDefaultStatusMessage(std::string text, std::string animation)
{
  mDefaultStatus = text;
}

void Logging::addStatusMessage(std::string text, std::string animation)
{
  mStatusMessages.push_back(text);
  mStatusAnimations.push_back(animation);

  while (mStatusMessages.size() > 10)
  {
    popStatus();
  }
}

std::string Logging::getStatusMessage()
{
  if (mStatusMessages.empty())
  { 
    return mDefaultStatus;
  }
  else 
  {
    return mStatusMessages.front();
  }
}

std::string Logging::getStatusAnimation()
{
  if (mStatusAnimations.empty())
  {
    return mDefaultStatus;
  }
  else
  {
    return mStatusAnimations.front();
  }
}

void Logging::popStatus()
{
  if (!mStatusMessages.empty())
  {
    mStatusMessages.pop_front();
    mStatusAnimations.pop_front();
  }
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
