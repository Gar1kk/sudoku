/*
Компиляция:
g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 *.cpp -o sudoku
./sudoku


*/

#include <iostream>
#include <random>
#include <string>

#include "console_ui.h"
#include "file_io.h"
#include "generator.h"
#include "solver.h"
#include "sudoku_grid.h"

namespace {

void PrintMenu() {
  std::cout << "\n===== SUDOKU =====\n"
            << "1) Показать текущее поле\n"
            << "2) Ввести поле вручную (9 строк)\n"
            << "3) Загрузить из файла\n"
            << "4) Сохранить в файл\n"
            << "5) Проверка корректности поля\n"
            << "6) Решить (итеративно, без рекурсии)\n"
            << "7) Сгенерировать заполненное корректное судоку\n"
            << "8) Создать задачу (удалить числа)\n"
            << "0) Выход\n";
}

}  // namespace

int main() {
  sudoku::SudokuGrid grid;
  std::random_device rd;
  std::mt19937 rng(rd());

  while (true) {
    PrintMenu();

    int choice = 0;
    if (!console_ui::ReadIntInRange("Выберите пункт: ", 0, 8, &choice)) {
      std::cout << "Ошибка ввода.\n";
      continue;
    }

    if (choice == 0) {
      std::cout << "Выход.\n";
      return 0;
    }

    switch (choice) {
      case 1: {
        std::cout << grid.ToPrettyString() << "\n";
        break;
      }
      case 2: {
        std::cout
            << "Введите 9 строк по 9 символов: цифры 1-9, 0 или '.' для пустых.\n";
        sudoku::SudokuGrid tmp;
        if (!console_ui::ReadGridFromStdin(&tmp)) {
          std::cout << "Не удалось прочитать поле (проверьте формат строк).\n";
          break;
        }
        std::string reason;
        if (!sudoku::IsGridValid(tmp, &reason)) {
          std::cout << "Поле некорректно: " << reason << "\n";
          break;
        }
        grid = tmp;
        std::cout << "Поле обновлено.\n";
        break;
      }
      case 3: {
        std::string path;
        if (!console_ui::ReadLine("Путь к файлу: ", &path)) {
          std::cout << "Ошибка ввода пути.\n";
          break;
        }
        std::string error;
        sudoku::SudokuGrid tmp;
        if (!sudoku::LoadGridFromFile(path, &tmp, &error)) {
          std::cout << "Ошибка загрузки: " << error << "\n";
          break;
        }
        std::string reason;
        if (!sudoku::IsGridValid(tmp, &reason)) {
          std::cout << "Файл загружен, но поле некорректно: " << reason << "\n";
          break;
        }
        grid = tmp;
        std::cout << "Загружено успешно.\n";
        break;
      }
      case 4: {
        std::string path;
        if (!console_ui::ReadLine("Путь для сохранения: ", &path)) {
          std::cout << "Ошибка ввода пути.\n";
          break;
        }
        std::string error;
        if (!sudoku::SaveGridToFile(path, grid, &error)) {
          std::cout << "Ошибка сохранения: " << error << "\n";
          break;
        }
        std::cout << "Сохранено.\n";
        break;
      }
      case 5: {
        std::string reason;
        if (sudoku::IsGridValid(grid, &reason)) {
          std::cout << "Поле корректно.\n";
        } else {
          std::cout << "Поле некорректно: " << reason << "\n";
        }
        break;
      }
      case 6: {
        std::string reason;
        if (!sudoku::IsGridValid(grid, &reason)) {
          std::cout << "Сначала исправьте поле: " << reason << "\n";
          break;
        }
        sudoku::SudokuGrid tmp = grid;
        if (!sudoku::SolveIterative(&tmp)) {
          std::cout << "Решение не найдено (или поле противоречиво).\n";
          break;
        }
        grid = tmp;
        std::cout << "Решено:\n" << grid.ToPrettyString() << "\n";
        break;
      }
      case 7: {
        grid = sudoku::GenerateSolvedGrid(&rng);
        std::cout << "Сгенерировано заполненное корректное судоку:\n"
                  << grid.ToPrettyString() << "\n";
        break;
      }
      case 8: {
        int remove_count = 0;
        if (!console_ui::ReadIntInRange("Сколько клеток удалить (0..81): ", 0, 81,
                                        &remove_count)) {
          std::cout << "Ошибка ввода.\n";
          break;
        }

        int ensure = 0;
        if (!console_ui::ReadIntInRange(
                "Проверять решаемость после каждого удаления? 1-да, 0-нет: ", 0, 1,
                &ensure)) {
          std::cout << "Ошибка ввода.\n";
          break;
        }

        if (!grid.IsComplete()) {
          std::cout << "Текущее поле не заполнено. Сначала сгенерируйте (пункт 7) "
                       "или загрузите готовое решение.\n";
          break;
        }

        std::string reason;
        if (!sudoku::IsGridValid(grid, &reason)) {
            std::cout << "Нельзя создать задачу: текущее решение некорректно: " << reason
                      << "\n";
            break;
}


        int removed = 0;
        sudoku::SudokuGrid puzzle =
            sudoku::CreatePuzzle(grid, remove_count, ensure == 1, &rng, &removed);

        grid = puzzle;
        std::cout << "Задача создана: удалено " << removed << " из " << remove_count
                  << ".\n"
                  << grid.ToPrettyString() << "\n";
        break;
      }
      default:
        std::cout << "Неизвестный пункт.\n";
        break;
    }
  }
}
