#include <algorithm>
#include <chrono>
#include <random>
#include <vector>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "sort.hpp"

static std::mt19937 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());

TEST_CASE("sorting algorithms functions for ints") {
    constexpr std::size_t ELEMENTS_COUNT = 500000;

    std::vector<int> to_sort;
    to_sort.reserve(ELEMENTS_COUNT);

    std::uniform_int_distribution<> dist;
    for (std::size_t i = 0; i < ELEMENTS_COUNT; ++i) {
        to_sort.push_back(dist(gen));
    }

    REQUIRE(to_sort.size() == ELEMENTS_COUNT);

    SECTION("bubble_sort") {
        SECTION("default compare") {
            alg::bubble_sort(to_sort.begin(), to_sort.end());
            REQUIRE(std::is_sorted(to_sort.begin(), to_sort.end()));
        }
        SECTION("descending") {
            alg::bubble_sort(to_sort.begin(), to_sort.end(), std::greater<int>());
            REQUIRE(std::is_sorted(to_sort.begin(), to_sort.end(), std::greater<int>()));
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
    constexpr std::size_t ELEMENTS_COUNT = 500000;

    std::vector<unsigned> to_sort;
    to_sort.reserve(ELEMENTS_COUNT);

    constexpr auto MAX_ELEMENT = 100000U;
    std::uniform_int_distribution<unsigned> dist(0U, MAX_ELEMENT);
    for (std::size_t i = 0; i < ELEMENTS_COUNT; ++i) {
        to_sort.push_back(dist(gen));
    }

    REQUIRE(to_sort.size() == ELEMENTS_COUNT);
    REQUIRE(*std::max_element(to_sort.begin(), to_sort.end()) <= MAX_ELEMENT);

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
    constexpr std::size_t ELEMENTS_COUNT = 100000;

    std::vector<double> to_sort;
    to_sort.reserve(ELEMENTS_COUNT);

    std::uniform_real_distribution<> dist;
    for (std::size_t i = 0; i < ELEMENTS_COUNT; ++i) {
        to_sort.push_back(dist(gen));
    }

    REQUIRE(to_sort.size() == ELEMENTS_COUNT);
    REQUIRE(*std::min_element(to_sort.begin(), to_sort.end()) >= 0.0);
    REQUIRE(*std::max_element(to_sort.begin(), to_sort.end()) < 1.0);

    alg::bucket_sort(to_sort.begin(), to_sort.end());
    REQUIRE(std::is_sorted(to_sort.begin(), to_sort.end()));
}
