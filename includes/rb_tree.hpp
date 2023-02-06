/** @file rb_tree.h
 *  This is an internal header file, included by map.hpp and set.hpp
 *  You should not attempt to use it directly.
 */

#ifndef __RB_TREE_HPP__
#define __RB_TREE_HPP__

#include "algobase.hpp"
#include "pair.hpp"
#include <memory>

namespace ft {

enum _Rb_tree_color { _S_red = false, _S_black = true };

struct _Rb_tree_node_base {
  typedef _Rb_tree_node_base* _Base_ptr;

  _Rb_tree_color _M_color;
  _Base_ptr      _M_parent;
  _Base_ptr      _M_left;
  _Base_ptr      _M_right;

  static _Base_ptr _S_minimum(_Base_ptr __x) {
    while (__x->_M_left != 0)
      __x = __x->_M_left;
    return __x;
  }

  static _Base_ptr _S_maximum(_Base_ptr __x) {
    while (__x->_M_right != 0)
      __x = __x->_M_right;
    return __x;
  }
};

template <typename _Val>
struct _Rb_tree_node : public _Rb_tree_node_base {
  typedef _Rb_tree_node<_Val>* _Link_type;
  _Val                         _M_value_field;
};

struct _Rb_tree_base_iterator {
  typedef _Rb_tree_node_base::_Base_ptr   _Base_ptr;
  typedef std::bidirectional_iterator_tag iterator_category;
  typedef ptrdiff_t                       difference_type;

  _Base_ptr _M_node;

  void _M_increment() {
    if (_M_node->_M_right != 0) {
      _M_node = _M_node->_M_right;
      while (_M_node->_M_left != 0)
        _M_node = _M_node->_M_left;
    } else {
      _Base_ptr __y = _M_node->_M_parent;
      while (_M_node == __y->_M_right) {
        _M_node = __y;
        __y = __y->_M_parent;
      }
      if (_M_node->_M_right != __y)
        _M_node = __y;
    }
  }

  void _M_decrement() {
    if (_M_node->_M_color == _S_red && _M_node->_M_parent->_M_parent == _M_node)
      _M_node = _M_node->_M_right;
    else if (_M_node->_M_left != 0) {
      _Base_ptr __y = _M_node->_M_left;
      while (__y->_M_right != 0)
        __y = __y->_M_right;
      _M_node = __y;
    } else {
      _Base_ptr __y = _M_node->_M_parent;
      while (_M_node == __y->_M_left) {
        _M_node = __y;
        __y = __y->_M_parent;
      }
      _M_node = __y;
    }
  }
};

template <typename _Val, typename _Ref, typename _Ptr>
struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
  typedef _Val                                              value_type;
  typedef _Ref                                              reference;
  typedef _Ptr                                              pointer;
  typedef _Rb_tree_iterator<_Val, _Val&, _Val*>             iterator;
  typedef _Rb_tree_iterator<_Val, const _Val&, const _Val*> const_iterator;
  typedef _Rb_tree_iterator<_Val, _Ref, _Ptr>               _Self;
  typedef _Rb_tree_node<_Val>*                              _Link_type;

  _Rb_tree_iterator() { }
  _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
  _Rb_tree_iterator(const iterator& __it) { _M_node = __it._M_node; }

  reference operator*() const { return _Link_type(_M_node)->_M_value_field; }

  pointer operator->() const { return &(operator*()); }

  _Self& operator++() {
    _M_increment();
    return *this;
  }

  _Self operator++(int) {
    _Self __tmp = *this;
    _M_increment();
    return __tmp;
  }

  _Self& operator--() {
    _M_decrement();
    return *this;
  }

  _Self operator--(int) {
    _Self __tmp = *this;
    _M_decrement();
    return __tmp;
  }
};

template <typename _Val, typename _Ref, typename _Ptr>
inline bool operator==(const _Rb_tree_iterator<_Val, _Ref, _Ptr>& __x,
                       const _Rb_tree_iterator<_Val, _Ref, _Ptr>& __y) {
  return __x._M_node == __y._M_node;
}

template <typename _Val>
inline bool
operator==(const _Rb_tree_iterator<_Val, const _Val&, const _Val*>& __x,
           const _Rb_tree_iterator<_Val, _Val&, _Val*>&             __y) {
  return __x._M_node == __y._M_node;
}

template <typename _Val>
inline bool
operator==(const _Rb_tree_iterator<_Val, _Val&, _Val*>&             __x,
           const _Rb_tree_iterator<_Val, const _Val&, const _Val*>& __y) {
  return __x._M_node == __y._M_node;
}

