#include <stdlib.h>
/* convert double to string.  Helper for sprintf. */

int __dtostr(double d,char *buf,unsigned int maxlen,unsigned int prec,unsigned int prec2) {
  unsigned long long *x=(unsigned long long *)&d;
  /* step 1: extract sign, mantissa and exponent */
  signed int s=*x>>63;
  signed long e=((*x>>52)&((1<<11)-1))-1023;
/*  unsigned long long m=*x & ((1ull<<52)-1); */
  /* step 2: exponent is base 2, compute exponent for base 10 */
  signed long e10=1+(long)(e*0.30102999566398119802); /* log10(2) */
  /* step 3: calculate 10^e10 */
  unsigned int i;
  double backup=d;
  double tmp=10.0;
  char *oldbuf=buf;

  /* Wir iterieren von Links bis wir bei 0 sind oder maxlen erreicht
   * ist.  Wenn maxlen erreicht ist, machen wir das nochmal in
   * scientific notation.  Wenn dann von prec noch was übrig ist, geben
   * wir einen Dezimalpunkt aus und geben prec2 Nachkommastellen aus.
   * Wenn prec2 Null ist, geben wir so viel Stellen aus, wie von prec
   * noch übrig ist. */
  if (d==0.0) {
    for (i=0; i<prec; ++i) buf[i]='0';
    buf[1]='.'; buf[i]=0;
    return i;
  }
  if (s) { d=-d; *buf='-'; --maxlen; buf++; }
/*  printf("e=%d e10=%d prec=%d\n",e,e10,prec); */
  if (e10>0) {
    int first=1;	/* are we about to write the first digit? */
    i=e10;
    while (i>10) { tmp=tmp*1e10; i-=10; }
    while (i>1) { tmp=tmp*10; --i; }
    /* the number is greater than 1. Iterate through digits before the
     * decimal point until we reach the decimal point or maxlen is
     * reached (in which case we switch to scientific notation). */
    while (tmp>0.9) {
      char digit;
      double fraction=d/tmp;
      /* see if we must round. */
      if (tmp<9 &&		/* last digit before decimal point */
	  !prec2 && prec-(buf-oldbuf)<2)	/* no digits after decimal point */
	digit=(int)(fraction+0.5);	/* round() */
      else
	digit=(int)(fraction);		/* floor() */
      if (!first || digit) {
	*buf=digit+'0'; ++buf;
	if (!maxlen) {
	  /* use scientific notation */
	  int len=__dtostr(backup/tmp,oldbuf,maxlen,prec,prec2);
	  int initial=1;
	  if (len==0) return 0;
	  maxlen-=len; buf+=len;
	  if (maxlen>0) {
	    *buf='e';
	    ++buf;
	  }
	  --maxlen;
	  for (len=1000; len>0; len/=10) {
	    if (e10>=len || !initial) {
	      if (maxlen>0) {
		*buf=(e10/len)+'0';
		++buf;
	      }
	      --maxlen;
	      initial=0;
	      e10=e10%len;
	    }
	  }
	  if (maxlen>0) goto fini;
	  return 0;
	}
	d-=digit*tmp;
	--maxlen;
      }
      tmp/=10.0;
    }
  }
  if (buf==oldbuf) {
    if (!maxlen) return 0; --maxlen;
    *buf='0'; ++buf;
  }
  if (prec2 || prec>(unsigned int)(buf-oldbuf)+1) {	/* more digits wanted */
    if (!maxlen) return 0; --maxlen;
    *buf='.'; ++buf;
    prec-=buf-oldbuf-1;
    if (prec2) prec=prec2;
    if (prec>maxlen) return 0;
    while (prec>0) {
      char digit;
      double fraction=d/tmp;
      /* see if we must round. */
      if (prec==1)
	digit=(int)(fraction+0.5);	/* round() */
      else
	digit=(int)(fraction);		/* floor() */
      *buf=digit+'0'; ++buf;
      d-=digit*tmp;
      tmp/=10.0;
      --prec;
    }
  }
fini:
  *buf=0;
  return buf-oldbuf;
}
