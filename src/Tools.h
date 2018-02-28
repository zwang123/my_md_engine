#ifndef TOOLS_H_INCLUDED
#define TOOLS_H_INCLUDED

#include <cctype>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <utility>

struct Tools
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
                                  const char *indicator_list)
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
};

#endif // TOOLS_H_INCLUDED
