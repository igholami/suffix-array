# Suffix Array
Implementation of Suffix array - Assignment 0 of CMSC701 - Computational Genomics

## Dependencies
Install dependencies
```shell
apt-get install libboost-dev-all cmake
```

## Build
```shell
bash build.sh
```

## Usage
Build index:
```shell
./buildsa path/to/genome.fasta 8 index.bin
```
Inspect index:
```shell
./inspectsa index.bin 64 inspect.stat
```
Query:
```shell
./querysa index.bim path/to/query.fasta naive naive.map
./querysa index.bim path/to/query.fasta simpaccel simpaccel.map
./querysa index.bim path/to/query.fasta prefaccel prefaccel.map
```