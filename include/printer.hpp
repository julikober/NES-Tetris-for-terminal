#ifndef PRINTER
#define PRINTER

#include <sys/ioctl.h>
#include <unistd.h>

#include <array>
#include <cstdint>
#include <iostream>
#include <vector>

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 224

class Printer {
 private:
  std::array<std::array<int32_t, SCREEN_WIDTH>, SCREEN_HEIGHT> _prev_frame;
  int _row;
  int _col;
  int32_t _color1;
  int32_t _color2;
  struct winsize _prev_winsize;

  void _print_color(int32_t color1, int32_t color2);

  Printer() : _row(0), _col(0), _color1(-1), _color2(-1), _prev_winsize({}) {};

 public:
  static Printer &getInstance() {
    static Printer instance;
    return instance;
  }

  Printer &operator=(Printer const &) = delete;
  Printer(Printer const &) = delete;

  void print(
      std::array<std::array<int32_t, SCREEN_WIDTH>, SCREEN_HEIGHT> &frame);
};
#endif /* PRINTER */
