/*
 * author: Armin Gh (arminghorbanian02@gmail.com)
 *
 * This file implements the following sorting algorithms:
 *    bubble_sort       stable      in-place
 *    insertion_sort    stable      in-place
 *    selection_sort    unstable    in-place
 *    merge_sort        stable      not-in-place
 *    quick_sort        unstable    in-place        (the introsort variant)
 *    heap_sort         unstable    in-place
 *    counting_sort     stable      not-in-place
 *    radix_sort        stable      not-in-place
 *    bucket_sort       stable      not-in-place
 *
 * And the following sorting-related algorithms:
 *    merge
 *    partition
 *    quick_select
 *    heapify_down
 *    make_heap
 */

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

namespace detail {

template<class ForwardIterator, class Compare>
inline void bubble_sort_impl(
    ForwardIterator first,
    ForwardIterator last,
    Compare compare,
    std::forward_iterator_tag
) noexcept {
    if (first == last) {
        return;
    }

    ForwardIterator current, next;
    for (; first != last; last = current) {
        current = next = first;

        for (++next; next != last; current = next, ++next) {
            if (compare(*next, *current)) {
                std::iter_swap(current, next);
            }
        }
    }
}

template<class BidirectionalIterator, class Compare>
inline void bubble_sort_impl(
    BidirectionalIterator first,
    BidirectionalIterator last,
    Compare compare,
    std::bidirectional_iterator_tag
) noexcept {
    if (first == last) {
        return;
    }

    BidirectionalIterator current, next, last_modified;
    for (--last; first != last; last = last_modified) {
        current = next = last_modified = first;

        for (++next; current != last; current = next, ++next) {
            if (compare(*next, *current)) {
                std::iter_swap(current, next);
                last_modified = current;
            }
        }
    }
}

}  // namespace detail

/**
 * @brief bubble sort algorithm
 *
 * @details This stable in-place O(n^2) algorithm is mostly useful
 * for small ranges containing less than 10 elements.
 * Usually you would be better off using alg::insertion_sort.
 *
 * @param first a forward iterator
 * @param last a forward iterator
 * @param compare a comparison functor
 */
template<class ForwardIterator, class Compare>
inline void bubble_sort(
    ForwardIterator first,
    ForwardIterator last,
    Compare compare
) noexcept {
    using iter_category = typename std::iterator_traits<ForwardIterator>::iterator_category;
    detail::bubble_sort_impl(first, last, compare, iter_category{});
}

template<class ForwardIterator>
inline void bubble_sort(ForwardIterator first, ForwardIterator last) noexcept {
    using value_type = typename std::iterator_traits<ForwardIterator>::value_type;
    bubble_sort(first, last, std::less<value_type>());
}

/**
 * @brief insertion sort algorithm
 *
 * @details This is a stable in-place O(n^2) algorithm.
 * It is also used in alg::merge_sort and alg::quick_sort
 * when the range gets smaller than 16 elements.
 *
 * @param first a bidirectional iterator
 * @param last a bidirectional iterator
 * @param compare a comparison functor
 */
template<
    class BidirectionalIterator,
    class Compare,
    class T = typename std::iterator_traits<BidirectionalIterator>::value_type
>
inline void insertion_sort(
    BidirectionalIterator first,
    BidirectionalIterator last,
    Compare compare
) noexcept(std::is_nothrow_move_assignable<T>::value) {
    if (first == last) {
        return;
    }

    auto it = first;
    for (++it; it != last; ++it) {
        const auto key = std::move(*it);
        auto insertPos = it;
        for (auto movePos = it; movePos != first && compare(key, *(--movePos)); --insertPos) {
            *insertPos = std::move(*movePos);
        }

        *insertPos = std::move(key);
    }
}

template<
    class BidirectionalIterator,
    class T = typename std::iterator_traits<BidirectionalIterator>::value_type
