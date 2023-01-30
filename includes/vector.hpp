#ifndef __VECTOR_HPP__
#define __VECTOR_HPP__

#include <memory>
#include "vector_iterator.hpp"
#include "type_traits.hpp"
#include "algobase.hpp"

namespace ft
{

template<typename T, typename Alloc = std::allocator<T> >
class vector_base
{
protected:
  typedef Alloc                            allocator_type;
  typedef typename Alloc::pointer          pointer;
  typedef typename Alloc::const_pointer    const_pointer;
  typedef typename Alloc::reference        reference;
  typedef typename Alloc::const_reference  const_reference;

  allocator_type                           alloc;
  pointer                                 _start;
  pointer                                 _finish;
  pointer                                 _end_of_storage;

public:

  vector_base(const allocator_type& a)
  : alloc(a), _start(NULL), _finish(NULL), _end_of_storage(NULL) { }

  vector_base(size_t n, const allocator_type& a)
  : alloc(a), _start(alloc.allocate(n)), _finish(_start), _end_of_storage(_start + n) { }

  ~vector_base() {
    if (_start)
      alloc.deallocate(_start, _end_of_storage - _start);
  }
};


template <typename T, typename Alloc = std::allocator<T> >
class vector : protected vector_base<T, Alloc>
{
  typedef vector_base<T, Alloc>           base;
  typedef vector<T, Alloc>                vector_type;

public:
  typedef T                                         value_type;
  typedef Alloc                                     allocator_type;
  typedef size_t                                    size_type;
  typedef ptrdiff_t                                 difference_type;

  typedef typename allocator_type::reference        reference;
  typedef typename allocator_type::const_reference  const_reference;
  typedef typename allocator_type::pointer          pointer;
  typedef typename allocator_type::const_pointer    const_pointer;

  typedef ft::vector_iterator<pointer>              iterator;
  typedef ft::vector_iterator<const_pointer>        const_iterator;
  // typedef ft::reverse_iterator<iterator>           reverse_iterator;
  // typedef ft::reverse_iterator<const_iterator>     const_reverse_iterator;

protected:
  allocator_type c;

public:

  /**
   * @brief constructor(default)
   * constructs an empty container, with no elements.
   * @example empty vector of ints
   *     ft::vector<int> first;
   */
  explicit
  vector(const allocator_type& a = allocator_type()) : base(a) {
    // std::cout << "constructor(default)" << std::endl;
  }

  /**
   * @brief constructor(fill)
   * constructs a container with n elements each initialized to a copy of v
   * @example four ints with value 100
   *     ft::vector<int> second(4, 100);
   */
  explicit
  vector(size_type n,
         const value_type& v = value_type(),
         const allocator_type& a = allocator_type()) : base(n, a) {
    // std::cout << "constructor(fill)" << std::endl;
    while (n--)
      c.construct(this->_finish++, v);
  }

  /**
   * @brief constructor(range)
   * @example iterating through second
   *     ft::vector<int> third(second.begin(), second.end());
   */
  template <class InputIterator>
  vector(InputIterator first,
         InputIterator last,
         const allocator_type& a = allocator_type()) : base(a) {
    // std::cout << "constructor(range)" << std::endl;
    // check if it's an integer type
    typedef typename ft::is_integral<InputIterator>::type  is_integer;
    _initialize_dispatch(first, last, is_integer());
  }


  /**
   * @brief copy constructor
   * @example a copy of second
   *     ft::vector<int> fourth(second);
   */
  vector(const vector& other) : base(other.size(), other.c) {
    this->insert(this->begin(), other.begin(), other.end());
  }


  /**
   * @brief destructor
   * Destroy the vector object
   */
  ~vector() {
    this->clear();
  }

  /**
   * @brief assignment operator
   * Assigns new contents to the container, replacing its current contents, and modifying its size accordingly.
   *
   * @param x container to be copied.
   * @return vector&
   */
  vector& operator=(const vector& x) {
    if (this != &x) {
      this->clear();
      // TODO: assign으로 변경 고려하기
      this->insert(this->begin(), x.begin(), x.end());
    }
    return *this;
  }

  //!@{ Iterators //////////////////////////////////////////////////////////////

  /**
   * @brief Returns a read/write iterator pointing to the first element in the vector.
   */
  iterator begin() {
    return iterator(this->_start);
  }

