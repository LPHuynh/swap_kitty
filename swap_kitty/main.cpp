#include "app.h"


int main()
{
  std::cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
  std::cout.precision(2);

  App app;
  app.run();

  return 0;
}