#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

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

template<typename RandomAccessIterator, typename Compare>
RandomAccessIterator selection(RandomAccessIterator first,
                               RandomAccessIterator last,
                               typename RandomAccessIterator::difference_type k,
                               Compare comp = std::less<typename RandomAccessIterator::value_type>()) {

}

template<typename RandomAccessIterator, typename Compare>
RandomAccessIterator partition(RandomAccessIterator first,
                               RandomAccessIterator last,
                               Compare comp = std::less<typename RandomAccessIterator::value_type>()) {
    auto median = selection(first, last, std::distance(first, last) / 2, comp);

    std::iter_swap(median, last - 1);

    auto iter = first - 1;
    for (; first != last; ++first) {
        if (comp(*first, *median)) {
            ++iter;
            std::iter_swap(*first, *iter);
        }
    }

    ++iter;
    std::iter_swap(--last, iter);
    return iter;
}

}  // namespace alg

#endif  // ALGORITHMS_HPP
