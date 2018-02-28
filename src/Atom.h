#ifndef ATOM_H_INCLUDED
#define ATOM_H_INCLUDED

//#include <cstddef>
//
//class Atom
//{
//  const double mass = 1.0;
//  double position;
//  double velocity = 0.0;
//  double acceleration = 0.0;
//
//  size_t type = 0;
//
//  static constexpr const int box = 10;
//public:
//  size_t getType() const {return type;}
//  double getPosition() const {return position;}
//
//  Atom();
//  void update(double);
//};

#include "AtomVector.h"

class Atom final
{
public:
  using Value              = AtomVector::Value;
  using ValueVector        = AtomVector::ValueVector;
  using Type               = AtomVector::Type;
  using TypeVector         = AtomVector::TypeVector;
  using ValueIterator      = AtomVector::ValueIterator;
  using ConstValueIterator = AtomVector::ConstValueIterator;
  using TypeIterator       = AtomVector::TypeIterator;

private:
  ConstValueIterator mass;
  ValueIterator position;
  ValueIterator velocity;
  ValueIterator acceleration;

  TypeIterator type;

//TODO pbc??
  static constexpr const int box = 10;
public:
  Type getType() const {return *type;}
  Value getPosition() const {return *position;}

  Atom();
  //void update(double);
};

#endif // ATOM_H_INCLUDED
