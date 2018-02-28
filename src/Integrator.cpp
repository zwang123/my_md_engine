#include "Integrator.h"

Integrator::Integrator (const class CommandOption &co) 
  : ModifyCommand(co), ts(parseValue("TIMESTEP"))
{
  auto rtn = parseOptional<TimeStep::StepType> ("CURRSTEP");
  if (!rtn.second)
    ts.setStep(rtn.first);
}
