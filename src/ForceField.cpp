#include "AtomVector.h"
#include "ForceField.h"
#include "PotentialEnergy.h"

#ifdef DEBUG
#include "print_vector.h"
#endif // DEBUG
void ForceField::calculate(std::shared_ptr<const AtomVector> av)
{
  energy = 0.0;
  force.assign(av->size(), 0.0);
  for (const auto &pe : pes) {
    pe->calculate(av);
    energy += pe->getPotential();
    //force += pe->getForce();

#ifdef DEBUG
    //printVector(pe->getForce()) << '\t' << "force" << std::endl;
#endif // DEBUG
    auto end = force.end();
    auto p2 = pe->getForce().cbegin();
    for (auto p1 = force.begin(); p1 != end; ++p1)
#ifdef DEBUG
    {
      //std::cout << p1 - force.begin() << std::endl;
      //std::cout << *p1 << " " << *p2 << std::endl;
      *p1 += *p2++;
      //std::cout << *p1 << " " << *p2 << std::endl;
    }
#else
      *p1 += *p2++;
#endif // DEBUG
#ifdef DEBUG
    //printVector(force) << '\t' << "force" << std::endl;
#endif // DEBUG
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
