#if !defined(MYSTL_LIST_H_)
#define MYSTL_LIST_H_

#include <initializer_list>
#include "iterator.h"
#include "memory.h"

namespace mystl {
template <typename T>
struct list_node {
    list_node<T>* next;
    list_node<T>* prev;
    T data;
};

template <typename T, typename Ref, typename Ptr>
struct list_iterator {
    using iterator = list_iterator<T, T&, T*>;
    using const_iterator = list_iterator<T, const T&, const T*>;
    using self = list_iterator<T, Ref, Ptr>;

    using iterator_category = bidirectional_iterator_tag;
    using value_type = T;
    using pointer = Ptr;
    using reference = Ref;
    using difference_type = ptrdiff_t;
    using size_type = size_t;

    using link_type = list_node<T>*;

    link_type node;

    list_iterator(link_type x) : node(x) {}
    list_iterator() {}
    list_iterator(const iterator& x) : node(x.node) {}

    bool operator==(const self& rhs) const { return node == rhs.node; }
    bool operator!=(const self& rhs) const { return !(node == rhs.node); }
    reference operator*() { return (*node).data; }
    pointer operator->() const { return &(operator*()); }
    self& operator++() {
        node = (*node).next;
        return *this;
    }
    self operator++(int) {
        self tmp = *this;
        ++*this;
        return tmp;
    }
    self& operator--() {
        node = (*node).prev;
        return *this;
    }
    self operator--(int) {
        self tmp = *this;
        --*this;
        return tmp;
    }
    self operator+(difference_type n) const {
        self tmp = *this;
        if(n > 0)
            for(; n != 0; --n, ++tmp);
        else
            for(; n != 0; ++n, --tmp);
        return tmp;
    }

    self operator-(difference_type n) const {
        self tmp = *this;
        difference_type i = -n;
        return (tmp + i);
    }
};  // end struct list_iterator

template <typename T, typename Alloc = mystl::alloc<list_node<T>>>
class list {
   public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    using link_type = list_node<T>*;
    using iterator = list_iterator<T, T&, T*>;
    using const_iterator = list_iterator<T, const T&, const T*>;
    using reverse_iter = reverse_iterator<iterator, T>;
    using const_reverse_iter = reverse_iterator<const_iterator, T, const T&, const T*>;

   protected:
    link_type node;
    /* 内部辅助函数 */
    link_type get_node() { return Alloc::allocate(); }
    void put_node(link_type ptr) { Alloc::deallocate(ptr); }
    link_type create_node(const T& value);
    void destroy_node(link_type ptr);
    void empty_init();
    void fill_init(size_type n, const T& value);
    template <typename InputIterator>
    void range_init(InputIterator first, InputIterator last);
    void transfer(iterator pos, iterator first, iterator last);

   public:
    /* 各种构造拷贝析构函数 */
    list() { empty_init(); }
    list(size_type n, const T& value) { fill_init(n, value); }
    list(int n, const T& value) { fill_init(size_type(n), value); }
    list(long n, const T& value) { fill_init(size_type(n), value); }
    explicit list(size_type n) { fill_init(n, T()); }
    template <typename InputIterator>
    list(InputIterator first, InputIterator last) {
        range_init(first, last);
    }
    list(const list<T, Alloc>& rhs) { range_init(rhs.begin(), rhs.end()); }
    list(std::initializer_list<T> rhs) { range_init(rhs.begin(), rhs.end()); }
    ~list() {
        clear();
        put_node(node);
    }
    list<T, Alloc>& operator=(const list<T, Alloc>& rhs);
    list<T, Alloc>& operator=(std::initializer_list<T> rhs);

    /* 迭代器相关操作 */
    iterator begin() noexcept { return node->next; }
    const_iterator begin() const noexcept { return node->next; }
    const_iterator cbegin() const noexcept { return begin(); }
    iterator end() noexcept { return node; }
    const_iterator end() const noexcept { return node; }
    const_iterator cend() const noexcept { return end(); }

    reverse_iter rbegin() noexcept { return reverse_iter(end()); }
    const_reverse_iter rbegin() const noexcept {
        return const_reverse_iter(end());
    }
    reverse_iter rend() noexcept { return reverse_iter(begin()); }
    const_reverse_iter rend() const noexcept {
        return const_reverse_iter(begin());
    }
    /* 容量相关操作 */
    bool empty() const noexcept { return node->next == node; }
    size_type size() const noexcept { return distance(begin(), end()); }
    size_type max_size() const noexcept { return size_type(-1); }

    /* 取值相关操作 */
    reference front() { return node->next->data; }
    const_reference front() const { return node->next->data; }
    reference back() { return node->prev->data; }
    const_reference back() const { return node->prev->data; }
    reference operator[](const size_type& n);

