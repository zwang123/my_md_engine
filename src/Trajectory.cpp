#include "AtomVector.h"
#include "CommandGenerator.h"
#include "Engine.h"
#include "ForceField.h"
#include "Integrator.h"
#include "System.h"
#include "TimeStep.h"
#include "Trajectory.h"
#include <cassert>

REGISTER_COMMAND(Trajectory)

Trajectory::Trajectory (const class CommandOption &co)
  : SetupCommand(co)
  , fname(parseString("FILE"))
  , ofs(fname)
  , os(fname.empty()?engine->getOstream():ofs)
  , av(engine->getSystem()->getAtomVector())
#ifdef THERMO
  , ofs2("thermo")
  , os2(ofs2)
#endif
{
  auto rtn = parseOptional<TimeStep::StepType> ("EVERY");
  nevery = rtn.second ? 1 : rtn.first;
  rtn = parseOptional<TimeStep::StepType> ("FLUSH");
  flush_every = rtn.second ? 10000 : rtn.first;

  assert(flush_every % nevery == 0);
  assert(fname.empty() || ofs.is_open());

#ifdef THERMO
  assert(ofs2.is_open());
#endif
}

Trajectory::~Trajectory()
{
  if (ofs.is_open())
    ofs.close();

#ifdef THERMO
  ofs2.close();
#endif
}

void Trajectory::write() const
{
  const auto ts = engine->getIntegrator()->getTimeStep();

  if (ts.getStep() % nevery) return;

  using size_type = AtomVector::size_type;
  static constexpr const auto dim = AtomVector::dim;
  static constexpr const char *delim = "  ";

  //using ConstValueIterator = AtomVector::ConstValueIterator;
  os << ts.getStep() << delim << ts.getTime() << std::endl;

  auto ke = av->kinetic();
  auto pe = engine->getSystem()->getForceField()->getPotential();

  os << ke << delim << pe << delim << ke + pe << std::endl;

#ifdef THERMO
  os2 << ts.getTime() << delim << ke << delim << pe << delim << ke + pe << std::endl;
#endif

  auto ptype = av->getConstTypeVector().cbegin();
  auto pmass = av->getConstMassVector().cbegin();
  auto ppos = av->getConstPositionVector().cbegin();
  auto pvel = av->getConstVelocityVector().cbegin();
  auto pfor  = av->getConstForceVector().cbegin();
  const auto natom = av->atomNumber();
  for (size_type i = 0; i != natom; ++i) {
    os << *ptype++ << delim << *pmass;
    pmass += dim;

    for (size_type j = 0; j != dim; ++j) {
      os << delim << *ppos++;
    }

    for (size_type j = 0; j != dim; ++j) {
      os << delim << *pvel++;
    }

    for (size_type j = 0; j != dim; ++j) {
      os << delim << *pfor++;
    }
    os << std::endl;
  }

  os << std::endl;

  if (ts.getStep() % flush_every == 0) os.flush();
#ifdef THERMO
  if (ts.getStep() % flush_every == 0) os2.flush();
#endif
}
