#ifndef FORCE_FIELD_H_INCLUDED
#define FORCE_FIELD_H_INCLUDED

#include <memory>
#include <vector>

class Atom;
class ForceField {
public:
  using AtomVec = std::vector<std::shared_ptr<Atom>>;
  // Potential of a group of atoms
  virtual double potential(const AtomVec &) = 0;
  // Force exerted on the first atom by others
  virtual double force(const AtomVec &) = 0;

  virtual ~ForceField() {}
};

#endif // FORCE_FIELD_H_INCLUDED
