#include "getopt.h"

/*
 * by Olaf Dreesen
 */

int opterr;

int optind=1;
char *optarg;

static int opt_unknown=1,opt_unknown_len;

static int getopt_check(int c,char*o,int ol)
{
  int i;
  if (c==':') return 2;
  for (i=0;i<ol;i++)
  {
    if (o[i]==c)
    {
      if (o[i+1]==':') return 1;
      return 0;
    }
  }
  return 2;
}

static int getopt_sort(char*v[],int oi)
{
  int i;
  char *tmp, *tmp2;

  if (opt_unknown_len)
  {
    tmp=v[optind-(1+oi)];
    if (oi) tmp2=v[optind-1];

    for (i=opt_unknown+opt_unknown_len;i>opt_unknown;i--) v[i+oi]=v[i-1];

    v[opt_unknown++]=tmp;
    if (oi) v[opt_unknown++]=tmp2;
  }
}

static char* nextchar;
int getopt(int c,char*v[],char*o)
{
  int ol=strlen(o);
  int ret=0;
  int oi=0;

  optarg=0;

  while (nextchar || (optind<c))
  {
    if (nextchar)
    {
      if (ret=*(++nextchar))
      {
	switch (getopt_check(ret,o,ol))
	{
	case 1:
	  if (*(++nextchar))
	    optarg=nextchar;
	  else
	  {
	    if (optind<c)
	    {
	      oi=1;
	      optarg=v[optind++];
	    }
	    else
	      ret='?';
	  }
	  nextchar=0;
	case 0:
	  if (!nextchar)
	    getopt_sort(v,oi);
	  else
	    if (!(*(nextchar+1)))
	      getopt_sort(v,oi);
	  return ret;
	  break;
	default:
	  return '?';
	  break;
	}
      }
      else
	nextchar=0;
    }
    else
    {
      if ((v[optind][0]=='-')&&((v[optind][1]!=0)))
      {
	if ((v[optind][1]=='-')&&(v[optind][2]==0))
	{
	  getopt_sort(v,oi);
	  optind=opt_unknown;
	  return -1;
	}
	else
	{
	  nextchar=v[optind];
	}
      }
      else
      {
	++opt_unknown_len;
      }
      ++optind;
    }
  }
  optind=opt_unknown;
  return -1;
}
