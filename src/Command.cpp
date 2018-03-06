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
  , args(parseOptionalVector<std::string>("ARG").first)
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

std::string Command::parseCompulsoryString(const char *key)
{
  auto result = parseString(key);
  if (result.empty())
    throw std::invalid_argument("Keyword " + std::string(key) + " not found!");
  return result;
}

double Command::parseValue(const char *key)
{
  const auto s = parseString(key);
  return atof(s.c_str());
}
