/** @file rb_tree.h
 *  This is an internal header file, included by map.hpp and set.hpp
 *  You should not attempt to use it directly.
 */

#ifndef _rb_TREE_HPP__
#define _rb_TREE_HPP__

#include "algobase.hpp"
#include "pair.hpp"
#include <memory>

namespace ft {

enum rb_tree_color { red = false, black = true };

//!@{ Tree Node ////////////////////////////////////////////////////////////////

struct rb_tree_node_base {
  typedef rb_tree_node_base* base_ptr;

  rb_tree_color m_color;
  base_ptr      m_parent;
  base_ptr      m_left;
  base_ptr      m_right;

  static base_ptr s_minimum(base_ptr x) {
    while (x->m_left != 0)
      x = x->m_left;
    return x;
  }

  static base_ptr s_maximum(base_ptr x) {
    while (x->m_right != 0)
      x = x->m_right;
    return x;
  }
};

template <typename Val>
struct rb_tree_node : public rb_tree_node_base {
  typedef rb_tree_node<Val>* link_type;
  Val                        m_value_field;
};

//!@}

//!@{ Tree Iterator ////////////////////////////////////////////////////////////

struct rb_tree_base_iterator {
  typedef rb_tree_node_base::base_ptr     base_ptr;
  typedef std::bidirectional_iterator_tag iterator_category;
  typedef ptrdiff_t                       difference_type;

  base_ptr m_node;

  void m_increment() {
    if (m_node->m_right != 0) {
      m_node = m_node->m_right;
      while (m_node->m_left != 0)
        m_node = m_node->m_left;
    } else {
      base_ptr y = m_node->m_parent;
      while (m_node == y->m_right) {
        m_node = y;
        y = y->m_parent;
      }
      if (m_node->m_right != y)
        m_node = y;
    }
  }

  void m_decrement() {
    if (m_node->m_color == red && m_node->m_parent->m_parent == m_node)
      m_node = m_node->m_right;
    else if (m_node->m_left != 0) {
      base_ptr y = m_node->m_left;
      while (y->m_right != 0)
        y = y->m_right;
      m_node = y;
    } else {
      base_ptr y = m_node->m_parent;
      while (m_node == y->m_left) {
        m_node = y;
        y = y->m_parent;
      }
      m_node = y;
    }
  }
};

template <typename Val, typename Ref, typename Ptr>
struct rb_tree_iterator : public rb_tree_base_iterator {
  typedef Val                                           value_type;
  typedef Ref                                           reference;
  typedef Ptr                                           pointer;
  typedef rb_tree_iterator<Val, Val&, Val*>             iterator;
  typedef rb_tree_iterator<Val, const Val&, const Val*> const_iterator;
  typedef rb_tree_iterator<Val, Ref, Ptr>               self;
  typedef rb_tree_node<Val>*                            link_type;

  rb_tree_iterator() { }
  rb_tree_iterator(rb_tree_node_base* x) { m_node = x; }
  rb_tree_iterator(const iterator& it) { m_node = it.m_node; }

  reference operator*() const { return link_type(m_node)->m_value_field; }

  pointer operator->() const { return &(operator*()); }

  self& operator++() {
    m_increment();
    return *this;
  }

  self operator++(int) {
    self tmp = *this;
    m_increment();
    return tmp;
  }

  self& operator--() {
    m_decrement();
    return *this;
  }

