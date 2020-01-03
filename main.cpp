#include <iostream>
#include "allocator.h"
#include "type_traits.h"
#include <vector>

using std::cout;
using std::endl;

int main(){
    std::vector<int,mystl::allocator<int>> vec(5,5);
    for(auto &each: vec)
        cout << each << " ";
    cout << endl;
    return 0;
}