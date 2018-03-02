#include "Dynamics.h"
#include "Engine.h"
#include <cassert>

Dynamics::Dynamics(const class CommandOption &co)
  : SetupCommand(co)
{
  assert(engine->getSystem());
}
