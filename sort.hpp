#ifndef SORT_HPP
#define SORT_HPP

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iterator>
#include <random>
#include <vector>

constexpr uint8_t operator"" _u8(uint64_t val) {
    return static_cast<uint8_t>(val);
}

namespace alg {

template<typename BidirectionalIterator,
         typename Compare = std::less<typename BidirectionalIterator::value_type>>
void bubble_sort(
    BidirectionalIterator first,
    BidirectionalIterator last,
    Compare comp = Compare{}
) {
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
void insertion_sort(
    const BidirectionalIterator first,
    const BidirectionalIterator last,
    Compare comp = Compare{}
) {
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
void merge(
    const RandomAccessIterator first,
    const RandomAccessIterator mid,
    const RandomAccessIterator last,
    Compare comp = Compare{}
) {
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

template<typename RandomAccessIterator,
         typename Compare = std::less<typename RandomAccessIterator::value_type>>
void merge_sort(
    const RandomAccessIterator first,
    const RandomAccessIterator last,
    Compare comp
) {
    if (last - 1 <= first) {
        return;
    }
    auto mid = first + std::distance(first, last)/2;
    merge_sort(first, mid, comp);
    merge_sort(mid, last, comp);
    merge(first, mid, last, comp);
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

template<typename RandomAccessIterator,
         typename Compare = std::less<typename RandomAccessIterator::value_type>>
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

namespace detail {

template<typename RandomAccessIterator,
         typename Compare = std::less<typename RandomAccessIterator::value_type>>
RandomAccessIterator find_median(
    const RandomAccessIterator first,
    const RandomAccessIterator last,
    Compare comp = Compare{}
) {
    insertion_sort(first, last, comp);
    return first + std::distance(first, last)/2;
}

}  // namespace detail

template<typename RandomAccessIterator,
         typename Compare = std::less<typename RandomAccessIterator::value_type>>
RandomAccessIterator selection(
    const RandomAccessIterator first,
    const RandomAccessIterator last,
    std::size_t k,
    Compare comp = Compare{}
) {
    static const auto ELEMENTS_IN_GROUP = 5_u8;

    if (last - 1 >= first) {
        return first;
    }

    std::size_t medians_count = (std::distance(first, last) + 1) / ELEMENTS_IN_GROUP;
    std::vector<typename RandomAccessIterator::value_type> medians;
    medians.reserve(medians_count);
    for (std::size_t i = 0; i < medians_count; ++i) {
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

template<typename RandomAccessIterator,
         typename Compare = std::less<typename RandomAccessIterator::value_type>>
RandomAccessIterator partition_median(
    const RandomAccessIterator first,
    const RandomAccessIterator last,
    Compare comp = Compare{}
) {
    auto pivot = selection(first, last, std::distance(first, last) / 2, comp);
    return partition(first, last, pivot, comp);
}

namespace detail {

template<typename RandomAccessIterator,
         typename Compare = std::less<typename RandomAccessIterator::value_type>>
void quick_sort_impl(
    const RandomAccessIterator first,
    const RandomAccessIterator last,
    const std::random_access_iterator_tag iter_tag,
    Compare comp = Compare{}
) {
    if (last - 1 <= first) {
        return;
    }
    auto pivot = partition_median(first, last, comp);
    quick_sort_impl(first, pivot, iter_tag, comp);
    quick_sort_impl(++pivot, last, iter_tag, comp);
}

template<typename BidirectionalIterator,
         typename Compare = std::less<typename BidirectionalIterator::value_type>>
void quick_sort_impl(
    BidirectionalIterator first,
    BidirectionalIterator last,
    const std::bidirectional_iterator_tag iter_tag,
    Compare comp = std::less<typename BidirectionalIterator::value_type>()
) {
    if (first == last || first == --last) {
        return;
    }
    ++last;
    auto pivot = partition_pivot_last(first, last, comp);
    quick_sort_impl(first, pivot, iter_tag, comp);
    quick_sort_impl(++pivot, last, iter_tag, comp);
}

}  // namespace detail

template<typename BidirectionalIterator,
         typename Compare = std::less<typename BidirectionalIterator::value_type>>
void quick_sort(
    const BidirectionalIterator first,
    const BidirectionalIterator last,
    Compare comp = Compare{}
) {
    using iter_category = typename std::iterator_traits<BidirectionalIterator>::iterator_category;
    return detail::quick_sort_impl(first, last, iter_category{}, comp);
}

template<typename RandomAccessIterator,
         typename Compare = std::less<typename RandomAccessIterator::value_type>>
void counting_sort(
    RandomAccessIterator first,
    RandomAccessIterator last,
    Compare comp = Compare{}
) {

}

template<typename RandomAccessIterator,
         typename Compare = std::less<typename RandomAccessIterator::value_type>>
void radix_sort(
    RandomAccessIterator first,
    RandomAccessIterator last,
    Compare comp = Compare{}
) {

}

template<typename RandomAccessIterator,
         typename Compare = std::less<typename RandomAccessIterator::value_type>>
void bucket_sort(
    RandomAccessIterator first,
    RandomAccessIterator last,
    Compare comp = Compare{}
) {

}

}  // namespace alg

#endif  // SORT_HPP
