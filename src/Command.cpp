#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include "Command.h"
#include "CommandOption.h"
#include "Engine.h"
#include "Tools.h"

Command::Command(const CommandOption &co)
  : engine(co.engine)
  , lines(co.lines)
  , label(parseString("LABEL"))
{
  if (label.empty())
    label = "@L" + std::to_string(engine->cmd.size());
  size_t cnt = 0;
  for (const auto &x : engine->cmd)
    if (x->label == label)
      ++cnt;
  //std::cout << engine->cmd.size() << std::endl;
  
  // Constructor must be called before pushed to cmd
  if (cnt != 0)
    throw std::runtime_error("Multiple labels");
}

std::string Command::findKey(const char *key)
{
  auto pe = lines.end();
  for (auto p = lines.begin(); p != pe; ++p) {
    auto words = Tools::parse_first(*p);
    if (Tools::toupper_cpy(words.first) == key) {
      lines.erase(p);
      return words.second;
    }
  }
  return "";
}

std::string Command::parseString(const char *key)
{
  return findKey(key);
}

#ifdef DEBUG
#include <iostream>
#endif // DEBUG
std::vector<std::string> Command::parseStringVector(const char *key)
{
  std::istringstream iss(findKey(key));
  std::vector<std::string> result;
  while (iss && !iss.eof()) {
    std::string item;
    iss >> item;
#ifdef DEBUG
    //std::cout << item << std::endl;
    //std::cout << iss.str().size() << std::endl;
    //std::cout << iss.eof() << std::endl;
#endif // DEBUG
    result.push_back(item);
  }
  return result;
}

double Command::parseValue(const char *key)
{
  const auto s = parseString(key);
  return atof(s.c_str());
}

template <typename T>
std::pair<T, int> Command::parseOptional(const char *key)
{
  std::istringstream iss(key);
  T result;
  if (iss && !iss.eof()) {
    iss >> result;
    if (iss) {
      if(!iss.eof()) {
        std::cerr << "Warning! parse string not ended" << std::endl;
      }
      return {result, 0};
    } else {
      throw std::runtime_error("Error parsing " + std::string(key));
      return {result, 1};
    }
  } else {
    return {result, 2};
  }
  throw std::logic_error("CODING ERROR");
}

template <typename T>
T Command::parse(const char *key)
{
  auto result = parseOptional<T>(key);
  switch (result.second) {
    case 0:
      return result.first;
      break;
    case 1:
      // Already handled
      break;
    case 2:
      throw std::runtime_error("Nothing to parse " + std::string(key));
      break;
    default:
      throw std::logic_error("CODING ERROR");
      break;
  }
}
// return something?
