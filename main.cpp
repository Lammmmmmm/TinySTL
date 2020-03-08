#include <algorithm>
#include <vector>
#include <functional>
#include "vector.h"
#include "priority_queue.h"
#include "tree.h"
using std::cout;
using std::endl;

int main() {
    mystl::rb_tree<int, int, std::_Identity<int>, std::less<int>> itree;
    cout << itree.size() << endl;
    itree.insert_unique(10);
    itree.insert_unique(7);
    itree.insert_unique(8);
    itree.insert_unique(15);
    itree.insert_unique(5);
    itree.insert_unique(6);
    cout << itree.size() << endl;
    for(auto &each: itree)
        cout << each << " ";
    cout << endl;
    return 0;
}