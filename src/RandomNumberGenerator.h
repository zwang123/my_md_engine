#ifndef RANDOM_NUMBER_GENERATOR_H_INCLUDED
#define RANDOM_NUMBER_GENERATOR_H_INCLUDED

//#include <limits>
#include <random>
#include <utility>

#ifdef __x86_64__
#  define DEFAULT_GENERATOR std::mt19937_64
#else
#  define DEFAULT_GENERATOR std::mt19937
#endif

// Default distribution: uniform in [0.0, 1.0)
// Default type: double
template <typename _Distribution = std::uniform_real_distribution<>,
          typename _Generator = DEFAULT_GENERATOR>
class RandomNumberGenerator final {
public:
  using result_type = typename _Distribution::result_type;
  using param_type = typename _Distribution::param_type;
  using seed_type = typename _Generator::result_type;

  // Constructor
  template <typename ...Args>
  explicit RandomNumberGenerator(
      seed_type seed = _Generator::default_seed,
      Args&&... args)
    : gen(seed), dis(std::forward<Args>(args)...)
  {}

  // Random number generator
  result_type operator() () {return dis(gen);}
  result_type operator() (const param_type &parm) {return dis(gen, parm);}
  
  // Reset
  void reset() {dis.reset();}

  // Parameters
  param_type param() const {return dis.param();}
  void param(const param_type &parm) {dis.param(parm);}

  // Min Max
  result_type min() const {return dis.min();}
  result_type max() const {return dis.max();}

  // set seed
  template <typename ...Args>
  void seed (Args&&... args) {gen.seed(std::forward<Args>(args)...);}

private:
  _Generator gen;
  _Distribution dis;
};

#endif // RANDOM_NUMBER_GENERATOR_H_INCLUDED
