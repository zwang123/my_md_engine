#ifndef PRINT_VECTOR_H_INCLUDED
#define PRINT_VECTOR_H_INCLUDED

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

template <class T>
std::ostream &printVector(const T &v, 
    std::ostream &os = std::cout, const char *delim = "\t")
{
  std::ostream_iterator<typename T::value_type> oi(os, delim);
  std::copy(std::begin(v), std::end(v), oi);
  return os;
}

#endif // PRINT_VECTOR_H_INCLUDED
