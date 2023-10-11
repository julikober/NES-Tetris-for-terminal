#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <mutex>
#include <numeric>
#include <thread>
#include <vector>

#include "game.hpp"

using namespace std::chrono;
struct termios t;
std::mutex mtx;
Game game;

void doInput() {
  while (true) {
    char c = getchar();
    if (c == 'q') {
      break;
    } else if (c == 27) {
      getchar();
      switch (getchar()) {
        case 66:
          mtx.lock();
          game.moveDown();
          mtx.unlock();
          break;
        case 67:
          mtx.lock();
          game.moveRight();
          mtx.unlock();
          break;
        case 68:
          mtx.lock();
          game.moveLeft();
          mtx.unlock();
          break;
      }
    } else if (c == 'x') {
      game.rotate();
    } else if (c == 'y') {
      game.rotateBack();
    }

    if (game.isOnGround()) {
      game.spawnShape();
    }
  }
}

int main(void) {
  struct termios old = {0};
  tcgetattr(0, &old);
  old.c_lflag &= ~ICANON;
  old.c_lflag &= ~ECHO;
  old.c_cc[VMIN] = 1;
  old.c_cc[VTIME] = 0;
  tcsetattr(0, TCSANOW, &old);

  game.init();
  std::cout << "\033[2J" << std::flush;

  std::vector<int> times;
  auto start = high_resolution_clock::now();

  std::thread input_thread(doInput);

  while (true) {
    game.clearField();
    game.drawField();

    mtx.lock();
    game.drawShape();
    mtx.unlock();

    game.print();

    std::vector<int> full_lines = game.getFullLines();
    if (full_lines.size() > 0) {
      for (int i = 0; i < FIELD_BLOCK_WIDTH / 2; i++) {
        for (auto &line : full_lines) {
          game.removeBlockAt(i + FIELD_BLOCK_WIDTH / 2, line);
          game.removeBlockAt(FIELD_BLOCK_WIDTH / 2 - i - 1, line);
        }

        game.clearField();
        game.drawField();
        game.print();
        std::this_thread::sleep_for(milliseconds(16));
      }

      for (auto &line : full_lines) {
        game.moveBlocksDown(line);
      }
    }
  }

  std::cout << "\033[0m\033[2J" << std::flush;
  std::cout << "Average FPS: "
            << 1000000 / (std::accumulate(times.begin(), times.end(), 0) /
                          times.size())
            << std::endl;
  std::cout << "Average time: "
            << std::accumulate(times.begin(), times.end(), 0) / times.size()
            << std::endl;
  std::cout << "Max time: " << *std::max_element(times.begin(), times.end())
            << std::endl;
  std::cout << "Min time: " << *std::min_element(times.begin(), times.end())
            << std::endl;

  return 0;
}