    /* 修改链表操作 */
    void swap(list<T, Alloc>& rhs) { std::swap(node, rhs.node); }
    iterator insert(iterator pos, const T& value);
    iterator insert(iterator pos);
    template <typename InputIterator>
    void insert(iterator pos, InputIterator first, InputIterator last);
    void insert(iterator pos, size_type n, const T& value);
    void insert(iterator pos, int n, const T& value);
    void insert(iterator pos, long n, const T& value);
    void push_front(const T& value) { insert(begin(), value); }
    void push_back(const T& value) { insert(end(), value); }
    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);
    void resize(size_type new_size, const T& value);
    void resize(size_type new_size) { resize(new_size, T()); }
    void clear() { erase(begin(), end()); }
    void pop_front() { erase(begin()); }
    void pop_back() { erase(--end()); }
    void splice(iterator pos, list& x);
    void splice(iterator pos, list&, iterator i);
    void splice(iterator pos, list&, iterator first, iterator last);
    void remove(const T& value);
    void unique();
    void merge(list& x);
    void reverse();
    void sort();

    /* 比较操作符重载 */

    // friend bool operator==(const list& lhs, const list& rhs);
    // friend bool operator<(const list& lhs, const list& rhs);
};  // class list

/* list 类的辅助函数实现 */

template <typename T, typename Alloc>
typename list<T, Alloc>::link_type list<T, Alloc>::create_node(const T& value) {
    link_type ptr = get_node();
    try {
        construct(&ptr->data, value);
    } catch (...) {
        put_node(ptr);
    }
    return ptr;
}

template <typename T, typename Alloc>
void list<T, Alloc>::destroy_node(link_type ptr) {
    destroy(&ptr->data);
    put_node(ptr);
}

template <typename T, typename Alloc>
void list<T, Alloc>::empty_init() {
    node = get_node();
    node->next = node;
    node->prev = node;
}

template <typename T, typename Alloc>
void list<T, Alloc>::fill_init(size_type n, const T& value) {
    empty_init();
    try {
        insert(begin(), n, value);
    } catch (...) {
        clear();
        put_node(node);
    }
}

template <typename T, typename Alloc>
template <typename InputIterator>
void list<T, Alloc>::range_init(InputIterator first, InputIterator last) {
    empty_init();
    try {
        insert(begin(), first, last);
    } catch (...) {
        clear();
        put_node(node);
    }
}

template <typename T, typename Alloc>
void list<T, Alloc>::transfer(iterator pos, iterator first, iterator last) {
    if (pos != last) {
        last.node->prev->next = pos.node;
        first.node->prev->next = last.node;
        pos.node->prev->next = first.node;
        link_type tmp = pos.node->prev;
        pos.node->prev = last.node->prev;
        last.node->prev = first.node->prev;
        first.node->prev = tmp;
    }
}

/* list 类公开成员函数的实现 */

template <typename T, typename Alloc>
list<T, Alloc>& list<T, Alloc>::operator=(const list<T, Alloc>& rhs) {
    if (&rhs != this) {
        iterator first1 = begin();
        iterator last1 = end();
        const_iterator first2 = rhs.cbegin();
        const_iterator last2 = rhs.cend();
        for (; first1 != last1 && first2 != last2; ++first1, ++first1)
            *first1 = *first2;
        if (first1 == last1)
            insert(last1, first2, last2);
        else
            erase(first1, last1);
    }
}

template <typename T, typename Alloc>
list<T, Alloc>& list<T, Alloc>::operator=(std::initializer_list<T> rhs) {
    iterator first1 = begin();
    iterator last1 = end();
    auto first2 = rhs.begin();
    auto last2 = rhs.end();
    for (; first1 != last1 && first2 != last2; ++first1, first2)
        *first1 = *first2;
    if (first1 == last1)
        insert(last1, first2, last2);
    else
        erase(first1, last1);
}

template <typename T, typename Alloc>
typename list<T, Alloc>::reference list<T, Alloc>::operator[](const size_type& n) {
    return *(begin() + n);
}

template <typename T, typename Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::insert(iterator pos,
                                                         const T& value) {
    link_type tmp = create_node(value);
    pos.node->prev->next = tmp;
    tmp->prev = pos.node->prev;
    pos.node->prev = tmp;
    tmp->next = pos.node;
}

template <typename T, typename Alloc>
template <typename InputIterator>
void list<T, Alloc>::insert(iterator pos,
                            InputIterator first,
                            InputIterator last) {
    for(; first != last; ++first)
        insert(pos, *first);
}

template <typename T, typename Alloc>
void list<T, Alloc>::insert(iterator pos, size_type n, const T& value) {
    for(; n > 0; --n)
        insert(pos, value);
}

