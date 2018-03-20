#ifndef __ATOM_VECTOR_H_INCLUDED
#define __ATOM_VECTOR_H_INCLUDED

#include <array>
#include <memory>
#include <valarray>
#include <vector>
#include <map>

#ifndef DIMENSIONALITY
#  define DIMENSIONALITY 3
#endif

class AtomVector final {
public:
  using Value              = double;
  using ValueVector        = std::vector<Value>;
  using size_type          = ValueVector::size_type; // atom index type
  using Type               = ValueVector::size_type;
  using TypeVector         = std::vector<Type>;
  using ValueIterator      = ValueVector::iterator;
  using ConstValueIterator = ValueVector::const_iterator;
  using TypeIterator       = TypeVector::iterator;
  using ConstTypeIterator  = TypeVector::const_iterator;
  static constexpr const size_type dim = DIMENSIONALITY;
  using ValueArray         = std::array<Value, dim>;
  using ValueValarray      = std::valarray<Value>;
  using ValueMatrix        = std::valarray<ValueValarray>;

  using TypeMap            = std::map<Type, std::vector<size_type>>;
private:
  // Expand mass Vector to 3N dimensional?
  ValueVector massVector;
  ValueVector positionVector;
  ValueVector velocityVector;
  ValueVector accelerationVector;
  TypeVector  typeVector;

  ValueVector forceVector;
#if (DIMENSIONALITY == 3)
  ValueValarray solveLinear (const ValueValarray &, 
                             const ValueValarray &) const;
#endif

  TypeMap     type2index;
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
  const ValueVector &getConstAccelerationVector() const
  {return accelerationVector;}
  const TypeVector  &getConstTypeVector() const {return typeVector;}
  const ValueVector &getConstForceVector() const {return forceVector;}

  const std::vector<size_type> &convertType2Index(const Type type) const 
  {return type2index.find(type)->second;}

  size_type size() const { return forceVector.size(); }
  size_type atomNumber() const { return typeVector.size(); }

  bool check() {return 
    massVector.size() == positionVector.size() &&
    massVector.size() == velocityVector.size() &&
    massVector.size() == accelerationVector.size() &&
    massVector.size() == dim * typeVector.size();
  }

  std::pair<std::vector<size_type>, ValueVector> 
    slicePositionVector(const std::vector<size_type> &) const;

  void clearForce() {forceVector.assign(forceVector.size(), 0.0);}

  std::shared_ptr<class Atom> operator[] (size_type);

  Value kinetic() const;
  ValueArray momentum() const;
  ValueValarray angular_momentum() const;
  ValueValarray angular_velocity() const;
  ValueValarray center_of_mass() const;

#if (DIMENSIONALITY == 2) || (DIMENSIONALITY == 3)
  ValueValarray rotational_inertia() const;
#endif
};

#endif // __ATOM_VECTOR_H_INCLUDED
