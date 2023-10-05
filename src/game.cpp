#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>
#include <sys/ioctl.h>
#include <unistd.h>

#include "decoder.hpp"

using namespace std::chrono;

class Printer
{
private:
  int32_t _color1;
  int32_t _color2;

  void _print_color(int32_t color1, int32_t color2)
  {
    if (color1 == _color1 && color2 == _color2)
    {
      return;
    }

    if ((color1 == -1 || color2 == -1) && (_color1 != -1 && _color2 != -1))
    {
      std::cout << "\033[0m";
    }

    if (color1 != _color1)
    {
      std::cout << "\033[38;5;" << color1 << "m";
      _color1 = color1;
    }
    if (color2 != _color2)
    {
      std::cout << "\033[48;5;" << color2 << "m";

      _color2 = color2;
    }

    _color1 = color1;
    _color2 = color2;
  }

public:
  Printer(){};

  void print(int32_t color1, int32_t color2)
  {
    if (color1 == -1 && color2 == -1)
    {
      std::cout << " ";
    }
    else if (color1 == -1 || color2 == -1)
    {
      _print_color(color1, color2);
      if (color1 == -1)
      {
        std::cout << "▄";
      }
      else
      {
        std::cout << "▀";
      }
    }
    else if (color1 == color2)
    {
      _print_color(color1, color2);
      std::cout << "█";
    }
    else
    {
      _print_color(color1, color2);
      std::cout << "▀";
    }
  }
};

int main(void)
{
  std::vector<std::vector<std::int32_t>> background =
      decoder::decode("assets/background.bin");

  std::vector<std::vector<std::int32_t>> background2 =
      decoder::decode("assets/background2.bin");

  std::vector<std::vector<std::int32_t>> frame = background;

  std::vector<std::vector<std::int32_t>> prev_frame;

  Printer printer;
  struct winsize winsize;
  struct winsize prev_winsize;

  int row;
  int col;

  std::vector<int> times;

  for (int k = 0; k < 1000; k++)
  {
    auto start = high_resolution_clock::now();
    frame = k % 2 == 0 ? background : background2;
    ioctl(0, TIOCGWINSZ, &winsize);

    if (winsize.ws_col != prev_winsize.ws_col || winsize.ws_row != prev_winsize.ws_row)
    {
      // clear frame
      std::cout << "\033[H\033[0m";

      row = winsize.ws_row;
      col = winsize.ws_col;

      // print empty frame
      for (int i = 0; i < winsize.ws_row; i++)
      {
        for (int j = 0; j < winsize.ws_col; j++)
        {
          std::cout << " ";
        }
      }

      prev_frame =
          std::vector<std::vector<std::int32_t>>(
              frame.size(), std::vector<std::int32_t>(frame[0].size(), -1));
      prev_winsize = winsize;
    }

    for (int i = 0; i < frame.size(); i += 2)
    {
      for (int j = 0; j < frame[i].size(); j++)
      {
        std::int32_t color1 = frame[i][j];
        std::int32_t color2 = frame[i + 1][j];

        if (color1 == prev_frame[i][j] && color2 == prev_frame[i + 1][j])
        {
          continue;
        }

        prev_frame[i][j] = color1;
        prev_frame[i + 1][j] = color2;

        if (row < i / 2)
        {
          std::cout << "\033[" << (i / 2 - row) << "B";
        }

        if (row > i / 2)
        {
          std::cout << "\033[" << (row - i / 2) << "A";
        }

        if (col < j)
        {
          std::cout << "\033[" << (j - col) << "C";
        }

        if (col > j)
        {
          std::cout << "\033[" << (col - j) << "D";
        }

        if (col < winsize.ws_col)
        {
        printer.print(color1, color2);
        }

        col = j + 1;
        row = i / 2;
      }
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    times.push_back(duration.count());
  }
  std::cout << "\033[0m" << std::endl;
  std::cout << "Average: " << std::accumulate(times.begin(), times.end(), 0) / times.size() << "ms" << std::endl;
  std::cout << "Min: " << *std::min_element(times.begin(), times.end()) << "ms" << std::endl;
  std::cout << "Max: " << *std::max_element(times.begin(), times.end()) << "ms" << std::endl;
  return 0;
}
