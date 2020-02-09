#if !defined(MYSTL_VECTOR_H_)
#define MYSTL_VECTOR_H_

// 该头文件用以实现 vector

#include <initializer_list>
#include "memory.h"
namespace mystl
{
template <typename T, typename Allocator = alloc<T>>
class vector {
public:
    using value_type        = T;
    using pointer           = T*;
    using const_pointer     = const T*;
    using iterator          = T*;
    using const_iterator    = const T*;
    using reference         = T&;
    using const_reference   = const T&;
    using size_type         = size_t;
    using difference_type   = ptrdiff_t;

    using reverse_iter       = reverse_iterator<iterator, T>;
    using const_reverse_iter = reverse_iterator<const_iterator, T, const_reference, difference_type>;
protected:
    iterator start;
    iterator finish;
    iterator end_of_storage;

    void insert_aux(iterator position, const T& value);
    void deallocate() {
        if (start) Allocator::deallocate(start, end_of_storage - start);
    }
    void fill_init(size_type n, const T& value);
    template <typename InputIterator>
    void copy_init(InputIterator first, InputIterator last);
public:
    // 构造与析构函数
    vector() : start(0), finish(0), end_of_storage(0) { }
    vector(size_type n, const T& value) { fill_init(n, value); }
    vector(int n, const T& value) { fill_init(n, value); }
    vector(long n, const T& value) { fill_init(n, value); }
    explicit vector(size_type n) { fill_init(n, T()); }
    vector(const vector<T>& vec) { copy_init(vec.begin(), vec.end()); }
    template <typename InputIterator>
    vector(InputIterator first, InputIterator last) {
        copy_init(first, last);
    }
    vector(std::initializer_list<T> rhs) {
        copy_init(rhs.begin(), rhs.end());
    }
    vector<T, Allocator>& operator=(const vector<T, Allocator>& vec);
    vector<T, Allocator>& operator=(std::initializer_list<T> rhs);
    ~vector() {
        destroy(start, finish);
        deallocate();
    }
    // 迭代器相关操作
    iterator begin() noexcept { return start; }
    const_iterator begin() const noexcept{ return start; }
    iterator end() noexcept { return finish; }
    const_iterator end() const noexcept { return finish; }

    reverse_iter rbegin() { return reverse_iter(end()); }
    const_reverse_iter rbegin() const noexcept { return const_reverse_iter(end()); }
    reverse_iter rend() noexcept { return reverse_iter(begin());}
    const_reverse_iter rend() const noexcept { return const_reverse_iter(begin()); }

    const_iterator cbegin() const noexcept { return begin(); }
    const_iterator cend() const noexcept { return end(); }
    const_reverse_iter crbegin() const noexcept { return rbegin(); }
    const_reverse_iter crend() const noexcept { return rend(); }

    // 容量相关操作
    size_type size() const { return finish - start; }
    size_type max_size() const { return size_type(-1) / sizeof(T); }
    size_type capacity() const { return end_of_storage - start; }
    bool empty() const { return start == finish; }
    void reserve(size_type n);

    // 访问相关操作
    reference front() { return *start; }
    const_reference front() const { return *start; }
    reference back() { return *(finish - 1); }
    const_reference back() const { return *(finish - 1); }
    reference operator[](size_type n) { return *(start + n); }
    const_reference operator[](size_type n) const { return *(start + n); }

