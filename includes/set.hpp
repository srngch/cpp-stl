#ifndef __SET_HPP__
#define __SET_HPP__

#include <memory>
#include "rb_tree.hpp"
#include "function.hpp"
#include "pair.hpp"

namespace ft
{

template <typename Key, typename Compare = std::less<Key>,
          typename Alloc = std::allocator<Key> >
class set {

public:
  typedef Key                                       key_type;
  typedef Key                                       value_type;
  typedef Compare                                   key_compare;
  typedef Compare                                   value_compare;

private:
  typedef rb_tree<key_type, value_type, _Identity<value_type>,
                   key_compare, Alloc>              rep_type;

  rep_type _tree;

public:
  typedef typename rep_type::allocator_type         allocator_type;
  typedef typename rep_type::size_type              size_type;
  typedef typename rep_type::difference_type        difference_type;

  typedef typename rep_type::const_reference        reference;
  typedef typename rep_type::const_reference        const_reference;
  typedef typename rep_type::const_pointer          pointer;
  typedef typename rep_type::const_pointer          const_pointer;

  typedef typename rep_type::const_iterator         iterator;
  typedef typename rep_type::const_iterator         const_iterator;
  typedef typename rep_type::const_reverse_iterator reverse_iterator;
  typedef typename rep_type::const_reverse_iterator const_reverse_iterator;

  bool operator()(const value_type& x, const value_type& y) const {
    return comp(x.first, y.first);
  }
  
  //!@{ construct/copy/destroy /////////////////////////////////////////////////
  set() : _tree(Compare(), allocator_type()) { }

  explicit set(const Compare& comp, const allocator_type& a = allocator_type())
      : _tree(comp, a) { }

  template <typename InputIterator>
  set(InputIterator first, InputIterator last) : _tree(Compare(), allocator_type()) {
    _tree.insert_unique(first, last);
  }

  template <typename InputIterator>
  set(InputIterator first, InputIterator last, const Compare& comp,
      const allocator_type& a = allocator_type()) : _tree(comp, a) {
    _tree.insert_unique(first, last);
  }

  set(const set<Key, Compare, Alloc>& other) : _tree(other._tree) { }

  //!@}

  set& operator=(const set& other) {
    _tree = other._tree;
    return *this;
  }
  
  allocator_type get_allocator() const {
    return _tree.get_allocator();
  }

  //!@{ Iterators //////////////////////////////////////////////////////////////
  
  iterator         begin() const  { return _tree.begin(); }
  iterator         end() const    { return _tree.end(); }
  reverse_iterator rbegin() const { return _tree.rbegin(); }
  reverse_iterator rend() const   { return _tree.rend(); }

  //!@}

  //!@{ Capacity ///////////////////////////////////////////////////////////////
  
  bool      empty() const    { return _tree.empty(); }
  size_type size() const     { return _tree.size(); }
  size_type max_size() const { return _tree.max_size(); }

  //!@}

  //!@{ Modifiers //////////////////////////////////////////////////////////////

  void clear() { _tree.clear(); }

  pair<iterator, bool> insert(const value_type& x) {
    pair<typename rep_type::iterator, bool> p = _tree.insert_unique(x);
    return pair<iterator, bool>(p.first, p.second);
  }

  iterator insert(iterator position, const value_type& x) {
    typedef typename rep_type::iterator rep_iterator;
    return _tree.insert_unique((rep_iterator&)position, x);
  }

  template <class InputIterator>
  void insert(InputIterator first, InputIterator last) {
    _tree.insert_unique(first, last);
  }

  void erase(iterator position) {
    typedef typename rep_type::iterator rep_iterator;
    _tree.erase((rep_iterator&)position);
  }

  size_type erase(const key_type& k) {
    return _tree.erase(k);
  }

  void erase(iterator first, iterator last) {
    typedef typename rep_type::iterator rep_iterator;
    _tree.erase((rep_iterator&)first, (rep_iterator&)last);
  }

  void swap(set &other) {
    _tree.swap(other._tree);
  }

  //!@}

  //!@{ Observers //////////////////////////////////////////////////////////////

  key_compare key_comp() const {
    return _tree.key_comp();
  }

  value_compare value_comp() const {
    return value_compare(_tree.key_comp());
  }

  //!@}

  //!@{ Operations /////////////////////////////////////////////////////////////

  iterator       find(const key_type& x)       { return _tree.find(x); }
  const_iterator find(const key_type& x) const { return _tree.find(x); }

  size_type      count(const key_type& x) const {
    return _tree.find(x) == _tree.end() ? 0 : 1;
  }

  iterator       lower_bound(const key_type& x) {
    return _tree.lower_bound(x);
  }

  const_iterator lower_bound(const key_type& x) const {
    return _tree.lower_bound(x);
  }

  iterator       upper_bound(const key_type& x) {
    return _tree.upper_bound(x);
  }

  const_iterator upper_bound(const key_type& x) const {
    return _tree.upper_bound(x);
  }

  pair<iterator, iterator>             equal_range(const key_type& x) {
    return _tree.equal_range(x);
  }

  pair<const_iterator, const_iterator> equal_range(const key_type& x) const {
    return _tree.equal_range(x);
  }

  //!@}

  template <typename K1, typename C1, typename A1>
  friend bool operator==(const set<K1, C1, A1>&,
                         const set<K1, C1, A1>&);

  template <typename K1, typename C1, typename A1>
  friend bool operator<(const set<K1, C1, A1>&,
                        const set<K1, C1, A1>&);
}; // set

//!@{ Non-member functions /////////////////////////////////////////////////////

template <typename Key, typename Compare, typename Alloc>
inline bool operator==(const set<Key, Compare, Alloc>& x,
                       const set<Key, Compare, Alloc>& y) {
  return x._tree == y._tree;
}

template <typename Key, typename Compare, typename Alloc>
inline bool operator<(const set<Key, Compare, Alloc>& x,
                      const set<Key, Compare, Alloc>& y) {
  return x._tree < y._tree;
}

template <typename Key, typename Compare, typename Alloc>
inline bool operator!=(const set<Key, Compare, Alloc>& x,
                       const set<Key, Compare, Alloc>& y) {
  return !(x == y);
}

template <typename Key, typename Compare, typename Alloc>
inline bool operator>(const set<Key, Compare, Alloc>& x,
                      const set<Key, Compare, Alloc>& y) {
  return y < x;
}

template <typename Key, typename Compare, typename Alloc>
inline bool operator<=(const set<Key, Compare, Alloc>& x,
                       const set<Key, Compare, Alloc>& y) {
  return !(y < x);
}

template <typename Key, typename Compare, typename Alloc>
inline bool operator>=(const set<Key, Compare, Alloc>& x,
                       const set<Key, Compare, Alloc>& y) {
  return !(x < y);
}

template <typename Key, typename Compare, typename Alloc>
inline void swap(set<Key, Compare, Alloc>& x,
                 set<Key, Compare, Alloc>& y) {
  x.swap(y);
}

//!@}

} /* namespace ft */

#endif /* __SET_HPP__ */
