#ifndef O_SHAPE
#define O_SHAPE
#include "shape.hpp"

namespace {
struct OShapeData {
  static constexpr std::array<std::array<int, SHAPE_SIZE_MATRIX>,
                              SHAPE_SIZE_MATRIX>
      shape = {{
          {0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0},
          {0, 1, 1, 0, 0},
          {0, 1, 1, 0, 0},
          {0, 0, 0, 0, 0},
      }};
  static constexpr BlockType type = type1;
  static constexpr int rotation_count = 1;
  static constexpr int rotation_direction = clockwise;
};
}  // namespace

class OShape : public Shape {
 public:
  OShape(std::array<int, 2> position)
      : Shape(position, OShapeData::type, OShapeData::shape, OShapeData::rotation_count,
              OShapeData::rotation_direction){};
  virtual ~OShape(){};
};

#endif /* O_SHAPE */
