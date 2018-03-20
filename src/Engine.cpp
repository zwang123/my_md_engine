#include <iosfwd>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <utility>
#include "Command.h"
#include "CommandGenerator.h"
#include "CommandOption.h"
#include "Dynamics.h"
#include "Engine.h"
#include "Integrator.h"
#include "System.h"
#include "RunCommand.h"
#include "Tools.h"

#ifdef DEBUG
#include "PotentialEnergy.h"
#include <iostream>
#include <iterator>
#include <algorithm>
#endif // DEBUG

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
#ifdef DEBUG
        //std::cout << __LINE__ << std::endl;
        //std::copy(cmdLines.cbegin(), cmdLines.cend(), 
        //  std::ostream_iterator<std::string>(std::cout, "\n"));
        //std::cout << std::endl;
#endif // DEBUG
        // WARNING All command lines are trimmed and non-empty
        auto curr_cmd = CommandGenerator::create(cmdName, 
              CommandOption(shared_from_this(), std::move(cmdLines)));
        cmdLines.clear();
        cmd.push_back(curr_cmd);
#ifdef DEBUG
        //std::cout << __LINE__ << std::endl;
#endif // DEBUG
        matched = false;

        // Could put them into postConstruct
        setPtr(sys, "SYSTEM");
        setPtr(integrator, "INTEGRATOR");
        setPtr(dyn, "DYNAMICS");

#ifdef DEBUG
        //auto doo = select<class PotentialEnergy>();
        //for (const auto &p : doo)
        //  std::cout << p.use_count() << std::endl;
        //std::cout << __LINE__ << std::endl;
#endif // DEBUG

        curr_cmd->postConstruct();

        std::shared_ptr<RunCommand> curr_run;

        if ((curr_run = std::dynamic_pointer_cast<RunCommand>(curr_cmd))) {
          setup();
          curr_run->run();
          cmd.pop_back();
        }
      } else {
        // line is trimmed
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

std::shared_ptr<Command> Engine::selectFromLabel(const std::string &label) 
  const noexcept
{ 
  for (const auto &x : cmd)
    if (x->getLabel() == label)
      return x;
  return nullptr;
}
