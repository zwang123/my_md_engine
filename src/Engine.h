#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Command;

// The class that stores all infos required by simulations
class Engine final
: public std::enable_shared_from_this<Engine>
{
  friend class Command;
  std::vector<std::shared_ptr<Command>> cmd;

  std::shared_ptr<class System> sys;
  std::shared_ptr<class Integrator> integrator;
  std::shared_ptr<class Dynamics> dyn;

  std::ostream &os;
public:
  constexpr static const char *COMMENT = "#";
  // Construct the empty engine
  explicit Engine(std::ostream &os = std::cout) : os(os) {}

  // Cannot do these because read(is) uses shared_from_this()
  //// Construct the engine from filename
  //explicit Engine(const std::string &filename,
  //    std::ostream &os = std::cout) : Engine(filename.c_str(), os) {}
  //// Construct the engine from filename
  //explicit Engine(const char *filename,
  //    std::ostream &os = std::cout) : os(os)
  //{std::ifstream ifs (filename); read(ifs); ifs.close();}
  //// Construct the engine from an istream
  //explicit Engine(std::istream &is, 
  //    std::ostream &os = std::cout) : os(os)
  //{read(is);}

  // Read file and construct commands
  std::istream &read(std::istream &);
  void setup();
  // Run the commands
  void run(size_t = 1);

  std::shared_ptr<class System> getSystem() const noexcept {return sys;}
  std::shared_ptr<class Integrator> getIntegrator() const noexcept {return integrator;}
  std::shared_ptr<class Dynamics> getDynamics() const noexcept {return dyn;}
  std::ostream &getOstream() noexcept {return os;}

public:
  // Select Commands based on type
  template <typename T>
  std::vector<std::shared_ptr<T>> select()
  const
  { 
    std::vector<std::shared_ptr<T>> result;
    result.reserve(cmd.size());
    for (const auto &x : cmd) {
      auto p = std::dynamic_pointer_cast<T>(x);
      if (p) result.push_back(p);
    }
    return result;
  }

private:

  template <typename T>
  bool checkDuplicate() const
  {
    return select<T>().size() > 1;
  }

  template <typename T>
  void setPtr(std::shared_ptr<T> &p, std::string cmdName)
  {
    std::shared_ptr<T> curr_ptr;
    if ((curr_ptr = std::dynamic_pointer_cast<T>(cmd.back()))) {
      if (p) {
        throw std::invalid_argument("Two " + cmdName + " commands");
      } else {
        p = curr_ptr;
      }
    }
  }

public:
  // Select Commands based on label
  std::shared_ptr<Command> selectFromLabel(const std::string &) const noexcept;

  template <typename T>
  std::shared_ptr<T> selectFromLabel(const std::string &label) const noexcept
  { return std::dynamic_pointer_cast<T>(selectFromLabel(label)); }

  template <typename T>
  std::vector<std::shared_ptr<T>> selectFromLabelVector(const std::vector<std::string> &labels) const noexcept
  { 
    std::vector<std::shared_ptr<T>> result;
    for (const auto &s : labels)
      result.push_back(selectFromLabel<T>(s));
    return result;
  }
};

#endif // ENGINE_H_INCLUDED
