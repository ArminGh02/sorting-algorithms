#ifndef SORT_HPP
#define SORT_HPP

#include <algorithm>

#include "algorithms.hpp"

namespace alg {

template<typename BidirectionalIterator, typename Compare>
void bubble_sort(const BidirectionalIterator first,
                 const BidirectionalIterator last,
                 Compare comp = std::less<typename BidirectionalIterator::value_type>()) {
    if (first == last) {
        return;
    }
    auto e = last;
    for (--e; first != e; ++first) {
        auto iter = first;
        for (++iter; iter != last; ++iter) {
            if (comp(*iter, *first)) {
                std::iter_swap(iter, first);
            }
        }
    }
}

template<typename BidirectionalIterator, typename Compare>
void insertion_sort(const BidirectionalIterator first,
                    const BidirectionalIterator last,
                    Compare comp = std::less<typename BidirectionalIterator::value_type>()) {
    if (first == last) {
        return;
    }
    auto iter = first;
    for (++iter; iter != last; ++iter) {
        const auto key(std::move(*iter));
        auto insertPos = iter;
        for (auto movePos = iter;
             movePos != first && compare(key, *(--movePos));
             --insertPos) {
            *insertPos = std::move(*movePos);
        }
        *insertPos = std::move(key);
    }
}

template<typename RandomAccessIterator, typename Compare>
void merge_sort(const RandomAccessIterator first,
                const RandomAccessIterator last,
                Compare comp = std::less<typename RandomAccessIterator::value_type>()) {
    if (last <= first) {
        return;
    }
    auto mid = first + std::distance(first, last)/2;
    merge_sort(first, mid, comp);
    merge_sort(mid + 1, last, comp);
    merge(first, mid + 1, last, comp);
}

template<typename RandomAccessIterator, typename Compare>
void quick_sort(const RandomAccessIterator first,
                const RandomAccessIterator last,
                Compare comp = std::less<typename RandomAccessIterator::value_type>()) {
    if (last <= first) {
        return;
    }
    auto pivot = partition_median(first, last, comp);
    quick_sort(first, pivot, comp);
    quick_sort(++pivot, last, comp);
}

template<typename BidirectionalIterator, typename Compare>
void quick_sort(const BidirectionalIterator first,
                const BidirectionalIterator last,
                Compare comp = std::less<typename BidirectionalIterator::value_type>()) {
    if (last <= first) {
        return;
    }
    auto pivot = partition_pivot_last(first, last, comp);
    quick_sort(first, pivot, comp);
    quick_sort(++pivot, last, comp);
}

template<typename RandomAccessIterator, typename Compare>
void counting_sort(RandomAccessIterator first,
                   RandomAccessIterator last,
                   Compare comp = std::less<typename RandomAccessIterator::value_type>()) {

}

template<typename RandomAccessIterator, typename Compare>
void radix_sort(RandomAccessIterator first,
                RandomAccessIterator last,
                Compare comp = std::less<typename RandomAccessIterator::value_type>()) {

}

template<typename RandomAccessIterator, typename Compare>
void bucket_sort(RandomAccessIterator first,
                 RandomAccessIterator last,
                 Compare comp = std::less<typename RandomAccessIterator::value_type>()) {

}

}  // namespace alg

#endif  // SORT_HPP
