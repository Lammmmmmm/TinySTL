#if !defined(MYSTL_PRIORITY_QUEUE_H_)
#define MYSTL_PRIORITY_QUEUE_H_

#include "vector.h"
#include "heap.h"

namespace mystl {
template <typename T, typename Sequence = vector<T>>
class priority_queue {
public:
    using value_type        = typename Sequence::value_type;
    using size_type         = typename Sequence::size_type;
    using reference         = typename Sequence::reference;
    using const_reference   = typename Sequence::const_reference;

    priority_queue() : contain() {}
    template <typename InputIter>
    priority_queue(InputIter first, InputIter last) : contain(first, last)
        { make_heap(contain.begin(), contain.end()); }

    bool            empty() const { return contain.empty(); }
    size_type       size()  const { return contain.size(); }
    const_reference top()   const { return contain.front(); }

    void push(const value_type& value) {
        contain.push_back(value);
        push_heap(contain.begin(), contain.end());
    }
    void pop() {
        pop_heap(contain.begin(), contain.end());
        contain.pop_back();
    }

protected:
    Sequence contain;
}; // end priority_queue

}  // namespace mystl

#endif  // MYSTL_PRIORITY_QUEUE_H_
