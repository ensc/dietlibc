#include <ctype.h>

int isspace(int ch)
{
  return ((ch==' ')||(ch=='\f')||(ch=='\t')||(ch=='\v')||(ch=='\r')||(ch=='\n'));
}
