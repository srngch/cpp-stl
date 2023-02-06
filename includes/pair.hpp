/** @file stl_pair.h
 *  This is an internal header file, included by rb_tree.hpp.
 *  You should not attempt to use it directly.
 */

#ifndef __PAIR_HPP__
#define __PAIR_HPP__

namespace ft
{

/**
 * @brief pair holds two objects of arbitrary type.
*/
template <class T1, class T2>
struct pair {
  typedef T1 first_type;
  typedef T2 second_type;

  T1 first;
  T2 second;

//   pair() : first(), second() {}
  pair() : first(T1()), second(T2()) {}

  pair(const T1& a, const T2& b) : first(a), second(b) {}

  template <class U1, class U2>
  pair(const pair<U1, U2>& p) : first(p.first), second(p.second) {}

  pair& operator=(const pair &p) {
	first = p.first;
	second = p.second;
	return *this;
  }
};

template <class T1, class T2>
inline bool operator==(const pair<T1, T2>& x, const pair<T1, T2>& y)
{ 
  return x.first == y.first && x.second == y.second; 
}

template <class T1, class T2>
inline bool operator<(const pair<T1, T2>& x, const pair<T1, T2>& y)
{ 
  return x.first < y.first || 
         (!(y.first < x.first) && x.second < y.second);
}

template <class T1, class T2>
inline bool operator!=(const pair<T1, T2>& x, const pair<T1, T2>& y) {
  return !(x == y);
}

template <class T1, class T2>
inline bool operator>(const pair<T1, T2>& x, const pair<T1, T2>& y) {
  return y < x;
}

template <class T1, class T2>
inline bool operator<=(const pair<T1, T2>& x, const pair<T1, T2>& y) {
  return !(y < x);
}

template <class T1, class T2>
inline bool operator>=(const pair<T1, T2>& x, const pair<T1, T2>& y) {
  return !(x < y);
}

/**
 *  @brief A convenience wrapper for creating a pair from two objects.
 *  @param  x  The first object.
 *  @param  y  The second object.
 *  @return   A newly-constructed pair<> object of the appropriate type.
 *
 *  The standard requires that the objects be passed by reference-to-const,
 *  but LWG issue #181 says they should be passed by const value.  We follow
 *  the LWG by default.
*/
template <class T1, class T2>
// inline pair<T1, T2> make_pair(const T1& x, const T2& y)
inline pair<T1, T2> make_pair(T1 x, T2 y) {
  return pair<T1, T2>(x, y);
}

} /* namespace ft */

#endif /* __PAIR_HPP__ */

