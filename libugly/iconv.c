#include "dietfeatures.h"
#include <errno.h>
#include "dieticonv.h"

size_t iconv(iconv_t cd, const char* * inbuf, size_t *
		    inbytesleft, char* * outbuf, size_t * outbytesleft) {
  size_t converted=0;
  int i,j,k,bits;
  if (!inbuf || !*inbuf) return 0;
  while (*inbytesleft) {
    unsigned int v=0;
    v=*(unsigned char*)*inbuf;
    i=j=1;
    switch (cd->from) {
    case UCS_4:
      v=*(unsigned int*)*inbuf;
      i=4;
    case ISO_8859_1:
      break;
    case UTF_8:
      if (!(v&0x80)) break;
      for (i=0xC0; i!=0xFC; i=(i>>1)+0x80)
	if ((v&((i>>1)|0x80))==i) {
	  v&=~i;
	  break;
	}
      for (i=1; ((*inbuf)[i]&0xc0)==0x80; ++i)
	v=(v<<6)|((*inbuf)[i]&0x3f);
/*      printf("got %u in %u bytes, buflen %u\n",v,i,*inbytesleft); */
      break;
    }
    switch (cd->to) {
    case ISO_8859_1:
      **outbuf=(unsigned char)v;
      break;
    case UCS_4:
      *(unsigned int*)*outbuf=v;
      j=4;
      break;
    case UTF_8:
      if (v>0x04000000) { bits=30; **outbuf=0xFC; j=6; } else
      if (v>0x00200000) { bits=24; **outbuf=0xF8; j=5; } else
      if (v>0x00010000) { bits=18; **outbuf=0xF0; j=4; } else
      if (v>0x00000800) { bits=12; **outbuf=0xE0; j=3; } else
      if (v>0x00000080) { bits=6; **outbuf=0xC0; j=2; } else
			{ bits=0; **outbuf=0; }
      **outbuf |= (unsigned char)(v>>bits);
      if (*outbytesleft<j) {
	errno=E2BIG;
	return (size_t)-1;
      }
      for (k=1; k<j; ++k) {
	bits-=6;
	(*outbuf)[k]=0x80+((v>>bits)&0x3F);
      }
      break;
    }
    *inbuf+=i; *inbytesleft-=i; ++converted;
    *outbuf+=j; *outbytesleft-=j;
    break;
  }
  return converted;
}
