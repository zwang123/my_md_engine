#ifndef __TOOLS_H_INCLUDED
#define __TOOLS_H_INCLUDED

#include <array>
#include <cctype>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <utility>
#include <valarray>
#include <vector>
#include "Engine.h"

struct Tools final
{
  static void toupper(char *s, size_t n)
  {
    for (size_t i = 0; i != n; ++i) s[i] = ::toupper(s[i]);
  }

  static void toupper(std::string &s)
  {
    for (auto & x : s) x = ::toupper(x);
  }

  static std::string toupper_cpy(std::string s)
  {
    toupper(s); return s;
  }

  // indicator_list must be null-terminated string
  static std::string trim_comment(const std::string &s, 
                                  const char *indicator_list)
  {
    auto pos = s.find_first_of(indicator_list);
    return s.substr(0, pos);
  }

  static std::string trim_whitespace(const std::string &s, 
                                     size_t pos = std::string::npos)
  {
    auto start = s.find_first_not_of(whiteSpace);
    if (start >= pos)
      return "";
    // this is guaranteed to be found
    auto endm1 = s.find_last_not_of(whiteSpace, pos);
    return s.substr(start, endm1 - start + 1);
  }

  // indicator_list must be null-terminated string
  static std::string trim_comment_whitespace(const std::string &s, 
                                  const char *indicator_list = 
                                  Engine::COMMENT)
  {
    auto pos = s.find_first_of(indicator_list);
    return trim_whitespace(s, pos);
  }

  // Assume head and tail whiteSpace is trimmed
  static std::pair<std::string, std::string>
  parse_first(const std::string &s)
  {
    auto end = s.find_first_of(whiteSpace);
    auto start2 = s.find_first_not_of(whiteSpace, end);
    if (start2 == std::string::npos)
      throw std::runtime_error("Second word not found.");
     
    // substr raise out_of_range if start2 > line.size()
    return std::make_pair<std::string, std::string>
      (s.substr(0, end), s.substr(start2));
  }
  //
  // Assume head and tail whiteSpace is trimmed
  static std::pair<std::string, std::string>
  parse_first_toupper(const std::string &s)
  {
    auto result = parse_first(s);
    toupper(result.first);
    toupper(result.second);
    return result;
  }

  // Only compatible with "C" locale
  const static std::string whiteSpace;

  template <typename T>
  static typename T::value_type sum(const T &v)
  {
    typename T::value_type tot = 0;
    for (const auto &x : v)
      tot += x;
    return tot;
  }

  // Exterior Algebra, only implement N = 1, 2, 3
  template <typename T, std::size_t N, std::size_t N2>
  static std::array<T, N2>
  cross(const std::array<T, N> &, const std::array<T, N> &);

  template <typename T>
  static std::array<T, 0>
  cross(const std::array<T, 1> &, const std::array<T, 1> &)
  { return {{}}; }

  template <typename T>
  static std::array<T, 1>
  cross(const std::array<T, 2> &lhs, const std::array<T, 2> &rhs)
  { return {{lhs[0] * rhs[1] - lhs[1] * rhs[0]}}; }

  template <typename T>
  static std::array<T, 3>
  cross(const std::array<T, 3> &lhs, const std::array<T, 3> &rhs)
  { return {{ lhs[1] * rhs[2] - lhs[2] * rhs[1],
              lhs[2] * rhs[0] - lhs[0] * rhs[2],
              lhs[0] * rhs[1] - lhs[1] * rhs[0]}}; }

  template <std::size_t N, typename T, typename U>
  static std::valarray<typename U::value_type> cross(const T &lhs, const U &rhs)
  {
    switch (N) {
      case 1:
        return {};
      case 2:
        return {lhs[0] * rhs[1] - lhs[1] * rhs[0]};
      case 3:
        return { lhs[1] * rhs[2] - lhs[2] * rhs[1],
                 lhs[2] * rhs[0] - lhs[0] * rhs[2],
                 lhs[0] * rhs[1] - lhs[1] * rhs[0]};
      default:
        throw std::invalid_argument("Not implemented yet (" + 
            std::string(__FILE__) + ", " +
            std::to_string(__LINE__) + ")");
        return {};
    }
    return {};
  }

  // N2 is the dim of rhs, lhs is the result of cross
  // therefore lhs's dim is exterior_dim[N2]
  template <std::size_t N2, typename T, typename U>
  static std::valarray<typename U::value_type> 
  cross2(const T &lhs, const U &rhs)
  {
    switch (N2) {
      case 1:
        return {};
      case 2:
        return {-lhs[0] * rhs[1],
                 lhs[0] * rhs[0]};
      case 3:
        return cross<3>(lhs, rhs);
      default:
        throw std::invalid_argument("Not implemented yet (" + 
            std::string(__FILE__) + ", " +
            std::to_string(__LINE__) + ")");
        return {};
    }
    return {};
  }

  constexpr const static std::array<std::size_t, 4> exterior_dim {{0, 0, 1, 3}};

private:
  Tools() = delete;
};

#endif // __TOOLS_H_INCLUDED
