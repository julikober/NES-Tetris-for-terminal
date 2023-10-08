#ifndef J_SHAPE
#define J_SHAPE
#include "shape.hpp"

namespace {
struct JShapeData {
  static constexpr std::array<std::array<int, SHAPE_SIZE_MATRIX>,
                              SHAPE_SIZE_MATRIX>
      shape = {{
          {0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0},
          {0, 1, 1, 1, 0},
          {0, 0, 0, 1, 0},
          {0, 0, 0, 0, 0},
      }};
  static constexpr BlockType type = type2;
  static constexpr int rotation_count = 4;
  static constexpr int rotation_direction = clockwise;
};
}  // namespace

class JShape : public Shape {
 public:
  JShape(std::array<int, 2> position)
      : Shape(position, JShapeData::type, JShapeData::shape, JShapeData::rotation_count,
              JShapeData::rotation_direction){};
  virtual ~JShape(){};
};

#endif /* J_SHAPE */
