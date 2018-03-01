#include "Engine.h"
#include "System.h"
#include "Integrator.h"

// must be after SYSTEM
Integrator::Integrator (const class CommandOption &co) 
  : ModifyCommand(co), ts(parseValue("TIMESTEP")), 
  av(engine->getSystem()->getAtomVector())
{
  auto rtn = parseOptional<TimeStep::StepType> ("CURRSTEP");
  if (!rtn.second)
    ts.setStep(rtn.first);
}
