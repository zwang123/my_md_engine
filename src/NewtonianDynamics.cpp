#include "AtomVector.h"
#include "NewtonianDynamics.h"

// assume one d?
void NewtonianDynamics::setAccelaration(AtomVector &av) const
{
  auto massVector = av.getConstMassVector();
  auto forceVector = av.getConstForceVector();
  auto accelerationVector = av.getAccelerationVector();

  auto pe = massVector.cend();

  auto pf = forceVector.cbegin();
  auto pa = accelerationVector.begin();

  for (auto pm = massVector.cbegin(); pm != pe; ++pm) {
    *pa++ = *pf++ / *pm;
  }
}
