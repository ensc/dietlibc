enum charset {
  INVALID,
  ISO_8859_1,
  UTF_8,
  UCS_4
};

struct dieticonv {
  enum charset from,to;
};

#include <iconv.h>
