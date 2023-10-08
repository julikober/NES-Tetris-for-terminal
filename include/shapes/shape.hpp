#ifndef SHAPE
#define SHAPE

#include <iostream>

#include "block.hpp"

#define SHAPE_SIZE_MATRIX 5

enum RotationDirection { clockwise, counterclockwise };

class Shape {
 private:
  const BlockType _type;
  const std::array<std::array<int, SHAPE_SIZE_MATRIX>, SHAPE_SIZE_MATRIX>
      _shape;
  const int _rotation_count;
  const int _rotation_direction;

  int _rotation = 0;

  std::array<int, 2> _position;

  std::vector<Block> _blocks;

 public:
  Shape(std::array<int, 2> position, BlockType type,
        std::array<std::array<int, SHAPE_SIZE_MATRIX>, SHAPE_SIZE_MATRIX> shape,
        int rotation_count, int rotation_direction)
      : _position(position),
        _type(type),
        _shape(shape),
        _rotation_count(rotation_count),
        _rotation_direction(rotation_direction) {
    std::cout << _type << std::endl;
    for (int i = 0; i < SHAPE_SIZE_MATRIX; i++) {
      for (int j = 0; j < SHAPE_SIZE_MATRIX; j++) {
        if (_shape[i][j] == 1) {
          _blocks.push_back(Block(_type, {i, j}));
        }
      }
    }
  };

  std::vector<Block> getBlocks() {
    std::vector<Block> blocks;
    for (auto &block : blocks) {
      blocks.push_back(
          Block(block.getType(), {block.getPosition()[0] + _position[0],
                                  block.getPosition()[1] + _position[1]}));
    }

    return blocks;
  }

  virtual ~Shape() = 0;
};

Shape::~Shape() {}

#endif /* SHAPE */
