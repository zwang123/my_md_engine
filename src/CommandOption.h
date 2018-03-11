#ifndef __COMMAND_OPTION_H_INCLUDED
#define __COMMAND_OPTION_H_INCLUDED

#include <memory>
#include <string>
#include <utility>
#include <vector>

class Command;
class CommandOption final {
  friend class Command;

  std::shared_ptr<class Engine> engine;
  std::vector<std::string> lines;
public:
  CommandOption(std::shared_ptr<class Engine> engine,
      std::vector<std::string> &&lines) noexcept
    : engine(engine)
    , lines(std::move(lines))
  {}
  CommandOption(std::shared_ptr<class Engine> engine,
      const std::vector<std::string> &lines) noexcept
    : engine(engine)
    , lines(lines)
  {}
};

#endif // __COMMAND_OPTION_H_INCLUDED
