#include <cassert>
#include <memory>
#include "Atom.h"
#include "AtomVector.h"
#include "Tools.h"

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
#include "RandomNumberGenerator.h"
#include "TimeSeed.h"
#include <iostream>
std::shared_ptr<Atom>
AtomVector::addAtom()
{
  constexpr auto box = Atom::box;
  static struct TempClass { TempClass() 
    { RandomNumberGenerator<>::seed(TimeSeed::gen_seed()); 
      std::cout << "seed: " << std::endl;
    }} tempClass;
  static RandomNumberGenerator<> rng(0.0, box);

  ValueArray pos;
  for (size_type i = 0; i != dim; ++i)
    pos[i] = rng();
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
  for (auto pm = massVector.cbegin(),
            pv = velocityVector.cbegin(),
            pend = velocityVector.cend();
            pv != pend; ++pv)
    ke += (*pm++) * (*pv) * (*pv);
  return (ke *= 0.5);
}

AtomVector::ValueArray AtomVector::momentum() const
{
  assert(size() % dim == 0);

  ValueArray tot {{0.0}};
  for (auto pm = massVector.cbegin(),
            pv = velocityVector.cbegin(),
            pend = velocityVector.cend();
            pv != pend;)
    for (size_type i = 0; i != dim; ++i)
      tot[i] += (*pm++) * (*pv++);
  return tot;
}

AtomVector::ValueValarray AtomVector::angular_momentum() const
{
  if (dim <= 1) return {};
  assert(size() % dim == 0);

  ValueValarray tot;
  const auto com = center_of_mass();
  bool is_first = true;
  auto pr = positionVector.data();
  for (auto pm = massVector.cbegin(),
            pv = velocityVector.cbegin(),
            pend = velocityVector.cend();
            pv != pend; pm += dim, pr += dim, pv += dim) {
    ValueValarray r(pr, dim);
    r -= com;
    if (is_first) {
      tot = (*pm) * Tools::cross<dim>(r, pv);
      is_first = false;
    } else {
      tot += (*pm) * Tools::cross<dim>(r, pv);
    }
  }
  return tot;
}

AtomVector::Value AtomVector::rotational_inertia() const
{
  assert(size() % dim == 0);
  Value tot = 0.0;
  const auto com = center_of_mass();
  for (auto pm = massVector.cbegin(),
            pr = positionVector.cbegin(),
            pend = positionVector.cend();
            pr != pend;)
    for (size_type i = 0; i != dim; ++i, ++pr) {
      auto r = *pr - com[i];
      tot += (*pm++) * r * r;
    }
  return tot;
}

// This is calculated three times... can remove this?
AtomVector::ValueValarray AtomVector::center_of_mass() const
{
  assert(size() % dim == 0);
  ValueValarray com(0.0, dim);
  Value tot_mass = 0;
  auto pr = positionVector.data();
  for (auto pm = massVector.cbegin(),
            pend = massVector.cend();
            pm != pend; pr += dim, pm += dim) {
    com += (*pm) * ValueValarray(pr, dim);
    tot_mass += (*pm);
  }
  return com /= tot_mass;
}
