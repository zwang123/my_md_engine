#ifndef __TIME_SEED_H_INCLUDED
#define __TIME_SEED_H_INCLUDED

#include <chrono>

class TimeSeed final {
public:
  using clock_type = std::chrono::high_resolution_clock;
  using rep = clock_type::rep;

  static rep gen_seed() 
  {return clock_type::now().time_since_epoch().count();}

  template <typename T>
  static T gen_seed() 
  {return static_cast<T>(gen_seed());}
};

#endif // __TIME_SEED_H_INCLUDED