>
inline void insertion_sort(
    BidirectionalIterator first,
    BidirectionalIterator last
) noexcept(std::is_nothrow_move_assignable<T>::value) {
    insertion_sort(first, last, std::less<T>());
}

/**
 * @brief selection sort algorithm
 *
 * @details This unstable in-place O(n^2) algorithm is
 * generally faster than bubble sort but slower than insertion sort.
 * The good thing about selection sort is it never makes more than O(n)
 * swaps and can be useful when memory write is a costly operation.
 *
 * @param first a forward iterator
 * @param last a forward iterator
 * @param compare a comparison functor
 */
template<class ForwardIterator, class Compare>
inline void selection_sort(
    ForwardIterator first,
    ForwardIterator last,
    Compare compare
) noexcept {
    if (first == last) {
        return;
    }

    for (; first != last; ++first) {
        auto minPos = first;
        auto it = first;
        for (++it; it != last; ++it) {
            if (compare(*it, *minPos)) {
                minPos = it;
            }
        }

        std::iter_swap(first, minPos);
    }
}

template<class BidirectionalIterator>
inline void selection_sort(
    BidirectionalIterator first,
    BidirectionalIterator last
) noexcept {
    using value_type = typename std::iterator_traits<BidirectionalIterator>::value_type;
    selection_sort(first, last, std::less<value_type>());
}

/**
 * @brief heapify down algorithm (iterative)
 *
 * @details This in-place O(log(n)) algorithm compares the i-th element
 * in a binary heap with its children and swaps if a child is larger.
 * This is repeated for the child until it becomes larger than its children.
 *
 * @param first a random access iterator
 * @param last a random access iterator
 * @param i the index at which we start to heapify
 * @param compare a comparison functor
 */
template<class RandomAccessIterator, class Compare>
inline void heapify_down(
    RandomAccessIterator first,
    RandomAccessIterator last,
    std::size_t i,
    Compare compare
) noexcept {
    std::size_t n = last - first;
    while (true) {
        auto left = i*2 + 1;
        auto right = i*2 + 2;

        auto largest = i;
        if (left < n && compare(*(first + largest), *(first + left))) {
            largest = left;
        }
        if (right < n && compare(*(first + largest), *(first + right))) {
            largest = right;
        }

        if (largest == i) {
            return;
        }

        std::iter_swap(first + largest, first + i);
        i = largest;
    }
}

template<class RandomAccessIterator>
inline void heapify_down(
    RandomAccessIterator first,
    RandomAccessIterator last,
    std::size_t i
) noexcept {
    using value_type = typename std::iterator_traits<RandomAccessIterator>::value_type;
    heapify_down(first, last, i, std::less<value_type>());
}

/**
 * @brief make heap algorithm
 *
 * @details This in-place O(n) algorithm turns the specified range into a binary heap.
 * It starts from the last non-leaf node (which is at index (n/2 - 1))
 * and heapifies down each node until it reaches the root.
 *
 * @param first a random access iterator
 * @param last a random access iterator
 * @param compare a comparison functor
 */
template<class RandomAccessIterator, class Compare>
inline void make_heap(
    RandomAccessIterator first,
    RandomAccessIterator last,
    Compare compare
) noexcept {
    // https://stackoverflow.com/a/3611799/15143062
    for (std::size_t i = (last - first) >> 1; i-- > 0;) {
        heapify_down(first, last, i, compare);
    }
}

template<class RandomAccessIterator>
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last) noexcept {
    using value_type = typename std::iterator_traits<RandomAccessIterator>::value_type;
    make_heap(first, last, std::less<value_type>());
}

/**
 * @brief heap sort algorithm
 *
 * @details This unstable in-place O(n*log(n)) algorithm first calls
 * alg::make_heap on the range to build a max heap. Next it divides its
 * input into a sorted and an unsorted region (like in selection sort),
 * and then iteratively shrinks the unsorted region by extracting
 * the largest element and inserting it into the sorted region.
 *
 * @param first a random access iterator
 * @param last a random access iterator
 * @param compare a comparison functor
 */
