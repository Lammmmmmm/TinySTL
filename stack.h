#if !defined(MYSLT_STACK_H_)
#define MYSLT_STACK_H_

#include "vector.h"

namespace mystl {

template <typename T, typename Container = mystl::vector<T>>
class stack {
public:
    using value_type        = typename Container::value_type;
    using size_type         = typename Container::size_type;
    using reference         = typename Container::reference;
    using const_reference   = typename Container::const_reference;

protected:
    Container con;

public:
    bool empty() const { return con.empty(); }
    size_type size() const { return con.size(); }
    reference top() { return con.back(); }
    const_reference top() const { return con.back(); }
    void push(const value_type& value) { con.push_back(value); }
    void pop() { con.pop_back(); }

private:
    friend bool operator==(const stack& lhs, const stack& rhs);
    friend bool operator<(const stack& lhs, const stack& rhs);
};

template <typename T, typename Container>
bool operator==(const stack<T, Container>& lhs,
                const stack<T, Container>& rhs) {
    return lhs.con == rhs.con;
}

template <typename T, typename Container>
bool operator<(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
    return lhs.con < rhs.con;
}
}  // namespace mystl

#endif  // MYSLT_STACK_H_
