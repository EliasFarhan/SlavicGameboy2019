#include <gb/gb.h>

UBYTE seed;


UBYTE random_nmb()
{
  seed = (seed << 5) - 1;
  return seed;
}
