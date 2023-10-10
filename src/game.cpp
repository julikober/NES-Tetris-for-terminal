#include "game.hpp"

#include <sys/ioctl.h>
#include <unistd.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

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

std::map<BlockType, std::array<std::array<int32_t, BLOCK_SIZE>, BLOCK_SIZE>>
Game::_getLevelColors()
{
  std::map<BlockType, std::array<std::array<int32_t, BLOCK_SIZE>, BLOCK_SIZE>>
      level_colors;
  int current_level = 0;
  for (int i = 0; i < _color_palette.size(); i += BLOCK_SIZE)
  {
    for (int j = 0; j < _color_palette[i].size(); j += BLOCK_SIZE * 3)
    {
      if (current_level == _level)
      {
        for (int type = 0; type < 3; type++)
        {
          for (int i2 = 0; i2 < BLOCK_SIZE; i2++)
          {
            for (int j2 = 0; j2 < BLOCK_SIZE; j2++)
            {
              level_colors[static_cast<BlockType>(type)][i2][j2] =
                  _color_palette[i + i2][j + j2 + type * BLOCK_SIZE];
            }
          }
        }
        break;
      }
      current_level++;
    }
    if (current_level == _level)
    {
      break;
    }
  }
  return level_colors;
}

void Game::_loadBackground(std::string file_path)
{
  std::vector<std::vector<std::int32_t>> background_data =
      decoder::decode(file_path);

  for (int i = 0; i < background_data.size(); i++)
  {
    for (int j = 0; j < background_data[i].size(); j++)
    {
      _screen_pixels[i][j] = background_data[i][j];
    }
  }
}

void Game::clearField()
{
  // Clear field on screen
  for (int i = 0; i < FIELD_BLOCK_HEIGHT * BLOCK_SIZE; i++)
  {
    for (int j = 0; j < FIELD_BLOCK_WIDTH * BLOCK_SIZE; j++)
    {
      _screen_pixels[i + FIELD_PIXEL_OFFSET_Y][j + FIELD_PIXEL_OFFSET_X] = -1;
    }
  }
}

void Game::init()
{
  _loadBackground(_background_file_path);
  _color_palette = decoder::decode(_level_colors_file_path);
  _current_shape = std::make_unique<JShape>(std::array<int, 2>{0, 0});
  _next_shape = std::make_unique<JShape>(
      std::array<int, 2>{0, 0});
}

void Game::_insertBlocks(std::vector<Block> blocks)
{
  std::map<BlockType, std::array<std::array<int32_t, BLOCK_SIZE>, BLOCK_SIZE>>
      level_colors = _getLevelColors();

  for (Block &block : blocks)
  {
    auto pixels = block.getPixels(level_colors);
    for (int i = 0; i < BLOCK_SIZE; i++)
    {
      for (int j = 0; j < BLOCK_SIZE; j++)
      {
        if (pixels[i][j] == -1)
        {
          continue;
        }
        _screen_pixels[block.getPosition()[1] * BLOCK_SIZE + i + FIELD_PIXEL_OFFSET_Y][block.getPosition()[0] * BLOCK_SIZE + j + FIELD_PIXEL_OFFSET_X] = pixels[i][j];
      }
    }
  }
}

void Game::print()
{
  std::array<std::array<std::int32_t, SCREEN_WIDTH>, SCREEN_HEIGHT> prev_frame;

  Printer printer;
  struct winsize winsize;
  struct winsize prev_winsize;

  int row;
  int col;

  std::vector<int> times;

  std::cout << "\033[?25l";

  // for (int k = 0; k < 1000; k++) {
  //   frame = k % 2 == 0 ? background : background2;
  ioctl(0, TIOCGWINSZ, &winsize);

  if (winsize.ws_col != prev_winsize.ws_col ||
      winsize.ws_row != prev_winsize.ws_row)
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
    
    // clear prev_frame
    for (int i = 0; i < winsize.ws_row; i++)
    {
      for (int j = 0; j < winsize.ws_col; j++)
      {
        prev_frame[i][j] = -1;
      }
    }
    prev_winsize = winsize;
  }
  for (int i = 0; i < _screen_pixels.size(); i += 2)
  {
    for (int j = 0; j < _screen_pixels[i].size(); j++)
    {
      std::int32_t color1 = _screen_pixels[i][j];
      std::int32_t color2 = _screen_pixels[i + 1][j];

      if (j >= winsize.ws_col - 1)
      {
        break;
      }
      if (color1 == prev_frame[i][j] && color2 == prev_frame[i + 1][j])
      {
        continue;
      }

      prev_frame[i][j] = color1;
      prev_frame[i + 1][j] = color2;

      if (row > i / 2)
      {
        std::cout << "\033[" << (row - i / 2) << "A";
      }

      if (row < i / 2)
      {
        std::cout << "\033[" << (i / 2 - row) << "B";
      }

      if (col < j)
      {
        std::cout << "\033[" << (j - col) << "C";
      }

      if (col > j)
      {
        std::cout << "\033[" << (col - j) << "D";
      }

      printer.print(color1, color2);

      col = j + 1;
      row = i / 2;
    }
  }
  // }
  std::cout << "\033[0m" << std::endl;
  std::cout << "\033[?25h";
  std::cout << "Average: "
            << std::accumulate(times.begin(), times.end(), 0) / times.size()
            << "ms" << std::endl;
  std::cout << "Min: " << *std::min_element(times.begin(), times.end()) << "ms"
            << std::endl;
  std::cout << "Max: " << *std::max_element(times.begin(), times.end()) << "ms"
            << std::endl;
}

bool Game::_isColliding(std::unique_ptr<Shape> &shape)
{
  for (auto &block : shape->getBlocks())
  {
    if (block.getPosition()[0] < 0 || block.getPosition()[0] >= FIELD_BLOCK_WIDTH ||
        block.getPosition()[1] >= FIELD_BLOCK_HEIGHT)
    {
      return true;
    }
  }
  return false;
}

void Game::moveDown()
{
  _current_shape->moveDown();
  if (_isColliding(_current_shape))
  {
    _current_shape->moveUp();
  }
}

void Game::moveLeft()
{
  _current_shape->moveLeft();
  if (_isColliding(_current_shape))
  {
    _current_shape->moveRight();
  }
}

void Game::moveRight()
{
  _current_shape->moveRight();
  if (_isColliding(_current_shape))
  {
    _current_shape->moveLeft();
  }
}

void Game::rotate()
{
  _current_shape->rotate();
  if (_isColliding(_current_shape))
  {
    _current_shape->rotateBack();
  }
}

void Game::rotateBack()
{
  _current_shape->rotateBack();
  if (_isColliding(_current_shape))
  {
    _current_shape->rotate();
  }
}