  /**
   * @brief Returns a read-only iterator pointing to the first element in the vector.
   */
  const_iterator begin() const {
    return const_iterator(this->_start);
  }

  /**
   * @brief Returns a read/write iterator pointing to one past the last element in the vector.
   */
  iterator end() {
    return iterator(this->_finish);
  }

  /**
   * @brief Returns a read-only iterator pointing to one past the last element in the vector.
   */
  const_iterator end() const {
    return const_iterator(this->_finish);
  }

  //!@} Iterators

  //!@{ Reverse Iterators //////////////////////////////////////////////////////
  // TODO: reverse_iterator 구현 후 계속

  /**
   * @brief Returns a read/write reverse iterator pointing to the last element in the vector
   */
  // reverse_iterator rbegin() {
  //   return reverse_iterator(end());
  // }

  /**
   * @brief a read-only reverse iterator pointing to the last element in the vector
   */
  // const_reverse_iterator rbegin() const {
  //   return const_reverse_iterator(end());
  // }

  /**
   * @brief a read/write reverse iterator pointing to one before the first element in the vector
   */
  // reverse_iterator rend() {
  //   return reverse_iterator(begin());
  // }

  /**
   * @brief Returns a read-only reverse iterator pointing to one before the first element in the vector
   */
  // const_reverse_iterator rend() const {
  //   return const_reverse_iterator(begin());
  // }

  //!@} Reverse Iterators

  //!@{ Capacity ///////////////////////////////////////////////////////////////

  /**
   * @brief Returns the number of elements in the vector.
   * This is the number of actual objects held in the vector, which is not necessarily equal to its storage capacity.
   * @return size of the container
   */
  size_type size() const {
    return size_type(end() - begin());
  }

  /**
   * @brief Returns the maximum number of elements that the vector can hold.
   */
  size_type max_size() const {
    return size_type(-1) / sizeof(value_type);
  }

  /**
   * @brief Resizes the container so that it contains n elements.
   * @param n new size of the container
   * @param v value to initialize the new elements with
  */
  void resize(size_type n, value_type v = value_type()) {
    // TODO: insert 구현 후 다시 체크
    if (n < size()) {
      erase(begin() + n, end());
    } else {
      insert(end(), n - size(), v);
    }
  }

  /**
   * @brief Returns the size of the storage space currently allocated for the vector, expressed in terms of elements.
   */
  size_type capacity() const {
    return size_type(const_iterator(this->_end_of_storage) - begin());
  }
  /**
   * @brief Returns whether the vector is empty: i.e. whether its size is 0.
   */
  bool empty() const {
    return begin() == end();
  }
  /**
   * @brief Requests that the vector capacity be at least enough to contain n elements.
   * @param n: new capacity of the container
   */
  void reserve(size_type n) {
    if (n < capacity())
      return;
    // TODO: if n is greater than the current vector capacity....
    // https://cplusplus.com/reference/vector/vector/reserve/
  }

  //!@} Capacity

  //!@{ Element Access /////////////////////////////////////////////////////////

  /**
   * @brief Returns a reference to the element at position n in the vector container.
   */
  reference operator[](size_type n) {
    return *(begin() + n);
  }
  const_reference operator[](size_type n) const {
    return *(begin() + n);
  }

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
  reference front() {
    return *begin();
  }
  const_reference front() const {
    return *begin();
  }

  /**
   * @brief Returns a reference to the last element in the vector.
   */
  reference back() {
    return *(end() - 1);
  }
  const_reference back() const {
    return *(end() - 1);
  }

  /**
   * @brief Returns a direct pointer to the memory array used internally by the vector to store its owned elements.
   */
  pointer data() {
    return this->_start;
  }
  const_pointer data() const {
    return this->_start;
  }

  //!@} Element Access

  //!@{ Modifier ///////////////////////////////////////////////////////////////

  /**
   * @brief Assigns new contents to the vector, replacing its current contents, and modifying its size accordingly.
   * @param n: new size of the container
   * @param val: value to initialize the new elements with
   */
  void assign(size_type n, const value_type& val) {
    _fill_assign(n, val);
  }

  /**
   * @brief Assigns new contents to the vector, replacing its current contents, and modifying its size accordingly.
   * @param first: InputIterator to the initial position in a sequence
   * @param last: InputIterator to the final position in a sequence
   */
  template <typename InputIterator>
  void assign(InputIterator first, InputIterator last) {
    typedef typename ft::is_integral<InputIterator>::type  is_integer;
    _assign_dispatch(first, last, is_integer());
  }

