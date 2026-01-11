#include "generator.h"

#include <algorithm>
#include <array>
#include <numeric>
#include <vector>

#include "solver.h"

namespace sudoku {
namespace {

int Pattern(int r, int c) {
  return (r * 3 + r / 3 + c) % 9;
}

template <typename T>
void Shuffle(std::vector<T>* v, std::mt19937* rng) {
  std::shuffle(v->begin(), v->end(), *rng);
}

std::vector<int> ShuffledIndicesByGroups(int group_count, int group_size,
                                         std::mt19937* rng) {
  std::vector<int> groups(group_count);
  std::iota(groups.begin(), groups.end(), 0);
  Shuffle(&groups, rng);

  std::vector<int> result;
  result.reserve(group_count * group_size);
  for (int g : groups) {
    std::vector<int> inner(group_size);
    std::iota(inner.begin(), inner.end(), 0);
    Shuffle(&inner, rng);
    for (int x : inner) {
      result.push_back(g * group_size + x);
    }
  }
  return result;
}

}  // namespace

SudokuGrid GenerateSolvedGrid(std::mt19937* rng) {
  SudokuGrid grid;
  if (rng == nullptr) return grid;

  std::vector<int> rows = ShuffledIndicesByGroups(3, 3, rng);
  std::vector<int> cols = ShuffledIndicesByGroups(3, 3, rng);

  std::array<int, 9> digits{};
  for (int i = 0; i < 9; ++i) digits[i] = i + 1;
  std::shuffle(digits.begin(), digits.end(), *rng);

  for (int r = 0; r < SudokuGrid::kSize; ++r) {
    for (int c = 0; c < SudokuGrid::kSize; ++c) {
      const int base = Pattern(rows[r], cols[c]);
      grid.Set(r, c, digits[base]);
    }
  }
  return grid;
}

SudokuGrid CreatePuzzle(const SudokuGrid& solved, int remove_count,
                        bool ensure_solvable, std::mt19937* rng,
                        int* removed_out) {
  if (removed_out != nullptr) *removed_out = 0;
  if (rng == nullptr) return solved;

  SudokuGrid puzzle = solved;

  std::vector<int> positions(SudokuGrid::kCellCount);
  std::iota(positions.begin(), positions.end(), 0);
  std::shuffle(positions.begin(), positions.end(), *rng);

  int removed = 0;
  int attempts = 0;
  constexpr int kMaxAttemptsMultiplier = 5;
  const int max_attempts = SudokuGrid::kCellCount * kMaxAttemptsMultiplier;

  for (int idx : positions) {
    if (removed >= remove_count) break;

    const int r = idx / SudokuGrid::kSize;
    const int c = idx % SudokuGrid::kSize;

    const int old = puzzle.Get(r, c);
    if (old == 0) continue;

    puzzle.Set(r, c, 0);

    if (!ensure_solvable) {
      ++removed;
      continue;
    }

    SudokuGrid check = puzzle;
    if (SolveIterative(&check)) {
      ++removed;
    } else {
      puzzle.Set(r, c, old);
      ++attempts;
      if (attempts >= max_attempts) break;
    }
  }

  if (removed_out != nullptr) *removed_out = removed;
  return puzzle;
}

}  // namespace sudoku