template <typename _Val, typename _Ref, typename _Ptr>
inline bool operator!=(const _Rb_tree_iterator<_Val, _Ref, _Ptr>& __x,
                       const _Rb_tree_iterator<_Val, _Ref, _Ptr>& __y) {
  return __x._M_node != __y._M_node;
}

template <typename _Val>
inline bool
operator!=(const _Rb_tree_iterator<_Val, const _Val&, const _Val*>& __x,
           const _Rb_tree_iterator<_Val, _Val&, _Val*>&             __y) {
  return __x._M_node != __y._M_node;
}

template <typename _Val>
inline bool
operator!=(const _Rb_tree_iterator<_Val, _Val&, _Val*>&             __x,
           const _Rb_tree_iterator<_Val, const _Val&, const _Val*>& __y) {
  return __x._M_node != __y._M_node;
}

inline void _Rb_tree_rotate_left(_Rb_tree_node_base*  __x,
                                 _Rb_tree_node_base*& __root) {
  _Rb_tree_node_base* __y = __x->_M_right;
  __x->_M_right = __y->_M_left;
  if (__y->_M_left != 0)
    __y->_M_left->_M_parent = __x;
  __y->_M_parent = __x->_M_parent;

  if (__x == __root)
    __root = __y;
  else if (__x == __x->_M_parent->_M_left)
    __x->_M_parent->_M_left = __y;
  else
    __x->_M_parent->_M_right = __y;
  __y->_M_left = __x;
  __x->_M_parent = __y;
}

inline void _Rb_tree_rotate_right(_Rb_tree_node_base*  __x,
                                  _Rb_tree_node_base*& __root) {
  _Rb_tree_node_base* __y = __x->_M_left;
  __x->_M_left = __y->_M_right;
  if (__y->_M_right != 0)
    __y->_M_right->_M_parent = __x;
  __y->_M_parent = __x->_M_parent;

  if (__x == __root)
    __root = __y;
  else if (__x == __x->_M_parent->_M_right)
    __x->_M_parent->_M_right = __y;
  else
    __x->_M_parent->_M_left = __y;
  __y->_M_right = __x;
  __x->_M_parent = __y;
}

inline void _Rb_tree_rebalance(_Rb_tree_node_base*  __x,
                               _Rb_tree_node_base*& __root) {
  __x->_M_color = _S_red;
  while (__x != __root && __x->_M_parent->_M_color == _S_red) {
    if (__x->_M_parent == __x->_M_parent->_M_parent->_M_left) {
      _Rb_tree_node_base* __y = __x->_M_parent->_M_parent->_M_right;
      if (__y && __y->_M_color == _S_red) {
        __x->_M_parent->_M_color = _S_black;
        __y->_M_color = _S_black;
        __x->_M_parent->_M_parent->_M_color = _S_red;
        __x = __x->_M_parent->_M_parent;
      } else {
        if (__x == __x->_M_parent->_M_right) {
          __x = __x->_M_parent;
          _Rb_tree_rotate_left(__x, __root);
        }
        __x->_M_parent->_M_color = _S_black;
        __x->_M_parent->_M_parent->_M_color = _S_red;
        _Rb_tree_rotate_right(__x->_M_parent->_M_parent, __root);
      }
    } else {
      _Rb_tree_node_base* __y = __x->_M_parent->_M_parent->_M_left;
      if (__y && __y->_M_color == _S_red) {
        __x->_M_parent->_M_color = _S_black;
        __y->_M_color = _S_black;
        __x->_M_parent->_M_parent->_M_color = _S_red;
        __x = __x->_M_parent->_M_parent;
      } else {
        if (__x == __x->_M_parent->_M_left) {
          __x = __x->_M_parent;
          _Rb_tree_rotate_right(__x, __root);
        }
        __x->_M_parent->_M_color = _S_black;
        __x->_M_parent->_M_parent->_M_color = _S_red;
        _Rb_tree_rotate_left(__x->_M_parent->_M_parent, __root);
      }
    }
  }
  __root->_M_color = _S_black;
}

