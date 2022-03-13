#ifndef SORT_HPP
#define SORT_HPP

#include <algorithm>

#include "algorithms.hpp"

namespace alg {

template<typename BidirectionalIterator, typename Compare>
void bubble_sort(BidirectionalIterator first,
                 BidirectionalIterator last,
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

template<typename RandomAccessIterator, typename Compare>
void insertion_sort(RandomAccessIterator first,
                    RandomAccessIterator last,
                    Compare comp = std::less<typename RandomAccessIterator::value_type>()) {
    if (first == last) {
        return;
    }
    for (auto iter1 = first + 1; iter1 != last; ++iter1) {
        const auto& key = *iter1;
        auto iter2 = iter1 - 1;
        for (; iter2 >= first && comp(key, *iter2); --iter2) {
            *(iter2 + 1) = *iter2;
        }
        *(iter2 + 1) = key;
    }
}

template<typename RandomAccessIterator, typename Compare>
void merge_sort(RandomAccessIterator first,
                RandomAccessIterator last,
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
void quick_sort(RandomAccessIterator first,
                RandomAccessIterator last,
                Compare comp = std::less<typename RandomAccessIterator::value_type>()) {
    if (last <= first) {
        return;
    }
    auto pivot = partition(first, last, comp);
    quick_sort(first, pivot - 1, comp);
    quick_sort(pivot + 1, last, comp);
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
