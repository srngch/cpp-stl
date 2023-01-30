#ifndef __ALGOBASE_HPP__
#define __ALGOBASE_HPP__

namespace ft
{

/**
 * @brief Swaps two values.
 * @param a A thing of arbitrary type.
 * @param b Another thing of arbitrary type.
 * @return Nothing.
 *
 *  This is the simple classic generic implementation.  It will work on
 *  any type which has a copy constructor and an assignment operator.
*/
template<typename _Tp>
inline void swap(_Tp& __a, _Tp& __b) {
  _Tp __tmp = __a;
  __a = __b;
  __b = __tmp;
}

/**
 * @brief Tests a range for element-wise equality.
 * @param first1 An input iterator.
 * @param last1  An input iterator.
 * @param first2 An input iterator.
 * @return A boolean true or false.
 *
 * This compares the elements of two ranges using == and returns true or
 * false depending on whether all of the corresponding elements of the
 * ranges are equal.
 */
template<typename InputIterator1, typename InputIterator2>
inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
  for ( ; first1 != last1; ++first1, ++first2) {
    if (!(*first1 == *first2))
      return false;
  }
  return true;
}

/**
 * @brief Performs "dictionary" comparison on ranges.
 * @param first1 An input iterator.
 * @param last1  An input iterator.
 * @param first2 An input iterator.
 * @param last2  An input iterator.
 * @return A boolean true or false.
 *
 * Returns true if the sequence of elements defined by the range
 * [first1,last1) is lexicographically less than the sequence of elements
 * defined by the range [first2,last2). Returns false otherwise.
 */
template<typename _InputIterator1, typename _InputIterator2>
bool lexicographical_compare(_InputIterator1 __first1, _InputIterator1 __last1,
                             _InputIterator2 __first2, _InputIterator2 __last2) {
  for (;__first1 != __last1 && __first2 != __last2; ++__first1, ++__first2) {
    if (*__first1 < *__first2)
      return true;
    if (*__first2 < *__first1)
      return false;
  }
  return __first1 == __last1 && __first2 != __last2;
}

} /* namespace ft */


#endif /* __ALGOBASE_HPP__ */
