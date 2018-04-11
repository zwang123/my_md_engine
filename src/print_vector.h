#ifndef __PRINT_VECTOR_H_INCLUDED
#define __PRINT_VECTOR_H_INCLUDED

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include <cstddef>

template <class T>
std::ostream &printVector(const T &v, 
    std::ostream &os = std::cout, const char *delim = "\t")
{
  std::ostream_iterator<typename T::value_type> oi(os, delim);
  std::copy(std::begin(v), std::end(v), oi);
  return os;
}

template <class T, std::size_t N>
std::ostream &printVector(const T (&array)[N], 
    std::ostream &os = std::cout, const char *delim = "\t")
{
  std::ostream_iterator<T> oi(os, delim);
  std::copy(std::begin(array), std::end(array), oi);
  return os;
}

template <class T>
std::ostream &printVector(T * const &ptr, std::size_t n,
    std::ostream &os = std::cout, const char *delim = "\t")
{
  std::ostream_iterator<T> oi(os, delim);
  std::copy(ptr, ptr + n, oi);
  return os;
}

#endif // __PRINT_VECTOR_H_INCLUDED
