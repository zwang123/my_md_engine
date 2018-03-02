#include "Engine.h"
#include "System.h"
#include "Integrator.h"
#include <cassert>

// must be after SYSTEM
Integrator::Integrator (const class CommandOption &co) 
  : ModifyCommand(co), ts(parse<double>("TIMESTEP")), 
  av(engine->getSystem()->getAtomVector())
{
  assert(engine->getSystem());
  auto rtn = parseOptional<TimeStep::StepType> ("CURRSTEP");
  if (!rtn.second)
    ts.setStep(rtn.first);
}
