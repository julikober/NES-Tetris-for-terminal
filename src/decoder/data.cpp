#include <iostream>

#include "decoder.hpp"

std::uint8_t decoder::Data::getBit() {
  if (bit_pos >= size * 8) {
    throw std::runtime_error("Out of bounds");
  }

  std::uint8_t bit = (data[bit_pos / 8] >> (7 - (bit_pos % 8))) & 1;
  bit_pos++;
  return bit;
}

uint32_t decoder::Data::getBits(int count) {
  uint32_t result = 0;
  for (int i = 0; i < count; i++) {
    result |= getBit() << (count - i - 1);
  }
  return result;
}