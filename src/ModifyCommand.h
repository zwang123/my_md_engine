#ifndef MODIFY_COMMAND_H_INCLUDED
#define MODIFY_COMMAND_H_INCLUDED

#include "Command.h"

class ModifyCommand
: public Command
{
public:
  ModifyCommand(const class CommandOption &co) : Command(co) {run_flag = true;}
};

#endif //  MODIFY_COMMAND_H_INCLUDED
