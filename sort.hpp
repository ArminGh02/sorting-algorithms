#ifndef SORT_HPP
#define SORT_HPP

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iterator>
#include <list>
#include <random>
#include <vector>

namespace alg {

template<class BidirectionalIterator,
         class Compare = std::less<typename BidirectionalIterator::value_type>>
inline void bubble_sort(
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

template<class BidirectionalIterator,
         class Compare = std::less<typename BidirectionalIterator::value_type>>
inline void insertion_sort(
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

template<class RandomAccessIterator,
         class Compare = std::less<typename RandomAccessIterator::value_type>>
inline void merge(
    const RandomAccessIterator first,
    const RandomAccessIterator mid,
    const RandomAccessIterator last,
    Compare comp = Compare{}
) {
    std::vector<typename RandomAccessIterator::value_type> temp;
    temp.reserve(std::distance(first, last));

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

template<class RandomAccessIterator,
         class Compare = std::less<typename RandomAccessIterator::value_type>>
inline void merge_sort(
    const RandomAccessIterator first,
    const RandomAccessIterator last,
    Compare comp = Compare{}
) {
    if (last - 1 <= first) {
        return;
    }
    auto mid = first + std::distance(first, last)/2;
    merge_sort(first, mid, comp);
    merge_sort(mid, last, comp);
    merge(first, mid, last, comp);
}

template<class BidirectionalIterator,
         class Compare = std::less<typename BidirectionalIterator::value_type>>
inline BidirectionalIterator partition(
    BidirectionalIterator first,
    BidirectionalIterator last,
    BidirectionalIterator pivot,
    Compare comp = Compare{}
) {
    --last;
    std::iter_swap(pivot, last);

    auto iter = first;
    for (; first != last; ++first) {
        if (comp(*first, *last)) {
            std::iter_swap(first, iter);
            ++iter;
        }
    }

    std::iter_swap(last, iter);
    return iter;
}

template<class BidirectionalIterator,
         class Compare = std::less<typename BidirectionalIterator::value_type>>
inline BidirectionalIterator partition_pivot_last(
    BidirectionalIterator first,
    BidirectionalIterator last,
    Compare comp = Compare{}
) {
    return partition(first, last, --last, comp);
}

template<class RandomAccessIterator,
         class Compare = std::less<typename RandomAccessIterator::value_type>>
inline RandomAccessIterator partition_random(
    const RandomAccessIterator first,
    const RandomAccessIterator last,
    Compare comp = Compare{}
) {
    static std::mt19937 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<> dist(0, std::distance(first, last) - 1);

    auto pivot = first + dist(gen);
    return partition(first, last, pivot, comp);
}

namespace detail {

template<class RandomAccessIterator,
         class Compare = std::less<typename RandomAccessIterator::value_type>>
inline RandomAccessIterator find_median(
    const RandomAccessIterator first,
    const RandomAccessIterator last,
    Compare comp = Compare{}
) {
    insertion_sort(first, last, comp);
    return first + std::distance(first, last)/2;
}

}  // namespace detail

inline namespace literals {

constexpr uint8_t operator"" _u8(uint64_t val) {
    return static_cast<uint8_t>(val);
}

}  // namespace literals

template<class RandomAccessIterator,
         class Compare = std::less<typename RandomAccessIterator::value_type>>
inline RandomAccessIterator selection(
    const RandomAccessIterator first,
    const RandomAccessIterator last,
    std::size_t k,
    Compare comp = Compare{}
) {
    static const auto ELEMENTS_IN_GROUP = 5_u8;

    if (last - 1 <= first) {
        return first;
    }

    std::size_t medians_count = std::distance(first, last) / ELEMENTS_IN_GROUP;
    std::vector<typename RandomAccessIterator::value_type> medians;
    medians.reserve(medians_count);
    for (std::size_t i = 0; i < medians_count; ++i) {
        medians.push_back(*detail::find_median(first + i*5, first + (i + 1)*5));
    }

    auto median_of_medians = selection(medians.begin(), medians.end(), medians.size() / 2, comp);

    auto pivot = partition(first, last, median_of_medians, comp);
    std::size_t index = std::distance(first, pivot);

    if (k < index) {
        return selection(first, pivot, k, comp);
    }
    if (k > index) {
        return selection(++pivot, last, k - index, comp);
    }
    return pivot;
}

template<class RandomAccessIterator,
         class Compare = std::less<typename RandomAccessIterator::value_type>>
inline RandomAccessIterator partition_median(
    const RandomAccessIterator first,
    const RandomAccessIterator last,
    Compare comp = Compare{}
) {
    auto pivot = selection(first, last, std::distance(first, last) / 2, comp);
    return partition(first, last, pivot, comp);
}

namespace detail {

template<class RandomAccessIterator,
         class Compare = std::less<typename RandomAccessIterator::value_type>>
inline void quick_sort_impl(
    const RandomAccessIterator first,
    const RandomAccessIterator last,
    const std::random_access_iterator_tag iter_tag,
    Compare comp = Compare{}
) {
    if (last - 1 <= first) {
        return;
    }
    auto pivot = partition_pivot_last(first, last, comp);
    quick_sort_impl(first, pivot, iter_tag, comp);
    quick_sort_impl(++pivot, last, iter_tag, comp);
}

template<class BidirectionalIterator,
         class Compare = std::less<typename BidirectionalIterator::value_type>>
inline void quick_sort_impl(
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

template<class BidirectionalIterator,
         class Compare = std::less<typename BidirectionalIterator::value_type>>
inline void quick_sort(
    const BidirectionalIterator first,
    const BidirectionalIterator last,
    Compare comp = Compare{}
) {
    using iter_category = typename std::iterator_traits<BidirectionalIterator>::iterator_category;
    return detail::quick_sort_impl(first, last, iter_category{}, comp);
}

template<class BidirectionalIterator,
         class Int = typename BidirectionalIterator::value_type,
         class = typename std::enable_if<std::is_integral<Int>::value>::type>
inline void counting_sort(
    BidirectionalIterator first,
    BidirectionalIterator last,
    Int max,
    std::size_t n
) {
    std::vector<Int> counter(max + 1);
    for (auto it = first; it != last; ++it) {
        ++counter[*it];
    }

    for (std::size_t i = 1; i < counter.size(); ++i) {
        counter[i] += counter[i - 1];
    }

    std::vector<Int> temp(n);
    --last;
    --first;
    for (auto it = last; it != first; --it) {
        temp[counter[*it] - 1] = *it;
        --counter[*it];
    }

    ++first;
    std::copy(temp.begin(), temp.end(), first);
}

template<class RandomAccessIterator,
         class Int = typename RandomAccessIterator::value_type,
         class = typename std::enable_if<std::is_integral<Int>::value>::type>
inline void counting_sort(
    RandomAccessIterator first,
    RandomAccessIterator last,
    std::size_t max
) {
    counting_sort(first, last, max, last - first);
}

namespace detail {

template<class RandomAccessIterator,
         class Int = typename RandomAccessIterator::value_type,
         class = typename std::enable_if<std::is_integral<Int>::value>::type>
inline void counting_sort_digit(
    RandomAccessIterator first,
    RandomAccessIterator last,
    Int exp,
    std::size_t n
) {
    std::vector<Int> counter(10);
    for (auto it = first; it != last; ++it) {
        ++counter[(*it / exp) % 10];
    }

    for (std::size_t i = 1; i < counter.size(); ++i) {
        counter[i] += counter[i - 1];
    }

    std::vector<Int> temp(n);
    --last;
    --first;
    for (auto it = last; it != first; --it) {
        temp[counter[(*it / exp) % 10] - 1] = *it;
        --counter[(*it / exp) % 10];
    }

    ++first;
    std::copy(temp.begin(), temp.end(), first);
}

}

template<class RandomAccessIterator,
         class Int = typename RandomAccessIterator::value_type,
         class = typename std::enable_if<std::is_integral<Int>::value>::type>
inline void radix_sort(
    const RandomAccessIterator first,
    const RandomAccessIterator last,
    Int max,
    std::size_t n
) {
    for (Int exp = 1; max / exp > 0; exp *= 10) {
        detail::counting_sort_digit(first, last, exp, n);
    }
}

template<class ForwardIterator,
         class Float = typename ForwardIterator::value_type,
         class = typename std::enable_if<std::is_floating_point<Float>::value>::type>
inline void bucket_sort(ForwardIterator first, ForwardIterator last, std::size_t n) {
    std::vector<std::list<Float>> buckets(n);
    for (auto it = first; it != last; ++it) {
        buckets[std::floor(*it * n)].push_back(*it);
    }

    for (auto& bucket : buckets) {
        insertion_sort(bucket.begin(), bucket.end());
    }

    for (const auto& bucket : buckets) {
        for (auto element : bucket) {
            *first = element;
            ++first;
        }
    }
}

template<class RandomAccessIterator,
         class Float = typename RandomAccessIterator::value_type,
         class = typename std::enable_if<std::is_floating_point<Float>::value>::type>
inline void bucket_sort(RandomAccessIterator first, RandomAccessIterator last) {
    return bucket_sort(first, last, last - first);
}

}  // namespace alg

#endif  // SORT_HPP
