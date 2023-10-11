#ifndef I_SHAPE
#define I_SHAPE
#include "shape.hpp"

namespace {
struct IShapeData {
  static constexpr std::array<std::array<int, SHAPE_SIZE_MATRIX>,
                              SHAPE_SIZE_MATRIX>
      shape = {{
          {0, 0, 1, 0, 0},
          {0, 0, 1, 0, 0},
          {0, 0, 1, 0, 0},
          {0, 0, 1, 0, 0},
          {0, 0, 0, 0, 0},
      }};
  static constexpr BlockType type = type1;
  static constexpr int rotation_count = 2;
  static constexpr int rotation_direction = counterclockwise;
};
}  // namespace

class IShape : public Shape {
 public:
  IShape(std::array<int, 2> position)
      : Shape(position, IShapeData::type, IShapeData::shape, IShapeData::rotation_count,
              IShapeData::rotation_direction){};
  virtual ~IShape(){};
};

#endif /* I_SHAPE */
