#include <sys/ioctl.h>
#include <unistd.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

#include "game.hpp"

using namespace std::chrono;

int main(void) {
  Game game = Game();
  game.print();

  return 0;
}
