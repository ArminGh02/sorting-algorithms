#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include <random>
#include <chrono>
#include <vector>

namespace alg {

template<typename RandomAccessIterator, typename Compare>
void merge(const RandomAccessIterator first,
           const RandomAccessIterator mid,
           const RandomAccessIterator last,
           Compare comp = std::less<typename RandomAccessIterator::value_type>()) {
    std::vector<typename RandomAccessIterator::value_type> temp;
    temp.reserve(std::distance(first, last) + 1);

    auto left = first;
    auto right = mid;
    while (left < mid && right <= last) {
        if (comp(*right, *left)) {
            temp.push_back(*right);
            ++right;
        } else {
            temp.push_back(*left);
            ++left;
        }
    }

    while (left <= mid - 1) {
        temp.push_back(*left);
        ++left;
    }
    while (right < last) {
        temp.push_back(*right);
        ++right;
    }

    std::copy(temp.begin(), temp.end(), first);
}

template<typename BidirectionalIterator, typename Compare>
BidirectionalIterator partition(
    BidirectionalIterator first,
    BidirectionalIterator last,
    BidirectionalIterator pivot,
    Compare comp = std::less<typename BidirectionalIterator::value_type>()
) {
    auto e = last;
    --e;
    std::iter_swap(pivot, e);

    auto iter = first;
    --iter;
    for (; first != last; ++first) {
        if (comp(*first, *pivot)) {
            ++iter;
            std::iter_swap(*first, *iter);
        }
    }

    ++iter;
    std::iter_swap(e, iter);
    return iter;
}

template<typename BidirectionalIterator, typename Compare>
BidirectionalIterator partition_pivot_last(
    const BidirectionalIterator first,
    const BidirectionalIterator last,
    Compare comp = std::less<typename BidirectionalIterator::value_type>()
) {
    return partition(first, last, --last, comp);
}

template<typename RandomAccessIterator, typename Compare>
RandomAccessIterator partition_random(
    const RandomAccessIterator first,
    const RandomAccessIterator last,
    Compare comp = std::less<typename RandomAccessIterator::value_type>()
) {
    static std::mt19937 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<> dist(0, std::distance(first, last));
    auto pivot = first + dist(gen);
    return partition(first, last, pivot, comp);
}

template<typename RandomAccessIterator, typename Compare>
RandomAccessIterator selection(
    const RandomAccessIterator first,
    const RandomAccessIterator last,
    typename RandomAccessIterator::difference_type k,
    Compare comp = std::less<typename RandomAccessIterator::value_type>()
) {

}

template<typename RandomAccessIterator, typename Compare>
RandomAccessIterator partition_median(
    const RandomAccessIterator first,
    const RandomAccessIterator last,
    Compare comp = std::less<typename RandomAccessIterator::value_type>()
) {
    auto pivot = selection(first, last, std::distance(first, last) / 2, comp);
    return partition(first, last, pivot, comp)
}

}  // namespace alg

#endif  // ALGORITHMS_HPP
