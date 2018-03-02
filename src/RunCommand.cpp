#include "CommandGenerator.h"
#include "RunCommand.h"

REGISTER_COMMAND(RunCommand)

RunCommand::RunCommand(const class CommandOption &co)
  : Command(co)
  , steps(parse<size_t>("STEP"))
{
  run_flag = false;
}
