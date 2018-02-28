#ifndef SETUP_COMMAND_H_INCLUDED
#define SETUP_COMMAND_H_INCLUDED

#include "Command.h"

class SetupCommand
: public Command
{
public:
  SetupCommand(const class CommandOption &co) 
    : Command(co) {run_flag = false;}
};

#endif // SETUP_COMMAND_H_INCLUDED
