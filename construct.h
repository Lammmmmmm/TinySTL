#if !defined(MYSTL_CONSTRUCT_H)
#define MYSTL_CONSTRUCT_H

#include <new>  // placement new 在此头文件内
#include "type_traits.h"
#include "iterator.h"
namespace mystl {
// 全局 construct 函数，使用 placement new 将一个值放入指针所指内存
// 使用了两个模板参数，并使用显式转换
template <typename T1, typename T2>
inline void construct(T1* ptr, const T2& value) {
    new (ptr) T1(value);
}

template <typename T1, typename T2>
inline void construct(T1* ptr, T2&& value) {
    new (ptr) T1(value);
}

template <typename T>
inline void construct(T* ptr) {
    new (ptr) T();
}
// 单个参数的全局 destroy 函数，直接调用对象的析构函数
template <typename T>
inline void destroy(T* ptr) {
    ptr->~T();
}

template <typename ForwardIterator>
inline void destroy_aux(ForwardIterator first,
                        ForwardIterator last,
                        _true_type) {}

template <typename ForwardIterator>
inline void destroy_aux(ForwardIterator first,
                        ForwardIterator last,
                        _false_type) {
    for (; first != last; ++first)
        destroy(first);
}

// 两个参数的全局 destroy 函数，根据其是否具有 trivial 析构函数进行重载
template <typename ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last) {
    using value_type =
        typename mystl::iterator_traits<ForwardIterator>::value_type;
    using is_trivial_dtor =
        typename type_traits<value_type>::has_trivial_destructor;
    destroy_aux(first, last, is_trivial_dtor());
}

}  // namespace mystl

#endif  // MYSTL_CONSTRUCT_H
