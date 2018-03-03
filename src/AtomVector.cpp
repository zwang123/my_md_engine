#include <memory>
#include "Atom.h"
#include "AtomVector.h"

#ifdef DEBUG
#endif // DEBUG

std::shared_ptr<Atom>
AtomVector::addAtom(const Type type,
                    const Value mass, 
                    const ValueArray &pos, 
                    const ValueArray &vel,
                    const ValueArray &acc)
{
  typeVector.push_back(type);
  positionVector.insert(positionVector.end(), pos.cbegin(), pos.cend());
  velocityVector.insert(velocityVector.end(), vel.cbegin(), vel.cend());
  accelerationVector.insert(accelerationVector.end(), acc.cbegin(), acc.cend());
  for (size_type i = 0; i != dim; ++i) {
    massVector.push_back(mass);
    forceVector.push_back(0.0);
  }
  
  return std::make_shared<Atom> 
      (typeVector.cend() - 1,
      massVector.cend() - dim,
      positionVector.end() - dim,
      velocityVector.end() - dim,
      accelerationVector.end() - dim,
      forceVector.end() - dim);
}

#ifdef DEBUG
#include <cstdlib>
std::shared_ptr<Atom>
AtomVector::addAtom()
{
  auto box = Atom::box;
  ValueArray pos;
  for (size_type i = 0; i != dim; ++i)
    pos[i] = rand() % box;
  return addAtom(0, 1.0, pos);
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


std::shared_ptr<Atom> AtomVector::operator[] (size_type i)
{
  auto idim = i * dim;
  return std::make_shared<Atom> 
      (typeVector.cbegin() + i,
      massVector.cbegin() + idim,
      positionVector.begin() + idim,
      velocityVector.begin() + idim,
      accelerationVector.begin() + idim,
      forceVector.begin() + idim);
}

AtomVector::Value AtomVector::kinetic() const
{
  Value ke = 0.0;
  auto pend = velocityVector.cend();
  for (auto pm = massVector.cbegin(),
            pv = velocityVector.cbegin();
            pv != pend; ++pv)
    ke += (*pm++) * (*pv) * (*pv);
  return (ke *= 0.5);
}
