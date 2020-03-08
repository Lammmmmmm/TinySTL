#if !defined(MYSTL_TREE_H_)
#define MYSTL_TREE_H_

#include <utility>
#include "memory.h"

/* 本头文件实现了红黑树类，由于细节实现十分复杂，所以仅对 SGI 源码做了简单修改*/
namespace mystl {

// 使用 bool 类型作为节点颜色类型
using tree_color_type = bool;
const tree_color_type red_node = false;
const tree_color_type black_node = true;

template <typename Value>
struct tree_node {
    using color_type    = tree_color_type;
    using link_type     = tree_node<Value>*;

    color_type  color;
    link_type   parent;
    link_type   left;
    link_type   right;
    Value       value;

    static link_type minimum(link_type ptr) {
        while (ptr->left != NULL)
            ptr = ptr->left;
        return ptr;
    }

    static link_type maximum(link_type ptr) {
        while (ptr->right != NULL)
            ptr = ptr->right;
        return ptr;
    }
};

template <typename Value, typename Ref, typename Ptr>
struct tree_iterator {
    using value_type        = Value;
    using reference         = Ref;
    using pointer           = Ptr;
    using difference_type   = ptrdiff_t;
    using iterator_category = bidirectional_iterator_tag;
    using iterator          = tree_iterator<Value, Value&, Value*>;
    using const_iterator    = tree_iterator<Value, const Value&, const Value*>;
    using self              = tree_iterator<Value, Ref, Ptr>;
    using link_type         = tree_node<Value>*;

    link_type node;

    tree_iterator(link_type x): node(x) {}

