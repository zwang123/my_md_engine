#ifndef ATOM_VECTOR_H_INCLUDED
#define ATOM_VECTOR_H_INCLUDED

#include <array>
#include <memory>
#include <vector>

class AtomVector final
{
public:
  using Value              = double;
  using ValueVector        = std::vector<Value>;
  using size_type          = ValueVector::size_type;
  using Type               = ValueVector::size_type;
  using TypeVector         = std::vector<Type>;
  using ValueIterator      = ValueVector::iterator;
  using ConstValueIterator = ValueVector::const_iterator;
  using TypeIterator       = TypeVector::iterator;
  using ConstTypeIterator  = TypeVector::const_iterator;
  static constexpr const size_type dim = 1;
  using ValueArray         = std::array<Value, dim>;
private:
  // Expand mass Vector to 3N dimensional?
  ValueVector massVector;
  ValueVector positionVector;
  ValueVector velocityVector;
  ValueVector accelerationVector;
  TypeVector  typeVector;

  ValueVector forceVector;
public:

#ifdef DEBUG
  std::shared_ptr<class Atom> addAtom();
#endif // DEBUG

  std::shared_ptr<class Atom> addAtom(const Type, const Value, 
      const ValueArray &, const ValueArray & = ValueArray{{0.0}}, 
      const ValueArray & = ValueArray{{0.0}});

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

  size_type size() const { return forceVector.size(); }
  size_type atomNumber() const { return typeVector.size(); }

  bool check() {return 
    massVector.size() == positionVector.size() &&
    massVector.size() == velocityVector.size() &&
    massVector.size() == accelerationVector.size() &&
    massVector.size() == dim * typeVector.size();
  }

  std::pair<std::vector<size_type>, ValueVector> slicePositionVector(const std::vector<size_type> &) const;

  void clearForce() {forceVector.assign(forceVector.size(), 0.0);}
};

#endif // ATOM_VECTOR_H_INCLUDED
