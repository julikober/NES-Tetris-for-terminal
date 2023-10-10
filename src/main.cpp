#include <sys/ioctl.h>
#include <unistd.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>
#include <termios.h>
#include <iostream>

#include "game.hpp"

using namespace std::chrono;
struct termios t;

int main(void)
{
  tcgetattr(fileno(stdin), &t);
  t.c_lflag &= ~(ICANON);
  tcsetattr(fileno(stdin), TCSAFLUSH, &t);

  Game game = Game();
  game.init();

  while (true) {
    // game.clearField();
    // game.drawShape();
    // game.drawField();
    game.print();

    // char c = getchar();
    // if (c == 'q') {
    //   break;
    // } else if (c == 27) {
    //   getchar();
    //   switch (getchar()) {
    //     case 66:
    //       game.moveDown();
    //       break;
    //     case 67:
    //       game.moveRight();
    //       break;
    //     case 68:
    //       game.moveLeft();
    //       break;
    //   }
    // }
  }

  return 0;
}