  self operator--(int) {
    self tmp = *this;
    m_decrement();
    return tmp;
  }
};

template <typename Val, typename Ref, typename Ptr>
inline bool operator==(const rb_tree_iterator<Val, Ref, Ptr>& x,
                       const rb_tree_iterator<Val, Ref, Ptr>& y) {
  return x.m_node == y.m_node;
}

template <typename Val>
inline bool operator==(const rb_tree_iterator<Val, const Val&, const Val*>& x,
                       const rb_tree_iterator<Val, Val&, Val*>&             y) {
  return x.m_node == y.m_node;
}

template <typename Val>
inline bool operator==(const rb_tree_iterator<Val, Val&, Val*>&             x,
                       const rb_tree_iterator<Val, const Val&, const Val*>& y) {
  return x.m_node == y.m_node;
}

template <typename Val, typename Ref, typename Ptr>
inline bool operator!=(const rb_tree_iterator<Val, Ref, Ptr>& x,
                       const rb_tree_iterator<Val, Ref, Ptr>& y) {
  return x.m_node != y.m_node;
}

template <typename Val>
inline bool operator!=(const rb_tree_iterator<Val, const Val&, const Val*>& x,
                       const rb_tree_iterator<Val, Val&, Val*>&             y) {
  return x.m_node != y.m_node;
}

template <typename Val>
inline bool operator!=(const rb_tree_iterator<Val, Val&, Val*>&             x,
                       const rb_tree_iterator<Val, const Val&, const Val*>& y) {
  return x.m_node != y.m_node;
}

inline void rb_tree_rotate_left(rb_tree_node_base*  x,
                                rb_tree_node_base*& root) {
  rb_tree_node_base* y = x->m_right;
  x->m_right = y->m_left;
  if (y->m_left != 0)
    y->m_left->m_parent = x;
  y->m_parent = x->m_parent;

  if (x == root)
    root = y;
  else if (x == x->m_parent->m_left)
    x->m_parent->m_left = y;
  else
    x->m_parent->m_right = y;
  y->m_left = x;
  x->m_parent = y;
}

inline void rb_tree_rotate_right(rb_tree_node_base*  x,
                                 rb_tree_node_base*& root) {
  rb_tree_node_base* y = x->m_left;
  x->m_left = y->m_right;
  if (y->m_right != 0)
    y->m_right->m_parent = x;
  y->m_parent = x->m_parent;

  if (x == root)
    root = y;
  else if (x == x->m_parent->m_right)
    x->m_parent->m_right = y;
  else
    x->m_parent->m_left = y;
  y->m_right = x;
  x->m_parent = y;
}

inline void rb_tree_rebalance(rb_tree_node_base* x, rb_tree_node_base*& root) {
  x->m_color = red;
  while (x != root && x->m_parent->m_color == red) {
    if (x->m_parent == x->m_parent->m_parent->m_left) {
      rb_tree_node_base* y = x->m_parent->m_parent->m_right;
      if (y && y->m_color == red) {
        x->m_parent->m_color = black;
        y->m_color = black;
        x->m_parent->m_parent->m_color = red;
        x = x->m_parent->m_parent;
      } else {
        if (x == x->m_parent->m_right) {
          x = x->m_parent;
          rb_tree_rotate_left(x, root);
        }
        x->m_parent->m_color = black;
        x->m_parent->m_parent->m_color = red;
        rb_tree_rotate_right(x->m_parent->m_parent, root);
      }
    } else {
      rb_tree_node_base* y = x->m_parent->m_parent->m_left;
      if (y && y->m_color == red) {
        x->m_parent->m_color = black;
        y->m_color = black;
        x->m_parent->m_parent->m_color = red;
        x = x->m_parent->m_parent;
      } else {
        if (x == x->m_parent->m_left) {
          x = x->m_parent;
          rb_tree_rotate_right(x, root);
        }
        x->m_parent->m_color = black;
        x->m_parent->m_parent->m_color = red;
        rb_tree_rotate_left(x->m_parent->m_parent, root);
      }
    }
  }
  root->m_color = black;
}

inline rb_tree_node_base*
rb_tree_rebalance_for_erase(rb_tree_node_base* z, rb_tree_node_base*& root,
                            rb_tree_node_base*& leftmost,
                            rb_tree_node_base*& rightmost) {
  rb_tree_node_base* y = z;
  rb_tree_node_base* x = 0;
  rb_tree_node_base* xparent = 0;
  if (y->m_left == 0)       // z has at most one non-null child. y == z.
    x = y->m_right;         // x might be null.
  else if (y->m_right == 0) // z has exactly one non-null child. y == z.
    x = y->m_left;          // x is not null.
  else {
    // z has two non-null children.  Set y to
    y = y->m_right; //   z's successor.  x might be null.
    while (y->m_left != 0)
      y = y->m_left;
    x = y->m_right;
  }
  if (y != z) {
    // relink y in place of z.  y is z's successor
    z->m_left->m_parent = y;
    y->m_left = z->m_left;
    if (y != z->m_right) {
      xparent = y->m_parent;
      if (x)
        x->m_parent = y->m_parent;
      y->m_parent->m_left = x; // y must be a child of m_left
      y->m_right = z->m_right;
      z->m_right->m_parent = y;
    } else
      xparent = y;
    if (root == z)
      root = y;
    else if (z->m_parent->m_left == z)
      z->m_parent->m_left = y;
    else
      z->m_parent->m_right = y;
    y->m_parent = z->m_parent;
    ft::swap(y->m_color, z->m_color);
    y = z;
    // y now points to node to be actually deleted
  } else { // y == z
    xparent = y->m_parent;
    if (x)
      x->m_parent = y->m_parent;
    if (root == z)
      root = x;
    else if (z->m_parent->m_left == z)
      z->m_parent->m_left = x;
    else
      z->m_parent->m_right = x;
    if (leftmost == z) {
      if (z->m_right == 0) { // z->m_left must be null also
        leftmost = z->m_parent;
        // makes leftmost == m_header if z == root
      } else {
        leftmost = rb_tree_node_base::s_minimum(x);
      }
    }
    if (rightmost == z) {
      if (z->m_left == 0) { // z->m_right must be null also
        rightmost = z->m_parent;
        // makes rightmost == m_header if z == root
      } else { // x == z->m_left
        rightmost = rb_tree_node_base::s_maximum(x);
      }
    }
  }
  if (y->m_color != red) {
    while (x != root && (x == 0 || x->m_color == black))
      if (x == xparent->m_left) {
        rb_tree_node_base* w = xparent->m_right;
        if (w->m_color == red) {
          w->m_color = black;
          xparent->m_color = red;
          rb_tree_rotate_left(xparent, root);
          w = xparent->m_right;
        }
        if ((w->m_left == 0 || w->m_left->m_color == black) &&
            (w->m_right == 0 || w->m_right->m_color == black)) {
          w->m_color = red;
          x = xparent;
          xparent = xparent->m_parent;
        } else {
          if (w->m_right == 0 || w->m_right->m_color == black) {
            w->m_left->m_color = black;
            w->m_color = red;
            rb_tree_rotate_right(w, root);
            w = xparent->m_right;
          }
          w->m_color = xparent->m_color;
          xparent->m_color = black;
          if (w->m_right)
            w->m_right->m_color = black;
          rb_tree_rotate_left(xparent, root);
          break;
        }
      } else {
        // same as above, with m_right <-> m_left.
        rb_tree_node_base* w = xparent->m_left;
        if (w->m_color == red) {
          w->m_color = black;
          xparent->m_color = red;
          rb_tree_rotate_right(xparent, root);
          w = xparent->m_left;
        }
        if ((w->m_right == 0 || w->m_right->m_color == black) &&
            (w->m_left == 0 || w->m_left->m_color == black)) {
          w->m_color = red;
          x = xparent;
          xparent = xparent->m_parent;
        } else {
          if (w->m_left == 0 || w->m_left->m_color == black) {
            w->m_right->m_color = black;
            w->m_color = red;
            rb_tree_rotate_left(w, root);
            w = xparent->m_left;
          }
          w->m_color = xparent->m_color;
          xparent->m_color = black;
          if (w->m_left)
            w->m_left->m_color = black;
          rb_tree_rotate_right(xparent, root);
          break;
        }
      }
    if (x)
      x->m_color = black;
  }
  return y;
}

//!@}

//!@{ Tree /////////////////////////////////////////////////////////////////////

// Base class to encapsulate the differences between old SGI-style
// allocators and standard-conforming allocators.  In order to avoid
// having an empty base class, we arbitrarily move one of rb_tree's
// data members into the base class.

// Base for general standard-conforming allocators.
template <typename Tp, typename Alloc>
class rb_tree_base {
public:
  typedef typename Alloc::template rebind<Tp>::other allocator_type;

