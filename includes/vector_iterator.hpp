#ifndef __VECTOR_ITERATOR_HPP__
#define __VECTOR_ITERATOR_HPP__

#include <memory>
#include "iterator.hpp"

namespace ft
{

// This iterator adapter is 'normal' in the sense that it does not
// change the semantics of any of the operators of its iterator
// parameter.  Its primary purpose is to convert an iterator that is
// not a class, e.g. a pointer, into an iterator that is a class.
// The _Container parameter exists solely so that different containers
// using this template can instantiate different types, even if the
// _Iterator parameter is the same.

template<typename Iterator>
class vector_iterator
{
protected:
  Iterator _current;

public:
  typedef typename iterator_traits<Iterator>::iterator_category  iterator_category;
  typedef typename iterator_traits<Iterator>::value_type         value_type;
  typedef typename iterator_traits<Iterator>::difference_type    difference_type;
  typedef typename iterator_traits<Iterator>::reference          reference;
  typedef typename iterator_traits<Iterator>::pointer            pointer;

  // Constructors
  vector_iterator() : _current(Iterator()) { }

  // Copy constructor
  explicit vector_iterator(const Iterator& i) : _current(i) { }

  // Allow iterator to const_iterator conversion
  template<typename Iter>
  inline vector_iterator(const vector_iterator<Iter>& i) : _current(i.base()) { }

  // Destructor
  ~vector_iterator() { }

  // Operator overloads

  reference operator*() const
  { return *_current; }

  pointer operator->() const
  { return _current; }

  vector_iterator& operator++()
  {
    ++_current;
    return *this;
  }

  vector_iterator operator++(int)
  { return vector_iterator(_current++); }

  vector_iterator& operator--()
  {
    --_current;
    return *this;
  }

  vector_iterator operator--(int)
  { return vector_iterator(_current--); }

  reference operator[](const difference_type& n) const
  { return _current[n]; }

  vector_iterator& operator+=(const difference_type& n)
  {
    _current += n;
    return *this;
  }

  vector_iterator operator+(const difference_type& n) const
  { return vector_iterator(_current + n); }

  vector_iterator& operator-=(const difference_type& n)
  {
    _current -= n;
    return *this;
  }

  vector_iterator operator-(const difference_type& n) const
  { return vector_iterator(_current - n); }

  const Iterator& base() const
  { return _current; }
};

template<typename IteratorL, typename IteratorR>
inline bool operator==(const vector_iterator<IteratorL>& lhs,
                       const vector_iterator<IteratorR>& rhs)
{ return lhs.base() == rhs.base(); }

template<typename IteratorL, typename IteratorR>
inline bool operator!=(const vector_iterator<IteratorL>& lhs,
                       const vector_iterator<IteratorR>& rhs)
{ return lhs.base() != rhs.base(); }

template<typename IteratorL, typename IteratorR>
inline bool operator<(const vector_iterator<IteratorL>& lhs,
                      const vector_iterator<IteratorR>& rhs)
{ return lhs.base() < rhs.base(); }

template<typename IteratorL, typename IteratorR>
inline bool operator>(const vector_iterator<IteratorL>& lhs,
                      const vector_iterator<IteratorR>& rhs)
{ return lhs.base() > rhs.base(); }

template<typename IteratorL, typename IteratorR>
inline bool operator<=(const vector_iterator<IteratorL>& lhs,
                       const vector_iterator<IteratorR>& rhs)
{ return lhs.base() <= rhs.base(); }

template<typename IteratorL, typename IteratorR>
inline bool operator>=(const vector_iterator<IteratorL>& lhs,
                       const vector_iterator<IteratorR>& rhs)
{ return lhs.base() >= rhs.base(); }

// lhs와 rhs의 거리를 반환
template<typename IteratorL, typename IteratorR>
inline typename vector_iterator<IteratorL>::difference_type operator-
  (const vector_iterator<IteratorL>& lhs,
   const vector_iterator<IteratorR>& rhs)
{ return lhs.base() - rhs.base(); }

template<typename Iterator>
inline vector_iterator<Iterator> operator+
  (typename vector_iterator<Iterator>::difference_type n,
   const vector_iterator<Iterator>& i)
{ return vector_iterator<Iterator>(i.base() + n); }

// distance()

template<typename Iterator>
inline typename vector_iterator<Iterator>::difference_type
distance(const vector_iterator<Iterator>& first,
         const vector_iterator<Iterator>& last,
         std::input_iterator_tag)
{
  typename vector_iterator<Iterator>::difference_type n = 0;
  while (first != last)
  {
    ++first;
    ++n;
  }
  return n;
}

template<typename Iterator>
inline typename vector_iterator<Iterator>::difference_type
distance(const vector_iterator<Iterator>& first,
         const vector_iterator<Iterator>& last,
         std::random_access_iterator_tag)
{ return last.base() - first.base(); }

} /* namespace ft */
#endif /* __VECTOR_ITERATOR_HPP__ */
