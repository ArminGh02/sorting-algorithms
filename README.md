
# Sorting Algorithms

- [Sorting Algorithms](#sorting-algorithms)
  - [About](#about)
  - [Requirements](#requirements)
  - [How to Use](#how-to-use)
  - [Currently Implemented Algorithms](#currently-implemented-algorithms)
  - [Benchmarks](#benchmarks)
  - [Unit Testing](#unit-testing)
  - [Contribution](#contribution)
    - [Project Goals](#project-goals)
    - [TODO List](#todo-list)

## About

In this repository, different sorting algorithms are implemented in C++ with an emphasis on performance.  
The functions are also tested and benchmarked.  
More algorithms will be implemented here and perhaps the performance of the implemented ones will improve.  
Inspired by **[EASTL](https://github.com/electronicarts/EASTL)** and **[MSVC STL](https://github.com/microsoft/STL)**.

## Requirements

All of the code requires at least C++11.  
The [unit tests](test/sort_test.cpp) use **[Catch2](https://github.com/catchorg/Catch2)**.  
The [benchmarks](benchmark/sort_benchmark.cpp) use **[Google Benchmark](https://github.com/google/benchmark)**.

## How to Use

This is a single-header library.  
Just copy the **[sort.hpp](include/sorting_algorithms/sort.hpp)** header file to your project and include it!  
All functions are in the ```alg::``` namespace.

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

The output of the benchmark is similar to:

```text
--------------------------------------------------------------------------------------------------------------------------------------------------------
Benchmark                                                                                                              Time             CPU   Iterations
--------------------------------------------------------------------------------------------------------------------------------------------------------
sorting std::vector<int> of size 10000 - shuffled - alg::bubble_sort/0/0                                        92116814 ns     91517857 ns            7
sorting std::vector<int> of size 10000 - shuffled - alg::insertion_sort/0/1                                      9788619 ns      9765625 ns           64
sorting std::vector<int> of size 10000 - shuffled - alg::selection_sort/0/2                                     38336053 ns     37828947 ns           19
sorting std::vector<int> of size 10000 - shuffled - alg::heap_sort/0/3                                            456290 ns       458984 ns         1600
sorting std::vector<int> of size 10000 - shuffled - alg::merge_sort/0/4                                           601313 ns       599888 ns         1120
sorting std::vector<int> of size 10000 - shuffled - alg::quick_sort/0/5                                           529547 ns       531250 ns         1000
sorting std::vector<int> of size 10000 - shuffled - std::stable_sort/0/9                                          647147 ns       641741 ns         1120
sorting std::vector<int> of size 10000 - shuffled - std::sort/0/10                                                613034 ns       627790 ns         1120
sorting std::vector<int> of size 10000 - sorted - alg::bubble_sort/1/0                                              7264 ns         7394 ns       112000
sorting std::vector<int> of size 10000 - sorted - alg::insertion_sort/1/1                                          13679 ns        15067 ns        74667
sorting std::vector<int> of size 10000 - sorted - alg::selection_sort/1/2                                       44639427 ns     44791667 ns           15
sorting std::vector<int> of size 10000 - sorted - alg::heap_sort/1/3                                              395939 ns       399013 ns         1723
sorting std::vector<int> of size 10000 - sorted - alg::merge_sort/1/4                                              78599 ns        80218 ns         8960
sorting std::vector<int> of size 10000 - sorted - alg::quick_sort/1/5                                             188497 ns       192540 ns         3733
sorting std::vector<int> of size 10000 - sorted - std::stable_sort/1/9                                             90311 ns        92072 ns         7467
sorting std::vector<int> of size 10000 - sorted - std::sort/1/10                                                   90970 ns        89979 ns         7467
sorting std::vector<int> of size 10000 - reverse sorted - alg::bubble_sort/2/0                                  67974422 ns     67708333 ns            9
sorting std::vector<int> of size 10000 - reverse sorted - alg::insertion_sort/2/1                               22192119 ns     21972656 ns           32
sorting std::vector<int> of size 10000 - reverse sorted - alg::selection_sort/2/2                               43719093 ns     43750000 ns           15
sorting std::vector<int> of size 10000 - reverse sorted - alg::heap_sort/2/3                                      391588 ns       389945 ns         1723
sorting std::vector<int> of size 10000 - reverse sorted - alg::merge_sort/2/4                                     136613 ns       134969 ns         4978
sorting std::vector<int> of size 10000 - reverse sorted - alg::quick_sort/2/5                                     227849 ns       230164 ns         2987
sorting std::vector<int> of size 10000 - reverse sorted - std::stable_sort/2/9                                    116118 ns       119629 ns         6400
sorting std::vector<int> of size 10000 - reverse sorted - std::sort/2/10                                           69432 ns        69754 ns        11200

sorting std::vector<std::string> of size 10000 - shuffled - alg::bubble_sort/0/0                               861626300 ns    859375000 ns            1
sorting std::vector<std::string> of size 10000 - shuffled - alg::insertion_sort/0/1                            197825600 ns    203125000 ns            4
sorting std::vector<std::string> of size 10000 - shuffled - alg::selection_sort/0/2                            324119900 ns    320312500 ns            2
sorting std::vector<std::string> of size 10000 - shuffled - alg::heap_sort/0/3                                   4469190 ns      4743304 ns          112
sorting std::vector<std::string> of size 10000 - shuffled - alg::merge_sort/0/4                                  5628212 ns      6250000 ns           90
sorting std::vector<std::string> of size 10000 - shuffled - alg::quick_sort/0/5                                  5092402 ns      6406250 ns          100
sorting std::vector<std::string> of size 10000 - shuffled - std::stable_sort/0/9                                 3389516 ns      3906250 ns          236
sorting std::vector<std::string> of size 10000 - shuffled - std::sort/0/10                                       3053135 ns      3111758 ns          236
sorting std::vector<std::string> of size 10000 - sorted - alg::bubble_sort/1/0                                    875693 ns      1063756 ns          896
sorting std::vector<std::string> of size 10000 - sorted - alg::insertion_sort/1/1                                2635234 ns      2722538 ns          264
sorting std::vector<std::string> of size 10000 - sorted - alg::selection_sort/1/2                              294893650 ns    289062500 ns            2
sorting std::vector<std::string> of size 10000 - sorted - alg::heap_sort/1/3                                     3549821 ns      3976004 ns          224
sorting std::vector<std::string> of size 10000 - sorted - alg::merge_sort/1/4                                    4006426 ns      4042289 ns          201
sorting std::vector<std::string> of size 10000 - sorted - alg::quick_sort/1/5                                    3686907 ns      3670934 ns          166
sorting std::vector<std::string> of size 10000 - sorted - std::stable_sort/1/9                                   2652773 ns      2848525 ns          373
sorting std::vector<std::string> of size 10000 - sorted - std::sort/1/10                                         2070250 ns      1947464 ns          345
sorting std::vector<std::string> of size 10000 - reverse sorted - alg::bubble_sort/2/0                         524949100 ns    531250000 ns            1
sorting std::vector<std::string> of size 10000 - reverse sorted - alg::insertion_sort/2/1                      400318600 ns    398437500 ns            2
sorting std::vector<std::string> of size 10000 - reverse sorted - alg::selection_sort/2/2                      536458600 ns    546875000 ns            1
sorting std::vector<std::string> of size 10000 - reverse sorted - alg::heap_sort/2/3                             3332122 ns      3244174 ns          236
sorting std::vector<std::string> of size 10000 - reverse sorted - alg::merge_sort/2/4                            3904944 ns      3613281 ns          160
sorting std::vector<std::string> of size 10000 - reverse sorted - alg::quick_sort/2/5                            3801915 ns      3491620 ns          179
sorting std::vector<std::string> of size 10000 - reverse sorted - std::stable_sort/2/9                           1597678 ns      1765971 ns          407
sorting std::vector<std::string> of size 10000 - reverse sorted - std::sort/2/10                                 1623644 ns      1842752 ns          407

sorting std::vector<unsigned> of size 10000 and max element <= 1000 - shuffled - alg::counting_sort/0/6            27281 ns        28250 ns        24889
sorting std::vector<unsigned> of size 10000 and max element <= 1000 - shuffled - alg::radix_sort/0/7              289306 ns       291561 ns         2358
sorting std::vector<unsigned> of size 10000 and max element <= 1000 - shuffled - std::stable_sort/0/9             555590 ns       544085 ns         1120
sorting std::vector<unsigned> of size 10000 and max element <= 1000 - shuffled - std::sort/0/10                   466985 ns       468750 ns         1400
sorting std::vector<unsigned> of size 10000 and max element <= 1000 - sorted - alg::counting_sort/1/6              26410 ns        26786 ns        37333
sorting std::vector<unsigned> of size 10000 and max element <= 1000 - sorted - alg::radix_sort/1/7                287388 ns       272042 ns         2240
sorting std::vector<unsigned> of size 10000 and max element <= 1000 - sorted - std::stable_sort/1/9                90988 ns        94164 ns         7467
sorting std::vector<unsigned> of size 10000 and max element <= 1000 - sorted - std::sort/1/10                     117619 ns       117187 ns         5600
sorting std::vector<unsigned> of size 10000 and max element <= 1000 - reverse sorted - alg::counting_sort/2/6      27009 ns        26995 ns        24889
sorting std::vector<unsigned> of size 10000 and max element <= 1000 - reverse sorted - alg::radix_sort/2/7        566725 ns       531878 ns         1792
sorting std::vector<unsigned> of size 10000 and max element <= 1000 - reverse sorted - std::stable_sort/2/9       169310 ns       167426 ns         3733
sorting std::vector<unsigned> of size 10000 and max element <= 1000 - reverse sorted - std::sort/2/10             134904 ns       128691 ns         4978

sorting std::vector<double> of size 10000 where 0<=vec[i]<1 - shuffled - alg::bucket_sort/0/8                    1279151 ns      1286396 ns          498
sorting std::vector<double> of size 10000 where 0<=vec[i]<1 - shuffled - std::stable_sort/0/9                     734219 ns       725446 ns         1120
sorting std::vector<double> of size 10000 where 0<=vec[i]<1 - shuffled - std::sort/0/10                           719776 ns       697545 ns          896
sorting std::vector<double> of size 10000 where 0<=vec[i]<1 - sorted - alg::bucket_sort/1/8                      1224998 ns      1255580 ns          560
sorting std::vector<double> of size 10000 where 0<=vec[i]<1 - sorted - std::stable_sort/1/9                       119184 ns       119978 ns         5600
sorting std::vector<double> of size 10000 where 0<=vec[i]<1 - sorted - std::sort/1/10                             142770 ns       142997 ns         4480
sorting std::vector<double> of size 10000 where 0<=vec[i]<1 - reverse sorted - alg::bucket_sort/2/8              1231825 ns      1223645 ns          498
sorting std::vector<double> of size 10000 where 0<=vec[i]<1 - reverse sorted - std::stable_sort/2/9               367886 ns       368238 ns         1867
sorting std::vector<double> of size 10000 where 0<=vec[i]<1 - reverse sorted - std::sort/2/10                     111245 ns       111607 ns         5600
```

## Unit Testing

All of the implemented functions are tested with **[Catch2](https://github.com/catchorg/Catch2)**.

## Contribution

### Project Goals

We aim to implement more and more sorting algorithms.  
We also plan to improve performance as much as possible.

### TODO List

- Use a better algorithm for choosing pivot in quick sort.
- Add an **in-place** merge sort implementation.
- Implement tim sort, shell sort, comb sort, shaker sort, etc.
