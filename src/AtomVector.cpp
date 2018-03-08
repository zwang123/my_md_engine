#include <cassert>
#include <memory>
#include <stdexcept>
#include "Atom.h"
#include "AtomVector.h"
#include "Tools.h"
#ifdef DEBUG
#include "print_vector.h"
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

#if (DIMENSIONALITY == 2)
AtomVector::ValueValarray AtomVector::rotational_inertia() const
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
  return {tot};
}
#elif (DIMENSIONALITY == 3)
AtomVector::ValueValarray AtomVector::rotational_inertia() const
{
#ifdef DEBUG
  //std::cout << "Enter function: " << __func__ << "\n";
#endif // DEBUG
  assert(size() % dim == 0);
  constexpr static const auto nmatrix = dim * dim;
  ValueValarray tot (0.0, nmatrix);

  const auto com = center_of_mass();

  auto pr = positionVector.data();
  for (auto pm = massVector.cbegin(),
            pend = massVector.cend();
            pm != pend; pm += dim, pr += dim) {
    ValueValarray r(pr, dim); r -= com;
    auto norm2 = (r*r).sum();
    auto mass = (*pm);
#ifdef DEBUG
    //std::cout << "pos, mass, norm2\n";
    //printVector(r) << std::endl;
    //std::cout << mass << " " << norm2 << std::endl;
    //std::cout << "end\n";
#endif // DEBUG

    ValueValarray one_inertia (0.0, nmatrix);
    for (size_type i = 0; i < nmatrix; i += dim + 1) {
      one_inertia[i] = norm2;
    }
    for (size_type i = 0, k = 0; i != dim; ++i) {
      for (size_type j = 0; j != dim; ++j, ++k) {
#ifdef DEBUG
        //std::cout << "i, j, k\n";
        //std::cout << i << j << k << " " << std::endl;
        //std::cout << "end\n";
#endif // DEBUG
#ifdef DEBUG
        //std::cout << "one_inertia[k]\n";
        //std::cout << one_inertia[k] << " " << k << std::endl;
        //std::cout << "end\n";
#endif // DEBUG
        one_inertia[k] -= r[i] * r[j];
        one_inertia[k] *= mass;
      }
    }
    tot += one_inertia;
  }
#ifdef DEBUG
  //std::cout << "tot\n";
  //printVector(tot) << std::endl;
  //std::cout << "end\n";
#endif // DEBUG
  return tot;
}
#endif

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

AtomVector::ValueValarray AtomVector::angular_velocity() const
{
#if (DIMENSIONALITY == 2)
  // both angular_momentum and rotational_inertia are 1-d
  ValueValarray angular_velocity (angular_momentum());
  angular_velocity /= rotational_inertia();
#elif (DIMENSIONALITY == 3)
  ValueValarray rot_inertia (rotational_inertia());
  ValueValarray angular_velocity (solveLinear(rotational_inertia(), 
                                              angular_momentum()));
#else
  throw std::invalid_argument("Not implemented yet (" + 
      std::string(__FILE__) + ", " +
      std::to_string(__LINE__) + ")");
#endif
  return angular_velocity;
}

#if (DIMENSIONALITY == 3)
// _a is _m*_n
// _x is _n*1
// _b is _m*1
#include <cstdlib>
#include <cmath>
#define TOL 1e-8
AtomVector::ValueValarray AtomVector::solveLinear(const ValueValarray &_a, 
                                                  const ValueValarray &_b) const
{
  const auto _m = _b.size();
  assert(_m);
  assert(_a.size() % _m == 0);
  const auto _n = _a.size() / _m;
  assert(_m == _n);

  ValueMatrix _am(_m);
  for (size_type i = 0, k = 0; i != _m; ++i) {
    ValueValarray row(_n + 1);
    for (size_type j = 0; j != _n; ++j, ++k) {
      row[j] = _a[k];
    }
    row[_n] = _b[i];
    _am[i] = row;
  }
#ifdef DEBUG
  //std::cout << "_am start\n";
  //for (const auto &row : _am)
  //  printVector(row) << std::endl;
  //std::cout << "_am end\n";
  //ValueValarray doooo = _am[1]-_am[0]*2.0;
  //printVector(doooo) << std::endl;
#endif // DEBUG

  for (size_type i = 0; i != _m; ++i) {
    Value pivot = _am[i][i];
    assert(std::abs(pivot) > TOL);
    _am[i] /= pivot;
    for (size_type j = i + 1; j < _m; ++j) {
      _am[j] -= _am[i] * static_cast<Value>(_am[j][i]);
    }
  }
#ifdef DEBUG
//  std::cout << "_am start\n";
//  for (const auto &row : _am)
//    printVector(row) << std::endl;
//  std::cout << "_am end\n";
#endif // DEBUG
  for (size_type i = _m - 1; i != 0; --i) {
    for (size_type j = 0; j != i; ++j) {
      _am[j] -= _am[i] * static_cast<Value>(_am[j][i]);
    }
  }

#ifdef DEBUG
//  std::cout << "_am start\n";
//  for (const auto &row : _am)
//    printVector(row) << std::endl;
//  std::cout << "_am end\n";
#endif // DEBUG

  ValueValarray rtn(_m);
  for (size_type i = 0 ; i != _m; ++i) {
    rtn[i] = _am[i][_n];
  }
  return rtn;
}
#endif
