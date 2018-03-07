#ifndef VELOCITY_INITIALIZER_H_INCLUDED
#define VELOCITY_INITIALIZER_H_INCLUDED

#include "RandomNumberGenerator.h"
#include "SetupCommand.h"
#include <memory>
#include <random>
#include <vector>

class VelocityInitializer final
: public SetupCommand {
public:
  using distribution_type = std::normal_distribution<double>;
  using rng_type = RandomNumberGenerator<distribution_type>;
  using seed_type = rng_type::seed_type;

private:
  const double temp;

  const seed_type seed;
  const bool is_scale;
  const bool is_zero_linear;
  const bool is_zero_angular;

  std::vector<rng_type> rngs;

public:
  VelocityInitializer(const class CommandOption &);
  void setVelocity(std::shared_ptr<class AtomVector>);
  static constexpr const char *directive = "VELOCITY";
};

#endif // VELOCITY_INITIALIZER_H_INCLUDED
