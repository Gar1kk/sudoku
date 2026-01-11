#ifndef SUDOKU_GENERATOR_H_
#define SUDOKU_GENERATOR_H_

#include <random>

#include "sudoku_grid.h"

namespace sudoku {

SudokuGrid GenerateSolvedGrid(std::mt19937* rng);

// remove_count: сколько клеток сделать пустыми.
// ensure_solvable: если true — удаление откатывается, если поле стало нерешаемым.
// removed_out: фактически удалено (может быть меньше remove_count).
SudokuGrid CreatePuzzle(const SudokuGrid& solved, int remove_count,
                        bool ensure_solvable, std::mt19937* rng,
                        int* removed_out);

}  // namespace sudoku

#endif  // SUDOKU_GENERATOR_H_
