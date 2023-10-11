#ifndef T_SHAPE
#define T_SHAPE
#include "shape.hpp"

namespace {
struct TShapeData {
  static constexpr std::array<std::array<int, SHAPE_SIZE_MATRIX>,
                              SHAPE_SIZE_MATRIX>
      shape = {{
          {0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0},
          {0, 1, 1, 1, 0},
          {0, 0, 1, 0, 0},
          {0, 0, 0, 0, 0},
      }};
  static constexpr BlockType type = type1;
  static constexpr int rotation_count = 4;
  static constexpr int rotation_direction = clockwise;
};
}  // namespace

class TShape : public Shape {
 public:
  TShape(std::array<int, 2> position)
      : Shape(position, TShapeData::type, TShapeData::shape, TShapeData::rotation_count,
              TShapeData::rotation_direction){};
  virtual ~TShape(){};
};

#endif /* T_SHAPE */
