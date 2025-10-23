#pragma once
#include <vector>

#include "coordinate.h"

namespace twodtree {
void applyHalf(std::vector<Coordinate> &coords, size_t left, size_t right,
               int level, int numberOfLevels) {
  assert(left <= right);
  assert(right <= coords.size());

  if (level >= numberOfLevels || left == right) {
    return;
  }

  if (level % 2 == 0) {
    std::sort(coords.begin() + left, coords.begin() + right,
              Coordinate::sortByLat);
  } else {
    std::sort(coords.begin() + left, coords.begin() + right,
              Coordinate::sortByLon);
  }

  size_t mid = left + (right - left) / 2;

  for (size_t i = left; i < mid; ++i) {
    coords[i].cellId |= (1 << (numberOfLevels - level - 1));
  }
  applyHalf(coords, left, mid, level + 1, numberOfLevels);
  applyHalf(coords, mid, right, level + 1, numberOfLevels);
}

void partition(std::vector<Coordinate> &coords, int numberOfLevels) {
  assert(numberOfLevels > 0);
  std::sort(coords.begin(), coords.end(), Coordinate::sortByLat);
  applyHalf(coords, 0, coords.size(), 0, numberOfLevels);
}
} // namespace twodtree
