#include "file_io.h"

#include <cctype>
#include <fstream>
#include <iterator>
#include <string>

namespace sudoku {
namespace {

bool CharToValue(char ch, int* value) {
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

bool LoadGridFromFile(const std::string& path, SudokuGrid* grid,
                      std::string* error) {
  if (grid == nullptr) {
    if (error) *error = "внутренняя ошибка: grid == nullptr";
    return false;
  }

  std::ifstream fin(path);
  if (!fin.is_open()) {
    if (error) *error = "не удалось открыть файл";
    return false;
  }

  std::string content((std::istreambuf_iterator<char>(fin)),
                      std::istreambuf_iterator<char>());

  SudokuGrid tmp;
  int count = 0;
  int r = 0;
  int c = 0;

  for (char ch : content) {
    if (std::isspace(static_cast<unsigned char>(ch))) continue;

    if (count >= SudokuGrid::kCellCount) {
      if (error) *error = "слишком много данных: нужно ровно 81 значение";
      return false;
    }

    int v = 0;
    if (!CharToValue(ch, &v)) {
      if (error) *error = "некорректный символ в файле (разрешено: 1-9, 0, .)";
      return false;
    }

    tmp.Set(r, c, v);
    ++count;

    ++c;
    if (c == SudokuGrid::kSize) {
      c = 0;
      ++r;
    }
  }

  if (count != SudokuGrid::kCellCount) {
    if (error) *error = "недостаточно данных: нужно ровно 81 значение";
    return false;
  }

  *grid = tmp;
  if (error) *error = "";
  return true;
}

bool SaveGridToFile(const std::string& path, const SudokuGrid& grid,
                    std::string* error) {
  std::ofstream fout(path);
  if (!fout.is_open()) {
    if (error) *error = "не удалось открыть файл на запись";
    return false;
  }

  for (int r = 0; r < SudokuGrid::kSize; ++r) {
    for (int c = 0; c < SudokuGrid::kSize; ++c) {
      const int v = grid.Get(r, c);
      fout << (v == 0 ? '.' : static_cast<char>('0' + v));
      if (c + 1 != SudokuGrid::kSize) fout << ' ';
    }
    fout << '\n';
  }

  if (!fout.good()) {
    if (error) *error = "ошибка записи в файл";
    return false;
  }

  if (error) *error = "";
  return true;
}

}  // namespace sudoku
