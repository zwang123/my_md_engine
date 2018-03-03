#include "AtomVector.h"
#include "CommandGenerator.h"
#include "Command.h"
#include "Dynamics.h"
#include "Engine.h"
#include "Function.h"
#include "Trajectory.h"
#include "VelocityVerlet.h"

#ifdef DEBUG
#include <stdexcept>
#include <iostream>
#endif // DEBUG

REGISTER_COMMAND(VelocityVerley)

void VelocityVerley::integrate()
{
  auto &positionVector = av->getPositionVector();
  auto &velocityVector = av->getVelocityVector();
  auto &accelerationVector = av->getAccelerationVector();
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
#ifdef DEBUG
  {
    (*px++) += *pv * dt;
    //(*px) += *pv * dt;
    ////decltype(*px) lll;
    //std::cout << *px << " " << *pv << " " << *pv * dt << std::endl;
    //++px;
  }
#else
    (*px++) += *pv * dt;
#endif // DEBUG

  if(engine->getDynamics()) {
    engine->getDynamics()->setAcceleration(av);
  }
  // does not change acceleration  if no dynamics
  // may be non-zero

  for (auto pv = velocityVector.begin(), 
            pa = accelerationVector.begin();
       pv != pve; ++pv)
    *pv += *pa++ * hdt;

  end_of_step();
}

void VelocityVerley::setup()
{
  Command::setup();
  if(engine->getDynamics()) {
    engine->getDynamics()->setAcceleration(av);
  }
  // does not change acceleration  if no dynamics
  // may be non-zero
  end_of_step();
}

void VelocityVerley::end_of_step()
{
  for (const auto &x : engine->select<Trajectory>()) {
    x->write();
  }
  for (const auto &x : engine->select<Function>()) {
    x->write();
  }
  ++ts;
}
