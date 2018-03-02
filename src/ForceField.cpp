#include "AtomVector.h"
#include "ForceField.h"
#include "PotentialEnergy.h"

void ForceField::calculate(std::shared_ptr<const AtomVector> av)
{
  energy = 0.0;
  force.assign(av->size(), 0.0);
  for (const auto &pe : pes) {
    pe->calculate(av);
    energy += pe->getPotential();
    //force += pe->getForce();

    auto end = force.end();
    for (auto p1 = force.begin(), p2 = pe->getForce().begin(); p1 != end; ++p1)
      *p1 += *p2++;
  }
}

void ForceField::apply(std::shared_ptr<AtomVector> av) const
{
  //av.getForceVector() += force;
  auto p2 = av->getForceVector().begin();
  auto end = force.end();
  for (auto p1 = force.begin(); p1 != end; ++p1)
    (*p2++) += *p1;
}
