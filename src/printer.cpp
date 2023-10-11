#include "printer.hpp"
#include <thread>

void Printer::_print_color(int32_t color1, int32_t color2) {
  if (color1 == _color1 && color2 == _color2) {
    return;
  }

  if ((color1 == -1 || color2 == -1) && (_color1 != -1 && _color2 != -1)) {
    std::cout << "\033[0m" << std::flush;
  }

  if (color1 != _color1) {
    std::cout << "\033[38;5;" << color1 << "m" << std::flush;
  }
  if (color2 != _color2) {
    std::cout << "\033[48;5;" << color2 << "m" << std::flush;
  }

  _color1 = color1;
  _color2 = color2;
}

void Printer::print(
    std::array<std::array<int32_t, SCREEN_WIDTH>, SCREEN_HEIGHT> &frame) {
  struct winsize winsize;

  // for (int k = 0; k < 1000; k++) {
  //   frame = k % 2 == 0 ? background : background2;
  ioctl(0, TIOCGWINSZ, &winsize);

  if (winsize.ws_col != _prev_winsize.ws_col ||
      winsize.ws_row != _prev_winsize.ws_row) {
    // clear frame
    std::cout << "\033[H\033[0m" << std::flush;

    _color1 = -1;
    _color2 = -1;

    // print empty frame
    for (int i = 0; i < winsize.ws_row; i++) {
      for (int j = 0; j < winsize.ws_col; j++) {
        std::cout << " " << std::flush;
      }
    }

    // clear prev_frame
    for (int i = 0; i < frame.size(); i++) {
      for (int j = 0; j < frame[0].size(); j++) {
        _prev_frame[i][j] = -1;
      }
    }
    _prev_winsize = winsize;

    std::cout << "\033[H" << std::flush;
    _row = 0;
    _col = 0;
  }

  for (int i = 0; i < frame.size(); i += 2) {
    if (i / 2 >= winsize.ws_row - 1) {
      break;
    }
    for (int j = 0; j < frame[i].size(); j++) {
      std::int32_t color1 = frame[i][j];
      std::int32_t color2 = frame[i + 1][j];

      if (j >= winsize.ws_col - 1) {
        break;
      }

      if (color1 == _prev_frame[i][j] && color2 == _prev_frame[i + 1][j]) {
        continue;
      }

      _prev_frame[i][j] = color1;
      _prev_frame[i + 1][j] = color2;

      if (_row > i / 2) {
        std::cout << "\033[" << (_row - i / 2) << "A" << std::flush;
      }

      if (_row < i / 2) {
        std::cout << "\033[" << (i / 2 - _row) << "B" << std::flush;
      }

      if (_col < j) {
        std::cout << "\033[" << (j - _col) << "C" << std::flush;
      }

      if (_col > j) {
        std::cout << "\033[" << (_col - j) << "D" << std::flush;
      }

      if (color1 == -1 && color2 == -1) {
        std::cout << " " << std::flush;
      } else if (color1 == -1 || color2 == -1) {
        _print_color(color1, color2);
        if (color1 == -1) {
          std::cout << "▄" << std::flush;
        } else {
          std::cout << "▀" << std::flush;
        }
      } else if (color1 == color2) {
        _print_color(color1, color2);
        std::cout << "█" << std::flush;
      } else {
        _print_color(color1, color2);
        std::cout << "▀" << std::flush;
      }

      _col = j + 1;
      _row = i / 2;
    }
  }
}