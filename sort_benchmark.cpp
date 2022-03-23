#include <algorithm>
#include <chrono>
#include <functional>
#include <random>
#include <unordered_map>
#include <vector>

#include <benchmark/benchmark.h>

#include "sort.hpp"

struct SortFunc { enum type {
    bubble_sort,
    insertion_sort,
    selection_sort,
    merge_sort,
    quick_sort,
    heap_sort,
    std_sort,
    std_stable_sort,
}; };

using FuncPtr = void (*)(std::vector<int>::iterator, std::vector<int>::iterator);

static std::unordered_map<SortFunc::type, FuncPtr> func_map = {
    { SortFunc::bubble_sort,     [](auto first, auto last) { alg::bubble_sort(first, last); } },
    { SortFunc::insertion_sort,  [](auto first, auto last) { alg::insertion_sort(first, last); } },
    { SortFunc::selection_sort,  [](auto first, auto last) { alg::selection_sort(first, last); } },
    { SortFunc::merge_sort,      [](auto first, auto last) { alg::merge_sort(first, last); } },
    { SortFunc::quick_sort,      [](auto first, auto last) { alg::quick_sort(first, last); } },
    { SortFunc::heap_sort,       [](auto first, auto last) { alg::heap_sort(first, last); } },
    { SortFunc::std_sort,        [](auto first, auto last) { std::sort(first, last); } },
    { SortFunc::std_stable_sort, [](auto first, auto last) { std::stable_sort(first, last); } },
};

static std::vector<int> random_int_vector(std::size_t size) {
    std::mt19937 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<> dist;

    std::vector<int> vec(size);
    std::generate(vec.begin(), vec.end(), [&gen, &dist]() -> int { return dist(gen); });

    return vec;
}

static std::vector<int> sorted_random_int_vector(std::size_t size) {
    auto vec = random_int_vector(size);
    std::sort(vec.begin(), vec.end());
    return vec;
}

static std::vector<int> reverse_sorted_random_int_vector(std::size_t size) {
    auto vec = random_int_vector(size);
    std::sort(vec.rbegin(), vec.rend());
    return vec;
}

static void bm_sort_random_vector(benchmark::State& state) {
    static auto vec = random_int_vector(10000);
    for (auto _ : state) {
        state.PauseTiming();
        auto tmp = vec;
        auto func_as_enum = static_cast<SortFunc::type>(state.range(0));
        auto sort_func = func_map[func_as_enum];
        state.ResumeTiming();

        sort_func(tmp.begin(), tmp.end());
    }
}

static void bm_sort_sorted_vector(benchmark::State& state) {
    static auto vec = sorted_random_int_vector(10000);
    for (auto _ : state) {
        state.PauseTiming();
        auto tmp = vec;
        auto func_as_enum = static_cast<SortFunc::type>(state.range(0));
        auto sort_func = func_map[func_as_enum];
        state.ResumeTiming();

        sort_func(tmp.begin(), tmp.end());
    }
}

static void bm_sort_reverse_sorted_vector(benchmark::State& state) {
    static auto vec = reverse_sorted_random_int_vector(10000);
    for (auto _ : state) {
        state.PauseTiming();
        auto tmp = vec;
        auto func_as_enum = static_cast<SortFunc::type>(state.range(0));
        auto sort_func = func_map[func_as_enum];
        state.ResumeTiming();

        sort_func(tmp.begin(), tmp.end());
    }
}

BENCHMARK(bm_sort_random_vector)
    ->Arg(SortFunc::bubble_sort)
    ->Arg(SortFunc::insertion_sort)
    ->Arg(SortFunc::selection_sort)
    ->Arg(SortFunc::merge_sort)
    ->Arg(SortFunc::quick_sort)
    ->Arg(SortFunc::heap_sort)
    ->Arg(SortFunc::std_sort)
    ->Arg(SortFunc::std_stable_sort);

BENCHMARK(bm_sort_sorted_vector)
    ->Arg(SortFunc::bubble_sort)
    ->Arg(SortFunc::insertion_sort)
    ->Arg(SortFunc::selection_sort)
    ->Arg(SortFunc::merge_sort)
    ->Arg(SortFunc::quick_sort)
    ->Arg(SortFunc::heap_sort)
    ->Arg(SortFunc::std_sort)
    ->Arg(SortFunc::std_stable_sort);

BENCHMARK(bm_sort_reverse_sorted_vector)
    ->Arg(SortFunc::bubble_sort)
    ->Arg(SortFunc::insertion_sort)
    ->Arg(SortFunc::selection_sort)
    ->Arg(SortFunc::merge_sort)
    ->Arg(SortFunc::quick_sort)
    ->Arg(SortFunc::heap_sort)
    ->Arg(SortFunc::std_sort)
    ->Arg(SortFunc::std_stable_sort);

BENCHMARK_MAIN();
