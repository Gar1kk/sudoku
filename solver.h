#ifndef SUDOKU_SOLVER_H_
#define SUDOKU_SOLVER_H_

#include "sudoku_grid.h"

namespace sudoku {

// Итеративный решатель без рекурсии.
bool SolveIterative(SudokuGrid* grid);

}  // namespace sudoku

#endif  // SUDOKU_SOLVER_H_
