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

struct TestType { enum type {
    random_shuffled,
    sorted,
    reverse_sorted,
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

template<typename T>
std::vector<T> get_random_vector(std::size_t size) = delete;

template<>
inline std::vector<int> get_random_vector<int>(std::size_t size) {
    return random_int_vector<int>(size);
}

template<typename T>
static void bm_sort_vector(benchmark::State& state) {
    static auto vec = get_random_vector<T>(10000U);

    auto test = static_cast<TestType::type>(state.range(0));
    switch (test) {
        case TestType::random_shuffled:
            break;
        case TestType::sorted:
            std::sort(vec.begin(), vec.end());
            break;
        case TestType::reverse_sorted:
            std::sort(vec.rbegin(), vec.rend());
            break;
    }

    for (auto _ : state) {
        state.PauseTiming();
        auto tmp = vec;
        auto func_as_enum = static_cast<SortFunc::type>(state.range(1));
        auto sort_func = func_map[func_as_enum];
        state.ResumeTiming();

        sort_func(tmp.begin(), tmp.end());
    }
}

static void bm_counting_sort_and_radix_sort(benchmark::State& state) {
    constexpr auto MAX = 1000U;
    static auto vec = random_int_vector(10000, MAX);

    auto test = static_cast<TestType::type>(state.range(0));
    switch (test) {
        case TestType::random_shuffled:
            break;
        case TestType::sorted:
            std::sort(vec.begin(), vec.end());
            break;
        case TestType::reverse_sorted:
            std::sort(vec.rbegin(), vec.rend());
            break;
    }

    for (auto _ : state) {
        state.PauseTiming();

        auto func = static_cast<SortFunc::type>(state.range(1));
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

BENCHMARK_TEMPLATE1(bm_sort_vector, int)
    ->Args({ TestType::random_shuffled, SortFunc::bubble_sort })
    ->Args({ TestType::random_shuffled, SortFunc::insertion_sort })
    ->Args({ TestType::random_shuffled, SortFunc::selection_sort })
    ->Args({ TestType::random_shuffled, SortFunc::heap_sort })
    ->Args({ TestType::random_shuffled, SortFunc::merge_sort })
    ->Args({ TestType::random_shuffled, SortFunc::quick_sort })
    ->Args({ TestType::random_shuffled, SortFunc::std_sort })
    ->Args({ TestType::random_shuffled, SortFunc::std_stable_sort })

    ->Args({ TestType::sorted, SortFunc::bubble_sort })
    ->Args({ TestType::sorted, SortFunc::insertion_sort })
    ->Args({ TestType::sorted, SortFunc::selection_sort })
    ->Args({ TestType::sorted, SortFunc::heap_sort })
    ->Args({ TestType::sorted, SortFunc::merge_sort })
    ->Args({ TestType::sorted, SortFunc::quick_sort })
    ->Args({ TestType::sorted, SortFunc::std_sort })
    ->Args({ TestType::sorted, SortFunc::std_stable_sort })

    ->Args({ TestType::reverse_sorted, SortFunc::bubble_sort })
    ->Args({ TestType::reverse_sorted, SortFunc::insertion_sort })
    ->Args({ TestType::reverse_sorted, SortFunc::selection_sort })
    ->Args({ TestType::reverse_sorted, SortFunc::heap_sort })
    ->Args({ TestType::reverse_sorted, SortFunc::merge_sort })
    ->Args({ TestType::reverse_sorted, SortFunc::quick_sort })
    ->Args({ TestType::reverse_sorted, SortFunc::std_sort })
    ->Args({ TestType::reverse_sorted, SortFunc::std_stable_sort });

BENCHMARK(bm_counting_sort_and_radix_sort)
    ->Args({ TestType::random_shuffled, SortFunc::counting_sort })
    ->Args({ TestType::random_shuffled, SortFunc::radix_sort })
    ->Args({ TestType::random_shuffled, SortFunc::std_sort })
    ->Args({ TestType::random_shuffled, SortFunc::std_stable_sort })

    ->Args({ TestType::sorted, SortFunc::counting_sort })
    ->Args({ TestType::sorted, SortFunc::radix_sort })
    ->Args({ TestType::sorted, SortFunc::std_sort })
    ->Args({ TestType::sorted, SortFunc::std_stable_sort })

    ->Args({ TestType::reverse_sorted, SortFunc::counting_sort })
    ->Args({ TestType::reverse_sorted, SortFunc::radix_sort })
    ->Args({ TestType::reverse_sorted, SortFunc::std_sort })
    ->Args({ TestType::reverse_sorted, SortFunc::std_stable_sort });

BENCHMARK_MAIN();