  allocator_type get_allocator() const { return node_allocator; }

  rb_tree_base(const allocator_type& a) : node_allocator(a) { }

protected:
  typename Alloc::template rebind<rb_tree_node<Tp> >::other node_allocator;

  rb_tree_node_base m_header;

  rb_tree_node<Tp>* m_get_node() { return node_allocator.allocate(1); }

  void m_put_node(rb_tree_node<Tp>* p) { node_allocator.deallocate(p, 1); }

  void m_destroynode(rb_tree_node<Tp>* p) { node_allocator.destroy(p); }
};

template <typename Key, typename Val, typename KeyOfValue,
          typename Compare = std::less<Key>,
          typename Alloc = std::allocator<Val> >
class rb_tree : protected rb_tree_base<Val, Alloc> {
  typedef rb_tree_base<Val, Alloc> Base;

protected:
  typedef rb_tree_node_base* base_ptr;
  typedef rb_tree_node<Val>  rb_tree_node;

public:
  typedef Key               keytype;
  typedef Val               value_type;
  typedef value_type*       pointer;
  typedef const value_type* const_pointer;
  typedef value_type&       reference;
  typedef const value_type& const_reference;
  typedef rb_tree_node*     link_type;
  typedef size_t            size_type;
  typedef ptrdiff_t         difference_type;

