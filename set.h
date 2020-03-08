#if !defined(MYSTL_SET_H_)
#define MYSTL_SET_H_

#include <functional>
#include "tree.h"

namespace mystl {
template <typename Key, typename Compare = std::less<Key>>
class set {
private:
    using rep_type = rb_tree<Key, Key, std::_Identity<Key>, Compare>;
    rep_type tree;

public:
    using key_type      = Key;
    using value_type    = Key;
    using key_compare   = Compare;
    using value_compare = Compare;

    using pointer           = typename rep_type::const_pointer;
    using const_point       = typename rep_type::const_pointer;
    using reference         = typename rep_type::const_reference;
    using const_reference   = typename rep_type::const_reference;
    using iterator          = typename rep_type::const_iterator;
    using const_iterator    = typename rep_type::const_iterator;
    using difference_type   = typename rep_type::difference_type;
    using size_type         = typename rep_type::size_type;

    // 构造函数
    template <typename InputIterator>
    set(InputIterator first, InputIterator last) : tree(Compare()) {
        tree.insert_unique(first, last);
    }
    template <typename InputIterator>
    set(InputIterator first, InputIterator last, const Compare& comp)
        : tree(comp) {
        tree.insert_unique(first, last);
    }
    set(const set<Key, Compare>& x) : tree(x.tree) {}
    set<Key, Compare>& operator=(const set<Key, Compare>& x) {
        tree = x.tree;
        return *this;
    }

    key_compare     key_comp()   const { return tree.key_comp(); }
    value_compare   value_comp() const { return tree.key_comp(); }
    iterator        begin()      const { return tree.begin(); }
    iterator        end()        const { return tree.end(); }
    bool            empty()      const { return tree.empty(); }
    size_type       size()       const { return tree.size(); }
    size_type       max_size()   const { return tree.max_size(); }
    void            swap(set<Key, Compare>& x) { tree.swap(x.tree); }

    std::pair<iterator, bool> insert(const value_type& x) {
        std::pair<typename rep_type::iterator, bool> p = tree.insert_unique(x);
        
        return std::make_pair(p.first, p.second);
    }

    iterator insert(iterator position, const value_type& x) {
        typedef typename rep_type::iterator rep_iterator;
        return tree.insert_unique((rep_iterator&)position, x);
    }

    template <typename InputIterator>
    void insert(InputIterator first, InputIterator last) {
        tree.insert_unique(first, last);
    }

    void erase(iterator position) {
        typedef typename rep_type::iterator rep_iterator;
        tree.erase((rep_iterator&)position);
    }
    size_type erase(const key_type& x) { return tree.erase(x); }
    void erase(iterator first, iterator last) {
        typedef typename rep_type::iterator rep_iterator;
        tree.erase((rep_iterator&)first, (rep_iterator&)last);
    }
    void clear() { tree.clear(); }

    iterator find(const key_type& x) const { return tree.find(x); }
    size_type count(const key_type& x) const { return tree.count(x); }
    iterator lower_bound(const key_type& x) const {
        return tree.lower_bound(x);
    }
    iterator upper_bound(const key_type& x) const {
        return tree.upper_bound(x);
    }
    std::pair<iterator, iterator> equal_range(const key_type& x) const {
        return tree.equal_range(x);
    }
}; // typename set

template <typename Key, typename Compare>
inline void swap(set<Key, Compare>& lhs, set<Key, Compare>& rhs) {
    lhs.swap(rhs);
}
}  // namespace mystl

#endif  // MYSTL_SET_H_
