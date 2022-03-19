#ifndef SORT_HPP
#define SORT_HPP

#include <algorithm>
#include <chrono>
#include <cmath>
#include <forward_list>
#include <iterator>
#include <random>
#include <vector>

namespace alg {

template<class BidirectionalIterator,
         class Compare = std::less<typename BidirectionalIterator::value_type>>
inline void bubble_sort(
    BidirectionalIterator first,
    BidirectionalIterator last,
    Compare comp = Compare{}
) noexcept {
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
) noexcept {
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

template<class BidirectionalIterator,
         class Compare = std::less<typename BidirectionalIterator::value_type>>
inline void selection_sort(
    BidirectionalIterator first,
    BidirectionalIterator last,
    Compare comp = Compare{}
) noexcept {
    if (first == last) {
        return;
    }
    auto e = last;
    for (--e; first != e; ++first) {
        auto minPos = first;
        auto it = first;
        for (++it; it != last; ++it) {
            if (comp(*it, *minPos)) {
                minPos = it;
            }
        }
        std::iter_swap(first, minPos);
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
            temp.push_back(std::move(*right));
            ++right;
        } else {
            temp.push_back(std::move(*left));
            ++left;
        }
    }

    while (left < mid) {
        temp.push_back(std::move(*left));
        ++left;
    }
    while (right < last) {
        temp.push_back(std::move(*right));
        ++right;
    }

    std::move(temp.begin(), temp.end(), first);
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
    BidirectionalIterator pivot,
    BidirectionalIterator last,
    Compare comp = Compare{}
) noexcept {
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
) noexcept {
    auto it = last;
    return partition(first, --it, last, comp);
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
    return partition(first, pivot, last, comp);
}

namespace detail {

template<class RandomAccessIterator,
         class Compare = std::less<typename RandomAccessIterator::value_type>>
inline RandomAccessIterator find_median(
    const RandomAccessIterator first,
    const RandomAccessIterator last,
    Compare comp = Compare{}
) noexcept {
    insertion_sort(first, last, comp);
    return first + (std::distance(first, last) - 1)/2;
}

}  // namespace detail

inline namespace literals {

constexpr uint8_t operator"" _u8(uint64_t val) noexcept {
    return static_cast<uint8_t>(val);
}

}  // namespace literals

template<class RandomAccessIterator,
         class Compare = std::less<typename RandomAccessIterator::value_type>>
inline RandomAccessIterator quick_select(
    const RandomAccessIterator first,
    const RandomAccessIterator last,
    std::size_t k,
    Compare comp = Compare{}
) {
    static constexpr auto GROUP_SIZE = 5_u8;

    if (last - 1 <= first) {
        return last - 1;
    }

    std::size_t n = std::distance(first, last);

    std::size_t medians_count = std::ceil(static_cast<double>(n) / GROUP_SIZE);
    std::vector<typename RandomAccessIterator::value_type> medians;
    medians.reserve(medians_count);

    std::size_t i;
    for (i = 0; i < n / GROUP_SIZE; ++i) {
        medians.push_back(*detail::find_median(first + i*GROUP_SIZE, first + (i + 1)*GROUP_SIZE));
    }
    if (i*GROUP_SIZE < n) {
        medians.push_back(*detail::find_median(first + i*GROUP_SIZE, last));
    }

    auto median_of_medians = (medians.size() == 1)
        ? first + (last - first - 1) / 2
        : std::find(first, last, *quick_select(medians.begin(), medians.end(), (medians.size() - 1) / 2, comp));

    auto pivot = partition(first, median_of_medians, last, comp);
    std::size_t index = std::distance(first, pivot);

    if (k < index) {
        return quick_select(first, pivot, k, comp);
    }
    if (k > index) {
        return quick_select(++pivot, last, k - index - 1, comp);
    }
    return pivot;
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
    auto pivot = partition_random(first, last, comp);
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
) noexcept {
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

template<class RandomAccessIterator,
         class Compare = std::less<typename RandomAccessIterator::value_type>>
inline void heapify_down(
    const RandomAccessIterator first,
    const RandomAccessIterator last,
    std::size_t i,
    Compare comp = Compare{}
) noexcept {
    std::size_t n = last - first;
    while (true) {
        auto left = i*2 + 1;
        auto right = i*2 + 2;

        auto largest = i;
        if (left < n && comp(*(first + largest), *(first + left))) {
            largest = left;
        }
        if (right < n && comp(*(first + largest), *(first + right))) {
            largest = right;
        }

        if (largest == i) {
            return;
        }

        std::iter_swap(first + largest, first + i);
        i = largest;
    }
}

template<class RandomAccessIterator,
         class Compare = std::less<typename RandomAccessIterator::value_type>>
inline void make_heap(
    const RandomAccessIterator first,
    const RandomAccessIterator last,
    Compare comp = Compare{}
) noexcept {
    // https://stackoverflow.com/a/3611799/15143062
    for (std::size_t i = (last - first) / 2; i-- > 0;) {
        heapify_down(first, last, i, comp);
    }
}

template<class RandomAccessIterator,
         class Compare = std::less<typename RandomAccessIterator::value_type>>
inline void heap_sort(
    RandomAccessIterator first,
    RandomAccessIterator last,
    Compare comp = Compare{}
) noexcept {
    if (first == last) {
        return;
    }
    alg::make_heap(first, last, comp);
    for (--last; last != first; --last) {
        std::iter_swap(first, last);
        heapify_down(first, last, 0, comp);
    }
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
    for (auto it = last; it-- != first;) {  // https://stackoverflow.com/a/3611799/15143062
        --counter[*it];
        temp[counter[*it]] = *it;
    }

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

template<class BidirectionalIterator,
         class Int = typename BidirectionalIterator::value_type,
         class = typename std::enable_if<std::is_integral<Int>::value>::type>
inline void counting_sort_digit(
    BidirectionalIterator first,
    BidirectionalIterator last,
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
    for (auto it = last; it-- != first;) {  // https://stackoverflow.com/a/3611799/15143062
        const auto digit = (*it / exp) % 10;
        --counter[digit];
        temp[counter[digit]] = *it;
    }

    std::copy(temp.begin(), temp.end(), first);
}

}

template<class BidirectionalIterator,
         class Int = typename BidirectionalIterator::value_type,
         class = typename std::enable_if<std::is_integral<Int>::value>::type>
inline void radix_sort(
    const BidirectionalIterator first,
    const BidirectionalIterator last,
    Int max,
    std::size_t n
) {
    for (Int exp = 1; max / exp > 0; exp *= 10) {
        detail::counting_sort_digit(first, last, exp, n);
    }
}

template<class RandomAccessIterator,
         class Int = typename RandomAccessIterator::value_type,
         class = typename std::enable_if<std::is_integral<Int>::value>::type>
inline void radix_sort(
    const RandomAccessIterator first,
    const RandomAccessIterator last,
    Int max
) {
    radix_sort(first, last, max, last - first);
}

template<class ForwardIterator,
         class Float = typename ForwardIterator::value_type,
         class = typename std::enable_if<std::is_floating_point<Float>::value>::type>
inline void bucket_sort(ForwardIterator first, ForwardIterator last, std::size_t n) {
    std::vector<std::forward_list<Float>> buckets(n);
    for (auto it = first; it != last; ++it) {
        buckets[std::floor(*it * n)].push_front(*it);
    }

    std::for_each(buckets.begin(), buckets.end(), [](std::forward_list<Float>& bucket) { bucket.sort(); });

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
