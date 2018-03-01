#include <memory>
#include "Atom.h"
#include "AtomVector.h"

#ifdef DEBUG
#endif // DEBUG

std::shared_ptr<Atom>
AtomVector::addAtom(const Type type,
                    const Value mass, const Value pos, const Value vel,
                    const Value acc)
{
  massVector.push_back(mass);
  positionVector.push_back(pos);
  velocityVector.push_back(vel);
  accelerationVector.push_back(acc);
  typeVector.push_back(type);
  forceVector.push_back(0.0);
  
  return std::make_shared<Atom> 
      (typeVector.cend() - 1,
      massVector.cend() - 1,
      positionVector.end() - 1,
      velocityVector.end() - 1,
      accelerationVector.end() - 1,
      forceVector.end() - 1);
}

#ifdef DEBUG
#include <cstdlib>
std::shared_ptr<Atom>
AtomVector::addAtom()
{
  auto box = Atom::box;
  return addAtom(0, 1.0, rand() % box);
}
#endif // DEBUG
