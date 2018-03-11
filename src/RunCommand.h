#ifndef __RUN_COMMAND_H_INCLUDED
#define __RUN_COMMAND_H_INCLUDED

#include "Command.h"
#include "Engine.h"

class RunCommand final
: public Command
{
  size_t steps;
public:
  RunCommand(const class CommandOption &);
  void run() override {engine->run(steps);}
  static constexpr const char *directive = "RUN";
};

#endif // __RUN_COMMAND_H_INCLUDED
