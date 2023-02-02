#ifndef __FUNCTION_HPP__
#define __FUNCTION_HPP__
namespace ft
{

template <typename T>
struct _Identity {
  T& operator()(T& x) const { return x; }
  const T& operator()(const T& x) const { return x; }
};

template <typename _Pair>
struct _Select1st {
  typename _Pair::first_type& operator()(_Pair& x) const {
    return x.first;
  }
  const typename _Pair::first_type& operator()(const _Pair& x) const {
    return x.first;
  }
};

} /* namespace ft */

#endif /* __FUNCTION_HPP__ */
