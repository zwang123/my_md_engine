#include "backtrace.h"
#include "MD.h"

int main(int argc, char **argv)
{
  MD md(argc, argv);
  return md.run();
}
