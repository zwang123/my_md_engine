#ifndef __FUNCTION_H_INCLUDED
#define __FUNCTION_H_INCLUDED

#include <string>
#include <utility>
#include <vector>
#include "SetupCommand.h"

#include "TimeStep.h"
#include <fstream>
//#include <iosfwd>

//namespace function {

class Function
: public SetupCommand
{
  //size_t nArguments = 0;
  double value = 0.0;
  std::vector<double> derivative;
  //bool updated = false;
  
  // output
  const TimeStep::StepType nevery;
  const TimeStep::StepType flush_every;

  std::ofstream ofs;
  //std::ostream &os;
protected:
  void setValue(const double v) { value = v; }
  // User make sure not out of range
  void setDerivative(const size_t i, const double v) {derivative[i] = v;}
  // User make sure length consistency
  void setDerivative(const std::vector<double> &d) {derivative = d;}
  // User make sure length consistency
  void setDerivative(std::vector<double> &&d) noexcept 
  {derivative = std::move(d);}
//  void setDerivative(Value*,int,double);
//  void addValueWithDerivatives();
//  void addComponentWithDerivatives( const std::string& name );

  // Reset derivative
  void setNumberOfArguments(const size_t n) {derivative.resize(n);}
  size_t getNumberOfArguments() const {return derivative.size();}
public:
  explicit Function(const class CommandOption &);
  ~Function() override;
  //void apply();
  virtual void calculate(const std::vector<double> &) = 0;
  //static void registerKeywords(Keywords&);
  //unsigned getNumberOfDerivatives();
  void error(const std::string &);
  double getValue() const { return value; }
  // User make sure not out of range
  double getDerivative(const size_t i) const {return derivative[i];}
  const std::vector<double> &getDerivative() const {return derivative;}
  void write();
};

//inline
//void Function::setDerivative(Value*v,int i,double d) {
//  v->addDerivative(i,d);
//}
//
//inline
//void Function::setDerivative(int i,double d) {
//  setDerivative(getPntrToValue(),i,d);
//}
//
//inline
//unsigned Function::getNumberOfDerivatives() {
//  return getNumberOfArguments();
//}

//}

#endif // __FUNCTION_H_INCLUDED
