#ifndef __ATOM_H_INCLUDED
#define __ATOM_H_INCLUDED

#include "AtomVector.h"

// This is dependent upon the underlying AtomVector
// If the AtomVector is destructed, the Atom object becomes invalid
class Atom final {
public:
  using Value              = AtomVector::Value;
  //using ValueVector        = AtomVector::ValueVector;
  using ValueArray         = AtomVector::ValueArray;
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
  ValueArray getPosition() const 
  {ValueArray rtn; 
    for (size_type i = 0; i != dim; ++i) rtn[i] = position[i]; return rtn;}
  ValueArray getVelocity() const 
  {ValueArray rtn; 
    for (size_type i = 0; i != dim; ++i) rtn[i] = velocity[i]; return rtn;}
  ValueArray getAcceleration() const 
  {ValueArray rtn; 
    for (size_type i = 0; i != dim; ++i) rtn[i] = acceleration[i]; return rtn;}
  ValueArray getForce() const 
  {ValueArray rtn; 
    for (size_type i = 0; i != dim; ++i) rtn[i] = force[i]; return rtn;}
  //ValueVector getPosition() const 
  //{return ValueVector(position, position + dim);}
  //ValueVector getVelocity() const 
  //{return ValueVector(velocity, velocity + dim);}
  //ValueVector getAcceleration() const 
  //{return ValueVector(acceleration, acceleration + dim);}
  //ValueVector getForce() const 
  //{return ValueVector(force, force + dim);}

  Atom (ConstTypeIterator type, ConstValueIterator mass, 
        ValueIterator position, ValueIterator velocity, 
        ValueIterator acceleration, ValueIterator force)
    : type(type), mass(mass), position(position), velocity(velocity)
    , acceleration(acceleration), force(force)
  {}
};

#endif // __ATOM_H_INCLUDED

