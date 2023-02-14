#ifndef __VECTOR_HPP__
#define __VECTOR_HPP__

#include <memory>
#include "vector_iterator.hpp"
#include "type_traits.hpp"
#include "algobase.hpp"

namespace ft {

template <typename T, typename Alloc = std::allocator<T> >
class vector {

public:
  typedef T                                        value_type;
  typedef Alloc                                    allocator_type;
  typedef size_t                                   size_type;
  typedef ptrdiff_t                                difference_type;

  typedef typename allocator_type::reference       reference;
  typedef typename allocator_type::const_reference const_reference;
  typedef typename allocator_type::pointer         pointer;
  typedef typename allocator_type::const_pointer   const_pointer;

  typedef ft::vector_iterator<pointer>             iterator;
  typedef ft::vector_iterator<const_pointer>       const_iterator;
  typedef ft::reverse_iterator<iterator>           reverse_iterator;
  typedef ft::reverse_iterator<const_iterator>     const_reverse_iterator;

private:
  allocator_type                                   _alloc;
  pointer                                          _start;
  pointer                                          _finish;
  pointer                                          _end_of_storage;

public:
  //!@{ construct/copy/destroy /////////////////////////////////////////////////

  /**
   * @brief constructor(default)
   * constructs an empty container, with no elements.
   * @example empty vector of ints
   *     ft::vector<int> first;
   */
  explicit vector(const allocator_type& a = allocator_type()) 
  : _alloc(a), _start(NULL), _finish(NULL), _end_of_storage(NULL) { }

  /**
   * @brief constructor(fill)
   * constructs a container with n elements each initialized to a copy of v
   * @example four ints with value 100
   *     ft::vector<int> second(4, 100);
   */
  explicit vector(size_type n, const value_type& v = value_type(),
                  const allocator_type& a = allocator_type())
  : _alloc(a), _start(NULL), _finish(NULL), _end_of_storage(NULL) {
    _start = _alloc.allocate(n);
    _finish = _start;
    _end_of_storage = _finish + n;

    while (n--)
      _alloc.construct(_finish++, v);
  }

  /**
   * @brief constructor(range)
   * @example iterating through second
   *     ft::vector<int> third(second.begin(), second.end());
   */
  
  template <class InputIterator>
  vector(InputIterator first,
         InputIterator last,
         const allocator_type& a = allocator_type(),
         typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = 0
         )
  : _alloc(a), _start(NULL), _finish(NULL), _end_of_storage(NULL) {
    difference_type n = ft::distance(first, last);

    _start = _alloc.allocate(n);
    _finish = _start;

    while (n--)
      _alloc.construct(_finish++, *first++);

    _end_of_storage = _finish;
  }

  /**
   * @brief copy constructor
   * @example a copy of second
   *     ft::vector<int> fourth(second);
   */
  vector(const vector& other)
   : _alloc(other._alloc), _start(NULL), _finish(NULL), _end_of_storage(NULL)
  {
    _start = _alloc.allocate(other.size());
    _finish = _start;
    _end_of_storage = _start + other.size();
    insert(begin(), other.begin(), other.end());
  }

  /**
   * @brief destructor
   * Destroy the vector object
   */
  ~vector() { 
    clear();
    for (pointer p = _start; p != _finish; p++)
      _alloc.destroy(p);
    _alloc.deallocate(_start, _finish - _start);
   }

  /**
   * @brief assignment operator
   * Assigns new contents to the container, replacing its current contents, and
   * modifying its size accordingly.
   *
   * @param x container to be copied.
   * @return vector&
   */
  vector& operator=(const vector& x) {
    if (this != &x) {
      clear();
      insert(begin(), x.begin(), x.end());
    }
    return *this;
  }

  //!@}

  //!@{ Iterators //////////////////////////////////////////////////////////////

  /**
   * @brief Returns a read/write iterator pointing to the first element in the
   * vector.
   */
  iterator begin() { return iterator(_start); }

