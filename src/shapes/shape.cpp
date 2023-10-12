#include "shape.hpp"

std::vector<Block> Shape::getBlocks()
{
  std::vector<Block> blocks;
  for (auto &block : _blocks)
  {
    blocks.push_back(
        Block(block.getType(), {block.getPosition()[0] + _position[0] - SHAPE_SIZE_MATRIX / 2,
                                block.getPosition()[1] + _position[1] - SHAPE_SIZE_MATRIX / 2}));
  }

  return blocks;
}

void Shape::resetBlocks()
{
  _blocks.clear();
  
  for (int i = 0; i < SHAPE_SIZE_MATRIX; i++)
  {
    for (int j = 0; j < SHAPE_SIZE_MATRIX; j++)
    {
      if (_shape[i][j] == 1)
      {
        _blocks.push_back(Block(_type, {j, i}));
      }
    }
  }
}

void Shape::rotate()
{
  if (++_rotation >= _rotation_count)
  {
    _rotation = 0;
    resetBlocks();
    return;
  }

  for (auto &block : _blocks)
  {
    int x = block.getPosition()[0];
    int y = block.getPosition()[1];

    if (_rotation_direction == clockwise)
    {
      block.setPosition({SHAPE_SIZE_MATRIX - 1 - y, x});
    }
    else
    {
      block.setPosition({y, SHAPE_SIZE_MATRIX - 1 - x});
    }
  }
}

void Shape::rotateBack()
{
  if (--_rotation < 0)
  {
    _rotation = 0;
    resetBlocks();
    for (int i = 0; i < _rotation_count - 1; i++)
    {
      rotate();
    }
    return;
  }

  for (auto &block : _blocks)
  {
    int x = block.getPosition()[0];
    int y = block.getPosition()[1];

    if (_rotation_direction == clockwise)
    {
      block.setPosition({y, SHAPE_SIZE_MATRIX - 1 - x});
    }
    else
    {
      block.setPosition({SHAPE_SIZE_MATRIX - 1 - y, x});
    }
  }
}

Shape::~Shape() {}