  typedef typename Base::allocator_type allocator_type;
  allocator_type get_allocator() const { return Base::get_allocator(); }

protected:
  using Base::m_destroynode;
  using Base::m_get_node;
  using Base::m_header;
  using Base::m_put_node;

  link_type m_create_node(const value_type& x) {
    link_type tmp = m_get_node();
    try {
      get_allocator().construct(&tmp->m_value_field, x);
    } catch (...) {
      m_put_node(tmp);
      throw;
    }
    return tmp;
  }

  link_type m_clone_node(link_type x) {
    link_type tmp = m_create_node(x->m_value_field);
    tmp->m_color = x->m_color;
    tmp->m_left = 0;
    tmp->m_right = 0;
    return tmp;
  }

  void destroynode(link_type p) {
    m_destroynode(p);
    m_put_node(p);
  }

  size_type m_node_count; // keeps track of size of tree
  Compare   m_key_compare;

  link_type& m_root() const { return (link_type&)this->m_header.m_parent; }

  link_type& m_leftmost() const { return (link_type&)this->m_header.m_left; }

  link_type& m_rightmost() const { return (link_type&)this->m_header.m_right; }

  link_type m_end() const { return (link_type) & this->m_header; }

  static link_type& s_left(link_type x) { return (link_type&)(x->m_left); }

  static link_type& s_right(link_type x) { return (link_type&)(x->m_right); }

  static link_type& s_parent(link_type x) { return (link_type&)(x->m_parent); }

  static reference s_value(link_type x) { return x->m_value_field; }

  static const Key& s_key(link_type x) { return KeyOfValue()(s_value(x)); }

  static link_type& s_left(base_ptr x) { return (link_type&)(x->m_left); }

  static link_type& s_right(base_ptr x) { return (link_type&)(x->m_right); }

  static link_type& s_parent(base_ptr x) { return (link_type&)(x->m_parent); }

  static reference s_value(base_ptr x) { return ((link_type)x)->m_value_field; }

  static const Key& s_key(base_ptr x) {
    return KeyOfValue()(s_value(link_type(x)));
  }

  static rb_tree_color& s_color(base_ptr x) { return x->m_color; }

  static link_type s_minimum(link_type x) {
    return (link_type)rb_tree_node_base::s_minimum(x);
  }