  /**
   * @brief Returns a read-only iterator pointing to the first element in the
   * vector.
   */
  const_iterator begin() const { return const_iterator(_start); }

  /**
   * @brief Returns a read/write iterator pointing to one past the last element
   * in the vector.
   */
  iterator end() { return iterator(_finish); }

  /**
   * @brief Returns a read-only iterator pointing to one past the last element
   * in the vector.
   */
  const_iterator end() const { return const_iterator(_finish); }

  //!@}

  //!@{ Reverse Iterators //////////////////////////////////////////////////////

  /**
   * @brief Returns a read/write reverse iterator pointing to the last element
   * in the vector
   */
  reverse_iterator rbegin() { return reverse_iterator(end()); }

  /**
   * @brief a read-only reverse iterator pointing to the last element in the
   * vector
   */
  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }

  /**
   * @brief a read/write reverse iterator pointing to one before the first
   * element in the vector
   */
  reverse_iterator rend() { return reverse_iterator(begin()); }

  /**
   * @brief Returns a read-only reverse iterator pointing to one before the
   * first element in the vector
   */
  const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }

  //!@}

  //!@{ Capacity ///////////////////////////////////////////////////////////////

  /**
   * @brief Returns the number of elements in the vector.
   * This is the number of actual objects held in the vector, which is not
   * necessarily equal to its storage capacity.
   * @return size of the container
   */
  size_type size() const { return size_type(end() - begin()); }

  /**
   * @brief Returns the maximum number of elements that the vector can hold.
   */
  size_type max_size() const { return size_type(-1) / sizeof(value_type); }

  /**
   * @brief Resizes the container so that it contains n elements.
   * @param n new size of the container
   * @param v value to initialize the new elements with
   */
  void resize(size_type n, value_type v = value_type()) {
    if (n < size()) {
      erase(begin() + n, end());
    } else {
      insert(end(), n - size(), v);
    }
  }

  /**
   * @brief Returns the size of the storage space currently allocated for the
   * vector, expressed in terms of elements.
   */
  size_type capacity() const {
    return size_type(const_iterator(_end_of_storage) - begin());
  }

  /**
   * @brief Returns whether the vector is empty: i.e. whether its size is 0.
   */
  bool empty() const { return begin() == end(); }

  /**
   * @brief Requests that the vector capacity be at least enough to contain n
   * elements.
   * @param n: new capacity of the container
   */
  void reserve(size_type n) {
    if (n < capacity())
      return;
    // TODO: if n is greater than the current vector capacity....
    // https://cplusplus.com/reference/vector/vector/reserve/
  }

  //!@}

  //!@{ Element Access /////////////////////////////////////////////////////////

  /**
   * @brief Returns a reference to the element at position n in the vector
   * container.
   */
  reference       operator[](size_type n) { return *(begin() + n); }
  const_reference operator[](size_type n) const { return *(begin() + n); }

  /**
   * @brief Returns a reference to the element at position n in the vector.
   * check if n is within the range of valid elements
   */
  reference at(size_type n) {
    if (n >= size())
      throw std::out_of_range("vector::at");
    return (*this)[n];
  }
  const_reference at(size_type n) const {
    if (n >= size())
      throw std::out_of_range("vector::at");
    return (*this)[n];
  }

  /**
   * @brief Returns a reference to the first element in the vector.
   */
  reference       front() { return *begin(); }
  const_reference front() const { return *begin(); }

  /**
   * @brief Returns a reference to the last element in the vector.
   */
  reference       back() { return *(end() - 1); }
  const_reference back() const { return *(end() - 1); }

  /**
   * @brief Returns a direct pointer to the memory array used internally by the
   * vector to store its owned elements.
   */
  pointer       data() { return _start; }
  const_pointer data() const { return _start; }

  //!@}

  //!@{ Modifier ///////////////////////////////////////////////////////////////

  /**
   * @brief Assigns new contents to the vector, replacing its current contents,
   * and modifying its size accordingly.
   * @param n: new size of the container
   * @param val: value to initialize the new elements with
   */
  void assign(size_type n, const value_type& val) { 
    clear();
    if (capacity() < n) {
      pointer old_start = _start;
      pointer old_end = _end_of_storage;

      _start = _alloc.allocate(n);
      _finish = _start;
      _end_of_storage = _start + n;

      while (old_end != old_start)
        _alloc.destroy(old_end--);

      while (n--)
        _alloc.construct(_finish++, val);
      _alloc.deallocate(old_start, old_end - old_start);
      return;
    }
    while (n--)
      _alloc.construct(_finish++, val);
  }

  /**
   * @brief Assigns new contents to the vector, replacing its current contents,
   * and modifying its size accordingly.
   * @param first: InputIterator to the initial position in a sequence
   * @param last: InputIterator to the final position in a sequence
   */
  template <typename InputIterator>
  void assign(InputIterator first, InputIterator last,
              typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = 0) {
    
    clear();
    size_type n = ft::distance(first, last);

    if (capacity() < n) {
      pointer old_start = _start;
      pointer old_end = _end_of_storage;

      _start = _alloc.allocate(n);
      _finish = _start;
      _end_of_storage = _start + n;
      pointer tmp = &(*first);

      while (old_end != old_start)
        _alloc.destroy(old_end--);

      while (n--)
        _alloc.construct(_finish++, *tmp++);
      _alloc.deallocate(old_start, old_end - old_start);
      return;
    }
    while (n--)
      _alloc.construct(_finish++, *first++);
  }

  /**
   * @brief Adds a new element at the end of the vector, after its current last
   * element.
   */
  void push_back(const value_type& val) {
    if (_finish != _end_of_storage) {
      _alloc.construct(_finish, val);
      ++_finish;
    } else {
      insert(end(), val);
    }
  }

  /**
   * @brief Removes the last element in the vector, effectively reducing the
   * container size by one.
   */
  void pop_back() {
    --_finish;
    _alloc.destroy(_finish);
  }

  /**
   * @brief Inserts elements at the specified location in the container.
   * Effectively increases the container size by the number of elements
   * inserted.
   * @param position iterator before which the content will be inserted
   * @param val value to be copied (or moved) to the inserted elements
   */
  iterator insert(iterator position, const value_type& val) {
    const size_type n = position - begin();
    _fill_insert(position, 1, val);
    return begin() + n;
  }

  /**
   * @brief Inserts n elements at the specified location in the container.
   * @param position iterator before which the content will be inserted
   * @param n number of elements to insert
   * @param val value to be copied (or moved) to the inserted elements
   */
  void insert(iterator position, size_type n, const value_type& val) {
    _fill_insert(position, n, val);
  }

  /**
   * @brief Inserts elements from range [first, last) at the specified location
   * in the container.
   * @param position iterator before which the content will be inserted
   * @param first beginning of the range(including the element pointed by first)
   * @param last end of the range(excluding the element pointed by last)
   */
  template <typename InputIterator>
  void insert(iterator position, InputIterator first, InputIterator last,
              typename ft::enable_if<!ft::is_integral<InputIterator>::value,
                                     InputIterator>::type* = 0) {
    while (first != last) {
      position = insert(position, *first);
      ++position;
      ++first;
    }
  }

  /**
   * @brief Remove single element at position.
   * @param position iterator pointing to a single element to be removed from
   * the vector.
   * @return An iterator pointing to the element that followed the last element
   * removed by the function call.
   */
  iterator erase(iterator position) {
    if (position != end() - 1)
      std::copy(position + 1, end(), position);
    pointer old_finish = _finish;
    --_finish;
    _alloc.destroy(old_finish);
    return position;
  }

  /**
   * @brief Removes a range of elements.
   * @param first iterator pointing to the first element to be removed from the
   * vector.
   * @param last iterator pointing to the element following the last element to
   * be removed from the vector.
   * @return An iterator pointing to the element that followed the last element
   * removed by the function call.
   */
  iterator erase(iterator first, iterator last) {
    if (first == last)
      return first;
    if (last != end())
      std::copy(last, end(), first);

    pointer old_finish = _finish;
    _finish = _finish - (last - first);
    while (old_finish != _finish) {
      _alloc.destroy(old_finish--);
    }
    return first;
  }

  /**
   * @brief Exchanges the content of the container by the content of x, which is
   * another vector object of the same type.
   *
   * @param x
   */
  void swap(vector& x) {
    ft::swap(_start, x._start);
    ft::swap(_finish, x._finish);
    ft::swap(_end_of_storage, x._end_of_storage);
  }

  /**
   * @brief Removes all elements from the vector (which are destroyed), leaving
   * the container with a size of 0.
   */
  void clear() { erase(begin(), end()); }

  //!@}

  //!@{ Allocator //////////////////////////////////////////////////////////////

  /**
   * @brief Get the allocator object
   */
  allocator_type get_allocator() const { return _alloc; }

  //!@}

