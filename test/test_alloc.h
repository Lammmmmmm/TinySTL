#if !defined(MYSTL_TEST_ALLOC_H_)
#define MYSTL_TEST_ALLOC_H_

#include <time.h>
#include <list>
#include <iostream>
#include "allocator.h"
#include "alloc.h"

namespace mystl
{
enum { NUMBERS = 10000000 };

void test_alloc() {
        // test of list
    // test of push_back
    std::list<int> list1;
    std::list<int, mystl::allocator<int>> list2;
    std::list<int, mystl::alloc<int>> list3;
    clock_t start = clock();
    for(size_t i = 0; i < NUMBERS; ++i)
        list1.push_back(1);
    clock_t end = clock();
    std::cout << "Time to insert " << NUMBERS
         << " numbers in list with STL alloctor: "
         << end - start << std::endl;
    start = clock();
    for(size_t i = 0; i < NUMBERS; ++i)
        list2.push_back(1);
    end = clock();
    std::cout << "Time to insert " << NUMBERS
         << " numbers in list with mystl alloctor: "
         << end - start << std::endl;
    start = clock();
    for(size_t i = 0; i < NUMBERS; ++i)
        list3.push_back(1);
    end = clock();
    std::cout << "Time to insert " << NUMBERS
         << " numbers in list with mystl SGI alloc: "
         << end - start << std::endl;
}  
} // namespace mystl



#endif // MYSTL_TEST_ALLOC_H_


