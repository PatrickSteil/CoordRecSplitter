#pragma once
#include <cstdint>

struct Coordinate {
  Coordinate(std::size_t index, double latitude, double longitude)
      : index(index), lat(latitude), lon(longitude), cellId(0) {}

  std::size_t index;
  double lat;
  double lon;
  int cellId;

  static bool sortByLat(const Coordinate &left, const Coordinate &right) {
    return left.lat < right.lat;
  }

  static bool sortByLon(const Coordinate &left, const Coordinate &right) {
    return left.lon < right.lon;
  }

  static bool sortById(const Coordinate &left, const Coordinate &right) {
    return left.index < right.index;
  }
};
