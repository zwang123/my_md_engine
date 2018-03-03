#include "AtomVector.h"
#include "CommandGenerator.h"
#include "Engine.h"
#include "ForceField.h"
#include "System.h"
#include "NewtonianDynamics.h"


#ifdef DEBUG
#include "print_vector.h"
#endif // DEBUG

REGISTER_COMMAND(NewtonianDynamics)

// assume one d?
void NewtonianDynamics::setAcceleration(std::shared_ptr<AtomVector> av) const
{

  av->clearForce();
  auto ff = engine->getSystem()->getForceField();
  ff->calculate(av);
  ff->apply(av);

  const auto massVector = av->getConstMassVector();
  const auto forceVector = av->getConstForceVector();
  auto &accelerationVector = av->getAccelerationVector();

#ifdef DEBUG
  //printVector(forceVector) << '\t' << "force" << std::endl;
#endif // DEBUG

  auto pe = massVector.cend();

  auto pf = forceVector.cbegin();
  auto pa = accelerationVector.begin();

  for (auto pm = massVector.cbegin(); pm != pe; ++pm) {
    *pa++ = *pf++ / *pm;
  }
}
