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

std::map<BlockType, std::array<std::array<int32_t, BLOCK_SIZE>, BLOCK_SIZE>>
Game::_getLevelColors() {
  std::map<BlockType, std::array<std::array<int32_t, BLOCK_SIZE>, BLOCK_SIZE>>
      level_colors;
  int current_level = 0;
  for (int i = 0; i < _color_palette.size(); i += BLOCK_SIZE) {
    for (int j = 0; j < _color_palette[i].size(); j += BLOCK_SIZE * 3) {
      if (current_level == _level) {
        for (int type = 0; type < 3; type++) {
          for (int i2 = 0; i2 < BLOCK_SIZE; i2++) {
            for (int j2 = 0; j2 < BLOCK_SIZE; j2++) {
              level_colors[static_cast<BlockType>(type)][i2][j2] =
                  _color_palette[i + i2][j + j2 + type * BLOCK_SIZE];
            }
          }
        }
        break;
      }
      current_level++;
    }
    if (current_level == _level) {
      break;
    }
  }
  return level_colors;
}

void Game::_loadBackground(std::string file_path) {
  std::vector<std::vector<std::int32_t>> background_data =
      decoder::decode(file_path);

  for (int i = 0; i < background_data.size(); i++) {
    for (int j = 0; j < background_data[i].size(); j++) {
      _background_pixels[i][j] = background_data[i][j];
    }
  }

  _screen_pixels = _background_pixels;
}

void Game::init() {
  _loadBackground(_background_file_path);
  _color_palette = decoder::decode(_level_colors_file_path);
  spawnShape();
}

void Game::_insertBlocks(std::vector<Block> blocks) {
  std::map<BlockType, std::array<std::array<int32_t, BLOCK_SIZE>, BLOCK_SIZE>>
      level_colors = _getLevelColors();

  for (Block &block : blocks) {
    auto pixels = block.getPixels(level_colors);
    for (int i = 0; i < BLOCK_SIZE; i++) {
      for (int j = 0; j < BLOCK_SIZE; j++) {
        if (pixels[i][j] == -1) {
          continue;
        }
        _screen_pixels[block.getPosition()[1] * BLOCK_SIZE + i +
                       FIELD_PIXEL_OFFSET_Y]
                      [block.getPosition()[0] * BLOCK_SIZE + j +
                       FIELD_PIXEL_OFFSET_X] = pixels[i][j];
      }
    }
  }
}

std::unique_ptr<Shape> Game::_generateShape() {
  std::unique_ptr<Shape> shape;
  int random = rand() % 7;
  switch (random) {
    case 0:
      shape = std::make_unique<IShape>(std::array<int, 2>{4, 0});
      break;
    case 1:
      shape = std::make_unique<JShape>(std::array<int, 2>{4, 0});
      break;
    case 2:
      shape = std::make_unique<LShape>(std::array<int, 2>{4, 0});
      break;
    case 3:
      shape = std::make_unique<OShape>(std::array<int, 2>{4, 0});
      break;
    case 4:
      shape = std::make_unique<SShape>(std::array<int, 2>{4, 0});
      break;
    case 5:
      shape = std::make_unique<TShape>(std::array<int, 2>{4, 0});
      break;
    case 6:
      shape = std::make_unique<ZShape>(std::array<int, 2>{4, 0});
      break;
  }

  return std::move(shape);
}

void Game::print() {
  Printer *printer = &Printer::getInstance();
  printer->print(_screen_pixels);
}

bool Game::_isColliding(std::unique_ptr<Shape> &shape) {
  for (auto &block : shape->getBlocks()) {
    if (block.getPosition()[0] < 0 ||
        block.getPosition()[0] >= FIELD_BLOCK_WIDTH ||
        block.getPosition()[1] >= FIELD_BLOCK_HEIGHT) {
      return true;
    }

    for (auto &other_block : _blocks) {
      if (block.getPosition()[0] == other_block.getPosition()[0] &&
          block.getPosition()[1] == other_block.getPosition()[1]) {
        return true;
      }
    }
  }
  return false;
}

std::vector<int> Game::getFullLines() {
  std::vector<int> full_lines;
  for (int i = 0; i < FIELD_BLOCK_HEIGHT; i++) {
    bool is_full = true;
    for (int j = 0; j < FIELD_BLOCK_WIDTH; j++) {
      bool is_block = false;
      for (auto &block : _blocks) {
        if (block.getPosition()[0] == j && block.getPosition()[1] == i) {
          is_block = true;
          break;
        }
      }
      if (!is_block) {
        is_full = false;
        break;
      }
    }
    if (is_full) {
      full_lines.push_back(i);
    }
  }
  return full_lines;
}

void Game::removeBlockAt(int x, int y) {
  _blocks.erase(std::remove_if(_blocks.begin(), _blocks.end(),
                               [x, y](Block &block) {
                                 return block.getPosition()[0] == x &&
                                        block.getPosition()[1] == y;
                               }),
                _blocks.end());
}

void Game::moveBlocksDown(int y) {
  for (auto &block : _blocks) {
    if (block.getPosition()[1] < y) {
      block.moveDown();
    }
  }
}

void Game::clearField() {
  for (int i = 0; i < FIELD_BLOCK_HEIGHT * BLOCK_SIZE; i++) {
    for (int j = 0; j < FIELD_BLOCK_WIDTH * BLOCK_SIZE; j++) {
      _screen_pixels[i + FIELD_PIXEL_OFFSET_Y][j + FIELD_PIXEL_OFFSET_X] =
          _background_pixels[i + FIELD_PIXEL_OFFSET_Y]
                            [j + FIELD_PIXEL_OFFSET_X];
    }
  }
}

void Game::drawField() { _insertBlocks(_blocks); };

void Game::drawShape() { _insertBlocks(_current_shape->getBlocks()); };

void Game::moveDown() {
  _current_shape->moveDown();
  if (_isColliding(_current_shape)) {
    _current_shape->moveUp();
  }
}

bool Game::isOnGround() {
  _current_shape->moveDown();
  bool is_on_ground = _isColliding(_current_shape);
  _current_shape->moveUp();
  return is_on_ground;
}

void Game::spawnShape() {
  std::unique_ptr<Shape> shape = _generateShape();

  if (_current_shape != nullptr) {
    for (auto &block : _current_shape->getBlocks()) {
      _blocks.push_back(block);
    }
    _current_shape = std::move(_next_shape);
    _next_shape = std::move(shape);
  } else {
    _current_shape = std::move(shape);
    _next_shape = std::move(_generateShape());
  }
}

void Game::moveLeft() {
  _current_shape->moveLeft();
  if (_isColliding(_current_shape)) {
    _current_shape->moveRight();
  }
}

void Game::moveRight() {
  _current_shape->moveRight();
  if (_isColliding(_current_shape)) {
    _current_shape->moveLeft();
  }
}

void Game::rotate() {
  _current_shape->rotate();
  if (_isColliding(_current_shape)) {
    _current_shape->rotateBack();
  }
}

void Game::rotateBack() {
  _current_shape->rotateBack();
  if (_isColliding(_current_shape)) {
    _current_shape->rotate();
  }
}