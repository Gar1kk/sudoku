#include "sudoku_grid.h"

#include <sstream>
#include <string>

namespace sudoku {
namespace {

int BoxIndex(int row, int col) {
  return (row / 3) * 3 + (col / 3);
}

}  // namespace

SudokuGrid::SudokuGrid() : cells_{} {
  cells_.fill(0);
}

int SudokuGrid::Get(int row, int col) const {
  return cells_.at(row * kSize + col);
}

void SudokuGrid::Set(int row, int col, int value) {
  cells_.at(row * kSize + col) = value;
}

void SudokuGrid::Clear(int row, int col) {
  Set(row, col, 0);
}

bool SudokuGrid::IsComplete() const {
  for (int i = 0; i < kCellCount; ++i) {
    if (cells_[i] == 0) return false;
  }
  return true;
}

std::string SudokuGrid::ToCompactString() const {
  std::string out;
  out.reserve(kCellCount);
  for (int i = 0; i < kCellCount; ++i) {
    const int v = cells_[i];
    out.push_back(v == 0 ? '.' : static_cast<char>('0' + v));
  }
  return out;
}

std::string SudokuGrid::ToPrettyString() const {
  std::ostringstream oss;
  for (int r = 0; r < kSize; ++r) {
    if (r % 3 == 0) oss << "+-------+-------+-------+\n";
    for (int c = 0; c < kSize; ++c) {
      if (c % 3 == 0) oss << "| ";
      const int v = Get(r, c);
      oss << (v == 0 ? '.' : static_cast<char>('0' + v)) << ' ';
    }
    oss << "|\n";
  }
  oss << "+-------+-------+-------+";
  return oss.str();
}

bool IsGridValid(const SudokuGrid& grid, std::string* reason) {
  std::array<int, 9> row_mask{};
  std::array<int, 9> col_mask{};
  std::array<int, 9> box_mask{};
  row_mask.fill(0);
  col_mask.fill(0);
  box_mask.fill(0);

  for (int r = 0; r < SudokuGrid::kSize; ++r) {
    for (int c = 0; c < SudokuGrid::kSize; ++c) {
      const int v = grid.Get(r, c);
      if (v < 0 || v > 9) {
        if (reason) *reason = "значение вне диапазона 0..9";
        return false;
      }
      if (v == 0) continue;

      const int bit = 1 << v;
      const int b = BoxIndex(r, c);

      if ((row_mask[r] & bit) != 0) {
        if (reason) *reason = "повтор в строке " + std::to_string(r + 1);
        return false;
      }
      if ((col_mask[c] & bit) != 0) {
        if (reason) *reason = "повтор в столбце " + std::to_string(c + 1);
        return false;
      }
      if ((box_mask[b] & bit) != 0) {
        if (reason) *reason = "повтор в блоке 3x3 #" + std::to_string(b + 1);
        return false;
      }

      row_mask[r] |= bit;
      col_mask[c] |= bit;
      box_mask[b] |= bit;
    }
  }

  if (reason) *reason = "";
  return true;
}

}  // namespace sudoku
