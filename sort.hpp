//
// author: Armin Gh (arminghorbanian02@gmail.com)
//
// This file implements following sorting algorithms:
//    bubble_sort       stable      in-place
//    insertion_sort    stable      in-place
//    selection_sort    unstable    in-place
//    merge_sort        stable      not-in-place
//    quick_sort        unstable    in-place
//    heap_sort         unstable    in-place
//    counting_sort     stable      not-in-place
//    radix_sort        stable      not-in-place
//    bucket_sort       stable      not-in-place
//
// And following sorting-related algorithms:
//    merge
//    is_pivot
//    partition
//    quick_select
//    heapify_down
//    make_heap
//

#ifndef SORT_HPP
#define SORT_HPP

#include <algorithm>
#include <chrono>
#include <cmath>
#include <forward_list>
#include <iterator>
#include <list>
#include <memory>
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

    BidirectionalIterator current, next, last_modified;
    for (--last; first != last; last = last_modified) {
        current = next = last_modified = first;

        for (++next; current != last; current = next, ++next) {
            if (comp(*next, *current)) {
                std::iter_swap(current, next);
                last_modified = current;
            }
        }
    }
}

template<class BidirectionalIterator,
         class T = typename BidirectionalIterator::value_type,
         class Compare = std::less<T>>
