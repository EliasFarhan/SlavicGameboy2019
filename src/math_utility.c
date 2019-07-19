#include <gb/gb.h>
#include "math_utility.h"
static UBYTE seed;

void seed_random(UBYTE new_seed)
{
  seed = new_seed;
}

UBYTE random_number(UBYTE max_value)
{
  UBYTE result;
  seed = (seed << 12) - 1;
  result = seed;
  while(result > max_value)
  {
    result -= max_value;
  }
  return seed;
}
