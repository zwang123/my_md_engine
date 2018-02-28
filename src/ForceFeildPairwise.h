#ifndef FORCE_FIELD_PAIRWISE_H_INCLUDED
#define FORCE_FIELD_PAIRWISE_H_INCLUDED

#include "ForceFeild.h"
#include <map>

class Function;
class ForceFieldPairwise
: public ForceField {
  using AtomTypePair = std::pair<size_t, size_t>;

  std::map<AtomTypePair, std::shared_ptr<Function>> potential_map;

  // User make sure AtomVec.size() == 2
  double potential(const AtomVec &) final;
  double force(const AtomVec &) final;
  virtual std::vector<double> genInput(const AtomVec &) const = 0;
  // Two dim tensor
  virtual std::vector<double> devInput(const AtomVec &) const = 0;
  virtual ~ForceFieldPairwise() {}
};

#endif // FORCE_FIELD_PAIRWISE_H_INCLUDED
