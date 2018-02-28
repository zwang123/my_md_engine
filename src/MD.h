#ifndef MD_H_INCLUDED
#define MD_H_INCLUDED

#include <fstream>
#include <memory>

class Engine;

// The class that de-codes the command-line options
// and run the engine
class MD {
  std::ifstream ifs;
  std::ofstream ofs;
  std::shared_ptr<Engine> engine;

  void print_help();
public:
  explicit MD(int, char **);
  ~MD();
  int run();
};

#endif // MD_H_INCLUDED
