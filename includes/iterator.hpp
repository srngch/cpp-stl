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

//!@{ Reverse Iterator /////////////////////////////////////////////////////////

  template<typename Iterator>
  class reverse_iterator
  : public iterator<typename iterator_traits<Iterator>::iterator_category,
                    typename iterator_traits<Iterator>::value_type,
                    typename iterator_traits<Iterator>::difference_type,
                    typename iterator_traits<Iterator>::pointer,
                    typename iterator_traits<Iterator>::reference> {
  protected:
    Iterator current;

  public:
    typedef Iterator                                            iterator_type;
    typedef typename iterator_traits<Iterator>::difference_type difference_type;
    typedef typename iterator_traits<Iterator>::reference       reference;
    typedef typename iterator_traits<Iterator>::pointer         pointer;

  public:
    /**
     * @brief The default constructor default-initializes member @p current.
     * If it is a pointer, that means it is zero-initialized.
     */
    reverse_iterator() : current() { }

    /**
     * @brief This %iterator will move in the opposite direction that @p x does.
     */
    explicit reverse_iterator(iterator_type __x) : current(__x) { }

    /**
     * @brief The copy constructor is normal.
     */
    reverse_iterator(const reverse_iterator& __x) : current(__x.current) { }

    /**
     * @brief A reverse_iterator across other types can be copied in the normal
     *  fashion.
     */
    template<typename Iter>
    reverse_iterator(const reverse_iterator<Iter>& __x) : current(__x.base()) { }

    /**
     * @return c current, the %iterator used for underlying work.
     */
    iterator_type base() const {
      return current;
    }

    reference operator*() const {
      Iterator tmp = current;
      return *--tmp;
    }

    pointer operator->() const {
      return &(operator*());
    }

    reverse_iterator& operator++() {
      --current;
      return *this;
    }

    reverse_iterator operator++(int) {
      reverse_iterator tmp = *this;
      --current;
      return tmp;
    }

    reverse_iterator& operator--() {
      ++current;
      return *this;
    }

    reverse_iterator operator--(int) {
      reverse_iterator tmp = *this;
      ++current;
      return tmp;
    }

    reverse_iterator operator+(difference_type __n) const {
      return reverse_iterator(current - __n);
    }

    reverse_iterator& operator+=(difference_type __n) {
      current -= __n;
      return *this;
    }

    reverse_iterator operator-(difference_type __n) const {
      return reverse_iterator(current + __n);
    }

    reverse_iterator& operator-=(difference_type __n) {
      current += __n;
      return *this;
    }

    reference operator[](difference_type __n) const {
      return *(*this + __n);
    }
  };

  /**
   *  @param  x  A %reverse_iterator.
   *  @param  y  A %reverse_iterator.
   *  @return  A simple bool.
   *
   *  Reverse iterators forward many operations to their underlying base()
   *  iterators.  Others are implemented in terms of one another.
   *
  */
  template<typename IteratorL, typename IteratorR>
  inline bool operator==(const reverse_iterator<IteratorL>& x,
                         const reverse_iterator<IteratorR>& y)
  { return x.base() == y.base(); }

  template<typename IteratorL, typename IteratorR>
  inline bool operator<(const reverse_iterator<IteratorL>& x,
                        const reverse_iterator<IteratorR>& y)
  { return y.base() < x.base(); }

  template<typename IteratorL, typename IteratorR>
  inline bool operator!=(const reverse_iterator<IteratorL>& x,
                         const reverse_iterator<IteratorR>& y)
  { return !(x == y); }

  template<typename IteratorL, typename IteratorR>
  inline bool operator>(const reverse_iterator<IteratorL>& x,
                        const reverse_iterator<IteratorR>& y)
  { return y < x; }

  template<typename IteratorL, typename IteratorR>
  inline bool operator<=(const reverse_iterator<IteratorL>& x,
                         const reverse_iterator<IteratorR>& y)
  { return !(y < x); }

  template<typename IteratorL, typename IteratorR>
  inline bool operator>=(const reverse_iterator<IteratorL>& x,
                         const reverse_iterator<IteratorR>& y)
  { return !(x < y); }

  template<typename IteratorL, typename IteratorR>
  inline typename reverse_iterator<IteratorL>::difference_type
  operator-(const reverse_iterator<IteratorL>& x,
            const reverse_iterator<IteratorR>& y)
  { return y.base() - x.base(); }

  template<typename Iterator>
  inline reverse_iterator<Iterator>
  operator+(typename reverse_iterator<Iterator>::difference_type __n,
            const reverse_iterator<Iterator>& x)
  { return reverse_iterator<Iterator>(x.base() - __n); }

  //!@} Reverse Iterator

  /*
   * TODO: Iterator operations - advance, distance
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