    self& operator++();
    self operator++(int);
    self& operator--();
    self operator--(int);
    reference operator*() const { return node->value; }
    pointer operator->() const { return &(operator*()); }
};

template <typename Value, typename Ref, typename Ptr>
typename tree_iterator<Value, Ref, Ptr>::self& tree_iterator<Value, Ref, Ptr>::
operator++() {
    if (node->right != NULL) {
        node = node->right;
        while (node->left != NULL)
            node = node->left;
    } else {
        link_type par = node->parent;
        while (node == par->right) {
            node = par;
            par = par->parent;
        }
        if (node->right != par)
            node = par;
    }
    return *this;
}

template <typename Value, typename Ref, typename Ptr>
typename tree_iterator<Value, Ref, Ptr>::self tree_iterator<Value, Ref, Ptr>::
operator++(int) {
    self tmp = *this;
    ++*this;
    return tmp;
}

template <typename Value, typename Ref, typename Ptr>
typename tree_iterator<Value, Ref, Ptr>::self& tree_iterator<Value, Ref, Ptr>::
operator--() {
    if (node->color == red_node && node->parent->parent == node)
        node = node->right;
    else if (node->left != NULL) {
        node = node->left;
        while (node->right != NULL)
            node = node->right;
    } else {
        link_type par = node->parent;
        while (node == par->left) {
            node = par;
            par = par->parent;
        }
        node = par;
    }
    return *this;
}

template <typename Value, typename Ref, typename Ptr>
typename tree_iterator<Value, Ref, Ptr>::self tree_iterator<Value, Ref, Ptr>::
operator--(int) {
    self tmp = *this;
    --*this;
    return tmp;
}
template <typename Value, typename Ref, typename Ptr>
inline bool operator==(const tree_iterator<Value, Ref, Ptr> lhs,
                       const tree_iterator<Value, Ref, Ptr> rhs) {
    return lhs.node == rhs.node;
}

template <typename Value, typename Ref, typename Ptr>
inline bool operator!=(const tree_iterator<Value, Ref, Ptr> lhs,
                       const tree_iterator<Value, Ref, Ptr> rhs) {
    return lhs.node != rhs.node;
}

// 左旋操作
template <typename Value>
inline void rb_tree_rotate_left(tree_node<Value>* x, tree_node<Value>*& root) {
    tree_node<Value>* y = x->right;
    x->right = y->left;
    if (y->left != 0)
        y->left->parent = x;
    y->parent = x->parent;

    if (x == root)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}
// 右旋操作
template <typename Value>
inline void rb_tree_rotate_right(tree_node<Value>* x, tree_node<Value>*& root) {
    tree_node<Value>* y = x->left;
    x->left = y->right;
    if (y->right != 0)
        y->right->parent = x;
    y->parent = x->parent;

    if (x == root)
        root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
    y->right = x;
    x->parent = y;
}

// 调整平衡
template <typename Value>
inline void rb_tree_rebalance(tree_node<Value>* x, tree_node<Value>*& root) {
    x->color = red_node;
    while (x != root && x->parent->color == red_node) {
        if (x->parent == x->parent->parent->left) {
            tree_node<Value>* y = x->parent->parent->right;
            if (y && y->color == red_node) {
                x->parent->color = black_node;
                y->color = black_node;
                x->parent->parent->color = red_node;
                x = x->parent->parent;
            } else {
                if (x == x->parent->right) {
                    x = x->parent;
                    rb_tree_rotate_left(x, root);
                }
                x->parent->color = black_node;
                x->parent->parent->color = red_node;
                rb_tree_rotate_right(x->parent->parent, root);
            }
        } else {
            tree_node<Value>* y = x->parent->parent->left;
            if (y && y->color == red_node) {
                x->parent->color = black_node;
                y->color = black_node;
                x->parent->parent->color = red_node;
                x = x->parent->parent;
            } else {
                if (x == x->parent->left) {
                    x = x->parent;
                    rb_tree_rotate_right(x, root);
                }
                x->parent->color = black_node;
                x->parent->parent->color = red_node;
                rb_tree_rotate_left(x->parent->parent, root);
            }
        }
    }
    root->color = black_node;
}

/* 这个代码太长了， 脑容量不够了，直接复制了，留着源码注释以后细读 */
// 删除节点后调整平衡
template <typename Value>
inline tree_node<Value>* rb_tree_rebalance_for_erase(tree_node<Value>* z,
                                                     tree_node<Value>*& root,
                                                     tree_node<Value>*& leftmost,
                                                     tree_node<Value>*& rightmost) {
    tree_node<Value>* y = z;
    tree_node<Value>* x = 0;
    tree_node<Value>* x_parent = 0;
    if (y->left == 0)        // z has at most one non-null child. y == z.
        x = y->right;        // x might be null.
    else if (y->right == 0)  // z has exactly one non-null child.  y == z.
        x = y->left;         // x is not null.
    else {                   // z has two non-null children.  Set y to
        y = y->right;        //   z's successor.  x might be null.
        while (y->left != 0)
            y = y->left;
        x = y->right;
    }
    if (y != z) {  // relink y in place of z.  y is z's successor
        z->left->parent = y;
        y->left = z->left;
        if (y != z->right) {
            x_parent = y->parent;
            if (x) x->parent = y->parent;
            y->parent->left = x;  // y must be a left child
            y->right = z->right;
            z->right->parent = y;
        } else
            x_parent = y;
        if (root == z)
            root = y;
        else if (z->parent->left == z)
            z->parent->left = y;
        else
            z->parent->right = y;
        y->parent = z->parent;
        std::swap(y->color, z->color);
        y = z;
    } else {  // y == z
        x_parent = y->parent;
        if (x)
            x->parent = y->parent;
        if (root == z)
            root = x;
        else if (z->parent->left == z)
            z->parent->left = x;
        else
            z->parent->right = x;
        if (leftmost == z)
            if (z->right == 0)  // z->left must be null also
                leftmost = z->parent;
            else
                leftmost = tree_node<Value>::minimum(x);
        if (rightmost == z)
            if (z->left == 0)  // z->right must be null also
                rightmost = z->parent;
            else  // x == z->left
                rightmost = tree_node<Value>::maximum(x);
    }
    if (y->color != red_node) {
        while (x != root && (x == 0 || x->color == black_node))
            if (x == x_parent->left) {
                tree_node<Value>* w = x_parent->right;
                if (w->color == red_node) {
                    w->color = black_node;
                    x_parent->color = red_node;
                    rb_tree_rotate_left(x_parent, root);
                    w = x_parent->right;
                }
                if ((w->left == 0 || w->left->color == black_node) &&
                    (w->right == 0 || w->right->color == black_node)) {
                    w->color = red_node;
                    x = x_parent;
                    x_parent = x_parent->parent;
                } else {
                    if (w->right == 0 || w->right->color == black_node) {
                        if (w->left)
                            w->left->color = black_node;
                        w->color = red_node;
                        rb_tree_rotate_right(w, root);
                        w = x_parent->right;
                    }
                    w->color = x_parent->color;
                    x_parent->color = black_node;
                    if (w->right)
                        w->right->color = black_node;
                    rb_tree_rotate_left(x_parent, root);
                    break;
                }
            } else {  // same as above, with right <-> left.
                tree_node<Value>* w = x_parent->left;
                if (w->color == red_node) {
                    w->color = black_node;
                    x_parent->color = red_node;
                    rb_tree_rotate_right(x_parent, root);
                    w = x_parent->left;
                }
                if ((w->right == 0 || w->right->color == black_node) &&
                    (w->left == 0 || w->left->color == black_node)) {
                    w->color = red_node;
                    x = x_parent;
                    x_parent = x_parent->parent;
                } else {
                    if (w->left == 0 || w->left->color == black_node) {
                        if (w->right)
                            w->right->color = black_node;
                        w->color = red_node;
                        rb_tree_rotate_left(w, root);
                        w = x_parent->left;
                    }
                    w->color = x_parent->color;
                    x_parent->color = black_node;
                    if (w->left)
                        w->left->color = black_node;
                    rb_tree_rotate_right(x_parent, root);
                    break;
                }
            }
        if (x)
            x->color = black_node;
    }
    return y;
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare>
class rb_tree {
   public:
    using key_type          = Key;
    using value_type        = Value;
    using pointer           = value_type*;
    using size_type         = size_t;
    using difference_type   = ptrdiff_t;
    using const_pointer     = const value_type*;
    using reference         = value_type&;
    using const_reference   = const value_type&;
    using link_type         = tree_node<Value>*;
    using iterator          = tree_iterator<Value, Value&, Value*>;
    using const_iterator    = tree_iterator<Value, Value&, Value*>;

   protected:
    using rb_tree_node  = tree_node<Value>;
    using node_alloc    = alloc<tree_node<Value>>;
    using color_type    = tree_color_type;

    size_type node_count;
    link_type header;
    Compare key_compare;

    link_type get_node() { return node_alloc::allocate(); }
    void put_node(link_type ptr) { node_alloc::deallocate(ptr); }

    link_type create_node(const value_type& value) {
        link_type tmp = get_node();
        try {
            construct(&tmp->value, value);
        } catch (...) {
            put_node(tmp);
            throw;
        }
        return tmp;
    }
    link_type clone_node(link_type cur) {
        link_type tmp = create_node(cur->value);
        tmp->color = cur->color;
        tmp->left = NULL;
        tmp->right = NULL;
        return tmp;
    }
    void destroy_node(link_type ptr) {
        destroy(&ptr->value);
        put_node(ptr);
    }

    link_type& root() const { return header->parent; }
    link_type& leftmost() const { return header->left; }
    link_type& rightmost() const { return header->right; }

    static link_type& left(link_type cur) { return cur->left; }
    static link_type& right(link_type cur) { return cur->right; }
    static link_type& parent(link_type cur) { return cur->parent; }
    static reference value(link_type cur) { return cur->value; }
    static const Key& key(link_type cur) { return KeyOfValue()(value(cur)); }
    static color_type& color(link_type cur) {
        return (color_type&)(cur->color);
    }
    static link_type minimum(link_type cur) {
        return rb_tree_node::minimum(cur);
    }
    static link_type maximum(link_type cur) {
        return rb_tree_node::maximum(cur);
    }

    iterator insert_aux(link_type, link_type, const value_type& value);
    link_type copy_aux(link_type, link_type);
    void erase_aux(link_type cur);
    void init() {
        header = get_node();
        color(header) = red_node;
        root() = NULL;
        leftmost() = header;
        rightmost() = header;
    }

   public:
    rb_tree(const Compare& comp = Compare())
        : node_count(0), key_compare(comp) {
        init();
    }
    rb_tree(const rb_tree<Key, Value, KeyOfValue, Compare>& tree) {
        header = get_node();
        color(header) = red_node;
        if (tree.root() == NULL) {
            root() = NULL;
            leftmost() = header;
            rightmost() = header;
        } else {
            try {
                root() = copy_aux(tree.root(), header);
            } catch (...) {
                put_node(header);
                throw;
            }
            leftmost() = minimum(root());
            rightmost() = maximun(root());
        }
        node_count = tree.node_count;
    }
    rb_tree<Key, Value, KeyOfValue, Compare>& operator=(
        const rb_tree<Key, Value, KeyOfValue, Compare>& x);
    ~rb_tree() {
        clear();
        put_node(header);
    }

    Compare key_comp() const { return key_compare; }
    iterator begin() { return leftmost(); }
    const_iterator begin() const { return leftmost(); }
    iterator end() { return header; }
    const_iterator end() const { return header; }
    bool empty() const { return node_count == 0; }
    size_type size() const { return node_count; }
    size_type max_size() const { return size_type(-1); }

    void swap(rb_tree<Key, Value, KeyOfValue, Compare>& rhs) {
        std::swap(header, rhs.header);
        std::swap(node_count, rhs.node_count);
        std::swap(key_compare, rhs.key_compare);
    }

    std::pair<iterator, bool> insert_unique(const value_type& value);
    iterator insert_equal(const value_type& value);
    iterator insert_unique(iterator position, const value_type& value);
    iterator insert_equal(iterator position, const value_type& value);

    template <typename InputIterator>
    void insert_unique(InputIterator first, InputIterator last);
    template <typename InputIterator>
    void insert_equal(InputIterator first, InputIterator last);

    void erase(iterator position);
    size_type erase(const key_type& x);
    void erase(iterator first, iterator last);
    void erase(const key_type* first, const key_type* last);
    void clear() {
        if (node_count != 0) {
            erase_aux(root());
            leftmost() = header;
            root() = 0;
            rightmost() = header;
            node_count = 0;
        }
    }

    iterator find(const key_type& x);
    const_iterator find(const key_type& x) const;
    size_type count(const key_type& x) const;
    iterator lower_bound(const key_type& x);
    const_iterator lower_bound(const key_type& x) const;
    iterator upper_bound(const key_type& x);
    const_iterator upper_bound(const key_type& x) const;
    std::pair<iterator, iterator> equal_range(const key_type& x);
    std::pair<const_iterator, const_iterator> equal_range(
        const key_type& x) const;
    bool rb_verify() const;
}; // class tree

template <typename Key, typename Value, typename KeyOfValue, typename Compare>
inline bool operator==(const rb_tree<Key, Value, KeyOfValue, Compare>& lhs,
                       const rb_tree<Key, Value, KeyOfValue, Compare>& rhs) {
    return lhs.size() == rhs.size() && equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare>
inline bool operator<(const rb_tree<Key, Value, KeyOfValue, Compare>& lhs,
                      const rb_tree<Key, Value, KeyOfValue, Compare>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare>
inline void swap(rb_tree<Key, Value, KeyOfValue, Compare>& lhs,
                 rb_tree<Key, Value, KeyOfValue, Compare>& rhs) {
    lhs.swap(rhs);
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare>
rb_tree<Key, Value, KeyOfValue, Compare>&
rb_tree<Key, Value, KeyOfValue, Compare>::operator=(
    const rb_tree<Key, Value, KeyOfValue, Compare>& x) {
    if (this != &x) {
        clear();
        node_count = 0;
        key_compare = x.key_compare;
        if (x.root() == 0) {
            root() = 0;
            leftmost() = header;
            rightmost() = header;
        } else {
            root() = __copy(x.root(), header);
            leftmost() = minimum(root());
            rightmost() = maximum(root());
            node_count = x.node_count;
        }
    }
    return *this;
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare>
typename rb_tree<Key, Value, KeyOfValue, Compare>::iterator
rb_tree<Key, Value, KeyOfValue, Compare>::insert_aux(link_type x,
                                                     link_type y,
                                                     const Value& v) {
    link_type z;
    if (y == header || x != 0 || key_compare(KeyOfValue()(v), key(y))) {
        z = create_node(v);
        left(y) = z;
        if (y == header) {
            root() = z;
            rightmost() = z;
        } else if (y == leftmost())
            leftmost() = z;
    } else {
        z = create_node(v);
        right(y) = z;
        if (y == rightmost())
            rightmost() = z;
    }
    parent(z) = y;
    left(z) = 0;
    right(z) = 0;
    rb_tree_rebalance(z, header->parent);
    ++node_count;
    return iterator(z);
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare>
typename rb_tree<Key, Value, KeyOfValue, Compare>::iterator
rb_tree<Key, Value, KeyOfValue, Compare>::insert_equal(const Value& v) {
    link_type y = header;
    link_type x = root();
    while (x != 0) {
        y = x;
        x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);
    }
    return insert_aux(x, y, v);
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare>
std::pair<typename rb_tree<Key, Value, KeyOfValue, Compare>::iterator, bool>
rb_tree<Key, Value, KeyOfValue, Compare>::insert_unique(const Value& v) {
    link_type y = header;
    link_type x = root();
    bool comp = true;
    while (x != 0) {
        y = x;
        comp = key_compare(KeyOfValue()(v), key(x));
        x = comp ? left(x) : right(x);
    }
    iterator j = iterator(y);
    if (comp)
        if (j == begin())
            return std::pair<iterator, bool>(insert_aux(x, y, v), true);
        else
            --j;
    if (key_compare(key(j.node), KeyOfValue()(v)))
        return std::pair<iterator, bool>(insert_aux(x, y, v), true);
    return std::pair<iterator, bool>(j, false);
}

template <typename Key, typename Val, typename KeyOfValue, typename Compare>
typename rb_tree<Key, Val, KeyOfValue, Compare>::iterator
rb_tree<Key, Val, KeyOfValue, Compare>::insert_unique(iterator position,
                                                      const Val& v) {
    if (position.node == header->left)
        if (size() > 0 && key_compare(KeyOfValue()(v), key(position.node)))
            return insert_aux(position.node, position.node, v);
        else
            return insert_unique(v).first;
    else if (position.node == header)
        if (key_compare(key(rightmost()), KeyOfValue()(v)))
            return insert_aux(0, rightmost(), v);
        else
            return insert_unique(v).first;
    else {
        iterator before = position;
        --before;
        if (key_compare(key(before.node), KeyOfValue()(v)) &&
            key_compare(KeyOfValue()(v), key(position.node)))
            if (right(before.node) == 0)
                return insert_aux(0, before.node, v);
            else
                return insert_aux(position.node, position.node, v);
        else
            return insert_unique(v).first;
    }
}

template <typename Key, typename Val, typename KeyOfValue, typename Compare>
typename rb_tree<Key, Val, KeyOfValue, Compare>::iterator
rb_tree<Key, Val, KeyOfValue, Compare>::insert_equal(iterator position,
                                                     const Val& v) {
    if (position.node == header->left)
        if (size() > 0 && key_compare(KeyOfValue()(v), key(position.node)))
            return insert_aux(position.node, position.node, v);
        else
            return insert_equal(v);
    else if (position.node == header)
        if (!key_compare(KeyOfValue()(v), key(rightmost())))
            return insert_aux(0, rightmost(), v);
        else
            return insert_equal(v);
    else {
        iterator before = position;
        --before;
        if (!key_compare(KeyOfValue()(v), key(before.node)) &&
            !key_compare(key(position.node), KeyOfValue()(v)))
            if (right(before.node) == 0)
                return insert_aux(0, before.node, v);
            else
                return insert_aux(position.node, position.node, v);
        // first argument just needs to be non-null
        else
            return insert_equal(v);
    }
}

template <typename K, typename V, typename KoV, typename Cmp>
template <typename II>
void rb_tree<K, V, KoV, Cmp>::insert_equal(II first, II last) {
    for (; first != last; ++first)
        insert_equal(*first);
}

template <typename K, typename V, typename KoV, typename Cmp>
template <typename II>
void rb_tree<K, V, KoV, Cmp>::insert_unique(II first, II last) {
    for (; first != last; ++first)
        insert_unique(*first);
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare>
inline void rb_tree<Key, Value, KeyOfValue, Compare>::erase(iterator position) {
    link_type y = (link_type)rb_tree_rebalance_for_erase(
        position.node, header->parent, header->left, header->right);
    destroy_node(y);
    --node_count;
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare>
typename rb_tree<Key, Value, KeyOfValue, Compare>::size_type
rb_tree<Key, Value, KeyOfValue, Compare>::erase(const Key& x) {
    std::pair<iterator, iterator> p = equal_range(x);
    size_type n = 0;
    distance(p.first, p.second, n);
    erase(p.first, p.second);
    return n;
}

template <typename K, typename V, typename KeyOfValue, typename Compare>
typename rb_tree<K, V, KeyOfValue, Compare>::link_type
rb_tree<K, V, KeyOfValue, Compare>::copy_aux(link_type x, link_type p) {
    link_type top = clone_node(x);
    top->parent = p;
    try {
        if (x->right)
            top->right = copy_aux(right(x), top);
        p = top;
        x = left(x);

        while (x != 0) {
            link_type y = clone_node(x);
            p->left = y;
            y->parent = p;
            if (x->right)
                y->right = copy_aux(right(x), y);
            p = y;
            x = left(x);
        }
    } catch (...) {
        (erase_aux(top));
        throw;
    }
    return top;
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare>
void rb_tree<Key, Value, KeyOfValue, Compare>::erase_aux(link_type x) {
    while (x != 0) {
        erase_aux(right(x));
        link_type y = left(x);
        destroy_node(x);
        x = y;
    }
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare>
void rb_tree<Key, Value, KeyOfValue, Compare>::erase(iterator first,
                                                     iterator last) {
    if (first == begin() && last == end())
        clear();
    else
        while (first != last)
            erase(first++);
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare>
void rb_tree<Key, Value, KeyOfValue, Compare>::erase(const Key* first,
                                                     const Key* last) {
    while (first != last)
        erase(*first++);
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare>
typename rb_tree<Key, Value, KeyOfValue, Compare>::iterator
rb_tree<Key, Value, KeyOfValue, Compare>::find(const Key& k) {
    link_type y = header;
    link_type x = root();

    while (x != 0)
        if (!key_compare(key(x), k))
            y = x, x = left(x);
        else
            x = right(x);

    iterator j = iterator(y);
    return (j == end() || key_compare(k, key(j.node))) ? end() : j;
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare>
typename rb_tree<Key, Value, KeyOfValue, Compare>::const_iterator
rb_tree<Key, Value, KeyOfValue, Compare>::find(const Key& k) const {
    link_type y = header;
    link_type x = root();

    while (x != 0) {
        if (!key_compare(key(x), k))
            y = x, x = left(x);
        else
            x = right(x);
    }
    const_iterator j = const_iterator(y);
    return (j == end() || key_compare(k, key(j.node))) ? end() : j;
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare>
typename rb_tree<Key, Value, KeyOfValue, Compare>::size_type
rb_tree<Key, Value, KeyOfValue, Compare>::count(const Key& k) const {
    std::pair<const_iterator, const_iterator> p = equal_range(k);
    size_type n = 0;
    distance(p.first, p.second, n);
    return n;
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare>
typename rb_tree<Key, Value, KeyOfValue, Compare>::iterator
rb_tree<Key, Value, KeyOfValue, Compare>::lower_bound(const Key& k) {
    link_type y = header;
    link_type x = root();

    while (x != 0)
        if (!key_compare(key(x), k))
            y = x, x = left(x);
        else
            x = right(x);
    return iterator(y);
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare>
typename rb_tree<Key, Value, KeyOfValue, Compare>::const_iterator
rb_tree<Key, Value, KeyOfValue, Compare>::lower_bound(const Key& k) const {
    link_type y = header;
    link_type x = root();

    while (x != 0)
        if (!key_compare(key(x), k))
            y = x, x = left(x);
        else
            x = right(x);
    return const_iterator(y);
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare>
typename rb_tree<Key, Value, KeyOfValue, Compare>::iterator
rb_tree<Key, Value, KeyOfValue, Compare>::upper_bound(const Key& k) {
    link_type y = header;
    link_type x = root();

    while (x != 0)
        if (key_compare(k, key(x)))
            y = x, x = left(x);
        else
            x = right(x);
    return iterator(y);
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare>
typename rb_tree<Key, Value, KeyOfValue, Compare>::const_iterator
rb_tree<Key, Value, KeyOfValue, Compare>::upper_bound(const Key& k) const {
    link_type y = header;
    link_type x = root();

    while (x != 0)
        if (key_compare(k, key(x)))
            y = x, x = left(x);
        else
            x = right(x);
    return const_iterator(y);
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare>
inline std::pair<typename rb_tree<Key, Value, KeyOfValue, Compare>::iterator,
                 typename rb_tree<Key, Value, KeyOfValue, Compare>::iterator>
rb_tree<Key, Value, KeyOfValue, Compare>::equal_range(const Key& k) {
    return std::pair<iterator, iterator>(lower_bound(k), upper_bound(k));
}

template <typename Key, typename Value, typename KoV, typename Compare>
inline std::pair<typename rb_tree<Key, Value, KoV, Compare>::const_iterator,
                 typename rb_tree<Key, Value, KoV, Compare>::const_iterator>
rb_tree<Key, Value, KoV, Compare>::equal_range(const Key& k) const {
    return std::pair<const_iterator, const_iterator>(lower_bound(k),
                                                     upper_bound(k));
}

template <typename Value>
inline int black_count(tree_node<Value>* node, tree_node<Value>* root) {
    if (node == 0)
        return 0;
    else {
        int bc = node->color == black_node ? 1 : 0;
        if (node == root)
            return bc;
        else
            return bc + black_count(node->parent, root);
    }
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare>
bool rb_tree<Key, Value, KeyOfValue, Compare>::rb_verify() const {
    if (node_count == 0 || begin() == end())
        return node_count == 0 && begin() == end() && header->left == header &&
               header->right == header;

    int len = black_count(leftmost(), root());
    for (const_iterator it = begin(); it != end(); ++it) {
        link_type x = (link_type)it.node;
        link_type L = left(x);
        link_type R = right(x);

        if (x->color == red_node)
            if ((L && L->color == red_node) || (R && R->color == red_node))
                return false;

        if (L && key_compare(key(x), key(L)))
            return false;
        if (R && key_compare(key(R), key(x)))
            return false;

        if (!L && !R && black_count(x, root()) != len)
            return false;
    }

    if (leftmost() != tree_node<Value>::minimum(root()))
        return false;
    if (rightmost() != tree_node<Value>::maximum(root()))
        return false;

    return true;
}

}  // namespace mystl

#endif  // MYSTL_TREE_H_
