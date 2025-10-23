#pragma once
#include <cassert>
#include <cmath>
#include <random>

#include "coordinate.h"

namespace twomeans {

double distance2(const Coordinate &a, const Coordinate &b) {
  double dlat = a.lat - b.lat;
  double dlon = a.lon - b.lon;
  return dlat * dlat + dlon * dlon;
}

Coordinate computeCentroid(const std::vector<Coordinate> &coords, size_t left,
                           size_t right) {
  double latSum = 0.0, lonSum = 0.0;
  size_t n = right - left;
  for (size_t i = left; i < right; ++i) {
    latSum += coords[i].lat;
    lonSum += coords[i].lon;
  }
  return Coordinate(0, latSum / n, lonSum / n);
}

size_t twoMeansSplit(std::vector<Coordinate> &coords, size_t left,
                     size_t right) {
  assert(right > left + 1);

  static thread_local std::mt19937 rng(std::random_device{}());
  std::uniform_int_distribution<size_t> dist(left, right - 1);
  Coordinate c1 = coords[dist(rng)];
  Coordinate c2 = coords[dist(rng)];

  constexpr int MAX_ITERS = 100;
  for (int iter = 0; iter < MAX_ITERS; ++iter) {
    size_t mid = left;
    for (size_t i = left; i < right; ++i) {
      double d1 = distance2(coords[i], c1);
      double d2 = distance2(coords[i], c2);
      if (d1 < d2) {
        std::swap(coords[i], coords[mid++]);
      }
    }

    Coordinate newC1 = computeCentroid(coords, left, mid);
    Coordinate newC2 = computeCentroid(coords, mid, right);

    double change = distance2(c1, newC1) + distance2(c2, newC2);
    c1 = newC1;
    c2 = newC2;
    if (change < 1e-10)
      break;
  }

  size_t mid = left;
  for (size_t i = left; i < right; ++i) {
    double d1 = distance2(coords[i], c1);
    double d2 = distance2(coords[i], c2);
    if (d1 < d2)
      std::swap(coords[i], coords[mid++]);
  }
  if (mid == left || mid == right)
    mid = left + (right - left) / 2;
  return mid;
}

void applyTwoMeans(std::vector<Coordinate> &coords, size_t left, size_t right,
                   int level, int numberOfLevels) {
  if (level >= numberOfLevels || right - left <= 1)
    return;

  size_t mid = twoMeansSplit(coords, left, right);

  for (size_t i = left; i < mid; ++i)
    coords[i].cellId |= (1 << (numberOfLevels - level - 1));

  applyTwoMeans(coords, left, mid, level + 1, numberOfLevels);
  applyTwoMeans(coords, mid, right, level + 1, numberOfLevels);
}

void partition(std::vector<Coordinate> &coords, int numberOfLevels) {
  assert(numberOfLevels > 0);
  applyTwoMeans(coords, 0, coords.size(), 0, numberOfLevels);
}
} // namespace twomeans
