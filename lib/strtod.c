#include <string.h>

extern char isspace(char c);

#ifdef __GNUC__
static inline char isdigit(char c) { return (c>='0' && c<='9'); }
#endif

double strtod(const char *nptr, char **endptr) {
  double d=0;
  register const char *c=nptr;
  char neg=0;
  while (*c && isspace(*c)) ++c;
  switch (*c) {
  case '-': neg=1;
  case '+': c++; break;
  default: break;
  }
  while (isdigit(*c)) {
    d=d*10+(*c-'0');
    ++c;
  }
  if (*c=='.') {
    double factor=.1;
    while (isdigit(*++c)) {
      d=d+(factor*(*c-'0'));
      factor/=10;
    }
  }
  if ((*c|32)=='e') {
    int exp=0;
    double factor=10;
    if (c[1]<'0') {
      switch (c[1]) {
      case '-': factor=0.1;
      case '+': c++; break;
      default:
	d=0;
	c=nptr;
	goto done;
      }
    }
    while (isdigit(*++c))
      exp=exp*10+(*c-'0');
    while (exp) {	/* XXX: this introduces rounding errors */
      d*=factor; --exp;
    }
  }
done:
  if (endptr) *endptr=(char*)c;
  return d;
}
