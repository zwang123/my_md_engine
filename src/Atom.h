#ifndef ATOM_H_INCLUDED
#define ATOM_H_INCLUDED

#include "AtomVector.h"

// This is dependent upon the underlying AtomVector
// If the AtomVector is destructed, the Atom object becomes invalid
class Atom final {
public:
  using Value              = AtomVector::Value;
  using ValueVector        = AtomVector::ValueVector;
  using Type               = AtomVector::Type;
  using size_type          = AtomVector::size_type;
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
  static constexpr const size_type dim = AtomVector::dim;

  Type getType() const {return *type;}
  Value getMass() const {return *mass;}
  ValueVector getPosition() const 
  {return ValueVector(position, position + dim);}
  ValueVector getVelocity() const 
  {return ValueVector(velocity, velocity + dim);}
  ValueVector getAcceleration() const 
  {return ValueVector(acceleration, acceleration + dim);}
  ValueVector getForce() const 
  {return ValueVector(force, force + dim);}

  Atom (ConstTypeIterator type, ConstValueIterator mass, 
        ValueIterator position, ValueIterator velocity, 
        ValueIterator acceleration, ValueIterator force)
    : type(type), mass(mass), position(position), velocity(velocity)
    , acceleration(acceleration), force(force)
  {}
};

#endif // ATOM_H_INCLUDED

