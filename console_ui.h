#ifndef SUDOKU_CONSOLE_UI_H_
#define SUDOKU_CONSOLE_UI_H_

#include <string>

#include "sudoku_grid.h"

namespace console_ui {

bool ReadLine(const std::string& prompt, std::string* out);
bool ReadIntInRange(const std::string& prompt, int min_value, int max_value,
                    int* out);

bool ReadGridFromStdin(sudoku::SudokuGrid* grid);

}  // namespace console_ui

#endif  // SUDOKU_CONSOLE_UI_H_
