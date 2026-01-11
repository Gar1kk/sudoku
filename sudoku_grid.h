#ifndef SUDOKU_SUDOKU_GRID_H_
#define SUDOKU_SUDOKU_GRID_H_

#include <array>
#include <string>

namespace sudoku {

class SudokuGrid {
 public:
  static constexpr int kSize = 9;
  static constexpr int kCellCount = kSize * kSize;

  SudokuGrid();

  int Get(int row, int col) const;
  void Set(int row, int col, int value);  // value: 0..9
  void Clear(int row, int col);

  bool IsComplete() const;
  std::string ToPrettyString() const;
  std::string ToCompactString() const;

 private:
  std::array<int, kCellCount> cells_;
};

bool IsGridValid(const SudokuGrid& grid, std::string* reason);

}  // namespace sudoku

#endif  // SUDOKU_SUDOKU_GRID_H_
