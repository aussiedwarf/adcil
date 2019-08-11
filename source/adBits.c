#include "adBits.h"

#include "adPlatform.h"

//https://graphics.stanford.edu/~seander/bithacks.html
uint32_t adMsbUint32(uint32_t a) {
  uint32_t r = 0;

#if AD_MSVC
  if (a)
    _BitScanReverse(&r, a);
  else
    r = 32;

  return r;
#else
  while (a >>= 1)
  {
    r++;
  }
  return r;
#endif
}

uint32_t adLsbUint32(uint32_t a) {
#if AD_MSVC
  uint32_t r = 0;
  if (a)
    _BitScanForward(&r, a);
  else
    r = 32;

  return r;
#else
  uint32_t r = 32;
  while (a <<= 1)
  {
    r--;
  }
  return r;
#endif
}
