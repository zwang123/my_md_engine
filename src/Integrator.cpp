#include "Engine.h"
#include "System.h"
#include "Integrator.h"
#include <cassert>

// must be after SYSTEM
Integrator::Integrator (const class CommandOption &co) 
  : ModifyCommand(co)
  , ts(parse<double>("TIMESTEP"),
       parseOptional<TimeStep::StepType>("CURRSTEP", 0))
  , av(engine->getSystem()->getAtomVector())
{
  assert(engine->getSystem());
}
