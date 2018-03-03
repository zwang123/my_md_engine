#include "CommandGenerator.h"
#include "Function.h"
#include "Matheval.h"

#include <matheval.h>
#include <string>
#include <utility>
#include <vector>

using std::vector;
using std::string;

//namespace function {


REGISTER_COMMAND(Matheval)

//void Matheval::registerKeywords(Keywords& keys) {
//  Function::registerKeywords(keys);
//  keys.use("ARG"); keys.use("PERIODIC");
//  keys.add("compulsory","FUNC","the function you wish to evaluate");
//  keys.add("optional","VAR","the names to give each of the arguments in the function.  If you have up to three arguments in your function you can use x, y and z to refer to them.  Otherwise you must use this flag to give your variables names.");
//}

#ifdef DEBUG
#include <iostream>
#include "Engine.h"
#endif // DEBUG

Matheval::Matheval(const CommandOption &co):
  Function(co),
  var(parseVector<string>("VAR")),
  func(parseCompulsoryString("FUNC")),
  // Assume evaluator_create does not change func
  evaluator(evaluator_create(const_cast<char*>(func.c_str())))
{
  //if(var.empty()) {
  //  var.resize(getNumberOfArguments());
  //  if(getNumberOfArguments()>3)
  //    error("Using more than 3 arguments you should explicitly write their names with VAR");
  //  if(var.size()>0) var[0]="x";
  //  if(var.size()>1) var[1]="y";
  //  if(var.size()>2) var[2]="z";
  //}
  if(var.empty()) {
    error("NO variables");
  }
  if(func.empty()) {
    error("NO function");
  }
  if(!evaluator) error("There was some problem in parsing matheval formula "+func);

  setNumberOfArguments(var.size());
  //if(var.size()!=getNumberOfArguments())
    //error("Size of VAR array should be the same as number of arguments");
  //parse("FUNC",func);
  //addValueWithDerivatives();
  //checkRead();

  //evaluator=evaluator_create(const_cast<char*>(func.c_str()));


  char **check_names;
  int    check_count;
  evaluator_get_variables(evaluator,&check_names,&check_count);
#ifdef DEBUG
  //for (int i = 0; i < check_count; ++i)
  //  std::cout << check_names[i] << std::endl;
  //std::cout << std::endl;
  //for (const auto &x : var)
  //  std::cout << x << std::endl;
#endif // DEBUG
  if(check_count>static_cast<int>(getNumberOfArguments())) {
    //string sc;
    //Tools::convert(check_count,sc);
    error("Your function string contains " +
          std::to_string(check_count) +
          " arguments. This should be at most the number of ARGs");
  }


  for(int j=0; j<check_count; ++j) {
    bool found=false;
    for(unsigned i=0; i<getNumberOfArguments(); ++i) {
      if(var[i]==check_names[j])found=true;
    }
    if(!found)
      //error("Variable "+var[i]+" cannot be found in your function string");
      error("Variable "+std::string(check_names[j])+" cannot be found in your arg");
  }

  size_t n = getNumberOfArguments();
  //evaluator_deriv.resize();
  evaluator_deriv.reserve(n);
  for(size_t i=0; i!=n; ++i) {
    auto p = evaluator_derivative(evaluator,const_cast<char*>(var[i].c_str()));
    if (!p)
      error("Cannot calculate derivative for var" + var[i]);
    evaluator_deriv.push_back(p);
  }
    //evaluator_deriv[i]=evaluator_derivative(evaluator,const_cast<char*>(var[i].c_str()));
  //for(auto & x : evaluator_deriv)
    //x=evaluator_derivative(evaluator,const_cast<char*>(var[i].c_str()));


  //log.printf("  with function : %s\n",func.c_str());
  //log.printf("  with variables :");
  //for(unsigned i=0; i<var.size(); i++) log.printf(" %s",var[i].c_str());
  //log.printf("\n");
  //log.printf("  function as parsed by matheval: %s\n", evaluator_get_string(evaluator));
  //log.printf("  derivatives as computed by matheval:\n");
  //for(unsigned i=0; i<var.size(); i++) log.printf("    %s\n",evaluator_get_string(evaluator_deriv[i]));
}

// Assume matheval does not change names or values
void Matheval::calculate(const vector<double> &values)
{
  //for(unsigned i=0; i<getNumberOfArguments(); i++) values[i]=getArgument(i);
  const size_t n = getNumberOfArguments();
  names.resize(n);
  for(size_t i=0; i!=n; ++i)
    names[i]=const_cast<char*>(var[i].c_str());
  setValue(evaluator_evaluate(evaluator,n,const_cast<char**>(names.data()),
                              const_cast<double*>(values.data())));

  //for (auto & x : evaluator_deriv)
  for(size_t i=0; i!=n; i++) {
    setDerivative(i,evaluator_evaluate(evaluator_deriv[i],
                                       n,const_cast<char**>(names.data()),
                                       const_cast<double*>(values.data())));
    //setDerivative(i,evaluator_evaluate(evaluator_deriv[i],n,&names[0],&values[0]));
  }
}

//void Matheval::setValues(const vector<double> &v)
//{
//  if (v.size() != getNumberOfArguments())
//    error("Inconsistent value vector");
//  values = v;
//}
//
//// User check length consistency
//void Matheval::setValues(vector<double> &&v) noexcept
//{
//  values = std::move(v);
//}

Matheval::~Matheval()
{
  evaluator_destroy(evaluator);
  for (auto & x : evaluator_deriv) evaluator_destroy(x);
}

//void Matheval::run()
//{
////TODO
//#ifdef DEBUG
//  std::cout << label << std::endl;
//  std::cout << (dynamic_cast<Matheval*>(engine->selectFromLabel("@L1").get())
//      == this) << std::endl;
//
//  //setValues(vector<double>{{1,2,3}});
//  //calculate();
//  if (getNumberOfArguments() == 0)
//    return;
//  vector<double> values{{2. , 3.1415926 * .5, 3. , 2.71828}};
//  calculate(values);
//
//  std::cout << "f(";
//  for (size_t i = 0; i != getNumberOfArguments() - 1; ++i)
//    std::cout << var[i] << ",";
//  std::cout << var.back() << ") = " << func << std::endl;
//
//  for (size_t i = 0; i != getNumberOfArguments(); ++i)
//    std::cout << var[i] << " = " << values[i] << std::endl;
//
//  std::cout << "Results:" << std::endl;
//  std::cout << "f = " << getValue() << std::endl;
//
//  for (size_t i = 0; i != getNumberOfArguments(); ++i)
//    std::cout << "df/d" << var[i] << " = " << getDerivative(i) << std::endl;
//#endif // DEBUG
//}


//}
//}


