/** @file rb_tree.h
 *  This is an internal header file, included by map.hpp and set.hpp
 *  You should not attempt to use it directly.
 */

#ifndef _RB_TREE_HPP__
#define _RB_TREE_HPP__

#include <memory>
#include "algobase.hpp"
#include "pair.hpp"

namespace ft {

enum rb_tree_color { red = false, black = true };

//!@{ Tree Node ////////////////////////////////////////////////////////////////

struct rb_tree_node_base {
  typedef rb_tree_node_base* base_ptr;

  rb_tree_color color;
  base_ptr      parent;
  base_ptr      left;
  base_ptr      right;

  // 현재 노드를 기준으로 서브트리의 최소값 노드를 찾는다.
  static base_ptr find_minimum(base_ptr x) {
    while (x->left != 0)
      x = x->left;
    return x;
  }

  // 현재 노드를 기준으로 서브트리의 최대값 노드를 찾는다.
  static base_ptr find_maximum(base_ptr x) {
    while (x->right != 0)
      x = x->right;
    return x;
  }
};

// rb_tree_node_base를 상속받아서 value를 저장하는 노드
template <typename Val>
struct rb_tree_node : public rb_tree_node_base {
  typedef rb_tree_node<Val>* link_type;
  Val                        m_value_field;
};

//!@}

//!@{ Tree Iterator ////////////////////////////////////////////////////////////

template <typename Val, typename Ref, typename Ptr>
struct rb_tree_iterator {
  typedef Val                                           value_type;
  typedef Ref                                           reference;
  typedef Ptr                                           pointer;
  typedef rb_tree_iterator<Val, Val&, Val*>             iterator;
  typedef rb_tree_iterator<Val, const Val&, const Val*> const_iterator;
  typedef rb_tree_iterator<Val, Ref, Ptr>               self;
  typedef rb_tree_node<Val>*                            link_type;

  typedef rb_tree_node_base::base_ptr                   base_ptr;
  typedef std::bidirectional_iterator_tag               iterator_category;
  typedef ptrdiff_t                                     difference_type;

  base_ptr                                              current_node;

  // 현재 노드를 기준으로 다음 노드를 찾는다. operator++에 사용된다.
  void increment() {
    // 현재 노드가 오른쪽 자식을 가지고 있으면, 오른쪽 자식의 서브트리에서 최소값 노드를 찾아 현재 노드로 바꾼다.
    if (current_node->right != 0) {
      current_node = current_node->right;
      while (current_node->left != 0)
        current_node = current_node->left;
    } else {
      // 현재 노드가 오른쪽 자식을 가지고 있지 않으면, 현재 노드의 부모 노드를 찾아 현재 노드로 바꾼다.
      base_ptr y = current_node->parent;
      while (current_node == y->right) {
        current_node = y;
        y = y->parent;
      }
      if (current_node->right != y)
        current_node = y;
    }
  }

  // 현재 노드를 기준으로 이전 노드를 찾는다. operator--에 사용된다.
  void decrement() {
    if (current_node->color == red && current_node->parent->parent == current_node)
      current_node = current_node->right;
    else if (current_node->left != 0) {
      base_ptr y = current_node->left;
      while (y->right != 0)
        y = y->right;
      current_node = y;
    } else {
      base_ptr y = current_node->parent;
      while (current_node == y->left) {
        current_node = y;
        y = y->parent;
      }
      current_node = y;
    }
  }

  rb_tree_iterator() { }
  rb_tree_iterator(rb_tree_node_base* x) { current_node = x; }
  rb_tree_iterator(const iterator& it) { current_node = it.current_node; }

  reference operator*() const { return link_type(current_node)->m_value_field; }

  pointer operator->() const { return &(operator*()); }

  self& operator++() {
    increment();
    return *this;
  }

  self operator++(int) {
    self tmp = *this;
    increment();
    return tmp;
  }

  self& operator--() {
    decrement();
    return *this;
  }

