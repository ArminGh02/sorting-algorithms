#include <algorithm>
#include <chrono>
#include <list>
#include <random>
#include <vector>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "sort.hpp"

static std::mt19937 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());

TEST_CASE("general sorting functions") {
    constexpr std::size_t ELEMENTS_COUNT = 500;

    std::vector<int> to_sort;
    to_sort.reserve(ELEMENTS_COUNT);

    std::uniform_int_distribution<> dist;
    for (std::size_t i = 0; i < ELEMENTS_COUNT; ++i) {
        to_sort.push_back(dist(gen));
    }

    SECTION("bubble_sort") {
        SECTION("default compare") {
            alg::bubble_sort(to_sort.begin(), to_sort.end());
            REQUIRE(std::is_sorted(to_sort.begin(), to_sort.end()));
        }
        SECTION("descending") {
            alg::bubble_sort(to_sort.begin(), to_sort.end(), std::greater<int>());
            REQUIRE(std::is_sorted(to_sort.begin(), to_sort.end(), std::greater<int>()));
        }
        SECTION("forward iterator") {
            std::forward_list<int> forward_list(to_sort.begin(), to_sort.end());
            alg::bubble_sort(forward_list.begin(), forward_list.end());
            REQUIRE(std::is_sorted(forward_list.begin(), forward_list.end()));
        }
    }
    SECTION("insertion_sort") {
        SECTION("default compare") {
            alg::insertion_sort(to_sort.begin(), to_sort.end());
            REQUIRE(std::is_sorted(to_sort.begin(), to_sort.end()));
        }
        SECTION("descending") {
            alg::insertion_sort(to_sort.begin(), to_sort.end(), std::greater<int>());
            REQUIRE(std::is_sorted(to_sort.begin(), to_sort.end(), std::greater<int>()));
        }
    }
    SECTION("selection_sort") {
        SECTION("default compare") {
            alg::selection_sort(to_sort.begin(), to_sort.end());
            REQUIRE(std::is_sorted(to_sort.begin(), to_sort.end()));
        }
        SECTION("descending") {
            alg::selection_sort(to_sort.begin(), to_sort.end(), std::greater<int>());
            REQUIRE(std::is_sorted(to_sort.begin(), to_sort.end(), std::greater<int>()));
        }
    }
    SECTION("merge_sort") {
        SECTION("default compare") {
            alg::merge_sort(to_sort.begin(), to_sort.end());
            REQUIRE(std::is_sorted(to_sort.begin(), to_sort.end()));
        }
        SECTION("descending") {
            alg::merge_sort(to_sort.begin(), to_sort.end(), std::greater<int>());
            REQUIRE(std::is_sorted(to_sort.begin(), to_sort.end(), std::greater<int>()));
        }
    }
    SECTION("quick_sort") {
        SECTION("default compare") {
            alg::quick_sort(to_sort.begin(), to_sort.end());
            REQUIRE(std::is_sorted(to_sort.begin(), to_sort.end()));
        }
        SECTION("descending") {
            alg::quick_sort(to_sort.begin(), to_sort.end(), std::greater<int>());
            REQUIRE(std::is_sorted(to_sort.begin(), to_sort.end(), std::greater<int>()));
        }
        SECTION("bidirectional iterator") {
            std::list<int> list(to_sort.begin(), to_sort.end());
            alg::quick_sort(list.begin(), list.end());
            REQUIRE(std::is_sorted(list.begin(), list.end()));
        }
    }
    SECTION("heap_sort") {
        SECTION("default compare") {
            alg::heap_sort(to_sort.begin(), to_sort.end());
            REQUIRE(std::is_sorted(to_sort.begin(), to_sort.end()));
        }
        SECTION("descending") {
            alg::heap_sort(to_sort.begin(), to_sort.end(), std::greater<int>());
            REQUIRE(std::is_sorted(to_sort.begin(), to_sort.end(), std::greater<int>()));
        }
    }
}

TEST_CASE("radix_sort & counting_sort") {
    constexpr std::size_t ELEMENTS_COUNT = 500;

    std::vector<unsigned> to_sort;
    to_sort.reserve(ELEMENTS_COUNT);

    constexpr auto MAX_ELEMENT = 100U;
    std::uniform_int_distribution<unsigned> dist(0U, MAX_ELEMENT);
    for (std::size_t i = 0; i < ELEMENTS_COUNT; ++i) {
        to_sort.push_back(dist(gen));
    }

    SECTION("counting_sort") {
        alg::counting_sort(to_sort.begin(), to_sort.end(), MAX_ELEMENT);
        REQUIRE(std::is_sorted(to_sort.begin(), to_sort.end()));
    }
    SECTION("radix_sort") {
        alg::radix_sort(to_sort.begin(), to_sort.end(), MAX_ELEMENT);
        REQUIRE(std::is_sorted(to_sort.begin(), to_sort.end()));
    }
}

TEST_CASE("bucket_sort") {
    constexpr std::size_t ELEMENTS_COUNT = 100;

    std::vector<double> to_sort;
    to_sort.reserve(ELEMENTS_COUNT);

    std::uniform_real_distribution<> dist;
    for (std::size_t i = 0; i < ELEMENTS_COUNT; ++i) {
        to_sort.push_back(dist(gen));
    }

    SECTION("random access iterator") {
        alg::bucket_sort(to_sort.begin(), to_sort.end());
        REQUIRE(std::is_sorted(to_sort.begin(), to_sort.end()));
    }
    SECTION("bidirectional iterator") {
        std::list<double> list(to_sort.begin(), to_sort.end());
        alg::bucket_sort(list.begin(), list.end(), list.size());
        REQUIRE(std::is_sorted(list.begin(), list.end()));
    }
    SECTION("forward iterator") {
        std::forward_list<double> forward_list(to_sort.begin(), to_sort.end());
        alg::bucket_sort(forward_list.begin(), forward_list.end(), to_sort.size());
        REQUIRE(std::is_sorted(forward_list.begin(), forward_list.end()));
    }
}

TEST_CASE("quick_select") {
    constexpr std::size_t ELEMENTS_COUNT = 100;

    std::vector<int> sample_array;
    sample_array.reserve(ELEMENTS_COUNT);

    std::uniform_int_distribution<> dist;
    for (std::size_t i = 0; i < ELEMENTS_COUNT; ++i) {
        sample_array.push_back(dist(gen));
    }

    auto sorted = sample_array;
    std::sort(sorted.begin(), sorted.end());

    for (std::size_t i = 0; i < sample_array.size(); ++i) {
        auto current_pos = sample_array.begin() + i;
        alg::quick_select(sample_array.begin(), current_pos, sample_array.end());

        REQUIRE(std::is_partitioned(sample_array.begin(), sample_array.end(), [current_pos](int a) {
            return a < *current_pos;
        }));

        REQUIRE(sample_array[i] == sorted[i]);
    }
}
