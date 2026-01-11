#ifndef SUDOKU_FILE_IO_H_
#define SUDOKU_FILE_IO_H_

#include <string>

#include "sudoku_grid.h"

namespace sudoku {

bool LoadGridFromFile(const std::string& path, SudokuGrid* grid,
                      std::string* error);
bool SaveGridToFile(const std::string& path, const SudokuGrid& grid,
                    std::string* error);

}  // namespace sudoku

#endif  // SUDOKU_FILE_IO_H_
