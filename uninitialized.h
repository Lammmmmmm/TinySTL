#if !defined(MYSTL_UNINITIALIZED_H_)
#define MYSTL_UNINITIALIZED_H_

#include <memory>
#include "iterator.h"
#include "construct.h"
#include "type_traits.h"

namespace mystl {
template <typename InputIterator, typename ForwardIterator>
inline ForwardIterator uninitialized_copy_aux(InputIterator first,
                                              InputIterator last,
                                              ForwardIterator result,
                                              _true_type) {
    return std::copy(first, last, result);
}

template <typename InputIterator, typename ForwardIterator>
inline ForwardIterator uninitialized_copy_aux(InputIterator first,
                                              InputIterator last,
                                              ForwardIterator result,
                                              _false_type) {
    ForwardIterator cur = result;
    try {
        for (; first != last; ++first, ++cur)
            construct(cur, *first);
        return cur;
    } catch (...) {
        destroy(result, cur);
        throw;
    }
}

template <typename InputIterator, typename ForwardIterator>
inline ForwardIterator uninitialized_copy(InputIterator first,
                                          InputIterator last,
                                          ForwardIterator result) {
    using value_type =
        typename mystl::iterator_traits<ForwardIterator>::value_type;
    using is_POD = typename type_traits<value_type>::is_POD_type;
    return uninitialized_copy_aux(first, last, result, is_POD());
}

template <typename ForwardIterator, typename T>
inline void uninitialized_fill_aux(ForwardIterator first,
                                              ForwardIterator last,
                                              const T& value,
                                              _true_type) {
    std::fill(first, last, value);
}

template <typename ForwardIterator, typename T>
inline void uninitialized_fill_aux(ForwardIterator first,
                                              ForwardIterator last,
                                              const T& value,
                                              _false_type) {
    ForwardIterator cur = first;
    try {
        for (; cur != last; ++cur)
            construct(cur, value);
        return cur;
    } catch (...) {
        destroy(first, cur);
        throw;
    }
}

template <typename ForwardIterator, typename T>
inline void uninitialized_fill(ForwardIterator first,
                                          ForwardIterator last,
                                          const T& value) {
    using value_type =
        typename mystl::iterator_traits<ForwardIterator>::value_type;
    using is_POD = typename type_traits<value_type>::is_POD_type;
    uninitialized_fill_aux(first, last, value, is_POD());
}

template <typename ForwardIterator, typename T>
inline ForwardIterator uninitialized_fill_n_aux(ForwardIterator first,
                                                size_t n,
                                                const T& value,
                                                _true_type) {
    return std::fill_n(first, n, value);
}

template <typename ForwardIterator, typename T>
inline ForwardIterator uninitialized_fill_n_aux(ForwardIterator first,
                                                size_t n,
                                                const T& value,
                                                _false_type) {
    ForwardIterator cur = first;
    try {
        for (; n != 0; --n, ++cur)
            construct(cur, value);
        return cur;
    } catch (...) {
        destroy(first, cur);
        throw;
    }
}

template <typename ForwardIterator, typename T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first,
                                            size_t n,
                                            const T& value) {
    using value_type =
        typename mystl::iterator_traits<ForwardIterator>::value_type;
    using is_POD = typename type_traits<value_type>::is_POD_type;
    return uninitialized_fill_n_aux(first, n, value, is_POD());
}
}  // namespace mystl

#endif  // MYSTL_UNINITIALIZED_H_
