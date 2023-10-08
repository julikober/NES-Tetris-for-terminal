#ifndef DECODER
#define DECODER

#include <cstdint>
#include <string>
#include <vector>

namespace decoder {
class Data {
 private:
  std::uint8_t *data;
  const std::size_t size;

 public:
  std::size_t bit_pos;

  Data(std::uint8_t *data, const std::size_t size)
      : data(data), size(size), bit_pos(0) {}

  std::uint8_t getBit();

  uint32_t getBits(int count);
};

std::vector<std::vector<std::int32_t>> decode(std::string file_path);
}  // namespace decoder

#endif /* DECODER */
