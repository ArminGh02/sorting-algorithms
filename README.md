
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

```
------------------------------------------------------------------------------------------------------------------------------------------------------------
Benchmark                                                                                                                  Time             CPU   Iterations
------------------------------------------------------------------------------------------------------------------------------------------------------------
sorting a std::vector<int> of size 10000 - shuffled - alg::bubble_sort/0/0                                          92183743 ns     91517857 ns            7
sorting a std::vector<int> of size 10000 - shuffled - alg::insertion_sort/0/1                                        9550217 ns      9521484 ns           64
sorting a std::vector<int> of size 10000 - shuffled - alg::selection_sort/0/2                                       41309918 ns     41360294 ns           17
sorting a std::vector<int> of size 10000 - shuffled - alg::heap_sort/0/3                                              376165 ns       376607 ns         1867
sorting a std::vector<int> of size 10000 - shuffled - alg::merge_sort/0/4                                             474033 ns       474330 ns         1120
sorting a std::vector<int> of size 10000 - shuffled - alg::quick_sort/0/5                                             441234 ns       439551 ns         1493
sorting a std::vector<int> of size 10000 - shuffled - std::stable_sort/0/9                                            476777 ns       486592 ns         1445
sorting a std::vector<int> of size 10000 - shuffled - std::sort/0/10                                                  422773 ns       414406 ns         1659
sorting a std::vector<int> of size 10000 - sorted - alg::bubble_sort/1/0                                               10233 ns         8998 ns        74667
sorting a std::vector<int> of size 10000 - sorted - alg::insertion_sort/1/1                                             8415 ns         8022 ns        89600
sorting a std::vector<int> of size 10000 - sorted - alg::selection_sort/1/2                                         45273573 ns     44791667 ns           15
sorting a std::vector<int> of size 10000 - sorted - alg::heap_sort/1/3                                                328704 ns       334762 ns         1867
sorting a std::vector<int> of size 10000 - sorted - alg::merge_sort/1/4                                                67127 ns        66964 ns        11200
sorting a std::vector<int> of size 10000 - sorted - alg::quick_sort/1/5                                               164894 ns       167411 ns         4480
sorting a std::vector<int> of size 10000 - sorted - std::stable_sort/1/9                                               79804 ns        80915 ns        11200
sorting a std::vector<int> of size 10000 - sorted - std::sort/1/10                                                    106143 ns       102534 ns         7467
sorting a std::vector<int> of size 10000 - reverse sorted - alg::bubble_sort/2/0                                    58422146 ns     58238636 ns           11
sorting a std::vector<int> of size 10000 - reverse sorted - alg::insertion_sort/2/1                                 19694747 ns     19761029 ns           34
sorting a std::vector<int> of size 10000 - reverse sorted - alg::selection_sort/2/2                                 31480432 ns     31250000 ns           22
sorting a std::vector<int> of size 10000 - reverse sorted - alg::heap_sort/2/3                                        318735 ns       314991 ns         2133
sorting a std::vector<int> of size 10000 - reverse sorted - alg::merge_sort/2/4                                        90393 ns        92072 ns         7467
sorting a std::vector<int> of size 10000 - reverse sorted - alg::quick_sort/2/5                                       185904 ns       188354 ns         3733
sorting a std::vector<int> of size 10000 - reverse sorted - std::stable_sort/2/9                                      103175 ns       104627 ns         7467
sorting a std::vector<int> of size 10000 - reverse sorted - std::sort/2/10                                             78642 ns        80218 ns         8960

sorting a std::vector<std::string> of size 10000 - shuffled - alg::bubble_sort/0/0                                 733410600 ns    734375000 ns            1
sorting a std::vector<std::string> of size 10000 - shuffled - alg::insertion_sort/0/1                              170928925 ns    171875000 ns            4
sorting a std::vector<std::string> of size 10000 - shuffled - alg::selection_sort/0/2                              286867250 ns    281250000 ns            2
sorting a std::vector<std::string> of size 10000 - shuffled - alg::heap_sort/0/3                                     4241075 ns      4894578 ns          166
sorting a std::vector<std::string> of size 10000 - shuffled - alg::merge_sort/0/4                                    5167622 ns      3565436 ns          149
sorting a std::vector<std::string> of size 10000 - shuffled - alg::quick_sort/0/5                                    4667681 ns      4996366 ns          172
sorting a std::vector<std::string> of size 10000 - shuffled - std::stable_sort/0/9                                   3104295 ns      3594484 ns          213
sorting a std::vector<std::string> of size 10000 - shuffled - std::sort/0/10                                         2869874 ns      3125000 ns          345
sorting a std::vector<std::string> of size 10000 - sorted - alg::bubble_sort/1/0                                     1156107 ns      1233553 ns          836
sorting a std::vector<std::string> of size 10000 - sorted - alg::insertion_sort/1/1                                  3174744 ns      3214713 ns          209
sorting a std::vector<std::string> of size 10000 - sorted - alg::selection_sort/1/2                                328347150 ns    320312500 ns            2
sorting a std::vector<std::string> of size 10000 - sorted - alg::heap_sort/1/3                                       3545580 ns      3906250 ns          236
sorting a std::vector<std::string> of size 10000 - sorted - alg::merge_sort/1/4                                      3884819 ns      4059436 ns          204
sorting a std::vector<std::string> of size 10000 - sorted - alg::quick_sort/1/5                                      3628622 ns      3229749 ns          179
sorting a std::vector<std::string> of size 10000 - sorted - std::stable_sort/1/9                                     1780437 ns      1804361 ns          407
sorting a std::vector<std::string> of size 10000 - sorted - std::sort/1/10                                           2016091 ns      2008929 ns          280
sorting a std::vector<std::string> of size 10000 - reverse sorted - alg::bubble_sort/2/0                           517760100 ns    515625000 ns            1
sorting a std::vector<std::string> of size 10000 - reverse sorted - alg::insertion_sort/2/1                        398868850 ns    406250000 ns            2
sorting a std::vector<std::string> of size 10000 - reverse sorted - alg::selection_sort/2/2                        544071200 ns    531250000 ns            1
sorting a std::vector<std::string> of size 10000 - reverse sorted - alg::heap_sort/2/3                               3356815 ns      3374413 ns          213
sorting a std::vector<std::string> of size 10000 - reverse sorted - alg::merge_sort/2/4                              4080381 ns      4177807 ns          187
sorting a std::vector<std::string> of size 10000 - reverse sorted - alg::quick_sort/2/5                              4015589 ns      4006410 ns          195
sorting a std::vector<std::string> of size 10000 - reverse sorted - std::stable_sort/2/9                             1810332 ns      1757812 ns          560
sorting a std::vector<std::string> of size 10000 - reverse sorted - std::sort/2/10                                   1850434 ns      1504630 ns          405

sorting std::vector<unsigned int> of size 10000 and max element <= 1000 - shuffled - alg::counting_sort/0/6            27448 ns        27169 ns        23579
sorting std::vector<unsigned int> of size 10000 and max element <= 1000 - shuffled - alg::radix_sort/0/7              285635 ns       284934 ns         2358
sorting std::vector<unsigned int> of size 10000 and max element <= 1000 - shuffled - std::stable_sort/0/9             540238 ns       544085 ns         1120
sorting std::vector<unsigned int> of size 10000 and max element <= 1000 - shuffled - std::sort/0/10                   440638 ns       439453 ns         1600
sorting std::vector<unsigned int> of size 10000 and max element <= 1000 - sorted - alg::counting_sort/1/6              26418 ns        27274 ns        26353
sorting std::vector<unsigned int> of size 10000 and max element <= 1000 - sorted - alg::radix_sort/1/7                285122 ns       282493 ns         2489
sorting std::vector<unsigned int> of size 10000 and max element <= 1000 - sorted - std::stable_sort/1/9                91847 ns        92072 ns         7467
sorting std::vector<unsigned int> of size 10000 and max element <= 1000 - sorted - std::sort/1/10                     134075 ns       128691 ns         4978
sorting std::vector<unsigned int> of size 10000 and max element <= 1000 - reverse sorted - alg::counting_sort/2/6      27438 ns        27623 ns        24889
sorting std::vector<unsigned int> of size 10000 and max element <= 1000 - reverse sorted - alg::radix_sort/2/7        290028 ns       284934 ns         2358
sorting std::vector<unsigned int> of size 10000 and max element <= 1000 - reverse sorted - std::stable_sort/2/9       130669 ns       128691 ns         4978
sorting std::vector<unsigned int> of size 10000 and max element <= 1000 - reverse sorted - std::sort/2/10             132778 ns       134969 ns         4978

sorting std::vector<double> of size 10000 where 0<=vec[i]<1 - shuffled - alg::bucket_sort/0/8                        1260542 ns      1255580 ns          560
sorting std::vector<double> of size 10000 where 0<=vec[i]<1 - shuffled - std::stable_sort/0/9                         699671 ns       697545 ns          896
sorting std::vector<double> of size 10000 where 0<=vec[i]<1 - shuffled - std::sort/0/10                               604800 ns       613839 ns         1120
sorting std::vector<double> of size 10000 where 0<=vec[i]<1 - sorted - alg::bucket_sort/1/8                          1208114 ns      1199777 ns          560
sorting std::vector<double> of size 10000 where 0<=vec[i]<1 - sorted - std::stable_sort/1/9                           116405 ns       112305 ns         6400
sorting std::vector<double> of size 10000 where 0<=vec[i]<1 - sorted - std::sort/1/10                                 139341 ns       138108 ns         4978
sorting std::vector<double> of size 10000 where 0<=vec[i]<1 - reverse sorted - alg::bucket_sort/2/8                  1217121 ns      1199777 ns          560
sorting std::vector<double> of size 10000 where 0<=vec[i]<1 - reverse sorted - std::stable_sort/2/9                   360817 ns       352926 ns         1948
sorting std::vector<double> of size 10000 where 0<=vec[i]<1 - reverse sorted - std::sort/2/10                         110470 ns       114746 ns         6400
```

## Unit Test
All implemented functions are tested with **[Catch2](https://github.com/catchorg/Catch2)**.

## Contribution
Feel free to contribute by implementing more sorting algorithms or improving performance of the existing ones.
