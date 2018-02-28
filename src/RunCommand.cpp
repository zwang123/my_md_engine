#include "RunCommand.h"

RunCommand::RunCommand(const class CommandOption &co)
  : Command(co)
  , steps(parse<size_t>("STEP"))
{
  run_flag = false;
}
