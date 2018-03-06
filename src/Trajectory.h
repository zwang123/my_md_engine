#ifndef TRAJECTORY_H_INCLUDED
#define TRAJECTORY_H_INCLUDED

#include "TimeStep.h"
#include "SetupCommand.h"
#include <iosfwd>
#include <memory>
#include <string>
#include <fstream>

// Must be after integrator and system
class Trajectory final
: public SetupCommand {
  const std::string fname;
  const TimeStep::StepType nevery;
  const TimeStep::StepType flush_every;


  std::ofstream ofs;
  std::ostream &os;

  std::shared_ptr<const class AtomVector> av;

#ifdef THERMO
  std::ofstream ofs2;
  std::ostream &os2;
#endif
public:
  explicit Trajectory(const class CommandOption &);
  ~Trajectory();
  void write() const;
  static constexpr const char *directive = "TRAJECTORY";
};

#endif // TRAJECTORY_H_INCLUDED
