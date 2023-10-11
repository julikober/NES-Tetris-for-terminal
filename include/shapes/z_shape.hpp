#ifndef Z_SHAPE
#define Z_SHAPE
#include "shape.hpp"

namespace {
struct ZShapeData {
  static constexpr std::array<std::array<int, SHAPE_SIZE_MATRIX>,
                              SHAPE_SIZE_MATRIX>
      shape = {{
          {0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0},
          {0, 1, 1, 0, 0},
          {0, 0, 1, 1, 0},
          {0, 0, 0, 0, 0},
      }};
  static constexpr BlockType type = type2;
  static constexpr int rotation_count = 2;
  static constexpr int rotation_direction = counterclockwise;
};
}  // namespace

class ZShape : public Shape {
 public:
  ZShape(std::array<int, 2> position)
      : Shape(position, ZShapeData::type, ZShapeData::shape, ZShapeData::rotation_count,
              ZShapeData::rotation_direction){};
  virtual ~ZShape(){};
};

#endif /* Z_SHAPE */