  self operator--(int) {
    self tmp = *this;
    decrement();
    return tmp;
  }
};

template <typename Val, typename Ref, typename Ptr>
inline bool operator==(const rb_tree_iterator<Val, Ref, Ptr>& x,
                       const rb_tree_iterator<Val, Ref, Ptr>& y) {
  return x.current_node == y.current_node;
}

template <typename Val>
inline bool operator==(const rb_tree_iterator<Val, const Val&, const Val*>& x,
                       const rb_tree_iterator<Val, Val&, Val*>&             y) {
  return x.current_node == y.current_node;
}

template <typename Val>
inline bool operator==(const rb_tree_iterator<Val, Val&, Val*>&             x,
                       const rb_tree_iterator<Val, const Val&, const Val*>& y) {
  return x.current_node == y.current_node;
}

template <typename Val, typename Ref, typename Ptr>
inline bool operator!=(const rb_tree_iterator<Val, Ref, Ptr>& x,
                       const rb_tree_iterator<Val, Ref, Ptr>& y) {
  return x.current_node != y.current_node;
}

template <typename Val>
inline bool operator!=(const rb_tree_iterator<Val, const Val&, const Val*>& x,
                       const rb_tree_iterator<Val, Val&, Val*>&             y) {
  return x.current_node != y.current_node;
}

template <typename Val>
inline bool operator!=(const rb_tree_iterator<Val, Val&, Val*>&             x,
                       const rb_tree_iterator<Val, const Val&, const Val*>& y) {
  return x.current_node != y.current_node;
}

//!@}

// 트리의 균형을 유지하기 위한 회전 함수

/**
 * @brief 왼쪽 회전
 * @param x 회전의 기준이 되는 노드
 * @param root 트리의 루트 노드
 * 
 * x와 y(노드 x의 오른쪽 자식)의 부모-자식 관계를 바꾼다.
*/
inline void rb_tree_rotate_left(rb_tree_node_base*  x,
                                rb_tree_node_base*& root) {
  rb_tree_node_base* y = x->right;
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

/**
 * @brief 오른쪽 회전
 * @param x 회전의 기준이 되는 노드
 * @param root 트리의 루트 노드
 * 
 * x와 y(노드 x의 왼쪽 자식)의 부모-자식 관계를 바꾼다.
*/
inline void rb_tree_rotate_right(rb_tree_node_base*  x,
                                 rb_tree_node_base*& root) {
  rb_tree_node_base* y = x->left;
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

/**
 * @brief 트리에 노드 삽입 후, 균형을 유지하기 위한 함수
 * @param x 새로 추가된 노드
 * @param root 트리의 루트 노드
*/
inline void rb_tree_rebalance(rb_tree_node_base* x, rb_tree_node_base*& root) {
  x->color = red;
  while (x != root && x->parent->color == red) {
    rb_tree_node_base* x_grandparent = x->parent->parent;

    // x의 부모가 x의 조부모의 왼쪽 자식인 경우
    if (x->parent == x_grandparent->left) {
      rb_tree_node_base* y = x_grandparent->right; // 삼촌 노드

      // 삼촌 노드가 존재하고, 삼촌 노드가 빨간색인 경우
      if (y && y->color == red) {
        x->parent->color = black;
        y->color = black;
        x_grandparent->color = red;
        x = x_grandparent;
      } else {
        if (x == x->parent->right) {
          x = x->parent;
          rb_tree_rotate_left(x, root);
        }
        x->parent->color = black;
        x_grandparent->color = red;
        rb_tree_rotate_right(x_grandparent, root);
      }
    } else { // x의 부모가 x의 조부모의 오른쪽 자식인 경우
      rb_tree_node_base* y = x_grandparent->left;
      if (y && y->color == red) {
        x->parent->color = black;
        y->color = black;
        x_grandparent->color = red;
        x = x_grandparent;
      } else {
        if (x == x->parent->left) {
          x = x->parent;
          rb_tree_rotate_right(x, root);
        }
        x->parent->color = black;
        x_grandparent->color = red;
        rb_tree_rotate_left(x_grandparent, root);
      }
    }
  }
  root->color = black;
}

/**
 * @brief 트리에서 노드 삭제 후, 균형을 유지하기 위한 함수
 * @param z 삭제할 노드
 * @param root 트리의 루트 노드
 * @param leftmost 트리의 가장 왼쪽 노드
 * @param rightmost 트리의 가장 오른쪽 노드
*/
inline rb_tree_node_base*
rb_tree_rebalance_for_erase(rb_tree_node_base* z, rb_tree_node_base*& root,
                            rb_tree_node_base*& leftmost,
                            rb_tree_node_base*& rightmost) {
  rb_tree_node_base* y = z;
  rb_tree_node_base* x = 0;
  rb_tree_node_base* xparent = 0;
  if (y->left == 0)
    x = y->right;
  else if (y->right == 0)
    x = y->left;
  else {
    y = y->right;
    while (y->left != 0)
      y = y->left;
    x = y->right;
  }
  if (y != z) {
    z->left->parent = y;
    y->left = z->left;
    if (y != z->right) {
      xparent = y->parent;
      if (x)
        x->parent = y->parent;
      y->parent->left = x;
      y->right = z->right;
      z->right->parent = y;
    } else
      xparent = y;
    if (root == z)
      root = y;
    else if (z->parent->left == z)
      z->parent->left = y;
    else
      z->parent->right = y;
    y->parent = z->parent;
    ft::swap(y->color, z->color);
    y = z;
  } else { // y == z
    xparent = y->parent;
    if (x)
      x->parent = y->parent;
    if (root == z)
      root = x;
    else if (z->parent->left == z)
      z->parent->left = x;
    else
      z->parent->right = x;
    if (leftmost == z) {
      if (z->right == 0) {
        leftmost = z->parent;
      } else {
        leftmost = rb_tree_node_base::find_minimum(x);
      }
    }
    if (rightmost == z) {
      if (z->left == 0) {
        rightmost = z->parent;
      } else { // x == z->left
        rightmost = rb_tree_node_base::find_maximum(x);
      }
    }
  }
  if (y->color != red) {
    while (x != root && (x == 0 || x->color == black))
      if (x == xparent->left) {
        rb_tree_node_base* w = xparent->right;
        if (w->color == red) {
          w->color = black;
          xparent->color = red;
          rb_tree_rotate_left(xparent, root);
          w = xparent->right;
        }
        if ((w->left == 0 || w->left->color == black) &&
            (w->right == 0 || w->right->color == black)) {
          w->color = red;
          x = xparent;
          xparent = xparent->parent;
        } else {
          if (w->right == 0 || w->right->color == black) {
            w->left->color = black;
            w->color = red;
            rb_tree_rotate_right(w, root);
            w = xparent->right;
          }
          w->color = xparent->color;
          xparent->color = black;
          if (w->right)
            w->right->color = black;
          rb_tree_rotate_left(xparent, root);
          break;
        }
      } else {
        // 위의 if문과 동일한 로직, 좌우 반전의 경우
        rb_tree_node_base* w = xparent->left;
        if (w->color == red) {
          w->color = black;
          xparent->color = red;
          rb_tree_rotate_right(xparent, root);
          w = xparent->left;
        }
        if ((w->right == 0 || w->right->color == black) &&
            (w->left == 0 || w->left->color == black)) {
          w->color = red;
          x = xparent;
          xparent = xparent->parent;
        } else {
          if (w->left == 0 || w->left->color == black) {
            w->right->color = black;
            w->color = red;
            rb_tree_rotate_left(w, root);
            w = xparent->left;
          }
          w->color = xparent->color;
          xparent->color = black;
          if (w->left)
            w->left->color = black;
          rb_tree_rotate_right(xparent, root);
          break;
        }
      }
    if (x)
      x->color = black;
  }
  return y;
}

//!@{ Tree /////////////////////////////////////////////////////////////////////
template <typename Key, typename Val, typename KeyOfValue,
          typename Compare = std::less<Key>,
          typename Alloc = std::allocator<Val> >
class rb_tree {
  typename Alloc::template rebind<rb_tree_node<Val> >::other node_allocator;

protected:
  typedef rb_tree_node_base* base_ptr;
  typedef rb_tree_node<Val>  rb_tree_node;

  rb_tree_node_base  m_header;

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

  typedef typename Alloc::template rebind<Val>::other allocator_type;
  allocator_type get_allocator() const { return node_allocator; }

protected:
  link_type create_node(const value_type& x) {
    link_type tmp = node_allocator.allocate(1);
    try {
      get_allocator().construct(&tmp->m_value_field, x);
    } catch(std::exception& e) {
      node_allocator.deallocate(tmp, 1);
      throw;
    }
    return tmp;
  }

  link_type clone_node(link_type x) {
    link_type tmp = create_node(x->m_value_field);
    tmp->color = x->color;
    tmp->left = 0;
    tmp->right = 0;
    return tmp;
  }

  void destroy_node(link_type p) {
    node_allocator.destroy(p);
    node_allocator.deallocate(p, 1);
  }

  size_type m_node_count;
  Compare   m_key_compare;

  link_type& m_root() const { return (link_type&)this->m_header.parent; }
  link_type& m_leftmost() const { return (link_type&)this->m_header.left; }
  link_type& m_rightmost() const { return (link_type&)this->m_header.right; }
  link_type m_end() const { return (link_type) & this->m_header; }

  static link_type& s_left(link_type x) { return (link_type&)(x->left); }
  static link_type& s_right(link_type x) { return (link_type&)(x->right); }
  static link_type& s_parent(link_type x) { return (link_type&)(x->parent); }
  static reference s_value(link_type x) { return x->m_value_field; }
  static const Key& s_key(link_type x) { return KeyOfValue()(s_value(x)); }
  static link_type& s_left(base_ptr x) { return (link_type&)(x->left); }
  static link_type& s_right(base_ptr x) { return (link_type&)(x->right); }
  static link_type& s_parent(base_ptr x) { return (link_type&)(x->parent); }
  static reference s_value(base_ptr x) { return ((link_type)x)->m_value_field; }

  static const Key& s_key(base_ptr x) {
    return KeyOfValue()(s_value(link_type(x)));
  }

  static rb_tree_color& s_color(base_ptr x) { return x->color; }

  static link_type find_minimum(link_type x) {
    return (link_type)rb_tree_node_base::find_minimum(x);
  }

  static link_type find_maximum(link_type x) {
    return (link_type)rb_tree_node_base::find_maximum(x);
  }

public:
  typedef rb_tree_iterator<value_type, reference, pointer> iterator;
  typedef rb_tree_iterator<value_type, const_reference, const_pointer>
      const_iterator;

  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef std::reverse_iterator<iterator>       reverse_iterator;

private:
  /**
   * @brief 트리에 노드를 삽입한다.
   * @param base_ptr_x 삽입할 위치의 부모 노드
   * @param base_ptr_y 삽입할 위치의 형제 노드
   * @param v 삽입할 값
  */
  iterator m_insert(base_ptr base_ptr_x, base_ptr base_ptr_y,
                    const value_type& v) {
    link_type x = (link_type)base_ptr_x;
    link_type y = (link_type)base_ptr_y;
    link_type z;

    if (y == &m_header || x != 0 ||
        m_key_compare(KeyOfValue()(v), s_key(y))) {
      z = create_node(v);
      s_left(y) = z;
      if (y == &m_header) {
        m_root() = z;
        m_rightmost() = z;
      } else if (y == m_leftmost())
        m_leftmost() = z;
    } else {
      z = create_node(v);
      s_right(y) = z;
      if (y == m_rightmost())
        m_rightmost() = z;
    }
    s_parent(z) = y;
    s_left(z) = 0;
    s_right(z) = 0;
    rb_tree_rebalance(z, this->m_header.parent);
    ++m_node_count;
    return iterator(z);
  }

  /**
   * @brief 구조적 복사를 수행한다.
   * @param x 복사할 노드(non-null여야 함)
   * @param p 부모 노드(non-null여야 함)
  */
  link_type m_copy(link_type x, link_type p) {
    link_type top = clone_node(x);
    top->parent = p;

    try {
      if (x->right)
        top->right = m_copy(s_right(x), top);
      p = top;
      x = s_left(x);

      while (x != 0) {
        link_type y = clone_node(x);
        p->left = y;
        y->parent = p;
        if (x->right)
          y->right = m_copy(s_right(x), y);
        p = y;
        x = s_left(x);
      }
    } catch(std::exception& e) {
      erase_without_rebalancing(top);
      throw;
    }
    return top;
  }

  void erase_without_rebalancing(link_type x) {
    while (x != 0) {
      erase_without_rebalancing(s_right(x));
      link_type y = s_left(x);
      destroy_node(x);
      x = y;
    }
  }

public:
  // allocation/deallocation
  rb_tree()
      : node_allocator(allocator_type()), m_node_count(0), m_key_compare() {
    empty_initialize();
  }

  rb_tree(const Compare& comp)
      : node_allocator(allocator_type()), m_node_count(0), m_key_compare(comp) {
    empty_initialize();
  }

  rb_tree(const Compare& comp, const allocator_type& a)
      : node_allocator(a), m_node_count(0), m_key_compare(comp) {
    empty_initialize();
  }

  rb_tree(const rb_tree<Key, Val, KeyOfValue, Compare, Alloc>& x)
      : node_allocator(x.get_allocator()), m_node_count(0),
        m_key_compare(x.m_key_compare) {
    if (x.m_root() == 0)
      empty_initialize();
    else {
      s_color(&m_header) = red;
      m_root() = m_copy(x.m_root(), m_end());
      m_leftmost() = find_minimum(m_root());
      m_rightmost() = find_maximum(m_root());
    }
    m_node_count = x.m_node_count;
  }

  ~rb_tree() { clear(); }

  rb_tree<Key, Val, KeyOfValue, Compare, Alloc>&
  operator=(const rb_tree<Key, Val, KeyOfValue, Compare, Alloc>& x) {
    if (this != &x) {
      clear();
      m_node_count = 0;
      m_key_compare = x.m_key_compare;
      if (x.m_root() == 0) {
        m_root() = 0;
        m_leftmost() = m_end();
        m_rightmost() = m_end();
      } else {
        m_root() = m_copy(x.m_root(), m_end());
        m_leftmost() = find_minimum(m_root());
        m_rightmost() = find_maximum(m_root());
        m_node_count = x.m_node_count;
      }
    }
    return *this;
  }

private:
  void empty_initialize() {
    s_color(&m_header) = red;
    m_root() = 0;
    m_leftmost() = m_end();
    m_rightmost() = m_end();
  }

public:
  // Accessors.
  iterator begin() { return m_leftmost(); }
  const_iterator begin() const { return m_leftmost(); }
  iterator end() { return &m_header; }
  const_iterator end() const { return const_cast<base_ptr>(&m_header); }
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

  Compare key_comp() const { return m_key_compare; }

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
        m_root()->parent = m_end();

        t.m_root() = 0;
        t.m_leftmost() = t.m_end();
        t.m_rightmost() = t.m_end();
      }
    } else if (t.m_root() == 0) {
      t.m_root() = m_root();
      t.m_leftmost() = m_leftmost();
      t.m_rightmost() = m_rightmost();
      t.m_root()->parent = t.m_end();

      m_root() = 0;
      m_leftmost() = m_end();
      m_rightmost() = m_end();
    } else {
      ft::swap(m_root(), t.m_root());
      ft::swap(m_leftmost(), t.m_leftmost());
      ft::swap(m_rightmost(), t.m_rightmost());

      m_root()->parent = m_end();
      t.m_root()->parent = t.m_end();
    }
    ft::swap(this->m_node_count, t.m_node_count);
    ft::swap(this->m_key_compare, t.m_key_compare);
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
    if (m_key_compare(s_key(j.current_node), KeyOfValue()(v)))
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
    if (position.current_node == this->m_header.left) {
      if (size() > 0 && m_key_compare(KeyOfValue()(v), s_key(position.current_node)))
        return m_insert(position.current_node, position.current_node, v);
      else
        return insert_unique(v).first;
    } else if (position.current_node == &m_header) {
      if (m_key_compare(s_key(m_rightmost()), KeyOfValue()(v)))
        return m_insert(0, m_rightmost(), v);
      else
        return insert_unique(v).first;
    } else {
      iterator before = position;
      --before;
      if (m_key_compare(s_key(before.current_node), KeyOfValue()(v)) &&
          m_key_compare(KeyOfValue()(v), s_key(position.current_node))) {
        if (s_right(before.current_node) == 0)
          return m_insert(0, before.current_node, v);
        else
          return m_insert(position.current_node, position.current_node, v);
      } else
        return insert_unique(v).first;
    }
  }

