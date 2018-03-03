#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

#include <memory>
#include <string>
#include <vector>

//namespace command {

class Command
{
protected:
  std::shared_ptr<class Engine> engine;
  std::vector<std::string> lines;
  std::string label;
  std::vector<std::string> args;

  bool setup_flag = true;
  bool run_flag = false;

  std::string findKey(const char *);
public:
  const std::string &getLabel() const {return label;}
  // This allow white space in string
  std::string parseString(const char *);
  std::string parseCompulsoryString(const char *);
  // Specification?
  double parseValue(const char *);
  //std::vector<std::string> parseStringVector(const char *);

  template <typename T> std::pair<T, int> parseOptional(const char *);
  template <typename T> T parse(const char *);
  template <typename T> std::pair<std::vector<T>, int> parseOptionalVector(const char *);
  template <typename T> std::vector<T> parseVector(const char *);

  // lines must be already trimmed
  explicit Command(const class CommandOption &);
  virtual ~Command() {}

  virtual void postConstruct() {}
  virtual void run() = 0;
  virtual void setup() {setup_flag = false;}

  bool currStep() const {return run_flag;}
  bool requireSetup() const {return setup_flag;}
};
//}

#include <iostream>
#include <sstream>
#include <stdexcept>
template <typename T>
std::pair<T, int> Command::parseOptional(const char *key)
{
  auto line = findKey(key);
  std::istringstream iss(line);
  T result;
  if (!line.empty() && iss && !iss.eof()) {
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
  return {result, 3};
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
  throw std::logic_error("CODING ERROR");
  return result.first;
}

template <typename T>
std::pair<std::vector<T>, int> 
Command::parseOptionalVector(const char *key)
{
  auto line = findKey(key);
  std::istringstream iss(line);
  std::vector<T> result;
  while (!line.empty() && iss && !iss.eof()) {
    T item;
    iss >> item;
    result.push_back(item);
  }
  if (!iss) {
    throw std::runtime_error("Error parsing " + std::string(key));
    return {result, 1};
  } else if (result.empty()) {
    return {result, 2};
  } else {
    return {result, 0};
  }
  throw std::logic_error("CODING ERROR");
  return {result, 3};
}

template <typename T>
std::vector<T> Command::parseVector(const char *key)
{
  auto result = parseOptionalVector<T>(key);
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
  throw std::logic_error("CODING ERROR");
  return result.first;
}

#endif // COMMAND_H_INCLUDED
