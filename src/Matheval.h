#ifndef MATHEVAL_H_INCLUDED
#define MATHEVAL_H_INCLUDED

#include "Function.h"
#include <string>
#include <vector>

class CommandOption;
class Matheval final :
  public Function
{
  // names of the input variables
  std::vector<std::string> var;
  // the string of the function to be evaluated
  const std::string func;
  // the evaluator that evaluates the function value
  void* evaluator = nullptr;
  // evaluators that evaluate the derivatives
  std::vector<void*> evaluator_deriv;

  //// values of the input variables
  //vector<double> values;
  // a duplicate of names of the input variables
  std::vector<char*> names;
public:
  explicit Matheval(const CommandOption &);
  ~Matheval();
  void calculate(const std::vector<double> &) override;
  //void setValues(const vector<double> &);
  //void setValues(vector<double> &&) noexcept;
  //static void registerKeywords(Keywords& keys);
  //void run() override;

  static constexpr const char *directive = "MATHEVAL";
};

#endif // MATHEVAL_H_INCLUDED
