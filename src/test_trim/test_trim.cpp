#include "../Tools.h"
#include <sstream>
#include <iostream>
#include <string>

void print(const std::string &str)
{
  std::istringstream isstr(str);
  std::string line;
  while (getline(isstr, line)) {
    //std::string trimmed(Tools::trim_comment(line, "#"));
    //std::string trimmed(Tools::trim_whitespace(line));
    
    std::string trimmed(Tools::trim_comment_whitespace(line));
    if (trimmed.empty())
      continue;
    std::istringstream iss(trimmed);
    
    //std::cout << trimmed << std::endl;

    std::string out;
    while((iss >> out)) {
      std::cout << out;
    }
    std::cout << std::endl;
  }
}

int main()
{
  std::string line ("# This is empty\n"
      "this has something# with comments\n"
      " This has leading whitespace#with comments \n"
      " This has leading and trailing ws#with comments     \n"
      " This has leading and trailing ws # as well as space betwen #   \n"
      " # This is full of # # # # \n"
      " There is no eol in the end ");
  std::string line2 ("# This is empty\n"
      "this has something# with comments\n"
      " This has leading whitespace#with comments \n"
      " This has leading and trailing ws#with comments     \n"
      " This has leading and trailing ws # as well as space betwen #   \n"
      " # This is full of # # # # \n"
      " # Final line is empty w/o eol ");
  print(line);
  print(line2);
  return 0;
}