template<class RandomAccessIterator, class Compare>
inline void heap_sort(
    RandomAccessIterator first,
    RandomAccessIterator last,
    Compare compare
) noexcept {
    if (first == last) {
        return;
    }
    alg::make_heap(first, last, compare);
    for (--last; last != first; --last) {
        std::iter_swap(first, last);
        heapify_down(first, last, 0, compare);
    }
}

template<class RandomAccessIterator>
inline void heap_sort(
    RandomAccessIterator first,
    RandomAccessIterator last
) noexcept {
    using value_type = typename std::iterator_traits<RandomAccessIterator>::value_type;
    heap_sort(first, last, std::less<value_type>());
}

/**
 * @brief merge two sorted ranges algorithm
 *
 * @details Merges the sorted ranges @p [first1,last1) and @p [first2,last2) into
 * the sorted range @p [result,result+(last1-first1)+(last2-first2)).
 * The output range must not overlap with either of the input ranges.
 * The merge is stable, that is, for equivalent elements in the two ranges,
 * elements from the first range will always come before the second.
 *
 * @note A call to this function causes the ranges @p [first1,last1) and @p [first2,last2)
 * to contain elements which are in an unspecified state due to being moved.
 *
 * @param first1 an input iterator
 * @param last1 an input iterator
 * @param first2 an input iterator
 * @param last2 an input iterator
 * @param result an output iterator
 * @param compare a comparison functor
 * @return an output iterator to the element following the last moved element.
 */
template<
    class InputIterator1,
    class InputIterator2,
    class OutputIterator,
    class Compare,
    class T = typename std::iterator_traits<InputIterator1>::value_type
