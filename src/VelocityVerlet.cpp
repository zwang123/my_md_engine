#include "AtomVector.h"
#include "CommandGenerator.h"
#include "Dynamics.h"
#include "Engine.h"
#include "VelocityVerlet.h"

#ifdef DEBUG
#include <stdexcept>
#endif // DEBUG

REGISTER_COMMAND(VelocityVerley)

void VelocityVerley::integrate()
{
  auto positionVector = av->getPositionVector();
  auto velocityVector = av->getVelocityVector();
  auto accelerationVector = av->getAccelerationVector();
  const auto dt = ts.getInterval();
  const auto hdt = .5 * dt;

#ifdef DEBUG
  if (!av->check())
    throw std::logic_error("Atom Vector size inconsistent");
#endif // DEBUG

  auto pve = velocityVector.end();
  for (auto pv = velocityVector.begin(), 
            pa = accelerationVector.begin();
       pv != pve; ++pv)
    *pv += *pa++ * hdt;
  for (auto pv = velocityVector.begin(), 
            px = positionVector.begin();
       pv != pve; ++pv)
    (*px++) += *pv * dt;
  setup();
  for (auto pv = velocityVector.begin(), 
            pa = accelerationVector.begin();
       pv != pve; ++pv)
    *pv += *pa++ * hdt;
}

void VelocityVerley::setup()
{
  if(engine->getDynamics()) {
    engine->getDynamics()->setAcceleration(av);
  }
}