    // 修改容器的操作
    void push_back(const T& value);
    void pop_back() { --finish; destroy(finish); }
    void swap(vector<T, Allocator>& rhs);
    iterator insert(iterator position, const T& value);
    iterator insert(iterator position) { return insert(position, T());}
    void insert(iterator position, size_type n, const T& value);
    iterator erase(iterator position);
    iterator erase(iterator first, iterator last);
    void resize(size_type new_size, const T& value);
    void resize(size_type new_size) { resize(new_size, T()); }
    void clear() { erase(start, finish); }
}; // end vector class

/* vector 内部辅助函数的实现 */
template <typename T, typename Alloc>
void vector<T, Alloc>::fill_init(size_type n, const T& value) {
    start = Alloc::allocate(n);
    try {
        uninitialized_fill_n(start, n, value);
        finish = start + n;
        end_of_storage = finish;
    }
    catch (...) {
        Alloc::deallocate(start, n);
    }
}

template <typename T, typename Alloc>
template <typename InputIterator>
void vector<T, Alloc>::copy_init(InputIterator first, InputIterator last) {
    size_type n = last - first;
    start = Alloc::allocate(n);
    try {
        uninitialized_copy(first, last, start);
        finish = start + n;
        end_of_storage = finish;
    }
    catch (...) {
        Alloc::deallocate(start, n);
        throw;
    }
}

template <typename T, typename Alloc>
void vector<T, Alloc>::insert_aux(iterator position, const T& value) {
    if (finish != end_of_storage) {
        construct(finish, *(finish - 1));
        ++finish;
        std::copy_backward(position, finish - 2, finish - 1);
        *position = value;
    }
    else {
        const size_type old_size = size();
        const size_type new_size = old_size ? old_size * 2 : 1;
        iterator new_start = Alloc::allocate(new_size);
        iterator new_finish = new_start;
        try {
            new_finish = uninitialized_copy(start, position, new_start);
            construct(new_finish++, value);
            new_finish = uninitialized_copy(position, finish, new_finish);
        }
        catch(...) {
            destroy(new_start, new_finish);
            Alloc::deallocate(new_start, new_size);
            throw;
        }
        destroy(start, finish);
        deallocate();
        start = new_start;
        finish = new_finish;
        end_of_storage = start + new_size;
    }
}

/* vector 其余接口实现 */
// 等号操作符的重载
template <typename T, typename Alloc>
vector<T, Alloc>& vector<T, Alloc>::operator=(const vector<T, Alloc>& vec) {
    if (&vec != this){
        size_type new_size = vec.size();
        if (new_size > capacity()) {
            iterator new_start = Alloc::allocate(new_size);
            end_of_storage = uninitialized_copy(vec.begin(), vec.end(), new_start);
            destroy(start, finish);
            deallocate();
            start = new_start;
        }
        else if(new_size < size()) {
            iterator iter = std::copy(vec.begin(), vec.end(), start);
            destroy(iter, finish);
        }
        else {
            std::copy(vec.begin(), vec.begin() + size(), start);
            uninitialized_copy(vec.begin() + size(), vec.end(), finish);
        }
        finish = start + new_size;
    }
    return *this;
}

template <typename T, typename Alloc>
vector<T, Alloc>& vector<T, Alloc>::operator=(std::initializer_list<T> rhs) {
    vector<T, Alloc> tmp(rhs.begin(), rhs.end());
    swap(tmp);
    return *this;
}

template <typename T, typename Alloc>
void vector<T, Alloc>::push_back(const T& value) {
    if (finish != end_of_storage)
        construct(finish++, value);
    else
        insert_aux(finish, value);
}

template <typename T, typename Alloc>
void vector<T, Alloc>::swap(vector<T, Alloc>& rhs){
    std::swap(start, rhs.start);
    std::swap(finish, rhs.finish);
    std::swap(end_of_storage, rhs.end_of_storage);
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator pos, const T& value) {
    size_type n = pos - start;
    if (finish != end_of_storage && pos == finish)
        construct(finish++, value);
    else insert_aux(pos, value);
    return start + n;
}

template<typename T, typename Alloc>
void vector<T, Alloc>::insert(iterator pos, size_type n, const T& value) {
    if (n == 0) return;
    if (size() + n < capacity()) {
        const size_type elems_after = finish - pos;
        if (elems_after > n) {
            uninitialized_copy(finish - n, finish, finish);
            std::copy_backward(pos, finish - n, finish);
            std::fill(pos, pos + n, value);
        }
        else {
            uninitialized_fill_n(finish, n - elems_after, value);
            uninitialized_copy(pos, finish, pos + n);
            std::fill(pos, finish, value);
        }
        finish += n;
    }
    else{
        const size_type old_size = size();
        const size_type new_size = old_size + old_size > n ? old_size : n;
        iterator new_start = Alloc::allocate(new_size);
        iterator new_finish = new_start;
        try {
            new_finish = uninitialized_copy(start, pos, new_start);
            new_finish = uninitialized_fill_n(new_finish, n, value);
            new_finish = uninitialized_copy(pos, finish, new_finish);
        }
        catch(...) {
            destroy(new_start, new_finish);
            Alloc::deallocate(new_start, new_size);
            throw;
        }
        destroy(start, finish);
        deallocate();
        start = new_start;
        finish = new_finish;
        end_of_storage = new_start + new_size;
    }
}

template <typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator pos) {
    if (pos != (finish - 1))
        std::copy(pos + 1, finish, pos);
    destroy(finish - 1);
    --finish;
    return pos;
}

template <typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator first, iterator last) {
    iterator new_finish = std::copy(last, finish, first);
    destroy(new_finish, finish);
    finish = new_finish;
    return first;
}

template <typename T, typename Alloc>
void vector<T, Alloc>::resize(size_type new_size, const T& value){
    if (new_size < size())
        erase(start + new_size, finish);
    else
        insert(finish, new_size - size(), value);
}

template <typename T, typename Alloc>
void vector<T, Alloc>::reserve(size_type n) {
    if (capacity() < n) {
        iterator new_start = Alloc::allocate(n);
        iterator new_finish = new_start;
        try{
        new_finish = uninitialized_copy(start, finish, new_start);
        }
        catch(...) {
            destroy(new_start, new_finish);
            Alloc::deallocate(new_start, n);
        }
        destroy(start, finish);
        deallocate();
        start = new_start;
        finish = new_finish;
        end_of_storage = start + n;
    }
}

template <typename T, typename Alloc>
inline bool operator==(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
    return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename T, typename Alloc>
inline bool operator<(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
    typename vector<T, Alloc>::iterator first1 = lhs.begin();
    auto last1 = lhs.end();
    auto first2 = rhs.begin();
    auto last2 = rhs.end();
    for(; first1 != last1 && first2 != last2; ++first1, ++first2) {
        if (*first1 < *first2) return true;
        if (*first2 < *first1) return false;
    }
    return first1 == last1 && first2 != last2;
}

template <typename T, typename Alloc>
inline void swap(vector<T, Alloc>& lhs, vector<T, Alloc>& rhs) {
    lhs.swap(rhs);
}
} // namespace mystl

#endif // MYSTL_VECTOR_H_
