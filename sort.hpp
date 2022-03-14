#ifndef SORT_HPP
#define SORT_HPP

#include <algorithm>

#include "algorithms.hpp"

namespace alg {

template<typename BidirectionalIterator,
         typename Compare = std::less<typename BidirectionalIterator::value_type>>
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

template<typename BidirectionalIterator,
         typename Compare = std::less<typename BidirectionalIterator::value_type>>
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
             movePos != first && comp(key, *(--movePos));
             --insertPos) {
            *insertPos = std::move(*movePos);
        }
        *insertPos = std::move(key);
    }
}

template<typename RandomAccessIterator,
         typename Compare = std::less<typename RandomAccessIterator::value_type>>
void merge_sort(const RandomAccessIterator first,
                const RandomAccessIterator last,
                Compare comp = Compare{}) {
    if (last - 1 <= first) {
        return;
    }
    auto mid = first + std::distance(first, last)/2;
    merge_sort(first, mid, comp);
    merge_sort(mid, last, comp);
    merge(first, mid, last, comp);
}

// template<typename RandomAccessIterator,
//          typename Compare = std::less<typename RandomAccessIterator::value_type>>
// void quick_sort(const RandomAccessIterator first,
//                 const RandomAccessIterator last,
//                 Compare comp = Compare{}) {
//     if (last - 1 <= first) {
//         return;
//     }
//     auto pivot = partition_median(first, last, comp);
//     quick_sort(first, pivot, comp);
//     quick_sort(++pivot, last, comp);
// }

// template<typename BidirectionalIterator,
//          typename Compare = std::less<typename BidirectionalIterator::value_type>>
// void quick_sort(const BidirectionalIterator first,
//                 const BidirectionalIterator last,
//                 Compare comp = std::less<typename BidirectionalIterator::value_type>()) {
//     if (--last <= first) {
//         return;
//     }
//     ++last;
//     auto pivot = partition_pivot_last(first, last, comp);
//     quick_sort(first, pivot, comp);
//     quick_sort(++pivot, last, comp);
// }

template<typename RandomAccessIterator,
         typename Compare = std::less<typename RandomAccessIterator::value_type>>
void counting_sort(RandomAccessIterator first,
                   RandomAccessIterator last,
                   Compare comp = Compare{}) {

}

template<typename RandomAccessIterator,
         typename Compare = std::less<typename RandomAccessIterator::value_type>>
void radix_sort(RandomAccessIterator first,
                RandomAccessIterator last,
                Compare comp = Compare{}) {

}

template<typename RandomAccessIterator,
         typename Compare = std::less<typename RandomAccessIterator::value_type>>
void bucket_sort(RandomAccessIterator first,
                 RandomAccessIterator last,
                 Compare comp = Compare{}) {

}

}  // namespace alg

#endif  // SORT_HPP
