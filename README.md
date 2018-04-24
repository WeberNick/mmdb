# Main Memory Database System

Implementation of a main memory database system with two different storage layouts. The project started as a bachelors thesis and development on the system continued as part of my work at the [Chair of Applied Computer Science III, University of Mannheim](http://lspi3.informatik.uni-mannheim.de/en/home/). This implementation is only suited for testing the performance differences between the storage layouts and research on main memory database systems.

## Getting Started

### Prerequisites

To build and run, you'll need the Build-Management-Tool _make_ and a C++11 compatible compiler (only tested with GCC)

### Installing

```
git clone https://github.com/WeberNick/MainMemoryDatabaseSystem.git
```

### Building

In the git repository directory (_mmdb_) go to directory '_src_' and run _make_:
```
cd DatabaseSystem/src
make
```

### Running (Quickstart guide)
To execute the program, go to the build directory _src_ and type 

```
./main/main
```

or go in the _main_ directory and type

```
./main
```

To actually use the DBS, several command line arguments are needed. To get a list of possible arguements (from the main directory) type

```
./main --help
```

First of all, a storage layout is needed, e.g., NSM:

```
./main --nsm
```

Secondly, a dataset is needed. To use the dataset of the _TPC-H_ benchmark, type

```
./main --nsm --tpch
```

Note: In order to use the _TPC-H_ dataset, place the TPC-H tables in a directory *tables_[SF]* (where SF is the scale factor of the dataset, e.g., tables_1.0) and place this directory in the directory _/DatabaseSystem/tables_. The scale factor to use (default 1.0) must be entered as command line argument 

```
./main --nsm --tpch --sf 0.001
```

Without any further arguments, the program will do nothing special and terminate. To execute all implemented tests type

```
./main --nsm --tpch --sf 0.001 --all
```

Note: Since this is still under construction, some tests may be disabled or commented out.

## Project Status

### Already implemented

* Storage Models "N-ary Storage Model" (NSM) and "Partition Attributes Across" (PAX)
* Bulk load from from file into main memory
* Relational Algebra Operators
  * Scan
  * Scan with Selection
  * Selection
  * Projection
  	* Optimized Switch Projection
  	* Materialized Projection
  	* Materialized Optimized Switch Projection
* Simple Test Queries
	* Scan and Selection
	* Projection
	* Updates
* Measurements of Queries
* Helper tools to visualize the measurements (using gnuplot)

### Todo

* Implement Joins
* Implement TPC-H Benchmarks