inline _Rb_tree_node_base* _Rb_tree_rebalance_for_erase(
    _Rb_tree_node_base* __z, _Rb_tree_node_base*& __root,
    _Rb_tree_node_base*& __leftmost, _Rb_tree_node_base*& __rightmost) {
  _Rb_tree_node_base* __y = __z;
  _Rb_tree_node_base* __x = 0;
  _Rb_tree_node_base* __x_parent = 0;
  if (__y->_M_left == 0)       // __z has at most one non-null child. y == z.
    __x = __y->_M_right;       // __x might be null.
  else if (__y->_M_right == 0) // __z has exactly one non-null child. y == z.
    __x = __y->_M_left;        // __x is not null.
  else {
    // __z has two non-null children.  Set __y to
    __y = __y->_M_right; //   __z's successor.  __x might be null.
    while (__y->_M_left != 0)
      __y = __y->_M_left;
    __x = __y->_M_right;
  }
  if (__y != __z) {
    // relink y in place of z.  y is z's successor
    __z->_M_left->_M_parent = __y;
    __y->_M_left = __z->_M_left;
    if (__y != __z->_M_right) {
      __x_parent = __y->_M_parent;
      if (__x)
        __x->_M_parent = __y->_M_parent;
      __y->_M_parent->_M_left = __x; // __y must be a child of _M_left
      __y->_M_right = __z->_M_right;
      __z->_M_right->_M_parent = __y;
    } else
      __x_parent = __y;
    if (__root == __z)
      __root = __y;
    else if (__z->_M_parent->_M_left == __z)
      __z->_M_parent->_M_left = __y;
    else
      __z->_M_parent->_M_right = __y;
    __y->_M_parent = __z->_M_parent;
    ft::swap(__y->_M_color, __z->_M_color);
    __y = __z;
    // __y now points to node to be actually deleted
  } else { // __y == __z
    __x_parent = __y->_M_parent;
    if (__x)
      __x->_M_parent = __y->_M_parent;
    if (__root == __z)
      __root = __x;
    else if (__z->_M_parent->_M_left == __z)
      __z->_M_parent->_M_left = __x;
    else
      __z->_M_parent->_M_right = __x;
    if (__leftmost == __z) {
      if (__z->_M_right == 0) { // __z->_M_left must be null also
        __leftmost = __z->_M_parent;
        // makes __leftmost == _M_header if __z == __root
      } else {
        __leftmost = _Rb_tree_node_base::_S_minimum(__x);
      }
    }
    if (__rightmost == __z) {
      if (__z->_M_left == 0) { // __z->_M_right must be null also
        __rightmost = __z->_M_parent;
        // makes __rightmost == _M_header if __z == __root
      } else { // __x == __z->_M_left
        __rightmost = _Rb_tree_node_base::_S_maximum(__x);
      }
    }
  }
  if (__y->_M_color != _S_red) {
    while (__x != __root && (__x == 0 || __x->_M_color == _S_black))
      if (__x == __x_parent->_M_left) {
        _Rb_tree_node_base* __w = __x_parent->_M_right;
        if (__w->_M_color == _S_red) {
          __w->_M_color = _S_black;
          __x_parent->_M_color = _S_red;
          _Rb_tree_rotate_left(__x_parent, __root);
          __w = __x_parent->_M_right;
        }
        if ((__w->_M_left == 0 || __w->_M_left->_M_color == _S_black) &&
            (__w->_M_right == 0 || __w->_M_right->_M_color == _S_black)) {
          __w->_M_color = _S_red;
          __x = __x_parent;
          __x_parent = __x_parent->_M_parent;
        } else {
          if (__w->_M_right == 0 || __w->_M_right->_M_color == _S_black) {
            __w->_M_left->_M_color = _S_black;
            __w->_M_color = _S_red;
            _Rb_tree_rotate_right(__w, __root);
            __w = __x_parent->_M_right;
          }
          __w->_M_color = __x_parent->_M_color;
          __x_parent->_M_color = _S_black;
          if (__w->_M_right)
            __w->_M_right->_M_color = _S_black;
          _Rb_tree_rotate_left(__x_parent, __root);
          break;
        }
      } else {
        // same as above, with _M_right <-> _M_left.
        _Rb_tree_node_base* __w = __x_parent->_M_left;
        if (__w->_M_color == _S_red) {
          __w->_M_color = _S_black;
          __x_parent->_M_color = _S_red;
          _Rb_tree_rotate_right(__x_parent, __root);
          __w = __x_parent->_M_left;
        }
        if ((__w->_M_right == 0 || __w->_M_right->_M_color == _S_black) &&
            (__w->_M_left == 0 || __w->_M_left->_M_color == _S_black)) {
          __w->_M_color = _S_red;
          __x = __x_parent;
          __x_parent = __x_parent->_M_parent;
        } else {
          if (__w->_M_left == 0 || __w->_M_left->_M_color == _S_black) {
            __w->_M_right->_M_color = _S_black;
            __w->_M_color = _S_red;
            _Rb_tree_rotate_left(__w, __root);
            __w = __x_parent->_M_left;
          }
          __w->_M_color = __x_parent->_M_color;
          __x_parent->_M_color = _S_black;
          if (__w->_M_left)
            __w->_M_left->_M_color = _S_black;
          _Rb_tree_rotate_right(__x_parent, __root);
          break;
        }
      }
    if (__x)
      __x->_M_color = _S_black;
  }
  return __y;
}

