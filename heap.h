#if !defined(MYSTL_HEAP_H_)
#define MYSTL_HEAP_H_

/* 本文件定义了一组 heap 算法，主要用于实现 priority_queue */

#include "iterator.h"

namespace mystl {

template <typename T, typename RandomAccessIterator, typename Distance>
void push_heap_aux(RandomAccessIterator first,
                   Distance holeIndex, 
                   Distance topIndex,
                   const T& value) {
    Distance parent = (holeIndex - 1) / 2;
    while(holeIndex > topIndex && *(first + parent) < value) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
}

// 在调用 push_heap 应保证新加入的元素在 (last - 1) 上
template <typename RandomAccessIterator>
void push_heap(RandomAccessIterator first,
               RandomAccessIterator last) {
    auto value = *(last - 1);
    auto holeIndex = (last - first) - 1;
    decltype(holeIndex) topIndex = 0;
    push_heap_aux(first, holeIndex, topIndex, value);    
}

template <typename T, typename RandomAccessIterator, typename Distance>
void adjust_heap(RandomAccessIterator first,
                 Distance holeIndex,
                 Distance len,
                 T value) {
    Distance topIndex = holeIndex;
    Distance secondChild = 2 * (holeIndex + 1);
    while(secondChild < len) {
        if (*(first + secondChild) < *(first + secondChild - 1))
            --secondChild;
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (holeIndex + 1);
    }
    if (secondChild == len) {
        *(first + holeIndex) = *(first + (secondChild - 1));
        holeIndex = secondChild - 1;
    }
    push_heap_aux(first, holeIndex, topIndex, value);
}

// 使用 pop_head 后最大值被设置到容器尾部，heap 长度-1
template <typename RandomAccessIterator>
void pop_heap(RandomAccessIterator first,
              RandomAccessIterator last) {
    auto value = *(last - 1);
    *(last - 1) = *first;
    auto len = last - first - 1;
    decltype(len) holeIndex = 0;
    adjust_heap(first, holeIndex, len, value);
}

// 将一个 heap 变成递增序列
// 在使用sort_heap 前应保证范围[first， last) 内为一个heap
template <typename RandomAccessIterator>
void sort_heap(RandomAccessIterator first,
               RandomAccessIterator last) {
    for(; last - first > 1; --last)
        pop_heap(first, last);
}

// 将范围[first, last)转换为一个heap
template <typename RandomAccessIterator>
void make_heap(RandomAccessIterator first,
               RandomAccessIterator last) {
    auto len = last - first;
    decltype(len) holeIndex = (len - 2) / 2;
    while (true) {
        adjust_heap(first, holeIndex, len, *(first + holeIndex));
        if (holeIndex == 0) return;
        --holeIndex;
    }
}

} // namespace mystl

#endif // MYSTL_HEAP_H_
