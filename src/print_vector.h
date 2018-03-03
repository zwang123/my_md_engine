#ifndef PRINT_VECTOR_H_INCLUDED
#define PRINT_VECTOR_H_INCLUDED

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

template <class T>
std::ostream &printVector(const std::vector<T> &v, 
    std::ostream &os = std::cout, const char *delim = "\t")
{
  std::ostream_iterator<T> oi(os, delim);
  std::copy(v.cbegin(), v.cend(), oi);
  return os;
}

#endif // PRINT_VECTOR_H_INCLUDED
