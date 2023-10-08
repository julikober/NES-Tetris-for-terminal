#ifndef GAME
#define GAME

#include <array>
#include <string>
#include <vector>

#include "block.hpp"
#include "decoder.hpp"

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 224

#define FIELD_BLOCK_WIDTH 10
#define FIELD_BLOCK_HEIGHT 20
#define HIDDEN_FIELD_BLOCK_HEIGHT 2

#define FIELD_PIXEL_OFFSET_X 96
#define FIELD_PIXEL_OFFSET_Y 40

class Game {
 private:
  const std::string _background_file_path = "assets/background.bin";
  std::vector<Block> _blocks;
  std::array<std::array<int32_t, SCREEN_WIDTH>, SCREEN_HEIGHT> _pixels;

  void _loadBackground(std::string file_path);

 public:
  Game() { _loadBackground(_background_file_path); };
  void print();
};

#endif /* GAME */
