#include <stdexcept>
#include "Atom.h"
#include "ForceFeildPairwise.h"
#include "Function.h"

double ForceFieldPairwise::potential(const AtomVec &av)
{
  AtomTypePair atp(av[0]->getType(), av[1]->getType());
  const auto &p = potential_map.find(atp);
  if(p == potential_map.end())
    throw std::runtime_error("Potential not found");
  const auto &func = p->second;
  func->calculate(this->genInput(av));
  return func->getValue();
}

// Assume genInput provides 1D vector
// func is a one-var function
double ForceFieldPairwise::force(const AtomVec &av)
{
  AtomTypePair atp(av[0]->getType(), av[1]->getType());
  const auto &func = potential_map.find(atp)->second;
  func->calculate(this->genInput(av));
  return -func->getDerivative(0) * devInput(av).at(0);
}