inline void insertion_sort(
    BidirectionalIterator first,
    BidirectionalIterator last,
    Compare comp = Compare{}
) noexcept(std::is_nothrow_move_assignable<T>::value) {
    if (first == last) {
        return;
    }

    auto it = first;
    for (++it; it != last; ++it) {
        const auto key = std::move(*it);
        auto insertPos = it;
        for (auto movePos = it; movePos != first && comp(key, *(--movePos)); --insertPos) {
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

template<class InputIterator,
         class OutputIterator,
         class T = typename InputIterator::value_type,
         class Compare = std::less<T>>
inline OutputIterator merge(
    InputIterator first1,
    InputIterator last1,
    InputIterator first2,
    InputIterator last2,
    OutputIterator result,
    Compare comp = Compare{}
) noexcept(std::is_nothrow_move_assignable<T>::value) {
    while (first1 != last1 && first2 != last2) {
        if (comp(*first1, *first2)) {
            *result = std::move(*first1);
            ++first1;
        } else {
            *result = std::move(*first2);
            ++first2;
        }
        ++result;
    }

    if (first1 == last1) {
        return std::move(first2, last2, result);
    } else {
        return std::move(first1, last1, result);
    }
}

namespace detail {

template<class RandomAccessIterator,
         std::size_t InsertionSortLimit,
         class Compare = std::less<typename RandomAccessIterator::value_type>>
class MergeSorter {

public:
    using pointer = typename RandomAccessIterator::pointer;

    static void sort(
        RandomAccessIterator first,
        RandomAccessIterator last,
        pointer buffer,
        Compare comp = Compare{}
    ) {
        if (sort_impl(first, last, buffer, comp) == ResultLocation::buf) {
            auto n = last - first;
            std::move(buffer, buffer + n, first);
        }
    }

private:
    enum class ResultLocation : bool {
        src,
        buf,
    };

    static ResultLocation sort_impl(
        RandomAccessIterator first,
        RandomAccessIterator last,
        pointer buffer,
        Compare comp = Compare{}
    ) {
        auto n = last - first;

        if (n <= 1) {
            return ResultLocation::src;
        }

        if (n <= InsertionSortLimit) {
            insertion_sort(first, last, comp);
            return ResultLocation::src;
        }

        auto mid = n / 2;

        auto first_half_location = sort_impl(first, first + mid, buffer, comp);
        auto second_half_location = sort_impl(first + mid, last, buffer + mid, comp);

        return merge_halves(first, last, mid, buffer, first_half_location, second_half_location, comp);
    }

    static ResultLocation merge_halves(
        RandomAccessIterator first,
        RandomAccessIterator last,
        std::size_t mid,
        pointer buffer,
        ResultLocation first_half_location,
        ResultLocation second_half_location,
        Compare comp = Compare{}
    ) {
        auto n = last - first;
        if (first_half_location == ResultLocation::src) {
            if (second_half_location == ResultLocation::src) {
                merge(first, first + mid, first + mid, last, buffer, comp);
                return ResultLocation::buf;
            } else {
                std::move(first, first + mid, buffer);
                merge(buffer, buffer + mid, buffer + mid, buffer + n, first, comp);
                return ResultLocation::src;
            }
        } else {
            if (second_half_location == ResultLocation::src) {
                std::move(first + mid, last, buffer + mid);
                merge(buffer, buffer + mid, buffer + mid, buffer + n, first, comp);
                return ResultLocation::src;
            } else {
                merge(buffer, buffer + mid, buffer + mid, buffer + n, first, comp);
                return ResultLocation::src;
            }
        }
    }
};

}

template<class RandomAccessIterator,
         class T = typename RandomAccessIterator::value_type,
         class Compare = std::less<T>>
inline void merge_sort_buf(
    RandomAccessIterator first,
    RandomAccessIterator last,
    T* buffer,
    Compare comp = Compare{}
) {
    detail::MergeSorter<RandomAccessIterator, 16, Compare>::sort(first, last, buffer, comp);
}

template<class RandomAccessIterator,
         class Allocator,
         class T = typename RandomAccessIterator::value_type,
         class Compare = std::less<T>>
inline void merge_sort(
    RandomAccessIterator first,
    RandomAccessIterator last,
    Allocator& allocator,
    Compare comp = Compare{}
) {
    auto n = last - first;

    if (n <= 1) {
        return;
    }

    auto buffer = std::allocator_traits<Allocator>::allocate(allocator, n);
    std::uninitialized_fill(buffer, buffer + n, T());

    merge_sort_buf(first, last, buffer, comp);

    std::allocator_traits<Allocator>::destroy(allocator, buffer);
    std::allocator_traits<Allocator>::deallocate(allocator, buffer, n);
}

template<class RandomAccessIterator,
         class T = typename RandomAccessIterator::value_type,
         class Compare = std::less<T>>
inline void merge_sort(
    RandomAccessIterator first,
    RandomAccessIterator last,
    Compare comp = Compare{}
) {
    std::allocator<T> allocator;
    merge_sort(first, last, allocator, comp);
}

template<class BidirectionalIterator,
         class T = typename BidirectionalIterator::value_type,
         class Compare = std::less<T>>
inline bool is_pivot(
    BidirectionalIterator first,
    BidirectionalIterator pivot,
    BidirectionalIterator last,
    Compare comp = Compare{}
) noexcept {
    return std::all_of(first, pivot, [pivot](const T& element) { return element <= *pivot; })
        && std::all_of(pivot, last, [pivot](const T& element) { return element >= *pivot; });
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

    auto it = first;
    for (; first != last; ++first) {
        if (comp(*first, *last)) {
            std::iter_swap(first, it);
            ++it;
        }
    }

    std::iter_swap(last, it);
    return it;
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
    RandomAccessIterator first,
    RandomAccessIterator last,
    Compare comp = Compare{}
) {
    static std::mt19937 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<> dist(0, last - first - 1);

    auto pivot = first + dist(gen);
    return partition(first, pivot, last, comp);
}

namespace detail {

template<class RandomAccessIterator,
         class Compare = std::less<typename RandomAccessIterator::value_type>>
inline RandomAccessIterator find_median(
    RandomAccessIterator first,
    RandomAccessIterator last,
    Compare comp = Compare{}
) noexcept {
    insertion_sort(first, last, comp);
    return first + (last - first - 1)/2;
}

}  // namespace detail

inline namespace literals {

constexpr uint8_t operator"" _u8(uint64_t val) noexcept {
    return static_cast<uint8_t>(val);
}

}  // namespace literals

template<class RandomAccessIterator,
         class T = typename RandomAccessIterator::value_type,
         class Compare = std::less<T>>
inline void quick_select(
    RandomAccessIterator first,
    RandomAccessIterator kth,
    RandomAccessIterator last,
    Compare comp = Compare{}
) {
    static constexpr auto GROUP_SIZE = 5_u8;

    if (last - 1 <= first) {
        return;
    }

    std::size_t n = last - first;

    std::size_t medians_count = std::ceil(static_cast<double>(n) / GROUP_SIZE);
    std::vector<T> medians;
    medians.reserve(medians_count);

    std::size_t i;
    for (i = 0; i < n / GROUP_SIZE; ++i) {
        medians.push_back(*detail::find_median(first + i*GROUP_SIZE, first + (i + 1)*GROUP_SIZE));
    }
    if (i*GROUP_SIZE < n) {
        medians.push_back(*detail::find_median(first + i*GROUP_SIZE, last));
    }

    RandomAccessIterator median_of_medians;
    if (medians.size() == 1) {
        median_of_medians = first + (last - first - 1) / 2;
    } else {
        median_of_medians = medians.begin() + (medians.size() - 1)/2;
        quick_select(medians.begin(), median_of_medians, medians.end(), comp);
        median_of_medians = std::find(first, last, *median_of_medians);
    }

    auto pivot = partition(first, median_of_medians, last, comp);
    if (kth < pivot) {
        quick_select(first, kth, pivot, comp);
    } else if (kth > pivot) {
        quick_select(++pivot, kth, last, comp);
    }
}

namespace detail {

template<class RandomAccessIterator,
         class Compare = std::less<typename RandomAccessIterator::value_type>>
inline void quick_sort_impl(
    RandomAccessIterator first,
    RandomAccessIterator last,
    std::random_access_iterator_tag iter_tag,
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
    BidirectionalIterator first,
    BidirectionalIterator last,
    Compare comp = Compare{}
) {
    using iter_category = typename std::iterator_traits<BidirectionalIterator>::iterator_category;
    return detail::quick_sort_impl(first, last, iter_category{}, comp);
}

template<class RandomAccessIterator,
         class Compare = std::less<typename RandomAccessIterator::value_type>>
inline void heapify_down(
    RandomAccessIterator first,
    RandomAccessIterator last,
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
    RandomAccessIterator first,
    RandomAccessIterator last,
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
    BidirectionalIterator first,
    BidirectionalIterator last,
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
    RandomAccessIterator first,
    RandomAccessIterator last,
    Int max
) {
    radix_sort(first, last, max, last - first);
}

namespace detail {

template<class ForwardIterator,
         class Float = typename ForwardIterator::value_type,
         class = typename std::enable_if<std::is_floating_point<Float>::value>::type>
inline void bucket_sort_impl(
    ForwardIterator first,
    ForwardIterator last,
    std::forward_iterator_tag,
    std::size_t n
) {
    std::vector<std::list<Float>> buckets(n);

    // https://stackoverflow.com/a/3611799/15143062
    for (auto it = first; it != last; ++it) {
        buckets[std::floor(*it * n)].push_back(*it);
    }

    std::for_each(buckets.begin(), buckets.end(), [](std::list<Float>& bucket) { bucket.sort(); });

    for (const auto& bucket : buckets) {
        for (auto element : bucket) {
            *first = element;
            ++first;
        }
    }
}

template<class BidirectionalIterator,
         class Float = typename BidirectionalIterator::value_type,
         class = typename std::enable_if<std::is_floating_point<Float>::value>::type>
inline void bucket_sort_impl(
    BidirectionalIterator first,
    BidirectionalIterator last,
    std::bidirectional_iterator_tag,
    std::size_t n
) {
    std::vector<std::forward_list<Float>> buckets(n);

    // we traverse in reverse order so that the algorithm remain stable
    // https://stackoverflow.com/a/3611799/15143062
    for (auto it = last; it-- != first;) {
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

}

template<class ForwardIterator,
         class Float = typename ForwardIterator::value_type,
         class = typename std::enable_if<std::is_floating_point<Float>::value>::type>
inline void bucket_sort(ForwardIterator first, ForwardIterator last, std::size_t n) {
    using iter_category = typename std::iterator_traits<ForwardIterator>::iterator_category;
    detail::bucket_sort_impl(first, last, iter_category{}, n);
}

template<class RandomAccessIterator,
         class Float = typename RandomAccessIterator::value_type,
         class = typename std::enable_if<std::is_floating_point<Float>::value>::type>
inline void bucket_sort(RandomAccessIterator first, RandomAccessIterator last) {
    detail::bucket_sort_impl(first, last, std::bidirectional_iterator_tag{}, last - first);
}

}  // namespace alg

#endif  // SORT_HPP
