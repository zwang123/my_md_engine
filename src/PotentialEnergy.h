#ifndef __POTENTIAL_ENERGY_H_INCLUDED
#define __POTENTIAL_ENERGY_H_INCLUDED

#include "AtomVector.h"
#include "SetupCommand.h"
#include <map>
#include <memory>
#include <string>

class PotentialEnergy final
: public std::enable_shared_from_this<PotentialEnergy>,
  public SetupCommand {
  using size_type = AtomVector::size_type;

  std::shared_ptr<class Function> fp;
  std::vector<std::shared_ptr<class Function>> inputFunction;

  std::map<std::vector<size_type>, size_type> potential_map;
  std::vector<std::vector<double>> parameters;

  double energy = 0.0;
  std::vector<double> force;

  void constructMapFromType(const std::string &);
  void constructMap(const std::string &);
  void constructPar(const std::string &);
  bool checkInput() const;
public:
  PotentialEnergy(const class CommandOption &co);
  void postConstruct() override;
  // Potential of a group of atoms
  double getPotential() const noexcept {return energy;}
  // Force vector
  const std::vector<double> &getForce() const noexcept {return force;}
  void calculate(std::shared_ptr<const AtomVector>);

  static constexpr const char *directive = "POTENTIAL_ENERGY";
};

#endif // __POTENTIAL_ENERGY_H_INCLUDED
