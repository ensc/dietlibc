#include <stdio.h>
/* convert double to string.  Helper for sprintf. */

int __dtostr(double d,char *buf,int maxlen,int prec);

int __dtostr(double d,char *buf,int maxlen,int prec) {
  unsigned long long *x=(unsigned long long *)&d;
  /* step 1: extract sign, mantissa and exponent */
  signed int s=*x>>63;
  signed long e=((*x>>52)&((1<<11)-1))-1023;
/*  unsigned long long m=*x & ((1ull<<52)-1); */
  /* step 2: exponent is base 2, compute exponent for base 10 */
  signed long e10=1+(long)(e*0.30102999566398119802); /* log10(2) */
  /* step 3: calculate 10^e10 */
  int i;
  double tmp=10.0;
  char *oldbuf=buf;
  int initial=1;

  if (d==0.0) {
    for (i=0; i<prec; ++i) buf[i]='0';
    buf[1]='.'; buf[i]=0;
    return i;
  }
  if (s) { d=-d; *buf='-'; --maxlen; buf++; }
/*  printf("e=%d e10=%d prec=%d\n",e,e10,prec); */
  if (e10>=0) {
    i=e10;
    while (i>10) { tmp=tmp*1e10; i-=10; }
    while (i>1) { tmp=tmp*10; --i; }
  } else {
    i=(e10=-e10);
    while (i>10) { tmp=tmp*1e-10; i-=10; }
    while (i>1) { tmp=tmp/10; --i; }
  }
  while (d/tmp<1) {
    --e10;
    tmp/=10.0;
  }
  /* step 4: see if precision is sufficient to display all digits */
  if (e10>prec) {
    /* use scientific notation */
    int len=__dtostr(d/tmp,buf,maxlen,prec);
    if (len==0) return 0;
    maxlen-=len; buf+=len;
    if (--maxlen>=0) {
      *buf='e';
      ++buf;
    }
    for (len=1000; len>0; len/=10) {
      if (e10>=len || !initial) {
	if (--maxlen>=0) {
	  *buf=(e10/len)+'0';
	  ++buf;
	}
	initial=0;
	e10=e10%len;
      }
    }
    if (maxlen>=0) return buf-oldbuf;
    return 0;
  }
  /* step 5: loop through the digits, inserting the decimal point when
   * appropriate */
  if (d<1.0) {
    double y=1.0;
    int first=1;
    do {
      if (--maxlen<0) return buf-oldbuf;
      *buf='0'; ++buf;
      if (first) {
	first=0;
	*buf='.'; ++buf;
	if (--maxlen<0) return buf-oldbuf;
      }
      y/=10.0;
    } while (y>d);
  }
  for (; prec>0; ) {
    double tmp2=d/tmp;
    char c;
    d-=((int)tmp2*tmp);
    c=((int)tmp2);
    if ((!initial)||c) {
      if (--maxlen>=0) {
	initial=0;
	*buf=c+'0';
	++buf;
      } else
	return 0;
      --prec;
    }
    if (tmp>0.5 && tmp<1.5) {
      tmp=1e-1;
      initial=0;
      if (--maxlen>=0) {
	*buf='.';
	++buf;
      } else
	return 0;
    } else
      tmp/=10.0;
  }
  *buf=0;
  return buf-oldbuf;
}