protected:

  /**
   * @brief Called by the 'insert' function
   * @param position iterator before which the content will be inserted
   * @param n number of elements to insert
   * @param val value to be copied (or moved) to the inserted elements
   */
  void _fill_insert(iterator position, size_type n, const value_type& v) {
    if (n == 0)
      return;
    if (max_size() < n)
      throw (std::length_error("vector::insert (fill)"));

    if (n <= size_type(_end_of_storage - _finish)) {
      // if there is enough space at the end of the vector
      size_type n_after = end() - position;
      pointer old_finish = _finish - 1;
      _finish += n;
      pointer p = _finish - 1;

      while (n_after--)
        _alloc.construct(p--, *(old_finish--));
      while (n--)
        _alloc.construct(p--, v);
      return;
    }
  
    // if there is not enough space at the end of the vector
    size_type n_before = position - begin();
    size_type n_after = end() - position;

    size_type new_size = size() + std::max(size(), n);
    pointer s = _start;
    pointer old_start = _start;

    _start = _alloc.allocate(new_size);
    _finish = _start;
    _end_of_storage = _start + new_size;

    while (n_before--) {
      _alloc.construct(_finish++, *s);
      _alloc.destroy(s++);
    }
    while (n--)
      _alloc.construct(_finish++, v);
    while (n_after--) {
      _alloc.construct(_finish++, *s);
      _alloc.destroy(s++);
    }

    _alloc.deallocate(old_start, capacity());
  }
}; // vector

