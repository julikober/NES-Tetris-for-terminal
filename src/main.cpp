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
std::mutex mtx;
bool is_running = true;
Game game;

void doInput() {
  while (is_running) {
    char c = getchar();
    if (c == 'q') {
      is_running = false;
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

  std::cout << "\033[2J" << std::flush;

  std::vector<int> times;
  auto start = high_resolution_clock::now();

  int frame_count = 0;

  std::thread input_thread(doInput);

  while (is_running) {
    game.setLevel(game.getLineClearCount() / 10);

    game.clearField();
    game.drawField();

    mtx.lock();
    game.drawShape();
    mtx.unlock();

    game.print();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    times.push_back(duration.count());
    std::this_thread::sleep_for(milliseconds(16) - duration);

    start = high_resolution_clock::now();

    if (++frame_count >= 48) {
      if (game.isOnGround()) {
        if (game.isColliding()) {
          break;
        }
        game.spawnShape();
      } else {
        game.moveDown();
      }
      frame_count = 0;
    }

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
        std::this_thread::sleep_for(milliseconds(16 * 3));
      }

      for (auto &line : full_lines) {
        game.moveBlocksDown(line);
      }

      game.addLineClearCount(full_lines.size());
    }
  }

  is_running = false;
  input_thread.join();

  old.c_lflag |= ICANON;
  old.c_lflag |= ECHO;
  old.c_cc[VMIN] = 1;
  old.c_cc[VTIME] = 0;
  tcsetattr(0, TCSANOW, &old);

  return 0;
}
