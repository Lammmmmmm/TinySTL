#if !defined(MYSTL_ALLOCATOR_H)
#define MYSTL_ALLOCATOR_H

#include "construct.h"

namespace mystl {
// 一个简单的 alloctor 模板类，仅仅是对 :operator new 和 :operator
// delete的简单封装
template <typename T>
class allocator {
   public:
    // STL 要求的类型别名定义
    using value_type 		= T;
    using pointer 			= T*;
    using const_pointer 	= const T*;
    using reference 		= T&;
    using const_reference 	= const T&;
    using size_type 		= size_t;
    using difference_type 	= ptrdiff_t;

   public:
    // STL 要求的类接口，使用静态函数实现可以使频繁调用下减小开销
    // 负责分配内存
    static T* allocate();
    static T* allocate(size_type n);
    // 负责释放内存
    static void deallocate(T* ptr);
    static void deallocate(T*, size_type n);
    // 负责构造对象
    static void construct(T* ptr);
    static void construct(T* ptr, const T& value);
    static void construct(T* ptr, T&& value);
    // 负责析构对象
    static void destroy(T* ptr);
    static void destroy(T* first, T* last);
    // 获取某对象的地址
    static T* address(T& val);
    // 获取可配置T类型对象的最大数目
    static size_t max_size();
    // 使T类型的alloctor可以为U类型的对象分配内存
    template <typename U>
    struct rebind {
        using other = allocator<U>;
    };
};

template <typename T>
T* allocator<T>::allocate() {
    return static_cast<T*>(::operator new(sizeof(T)));
}

template <typename T>
T* allocator<T>::allocate(size_type n) {
    return static_cast<T*>(::operator new(n * sizeof(T)));
}

template <typename T>
void allocator<T>::deallocate(T* ptr) {
    if (ptr != nullptr)
        ::operator delete(ptr);
}

template <typename T>
void allocator<T>::deallocate(T* ptr, size_type) {
    if (ptr != nullptr)
        ::operator delete(ptr);
}

template <typename T>
void allocator<T>::construct(T* ptr) {
    mystl::construct(ptr);
}

template <typename T>
void allocator<T>::construct(T* ptr, const T& value) {
    mystl::construct(ptr, value);
}

template <typename T>
void allocator<T>::construct(T* ptr, T&& value) {
    mystl::construct(ptr, std::move(value));
}

template <typename T>
void allocator<T>::destroy(T* ptr) {
    mystl::destroy(ptr);
}

template <typename T>
void allocator<T>::destroy(T* first, T* last) {
    mystl::destroy(first, last);
}

template <typename T>
T* allocator<T>::address(T& val) {
    return (T*)&val;
}

template <typename T>
size_t allocator<T>::max_size() {
    return size_t(WINT_MAX / sizeof(T));
}
// 具有 SGI STL特色的两级分配器见头文件 alloc.h
}  // namespace mystl

#endif  // MYSTL_ALLOCATOR_H
