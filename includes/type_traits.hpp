#ifndef __TYPE_TRAITS_HPP__
#define __TYPE_TRAITS_HPP__

namespace ft {

/**
  @brief is_integral
*/

template <class T> struct is_integral            { static const bool value = false; };
template <>        struct is_integral<bool>      { static const bool value = true; };
template <>        struct is_integral<char>      { static const bool value = true; };
template <>        struct is_integral<int>       { static const bool value = true; };
template <>        struct is_integral<long>      { static const bool value = true; };
template <>        struct is_integral<long long> { static const bool value = true; };
template <>        struct is_integral<double>    { static const bool value = true; };

/**
  @brief enable_if
*/

template<bool, typename T = void>
struct enable_if { static const bool value = false; };

template<typename T>
struct enable_if<true, T> { typedef T type; static const bool value = true; };

} /* namespace ft */

#endif /* __TYPE_TRAITS_HPP__ */
