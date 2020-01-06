#include <iostream>
#include "allocator.h"
#include "alloc.h"
#include "type_traits.h"
#include <vector>
#include "test_alloc.h"

using std::cout;
using std::endl;
using FunPtr = void (*)(int&);
void bigger(int& num){
    num += 1;
}
void (* set_m(FunPtr))(){

}

union obj {
    union obj* next;
    char data[1];
};

int main(){
    mystl::test_alloc();
    return 0;
}