// Base class to encapsulate the differences between old SGI-style
// allocators and standard-conforming allocators.  In order to avoid
// having an empty base class, we arbitrarily move one of rb_tree's
// data members into the base class.

// _Base for general standard-conforming allocators.
template <typename _Tp, typename _Alloc>
class _Rb_tree_base {
public:
  typedef typename _Alloc::template rebind<_Tp>::other allocator_type;

  allocator_type get_allocator() const { return node_allocator; }

  _Rb_tree_base(const allocator_type& __a) : node_allocator(__a) { }

protected:
  typename _Alloc::template rebind<_Rb_tree_node<_Tp> >::other node_allocator;

  _Rb_tree_node_base _M_header;

  _Rb_tree_node<_Tp>* _M_get_node() { return node_allocator.allocate(1); }

  void _M_put_node(_Rb_tree_node<_Tp>* __p) {
    node_allocator.deallocate(__p, 1);
  }

  void _M_destroy_node(_Rb_tree_node<_Tp> *__p) {
    node_allocator.destroy(__p);
  }
};

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare= std::less<_Key>,
          typename _Alloc = std::allocator<_Val> >
class _Rb_tree : protected _Rb_tree_base<_Val, _Alloc> {
  typedef _Rb_tree_base<_Val, _Alloc> _Base;

protected:
  typedef _Rb_tree_node_base* _Base_ptr;
  typedef _Rb_tree_node<_Val> _Rb_tree_node;

public:
  typedef _Key              key_type;
  typedef _Val              value_type;
  typedef value_type*       pointer;
  typedef const value_type* const_pointer;
  typedef value_type&       reference;
  typedef const value_type& const_reference;
  typedef _Rb_tree_node*    _Link_type;
  typedef size_t            size_type;
  typedef ptrdiff_t         difference_type;

  typedef typename _Base::allocator_type allocator_type;
  allocator_type get_allocator() const { return _Base::get_allocator(); }

protected:
  using _Base::_M_get_node;
  using _Base::_M_header;
  using _Base::_M_put_node;
  using _Base::_M_destroy_node;

  _Link_type _M_create_node(const value_type& __x) {
    _Link_type __tmp = _M_get_node();
    try {
      get_allocator().construct(&__tmp->_M_value_field, __x);
    } catch (...) {
      _M_put_node(__tmp);
      throw;
    }
    return __tmp;
  }

  _Link_type _M_clone_node(_Link_type __x) {
    _Link_type __tmp = _M_create_node(__x->_M_value_field);
    __tmp->_M_color = __x->_M_color;
    __tmp->_M_left = 0;
    __tmp->_M_right = 0;
    return __tmp;
  }

  void destroy_node(_Link_type p) {
    _M_destroy_node(p);
    _M_put_node(p);
  }

  size_type _M_node_count; // keeps track of size of tree
  _Compare  _M_key_compare;

  _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_parent; }

  _Link_type& _M_leftmost() const {
    return (_Link_type&)this->_M_header._M_left;
  }

  _Link_type& _M_rightmost() const {
    return (_Link_type&)this->_M_header._M_right;
  }

  _Link_type _M_end() const { return (_Link_type) & this->_M_header; }

  static _Link_type& _S_left(_Link_type __x) {
    return (_Link_type&)(__x->_M_left);
  }

  static _Link_type& _S_right(_Link_type __x) {
    return (_Link_type&)(__x->_M_right);
  }

  static _Link_type& _S_parent(_Link_type __x) {
    return (_Link_type&)(__x->_M_parent);
  }

  static reference _S_value(_Link_type __x) { return __x->_M_value_field; }

  static const _Key& _S_key(_Link_type __x) {
    return _KeyOfValue()(_S_value(__x));
  }

  static _Link_type& _S_left(_Base_ptr __x) {
    return (_Link_type&)(__x->_M_left);
  }

  static _Link_type& _S_right(_Base_ptr __x) {
    return (_Link_type&)(__x->_M_right);
  }

  static _Link_type& _S_parent(_Base_ptr __x) {
    return (_Link_type&)(__x->_M_parent);
  }

  static reference _S_value(_Base_ptr __x) {
    return ((_Link_type)__x)->_M_value_field;
  }

  static const _Key& _S_key(_Base_ptr __x) {
    return _KeyOfValue()(_S_value(_Link_type(__x)));
  }

  static _Rb_tree_color& _S_color(_Base_ptr __x) { return __x->_M_color; }

  static _Link_type _S_minimum(_Link_type __x) {
    return (_Link_type)_Rb_tree_node_base::_S_minimum(__x);
  }

  static _Link_type _S_maximum(_Link_type __x) {
    return (_Link_type)_Rb_tree_node_base::_S_maximum(__x);
  }