  iterator insert_equal(iterator position, const value_type& v) {
    if (position.current_node == this->m_header.left) {
      if (size() > 0 && !m_key_compare(s_key(position.current_node), KeyOfValue()(v)))
        return m_insert(position.current_node, position.current_node, v);
      else
        return insert_equal(v);
    } else if (position.current_node == &m_header) {
      if (!m_key_compare(KeyOfValue()(v), s_key(m_rightmost())))
        return m_insert(0, m_rightmost(), v);
      else
        return insert_equal(v);
    } else {
      iterator before = position;
      --before;
      if (!m_key_compare(KeyOfValue()(v), s_key(before.current_node)) &&
          !m_key_compare(s_key(position.current_node), KeyOfValue()(v))) {
        if (s_right(before.current_node) == 0)
          return m_insert(0, before.current_node, v);
        else
          return m_insert(position.current_node, position.current_node, v);
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
        position.current_node, this->m_header.parent, this->m_header.left,
        this->m_header.right);
    destroy_node(y);
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
      erase_without_rebalancing(m_root());
      m_leftmost() = m_end();
      m_root() = 0;
      m_rightmost() = m_end();
      m_node_count = 0;
    }
  }

  // Set operations.

  iterator find(const keytype& k) {
    link_type y = m_end();
    link_type x = m_root();

    while (x != 0)
      if (!m_key_compare(s_key(x), k))
        y = x, x = s_left(x);
      else
        x = s_right(x);

    iterator j = iterator(y);
    return (j == end() || m_key_compare(k, s_key(j.current_node))) ? end() : j;
  }

