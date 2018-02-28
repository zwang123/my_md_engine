#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

#include <memory>
#include <string>
#include <vector>

//namespace command {

class Command
{
protected:
  std::shared_ptr<class Engine> engine;
  std::vector<std::string> lines;
  std::string label;

  bool setup_flag = true;
  bool run_flag = false;

  std::string findKey(const char *);
public:
  std::string getLabel() const {return label;}
  std::string parseString(const char *);
  double parseValue(const char *);
  template <typename T> T parse(const char *);
  template <typename T> std::pair<T, int> parseOptional(const char *);
  std::vector<std::string> parseStringVector(const char *);
  // lines must be already trimmed
  explicit Command(const class CommandOption &);
  virtual ~Command() {}
  virtual void run() = 0;
  virtual void setup() {setup_flag = false;}

  bool currStep() const {return run_flag;}
  bool requireSetup() const {return setup_flag;}
};
//}

#endif // COMMAND_H_INCLUDED
