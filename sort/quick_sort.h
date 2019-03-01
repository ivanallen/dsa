#pragma once

#include <vector>

template <typename T>
int partition(std::vector<T>& arr, int l, int r) {
    // 5 9 2 3 4 6 7 8 1
    // p i             j
    // 5 1 2 3 4 6 7 8 9
    // p       j i
    //
    // 4 1 2 3 5 6 7 8 9
    //         j i
    T& pivot = arr[l];
    int i = l + 1;
    int j = r;

    // arr 中的元素，一定要和 pivot 进行比较
    // 因此 i <= j 而不是 i < j
    while(i <= j) {
        // 下面两个比较，一定要是互补的，否会死循环
        while(arr[i] <= pivot) ++i;
        while(pivot < arr[j]) --j;
        if (i < j) std::swap(arr[i], arr[j]);
    }
    std::swap(arr[l], arr[j]);
    return j;
}

template <typename T>
void quick_sort_helper(std::vector<T>& arr, int l, int r) {
    if (l >= r) return;
    int pivot = partition(arr, l, r);
    quick_sort_helper(arr, l, pivot - 1);
    quick_sort_helper(arr, pivot + 1, r);
}

template <typename T>
void quick_sort(std::vector<T>& arr) {
    quick_sort_helper(arr, 0, arr.size() - 1);
}
