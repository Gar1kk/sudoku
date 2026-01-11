#include "console_ui.h"

#include <cctype>
#include <iostream>
#include <sstream>
#include <string>

namespace console_ui {
namespace {

bool ParseIntNoThrow(const std::string& s, int* out) {
  if (out == nullptr) return false;

  std::istringstream iss(s);
  iss >> std::ws;

  int value = 0;
  if (!(iss >> value)) return false;

  iss >> std::ws;
  if (iss.peek() != std::char_traits<char>::eof()) return false;

  *out = value;
  return true;
}

bool CharToCellValue(char ch, int* value) {
  if (value == nullptr) return false;

  if (ch == '.' || ch == '0') {
    *value = 0;
    return true;
  }
  if (ch >= '1' && ch <= '9') {
    *value = ch - '0';
    return true;
  }
  return false;
}

}  // namespace

bool ReadLine(const std::string& prompt, std::string* out) {
  if (out == nullptr) return false;

  std::cout << prompt;
  std::string line;
  if (!std::getline(std::cin, line)) return false;

  *out = line;
  return true;
}

bool ReadIntInRange(const std::string& prompt, int min_value, int max_value,
                    int* out) {
  if (out == nullptr) return false;

  while (true) {
    std::string line;
    if (!ReadLine(prompt, &line)) return false;

    int value = 0;
    if (!ParseIntNoThrow(line, &value)) {
      std::cout << "Введите целое число.\n";
      continue;
    }
    if (value < min_value || value > max_value) {
      std::cout << "Введите число в диапазоне [" << min_value << ".."
                << max_value << "].\n";
      continue;
    }
    *out = value;
    return true;
  }
}

bool ReadGridFromStdin(sudoku::SudokuGrid* grid) {
  if (grid == nullptr) return false;

  sudoku::SudokuGrid tmp;

  for (int r = 0; r < sudoku::SudokuGrid::kSize; ++r) {
    std::string line;
    if (!ReadLine("Строка " + std::to_string(r + 1) + ": ", &line)) return false;

    int c = 0;
    for (char ch : line) {
      if (std::isspace(static_cast<unsigned char>(ch))) continue;

      if (c >= sudoku::SudokuGrid::kSize) {
        // Лишние значимые символы в строке.
        return false;
      }

      int v = 0;
      if (!CharToCellValue(ch, &v)) return false;

      tmp.Set(r, c, v);
      ++c;
    }

    if (c != sudoku::SudokuGrid::kSize) {
      // Недостаточно значимых символов.
      return false;
    }
  }

  *grid = tmp;
  return true;
}

}  // namespace console_ui
