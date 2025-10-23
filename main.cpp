#include <algorithm>
#include <bitset>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

#include "coordinate.h"
#include "csv.h"
#include "twodtree.h"
#include "twomeans.h"

void logDuration(const std::string &message,
                 std::chrono::high_resolution_clock::time_point start) {
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> duration = end - start;
  std::cout << "[LOG] " << message << " in " << duration.count() << " [ms]!"
            << std::endl;
}

int main(int argc, char *argv[]) {
  using namespace std::chrono;

  if (argc != 4) {
    std::cerr << "Usage: " << argv[0]
              << " <stops_file> <number_of_levels> <cell_id_output_file>"
              << std::endl;
    return 1;
  }

  std::string stopsFile = argv[1];
  int numberOfLevels = std::stoi(argv[2]);
  std::string outputFile = argv[3];

  std::vector<Coordinate> coords;
  coords.reserve(1 << 13);

  std::cout << "[LOG] Reading data from " << stopsFile << " ..." << std::endl;
  auto startReading = high_resolution_clock::now();

  io::CSVReader<3, io::trim_chars<' ', '\t'>, io::double_quote_escape<',', '"'>>
      in(stopsFile);
  in.read_header(io::ignore_extra_column | io::ignore_missing_column |
                     io::ignore_no_column,
                 "StopId", "Latitude", "Longitude");

  size_t id(0);
  double lat(0.0);
  double lon(0.0);

  while (in.read_row(id, lat, lon)) {
    coords.emplace_back(id, lat, lon);
  }

  logDuration("Reading data", startReading);

  std::cout << "[LOG] Partitioning data ..." << std::endl;
  auto startPartition = high_resolution_clock::now();

#ifdef KDTREE
  twodtree::partition(coords, numberOfLevels);
#else
  twomeans::partition(coords, numberOfLevels);
#endif

  logDuration("Partitioning data", startPartition);

  std::cout << "[LOG] Writing data to " << outputFile << " ..." << std::endl;
  auto startWriting = high_resolution_clock::now();

  std::ofstream outFile(outputFile);
  if (!outFile) {
    std::cerr << "Error opening output file (" << outputFile << ")!"
              << std::endl;
    return 1;
  }

  std::sort(coords.begin(), coords.end(), Coordinate::sortById);
  for (const auto &coord : coords) {
    outFile << coord.cellId << std::endl;
  }

  if (outFile.fail()) {
    std::cerr << "Error writing to output file (" << outputFile << ")!"
              << std::endl;
    return 1;
  }

  outFile.close();

  logDuration("Writing data", startWriting);
  return 0;
}
