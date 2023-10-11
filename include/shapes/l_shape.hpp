#ifndef L_SHAPE
#define L_SHAPE
#include "shape.hpp"

namespace {
struct LShapeData {
  static constexpr std::array<std::array<int, SHAPE_SIZE_MATRIX>,
                              SHAPE_SIZE_MATRIX>
      shape = {{
          {0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0},
          {0, 1, 1, 1, 0},
          {0, 1, 0, 0, 0},
          {0, 0, 0, 0, 0},
      }};
  static constexpr BlockType type = type2;
  static constexpr int rotation_count = 4;
  static constexpr int rotation_direction = clockwise;
};
}  // namespace

class LShape : public Shape {
 public:
  LShape(std::array<int, 2> position)
      : Shape(position, LShapeData::type, LShapeData::shape, LShapeData::rotation_count,
              LShapeData::rotation_direction){};
  virtual ~LShape(){};
};

#endif /* L_SHAPE */
