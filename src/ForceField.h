#ifndef FORCE_FIELD_H_INCLUDED
#define FORCE_FIELD_H_INCLUDED

#include <memory>
#include <vector>

//class Atom;
class AtomVector;
//class ForceField {
//public:
//  using AtomVec = std::vector<std::shared_ptr<Atom>>;
//  // Potential of a group of atoms
//  virtual double potential(const AtomVec &) = 0;
//  // Force exerted on the first atom by others
//  virtual double force(const AtomVec &) = 0;
//
//  virtual ~ForceField() {}
//};

class ForceField final {
  friend class PotentialEnergy;
  std::vector<std::shared_ptr<class PotentialEnergy>> pes;
  double energy = 0.0;
  std::vector<double> force;
public:
  // Potential of a group of atoms
  double getPotential() const noexcept {return energy;}
  // Force vector
  std::vector<double> getForce() const noexcept {return force;}
  void calculate(std::shared_ptr<const AtomVector>);
  void apply(std::shared_ptr<AtomVector>) const;

};

#endif // FORCE_FIELD_H_INCLUDED
