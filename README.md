# Weather Data Analyser

A C++ program that reads weather station data from CSV files and reports statistics on wind speed, temperature and solar radiation.

Written for ICT283 Data Structures and Abstractions at Murdoch University Dubai.

## What it does

The program loads readings from one or more CSV files, groups them by month, and answers four questions from a menu:

1. Average wind speed and sample standard deviation for a given month and year
2. Average temperature and sample standard deviation for every month of a year
3. Pearson correlation between wind speed, temperature and solar radiation for a given month
4. A full yearly summary written out to `WindTempSolar.csv`

Against the full Murdoch dataset it handles 208,891 readings spanning 51 months across six multi-year files.

## How it is built

| Component | What it does and why |
|---|---|
| `Bst<T>` | A template binary search tree written from scratch. Traversals take a function pointer, so the tree never needs to know what type it holds or what the caller wants done with each item. |
| `Map<K, V>` | Maps a year and month key to one BST of readings, so a query only ever touches the month it needs. |
| `Vector<T>` | A dynamic array written from scratch rather than using `std::vector`. |
| `WeatherDataReader` | Reads the header row and finds each column by name, so files with reordered columns still load correctly. A file missing a required column is rejected rather than partially loaded. |
| `Statistics` | Mean, sample standard deviation and Pearson correlation. |

Two design notes worth calling out:

Traversal callbacks receive each item as `const T&`. If a callback could modify an item in place, it might change the field the tree sorts on, which would silently break the ordering with nothing to warn anyone.

The CSV reader strips carriage returns, because the source files use Windows line endings and the trailing `\r` would otherwise end up inside the last parsed value.

## Building and running

The project was developed in Code::Blocks with the MinGW GCC compiler on Windows, and `WeatherDataAnalyser.cbp` is included so it opens as a working project. Open that file in Code::Blocks and press F9 to build and run.

The build settings carried in the project file are:

```
-std=c++11 -Wall -Wextra -Weffc++ -Wfloat-equal -Isrc
```

`-Weffc++` is the strict one. It warns about things like constructors that leave members uninitialised, and the project compiles clean under it.

It builds just as well without an IDE:

```
g++ -std=c++11 -Wall -Wextra src/*.cpp -o weather
./weather
```

The program reads `data/data_source.txt`, which lists the CSV files to load, one filename per line. Run it from the project root so it can find the `data` folder. In Code::Blocks that means leaving the working directory as the project folder, which is the default.

## About the data

This repository includes two small sample files of 90 readings each, taken from 31 March 2016. They are enough to clone the project and see it run.

The full dataset is not included. Each yearly file is between 4 and 5 MB, and the complete set is around 45 MB, which is more than belongs in a Git repository. To use your own data, drop the CSV files into `data` and list their names in `data_source.txt`.

## Testing

Correlation results were checked against Excel's `PEARSON` function and matched to two decimal places (0.48, 0.26 and 0.50 for June).

Unit tests for the tree and the map are in `tests`, each with its own Code::Blocks project (`BstTest.cbp` and `MapTest.cbp`) so they build separately from the main program. From the command line:

```
g++ -std=c++11 -Isrc tests/BstTest.cpp -o bsttest
g++ -std=c++11 -Isrc tests/MapTest.cpp -o maptest
```

The project compiles with no warnings under `-Wall -Wextra -Weffc++ -Wfloat-equal`.

A written rationale comparing the chosen data structures against the alternatives, along with the test plan, is in `docs`.
