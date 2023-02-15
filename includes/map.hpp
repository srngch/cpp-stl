#ifndef __MAP_HPP__
#define __MAP_HPP__

#include <memory>
#include "rb_tree.hpp"
#include "function.hpp"
#include "pair.hpp"

namespace ft
{

template <typename Key, typename T, typename Compare = std::less<Key>,
          typename Alloc = std::allocator<pair<const Key, T> > >
class map {

public:
  typedef Key                                       key_type;
  typedef T                                         mapped_type;
  typedef ft::pair<const Key, T>                    value_type;
  typedef Compare                                   key_compare;

private:
  typedef rb_tree<key_type, value_type, _Select1st<value_type>,
                   key_compare, Alloc>              rep_type;

  rep_type                                          _tree;

public:
  typedef typename rep_type::allocator_type         allocator_type;
  typedef typename rep_type::size_type              size_type;
  typedef typename rep_type::difference_type        difference_type;

  typedef typename rep_type::reference              reference;
  typedef typename rep_type::const_reference        const_reference;
  typedef typename rep_type::pointer                pointer;
  typedef typename rep_type::const_pointer          const_pointer;

  typedef typename rep_type::iterator               iterator;
  typedef typename rep_type::const_iterator         const_iterator;
  typedef typename rep_type::reverse_iterator       reverse_iterator;
  typedef typename rep_type::const_reverse_iterator const_reverse_iterator;

  class value_compare 
  : public std::binary_function<value_type, value_type, bool> {
    friend class map<Key, T, Compare, Alloc>;

  protected:
    Compare comp;

    value_compare(Compare c) : comp(c) { }
  
  public:
    bool operator()(const value_type& x, const value_type& y) const {
      return comp(x.first, y.first);
    }
  };
  
  //!@{ construct/copy/destroy /////////////////////////////////////////////////

  map() : _tree(Compare(), allocator_type()) { }

  explicit map(const Compare& comp, const allocator_type& a = allocator_type())
  : _tree(comp, a) { }

  template <typename InputIterator>
  map(InputIterator first, InputIterator last)
      : _tree(Compare(), allocator_type()) {
    _tree.insert_unique(first, last);
  }

  template <typename InputIterator>
  map(InputIterator first, InputIterator last, const Compare& comp,
      const allocator_type& a = allocator_type())
      : _tree(comp, a) {
    _tree.insert_unique(first, last);
  }

  map(const map& other) : _tree(other._tree) { }

  //!@}

  map& operator=(const map& other) {
    _tree = other._tree;
    return *this;
  }

  allocator_type get_allocator() const {
    return _tree.get_allocator();
  }

  //!@{ Iterators //////////////////////////////////////////////////////////////
  
  iterator               begin()        { return _tree.begin(); }
  const_iterator         begin() const  { return _tree.begin(); }
  iterator               end()          { return _tree.end(); }
  const_iterator         end() const    { return _tree.end(); }
  reverse_iterator       rbegin()       { return _tree.rbegin(); }
  const_reverse_iterator rbegin() const { return _tree.rbegin(); }
  reverse_iterator       rend()         { return _tree.rend(); }
  const_reverse_iterator rend() const   { return _tree.rend(); }

  //!@}

  //!@{ Capacity ///////////////////////////////////////////////////////////////
  
  bool      empty() const    { return _tree.empty(); }
  size_type size() const     { return _tree.size(); }
  size_type max_size() const { return _tree.max_size(); }

  //!@}

  //!@{ Element access /////////////////////////////////////////////////////////
 
  mapped_type& at(const key_type& k) {
    iterator i = lower_bound(k);

    if (i == end() || key_comp()(k, i->first))
      throw std::out_of_range("ft::map::at");
    return i->second;
  }

  const mapped_type& at(const key_type& k) const {
    const_iterator i = lower_bound(k);

    if (i == end() || key_comp()(k, i->first))
      throw std::out_of_range("ft::map::at");
    return i->second;
  }

  mapped_type& operator[](const key_type& k) {
    iterator i = lower_bound(k);

    if (i == end() || key_comp()(k, i->first))
      i = insert(i, value_type(k, mapped_type()));
    return i->second;
  }

  //!@}

  //!@{ Modifiers //////////////////////////////////////////////////////////////

  void clear() { _tree.clear(); }

  pair<iterator, bool> insert(const value_type& x) {
    return _tree.insert_unique(x);
  }

  iterator insert(iterator position, const value_type& x) {
    return _tree.insert_unique(position, x);
  }

  template<typename InputIterator>
  void insert(InputIterator first, InputIterator last) {
    _tree.insert_unique(first, last);
  }

  void erase (iterator position) {
    _tree.erase(position);
  }

  size_type erase (const key_type& k) {
    return _tree.erase(k);
  }

  void erase (iterator first, iterator last) {
    _tree.erase(first, last);
  }

  void swap(map &other) {
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

  size_type count(const key_type& x) const {
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

  template <typename K1, typename T1, typename C1, typename A1>
  friend bool operator==(const map<K1, T1, C1, A1>&,
                         const map<K1, T1, C1, A1>&);

  template <typename K1, typename T1, typename C1, typename A1>
  friend bool operator<(const map<K1, T1, C1, A1>&,
                        const map<K1, T1, C1, A1>&);
}; // map

//!@{ Non-member functions /////////////////////////////////////////////////////

template <typename Key, typename Tp, typename Compare, typename Alloc>
inline bool operator==(const map<Key, Tp, Compare, Alloc>& x,
                       const map<Key, Tp, Compare, Alloc>& y) {
  return x._tree == y._tree;
}

template <typename Key, typename Tp, typename Compare, typename Alloc>
inline bool operator<(const map<Key, Tp, Compare, Alloc>& x,
                      const map<Key, Tp, Compare, Alloc>& y) {
  return x._tree < y._tree;
}

template <typename Key, typename Tp, typename Compare, typename Alloc>
inline bool operator!=(const map<Key, Tp, Compare, Alloc>& x,
                       const map<Key, Tp, Compare, Alloc>& y) {
  return !(x == y);
}

template <typename Key, typename Tp, typename Compare, typename Alloc>
inline bool operator>(const map<Key, Tp, Compare, Alloc>& x,
                      const map<Key, Tp, Compare, Alloc>& y) {
  return y < x;
}

template <typename Key, typename Tp, typename Compare, typename Alloc>
inline bool operator<=(const map<Key, Tp, Compare, Alloc>& x,
                       const map<Key, Tp, Compare, Alloc>& y) {
  return !(y < x);
}

template <typename Key, typename Tp, typename Compare, typename Alloc>
inline bool operator>=(const map<Key, Tp, Compare, Alloc>& x,
                       const map<Key, Tp, Compare, Alloc>& y) {
  return !(x < y);
}

template <typename Key, typename Tp, typename Compare, typename Alloc>
inline void swap(map<Key, Tp, Compare, Alloc>& x,
                 map<Key, Tp, Compare, Alloc>& y) {
  x.swap(y);
}

//!@}

} /* namespace ft */

#endif /* __MAP_HPP__ */
