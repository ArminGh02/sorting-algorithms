#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include <vector>

namespace alg {

template<typename RandomAccessIterator, typename Compare>
void merge(RandomAccessIterator first,
           RandomAccessIterator mid,
           RandomAccessIterator last,
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

}  // namespace alg

#endif  // ALGORITHMS_HPP
