#include "dietfeatures.h"
#include <stdarg.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include "dietwarning.h"

extern int __ltostr(char *s, int size, unsigned long i, int base, char UpCase);
extern int __dtostr(double d,char *buf,int maxlen,int prec);

int vsnprintf (char *str, size_t size, const char *format, va_list arg_ptr)
{
  size_t apos,i;
  char ch,buf[1024];
  char *pb;
  char flag_in_sign,flag_upcase;
  char flag_hash,flag_zero,flag_left,flag_space,flag_sign,flag_dot,flag_long;
  long number,width,preci,buf_len,pad;
#ifdef WANT_LONGLONG_PRINTF
  long long llnumber;
#endif
  char padwith;

  size--;

  apos=0;
  while (apos<size)
  {
    ch=*format++;
    switch (ch)
    {
    case '%':
      flag_hash=flag_upcase=0;
      flag_zero=0;
      flag_left=0;
      flag_space=0;
      flag_sign=0;
      flag_dot=0;
      flag_in_sign=0;
      flag_long=0;

      width=0;
      padwith=' ';

inn_vsnprintf:
      if (apos>=size) continue; /* ARGL !!! */

      ch=*format++;
      switch (ch)
      {
/* Format end ?!? */
      case 0:
	return -1;
	break;

/* Format flag chars */
      case '#':
	flag_hash=1;
	goto inn_vsnprintf;

      case 'l':
	++flag_long;
	goto inn_vsnprintf;

      case '0':
	padwith='0';
	goto inn_vsnprintf;

      case '-':
	flag_left=1;
	goto inn_vsnprintf;

      case ' ':
	flag_space=1;
	goto inn_vsnprintf;

      case '+':
	flag_sign=1;
	goto inn_vsnprintf;

      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
	if(flag_dot) return -1;
	width=strtol(--format,&pb,10);
	format=pb;
	goto inn_vsnprintf;

      case '*':
	width=va_arg(arg_ptr,int);
	goto inn_vsnprintf;

      case '.':
	flag_dot=1;
	if (*format=='*') {
	  preci=va_arg(arg_ptr,int);
	  ++format;
	} else {
	  preci=strtol(format,&pb,10);
	  format=pb;
	}
	goto inn_vsnprintf;

/* Format conversion chars */
      case 'c':
	ch=(char)va_arg(arg_ptr,int);
      case '%':
	if (str) str[apos]=ch; ++apos;
	break;

      case 's':
	pb=va_arg(arg_ptr,char *);
#ifdef WANT_NULL_PRINTF
	if (!pb) pb="(null)";
#endif
	buf_len=strlen(pb);
	if (flag_dot && buf_len>preci) buf_len=preci;
	if (buf_len>size-apos) buf_len=size-apos;

print_out:
	if (str) {
	  if (width && (!flag_left))
	  {
	    for (pad=width-buf_len; pad>0; --pad) str[apos++]=padwith;
	  }
	  for(i=0;i<buf_len;++i) { str[apos++]=pb[i]; } /* strncpy */
	  if (width && (flag_left))
	  { /* left pad ALLWAYS with blanks ... */
	    for (pad=width-buf_len; pad>0; --pad) str[apos++]=' ';
	  }
	} else {
	  if (width) {
	    apos+=width>buf_len?width:buf_len;
	  } else {
	    apos+=size>buf_len?buf_len:size;
	  }
	}

	break;

	/* Numbers */
      case 'b':
	i=2;
	goto num_vsnprintf;
      case 'p':
	flag_hash=1;
	width=sizeof(void *)<<1;
	padwith='0';
	ch='x';
      case 'X':
	flag_upcase=(ch=='X');
      case 'x':
	i=16;
	if (flag_hash)
	{
	  if (str) {
	    str[apos++]='0';
	    str[apos++]=ch;
	  } else
	    apos+=2;
	}
	goto num_vsnprintf;
      case 'd':
      case 'i':
	flag_in_sign=1;
      case 'u':
	i=10;
	goto num_vsnprintf;
      case 'o':
	i=8;
	if (flag_hash) { if (str) str[apos]='0'; ++apos; }

num_vsnprintf:
	if (apos>=size) continue; /* ARGL !!! */

	if (flag_long)
#ifdef WANT_LONGLONG_PRINTF
	{
	  if (flag_long>1)
	    llnumber=va_arg(arg_ptr,long long);
	  else
	    number=va_arg(arg_ptr,long);
	}
#else
	  number=va_arg(arg_ptr,long);
#endif
	else
	  number=va_arg(arg_ptr,int);

	if (flag_in_sign && (number<0))
	{
#ifdef WANT_LONGLONG_PRINTF
	  if (flag_long>1)
	    llnumber=-llnumber;
	  else
#endif
	  number=-number;
	  flag_in_sign=2;
	}

#ifdef WANT_LONGLONG_PRINTF
	if (flag_long>1)
	  buf_len=__lltostr(buf+1,sizeof(buf)-1,(unsigned long long) llnumber,i,flag_upcase);
	else
#endif
	  buf_len=__ltostr(buf+1,sizeof(buf)-1,(unsigned long) number,i,flag_upcase);
	pb=buf+1;

	if (flag_in_sign==2)
	{
	  *(--pb)='-';
	  buf_len++;
	}
	else if ((flag_in_sign)&&(flag_sign || flag_space))
	{
	  *(--pb)=(flag_sign)?'+':' ';
	  buf_len++;
	}
	goto print_out;

#ifdef WANT_FLOATING_POINT_IN_PRINTF
      case 'f':
      case 'g':
	{
	  int g=(ch=='g');
	  double d=va_arg(arg_ptr,double);
	  buf_len=__dtostr(d,buf,sizeof(buf),width?width+(flag_dot?preci+1:1):6);
	  if (flag_dot) {
	    char *tmp;
	    if ((tmp=strchr(buf,'.'))) {
	      while (preci>-1 && *++tmp) --preci;
	      *tmp=0;
	    }
	  }
	  if (g) {
	    char *tmp;
	    if ((tmp=strchr(buf,'.'))) {
	      while (*tmp) ++tmp;
	      while (*--tmp=='0') ;
	      if (*--tmp=='.') *tmp=0; else tmp[1]=0;
	    }
	  }
	  buf_len=strlen(buf);
	  pb=buf;
	  goto print_out;
	}
#endif
      default:
	break;
      }
      break;
    case 0:
      if (str) str[apos]=0;
      return apos;
    default:
      if (str) str[apos]=ch; apos++;
      break;
    }
  }
  if (str) str[apos]=0;
  return apos;
}

link_warning("vsnprintf","warning: the printf functions add several kilobytes of bloat.")

