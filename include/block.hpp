#ifndef BLOCK
#define BLOCK

#include <array>
#include <cstdint>
#include <map>
#include <vector>

#define BLOCK_SIZE 8

enum BlockType { type1, type2, type3 };

class Block {
 private:
  std::array<int, 2> _position;
  BlockType _type;

 public:
  Block(BlockType type, std::array<int, 2> position)
      : _type(type), _position(position){};

  BlockType getType() const { return _type; }
  std::array<int, 2> getPosition() const { return _position; }

  inline std::array<std::array<int32_t, BLOCK_SIZE>, BLOCK_SIZE> getPixels(
      std::map<BlockType,
               std::array<std::array<int32_t, BLOCK_SIZE>, BLOCK_SIZE>>
          types) {
    return types[_type];
  }

  inline void setPosition(std::array<int, 2> position) { _position = position; }

  inline void moveDown() { _position[1]++; }
  inline void moveUp() { _position[1]--; }
  inline void moveLeft() { _position[0]--; }
  inline void moveRight() { _position[0]++; }
};

#endif /* BLOCK */
