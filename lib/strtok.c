char *strtok_r(char *s, const char *delim, char **ptrptr);

static char *strtok_pos;

char *strtok(char *s, const char *delim)
{
  return strtok_r(s,delim,&strtok_pos);
}
