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
    std_stable_sort,
    std_sort,
}; };

struct TestType { enum type {
    shuffled,
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
    { SortFunc::std_stable_sort, std::stable_sort },
    { SortFunc::std_sort,        std::sort },
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

static std::vector<double> random_double_vector(std::size_t size, double min, double max) {
    static std::mt19937 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<> dist(min, max);

    std::vector<double> vec(size);

    std::generate(vec.begin(), vec.end(), [&dist]() {
        return dist(gen);
    });

    return vec;
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
    static auto last_test = TestType::shuffled;

    auto test = static_cast<TestType::type>(state.range(0));
    if (test != last_test) {
        switch (test) {
        case TestType::shuffled:
            break;
        case TestType::sorted:
            std::sort(vec.begin(), vec.end());
            break;
        case TestType::reverse_sorted:
            std::sort(vec.rbegin(), vec.rend());
            break;
        }
        last_test = test;
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
    static auto last_test = TestType::shuffled;

    auto test = static_cast<TestType::type>(state.range(0));
    if (test != last_test) {
        switch (test) {
        case TestType::shuffled:
            break;
        case TestType::sorted:
            std::sort(vec.begin(), vec.end());
            break;
        case TestType::reverse_sorted:
            std::sort(vec.rbegin(), vec.rend());
            break;
        }
        last_test = test;
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

static void bm_bucket_sort(benchmark::State& state) {
    static auto vec = random_double_vector(10000U, 0.0, 1.0);
    static auto last_test = TestType::shuffled;

    auto test = static_cast<TestType::type>(state.range(0));
    if (test != last_test) {
        switch (test) {
        case TestType::shuffled:
            break;
        case TestType::sorted:
            std::sort(vec.begin(), vec.end());
            break;
        case TestType::reverse_sorted:
            std::sort(vec.rbegin(), vec.rend());
            break;
        }
        last_test = test;
    }

    for (auto _ : state) {
        state.PauseTiming();

        auto func = static_cast<SortFunc::type>(state.range(1));
        auto tmp = vec;

        switch (func) {
        case SortFunc::bucket_sort:
            state.ResumeTiming();
            alg::bucket_sort(tmp.begin(), tmp.end());
            break;
        case SortFunc::std_stable_sort:
            state.ResumeTiming();
            std::stable_sort(tmp.begin(), tmp.end());
            break;
        case SortFunc::std_sort:
            state.ResumeTiming();
            std::sort(tmp.begin(), tmp.end());
            break;
        }
    }
}

//////////////////////
// std::vector<int> //
//////////////////////
BENCHMARK(bm_sort_vector<int>)
    ->Name("sorting std::vector<int> of size 10000 - shuffled - alg::bubble_sort")
    ->Args({ TestType::shuffled, SortFunc::bubble_sort });

BENCHMARK(bm_sort_vector<int>)
    ->Name("sorting std::vector<int> of size 10000 - shuffled - alg::insertion_sort")
    ->Args({ TestType::shuffled, SortFunc::insertion_sort });

BENCHMARK(bm_sort_vector<int>)
    ->Name("sorting std::vector<int> of size 10000 - shuffled - alg::selection_sort")
    ->Args({ TestType::shuffled, SortFunc::selection_sort });

BENCHMARK(bm_sort_vector<int>)
    ->Name("sorting std::vector<int> of size 10000 - shuffled - alg::heap_sort")
    ->Args({ TestType::shuffled, SortFunc::heap_sort });

BENCHMARK(bm_sort_vector<int>)
    ->Name("sorting std::vector<int> of size 10000 - shuffled - alg::merge_sort")
    ->Args({ TestType::shuffled, SortFunc::merge_sort });

BENCHMARK(bm_sort_vector<int>)
    ->Name("sorting std::vector<int> of size 10000 - shuffled - alg::quick_sort")
    ->Args({ TestType::shuffled, SortFunc::quick_sort });

BENCHMARK(bm_sort_vector<int>)
    ->Name("sorting std::vector<int> of size 10000 - shuffled - std::stable_sort")
    ->Args({ TestType::shuffled, SortFunc::std_stable_sort });

BENCHMARK(bm_sort_vector<int>)
    ->Name("sorting std::vector<int> of size 10000 - shuffled - std::sort")
    ->Args({ TestType::shuffled, SortFunc::std_sort });

BENCHMARK(bm_sort_vector<int>)
    ->Name("sorting std::vector<int> of size 10000 - sorted - alg::bubble_sort")
    ->Args({ TestType::sorted, SortFunc::bubble_sort });

BENCHMARK(bm_sort_vector<int>)
    ->Name("sorting std::vector<int> of size 10000 - sorted - alg::insertion_sort")
    ->Args({ TestType::sorted, SortFunc::insertion_sort });

BENCHMARK(bm_sort_vector<int>)
    ->Name("sorting std::vector<int> of size 10000 - sorted - alg::selection_sort")
    ->Args({ TestType::sorted, SortFunc::selection_sort });

BENCHMARK(bm_sort_vector<int>)
    ->Name("sorting std::vector<int> of size 10000 - sorted - alg::heap_sort")
    ->Args({ TestType::sorted, SortFunc::heap_sort });

BENCHMARK(bm_sort_vector<int>)
    ->Name("sorting std::vector<int> of size 10000 - sorted - alg::merge_sort")
    ->Args({ TestType::sorted, SortFunc::merge_sort });

BENCHMARK(bm_sort_vector<int>)
    ->Name("sorting std::vector<int> of size 10000 - sorted - alg::quick_sort")
    ->Args({ TestType::sorted, SortFunc::quick_sort });

BENCHMARK(bm_sort_vector<int>)
    ->Name("sorting std::vector<int> of size 10000 - sorted - std::stable_sort")
    ->Args({ TestType::sorted, SortFunc::std_stable_sort });

BENCHMARK(bm_sort_vector<int>)
    ->Name("sorting std::vector<int> of size 10000 - sorted - std::sort")
    ->Args({ TestType::sorted, SortFunc::std_sort });

BENCHMARK(bm_sort_vector<int>)
    ->Name("sorting std::vector<int> of size 10000 - reverse sorted - alg::bubble_sort")
    ->Args({ TestType::reverse_sorted, SortFunc::bubble_sort });

BENCHMARK(bm_sort_vector<int>)
    ->Name("sorting std::vector<int> of size 10000 - reverse sorted - alg::insertion_sort")
    ->Args({ TestType::reverse_sorted, SortFunc::insertion_sort });

BENCHMARK(bm_sort_vector<int>)
    ->Name("sorting std::vector<int> of size 10000 - reverse sorted - alg::selection_sort")
    ->Args({ TestType::reverse_sorted, SortFunc::selection_sort });

BENCHMARK(bm_sort_vector<int>)
    ->Name("sorting std::vector<int> of size 10000 - reverse sorted - alg::heap_sort")
    ->Args({ TestType::reverse_sorted, SortFunc::heap_sort });

BENCHMARK(bm_sort_vector<int>)
    ->Name("sorting std::vector<int> of size 10000 - reverse sorted - alg::merge_sort")
    ->Args({ TestType::reverse_sorted, SortFunc::merge_sort });

BENCHMARK(bm_sort_vector<int>)
    ->Name("sorting std::vector<int> of size 10000 - reverse sorted - alg::quick_sort")
    ->Args({ TestType::reverse_sorted, SortFunc::quick_sort });

BENCHMARK(bm_sort_vector<int>)
    ->Name("sorting std::vector<int> of size 10000 - reverse sorted - std::stable_sort")
    ->Args({ TestType::reverse_sorted, SortFunc::std_stable_sort });

BENCHMARK(bm_sort_vector<int>)
    ->Name("sorting std::vector<int> of size 10000 - reverse sorted - std::sort")
    ->Args({ TestType::reverse_sorted, SortFunc::std_sort });

//////////////////////////////
// std::vector<std::string> //
//////////////////////////////
BENCHMARK(bm_sort_vector<std::string>)
    ->Name("sorting std::vector<std::string> of size 10000 - shuffled - alg::bubble_sort")
    ->Args({ TestType::shuffled, SortFunc::bubble_sort });

BENCHMARK(bm_sort_vector<std::string>)
    ->Name("sorting std::vector<std::string> of size 10000 - shuffled - alg::insertion_sort")
    ->Args({ TestType::shuffled, SortFunc::insertion_sort });

BENCHMARK(bm_sort_vector<std::string>)
    ->Name("sorting std::vector<std::string> of size 10000 - shuffled - alg::selection_sort")
    ->Args({ TestType::shuffled, SortFunc::selection_sort });

BENCHMARK(bm_sort_vector<std::string>)
    ->Name("sorting std::vector<std::string> of size 10000 - shuffled - alg::heap_sort")
    ->Args({ TestType::shuffled, SortFunc::heap_sort });

BENCHMARK(bm_sort_vector<std::string>)
    ->Name("sorting std::vector<std::string> of size 10000 - shuffled - alg::merge_sort")
    ->Args({ TestType::shuffled, SortFunc::merge_sort });

BENCHMARK(bm_sort_vector<std::string>)
    ->Name("sorting std::vector<std::string> of size 10000 - shuffled - alg::quick_sort")
    ->Args({ TestType::shuffled, SortFunc::quick_sort });

BENCHMARK(bm_sort_vector<std::string>)
    ->Name("sorting std::vector<std::string> of size 10000 - shuffled - std::stable_sort")
    ->Args({ TestType::shuffled, SortFunc::std_stable_sort });

BENCHMARK(bm_sort_vector<std::string>)
    ->Name("sorting std::vector<std::string> of size 10000 - shuffled - std::sort")
    ->Args({ TestType::shuffled, SortFunc::std_sort });

BENCHMARK(bm_sort_vector<std::string>)
    ->Name("sorting std::vector<std::string> of size 10000 - sorted - alg::bubble_sort")
    ->Args({ TestType::sorted, SortFunc::bubble_sort });

BENCHMARK(bm_sort_vector<std::string>)
    ->Name("sorting std::vector<std::string> of size 10000 - sorted - alg::insertion_sort")
    ->Args({ TestType::sorted, SortFunc::insertion_sort });

BENCHMARK(bm_sort_vector<std::string>)
    ->Name("sorting std::vector<std::string> of size 10000 - sorted - alg::selection_sort")
    ->Args({ TestType::sorted, SortFunc::selection_sort });

BENCHMARK(bm_sort_vector<std::string>)
    ->Name("sorting std::vector<std::string> of size 10000 - sorted - alg::heap_sort")
    ->Args({ TestType::sorted, SortFunc::heap_sort });

BENCHMARK(bm_sort_vector<std::string>)
    ->Name("sorting std::vector<std::string> of size 10000 - sorted - alg::merge_sort")
    ->Args({ TestType::sorted, SortFunc::merge_sort });

BENCHMARK(bm_sort_vector<std::string>)
    ->Name("sorting std::vector<std::string> of size 10000 - sorted - alg::quick_sort")
    ->Args({ TestType::sorted, SortFunc::quick_sort });

BENCHMARK(bm_sort_vector<std::string>)
    ->Name("sorting std::vector<std::string> of size 10000 - sorted - std::stable_sort")
    ->Args({ TestType::sorted, SortFunc::std_stable_sort });

BENCHMARK(bm_sort_vector<std::string>)
    ->Name("sorting std::vector<std::string> of size 10000 - sorted - std::sort")
    ->Args({ TestType::sorted, SortFunc::std_sort });

BENCHMARK(bm_sort_vector<std::string>)
    ->Name("sorting std::vector<std::string> of size 10000 - reverse sorted - alg::bubble_sort")
    ->Args({ TestType::reverse_sorted, SortFunc::bubble_sort });

BENCHMARK(bm_sort_vector<std::string>)
    ->Name("sorting std::vector<std::string> of size 10000 - reverse sorted - alg::insertion_sort")
    ->Args({ TestType::reverse_sorted, SortFunc::insertion_sort });

BENCHMARK(bm_sort_vector<std::string>)
    ->Name("sorting std::vector<std::string> of size 10000 - reverse sorted - alg::selection_sort")
    ->Args({ TestType::reverse_sorted, SortFunc::selection_sort });

BENCHMARK(bm_sort_vector<std::string>)
    ->Name("sorting std::vector<std::string> of size 10000 - reverse sorted - alg::heap_sort")
    ->Args({ TestType::reverse_sorted, SortFunc::heap_sort });

BENCHMARK(bm_sort_vector<std::string>)
    ->Name("sorting std::vector<std::string> of size 10000 - reverse sorted - alg::merge_sort")
    ->Args({ TestType::reverse_sorted, SortFunc::merge_sort });

BENCHMARK(bm_sort_vector<std::string>)
    ->Name("sorting std::vector<std::string> of size 10000 - reverse sorted - alg::quick_sort")
    ->Args({ TestType::reverse_sorted, SortFunc::quick_sort });

BENCHMARK(bm_sort_vector<std::string>)
    ->Name("sorting std::vector<std::string> of size 10000 - reverse sorted - std::stable_sort")
    ->Args({ TestType::reverse_sorted, SortFunc::std_stable_sort });

BENCHMARK(bm_sort_vector<std::string>)
    ->Name("sorting std::vector<std::string> of size 10000 - reverse sorted - std::sort")
    ->Args({ TestType::reverse_sorted, SortFunc::std_sort });

//////////////////////////////////
// counting sort and radix sort //
//////////////////////////////////
BENCHMARK(bm_counting_sort_and_radix_sort)
    ->Name("sorting std::vector<unsigned> of size 10000 and max element <= 1000 - shuffled - alg::counting_sort")
    ->Args({ TestType::shuffled, SortFunc::counting_sort });

BENCHMARK(bm_counting_sort_and_radix_sort)
    ->Name("sorting std::vector<unsigned> of size 10000 and max element <= 1000 - shuffled - alg::radix_sort")
    ->Args({ TestType::shuffled, SortFunc::radix_sort });

BENCHMARK(bm_counting_sort_and_radix_sort)
    ->Name("sorting std::vector<unsigned> of size 10000 and max element <= 1000 - shuffled - std::stable_sort")
    ->Args({ TestType::shuffled, SortFunc::std_stable_sort });

BENCHMARK(bm_counting_sort_and_radix_sort)
    ->Name("sorting std::vector<unsigned> of size 10000 and max element <= 1000 - shuffled - std::sort")
    ->Args({ TestType::shuffled, SortFunc::std_sort });

BENCHMARK(bm_counting_sort_and_radix_sort)
    ->Name("sorting std::vector<unsigned> of size 10000 and max element <= 1000 - sorted - alg::counting_sort")
    ->Args({ TestType::sorted, SortFunc::counting_sort });

BENCHMARK(bm_counting_sort_and_radix_sort)
    ->Name("sorting std::vector<unsigned> of size 10000 and max element <= 1000 - sorted - alg::radix_sort")
    ->Args({ TestType::sorted, SortFunc::radix_sort });

BENCHMARK(bm_counting_sort_and_radix_sort)
    ->Name("sorting std::vector<unsigned> of size 10000 and max element <= 1000 - sorted - std::stable_sort")
    ->Args({ TestType::sorted, SortFunc::std_stable_sort });

BENCHMARK(bm_counting_sort_and_radix_sort)
    ->Name("sorting std::vector<unsigned> of size 10000 and max element <= 1000 - sorted - std::sort")
    ->Args({ TestType::sorted, SortFunc::std_sort });

BENCHMARK(bm_counting_sort_and_radix_sort)
    ->Name("sorting std::vector<unsigned> of size 10000 and max element <= 1000 - reverse sorted - alg::counting_sort")
    ->Args({ TestType::reverse_sorted, SortFunc::counting_sort });

BENCHMARK(bm_counting_sort_and_radix_sort)
    ->Name("sorting std::vector<unsigned> of size 10000 and max element <= 1000 - reverse sorted - alg::radix_sort")
    ->Args({ TestType::reverse_sorted, SortFunc::radix_sort });

BENCHMARK(bm_counting_sort_and_radix_sort)
    ->Name("sorting std::vector<unsigned> of size 10000 and max element <= 1000 - reverse sorted - std::stable_sort")
    ->Args({ TestType::reverse_sorted, SortFunc::std_stable_sort });

BENCHMARK(bm_counting_sort_and_radix_sort)
    ->Name("sorting std::vector<unsigned> of size 10000 and max element <= 1000 - reverse sorted - std::sort")
    ->Args({ TestType::reverse_sorted, SortFunc::std_sort });

/////////////////
// bucket sort //
/////////////////
BENCHMARK(bm_bucket_sort)
    ->Name("sorting std::vector<double> of size 10000 where 0<=vec[i]<1 - shuffled - alg::bucket_sort")
    ->Args({ TestType::shuffled, SortFunc::bucket_sort });

BENCHMARK(bm_bucket_sort)
    ->Name("sorting std::vector<double> of size 10000 where 0<=vec[i]<1 - shuffled - std::stable_sort")
    ->Args({ TestType::shuffled, SortFunc::std_stable_sort });

BENCHMARK(bm_bucket_sort)
    ->Name("sorting std::vector<double> of size 10000 where 0<=vec[i]<1 - shuffled - std::sort")
    ->Args({ TestType::shuffled, SortFunc::std_sort });

BENCHMARK(bm_bucket_sort)
    ->Name("sorting std::vector<double> of size 10000 where 0<=vec[i]<1 - sorted - alg::bucket_sort")
    ->Args({ TestType::sorted, SortFunc::bucket_sort });

BENCHMARK(bm_bucket_sort)
    ->Name("sorting std::vector<double> of size 10000 where 0<=vec[i]<1 - sorted - std::stable_sort")
    ->Args({ TestType::sorted, SortFunc::std_stable_sort });

BENCHMARK(bm_bucket_sort)
    ->Name("sorting std::vector<double> of size 10000 where 0<=vec[i]<1 - sorted - std::sort")
    ->Args({ TestType::sorted, SortFunc::std_sort });

BENCHMARK(bm_bucket_sort)
    ->Name("sorting std::vector<double> of size 10000 where 0<=vec[i]<1 - reverse sorted - alg::bucket_sort")
    ->Args({ TestType::reverse_sorted, SortFunc::bucket_sort });

BENCHMARK(bm_bucket_sort)
    ->Name("sorting std::vector<double> of size 10000 where 0<=vec[i]<1 - reverse sorted - std::stable_sort")
    ->Args({ TestType::reverse_sorted, SortFunc::std_stable_sort });

BENCHMARK(bm_bucket_sort)
    ->Name("sorting std::vector<double> of size 10000 where 0<=vec[i]<1 - reverse sorted - std::sort")
    ->Args({ TestType::reverse_sorted, SortFunc::std_sort });

BENCHMARK_MAIN();
