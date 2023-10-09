#ifndef SHAPE
#define SHAPE

#include <iostream>

#include "block.hpp"

#define SHAPE_SIZE_MATRIX 5

enum RotationDirection
{
  clockwise,
  counterclockwise
};

class Shape
{
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
        _rotation_direction(rotation_direction)
  {
    resetBlocks();
  };

  void resetBlocks();
  std::vector<Block> getBlocks();

  void rotate();
  void rotateBack();

  virtual ~Shape() = 0;
};

#endif /* SHAPE */
