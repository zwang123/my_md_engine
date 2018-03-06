#include "AtomVector.h"
#include "CommandGenerator.h"
#include "RandomNumberGenerator.h"
#include "TimeSeed.h"
#include "VelocityInitializer.h"

REGISTER_COMMAND(VelocityInitializer)

VelocityInitializer::VelocityInitializer(const class CommandOption &co)
  : SetupCommand(co)
  , temp(parse<double>("TEMP"))
  , seed(parseOptional("SEED", TimeSeed::gen_seed<seed_type>()))
  //, rng(seed, 0.0, ?)
  , rng(seed)
{
}


void VelocityInitializer::setVelocity(std::shared_ptr<AtomVector> av) const
{
  auto &vel = av->getVelocityVector();
}