public:
  typedef _Rb_tree_iterator<value_type, reference, pointer> iterator;
  typedef _Rb_tree_iterator<value_type, const_reference, const_pointer>
      const_iterator;

  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef std::reverse_iterator<iterator>       reverse_iterator;

private:
  iterator _M_insert(_Base_ptr __x_, _Base_ptr __y_, const value_type& __v) {
    _Link_type __x = (_Link_type)__x_;
    _Link_type __y = (_Link_type)__y_;
    _Link_type __z;

    if (__y == &this->_M_header || __x != 0 ||
        _M_key_compare(_KeyOfValue()(__v), _S_key(__y))) {
      __z = _M_create_node(__v);
      _S_left(__y) = __z; // also makes _M_leftmost() = __z
      //    when __y == &_M_header
      if (__y == &this->_M_header) {
        _M_root() = __z;
        _M_rightmost() = __z;
      } else if (__y == _M_leftmost())
        _M_leftmost() = __z; // maintain _M_leftmost() pointing to min node
    } else {
      __z = _M_create_node(__v);
      _S_right(__y) = __z;
      // Maintain _M_rightmost() pointing to max node.
      if (__y == _M_rightmost())
        _M_rightmost() = __z;
    }
    _S_parent(__z) = __y;
    _S_left(__z) = 0;
    _S_right(__z) = 0;
    _Rb_tree_rebalance(__z, this->_M_header._M_parent);
    ++_M_node_count;
    return iterator(__z);
  }

  _Link_type _M_copy(_Link_type __x, _Link_type __p) {
    // Structural copy.  __x and __p must be non-null.
    _Link_type __top = _M_clone_node(__x);
    __top->_M_parent = __p;

    try {
      if (__x->_M_right)
        __top->_M_right = _M_copy(_S_right(__x), __top);
      __p = __top;
      __x = _S_left(__x);

      while (__x != 0) {
        _Link_type __y = _M_clone_node(__x);
        __p->_M_left = __y;
        __y->_M_parent = __p;
        if (__x->_M_right)
          __y->_M_right = _M_copy(_S_right(__x), __y);
        __p = __y;
        __x = _S_left(__x);
      }
    } catch (...) {
      _M_erase(__top);
      throw;
    }
    return __top;
  }

  void _M_erase(_Link_type __x) {
    // Erase without rebalancing.
    while (__x != 0) {
      _M_erase(_S_right(__x));
      _Link_type __y = _S_left(__x);
      destroy_node(__x);
      __x = __y;
    }
  }

public:
  // allocation/deallocation
  _Rb_tree() : _Base(allocator_type()), _M_node_count(0), _M_key_compare() {
    _M_empty_initialize();
  }

  _Rb_tree(const _Compare& __comp)
      : _Base(allocator_type()), _M_node_count(0), _M_key_compare(__comp) {
    _M_empty_initialize();
  }

  _Rb_tree(const _Compare& __comp, const allocator_type& __a)
      : _Base(__a), _M_node_count(0), _M_key_compare(__comp) {
    _M_empty_initialize();
  }

  _Rb_tree(const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __x)
      : _Base(__x.get_allocator()), _M_node_count(0),
        _M_key_compare(__x._M_key_compare) {
    if (__x._M_root() == 0)
      _M_empty_initialize();
    else {
      _S_color(&this->_M_header) = _S_red;
      _M_root() = _M_copy(__x._M_root(), _M_end());
      _M_leftmost() = _S_minimum(_M_root());
      _M_rightmost() = _S_maximum(_M_root());
    }
    _M_node_count = __x._M_node_count;
  }

  ~_Rb_tree() { clear(); }

  _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>&
  operator=(const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __x) {
    if (this != &__x) {
      // Note that _Key may be a constant type.
      clear();
      _M_node_count = 0;
      _M_key_compare = __x._M_key_compare;
      if (__x._M_root() == 0) {
        _M_root() = 0;
        _M_leftmost() = _M_end();
        _M_rightmost() = _M_end();
      } else {
        _M_root() = _M_copy(__x._M_root(), _M_end());
        _M_leftmost() = _S_minimum(_M_root());
        _M_rightmost() = _S_maximum(_M_root());
        _M_node_count = __x._M_node_count;
      }
    }
    return *this;
  }

