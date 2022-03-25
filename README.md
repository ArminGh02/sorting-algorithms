
# Sorting Algorithms

- [About](#about)
- [Requirements](#requirements)
- [How to Use](#how-to-use)
- [Currently Implemented Algorithms](#currently-implemented-algorithms)
- [Benchmarks](#benchmarks)
- [Unit Test](#unit-test)
- [Contribution](#contribution)

## About
This repository implements sorting algorithms with an emphasis on high performance.
It also tests and benchmarks the implemented functions.
It is going to implement more algorithms and improve performance of implemented algorithms.
Inspired by **[EASTL](https://github.com/electronicarts/EASTL)** and **[MSVC STL](https://github.com/microsoft/STL)**.

## Requirements
This header-only library requires at least C++11 but benchmarks require at least C++14.
**[Benchmarks](sort_benchmark.cpp)** require **[google benchmark](https://github.com/google/benchmark)**.
**[Unit tests](sort_test.cpp)** require **[Catch2](https://github.com/catchorg/Catch2)**.

## How to Use
Just copy the **[sort.hpp](sort.hpp)** header file to your project and include it!

## Currently Implemented Algorithms
- Bubble Sort
- Insertion Sort
- Selection Sort
- Heap Sort
- Merge Sort
- Quick Sort (Introsort)
- Counting Sort
- Radix Sort
- Bucket Sort
- and more to come!

## Benchmarks
Output of benchmarks is similar to:

| Benchmark                                                                                                        | Time         |
| ---------------------------------------------------------------------------------------------------------------- | ------------ |
| sorting a std::vector\<int\> of size 10000 - shuffled - alg::bubble_sort                                         |  92183743 ns |
| sorting a std::vector\<int\> of size 10000 - shuffled - alg::insertion_sort                                      |   9550217 ns |
| sorting a std::vector\<int\> of size 10000 - shuffled - alg::selection_sort                                      |  41309918 ns |
| sorting a std::vector\<int\> of size 10000 - shuffled - alg::heap_sort                                           |    376165 ns |
| sorting a std::vector\<int\> of size 10000 - shuffled - alg::merge_sort                                          |    474033 ns |
| sorting a std::vector\<int\> of size 10000 - shuffled - alg::quick_sort                                          |    441234 ns |
| sorting a std::vector\<int\> of size 10000 - shuffled - std::stable_sort                                         |    476777 ns |
| sorting a std::vector\<int\> of size 10000 - shuffled - std::sort                                                |    422773 ns |
| sorting a std::vector\<int\> of size 10000 - sorted - alg::bubble_sort                                           |     10233 ns |
| sorting a std::vector\<int\> of size 10000 - sorted - alg::insertion_sort                                        |      8415 ns |
| sorting a std::vector\<int\> of size 10000 - sorted - alg::selection_sort                                        |  45273573 ns |
| sorting a std::vector\<int\> of size 10000 - sorted - alg::heap_sort                                             |    328704 ns |
| sorting a std::vector\<int\> of size 10000 - sorted - alg::merge_sort                                            |     67127 ns |
| sorting a std::vector\<int\> of size 10000 - sorted - alg::quick_sort                                            |    164894 ns |
| sorting a std::vector\<int\> of size 10000 - sorted - std::stable_sort                                           |     79804 ns |
| sorting a std::vector\<int\> of size 10000 - sorted - std::sort                                                  |    106143 ns |
| sorting a std::vector\<int\> of size 10000 - reverse sorted - alg::bubble_sort                                   |  58422146 ns |
| sorting a std::vector\<int\> of size 10000 - reverse sorted - alg::insertion_sort                                |  19694747 ns |
| sorting a std::vector\<int\> of size 10000 - reverse sorted - alg::selection_sort                                |  31480432 ns |
| sorting a std::vector\<int\> of size 10000 - reverse sorted - alg::heap_sort                                     |    318735 ns |
| sorting a std::vector\<int\> of size 10000 - reverse sorted - alg::merge_sort                                    |     90393 ns |
| sorting a std::vector\<int\> of size 10000 - reverse sorted - alg::quick_sort                                    |    185904 ns |
| sorting a std::vector\<int\> of size 10000 - reverse sorted - std::stable_sort                                   |    103175 ns |
| sorting a std::vector\<int\> of size 10000 - reverse sorted - std::sort                                          |     78642 ns |
| sorting a std::vector\<std::string\> of size 10000 - shuffled - alg::bubble_sort                                 | 733410600 ns |
| sorting a std::vector\<std::string\> of size 10000 - shuffled - alg::insertion_sort                              | 170928925 ns |
| sorting a std::vector\<std::string\> of size 10000 - shuffled - alg::selection_sort                              | 286867250 ns |
| sorting a std::vector\<std::string\> of size 10000 - shuffled - alg::heap_sort                                   |   4241075 ns |
| sorting a std::vector\<std::string\> of size 10000 - shuffled - alg::merge_sort                                  |   5167622 ns |
| sorting a std::vector\<std::string\> of size 10000 - shuffled - alg::quick_sort                                  |   4667681 ns |
| sorting a std::vector\<std::string\> of size 10000 - shuffled - std::stable_sort                                 |   3104295 ns |
| sorting a std::vector\<std::string\> of size 10000 - shuffled - std::sort                                        |   2869874 ns |
| sorting a std::vector\<std::string\> of size 10000 - sorted - alg::bubble_sort                                   |   1156107 ns |
| sorting a std::vector\<std::string\> of size 10000 - sorted - alg::insertion_sort                                |   3174744 ns |
| sorting a std::vector\<std::string\> of size 10000 - sorted - alg::selection_sort                                | 328347150 ns |
| sorting a std::vector\<std::string\> of size 10000 - sorted - alg::heap_sort                                     |   3545580 ns |
| sorting a std::vector\<std::string\> of size 10000 - sorted - alg::merge_sort                                    |   3884819 ns |
| sorting a std::vector\<std::string\> of size 10000 - sorted - alg::quick_sort                                    |   3628622 ns |
| sorting a std::vector\<std::string\> of size 10000 - sorted - std::stable_sort                                   |   1780437 ns |
| sorting a std::vector\<std::string\> of size 10000 - sorted - std::sort                                          |   2016091 ns |
| sorting a std::vector\<std::string\> of size 10000 - reverse sorted - alg::bubble_sort                           | 517760100 ns |
| sorting a std::vector\<std::string\> of size 10000 - reverse sorted - alg::insertion_sort                        | 398868850 ns |
| sorting a std::vector\<std::string\> of size 10000 - reverse sorted - alg::selection_sort                        | 544071200 ns |
| sorting a std::vector\<std::string\> of size 10000 - reverse sorted - alg::heap_sort                             |   3356815 ns |
| sorting a std::vector\<std::string\> of size 10000 - reverse sorted - alg::merge_sort                            |   4080381 ns |
| sorting a std::vector\<std::string\> of size 10000 - reverse sorted - alg::quick_sort                            |   4015589 ns |
| sorting a std::vector\<std::string\> of size 10000 - reverse sorted - std::stable_sort                           |   1810332 ns |
| sorting a std::vector\<std::string\> of size 10000 - reverse sorted - std::sort                                  |   1850434 ns |
| sorting std::vector\<unsigned int\> of size 10000 and max element \<= 1000 - shuffled - alg::counting_sort       |     27448 ns |
| sorting std::vector\<unsigned int\> of size 10000 and max element \<= 1000 - shuffled - alg::radix_sort          |    285635 ns |
| sorting std::vector\<unsigned int\> of size 10000 and max element \<= 1000 - shuffled - std::stable_sort         |    540238 ns |
| sorting std::vector\<unsigned int\> of size 10000 and max element \<= 1000 - shuffled - std::sort                |    440638 ns |
| sorting std::vector\<unsigned int\> of size 10000 and max element \<= 1000 - sorted - alg::counting_sort         |     26418 ns |
| sorting std::vector\<unsigned int\> of size 10000 and max element \<= 1000 - sorted - alg::radix_sort            |    285122 ns |
| sorting std::vector\<unsigned int\> of size 10000 and max element \<= 1000 - sorted - std::stable_sort           |     91847 ns |
| sorting std::vector\<unsigned int\> of size 10000 and max element \<= 1000 - sorted - std::sort                  |    134075 ns |
| sorting std::vector\<unsigned int\> of size 10000 and max element \<= 1000 - reverse sorted - alg::counting_sort |     27438 ns |
| sorting std::vector\<unsigned int\> of size 10000 and max element \<= 1000 - reverse sorted - alg::radix_sort    |    290028 ns |
| sorting std::vector\<unsigned int\> of size 10000 and max element \<= 1000 - reverse sorted - std::stable_sort   |    130669 ns |
| sorting std::vector\<unsigned int\> of size 10000 and max element \<= 1000 - reverse sorted - std::sort          |    132778 ns |
| sorting std::vector\<double\> of size 10000 where 0\<=vec[i]\<1 - shuffled - alg::bucket_sort                    |   1260542 ns |
| sorting std::vector\<double\> of size 10000 where 0\<=vec[i]\<1 - shuffled - std::stable_sort                    |    699671 ns |
| sorting std::vector\<double\> of size 10000 where 0\<=vec[i]\<1 - shuffled - std::sort                           |    604800 ns |
| sorting std::vector\<double\> of size 10000 where 0\<=vec[i]\<1 - sorted - alg::bucket_sort                      |   1208114 ns |
| sorting std::vector\<double\> of size 10000 where 0\<=vec[i]\<1 - sorted - std::stable_sort                      |    116405 ns |
| sorting std::vector\<double\> of size 10000 where 0\<=vec[i]\<1 - sorted - std::sort                             |    139341 ns |
| sorting std::vector\<double\> of size 10000 where 0\<=vec[i]\<1 - reverse sorted - alg::bucket_sort              |   1217121 ns |
| sorting std::vector\<double\> of size 10000 where 0\<=vec[i]\<1 - reverse sorted - std::stable_sort              |    360817 ns |
| sorting std::vector\<double\> of size 10000 where 0\<=vec[i]\<1 - reverse sorted - std::sort                     |    110470 ns |

## Unit Test
All implemented functions are tested with **[Catch2](https://github.com/catchorg/Catch2)**.

## Contribution
Feel free to contribute by implementing more sorting algorithms or improving performance of the existing ones.
