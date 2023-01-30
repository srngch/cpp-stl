#ifndef __ITERATOR_HPP__
#define __ITERATOR_HPP__

/*
 * Iterator Base Classes
*/

#include <memory>

namespace ft
{
  /*
   * Iterator tags
   */
  // struct input_iterator_tag {};
  // struct output_iterator_tag {};
  // struct forward_iterator_tag : public input_iterator_tag {};
  // struct bidirectional_iterator_tag : public forward_iterator_tag {};
  // struct random_access_iterator_tag : public bidirectional_iterator_tag {};

  /*
   * Iterator
   */
  template <  typename Category,
        typename T,
        typename Distance = std::ptrdiff_t,
        typename Pointer = T*,
        typename Reference = T&>
  class iterator {
  public:
    typedef Category  iterator_category;
    typedef T         value_type;
    typedef Distance  difference_type;
    typedef Pointer   pointer;
    typedef Reference reference;
  };

  template <typename Iterator>
  class iterator_traits {
  public:
    typedef typename Iterator::iterator_category iterator_category;
    typedef typename Iterator::value_type        value_type;
    typedef typename Iterator::difference_type   difference_type;
    typedef typename Iterator::pointer           pointer;
    typedef typename Iterator::reference         reference;
  };

  template <typename T>
  class iterator_traits<T*> {
  public:
    typedef std::random_access_iterator_tag  iterator_category;
    typedef T                           value_type;
    typedef ptrdiff_t                   difference_type;
    typedef T*                          pointer;
    typedef T&                          reference;
  };

  template <typename T>
  class iterator_traits<const T*> {
  public:
    typedef std::random_access_iterator_tag  iterator_category;
    typedef T                           value_type;
    typedef ptrdiff_t                   difference_type;
    typedef const T*                    pointer;
    typedef const T&                    reference;
  };

  // TODO: Iterator adaptor - Reverse Iterator
  /********************
   * Reverse Iterator *
   ********************/

  /*
   * TODO: Iterator operations - advance , distance
   */
  template <typename InputIterator>
  inline typename iterator_traits<InputIterator>::difference_type distance(InputIterator first, InputIterator last) {
    typename iterator_traits<InputIterator>::difference_type n = 0;

    while (first != last) {
      ++first;
      ++n;
    }
    return n;
  }
} /* namespace ft */
#endif /* __ITERATOR_HPP__ */
