#include <sys/types.h>
#include <time.h>

static char *sweekdays[]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
static char *weekdays[]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
static char *smonths[]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
static char *months[]={"January","February","March","April","May","June","July","August","September","October","November","December"};
static char *ampm[]={"am","pm","AM","PM"};

static int i2a(char* dest,unsigned int x) {
  register unsigned int tmp=x;
  register int len=0;
  *dest++=tmp/10+'0'; tmp=tmp%10; ++len;
  *dest++=tmp+'0';
  return 2;
}

static int i2as(char* dest,unsigned int x) {
  int len=i2a(dest,x);
  if (*dest=='0') *dest=' ';
  return len;
}

size_t strftime(char *s, size_t max, const char *format, const struct tm *tm) {
  char *t=s;
  const char *src;
  char buf[5];
  while (*format) {
    switch (*format) {
    case 0: break;
    case '%':
      switch (*++format) {
      case '%': *t='%'; ++t; break;
      case 'a': src=sweekdays[tm->tm_wday]; goto append;
      case 'A': src=weekdays[tm->tm_wday]; goto append;
      case 'h':
      case 'b': src=smonths[tm->tm_mon]; goto append;
      case 'B': src=months[tm->tm_mon]; goto append;
      case 'c': t+=strftime(t,max-(t-s),"%b %a %d %k:%M:%S %Z %Y",tm); break;
      case 'C': buf[i2a(buf,(tm->tm_year+1900)/100)]=0; src=buf; goto append;
      case 'd': buf[i2a(buf,tm->tm_mday)]=0; src=buf; goto append;
      case 'e': buf[i2as(buf,tm->tm_mday)]=0; src=buf; goto append;
      case 'H': buf[i2a(buf,tm->tm_hour)]=0; src=buf; goto append;
      case 'I': buf[i2a(buf,tm->tm_hour%12)]=0; src=buf; goto append;
      case 'j': buf[i2a(buf,tm->tm_yday)]=0; src=buf; goto append;
      case 'k': buf[i2as(buf,tm->tm_hour)]=0; src=buf; goto append;
      case 'l': buf[i2as(buf,tm->tm_hour%12)]=0; src=buf; goto append;
      case 'm': buf[i2a(buf,tm->tm_mon+1)]=0; src=buf; goto append;
      case 'M': buf[i2a(buf,tm->tm_min)]=0; src=buf; goto append;
      case 'n': *t='\n'; break;
      case 'p': src=ampm[tm->tm_hour>11?3:2]; goto append;
      case 'P': src=ampm[tm->tm_hour>11?1:0]; goto append;
      case 'r': t+=strftime(t,max-(t-s),"%I:%M:%S %p",tm); break;
      case 'R': t+=strftime(t,max-(t-s),"%H:%M",tm); break;
      case 'S': buf[i2a(buf,tm->tm_sec)]=0; src=buf; goto append;
      case 't': *t='\t'; break;
      case 'T': t+=strftime(t,max-(t-s),"%H:%M:%S",tm); break;
      case 'u': buf[i2a(buf,tm->tm_wday?tm->tm_wday:7)]=0; src=buf; goto append;
      case 'w': buf[i2a(buf,tm->tm_wday)]=0; src=buf; goto append;
      case 'x': t+=strftime(t,max-(t-s),"%b %a %d",tm); break;
      case 'X': t+=strftime(t,max-(t-s),"%k:%M:%S",tm); break;
      case 'y': buf[i2a(buf,tm->tm_year%100)]=0; src=buf; goto append;
      case 'Y': i2a(buf,(tm->tm_year+1900)/100); buf[i2a(buf+2,(tm->tm_year%100))+2]=0; src=buf; goto append;
      case 'Z': src="GMT"; goto append; /* hehe */
append:
      while (*src && t<s+max) { *t=*src; ++t; ++src; }
      };
      break;
    default:
      *t=*format; ++t;
    }
    ++format;
    if (t>=s+max) break;
    continue;
  }
  *t=0; return t-s;
}



/*
       %u     The day of the week as a decimal,  range  1  to  7,
              Monday being 1.  See also %w. (SU)

       %U     The  week  number  of the current year as a decimal
              number, range 00 to 53,  starting  with  the  first
              Sunday as the first day of week 01. See also %V and
              %W.

       %V     The ISO 8601:1988 week number of the  current  year
              as  a  decimal number, range 01 to 53, where week 1
              is the first week that has at least 4 days  in  the
              current  year,  and with Monday as the first day of
              the week. See also %U and %W. (SU)

       %w     The day of the week as a decimal,  range  0  to  6,
              Sunday being 0.  See also %u.

       %W     The  week  number  of the current year as a decimal
              number, range 00 to 53,  starting  with  the  first
              Monday as the first day of week 01.

       %x     The  preferred  date representation for the current
              locale without the time.

       %X     The preferred time representation for  the  current
              locale without the date.

       %y     The  year  as  a  decimal  number without a century
              (range 00 to 99).

       %Y     The year as a decimal number including the century.

       %z     The time-zone as hour offset from GMT.  Required to
              emit RFC822-conformant dates (using "%a, %d  %b  %Y
              %H:%M:%S %z"). (GNU)

       %Z     The time zone or name or abbreviation.

       %+     The date and time in date(1) format. (TZ)

       %%     A literal `%' character.

       Some  conversion  specifiers  can be modified by preceding
       them by the E or O modifier to indicate that  an  alterna­
       tive  format should be used.  If the alternative format or
       specification does not exist for the current  locale,  the
       behaviour will be as if the unmodified conversion specifi­
       cation were used. (SU) The Single Unix Specification  men­
       tions  %Ec,  %EC,  %Ex, %EX, %Ry, %EY, %Od, %Oe, %OH, %OI,
       %Om, %OM, %OS, %Ou, %OU, %OV, %Ow,  %OW,  %Oy,  where  the
       effect  of  the  O  modifier is to use alternative numeric
       symbols (say, roman numerals), and that of the E  modifier



GNU                       29 March 1999                         3





STRFTIME(3)         Linux Programmer's Manual         STRFTIME(3)


       is to use a locale-dependent alternative representation.

       The  broken-down time structure tm is defined in <time.h>.
       See also ctime(3).


RETURN VALUE
       The strftime() function returns the number  of  characters
       placed  in  the array s, not including the terminating NUL
       character, provided the string, including the  terminating
       NUL,  fits.   Otherwise, it returns 0, and the contents of
       the array is undefined.  (Thus at least since libc  4.4.4;
       very  old  versions  of  libc,  such  as libc 4.4.1, would
       return max if the array was too small.)

       Note that the return value 0 does not necessarily indicate
       an  error; for example, in many locales %p yields an empty
       string.

ENVIRONMENT
       The environment variables TZ and LC_TIME are used.

CONFORMING TO
       ANSI C, SVID 3, ISO 9899.   There  are  strict  inclusions
       between the set of conversions given in ANSI C (unmarked),
       those given in the Single Unix Specification (marked  SU),
       those  given  in Olson's timezone package (marked TZ), and
       those given in glibc (marked GNU), except that %+  is  not
       supported  in glibc2. On the other hand glibc2 has several
       more extensions.  POSIX.1 only refers to ANSI  C;  POSIX.2
       describes  under  date(1)  several  extensions  that could
       apply to strftime as well.

SEE ALSO
       date(1), time(2), ctime(3), setlocale(3), sprintf(3)

*/
