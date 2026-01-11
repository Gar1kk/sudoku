#include "solver.h"

#include <array>
#include <vector>

namespace sudoku {
namespace {

int BoxIndex(int row, int col) {
  return (row / 3) * 3 + (col / 3);
}

// Маски строк/столбцов/блоков: бит v установлен, если цифра v уже присутствует.
// Это ускоряет проверки и делает итеративный бэктрекинг простым.
bool BuildMasks(const SudokuGrid& grid, std::array<int, 9>* row_mask,
                std::array<int, 9>* col_mask, std::array<int, 9>* box_mask) {
  row_mask->fill(0);
  col_mask->fill(0);
  box_mask->fill(0);

  for (int r = 0; r < SudokuGrid::kSize; ++r) {
    for (int c = 0; c < SudokuGrid::kSize; ++c) {
      const int v = grid.Get(r, c);
      if (v == 0) continue;

      const int bit = 1 << v;
      const int b = BoxIndex(r, c);

      if (((*row_mask)[r] & bit) != 0 || ((*col_mask)[c] & bit) != 0 ||
          ((*box_mask)[b] & bit) != 0) {
        return false;
      }

      (*row_mask)[r] |= bit;
      (*col_mask)[c] |= bit;
      (*box_mask)[b] |= bit;
    }
  }
  return true;
}

bool IsAllowed(const std::array<int, 9>& row_mask, const std::array<int, 9>& col_mask,
               const std::array<int, 9>& box_mask, int row, int col, int value) {
  const int bit = 1 << value;
  const int b = BoxIndex(row, col);
  return (row_mask[row] & bit) == 0 && (col_mask[col] & bit) == 0 &&
         (box_mask[b] & bit) == 0;
}

void Place(std::array<int, 9>* row_mask, std::array<int, 9>* col_mask,
           std::array<int, 9>* box_mask, SudokuGrid* grid, int row, int col,
           int value) {
  const int bit = 1 << value;
  const int b = BoxIndex(row, col);

  (*row_mask)[row] |= bit;
  (*col_mask)[col] |= bit;
  (*box_mask)[b] |= bit;

  grid->Set(row, col, value);
}

void Unplace(std::array<int, 9>* row_mask, std::array<int, 9>* col_mask,
             std::array<int, 9>* box_mask, SudokuGrid* grid, int row, int col) {
  const int value = grid->Get(row, col);
  if (value == 0) return;

  const int bit = 1 << value;
  const int b = BoxIndex(row, col);

  (*row_mask)[row] &= ~bit;
  (*col_mask)[col] &= ~bit;
  (*box_mask)[b] &= ~bit;

  grid->Set(row, col, 0);
}

}  // namespace

bool SolveIterative(SudokuGrid* grid) {
  if (grid == nullptr) return false;

  std::array<int, 9> row_mask{};
  std::array<int, 9> col_mask{};
  std::array<int, 9> box_mask{};
  if (!BuildMasks(*grid, &row_mask, &col_mask, &box_mask)) {
    return false;
  }

  std::vector<int> empties;
  empties.reserve(SudokuGrid::kCellCount);
  for (int r = 0; r < SudokuGrid::kSize; ++r) {
    for (int c = 0; c < SudokuGrid::kSize; ++c) {
      if (grid->Get(r, c) == 0) {
        empties.push_back(r * SudokuGrid::kSize + c);
      }
    }
  }

  std::vector<int> next_value(empties.size(), 1);
  int pos = 0;

  while (true) {
    if (pos == static_cast<int>(empties.size())) {
      return true;
    }
    if (pos < 0) {
      return false;
    }

    const int idx = empties[pos];
    const int r = idx / SudokuGrid::kSize;
    const int c = idx % SudokuGrid::kSize;

    bool placed = false;
    for (int v = next_value[pos]; v <= 9; ++v) {
      if (IsAllowed(row_mask, col_mask, box_mask, r, c, v)) {
        Place(&row_mask, &col_mask, &box_mask, grid, r, c, v);
        next_value[pos] = v + 1;
        ++pos;
        placed = true;
        break;
      }
    }

    if (placed) continue;

    next_value[pos] = 1;
    if (pos == 0) return false;

    --pos;
    const int back_idx = empties[pos];
    const int br = back_idx / SudokuGrid::kSize;
    const int bc = back_idx % SudokuGrid::kSize;
    Unplace(&row_mask, &col_mask, &box_mask, grid, br, bc);
  }
}

}  // namespace sudoku