  static link_type s_maximum(link_type x) {
    return (link_type)rb_tree_node_base::s_maximum(x);
  }

public:
  typedef rb_tree_iterator<value_type, reference, pointer> iterator;
  typedef rb_tree_iterator<value_type, const_reference, const_pointer>
      const_iterator;

  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef std::reverse_iterator<iterator>       reverse_iterator;

private:
  iterator m_insert(base_ptr base_ptr_x, base_ptr base_ptr_y, const value_type& v) {
    link_type x = (link_type)base_ptr_x;
    link_type y = (link_type)base_ptr_y;
    link_type z;

    if (y == &this->m_header || x != 0 ||
        m_key_compare(KeyOfValue()(v), s_key(y))) {
      z = m_create_node(v);
      s_left(y) = z; // also makes m_leftmost() = z
      //    when y == &m_header
      if (y == &this->m_header) {
        m_root() = z;
        m_rightmost() = z;
      } else if (y == m_leftmost())
        m_leftmost() = z; // maintain m_leftmost() pointing to min node
    } else {
      z = m_create_node(v);
      s_right(y) = z;
      // Maintain m_rightmost() pointing to max node.
      if (y == m_rightmost())
        m_rightmost() = z;
    }
    s_parent(z) = y;
    s_left(z) = 0;
    s_right(z) = 0;
    rb_tree_rebalance(z, this->m_header.m_parent);
    ++m_node_count;
    return iterator(z);
  }

  link_type m_copy(link_type x, link_type p) {
    // Structural copy.  x and p must be non-null.
    link_type top = m_clone_node(x);
    top->m_parent = p;

    try {
      if (x->m_right)
        top->m_right = m_copy(s_right(x), top);
      p = top;
      x = s_left(x);

      while (x != 0) {
        link_type y = m_clone_node(x);
        p->m_left = y;
        y->m_parent = p;
        if (x->m_right)
          y->m_right = m_copy(s_right(x), y);
        p = y;
        x = s_left(x);
      }
    } catch (...) {
      m_erase(top);
      throw;
    }
    return top;
  }

  void m_erase(link_type x) {
    // Erase without rebalancing.
    while (x != 0) {
      m_erase(s_right(x));
      link_type y = s_left(x);
      destroynode(x);
      x = y;
    }
  }

public:
  // allocation/deallocation
  rb_tree() : Base(allocator_type()), m_node_count(0), m_key_compare() {
    m_emptyinitialize();
  }

  rb_tree(const Compare& comp)
      : Base(allocator_type()), m_node_count(0), m_key_compare(comp) {
    m_emptyinitialize();
  }

  rb_tree(const Compare& comp, const allocator_type& a)
      : Base(a), m_node_count(0), m_key_compare(comp) {
    m_emptyinitialize();
  }

  rb_tree(const rb_tree<Key, Val, KeyOfValue, Compare, Alloc>& x)
      : Base(x.get_allocator()), m_node_count(0), m_key_compare(x.m_key_compare) {
    if (x.m_root() == 0)
      m_emptyinitialize();
    else {
      s_color(&this->m_header) = red;
      m_root() = m_copy(x.m_root(), m_end());
      m_leftmost() = s_minimum(m_root());
      m_rightmost() = s_maximum(m_root());
    }
    m_node_count = x.m_node_count;
  }

  ~rb_tree() { clear(); }

  rb_tree<Key, Val, KeyOfValue, Compare, Alloc>&
  operator=(const rb_tree<Key, Val, KeyOfValue, Compare, Alloc>& x) {
    if (this != &x) {
      // Note that Key may be a constant type.
      clear();
      m_node_count = 0;
      m_key_compare = x.m_key_compare;
      if (x.m_root() == 0) {
        m_root() = 0;
        m_leftmost() = m_end();
        m_rightmost() = m_end();
      } else {
        m_root() = m_copy(x.m_root(), m_end());
        m_leftmost() = s_minimum(m_root());
        m_rightmost() = s_maximum(m_root());
        m_node_count = x.m_node_count;
      }
    }
    return *this;
  }

private:
  void m_emptyinitialize() {
    // Used to distinguish header from root, in iterator.operator++.
    s_color(&this->m_header) = red;
    m_root() = 0;
    m_leftmost() = m_end();
    m_rightmost() = m_end();
  }

public:
  // Accessors.
  Compare key_comp() const { return m_key_compare; }

  iterator begin() { return m_leftmost(); }

  const_iterator begin() const { return m_leftmost(); }

  iterator end() { return &this->m_header; }

  const_iterator end() const { return const_cast<base_ptr>(&this->m_header); }

