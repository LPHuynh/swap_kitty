#include "world.h"


World::World()
{
}

World::~World()
{
}

bool World::init(std::string daemonHost, uint16_t daemonPort, uint16_t walletPort)
{
  if (!daemonAPI.init(daemonHost, daemonPort))
  {
    return false;
  }
  if (!walletAPI.init(daemonHost, daemonPort, walletPort))
  {
    return false;
  }
  return true;
}