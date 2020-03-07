#include <algorithm>
#include <vector>
#include "vector.h"
#include "priority_queue.h"
using std::cout;
using std::endl;
using std::vector;

int main() {
    mystl::vector<int> vec = {1, 2, 3, 4, 5};
    mystl::priority_queue<int> pq(vec.begin(), vec.end());
    cout << pq.empty() << endl;       
    cout << pq.size() << endl; 
    cout << pq.top() << endl;   
    pq.push(6);
    cout << pq.top() << endl;   
    pq.pop();
    cout << pq.top() << endl;    
    return 0;
}