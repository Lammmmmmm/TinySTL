#if !defined(MYSTL_ALLOC_H_)
#define MYSTL_ALLOC_H_

#include <stdlib.h>
#include <cstring>
#include <iostream>
#include "construct.h"

/* 本头文件中实现了具有 SGI 特色的两级分配器，我的个人博客 https://choubin.site 有详细讲解*/

namespace mystl {
// 使用 malloc 和 free 实现的一级分配器
// 可以由客端设置 OOM 时的 new_handler
class malloc_alloc {
private:
    // 函数指针类型
    using FunPtr = void (*)();

public:
    static void* allocate(size_t);
    static void deallocate(void* ptr) { free(ptr); }
    static void* reallocate(void*, size_t, size_t new_sz);
    static FunPtr set_malloc_handler(FunPtr f);

private:
    static void* oom_malloc(size_t);
    static void* oom_realloc(void*, size_t);
    static void (*malloc_alloc_oom_handler)();
};

void* malloc_alloc::allocate(size_t n) {
    void* result = malloc(n);
    if (result == 0)
        result = malloc_alloc::oom_malloc(n);
    return result;
}

void* malloc_alloc::reallocate(void* ptr, size_t old_sz, size_t new_sz) {
    void* result = realloc(ptr, new_sz);
    if (result == 0)
        result = malloc_alloc::oom_realloc(ptr, new_sz);
    return result;
}

void (*malloc_alloc::malloc_alloc_oom_handler)() = 0;

typename malloc_alloc::FunPtr malloc_alloc::set_malloc_handler(FunPtr fptr) {
    FunPtr old = malloc_alloc_oom_handler;
    malloc_alloc_oom_handler = fptr;
    return old;
}

void* malloc_alloc::oom_malloc(size_t n) {
    if (malloc_alloc_oom_handler == 0) {
        std::cerr << "out of memory" << std::endl;
        exit(1);
    }
    void* result;
    for (;;) {
        malloc_alloc_oom_handler();
        result = malloc(n);
        if (result)
            return result;
    }
}

void* malloc_alloc::oom_realloc(void* ptr, size_t new_sz) {
    if (malloc_alloc_oom_handler == 0) {
        std::cerr << "out of memory" << std::endl;
        exit(1);
    }
    void* result;
    for (;;) {
        malloc_alloc_oom_handler();
        result = realloc(ptr, new_sz);
        if (result)
            return result;
    }
}

class default_alloc {
private:
    enum { ALIGN = 8 };
    enum { MAX_BYTES = 128 };
    enum { NFREELISTS = MAX_BYTES / ALIGN };

    union obj {
        union obj* next;
        char client_data[1];
    };

