#include <algorithm>
#include <vector>
#include "vector.h"
#include "priority_queue.h"
using std::cout;
using std::endl;
using std::vector;

int main() {
    // mystl::list_test();
    mystl::vector<int> vec = {1, 2, 3, 4, 5};
    mystl::priority_queue<int> pq(vec.begin(), vec.end());
    cout << pq.empty() << endl;        // 5
    cout << pq.size() << endl;  // 4
    cout << pq.top() << endl;    // 1
    pq.push(6);
    cout << pq.top() << endl;    // 1
    pq.pop();
    cout << pq.top() << endl;    // 1
    return 0;
}