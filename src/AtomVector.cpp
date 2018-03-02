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

auto AtomVector::slicePositionVector(const std::vector<size_type> &slice) const
-> std::pair<std::vector<size_type>, ValueVector>
{
  ValueVector result;
  std::vector<size_type> indicies;
  for (const auto &index : slice) {
    auto start = index * dim;
    auto end = start + dim;
    for (auto j = start; j != end; ++j) {
      indicies.push_back(j);
      result.push_back(positionVector[j]);
    }
  }
  return {indicies, result};
}
