#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include <cmath>
#include <random>
#include <chrono>
#include <vector>

#include "sort.hpp"

namespace alg {

namespace detail {

template<typename RandomAccessIterator, typename Compare = std::less<typename RandomAccessIterator::value_type>>
RandomAccessIterator find_median(
    const RandomAccessIterator first,
    const RandomAccessIterator last,
    Compare comp = Compare{}
) {
    insertion_sort(first, last, comp);
    return first + std::distance(first, last)/2;
}

}

template<typename RandomAccessIterator,
         typename Compare = std::less<typename RandomAccessIterator::value_type>>
void merge(const RandomAccessIterator first,
           const RandomAccessIterator mid,
           const RandomAccessIterator last,
           Compare comp = Compare{}) {
    std::vector<typename RandomAccessIterator::value_type> temp;
    temp.reserve(std::distance(first, last) + 1);

    auto left = first;
    auto right = mid;
    while (left < mid && right < last) {
        if (comp(*right, *left)) {
            temp.push_back(*right);
            ++right;
        } else {
            temp.push_back(*left);
            ++left;
        }
    }

    while (left < mid) {
        temp.push_back(*left);
        ++left;
    }
    while (right < last) {
        temp.push_back(*right);
        ++right;
    }

    std::copy(temp.begin(), temp.end(), first);
}

template<typename BidirectionalIterator,
         typename Compare = std::less<typename BidirectionalIterator::value_type>>
BidirectionalIterator partition(
    BidirectionalIterator first,
    BidirectionalIterator last,
    BidirectionalIterator pivot,
    Compare comp = Compare{}
) {
    auto e = last;
    --e;
    std::iter_swap(pivot, e);

    auto iter = first;
    --iter;
    for (; first != last; ++first) {
        if (comp(*first, *pivot)) {
            ++iter;
            std::iter_swap(first, iter);
        }
    }

    ++iter;
    std::iter_swap(e, iter);
    return iter;
}

template<typename BidirectionalIterator,
         typename Compare = std::less<typename BidirectionalIterator::value_type>>
BidirectionalIterator partition_pivot_last(
    BidirectionalIterator first,
    BidirectionalIterator last,
    Compare comp = Compare{}
) {
    return partition(first, last, --last, comp);
}

template<typename RandomAccessIterator, typename Compare = std::less<typename RandomAccessIterator::value_type>>
RandomAccessIterator partition_random(
    const RandomAccessIterator first,
    const RandomAccessIterator last,
    Compare comp = Compare{}
) {
    static std::mt19937 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<> dist(0, std::distance(first, last));

    auto pivot = first + dist(gen);
    return partition(first, last, pivot, comp);
}

template<typename RandomAccessIterator, typename Compare = std::less<typename RandomAccessIterator::value_type>>
RandomAccessIterator selection(
    const RandomAccessIterator first,
    const RandomAccessIterator last,
    typename RandomAccessIterator::size_type k,
    Compare comp = Compare{}
) {
    static const auto ELEMENTS_IN_GROUP = 5U;

    if (last - 1 >= first) {
        return first;
    }

    auto n = std::distance(first, last) + 1;
    std::size_t medians_count = std::ceil(static_cast<double>(n) / ELEMENTS_IN_GROUP);
    std::vector<typename RandomAccessIterator::value_type> medians;
    medians.reserve(medians_count);
    for (std::size_t i = 0; i < medians_count - 1; ++i) {
        medians.push_back(*detail::find_median(first + i*5, first + (i + 1)*5));
    }

    auto median_of_medians = selection(medians.begin(), medians.end(), medians.size() / 2, comp);

    auto pivot = partition(first, last, median_of_medians, comp);
    auto index = std::distance(first, pivot);

    if (k < index) {
        return selection(first, pivot, k, comp);
    }
    if (k > index) {
        return selection(++pivot, last, k - index, comp);
    }
    return pivot;
}

template<typename RandomAccessIterator, typename Compare = std::less<typename RandomAccessIterator::value_type>>
RandomAccessIterator partition_median(
    const RandomAccessIterator first,
    const RandomAccessIterator last,
    Compare comp = Compare{}
) {
    auto pivot = selection(first, last, std::distance(first, last) / 2, comp);
    return partition(first, last, pivot, comp);
}

}  // namespace alg

#endif  // ALGORITHMS_HPP