    static char* start_free;
    static char* end_free;
    static size_t heap_size;
    static obj* volatile free_list[NFREELISTS];

private:
    static size_t round_up(size_t bytes) {
        return ((bytes + ALIGN - 1) & ~(ALIGN - 1));
    }
    static size_t freelist_index(size_t bytes) {
        return ((bytes + ALIGN - 1) / ALIGN - 1);
    }
    static void* refill(size_t n);
    static char* chunk_alloc(size_t size, int& nobjs);

public:
    static void* allocate(size_t n);
    static void deallocate(void* ptr, size_t n);
    static void* reallocate(void* ptr, size_t old_size, size_t new_size);
};

// 静态成员初始化
char* default_alloc::start_free = 0;
char* default_alloc::end_free = 0;
size_t default_alloc::heap_size = 0;
default_alloc::obj* volatile default_alloc::free_list[NFREELISTS] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

void* default_alloc::allocate(size_t n) {
    if (n > 128)
        return malloc_alloc::allocate(n);
    obj* volatile* my_free_list = free_list + freelist_index(n);
    obj* result = *my_free_list;
    if (result == 0)
        return refill(n);
    *my_free_list = (*my_free_list)->next;
    return static_cast<void*>(result);
}

void default_alloc::deallocate(void* ptr, size_t n) {
    if (n > 128) {
        malloc_alloc::deallocate(ptr);
        return;
    }
    obj* p = static_cast<obj*>(ptr);
    obj* volatile* my_free_list = free_list + freelist_index(n);
    p->next = *my_free_list;
    *my_free_list = p;
}

void* default_alloc::reallocate(void* ptr, size_t old_size, size_t new_size) {
    if (old_size > MAX_BYTES && new_size > MAX_BYTES)
        return realloc(ptr, new_size);
    if (round_up(old_size) == round_up(new_size))
        return ptr;
    void* result = allocate(new_size);
    size_t copy_sz = new_size > old_size ? new_size : old_size;
    memcpy(result, ptr, copy_sz);
    deallocate(ptr, old_size);
    return result;
}

char* default_alloc::chunk_alloc(size_t size, int& nobjs) {
    char* result;
    size_t total_bytes = size * nobjs;
    size_t bytes_left = end_free - start_free;
    if (bytes_left >= total_bytes) {
        result = start_free;
        start_free += total_bytes;
        return result;
    } else if (bytes_left >= size) {
        nobjs = bytes_left / size;
        result = start_free;
        start_free += nobjs * size;
        return result;
    } else {
        size_t bytes_to_get = 2 * total_bytes + round_up(heap_size >> 4);
        if (bytes_left > 0) {
            obj* volatile* my_free_list =
                free_list + freelist_index(bytes_left);
            ((obj*)start_free)->next = *my_free_list;
            *my_free_list = (obj*)start_free;
        }
        start_free = (char*)malloc(bytes_to_get);
        if (start_free == 0) {
            obj* volatile* my_free_list;
            obj* ptr = *my_free_list;
            for (size_t i = size; i <= MAX_BYTES; i += ALIGN) {
                my_free_list = free_list + freelist_index(i);
                ptr = *my_free_list;
                if (ptr != 0) {
                    *my_free_list = ptr->next;
                    start_free = (char*)ptr;
                    end_free = start_free + i;
                    return chunk_alloc(size, nobjs);
                }
            }
            end_free = 0;
            start_free = (char*)malloc_alloc::allocate(bytes_to_get);
        }
        heap_size += bytes_to_get;
        end_free = start_free + bytes_to_get;
        return chunk_alloc(size, nobjs);
    }
}

void* default_alloc::refill(size_t n) {
    int nobjs = 20;
    char* chunk = chunk_alloc(n, nobjs);
    if (nobjs == 1)
        return static_cast<void*>(chunk);
    void* result = static_cast<void*>(chunk);
    obj *next_obj, *current_obj;
    obj* volatile* my_free_list = free_list + freelist_index(n);
    *my_free_list = next_obj = (obj*)(chunk + n);
    for (int i = 1;; ++i) {
        current_obj = next_obj;
        next_obj = (obj*)((char*)next_obj + n);
        if (nobjs - 1 == i) {
            current_obj->next = 0;
            break;
        } else
            current_obj->next = next_obj;
    }
    return result;
}

// SGI STL 特色分配器，需要一个模板参数，具有 STL 标准接口
template <typename T>
class alloc {
public:
    // STL 要求的类型别名定义
    using value_type        = T;
    using pointer           = T*;
    using const_pointer     = const T*;
    using reference         = T&;
    using const_reference   = const T&;
    using size_type         = size_t;
    using difference_type   = ptrdiff_t;

public:
    // STL 要求的类接口，使用静态函数实现可以使频繁调用下减小开销
    static T* allocate();
    static T* allocate(size_type n);

    static void deallocate(T* ptr);
    static void deallocate(T*, size_type n);

    static void construct(T* ptr);
    static void construct(T* ptr, const T& value);
    static void construct(T* ptr, T&& value);

    static void destroy(T* ptr);
    static void destroy(T* first, T* last);

    static T* address(T& val);
    static size_t max_size();
    template <typename U>
    struct rebind {
        using other = alloc<U>;
    };
};

template <typename T>
T* alloc<T>::allocate(size_t n) {
    return n == 0 ? 0 : static_cast<T*>(default_alloc::allocate(n * sizeof(T)));
}

template <typename T>
T* alloc<T>::allocate() {
    return static_cast<T*>(default_alloc::allocate(sizeof(T)));
}

template <typename T>
void alloc<T>::deallocate(T* ptr, size_t n) {
    if (n != 0)
        default_alloc::deallocate((void*)ptr, n * sizeof(T));
}

template <typename T>
void alloc<T>::deallocate(T* ptr) {
    if (ptr)
        default_alloc::deallocate((void*)ptr, sizeof(T));
}

template <typename T>
void alloc<T>::construct(T* ptr) {
    mystl::construct(ptr);
}

template <typename T>
void alloc<T>::construct(T* ptr, const T& value) {
    mystl::construct(ptr, value);
}

template <typename T>
void alloc<T>::construct(T* ptr, T&& value) {
    mystl::construct(ptr, std::move(value));
}

template <typename T>
void alloc<T>::destroy(T* ptr) {
    mystl::destroy(ptr);
}

template <typename T>
void alloc<T>::destroy(T* first, T* last) {
    mystl::destroy(first, last);
}

template <typename T>
T* alloc<T>::address(T& val) {
    return (T*)(&val);
}

template <typename T>
size_t alloc<T>::max_size() {
    return (size_t)(WINT_MAX / sizeof(T));
}
}  // namespace mystl

#endif  // MYSTL_ALLOC_H_
