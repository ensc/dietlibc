enum charset {
  INVALID=0,
  ISO_8859_1,
  UTF_8,
  UCS_2,
  UCS_4
};

#define ic_from(x)	(((x)    )&0xffff)
#define ic_to(x)	(((x)>>16)&0xffff)

#include <iconv.h>
