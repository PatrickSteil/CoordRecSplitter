# Coordinate Partitioning

This project reads coordinate data from a CSV file, partitions the coordinates into cells based on latitude and longitude using recursive binary partitioning, and outputs the resulting cell IDs to a file. Similar to a 2D-Tree, where on each "new" level, we alter between `latitude` and `longitude`.

## Usage

1. **Compile the project:**
```
make
```
  3. **Execute the programm**
```
./partition <stops_file> <number_of_levels> <cell_id_output_file>
```
  -   `<stops_file>`: Path to the input CSV file containing the coordinates.
  -   `<number_of_levels>`: Number of partitioning levels.
  -   `<cell_id_output_file>`: Path to the output file where the cell IDs will be written.
