#include <cassert>
#include <stdexcept>
#include "Engine.h"
#include "Function.h"
#include "Integrator.h"
#include "TimeStep.h"

//void Function::addValueWithDerivatives() {
//  plumed_massert( getNumberOfArguments()!=0, 
//       "for functions you must requestArguments before adding values");
//  ActionWithValue::addValueWithDerivatives();
//  getPntrToValue()->resizeDerivatives(getNumberOfArguments());
//
//  if( keywords.exists("PERIODIC") ) {
//    std::vector<std::string> period;
//    parseVector("PERIODIC",period);
//    if(period.size()==1 && period[0]=="NO") {
//      setNotPeriodic();
//    } else if(period.size()==2) {
//      setPeriodic(period[0],period[1]);
//    } else error("missing PERIODIC keyword");
//  }
//}
//
//void Function::addComponentWithDerivatives( const std::string& name ) {
//  plumed_massert( getNumberOfArguments()!=0, 
//        "for functions you must requestArguments before adding values");
//  ActionWithValue::addComponentWithDerivatives(name);
//  getPntrToComponent(name)->resizeDerivatives(getNumberOfArguments());
//}

//void Function::apply()
//{
//  const unsigned noa=getNumberOfArguments();
//  const unsigned ncp=getNumberOfComponents();
//  const unsigned cgs=comm.Get_size();
//
//  vector<double> f(noa,0.0);
//
//  unsigned stride=1;
//  unsigned rank=0;
//  if(ncp>4*cgs) {
//    stride=comm.Get_size();
//    rank=comm.Get_rank();
//  }
//
//  unsigned at_least_one_forced=0;
//  #pragma omp parallel num_threads(OpenMP::getNumThreads()) shared(f)
//  {
//    vector<double> omp_f(noa,0.0);
//    vector<double> forces(noa);
//    #pragma omp for reduction( + : at_least_one_forced)
//    for(unsigned i=rank; i<ncp; i+=stride) {
//      if(getPntrToComponent(i)->applyForce(forces)) {
//        at_least_one_forced+=1;
//        for(unsigned j=0; j<noa; j++) omp_f[j]+=forces[j];
//      }
//    }
//    #pragma omp critical
//    for(unsigned j=0; j<noa; j++) f[j]+=omp_f[j];
//  }
//
//  if(noa>0&&ncp>4*cgs) { comm.Sum(&f[0],noa); comm.Sum(at_least_one_forced); }
//
//  if(at_least_one_forced>0) for(unsigned i=0; i<noa; ++i) 
//      getPntrToArgument(i)->addForce(f[i]);
//}

Function::Function(const class CommandOption &co) 
  : SetupCommand(co)
  //, fname(parseString("FILE"))
  , ofs(parseString("FILE"))
  //, ofs(fname)
  //, os(fname.empty()?engine->getOstream():ofs)
{
  auto rtn = parseOptional<TimeStep::StepType> ("EVERY");
  nevery = rtn.second ? 1 : rtn.first;

  rtn = parseOptional<TimeStep::StepType> ("FLUSH");
  flush_every = rtn.second ? 10000 : rtn.first;
  
  assert(flush_every % nevery == 0);
  //assert(fname.empty() || ofs.is_open());
  ofs << "# Time    " 
      << label + ".value    " << label + ".derivatives" 
      << std::endl;
}

Function::~Function()
{
  if (ofs.is_open())
    ofs.close();
}

void Function::error(const std::string &errMsg)
{
  throw std::runtime_error(errMsg);
}

// User make sure the values are updated
void Function::write()
{
  //std::cout << __LINE__ << std::endl;
  if (!ofs.is_open()) return;
  const auto ts = engine->getIntegrator()->getTimeStep();
  //std::cout << __LINE__ << std::endl;

  if (ts.getStep() % nevery) return;
  //std::cout << __LINE__ << std::endl;

  static constexpr const char *delim = "  ";

  //using ConstValueIterator = AtomVector::ConstValueIterator;
  ofs << ts.getTime() << delim << getValue();
  auto n = getNumberOfArguments();
  for (decltype(n) i = 0; i != n; ++i) {
    ofs << delim << getDerivative(i);
  }
  ofs << std::endl;

  if (ts.getStep() % flush_every == 0) ofs.flush();
}
