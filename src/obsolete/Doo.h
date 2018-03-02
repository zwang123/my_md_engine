#ifndef DOO_H_INCLUDED
#define DOO_H_INCLUDED

#include "ForceFeildPairwise.h"
#include "Atom.h"
#include <iostream>
#include <cmath>
#include <cstdlib>


class Doo : public ForceFieldPairwise final {
  std::vector<double> genInput(const AtomVec &av) const override

{
  std::cout << "DOO::GENINPUT" << std::endl;
  return std::vector<double>(std::abs(av[0]->getPosition()-av[1]->getPosition()));
}
  std::vector<double> devInput(const AtomVec &av) const override
{
  std::cout << "DOO::DEVINPUT" << std::endl;
  return av[0]->getPosition() < av[1]->getPosition() ? -1 : 1;
}

};

#endif // DOO_H_INCLUDED
