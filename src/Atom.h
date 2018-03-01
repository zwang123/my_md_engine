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
  using ConstTypeIterator  = AtomVector::ConstTypeIterator;

private:
  ConstTypeIterator type;
  ConstValueIterator mass;

  ValueIterator position;
  ValueIterator velocity;
  ValueIterator acceleration;

  ValueIterator force;

public:
//TODO pbc??
  static constexpr const int box = 10;
  static constexpr const AtomVector::Type dim = AtomVector::dim;

  Type getType() const {return *type;}
  Value getPosition() const {return *position;}

  Atom (ConstTypeIterator type, ConstValueIterator mass, 
        ValueIterator position, ValueIterator velocity, 
        ValueIterator acceleration, ValueIterator force)
    : type(type), mass(mass), position(position), velocity(velocity)
    , acceleration(acceleration), force(force)
  {}
  //void update(double);
};

#endif // ATOM_H_INCLUDED