private:
  void _M_empty_initialize() {
    // Used to distinguish header from __root, in iterator.operator++.
    _S_color(&this->_M_header) = _S_red;
    _M_root() = 0;
    _M_leftmost() = _M_end();
    _M_rightmost() = _M_end();
  }

public:
  // Accessors.
  _Compare key_comp() const { return _M_key_compare; }

  iterator begin() { return _M_leftmost(); }

  const_iterator begin() const { return _M_leftmost(); }

  iterator end() { return &this->_M_header; }

  const_iterator end() const { return const_cast<_Base_ptr>(&this->_M_header); }

  reverse_iterator rbegin() { return reverse_iterator(end()); }

  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }

  reverse_iterator rend() { return reverse_iterator(begin()); }

  const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }

  bool empty() const { return _M_node_count == 0; }

  size_type size() const { return _M_node_count; }

  size_type max_size() const { return size_type(-1); }

  void swap(_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __t) {
    if (_M_root() == 0) {
      if (__t._M_root() != 0) {
        _M_root() = __t._M_root();
        _M_leftmost() = __t._M_leftmost();
        _M_rightmost() = __t._M_rightmost();
        _M_root()->_M_parent = _M_end();

        __t._M_root() = 0;
        __t._M_leftmost() = __t._M_end();
        __t._M_rightmost() = __t._M_end();
      }
    } else if (__t._M_root() == 0) {
      __t._M_root() = _M_root();
      __t._M_leftmost() = _M_leftmost();
      __t._M_rightmost() = _M_rightmost();
      __t._M_root()->_M_parent = __t._M_end();

      _M_root() = 0;
      _M_leftmost() = _M_end();
      _M_rightmost() = _M_end();
    } else {
      std::swap(_M_root(), __t._M_root());
      std::swap(_M_leftmost(), __t._M_leftmost());
      std::swap(_M_rightmost(), __t._M_rightmost());

      _M_root()->_M_parent = _M_end();
      __t._M_root()->_M_parent = __t._M_end();
    }
    // No need to swap header's color as it does not change.
    std::swap(this->_M_node_count, __t._M_node_count);
    std::swap(this->_M_key_compare, __t._M_key_compare);
  }

  // Insert/erase.

  pair<iterator, bool> insert_unique(const value_type& __v) {
    _Link_type __y = _M_end();
    _Link_type __x = _M_root();
    bool       __comp = true;
    while (__x != 0) {
      __y = __x;
      __comp = _M_key_compare(_KeyOfValue()(__v), _S_key(__x));
      __x = __comp ? _S_left(__x) : _S_right(__x);
    }
    iterator __j = iterator(__y);
    if (__comp) {
      if (__j == begin())
        return pair<iterator, bool>(_M_insert(__x, __y, __v), true);
      else
        --__j;
    }
    if (_M_key_compare(_S_key(__j._M_node), _KeyOfValue()(__v)))
      return pair<iterator, bool>(_M_insert(__x, __y, __v), true);
    return pair<iterator, bool>(__j, false);
  }

  iterator insert_equal(const value_type& __v) {
    _Link_type __y = _M_end();
    _Link_type __x = _M_root();
    while (__x != 0) {
      __y = __x;
      __x = _M_key_compare(_KeyOfValue()(__v), _S_key(__x)) ? _S_left(__x)
                                                            : _S_right(__x);
    }
    return _M_insert(__x, __y, __v);
  }

  iterator insert_unique(iterator __position, const value_type& __v) {
    if (__position._M_node == this->_M_header._M_left) {
      // begin()
      if (size() > 0 &&
          _M_key_compare(_KeyOfValue()(__v), _S_key(__position._M_node)))
        return _M_insert(__position._M_node, __position._M_node, __v);
      // first argument just needs to be non-null
      else
        return insert_unique(__v).first;
    } else if (__position._M_node == &this->_M_header) {
      // end()
      if (_M_key_compare(_S_key(_M_rightmost()), _KeyOfValue()(__v)))
        return _M_insert(0, _M_rightmost(), __v);
      else
        return insert_unique(__v).first;
    } else {
      iterator __before = __position;
      --__before;
      if (_M_key_compare(_S_key(__before._M_node), _KeyOfValue()(__v)) &&
          _M_key_compare(_KeyOfValue()(__v), _S_key(__position._M_node))) {
        if (_S_right(__before._M_node) == 0)
          return _M_insert(0, __before._M_node, __v);
        else
          return _M_insert(__position._M_node, __position._M_node, __v);
        // first argument just needs to be non-null
      } else
        return insert_unique(__v).first;
    }
  }

  iterator insert_equal(iterator __position, const value_type& __v) {
    if (__position._M_node == this->_M_header._M_left) {
      // begin()
      if (size() > 0 &&
          !_M_key_compare(_S_key(__position._M_node), _KeyOfValue()(__v)))
        return _M_insert(__position._M_node, __position._M_node, __v);
      // first argument just needs to be non-null
      else
        return insert_equal(__v);
    } else if (__position._M_node == &this->_M_header) {
      // end()
      if (!_M_key_compare(_KeyOfValue()(__v), _S_key(_M_rightmost())))
        return _M_insert(0, _M_rightmost(), __v);
      else
        return insert_equal(__v);
    } else {
      iterator __before = __position;
      --__before;
      if (!_M_key_compare(_KeyOfValue()(__v), _S_key(__before._M_node)) &&
          !_M_key_compare(_S_key(__position._M_node), _KeyOfValue()(__v))) {
        if (_S_right(__before._M_node) == 0)
          return _M_insert(0, __before._M_node, __v);
        else
          return _M_insert(__position._M_node, __position._M_node, __v);
        // first argument just needs to be non-null
      } else
        return insert_equal(__v);
    }
  }

  template <typename _InputIterator>
  void insert_unique(_InputIterator __first, _InputIterator __last) {
    for (; __first != __last; ++__first)
      insert_unique(*__first);
  }

  template <class _InputIterator>
  void insert_equal(_InputIterator __first, _InputIterator __last) {
    for (; __first != __last; ++__first)
      insert_equal(*__first);
  }

  inline void erase(iterator __position) {
    _Link_type __y = (_Link_type)_Rb_tree_rebalance_for_erase(
        __position._M_node, this->_M_header._M_parent, this->_M_header._M_left,
        this->_M_header._M_right);
    destroy_node(__y);
    --_M_node_count;
  }

  size_type erase(const key_type& __x) {
    pair<iterator, iterator> __p = equal_range(__x);
    size_type                __n = std::distance(__p.first, __p.second);
    erase(__p.first, __p.second);
    return __n;
  }

  void erase(iterator __first, iterator __last) {
    if (__first == begin() && __last == end())
      clear();
    else
      while (__first != __last)
        erase(__first++);
  }

  void erase(const key_type* __first, const key_type* __last) {
    while (__first != __last)
      erase(*__first++);
  }

  void clear() {
    if (_M_node_count != 0) {
      _M_erase(_M_root());
      _M_leftmost() = _M_end();
      _M_root() = 0;
      _M_rightmost() = _M_end();
      _M_node_count = 0;
    }
  }

  // Set operations.

  iterator find(const key_type& __k) {
    _Link_type __y = _M_end();  // Last node which is not less than __k.
    _Link_type __x = _M_root(); // Current node.

    while (__x != 0)
      if (!_M_key_compare(_S_key(__x), __k))
        __y = __x, __x = _S_left(__x);
      else
        __x = _S_right(__x);

    iterator __j = iterator(__y);
    return (__j == end() || _M_key_compare(__k, _S_key(__j._M_node))) ? end()
                                                                      : __j;
  }

  const_iterator find(const key_type& __k) const {
    _Link_type __y = _M_end();  // Last node which is not less than __k.
    _Link_type __x = _M_root(); // Current node.

    while (__x != 0) {
      if (!_M_key_compare(_S_key(__x), __k))
        __y = __x, __x = _S_left(__x);
      else
        __x = _S_right(__x);
    }
    const_iterator __j = const_iterator(__y);
    return (__j == end() || _M_key_compare(__k, _S_key(__j._M_node))) ? end()
                                                                      : __j;
  }

  size_type count(const key_type& __k) const {
    pair<const_iterator, const_iterator> __p = equal_range(__k);
    size_type __n = std::distance(__p.first, __p.second);
    return __n;
  }

  iterator lower_bound(const key_type& __k) {
    _Link_type __y = _M_end();  // Last node which is not less than __k
    _Link_type __x = _M_root(); // Current node.

    while (__x != 0)
      if (!_M_key_compare(_S_key(__x), __k))
        __y = __x, __x = _S_left(__x);
      else
        __x = _S_right(__x);

    return iterator(__y);
  }

  const_iterator lower_bound(const key_type& __k) const {
    _Link_type __y = _M_end();  // Last node which is not less than __k.
    _Link_type __x = _M_root(); // Current node.

    while (__x != 0)
      if (!_M_key_compare(_S_key(__x), __k))
        __y = __x, __x = _S_left(__x);
      else
        __x = _S_right(__x);

    return const_iterator(__y);
  }

  iterator upper_bound(const key_type& __k) {
    _Link_type __y = _M_end();  // Last node which is greater than __k.
    _Link_type __x = _M_root(); // Current node.

    while (__x != 0)
      if (_M_key_compare(__k, _S_key(__x)))
        __y = __x, __x = _S_left(__x);
      else
        __x = _S_right(__x);

    return iterator(__y);
  }

  const_iterator upper_bound(const key_type& __k) const {
    _Link_type __y = _M_end();  // Last node which is greater than __k.
    _Link_type __x = _M_root(); // Current node.

    while (__x != 0)
      if (_M_key_compare(__k, _S_key(__x)))
        __y = __x, __x = _S_left(__x);
      else
        __x = _S_right(__x);

    return const_iterator(__y);
  }

  inline pair<iterator, iterator> equal_range(const key_type& __k) {
    return pair<iterator, iterator>(lower_bound(__k), upper_bound(__k));
  }

  inline pair<const_iterator, const_iterator>
  equal_range(const key_type& __k) const {
    return pair<const_iterator, const_iterator>(lower_bound(__k),
                                                upper_bound(__k));
  }

  // Debugging.
  bool __rb_verify() const {
    if (_M_node_count == 0 || begin() == end())
      return _M_node_count == 0 && begin() == end() &&
             this->_M_header._M_left == &this->_M_header &&
             this->_M_header._M_right == &this->_M_header;

    int __len = __black_count(_M_leftmost(), _M_root());
    for (const_iterator __it = begin(); __it != end(); ++__it) {
      _Link_type __x = (_Link_type)__it._M_node;
      _Link_type __L = _S_left(__x);
      _Link_type __R = _S_right(__x);

      if (__x->_M_color == _S_red)
        if ((__L && __L->_M_color == _S_red) ||
            (__R && __R->_M_color == _S_red))
          return false;

      if (__L && _M_key_compare(_S_key(__x), _S_key(__L)))
        return false;
      if (__R && _M_key_compare(_S_key(__R), _S_key(__x)))
        return false;

      if (!__L && !__R && __black_count(__x, _M_root()) != __len)
        return false;
    }

    if (_M_leftmost() != _Rb_tree_node_base::_S_minimum(_M_root()))
      return false;
    if (_M_rightmost() != _Rb_tree_node_base::_S_maximum(_M_root()))
      return false;
    return true;
  }
};

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
inline bool
operator==(const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __x,
           const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __y) {
  return __x.size() == __y.size() && equal(__x.begin(), __x.end(), __y.begin());
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
inline bool
operator<(const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __x,
          const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __y) {
  return lexicographical_compare(__x.begin(), __x.end(), __y.begin(),
                                 __y.end());
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
inline bool
operator!=(const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __x,
           const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __y) {
  return !(__x == __y);
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
inline bool
operator>(const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __x,
          const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __y) {
  return __y < __x;
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
inline bool
operator<=(const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __x,
           const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __y) {
  return !(__y < __x);
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
inline bool
operator>=(const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __x,
           const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __y) {
  return !(__x < __y);
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
inline void swap(_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __x,
                 _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __y) {
  __x.swap(__y);
}

inline int __black_count(_Rb_tree_node_base* __node,
                         _Rb_tree_node_base* __root) {
  if (__node == 0)
    return 0;
  int __sum = 0;
  do {
    if (__node->_M_color == _S_black)
      ++__sum;
    if (__node == __root)
      break;
    __node = __node->_M_parent;
  } while (1);
  return __sum;
}

} /* namespace ft */

#endif /* __RB_TREE_HPP__ */