template <typename T, typename Alloc>
void list<T, Alloc>::insert(iterator pos, int n, const T& value) {
    insert(pos, size_type(n), value);
}

template <typename T, typename Alloc>
void list<T, Alloc>::insert(iterator pos, long n, const T& value) {
    insert(pos, size_type(n), value);
}

template <typename T, typename Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::erase(iterator pos) {
    iterator tmp = pos;
    ++tmp;
    pos.node->prev->next = tmp.node;
    tmp.node->prev = pos.node->prev;
    destroy_node(pos.node);
    return tmp;
}

template <typename T, typename Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::erase(iterator first,
                                                        iterator last) {
    while(first != last)
        erase(first++);
    return last;
}

template <typename T, typename Alloc>
void list<T, Alloc>::resize(size_type new_size, const T& value) {
    size_type old_size = size();
    if (old_size < new_size) {
        insert(end(), new_size - old_size, value);
    } else {
        iterator new_end = end();
        for (; old_size != new_size; --old_size, --new_end)
            ;
        erase(new_end, end());
    }
}

template <typename T, typename Alloc>
void list<T, Alloc>::splice(iterator pos, list<T, Alloc>& x) {
    if (x.empty())
        return;
    transfer(pos, x.begin(), x.end());
}

template <typename T, typename Alloc>
void list<T, Alloc>::splice(iterator pos, list&, iterator i) {
    iterator j = i;
    ++j;
    if (pos == i || pos == j)
        return;
    transfer(pos, i, j);
}

template <typename T, typename Alloc>
void list<T, Alloc>::splice(iterator pos,
                            list&,
                            iterator first,
                            iterator last) {
    if (first == last)
        return;
    transfer(pos, first, last);
}

template <typename T, typename Alloc>
void list<T, Alloc>::remove(const T& value) {
    iterator first = begin();
    iterator last = end();
    for (; first != last; ++first)
        if (*first == value)
            erase(first);
}

template <typename T, typename Alloc>
void list<T, Alloc>::unique() {
    if (empty())
        return;
    iterator first = begin();
    iterator last = end();
    iterator next = first;
    while (++next != last) {
        if (*first == *next)
            first = erase(first);
        else
            first = next;
    }
}

template <typename T, typename Alloc>
void list<T, Alloc>::merge(list<T, Alloc>& x) {
    if (x.empty())
        return;
    iterator first1 = begin();
    iterator last1 = end();
    iterator first2 = x.begin();
    iterator last2 = x.end();
    while (first1 != last1 && first2 != last2) {
        if (*first2 < *first1) {
            splice(first1, x, first2);
            first2 = x.begin();
        } else
            ++first1;
    }
    if (first1 == last1)
        splice(last1, x, first2, last2);
}

template <typename T, typename Alloc>
void list<T, Alloc>::reverse() {
    if (node->next == node || node->next->next == node)
        return;
    iterator first = begin();
    iterator last = end();
    ++first;
    while (first != last) {
        iterator old = first;
        ++first;
        transfer(begin(), old, first);
    }
}

template <typename T, typename Alloc>
void list<T, Alloc>::sort() {
    if (node->next == node || node->next->next == node)
        return;
    list<T, Alloc> carry;
    list<T, Alloc> counter[64];
    int fill = 0;
    while (!empty()) {
        carry.splice(carry.begin(), *this, begin());
        int i = 0;
        while (i < fill && !counter[i].empty()) {
            counter[i].merge(carry);
            carry.swap(counter[i++]);
        }
        carry.swap(counter[i]);
        if (i == fill)
            ++fill;
    }
    for (int i = 1; i < fill; ++i)
        counter[i].merge(counter[i - 1]);
    swap(counter[fill - 1]);
}

template <typename T, class Alloc>
inline bool operator==(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
    auto end1 = lhs.node;
    auto end2 = rhs.node;
    auto first1 = end1->next;
    auto first2 = end2->next;
    for (; first1 != end1 && first2 != end2;
         first1 = first1->next, first2 = first2->next)
        if (first1->data != first2->data)
            return false;
    return first1 == end1 && first2 == end2;
}

template <typename T, typename Alloc>
inline bool operator<(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
    auto end1 = lhs.node;
    auto end2 = rhs.node;
    auto first1 = end1->next;
    auto first2 = end2->next;
    for (; first1 != end1 && first2 != end2;
         first1 = first1->next, first2 = first2->next) {
        if (first1->data < first2->date)
            return true;
        else if (first2->data < first1->data)
            return false;
    }
    return first1 != end1 && first2 == end2;
}

}  // namespace mystl

#endif  // MYSTL_LIST_H_
