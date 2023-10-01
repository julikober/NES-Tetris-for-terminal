#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "decoder.hpp"

enum PixelType { Empty = 0, Color = 1 };

std::vector<std::vector<std::int32_t>> decoder::decode(
    std::string file_path) {
  std::ifstream file(file_path, std::ios::binary | std::ios::in);
  int file_size;

  file.seekg(0, std::ios::end);
  file_size = file.tellg();
  file.seekg(0, std::ios::beg);

  std::uint8_t *c = new std::uint8_t[file_size];
  file.read((char *)c, file_size);

  file.close();

  Data data = Data(c, file_size);

  std::uint32_t col_count = data.getBits(16);
  std::uint8_t color_bit_length = data.getBits(8);

  std::vector<std::vector<std::int32_t>> pixels;
  std::vector<std::vector<bool>> is_used;

  size_t row = 0;
  size_t col = 0;

  while (data.bit_pos < file_size * 8 - 8) {
    PixelType type = (PixelType)data.getBit();
    std::uint8_t size_bit_length = data.getBits(3) * 4;

    std::int32_t color;
    if (type == PixelType::Empty) {
      color = -1;
    } else if (type == PixelType::Color) {
      color = data.getBits(color_bit_length);
    }

    uint16_t pixel_row_count;
    uint16_t pixel_col_count;
    if (size_bit_length == 0) {
      pixel_row_count = 1;
      pixel_col_count = 1;
    } else {
      pixel_row_count = data.getBits(size_bit_length / 2);
      pixel_col_count = data.getBits(size_bit_length / 2);
    }

    for (int i = 0; i < pixel_row_count; i++) {
      if (row + i >= pixels.size()) {
        pixels.push_back(std::vector<std::int32_t>(col_count, -1));
        is_used.push_back(std::vector<bool>(col_count, false));
      }
      for (int j = 0; j < pixel_col_count; j++) {
        pixels[row + i][col + j] = color;
        is_used[row + i][col + j] = true;
      }
    }

    while (row < pixels.size() && is_used[row][col] == true) {
      col += 1;
      if (col >= col_count) {
        col = 0;
        row += 1;
      }
    }
  }
  
  return pixels;

  //   for (int i = 0; i < pixels.size(); i += 2)
  //   {
  //     for (int j = 0; j < pixels[i].size(); j++)
  //     {
  //       std::int32_t color1 = pixels[i][j];
  //       std::int32_t color2 = pixels[i + 1][j];

  //       if (color1 == -1 && color2 == -1)
  //       {
  //         std::cout << "\033[0m"
  //                   << " ";
  //       }
  //       else if (color1 == -1 || color2 == -1)
  //       {
  //         std::cout << "\033[0m";

  //         if (color1 == -1)
  //         {
  //           std::cout << "\033[38;5;" << color2 << "m"
  //                     << "▄";
  //         }
  //         else
  //         {
  //           std::cout << "\033[38;5;" << color1 << "m"
  //                     << "▀";
  //         }
  //       }
  //       else if (color1 == color2)
  //       {
  //         std::cout << "\033[0m\033[38;5;" << color1 << ";48;5;" << (color1)
  //         << "m"
  //                   << "█";
  //       }
  //       else
  //       {
  //         std::cout << "\033[0m\033[38;5;" << color1 << ";48;5;" << (color2)
  //         << "m"
  //                   << "▀";
  //       }
  //     }
  //     std::cout << "\033[0m" << std::endl;
  //   }
}