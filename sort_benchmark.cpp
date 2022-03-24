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

template<class Iterator>
using SortFuncPtr = void (*)(Iterator, Iterator);

template<class Iterator>
static std::unordered_map<SortFunc::type, SortFuncPtr<Iterator>> func_map = {
    { SortFunc::bubble_sort,     alg::bubble_sort },
    { SortFunc::insertion_sort,  alg::insertion_sort },
    { SortFunc::selection_sort,  alg::selection_sort },
    { SortFunc::heap_sort,       alg::heap_sort },
    { SortFunc::merge_sort,      alg::merge_sort },
    { SortFunc::quick_sort,      alg::quick_sort },
    { SortFunc::std_sort,        std::sort },
    { SortFunc::std_stable_sort, std::stable_sort },
};

template<class Int>
static std::vector<Int> random_int_vector(std::size_t size, Int max = std::numeric_limits<Int>::max()) {
    static std::mt19937 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<Int> dist(std::numeric_limits<Int>::min(), max);

    std::vector<Int> vec(size);
    std::generate(vec.begin(), vec.end(), [&dist]() {
        return dist(gen);
    });

    return vec;
}

static std::string random_string(std::size_t min_len, std::size_t max_len) {
    const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    static std::mt19937 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    static std::uniform_int_distribution<std::size_t> index_dist(0U, sizeof(charset) - 2);
    std::uniform_int_distribution<std::size_t> length_dist(min_len, max_len);

    auto len = length_dist(gen);
    std::string str(len, '\0');

    std::generate_n(str.begin(), len, [charset]() {
        return charset[index_dist(gen)];
    });

    return str;
}

template<class T>
static std::vector<T> get_random_vector(std::size_t size) = delete;

template<>
inline std::vector<int> get_random_vector<int>(std::size_t size) {
    return random_int_vector<int>(size);
}

template<>
inline std::vector<std::string> get_random_vector<std::string>(std::size_t size) {
    std::vector<std::string> vec(size);

    std::generate(vec.begin(), vec.end(), []() {
        return random_string(0U, 1000U);
    });

    return vec;
}

template<class T>
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
        auto sort_func = func_map<typename std::vector<T>::iterator>[func_as_enum];
        state.ResumeTiming();

        sort_func(tmp.begin(), tmp.end());
    }
}

static void bm_counting_sort_and_radix_sort(benchmark::State& state) {
    constexpr auto MAX = 1000U;
    static auto vec = random_int_vector(10000U, MAX);

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

BENCHMARK_TEMPLATE1(bm_sort_vector, std::string)
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