  reverse_iterator rbegin() { return reverse_iterator(end()); }

  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }

  reverse_iterator rend() { return reverse_iterator(begin()); }

  const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }

  bool empty() const { return m_node_count == 0; }

  size_type size() const { return m_node_count; }

  size_type max_size() const {
    return std::numeric_limits<difference_type>::max();
  }

  void swap(rb_tree<Key, Val, KeyOfValue, Compare, Alloc>& t) {
    if (m_root() == 0) {
      if (t.m_root() != 0) {
        m_root() = t.m_root();
        m_leftmost() = t.m_leftmost();
        m_rightmost() = t.m_rightmost();
        m_root()->m_parent = m_end();

        t.m_root() = 0;
        t.m_leftmost() = t.m_end();
        t.m_rightmost() = t.m_end();
      }
    } else if (t.m_root() == 0) {
      t.m_root() = m_root();
      t.m_leftmost() = m_leftmost();
      t.m_rightmost() = m_rightmost();
      t.m_root()->m_parent = t.m_end();

      m_root() = 0;
      m_leftmost() = m_end();
      m_rightmost() = m_end();
    } else {
      std::swap(m_root(), t.m_root());
      std::swap(m_leftmost(), t.m_leftmost());
      std::swap(m_rightmost(), t.m_rightmost());

      m_root()->m_parent = m_end();
      t.m_root()->m_parent = t.m_end();
    }
    // No need to swap header's color as it does not change.
    std::swap(this->m_node_count, t.m_node_count);
    std::swap(this->m_key_compare, t.m_key_compare);
  }

  // Insert/erase.

  pair<iterator, bool> insert_unique(const value_type& v) {
    link_type y = m_end();
    link_type x = m_root();
    bool      comp = true;
    while (x != 0) {
      y = x;
      comp = m_key_compare(KeyOfValue()(v), s_key(x));
      x = comp ? s_left(x) : s_right(x);
    }
    iterator j = iterator(y);
    if (comp) {
      if (j == begin())
        return pair<iterator, bool>(m_insert(x, y, v), true);
      else
        --j;
    }
    if (m_key_compare(s_key(j.m_node), KeyOfValue()(v)))
      return pair<iterator, bool>(m_insert(x, y, v), true);
    return pair<iterator, bool>(j, false);
  }

  iterator insert_equal(const value_type& v) {
    link_type y = m_end();
    link_type x = m_root();
    while (x != 0) {
      y = x;
      x = m_key_compare(KeyOfValue()(v), s_key(x)) ? s_left(x) : s_right(x);
    }
    return m_insert(x, y, v);
  }

  iterator insert_unique(iterator position, const value_type& v) {
    if (position.m_node == this->m_header.m_left) {
      // begin()
      if (size() > 0 && m_key_compare(KeyOfValue()(v), s_key(position.m_node)))
        return m_insert(position.m_node, position.m_node, v);
      // first argument just needs to be non-null
      else
        return insert_unique(v).first;
    } else if (position.m_node == &this->m_header) {
      // end()
      if (m_key_compare(s_key(m_rightmost()), KeyOfValue()(v)))
        return m_insert(0, m_rightmost(), v);
      else
        return insert_unique(v).first;
    } else {
      iterator before = position;
      --before;
      if (m_key_compare(s_key(before.m_node), KeyOfValue()(v)) &&
          m_key_compare(KeyOfValue()(v), s_key(position.m_node))) {
        if (s_right(before.m_node) == 0)
          return m_insert(0, before.m_node, v);
        else
          return m_insert(position.m_node, position.m_node, v);
        // first argument just needs to be non-null
      } else
        return insert_unique(v).first;
    }
  }

  iterator insert_equal(iterator position, const value_type& v) {
    if (position.m_node == this->m_header.m_left) {
      // begin()
      if (size() > 0 && !m_key_compare(s_key(position.m_node), KeyOfValue()(v)))
        return m_insert(position.m_node, position.m_node, v);
      // first argument just needs to be non-null
      else
        return insert_equal(v);
    } else if (position.m_node == &this->m_header) {
      // end()
      if (!m_key_compare(KeyOfValue()(v), s_key(m_rightmost())))
        return m_insert(0, m_rightmost(), v);
      else
        return insert_equal(v);
    } else {
      iterator before = position;
      --before;
      if (!m_key_compare(KeyOfValue()(v), s_key(before.m_node)) &&
          !m_key_compare(s_key(position.m_node), KeyOfValue()(v))) {
        if (s_right(before.m_node) == 0)
          return m_insert(0, before.m_node, v);
        else
          return m_insert(position.m_node, position.m_node, v);
        // first argument just needs to be non-null
      } else
        return insert_equal(v);
    }
  }

  template <typename _InputIterator>
  void insert_unique(_InputIterator first, _InputIterator last) {
    for (; first != last; ++first)
      insert_unique(*first);
  }

  template <class _InputIterator>
  void insert_equal(_InputIterator first, _InputIterator last) {
    for (; first != last; ++first)
      insert_equal(*first);
  }

  inline void erase(iterator position) {
    link_type y = (link_type)rb_tree_rebalance_for_erase(
        position.m_node, this->m_header.m_parent, this->m_header.m_left,
        this->m_header.m_right);
    destroynode(y);
    --m_node_count;
  }

  size_type erase(const keytype& x) {
    pair<iterator, iterator> p = equal_range(x);
    size_type                n = std::distance(p.first, p.second);
    erase(p.first, p.second);
    return n;
  }

  void erase(iterator first, iterator last) {
    if (first == begin() && last == end())
      clear();
    else
      while (first != last)
        erase(first++);
  }

  void erase(const keytype* first, const keytype* last) {
    while (first != last)
      erase(*first++);
  }

  void clear() {
    if (m_node_count != 0) {
      m_erase(m_root());
      m_leftmost() = m_end();
      m_root() = 0;
      m_rightmost() = m_end();
      m_node_count = 0;
    }
  }

  // Set operations.

  iterator find(const keytype& k) {
    link_type y = m_end();  // Last node which is not less than k.
    link_type x = m_root(); // Current node.

    while (x != 0)
      if (!m_key_compare(s_key(x), k))
        y = x, x = s_left(x);
      else
        x = s_right(x);

    iterator j = iterator(y);
    return (j == end() || m_key_compare(k, s_key(j.m_node))) ? end() : j;
  }

  const_iterator find(const keytype& k) const {
    link_type y = m_end();  // Last node which is not less than k.
    link_type x = m_root(); // Current node.

    while (x != 0) {
      if (!m_key_compare(s_key(x), k))
        y = x, x = s_left(x);
      else
        x = s_right(x);
    }
    const_iterator j = const_iterator(y);
    return (j == end() || m_key_compare(k, s_key(j.m_node))) ? end() : j;
  }

  size_type count(const keytype& k) const {
    pair<const_iterator, const_iterator> p = equal_range(k);
    size_type                            n = std::distance(p.first, p.second);
    return n;
  }

  iterator lower_bound(const keytype& k) {
    link_type y = m_end();  // Last node which is not less than k
    link_type x = m_root(); // Current node.

    while (x != 0)
      if (!m_key_compare(s_key(x), k))
        y = x, x = s_left(x);
      else
        x = s_right(x);

    return iterator(y);
  }

  const_iterator lower_bound(const keytype& k) const {
    link_type y = m_end();  // Last node which is not less than k.
    link_type x = m_root(); // Current node.

    while (x != 0)
      if (!m_key_compare(s_key(x), k))
        y = x, x = s_left(x);
      else
        x = s_right(x);

    return const_iterator(y);
  }

  iterator upper_bound(const keytype& k) {
    link_type y = m_end();  // Last node which is greater than k.
    link_type x = m_root(); // Current node.

    while (x != 0)
      if (m_key_compare(k, s_key(x)))
        y = x, x = s_left(x);
      else
        x = s_right(x);

    return iterator(y);
  }

  const_iterator upper_bound(const keytype& k) const {
    link_type y = m_end();  // Last node which is greater than k.
    link_type x = m_root(); // Current node.

    while (x != 0)
      if (m_key_compare(k, s_key(x)))
        y = x, x = s_left(x);
      else
        x = s_right(x);

    return const_iterator(y);
  }

  inline pair<iterator, iterator> equal_range(const keytype& k) {
    return pair<iterator, iterator>(lower_bound(k), upper_bound(k));
  }

  inline pair<const_iterator, const_iterator>
  equal_range(const keytype& k) const {
    return pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k));
  }

  // Debugging.
  bool _rb_verify() const {
    if (m_node_count == 0 || begin() == end())
      return m_node_count == 0 && begin() == end() &&
             this->m_header.m_left == &this->m_header &&
             this->m_header.m_right == &this->m_header;

    int len = __black_count(m_leftmost(), m_root());
    for (const_iterator it = begin(); it != end(); ++it) {
      link_type x = (link_type)it.m_node;
      link_type l = s_left(x);
      link_type r = s_right(x);

      if (x->m_color == red)
        if ((l && l->m_color == red) || (r && r->m_color == red))
          return false;

      if (l && m_key_compare(s_key(x), s_key(l)))
        return false;
      if (r && m_key_compare(s_key(r), s_key(x)))
        return false;

      if (!l && !r && __black_count(x, m_root()) != len)
        return false;
    }

    if (m_leftmost() != rb_tree_node_base::s_minimum(m_root()))
      return false;
    if (m_rightmost() != rb_tree_node_base::s_maximum(m_root()))
      return false;
    return true;
  }
};

