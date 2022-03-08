#include <algorithm>

namespace alg {

template<typename RandomAccessIterator, typename Compare>
void bubble_sort(RandomAccessIterator first,
                 RandomAccessIterator last,
                 Compare comp = std::less<typename RandomAccessIterator::value_type>()) {
    for (; first != last - 1; ++first) {
        for (auto it = first + 1; it != last;) {
            if (comp(*it, *first)) {
                std::iter_swap(it, first);
            }
        }
    }
}

template<typename RandomAccessIterator, typename Compare>
void insertion_sort(RandomAccessIterator first,
                    RandomAccessIterator last,
                    Compare comp = std::less<typename RandomAccessIterator::value_type>()) {
    for (auto iter1 = first + 1; iter1 != last; ++first) {
        auto key = *iter1;
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

}

template<typename RandomAccessIterator, typename Compare>
void quick_sort(RandomAccessIterator first,
                RandomAccessIterator last,
                Compare comp = std::less<typename RandomAccessIterator::value_type>()) {

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