  /**
   * @brief Adds a new element at the end of the vector, after its current last element.
   */
  void push_back(const value_type& val) {
    if (this->_finish != this->_end_of_storage) {
      c.construct(this->_finish, val);
      ++this->_finish;
    } else {
      insert(end(), val);
    }
  }

  /**
   * @brief Removes the last element in the vector, effectively reducing the container size by one.
   */
  void pop_back() {
    --this->_finish;
    c.destroy(this->_finish);
  }

  /**
   * @brief Inserts elements at the specified location in the container.
   * Effectively increases the container size by the number of elements inserted.
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
   * @brief Inserts elements from range [first, last) at the specified location in the container.
   * @param position iterator before which the content will be inserted
   * @param first beginning of the range(including the element pointed by first)
   * @param last end of the range(excluding the element pointed by last)
   */
  template <typename InputIterator>
  void insert(iterator position, InputIterator first, InputIterator last) {
    // check if it's an integer type
    typedef typename ft::is_integral<InputIterator>::type  is_integer;
    _insert_dispatch(position, first, last, is_integer());
  }

  /**
   * @brief Remove single element at position.
   * @param position iterator pointing to a single element to be removed from the vector.
   * @return An iterator pointing to the element that followed the last element removed by the function call.
   */
  iterator erase(iterator position) {
    if (position != end() - 1)
      std::copy(position + 1, end(), position);
    pointer old_finish = this->_finish;
    --this->_finish;
    c.destroy(old_finish);
    return position;
  }

