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
: private std::enable_shared_from_this<Engine>
{
  friend class Command;
  std::vector<std::shared_ptr<Command>> cmd;

  std::shared_ptr<class System> sys;
  std::shared_ptr<class Integrator> integrator;
  std::shared_ptr<class Dynamics> dyn;

  constexpr static const char *COMMENT = "#";
  std::ostream &os;
public:
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
private:
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
public:
  // Select Commands based on label
  std::shared_ptr<Command> selectFromLabel(std::string) const noexcept;
};

#endif // ENGINE_H_INCLUDED