template <typename Key, typename Val, typename KeyOfValue, typename Compare,
          typename Alloc>
inline bool operator==(const rb_tree<Key, Val, KeyOfValue, Compare, Alloc>& x,
                       const rb_tree<Key, Val, KeyOfValue, Compare, Alloc>& y) {
  return x.size() == y.size() && equal(x.begin(), x.end(), y.begin());
}

template <typename Key, typename Val, typename KeyOfValue, typename Compare,
          typename Alloc>
inline bool operator<(const rb_tree<Key, Val, KeyOfValue, Compare, Alloc>& x,
                      const rb_tree<Key, Val, KeyOfValue, Compare, Alloc>& y) {
  return lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

template <typename Key, typename Val, typename KeyOfValue, typename Compare,
          typename Alloc>
inline bool operator!=(const rb_tree<Key, Val, KeyOfValue, Compare, Alloc>& x,
                       const rb_tree<Key, Val, KeyOfValue, Compare, Alloc>& y) {
  return !(x == y);
}

template <typename Key, typename Val, typename KeyOfValue, typename Compare,
          typename Alloc>
inline bool operator>(const rb_tree<Key, Val, KeyOfValue, Compare, Alloc>& x,
                      const rb_tree<Key, Val, KeyOfValue, Compare, Alloc>& y) {
  return y < x;
}

template <typename Key, typename Val, typename KeyOfValue, typename Compare,
          typename Alloc>
inline bool operator<=(const rb_tree<Key, Val, KeyOfValue, Compare, Alloc>& x,
                       const rb_tree<Key, Val, KeyOfValue, Compare, Alloc>& y) {
  return !(y < x);
}

template <typename Key, typename Val, typename KeyOfValue, typename Compare,
          typename Alloc>
inline bool operator>=(const rb_tree<Key, Val, KeyOfValue, Compare, Alloc>& x,
                       const rb_tree<Key, Val, KeyOfValue, Compare, Alloc>& y) {
  return !(x < y);
}

template <typename Key, typename Val, typename KeyOfValue, typename Compare,
          typename Alloc>
inline void swap(rb_tree<Key, Val, KeyOfValue, Compare, Alloc>& x,
                 rb_tree<Key, Val, KeyOfValue, Compare, Alloc>& y) {
  x.swap(y);
}

inline int __black_count(rb_tree_node_base* node, rb_tree_node_base* root) {
  if (node == 0)
    return 0;
  int sum = 0;
  do {
    if (node->m_color == black)
      ++sum;
    if (node == root)
      break;
    node = node->m_parent;
  } while (1);
  return sum;
}

//!@}

} /* namespace ft */

#endif /* _rb_TREE_HPP__ */
