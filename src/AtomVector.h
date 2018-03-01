#ifndef ATOM_VECTOR_H_INCLUDED
#define ATOM_VECTOR_H_INCLUDED

#include <memory>
#include <vector>

class AtomVector final
{
public:
  using Value              = double;
  using ValueVector        = std::vector<double>;
  using Type               = ValueVector::size_type;
  using TypeVector         = std::vector<Type>;
  using ValueIterator      = ValueVector::iterator;
  using ConstValueIterator = ValueVector::const_iterator;
  using TypeIterator       = TypeVector::iterator;
  using ConstTypeIterator  = TypeVector::const_iterator;
private:
  // Expand mass Vector to 3N dimensional?
  ValueVector massVector;
  ValueVector positionVector;
  ValueVector velocityVector;
  ValueVector accelerationVector;
  TypeVector  typeVector;

  ValueVector forceVector;
public:

  static constexpr const Type dim = 1;
#ifdef DEBUG
  std::shared_ptr<class Atom> addAtom();
#endif // DEBUG

  std::shared_ptr<class Atom> addAtom(const Type, const Value, const Value, const Value = 0.0, const Value = 0.0);

  ValueVector &getMassVector() {return massVector;}
  ValueVector &getPositionVector() {return positionVector;}
  ValueVector &getVelocityVector() {return velocityVector;}
  ValueVector &getAccelerationVector() {return accelerationVector;}
  TypeVector  &getTypeVector() {return typeVector;}
  ValueVector &getForceVector() {return forceVector;}
  const ValueVector &getConstMassVector() const {return massVector;}
  const ValueVector &getConstPositionVector() const {return positionVector;}
  const ValueVector &getConstVelocityVector() const {return velocityVector;}
  const ValueVector &getConstAccelerationVector() const {return accelerationVector;}
  const TypeVector  &getConstTypeVector() const {return typeVector;}
  const ValueVector &getConstForceVector() const {return forceVector;}

  bool check() {return 
    dim * massVector.size() == positionVector.size() &&
    dim * massVector.size() == velocityVector.size() &&
    dim * massVector.size() == accelerationVector.size() &&
    massVector.size() == typeVector.size();
  }
};

#endif // ATOM_VECTOR_H_INCLUDED