>
inline OutputIterator merge(
    InputIterator1 first1,
    InputIterator1 last1,
    InputIterator2 first2,
    InputIterator2 last2,
    OutputIterator result,
    Compare compare
) noexcept(std::is_nothrow_move_assignable<T>::value) {
    while (first1 != last1 && first2 != last2) {
        if (compare(*first1, *first2)) {
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

template<
    class InputIterator1,
    class InputIterator2,
    class OutputIterator,
    class T = typename std::iterator_traits<InputIterator1>::value_type
>
inline OutputIterator merge(
    InputIterator1 first1,
    InputIterator1 last1,
    InputIterator2 first2,
    InputIterator2 last2,
    OutputIterator result
) noexcept(std::is_nothrow_move_assignable<T>::value) {
    alg::merge(first1, last1, first2, last2, result, std::less<T>());
}

namespace detail {

template<
    class RandomAccessIterator,
    class Compare,
    uint8_t InsertionSortLimit
>
class MergeSorter {

public:
    using pointer = typename std::iterator_traits<RandomAccessIterator>::pointer;
    using difference_type = typename std::iterator_traits<RandomAccessIterator>::difference_type;

    static void sort(
        RandomAccessIterator first,
        RandomAccessIterator last,
        pointer buffer,
        Compare compare
    ) {
        if (sort_impl(first, last, buffer, compare) == ResultLocation::buf) {
            auto n = last - first;
            std::move(buffer, buffer + n, first);
        }
    }

private:
    enum class ResultLocation : bool {
        src,  // indicates that the result is in the source range
        buf,  // indicates that the result is in the buffer
    };

    static ResultLocation sort_impl(
        RandomAccessIterator first,
        RandomAccessIterator last,
        pointer buffer,
        Compare compare
    ) {
        auto n = last - first;

        if (n <= 1) {
            return ResultLocation::src;
        }

        if (n <= InsertionSortLimit) {
            insertion_sort(first, last, compare);
            return ResultLocation::src;
        }

        auto mid = n >> 1;

        auto first_half_location = sort_impl(first, first + mid, buffer, compare);
        auto second_half_location = sort_impl(first + mid, last, buffer + mid, compare);

        return merge_halves(first, last, mid, buffer, first_half_location, second_half_location, compare);
    }

    static ResultLocation merge_halves(
        RandomAccessIterator first,
        RandomAccessIterator last,
        difference_type mid,
        pointer buffer,
        ResultLocation first_half_location,
        ResultLocation second_half_location,
        Compare compare
    ) {
        auto n = last - first;
        if (first_half_location == ResultLocation::src) {
            if (second_half_location == ResultLocation::src) {
                alg::merge(first, first + mid, first + mid, last, buffer, compare);
                return ResultLocation::buf;
            } else {
                std::move(first, first + mid, buffer);
                alg::merge(buffer, buffer + mid, buffer + mid, buffer + n, first, compare);
                return ResultLocation::src;
            }
        } else {
            if (second_half_location == ResultLocation::src) {
                std::move(first + mid, last, buffer + mid);
                alg::merge(buffer, buffer + mid, buffer + mid, buffer + n, first, compare);
                return ResultLocation::src;
            } else {
                alg::merge(buffer, buffer + mid, buffer + mid, buffer + n, first, compare);
                return ResultLocation::src;
            }
        }
    }

};  // class MergeSorter

}  // namespace detail

template<
    class RandomAccessIterator,
    class Compare,
    class T = typename std::iterator_traits<RandomAccessIterator>::value_type
>
inline void merge_sort_buf(
    RandomAccessIterator first,
    RandomAccessIterator last,
    T* buffer,
    Compare compare
) {
    detail::MergeSorter<RandomAccessIterator, Compare, 16>::sort(first, last, buffer, compare);
}

template<
    class RandomAccessIterator,
    class T = typename std::iterator_traits<RandomAccessIterator>::value_type
>
inline void merge_sort_buf(
    RandomAccessIterator first,
    RandomAccessIterator last,
    T* buffer
) {
    merge_sort_buf(first, last, buffer, std::less<T>());
}

/**
 * @brief merge sort algorithm
 *
 * @details This is a stable not-in-place O(n*log(n)) divide and conquer algorithm.
 *
 * @note Switches to insertion sort when the range gets smaller than 16 elements.
 *
 * @param first a random access iterator
 * @param last a random access iterator
 * @param allocator an allocator object
 * @param compare a comparison functor
 */
template<
    class RandomAccessIterator,
    class Allocator,
    class Compare
>
inline void merge_sort(
    RandomAccessIterator first,
    RandomAccessIterator last,
    Allocator& allocator,
    Compare compare
) {
    using value_type = typename std::iterator_traits<RandomAccessIterator>::value_type;

    auto n = last - first;

    if (n <= 1) {
        return;
    }

    auto buffer = std::allocator_traits<Allocator>::allocate(allocator, n);
    std::uninitialized_fill(buffer, buffer + n, value_type());

    merge_sort_buf(first, last, buffer, compare);

    std::allocator_traits<Allocator>::destroy(allocator, buffer);
    std::allocator_traits<Allocator>::deallocate(allocator, buffer, n);
}

template<class RandomAccessIterator, class Compare>
inline void merge_sort(
    RandomAccessIterator first,
    RandomAccessIterator last,
    Compare compare
) {
    using value_type = typename std::iterator_traits<RandomAccessIterator>::value_type;
    std::allocator<value_type> allocator;
    merge_sort(first, last, allocator, compare);
}

template<class RandomAccessIterator, class Allocator>
inline void merge_sort(
    RandomAccessIterator first,
    RandomAccessIterator last,
    Allocator& allocator
) {
    using value_type = typename std::iterator_traits<RandomAccessIterator>::value_type;
    merge_sort(first, last, allocator, std::less<value_type>());
}

template<class RandomAccessIterator>
inline void merge_sort(
    RandomAccessIterator first,
    RandomAccessIterator last
) {
    using value_type = typename std::iterator_traits<RandomAccessIterator>::value_type;
    std::allocator<value_type> allocator;
    merge_sort(first, last, allocator, std::less<value_type>());
}

template<class BidirectionalIterator, class Compare>
inline BidirectionalIterator partition(
    BidirectionalIterator first,
    BidirectionalIterator pivot,
    BidirectionalIterator last,
    Compare compare
) noexcept {
    --last;
    std::iter_swap(pivot, last);

    auto it = first;
    for (; first != last; ++first) {
        if (compare(*first, *last)) {
            std::iter_swap(first, it);
            ++it;
        }
    }

    std::iter_swap(last, it);
    return it;
}

template<class BidirectionalIterator>
inline BidirectionalIterator partition(
    BidirectionalIterator first,
    BidirectionalIterator pivot,
    BidirectionalIterator last
) noexcept {
    using value_type = typename std::iterator_traits<BidirectionalIterator>::value_type;
    partition(first, pivot, last, std::less<value_type>());
}

template<class BidirectionalIterator, class Compare>
inline BidirectionalIterator partition_pivot_last(
    BidirectionalIterator first,
    BidirectionalIterator last,
    Compare compare
) noexcept {
    auto it = last;
    return partition(first, --it, last, compare);
}

template<class BidirectionalIterator>
inline BidirectionalIterator partition_pivot_last(
    BidirectionalIterator first,
    BidirectionalIterator last
) noexcept {
    using value_type = typename std::iterator_traits<BidirectionalIterator>::value_type;
    partition_pivot_last(first, last, std::less<value_type>());
}

template<class RandomAccessIterator, class Compare>
inline RandomAccessIterator partition_random(
    RandomAccessIterator first,
    RandomAccessIterator last,
    Compare compare
) {
    static std::mt19937 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<> dist(0, last - first - 1);

    auto pivot = first + dist(gen);
    return partition(first, pivot, last, compare);
}

template<class RandomAccessIterator>
inline RandomAccessIterator partition_random(
    RandomAccessIterator first,
    RandomAccessIterator last
) {
    using value_type = typename std::iterator_traits<RandomAccessIterator>::value_type;
    partition_random(first, last, std::less<value_type>());
}

namespace detail {

template<class RandomAccessIterator, class Compare>
inline RandomAccessIterator find_median(
    RandomAccessIterator first,
    RandomAccessIterator last,
    Compare compare
) noexcept {
    insertion_sort(first, last, compare);
    return first + ((last - first - 1) >> 1);
}

}  // namespace detail

inline namespace literals {

constexpr uint8_t operator"" _u8(uint64_t val) noexcept {
    return static_cast<uint8_t>(val);
}

}  // namespace literals

/**
 * @brief quick select algorithm
 *
 * @details It divides the range into groups of 5 elements,
 * sorts each group, and find their median. Then recursively calls itself for
 * the medians of the groups to find the median of those medians.
 * It will then pick the median of medians as the pivot and partitions the range.
 *
 * @param first a random access iterator
 * @param kth a random access iterator which will point to the k-th element
 *            of the sorted array after the function is called.
 * @param last a random access iterator
 * @param compare a comparison functor
 */
template<class RandomAccessIterator, class Compare>
inline void quick_select(
    RandomAccessIterator first,
    RandomAccessIterator kth,
    RandomAccessIterator last,
    Compare compare
) {
    using value_type = typename std::iterator_traits<RandomAccessIterator>::value_type;

    constexpr auto GROUP_SIZE = 5_u8;

    auto n = last - first;

    if (n <= 1) {
        return;
    }

    std::size_t medians_count = std::ceil(static_cast<double>(n) / GROUP_SIZE);
    std::vector<value_type> medians;
    medians.reserve(medians_count);

    std::size_t i;
    for (i = 0; i < n / GROUP_SIZE; ++i) {
        medians.push_back(*detail::find_median(first + i*GROUP_SIZE, first + (i + 1)*GROUP_SIZE, compare));
    }
    if (i*GROUP_SIZE < n) {
        medians.push_back(*detail::find_median(first + i*GROUP_SIZE, last, compare));
    }

    RandomAccessIterator median_of_medians;
    if (medians.size() == 1) {
        median_of_medians = first + ((last - first - 1) >> 1);
    } else {
        median_of_medians = medians.begin() + ((medians.size() - 1) >> 1);
        quick_select(medians.begin(), median_of_medians, medians.end(), compare);
        median_of_medians = std::find(first, last, *median_of_medians);
    }

    auto pivot = partition(first, median_of_medians, last, compare);
    if (kth < pivot) {
        quick_select(first, kth, pivot, compare);
    } else if (kth > pivot) {
        quick_select(++pivot, kth, last, compare);
    }
}

template<class RandomAccessIterator>
inline void quick_select(
    RandomAccessIterator first,
    RandomAccessIterator kth,
    RandomAccessIterator last
) {
    using value_type = typename std::iterator_traits<RandomAccessIterator>::value_type;
    quick_select(first, kth, last, std::less<value_type>());
}

namespace detail {

template<class Int>
inline Int log2(Int n) {
	Int i;
	for (i = 0; n != 0; ++i) {
		n >>= 1;
    }
	return i - 1;
}

template<class RandomAccessIterator, class Compare>
inline void quick_sort_impl_helper(
    RandomAccessIterator first,
    RandomAccessIterator last,
    Compare compare,
    int recursion_count
) {
    if (last - first <= 16) { // small
        insertion_sort(first, last, compare);
        return;
    }
    if (recursion_count <= 0) { // too many divisions
        heap_sort(first, last, compare);
        return;
    }
    auto pivot = partition_random(first, last, compare);
    quick_sort_impl_helper(first, pivot, compare, recursion_count - 1);
    quick_sort_impl_helper(++pivot, last, compare, recursion_count - 1);
}

template<class RandomAccessIterator, class Compare>
inline void quick_sort_impl(
    RandomAccessIterator first,
    RandomAccessIterator last,
    Compare compare,
    std::random_access_iterator_tag
) {
    auto recursion_count = 2 * detail::log2(last - first);
    quick_sort_impl_helper(first, last, compare, recursion_count);
}

template<class BidirectionalIterator, class Compare>
inline void quick_sort_impl(
    BidirectionalIterator first,
    BidirectionalIterator last,
    Compare compare,
    std::bidirectional_iterator_tag iter_tag
) noexcept {
    if (first == last || first == --last) {
        return;
    }
    ++last;
    auto pivot = partition_pivot_last(first, last, compare);
    quick_sort_impl(first, pivot, compare, iter_tag);
    quick_sort_impl(++pivot, last, compare, iter_tag);
}

}  // namespace detail

/**
 * @brief quick sort algorithm
 *
 * @details Uses introsort if the iterator is a random access iterator.
 * Introsort uses insertion sort once the range gets small, and if the recursion depth
 * becomes more than 2*log2(n) it uses heapsort.
 * A random pivot is used for the partitioning if the iterator is a random access iterator.
 * The last element is used as pivot if the iterator is a bidirectional iterator.
 *
 * @param first a bidirectional iterator
 * @param last a bidirectional iterator
 * @param compare a comparison functor
 */
template<class BidirectionalIterator, class Compare>
inline void quick_sort(
    BidirectionalIterator first,
    BidirectionalIterator last,
    Compare compare
) {
    using iter_category = typename std::iterator_traits<BidirectionalIterator>::iterator_category;
    return detail::quick_sort_impl(first, last, compare, iter_category{});
}

template<class BidirectionalIterator>
inline void quick_sort(
    BidirectionalIterator first,
    BidirectionalIterator last
) {
    using value_type = typename std::iterator_traits<BidirectionalIterator>::value_type;
    quick_sort(first, last, std::less<value_type>());
}

/**
 * @brief counting sort algorithm
 *
 * @details This stable not-in-place algorithm is
 * of O(n+max) where max is the max value in the range.
 * It works great when the max value is relatively small and there are repeated values.
 *
 * @param first a bidirectional iterator to integer sequence
 * @param last a bidirectional iterator to integer sequence
 * @param max the max integer value in the range
 * @param n the size of the range
 */
template<
    class BidirectionalIterator,
    class Int = typename std::iterator_traits<BidirectionalIterator>::value_type,
    class = typename std::enable_if<std::is_integral<Int>::value>::type
>
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

template<
    class RandomAccessIterator,
    class Int = typename std::iterator_traits<RandomAccessIterator>::value_type,
    class = typename std::enable_if<std::is_integral<Int>::value>::type
>
inline void counting_sort(
    RandomAccessIterator first,
    RandomAccessIterator last,
    std::size_t max
) {
    counting_sort(first, last, max, last - first);
}

namespace detail {

/**
 * @brief helper function for alg::radix_sort that implements counting sort
 */
template<
    class BidirectionalIterator,
    class Int = typename std::iterator_traits<BidirectionalIterator>::value_type,
    class = typename std::enable_if<std::is_integral<Int>::value>::type
>
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

/**
 * @brief radix sort algorithm
 *
 * @details For i from 0 to max value's digits,
 * calls counting sort to sort digits at the i-th place from the right side of the integers.
 *
 * @param first a bidirectional iterator to an integer range
 * @param last a bidirectional iterator to an integer range
 * @param max the max value in the range
 * @param n the size of the range
 */
template<
    class BidirectionalIterator,
    class Int = typename std::iterator_traits<BidirectionalIterator>::value_type,
    class = typename std::enable_if<std::is_integral<Int>::value>::type
>
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

template<
    class RandomAccessIterator,
    class Int = typename std::iterator_traits<RandomAccessIterator>::value_type,
    class = typename std::enable_if<std::is_integral<Int>::value>::type
>
inline void radix_sort(
    RandomAccessIterator first,
    RandomAccessIterator last,
    Int max
) {
    radix_sort(first, last, max, last - first);
}

namespace detail {

template<
    class ForwardIterator,
    class Float = typename std::iterator_traits<ForwardIterator>::value_type,
    class = typename std::enable_if<std::is_floating_point<Float>::value>::type
>
inline void bucket_sort_impl(
    ForwardIterator first,
    ForwardIterator last,
    std::size_t n,
    std::forward_iterator_tag
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

template<
    class BidirectionalIterator,
    class Float = typename std::iterator_traits<BidirectionalIterator>::value_type,
    class = typename std::enable_if<std::is_floating_point<Float>::value>::type
>
inline void bucket_sort_impl(
    BidirectionalIterator first,
    BidirectionalIterator last,
    std::size_t n,
    std::bidirectional_iterator_tag
) {
    std::vector<std::forward_list<Float>> buckets(n);

    // we traverse in reverse order so that the algorithm remains stable
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

/**
 * @brief bucket sort algorithm
 *
 * @note Requires the given values to be floating-point numbers in the range [0, 1)
 *
 * @param first a forward iterator to a floating-point range
 * @param last a forward iterator to a floating-point range
 * @param n size of the range
 */
template<
    class ForwardIterator,
    class Float = typename std::iterator_traits<ForwardIterator>::value_type,
    class = typename std::enable_if<std::is_floating_point<Float>::value>::type
>
inline void bucket_sort(ForwardIterator first, ForwardIterator last, std::size_t n) {
    using iter_category = typename std::iterator_traits<ForwardIterator>::iterator_category;
    detail::bucket_sort_impl(first, last, n, iter_category{});
}

template<
    class RandomAccessIterator,
    class Float = typename std::iterator_traits<RandomAccessIterator>::value_type,
    class = typename std::enable_if<std::is_floating_point<Float>::value>::type
>
inline void bucket_sort(RandomAccessIterator first, RandomAccessIterator last) {
    detail::bucket_sort_impl(first, last, last - first, std::random_access_iterator_tag{});
}

}  // namespace alg

#endif  // SORT_HPP
