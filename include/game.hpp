#ifndef GAME
#define GAME

#include <array>
#include <string>
#include <vector>
#include <memory>
#include <cstdint>

#include "block.hpp"
#include "decoder.hpp"
#include "shapes.hpp"
#include "printer.hpp"

#define FIELD_BLOCK_WIDTH 10
#define FIELD_BLOCK_HEIGHT 20
#define HIDDEN_FIELD_BLOCK_HEIGHT 2

#define FIELD_PIXEL_OFFSET_X 96
#define FIELD_PIXEL_OFFSET_Y 40

class Game
{
private:
  const std::string _background_file_path = "assets/background.bin";
  const std::string _level_colors_file_path = "assets/level_colors.bin";
  std::vector<Block> _blocks;
  std::unique_ptr<Shape> _current_shape;
  std::unique_ptr<Shape> _next_shape;
  std::array<std::array<int32_t, SCREEN_WIDTH>, SCREEN_HEIGHT> _background_pixels;
  std::array<std::array<int32_t, SCREEN_WIDTH>, SCREEN_HEIGHT> _screen_pixels;

  std::uint8_t _level;
  std::vector<std::vector<std::int32_t>> _color_palette;

  void _loadBackground(std::string file_path);
  std::map<BlockType, std::array<std::array<int32_t, BLOCK_SIZE>, BLOCK_SIZE>> _getLevelColors();
  void _insertBlocks(std::vector<Block> blocks);
  bool _isColliding(std::unique_ptr<Shape> &shape);
  void _generateShape();

public:
  Game(std::uint8_t level = 0) : _level(level){};
  void init();
  void print();

  void clearField();
  void drawField();
  void drawShape();

  bool isOnGround();
  void spawnShape();

  void moveDown();
  void moveLeft();
  void moveRight();

  void rotate();
  void rotateBack();
};

#endif /* GAME */
