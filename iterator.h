#if !defined(MYSTL_ITERATOR_H_)
#define MYSTL_ITERATOR_H_

#include <cstddef>

namespace mystl{
// 分别表示5种迭代器 category 的 struct
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

// 迭代器模板
template<typename Category, typename T, typename Distance = ptrdiff_t,
         typename Pointer = T*, typename Reference = T&>
struct iterator{
    using iterator_category = Category;
    using value_type        = T;
    using difference_type   = Distance;
    using pointer           = Pointer;
    using reference         = Reference;
};

// 迭代器的 traits
template <typename Iterator>
struct iterator_traits{
    using value_type        = typename Iterator::value_type;
    using iterator_category = typename Iterator::iterator_category;
    using difference_type   = typename Iterator::difference_type;
    using pointer           = typename Iterator::pointer;
    using reference         = typename Iterator::reference;
};

// 偏特化版本
template <typename T>
struct iterator_traits<T*>{
    using value_type        = T;
    using iterator_category = random_access_iterator_tag;
    using difference_type   = ptrdiff_t;
    using pointer           = T*;
    using reference         = T&;
};

template <typename T>
struct iterator_traits<const T*>{
    using value_type        = T;
    using iterator_category = random_access_iterator_tag;
    using difference_type   = ptrdiff_t;
    using pointer           = const T*;
    using reference         = const T&;
};

template <typename Iterator>
inline typename iterator_traits<Iterator>::iterator_category iterator_category(const Iterator&) {
    using category = typename iterator_traits<Iterator>::iterator_category;
    return category();
}

template <typename Iterator>
inline typename iterator_traits<Iterator>::difference_type* difference_type(const Iterator&) {
    return static_cast<typename iterator_traits<Iterator>::difference*>(0);
}

template <typename Iterator>
inline typename iterator_traits<Iterator>::value_type* value_type(const Iterator&) {
    return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

template <typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
distance_aux(InputIterator first, InputIterator last, input_iterator_tag) {
    typename iterator_traits<InputIterator>::difference_type n = 0;
    for (; first != last; ++first, ++n);
    return n;
}

template <typename RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type
distance_aux(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag) {
    return last - first;
}

template <typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last) {
    return distance_aux(first, last, iterator_category(first));
}

template <typename InputIterator, typename Distance>
inline void distance_aux(InputIterator first, InputIterator last,
                         Distance& n, input_iterator_tag) {
    for(; first != last; ++first, ++n);
}

template <typename RandomAccessIterator, typename Distance>
inline void distance_aux(RandomAccessIterator first, RandomAccessIterator last, Distance& n, random_access_iterator_tag) {
    n += last - first;
}

template <typename InputIterator, typename Distance>
inline void distance(InputIterator first, InputIterator last, Distance& n) {
    distance_aux(first, last, n, iterator_category(first));
}

template <typename InputIterator, typename Distance>
inline void advance_aux(InputIterator iter,const Distance& n, input_iterator_tag) {
    for(; n != 0; --n, ++iter);
}

template <typename BidirectionIterator, typename Distance>
inline void advance_aux(BidirectionIterator iter, const Distance& n, bidirectional_iterator_tag) {
    if (n >= 0)
        for(; n != 0; --n, ++iter);
    else
        for(; n != 0; ++n, --iter);
}

template <typename RandomAccessIterator, typename Distance>
inline void advance_aux(RandomAccessIterator iter, const Distance& n, random_access_iterator_tag) {
    iter += n;
}

template <typename InputIterator, typename Distance>
inline void advance(InputIterator iter, const Distance& n) {
    advance_aux(iter, n, iterator_category(iter));
}

// 反向迭代器的实现
template <typename RandomAccessIterator, typename T,
          typename Reference = T&,
          typename Distance = ptrdiff_t>
class reverse_iterator{
protected:
    using self = reverse_iterator<RandomAccessIterator, T>;
    RandomAccessIterator current;
public:
    using iterator_category = random_access_iterator_tag;
    using value_type        = T;
    using difference_type   = Distance;
    using pointer           = T*;
    using reference         = Reference;
    reverse_iterator() {}
    explicit reverse_iterator(RandomAccessIterator iter) : current(iter) {}
    RandomAccessIterator base() const { return current; }
    // 各种操作符重载
    Reference operator*() const { return *(current - 1); }
    pointer operator->() { return &(operator*()); }
    self& operator++() {
        --current;
        return *this;
    }
    self operator++(int) {
        self tmp = *this;
        --current;
        return tmp;
    }
    self& operator--() {
        ++current;
        return *this;
    }
    self operator--(int) {
        self tmp = *this;
        ++current;
        return *this;
    }
    self operator+(Distance n) {
        return self(current - n);
    }
    self& operator+=(Distance n) {
        current -= n;
        return *this;
    }
    self operator-(Distance n) {
        return self(current + n);
    }
    self& operator-=(Distance n) {
        current += n;
        return *this;
    }
    Reference operator[](Distance n) const { return *(*this); }
}; // end reverse_iterator

template <typename RandomAccessIterator,
          typename T,
          typename Reference,
          typename Distance>
inline bool operator==(reverse_iterator<RandomAccessIterator, T, Reference, Distance>& lhs,
                       reverse_iterator<RandomAccessIterator, T, Reference, Distance>& rhs) {
    return lhs.base() == rhs.base();
}

template <typename RandomAccessIterator,
          typename T,
          typename Reference,
          typename Distance>
inline bool operator<(reverse_iterator<RandomAccessIterator, T, Reference, Distance>& lhs,
                      reverse_iterator<RandomAccessIterator, T, Reference, Distance>& rhs) {
    return lhs.base() < rhs.base();
}

template <typename RandomAccessIterator,
          typename T,
          typename Reference,
          typename Distance>
inline Distance operator-(reverse_iterator<RandomAccessIterator, T, Reference, Distance>& lhs,
                          reverse_iterator<RandomAccessIterator, T, Reference, Distance>& rhs) {
    return lhs.base() - rhs.base();
}

template <typename RandomAccessIterator,
          typename T,
          typename Reference,
          typename Distance>
inline reverse_iterator<RandomAccessIterator, T, Reference, Distance> operator+(Distance n,
                      reverse_iterator<RandomAccessIterator, T, Reference, Distance>& iter) {
    return reverse_iterator<RandomAccessIterator, T, Reference, Distance>(iter.base() - n);
}
} // namespace mystl

#endif // MYSTL_ITERATOR_H_
