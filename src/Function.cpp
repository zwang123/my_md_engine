#include <cassert>
#include <stdexcept>
#include "Engine.h"
#include "Function.h"
#include "Integrator.h"
#include "TimeStep.h"

Function::Function(const class CommandOption &co) 
  : SetupCommand(co)
  , nevery(parseOptional<TimeStep::StepType> ("EVERY", 1))
  , flush_every(parseOptional<TimeStep::StepType> ("FLUSH", 10000))
  , ofs(parseString("FILE"))
  //, os(fname.empty()?engine->getOstream():ofs)
{
  assert(flush_every % nevery == 0);
  //assert(fname.empty() || ofs.is_open());
  if (ofs.is_open()) 
    ofs << "# Time    " 
        << label + ".value    " << label + ".derivatives" 
        << std::endl;
}

Function::~Function()
{
  if (ofs.is_open())
    ofs.close();
}

void Function::error(const std::string &errMsg)
{
  throw std::runtime_error(errMsg);
}

// User make sure the values are updated
void Function::write()
{
  //std::cout << __LINE__ << std::endl;
  if (!ofs.is_open()) return;
  const auto ts = engine->getIntegrator()->getTimeStep();
  //std::cout << __LINE__ << std::endl;

  if (ts.getStep() % nevery) return;
  //std::cout << __LINE__ << std::endl;

  static constexpr const char *delim = "  ";

  //using ConstValueIterator = AtomVector::ConstValueIterator;
  ofs << ts.getTime() << delim << getValue();
  auto n = getNumberOfArguments();
  for (decltype(n) i = 0; i != n; ++i) {
    ofs << delim << getDerivative(i);
  }
  ofs << std::endl;

  if (ts.getStep() % flush_every == 0) ofs.flush();
}
