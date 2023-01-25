#ifndef __TYPE_TRAITS_HPP__
#define __TYPE_TRAITS_HPP__

namespace ft {

// type for true/false

struct __true_type {};
struct __false_type {};

// is_integral

template <class T> struct is_integral            { typedef __false_type type; };
template <>        struct is_integral<bool>      { typedef __true_type type; };
template <>        struct is_integral<char>      { typedef __true_type type; };
template <>        struct is_integral<int>       { typedef __true_type type; };
template <>        struct is_integral<long>      { typedef __true_type type; };
template <>        struct is_integral<long long> { typedef __true_type type; };
template <>        struct is_integral<double>    { typedef __true_type type; };

} /* namespace ft */

#endif /* __TYPE_TRAITS_HPP__ */
