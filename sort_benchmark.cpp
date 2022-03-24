#include <algorithm>
#include <chrono>
#include <limits>
#include <random>
#include <unordered_map>
#include <vector>

#include <benchmark/benchmark.h>

#include "sort.hpp"

struct SortFunc { enum type {
    bubble_sort,
    insertion_sort,
    selection_sort,
    heap_sort,
    merge_sort,
    quick_sort,
    counting_sort,
    radix_sort,
    bucket_sort,
    std_sort,
    std_stable_sort,
}; };

using FuncPtr = void (*)(std::vector<int>::iterator, std::vector<int>::iterator);

static std::unordered_map<SortFunc::type, FuncPtr> func_map = {
    { SortFunc::bubble_sort,     [](auto first, auto last) { alg::bubble_sort(first, last); } },
    { SortFunc::insertion_sort,  [](auto first, auto last) { alg::insertion_sort(first, last); } },
    { SortFunc::selection_sort,  [](auto first, auto last) { alg::selection_sort(first, last); } },
    { SortFunc::heap_sort,       [](auto first, auto last) { alg::heap_sort(first, last); } },
    { SortFunc::merge_sort,      [](auto first, auto last) { alg::merge_sort(first, last); } },
    { SortFunc::quick_sort,      [](auto first, auto last) { alg::quick_sort(first, last); } },
    { SortFunc::std_sort,        [](auto first, auto last) { std::sort(first, last); } },
    { SortFunc::std_stable_sort, [](auto first, auto last) { std::stable_sort(first, last); } },
};

template<typename Int>
static std::vector<Int> random_int_vector(std::size_t size, Int max = std::numeric_limits<Int>::max()) {
    std::mt19937 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<Int> dist(std::numeric_limits<Int>::min(), max);

    std::vector<Int> vec(size);
    std::generate(vec.begin(), vec.end(), [&gen, &dist]() -> Int { return dist(gen); });

    return vec;
}

template<typename Int>
static std::vector<Int> sorted_random_int_vector(std::size_t size, Int max = std::numeric_limits<Int>::max()) {
    auto vec = random_int_vector(size, max);
    std::sort(vec.begin(), vec.end());
    return vec;
}

template<typename Int>
static std::vector<Int> reverse_sorted_random_int_vector(std::size_t size, Int max = std::numeric_limits<Int>::max()) {
    auto vec = random_int_vector(size, max);
    std::sort(vec.rbegin(), vec.rend());
    return vec;
}

static void bm_sort_vector(benchmark::State& state) {
    static auto vec = random_int_vector<int>(10000);
    for (auto _ : state) {
        state.PauseTiming();
        auto tmp = vec;
        auto func_as_enum = static_cast<SortFunc::type>(state.range(0));
        auto sort_func = func_map[func_as_enum];
        state.ResumeTiming();

        sort_func(tmp.begin(), tmp.end());
    }
}

static void bm_sort_vector_sorted(benchmark::State& state) {
    static auto vec = sorted_random_int_vector<int>(10000);
    for (auto _ : state) {
        state.PauseTiming();
        auto tmp = vec;
        auto func_as_enum = static_cast<SortFunc::type>(state.range(0));
        auto sort_func = func_map[func_as_enum];
        state.ResumeTiming();

        sort_func(tmp.begin(), tmp.end());
    }
}

static void bm_sort_vector_reverse_sorted(benchmark::State& state) {
    static auto vec = reverse_sorted_random_int_vector<int>(10000);
    for (auto _ : state) {
        state.PauseTiming();
        auto tmp = vec;
        auto func_as_enum = static_cast<SortFunc::type>(state.range(0));
        auto sort_func = func_map[func_as_enum];
        state.ResumeTiming();

        sort_func(tmp.begin(), tmp.end());
    }
}

static void bm_counting_sort_and_radix_sort(benchmark::State& state) {
    constexpr auto MAX = 1000U;
    static auto vec = random_int_vector(10000, MAX);
    for (auto _ : state) {
        state.PauseTiming();

        auto func = static_cast<SortFunc::type>(state.range(0));
        auto tmp = vec;

        switch (func) {
        case SortFunc::counting_sort:
            state.ResumeTiming();
            alg::counting_sort(tmp.begin(), tmp.end(), MAX);
            break;
        case SortFunc::radix_sort:
            state.ResumeTiming();
            alg::radix_sort(tmp.begin(), tmp.end(), MAX);
            break;
        case SortFunc::std_sort:
            state.ResumeTiming();
            std::sort(tmp.begin(), tmp.end());
            break;
        case SortFunc::std_stable_sort:
            state.ResumeTiming();
            std::stable_sort(tmp.begin(), tmp.end());
            break;
        }
    }
}

static void bm_counting_sort_and_radix_sort_sorted(benchmark::State& state) {
    constexpr auto MAX = 1000U;
    static auto vec = sorted_random_int_vector(10000, MAX);
    for (auto _ : state) {
        state.PauseTiming();

        auto func = static_cast<SortFunc::type>(state.range(0));
        auto tmp = vec;

        switch (func) {
        case SortFunc::counting_sort:
            state.ResumeTiming();
            alg::counting_sort(tmp.begin(), tmp.end(), MAX);
            break;
        case SortFunc::radix_sort:
            state.ResumeTiming();
            alg::radix_sort(tmp.begin(), tmp.end(), MAX);
            break;
        case SortFunc::std_sort:
            state.ResumeTiming();
            std::sort(tmp.begin(), tmp.end());
            break;
        case SortFunc::std_stable_sort:
            state.ResumeTiming();
            std::stable_sort(tmp.begin(), tmp.end());
            break;
        }
    }
}

BENCHMARK(bm_sort_vector)
    ->Arg(SortFunc::bubble_sort)
    ->Arg(SortFunc::insertion_sort)
    ->Arg(SortFunc::selection_sort)
    ->Arg(SortFunc::heap_sort)
    ->Arg(SortFunc::merge_sort)
    ->Arg(SortFunc::quick_sort)
    ->Arg(SortFunc::std_sort)
    ->Arg(SortFunc::std_stable_sort);

BENCHMARK(bm_sort_vector_sorted)
    ->Arg(SortFunc::bubble_sort)
    ->Arg(SortFunc::insertion_sort)
    ->Arg(SortFunc::selection_sort)
    ->Arg(SortFunc::heap_sort)
    ->Arg(SortFunc::merge_sort)
    ->Arg(SortFunc::quick_sort)
    ->Arg(SortFunc::std_sort)
    ->Arg(SortFunc::std_stable_sort);

BENCHMARK(bm_sort_vector_reverse_sorted)
    ->Arg(SortFunc::bubble_sort)
    ->Arg(SortFunc::insertion_sort)
    ->Arg(SortFunc::selection_sort)
    ->Arg(SortFunc::heap_sort)
    ->Arg(SortFunc::merge_sort)
    ->Arg(SortFunc::quick_sort)
    ->Arg(SortFunc::std_sort)
    ->Arg(SortFunc::std_stable_sort);

BENCHMARK(bm_counting_sort_and_radix_sort)
    ->Arg(SortFunc::counting_sort)
    ->Arg(SortFunc::radix_sort)
    ->Arg(SortFunc::std_sort)
    ->Arg(SortFunc::std_stable_sort);

BENCHMARK(bm_counting_sort_and_radix_sort_sorted)
    ->Arg(SortFunc::counting_sort)
    ->Arg(SortFunc::radix_sort)
    ->Arg(SortFunc::std_sort)
    ->Arg(SortFunc::std_stable_sort);

BENCHMARK_MAIN();
