#ifndef COMMAND_GENERATOR_H_INCLUDED
#define COMMAND_GENERATOR_H_INCLUDED

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <stdexcept>
#include <utility>
#include <vector>

#include "Command.h"

//class Command; // Do not allow incomplete type
//
//To avoid bad_weak_ptr in derived class of Command, 
//  use shared_ptr instead of unique_ptr
//  because the counter of internal weak_ptr is set 
//  in the constructor of shared_ptr
class CommandOption;

class CommandGenerator final {
  using CommandConstructor = std::function<
  std::shared_ptr<Command>(const CommandOption &)>;
  
private:
  template <typename T, typename ...Args>
  static std::shared_ptr<Command> commandConstruct (Args&&... args)
  {
    return std::make_shared<T>(std::forward<Args>(args)...);
  }
 
  static std::map<std::string, CommandConstructor> commandMap;

  template <typename T>
  static std::shared_ptr<Command> commandConstructFromOption
  (const CommandOption &co)
  {
    return commandConstruct<T>(co);
  }

public:
  template <typename classname>
  static void add (const std::string &directive)
  {
    if (commandMap.find(directive) != commandMap.end()) {
      throw std::logic_error("Directive " + directive
          + " is already registered!");
    } else {
      commandMap[directive] =  commandConstructFromOption<classname>;
    }
  }

  static std::shared_ptr<Command> create (const std::string &directive,
                                          const CommandOption &co)
  {
    auto p = commandMap.find(directive);
    if (p != commandMap.end()) {
      return p->second(co);
    } else {
      throw std::runtime_error("Directive " + directive
          + " is not registered!");
      return nullptr;
    }
  }
};

#define __CONCATENATE_DIRECT(s1, s2) s1##s2
#define __CONCATENATE(s1, s2) __CONCATENATE_DIRECT(s1, s2)
#define __UNIQUENAME(str) __CONCATENATE(str, __LINE__)

// If fail to compile, move this line in between 
//   REGISTER_COMMAND and static struct
// and decomment
  //constexpr const char *classname::directive; 
  //
#define REGISTER_COMMAND(classname) \
  static struct  __UNIQUENAME(classname##RegisterMe) {\
    __UNIQUENAME(classname##RegisterMe)() {\
      CommandGenerator::add<classname>(classname::directive); \
    }\
  } __UNIQUENAME(classname##RegisterMeObject);

#endif // COMMAND_GENERATOR_H_INCLUDED
