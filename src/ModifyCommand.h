#ifndef __MODIFY_COMMAND_H_INCLUDED
#define __MODIFY_COMMAND_H_INCLUDED

#include "Command.h"

class ModifyCommand
: public Command
{
public:
  ModifyCommand(const class CommandOption &co) : Command(co) {run_flag = true;}
};

#endif // __MODIFY_COMMAND_H_INCLUDED
