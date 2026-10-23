#pragma once

#include <deque>

template<class T, class Comp>
std::deque<T> Merge(const std::deque<T>& half1, const std::deque<T>& half2, const Comp& comparator) {
    std::deque<T> result;

    size_t index1 = 0;
    size_t index2 = 0;
    while (index1 < half1.size() && index2 < half2.size()) {
        if (comparator(half2[index2], half1[index1])) {
            result.push_back(half2[index2]);
            ++index2;
        } else {
            result.push_back(half1[index1]);
            ++index1;
        }
    }

    result.insert(result.end(), half1.begin() + index1, half1.end());
    result.insert(result.end(), half2.begin() + index2, half2.end());
    return result;
}

template<class T, class Comp>
std::deque<T> MergeSort(const std::deque<T>& src, const Comp& comparator) {
    if (src.size() <= 1) {
        return src;
    }

    const size_t middle = src.size() / 2;
    const std::deque<T> left_sorted = MergeSort(std::deque<T>(src.begin(), src.begin() + middle), comparator);
    const std::deque<T> right_sorted = MergeSort(std::deque<T>(src.begin() + middle, src.end()), comparator);

    return Merge(left_sorted, right_sorted, comparator);
}
