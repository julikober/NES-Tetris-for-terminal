#ifndef S_SHAPE
#define S_SHAPE
#include "shape.hpp"

namespace {
struct SShapeData {
  static constexpr std::array<std::array<int, SHAPE_SIZE_MATRIX>,
                              SHAPE_SIZE_MATRIX>
      shape = {{
          {0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0},
          {0, 0, 1, 1, 0},
          {0, 1, 1, 0, 0},
          {0, 0, 0, 0, 0},
      }};
  static constexpr BlockType type = type3;
  static constexpr int rotation_count = 2;
  static constexpr int rotation_direction = counterclockwise;
};
}  // namespace

class SShape : public Shape {
 public:
  SShape(std::array<int, 2> position)
      : Shape(position, SShapeData::type, SShapeData::shape, SShapeData::rotation_count,
              SShapeData::rotation_direction){};
  virtual ~SShape(){};
};

#endif /* S_SHAPE */