  const_iterator find(const keytype& k) const {
    link_type y = m_end();
    link_type x = m_root();

    while (x != 0) {
      if (!m_key_compare(s_key(x), k))
        y = x, x = s_left(x);
      else
        x = s_right(x);
    }
    const_iterator j = const_iterator(y);
    return (j == end() || m_key_compare(k, s_key(j.current_node))) ? end() : j;
  }

  size_type count(const keytype& k) const {
    pair<const_iterator, const_iterator> p = equal_range(k);
    size_type                            n = std::distance(p.first, p.second);
    return n;
  }

  iterator lower_bound(const keytype& k) {
    link_type y = m_end();
    link_type x = m_root();

    while (x != 0)
      if (!m_key_compare(s_key(x), k))
        y = x, x = s_left(x);
      else
        x = s_right(x);

    return iterator(y);
  }

  const_iterator lower_bound(const keytype& k) const {
    link_type y = m_end();
    link_type x = m_root();

    while (x != 0)
      if (!m_key_compare(s_key(x), k))
        y = x, x = s_left(x);
      else
        x = s_right(x);

    return const_iterator(y);
  }

  iterator upper_bound(const keytype& k) {
    link_type y = m_end();
    link_type x = m_root();

    while (x != 0)
      if (m_key_compare(k, s_key(x)))
        y = x, x = s_left(x);
      else
        x = s_right(x);

    return iterator(y);
  }

  const_iterator upper_bound(const keytype& k) const {
    link_type y = m_end();
    link_type x = m_root();

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

//!@}

} /* namespace ft */

#endif /* _RB_TREE_HPP__ */
