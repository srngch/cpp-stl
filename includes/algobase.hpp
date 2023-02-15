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
  while (first1 != last1) {
    if (!(*first1 == *first2))
      return false;
    ++first1;
    ++first2;
  }
  return true;
}

/**
 * @brief Performs "dictionary" comparison on ranges.
 */
template <typename InputIterator1, typename InputIterator2>
bool lexicographical_compare(InputIterator1 first1,
                             InputIterator1 last1,
                             InputIterator2 first2,
                             InputIterator2 last2) {
  while (first1 != last1 && first2 != last2) {
    if (*first1 < *first2)
      return true;
    if (*first2 < *first1)
      return false;
    ++first1;
    ++first2;
  }
  return first1 == last1 && first2 != last2;
}

} /* namespace ft */

#endif /* __ALGOBASE_HPP__ */