//!@{ Non-member functions /////////////////////////////////////////////////////

template <typename T, typename Alloc>
inline bool operator==(const vector<T, Alloc>& lhs,
                       const vector<T, Alloc>& rhs) {
  return lhs.size() == rhs.size() &&
         ft::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename T, typename Alloc>
inline bool operator!=(const vector<T, Alloc>& lhs,
                       const vector<T, Alloc>& rhs) {
  return !(lhs == rhs);
}

template <typename T, typename Alloc>
inline bool operator<(const vector<T, Alloc>& lhs,
                      const vector<T, Alloc>& rhs) {
  return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                     rhs.end());
}

template <typename T, typename Alloc>
inline bool operator<=(const vector<T, Alloc>& lhs,
                       const vector<T, Alloc>& rhs) {
  return !(rhs < lhs);
}

template <typename T, typename Alloc>
inline bool operator>(const vector<T, Alloc>& lhs,
                      const vector<T, Alloc>& rhs) {
  return rhs < lhs;
}

template <typename T, typename Alloc>
inline bool operator>=(const vector<T, Alloc>& lhs,
                       const vector<T, Alloc>& rhs) {
  return !(lhs < rhs);
}

template <typename T, typename Alloc>
inline void swap(vector<T, Alloc>& x, vector<T, Alloc>& y) {
  x.swap(y);
}

//!@}

} /* namespace ft */

#endif /* __VECTOR_HPP__ */
