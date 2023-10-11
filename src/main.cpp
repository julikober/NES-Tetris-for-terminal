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

char getch()
{
  char buf = 0;
  struct termios old = {0};
  if (tcgetattr(0, &old) < 0)
    perror("tcsetattr()");
  old.c_lflag &= ~ICANON;
  old.c_lflag &= ~ECHO;
  old.c_cc[VMIN] = 1;
  old.c_cc[VTIME] = 0;
  if (tcsetattr(0, TCSANOW, &old) < 0)
    perror("tcsetattr ICANON");
  if (read(0, &buf, 1) < 0)
    perror("read()");
  old.c_lflag |= ICANON;
  old.c_lflag |= ECHO;
  if (tcsetattr(0, TCSADRAIN, &old) < 0)
    perror("tcsetattr ~ICANON");
  return (buf);
}

int main(void)
{
  tcgetattr(fileno(stdin), &t);
  t.c_lflag &= ~(ICANON);
  tcsetattr(fileno(stdin), TCSAFLUSH, &t);

  Game game = Game();
  game.init();
  std::cout << "\033[2J" << std::flush;

  std::vector<int> times;
  auto start = high_resolution_clock::now();
  while (true)
  {
    game.clearField();
    game.drawField();
    game.drawShape();
    game.print();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    times.push_back(duration.count());

    char c = getch();
    start = high_resolution_clock::now();
    if (c == 'q')
    {
      break;
    }
    else if (c == 27)
    {
      getch();
      switch (getch())
      {
      case 66:
        game.moveDown();
        break;
      case 67:
        game.moveRight();
        break;
      case 68:
        game.moveLeft();
        break;
      }
    }
    else if (c == 'x')
    {
      game.rotate();
    }
    else if (c == 'y')
    {
      game.rotateBack();
    }

    if (game.isOnGround())
    {
      game.spawnShape();
    }
  }

  std::cout << "\033[0m\033[2J" << std::flush;
  std::cout << "Average FPS: " << 1000000 / (std::accumulate(times.begin(), times.end(), 0) / times.size()) << std::endl;
  std::cout << "Average time: " << std::accumulate(times.begin(), times.end(), 0) / times.size() << std::endl;
  std::cout << "Max time: " << *std::max_element(times.begin(), times.end()) << std::endl;
  std::cout << "Min time: " << *std::min_element(times.begin(), times.end()) << std::endl;

  return 0;
}
