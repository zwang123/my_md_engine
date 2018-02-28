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
private:
  ValueVector massVector;
  ValueVector positionVector;
  ValueVector velocityVector;
  ValueVector accelerationVector;
  TypeVector typeVector;
public:
  std::shared_ptr<class Atom> addAtom();
  std::shared_ptr<class Atom> addAtom(const Value, const Value, const Value, const Value, const Type);
};

#endif // ATOM_VECTOR_H_INCLUDED
