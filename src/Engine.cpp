#include <iosfwd>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <utility>
#include "Command.h"
#include "CommandGenerator.h"
#include "CommandOption.h"
#include "Engine.h"
#include "RunCommand.h"
#include "Tools.h"

std::istream &Engine::read(std::istream &is)
{
  // one line of the file
  std::string line;
  // the command name
  std::string cmdName;
  // lines of file to construct a command
  std::vector<std::string> cmdLines;
  // whether a command header is matched or not
  bool matched = false;
  while (getline(is, line)) {
    line = Tools::trim_comment_whitespace(line, COMMENT);
    if (line.empty())
      continue;

    if (matched) {
      if (Tools::toupper_cpy(line) == "&END") {
        cmd.push_back(CommandGenerator::create(cmdName, 
              CommandOption(shared_from_this(), std::move(cmdLines))));
        cmdLines.clear();
        matched = false;

        auto curr_cmd = cmd.back();
        auto curr_sys = sys;
        std::shared_ptr<RunCommand> curr_run;

        if ((curr_sys = std::dynamic_pointer_cast<class System>(curr_cmd))) {
          if (sys) {
            throw std::invalid_argument("Two SYSTEM commands");
          } else {
            sys = curr_sys;
          }
        } else if ((curr_run = std::dynamic_pointer_cast<RunCommand>(curr_cmd))) {
          setup();
          curr_run->run();
        }
      } else {
        cmdLines.push_back(line);
      }
    } else {
      auto words = Tools::parse_first_toupper(line);
     
      if (words.first == "&BEGIN") {
        cmdName = words.second;
        matched = true;
      } else {
        throw std::runtime_error("First word unmatched. "
            "Should be \"&BEGIN\".");
      }
    }
  }
  if (matched) {
    throw std::runtime_error("&END not found for command " + 
        cmdName + ".");
  }
  return is;
}

void Engine::setup()
{
  if (!sys) return;

  for (const auto &x : cmd)
    if (x->requireSetup())
      x->setup();
  //TODO
}

void Engine::run(size_t n)
{
  if (!sys) return;

  for (size_t i = 0; i != n; ++i)
    for (const auto &x : cmd)
      if (x->currStep())
        x->run();
  //TODO
}

std::shared_ptr<Command> Engine::selectFromLabel(std::string label) const noexcept
{ 
  for (const auto &x : cmd)
    if (x->getLabel() == label)
      return x;
  return nullptr;
}