  /**
   * @brief Removes a range of elements.
   * @param first iterator pointing to the first element to be removed from the vector.
   * @param last iterator pointing to the element following the last element to be removed from the vector.
   * @return An iterator pointing to the element that followed the last element removed by the function call.
   */
  iterator erase(iterator first, iterator last) {
    if (first == last)
      return first;
    if (last != end())
      std::copy(last, end(), first);

    pointer old_finish = this->_finish;
    this->_finish = this->_finish - (last - first);
    while (old_finish != this->_finish) {
      c.destroy(old_finish--);
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
    ft::swap(this->_start, x._start);
    ft::swap(this->_finish, x._finish);
    ft::swap(this->_end_of_storage, x._end_of_storage);
  }

  /**
   * @brief Removes all elements from the vector (which are destroyed), leaving the container with a size of 0.
   */
  void clear() {
    erase(begin(), end());
  }

  //!@} Modifiers

  //!@{ Allocator //////////////////////////////////////////////////////////////

  /**
   * @brief Get the allocator object
   */
  allocator_type get_allocator() const {
    return c;
  }

  //!@} Allocator

protected:

  /**
   * @brief Called by the 'constructor 3: range'
   */
  template <typename InputIterator>
  void _initialize_dispatch(InputIterator first, InputIterator last, ft::__false_type) {
    difference_type n = ft::distance(first, last);

    this->_start = c.allocate(n);
    this->_finish = this->_start;

    while (n--)
      c.construct(this->_finish++, *first++);

    this->_end_of_storage = this->_finish;
  }

  /**
   * @brief if the iterator is an integer type, then it is assumed to be a size.
   * Called by the 'constructor 3: range'
   */
  template <typename Integer>
  void _initialize_dispatch(Integer n, Integer val, ft::__true_type) {
    this->_start = c.allocate(n);
    this->_finish = this->_start;
    this->_end_of_storage = this->_finish + n;

    while (n--)
      c.construct(this->_finish++, val);
  }

  void _fill_assign(size_type n, const value_type& val) {
    // if the capacity is not enough, then reallocate
    if (n > capacity()) {
      pointer old_start = this->_start;
      pointer old_finish = this->_finish;

      this->_start = c.allocate(n);
      this->_finish = this->_start;
      this->_end_of_storage = this->_start + n;

      while (old_finish != old_start)
        c.destroy(old_finish--);
      c.deallocate(old_start, old_finish - old_start);

      while (n--)
        c.construct(this->_finish++, val);
      return;
    }

    // if the capacity is enough, but the size is not enough, then assign and construct
    if (n > size()) {
      size_type tmp_n = n;
      pointer i = this->_start;

      while (i != this->_finish) {
        *i++ = val;
      }
      while (tmp_n--)
        c.construct(this->_finish++, val);
      this->_finish = this->_start + n;
      return;
    }

    // if the capacity is enough, and the size is enough, then just assign
    size_type tmp_n = n;
    pointer i = this->_start;
    while (tmp_n--)
      *i++ = val;
    while (i != this->_finish)
      c.destroy(i++);
    this->_finish = this->_start + n;
  }

  template <typename Integer>
  void _assign_dispatch(Integer n, Integer val, ft::__true_type) {
    _fill_assign(static_cast<size_type>(n), static_cast<value_type>(val));
  }

  template <typename InputIterator>
  void _assign_dispatch(InputIterator first, InputIterator last, ft::__false_type) {
    iterator it = begin();
    while (first != last && it != end())
      *it++ = *first++;
    if (first == last)
      erase(it, end());
    else
      insert(end(), first, last);
  }

  /**
   * @brief Called by the 'insert' function
   */
  template<typename InputIterator>
  void _insert_dispatch(iterator position, InputIterator first, InputIterator last, __false_type) {
    while (first != last) {
      position = insert(position, *first);
      ++position;
      ++first;
    }
  }

  /**
   * @brief Called by the 'insert' function
   * @param position iterator before which the content will be inserted
   * @param n number of elements to insert
   * @param val value to be copied (or moved) to the inserted elements
   * @param is_integer check if it's an integer type
   */
  template<typename Integer>
  void _insert_dispatch(iterator position, Integer n, Integer val, __true_type) {
    _fill_insert(position, static_cast<size_type>(n), static_cast<value_type>(val));
  }

  /**
   * @brief Called by the '_insert_dispatch' function above
   * @param position iterator before which the content will be inserted
   * @param n number of elements to insert
   * @param val value to be copied (or moved) to the inserted elements
   */
  void _fill_insert(iterator position, size_type n, const value_type& v) {
    if (n == 0)
      return;

    // if the capacity is enough. Do not need to allocate new memory.
    if (static_cast<size_type>(this->_end_of_storage - this->_finish) >= n) {
      value_type v_copy = v;

      // if position is at the end of the vector
      if (position == end()) {
        this->_finish += n;
        pointer p = this->_finish - 1;

        while (n--)
          c.construct(p--, v_copy);
        return;
      }

      // if position is not at the end of the vector
      size_type n_elems_after = end() - position;
      pointer p_old = this->_finish - 1;
      this->_finish += n;
      pointer p = this->_finish - 1;

      while (n_elems_after--)
        c.construct(p--, *(p_old--));
      while (n--)
        c.construct(p--, v_copy);
      return ;
    }

    // if the capacity is not enough. Need to allocate new memory.
    pointer old_start = this->_start;
    const size_type old_size = size();
    size_type n_elems_before = position - begin();
    size_type n_elems_after = end() - position;

    const size_type len = old_size + std::max(old_size, n);
    // const size_type len = old_size + n;
    this->_start = c.allocate(len);
    this->_finish = this->_start;
    this->_end_of_storage = this->_start + len;

    // copy the elements before position
    while (n_elems_before--) {
      c.construct(this->_finish++, *old_start);
      c.destroy(old_start);
      ++old_start;
    }
    // insert n elements with value v
    while (n--)
      c.construct(this->_finish++, v);
    // copy the elements after position
    while (n_elems_after--) {
      c.construct(this->_finish++, *old_start);
      c.destroy(old_start);
      ++old_start;
    }
  }

}; // vector

//! Non-member functions overloads

template <typename T, typename Alloc>
inline bool operator==(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
  return lhs.size() == rhs.size() &&
    ft::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename T, typename Alloc>
inline bool operator!=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
  return !(lhs == rhs);
}

template <typename T, typename Alloc>
inline bool operator<(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
  return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T, typename Alloc>
inline bool operator<=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
  return !(rhs < lhs);
}

template <typename T, typename Alloc>
inline bool operator>(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
  return rhs < lhs;
}

template <typename T, typename Alloc>
inline bool operator>=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
  return !(lhs < rhs);
}

template <typename T, typename Alloc>
inline void swap(vector<T, Alloc>& x, vector<T, Alloc>& y) {
  x.swap(y);
}

} /* namespace ft */


#endif /* __VECTOR_HPP__ */
