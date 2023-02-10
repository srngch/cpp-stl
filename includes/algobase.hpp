#ifndef __ALGOBASE_HPP__
#define __ALGOBASE_HPP__

namespace ft {

/**
 * @brief Swaps two values.
 */
template <typename Tp>
inline void swap(Tp& a, Tp& b) {
  Tp tmp = a;
  a = b;
  b = tmp;
}

/**
 * @brief Tests a range for element-wise equality.
 */
template <typename InputIterator1, typename InputIterator2>
inline bool equal(InputIterator1 first1, InputIterator1 last1,
                  InputIterator2 first2) {
  for (; first1 != last1; ++first1, ++first2) {
    if (!(*first1 == *first2))
      return false;
  }
  return true;
}

/**
 * @brief Performs "dictionary" comparison on ranges.
 *
 * Returns true if the sequence of elements defined by the range
 * [first1,last1) is lexicographically less than the sequence of elements
 * defined by the range [first2,last2). Returns false otherwise.
 */
template <typename InputIterator1, typename InputIterator2>
bool lexicographical_compare(InputIterator1 first1,
                             InputIterator1 last1,
                             InputIterator2 first2,
                             InputIterator2 last2) {
  for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
    if (*first1 < *first2)
      return true;
    if (*first2 < *first1)
      return false;
  }
  return first1 == last1 && first2 != last2;
}

} /* namespace ft */

#endif /* __ALGOBASE_HPP__ */
