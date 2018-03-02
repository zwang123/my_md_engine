#include "AtomVector.h"
#include "CommandGenerator.h"
#include "ForceField.h"
#include "System.h"
#include <memory>

REGISTER_COMMAND(System)

System::System(const class CommandOption &co)
  : SetupCommand(co)
  , av(std::make_shared<AtomVector>())
  , ff(std::make_shared<ForceField>())
{}
