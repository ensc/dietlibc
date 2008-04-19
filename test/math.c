#include <math.h>
#include <float.h>
#include <assert.h>

int main()
{
  extern int __isinf(double d);
  extern int __isnan(double d);

#if (__GNUC__ > 3) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 3))
  assert(__isinf(__builtin_inff())  == +1);
  assert(__isinf(-__builtin_inff()) == -1);

  assert(__isinf(__builtin_inf())   == +1);
  assert(__isinf(-__builtin_inf())  == -1);

  assert(__isnan(__builtin_nan("")));
#endif

  assert(__isinf((DBL_MAX * DBL_MAX))  == +1);
  assert(__isinf(-(DBL_MAX * DBL_MAX)) == -1);

  assert(isinf((DBL_MAX * DBL_MAX)));
  assert(isinf(-(DBL_MAX * DBL_MAX)));

  //assert(isnan(nan("")));

  return 0;
}
