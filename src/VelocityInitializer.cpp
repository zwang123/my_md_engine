#include "AtomVector.h"
#include "CommandGenerator.h"
#include "Constants.h"
#include "Engine.h"
#include "RandomNumberGenerator.h"
#include "System.h"
#include "TimeSeed.h"
#include "Tools.h"
#include "VelocityInitializer.h"
#include <cassert>
#include <cmath>

#ifdef DEBUG
#include <iostream>
#include "print_vector.h"
#endif // DEBUG

REGISTER_COMMAND(VelocityInitializer)

VelocityInitializer::VelocityInitializer(const class CommandOption &co)
  : SetupCommand(co)
  , temp(parse<double>("TEMP"))
  , seed(parseOptional("SEED", TimeSeed::gen_seed<seed_type>()))
  , is_scale(Tools::toupper_cpy(parseString("SCALE")) == "TRUE")
  , is_zero_linear(Tools::toupper_cpy(parseString("ZERO_LINEAR")) == "TRUE")
  , is_zero_angular(AtomVector::dim > 1 && is_zero_linear && 
                    Tools::toupper_cpy(parseString("ZERO_ANGULAR")) == "TRUE")

{
  assert(engine->getSystem());
  const std::shared_ptr<const AtomVector> av 
    = engine->getSystem()->getAtomVector();
  assert(av);

  // Avoid seedding after generating distributions
  rng_type::seed(seed);
  rngs.reserve(av->size());
  for (const auto &mass : av->getConstMassVector()) {
    rngs.emplace_back(0.0, sqrt(Constants::boltzmann * temp / mass));
  }
}

void VelocityInitializer::setVelocity(std::shared_ptr<AtomVector> av)
{
  static constexpr const auto dim = AtomVector::dim;
  using ValueArray = AtomVector::ValueArray;
  using ValueValarray = AtomVector::ValueValarray;
  using size_type = AtomVector::size_type;

  auto &vel = av->getVelocityVector();
  assert(rngs.size() == vel.size());
  auto prng = rngs.begin();
  auto pe = vel.end();
  for (auto pvel = vel.begin(); pvel != pe; ++prng, ++pvel)
    *pvel = (*prng)();

  if (is_zero_linear) {
#ifdef DEBUG
    std::cout << "Zero Linear" << std::endl;
#endif // DEBUG
    auto tot_momentum = av->momentum();
    auto inv_tot_mass = dim / Tools::sum(av->getConstMassVector());
#ifdef DEBUG
    std::cout << "inv mass (should be " << 1.0/(0.5*av->atomNumber())
      << ") " << inv_tot_mass << std::endl;
    printVector(tot_momentum) << std::endl;
#endif // DEBUG
    ValueArray vel_adjust;
    for (size_type i = 0; i != dim; ++i)
      vel_adjust[i] = tot_momentum[i] * inv_tot_mass;
    for (auto pvel = vel.begin(); pvel != pe;)
      for (size_type i = 0; i != dim; ++i)
        (*pvel++) -= vel_adjust[i];
  }

  if (is_zero_angular) {
#ifdef DEBUG
    std::cout << "Zero Angular" << std::endl;
    printVector(av->angular_momentum()) << std::endl;
                                           //mass  box  size
    std::cout << "Rot inertia (should be " << 0.5 * 3.0 * 3.0 / 12 * av->size() 
      << ") " << av->rotational_inertia()
      << std::endl;
#endif // DEBUG
    ValueValarray angular_velocity (av->angular_momentum());
    angular_velocity /= av->rotational_inertia();
#ifdef DEBUG
    printVector(angular_velocity) << std::endl;
#endif // DEBUG
    auto pr = av->getConstPositionVector().data();
    auto com = av->center_of_mass();

#ifdef DEBUG
    ValueValarray tot_r(0.0, dim);
#endif // DEBUG
    for (auto pvel = vel.begin(); pvel != pe; pr += dim) {
      ValueValarray r(pr, dim);
      r -= com;
#ifdef DEBUG
      tot_r += r * av->getConstMassVector()[pvel - vel.begin()];
#endif // DEBUG
      auto vel_adjust = Tools::cross2<dim>(angular_velocity, r);
      for (size_type i = 0; i != dim; ++i)
        (*pvel++) -= vel_adjust[i];
    }
#ifdef DEBUG
    printVector(tot_r) << std::endl;
#endif // DEBUG
  }

  if (is_scale) {
#ifdef DEBUG
    std::cout << "Scale" << std::endl;
#endif // DEBUG
    auto cal_ke = av->kinetic();
    auto ref_ke = .5 * av->size() * Constants::boltzmann * temp;
    auto vel_multiplier = sqrt(ref_ke / cal_ke);
    for (auto pvel = vel.begin(); pvel != pe; ++pvel)
      *pvel *= vel_multiplier;
  }
}
