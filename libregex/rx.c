#include <regex.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>

/* this is ugly.
 * the idea is to build a parse tree, then do some poor man's OOP with a
 * generic matcher function call that is always that the start of each
 * record, and a next pointer.  When the parse tree is done, we need to
 * recursively set the next pointers to point to the part of the parse
 * tree that needs to match next.
 * This is the prototype of the generic match function call pointer.
 * The first argument is the "this" pointer, the second is the text to
 * be matched against, ofs is the offset from the start of the matched
 * text (so we can match "^") and matches is an array where match
 * positions are stored. */
/* now declared in regex.h: */
/* typedef int (*matcher)(void*,const char*,int ofs,regmatch_t* matches,int plus); */

/* first part: parse a regex into a parse tree */
struct bracketed {
  unsigned int cc[32];
};

/* now declared in regex.h:
struct regex {
  matcher m;
  void* next;
  int pieces;
  int num;
  struct branch *b;
}; */

struct atom {
  matcher m;
  void* next;
  enum { EMPTY, REGEX, BRACKET, ANY, LINESTART, LINEEND, CHAR, } type;
  int bnum;
  union {
    struct regex r;
    struct bracketed b;
    char c;
  } u;
};

struct piece {
  matcher m;
  void* next;
  struct atom a;
  unsigned char min,max;
};

struct branch {
  matcher m;
  void* next;
  int num;
  struct piece *p;
};

static void clearcc(unsigned int* x) {
  memset(x,0,sizeof(*x));
}

static void setcc(unsigned int* x,unsigned int bit) {
  x[bit/32]|=(1<<((bit%32)-1));
}

static int issetcc(unsigned int* x,unsigned int bit) {
  return x[bit/32] & (1<<((bit%32)-1));
}

static const char* parsebracketed(struct bracketed* b,const char* s) {
  const char* t;
  int i,negflag=0;
  if (*s!='[') return s;
  t=s+1;
  clearcc(b->cc);
  if (*t=='^') { negflag=1; ++t; }
  do {
    if (*t==0) return s;
    setcc(b->cc,*t);
    if (t[1]=='-' && t[2]!=']') {
      for (i=*t+1; i<=t[2]; ++i) setcc(b->cc,i);
      t+=2;
    }
    ++t;
  } while (*t!=']');
  if (negflag) for (i=0; i<32; ++i) b->cc[i]=~b->cc[i];
  return t+1;
}

static const char* parseregex(struct regex* r,const char* s,int* bnum);

static int matchatom(void* x,const char* s,int ofs,regmatch_t* matches,int plus) {
  register struct atom* a=(struct atom*)x;
  int matchlen=1;
  switch (a->type) {
  case EMPTY:
//    printf("matching EMPTY against \"%s\"\n",s);
    if (matches) matches[a->bnum].rm_so=matches[a->bnum].rm_eo=ofs;
    matchlen=0;
    goto match;
  case REGEX:
//    printf("matching REGEX against \"%s\"\n",s);
    if ((matchlen=a->u.r.m(a,s,ofs,matches,0))>=0) {
      if (matches) {
	matches[a->bnum].rm_so=ofs;
	matches[a->bnum].rm_eo=ofs+matchlen;
      }
      goto match;
    }
    break;
  case BRACKET:
//    printf("matching BRACKET against \"%s\"\n",s);
    if (*s && issetcc(a->u.b.cc,*s))
      goto match;
    break;
  case ANY:
//    printf("matching ANY against \"%s\"\n",s);
    if (*s) goto match;
    break;
  case LINESTART:
//    printf("matching LINESTART against \"%s\"\n",s);
    if (ofs==0) {
      matchlen=0;
      goto match;
    }
    break;
  case LINEEND:
//    printf("matching LINEEND against \"%s\"\n",s);
    if (*s) break;
    matchlen=0;
    goto match;
  case CHAR:
//    printf("matching CHAR %c against \"%s\"\n",a->u.c,s);
    if (*s==a->u.c) goto match;
    break;
  }
  return -1;
match:
  if (a->next)
    return ((struct atom*)(a->next))->m(a->next,s+matchlen,ofs+matchlen,matches,plus+matchlen);
  else
    return plus+matchlen;
}

static const char* parseatom(struct atom* a,const char* s,int* bnum) {
  const char *tmp;
  a->m=matchatom;
  a->bnum=-1;
  switch (*s) {
  case '(':
    a->bnum=*++bnum;
    if (s[1]==')') {
      a->type=EMPTY;
      return s+2;
    }
    a->type=REGEX;
    if ((tmp=parseregex(&a->u.r,s+1,bnum))!=s) {
      if (*tmp==')')
	return tmp+1;
    }
  case 0:
  case '|':
  case ')':
    return s;
  case '[':
    a->type=BRACKET;
    if ((tmp=parsebracketed(&a->u.b,s))!=s)
      return tmp;
    return s;
  case '.':
    a->type=ANY;
    return s+1;
  case '^':
    a->type=LINESTART;
    return s+1;
  case '$':
    a->type=LINEEND;
    return s+1;
  case '\\':
    if (!*++s) return s;
  default:
    a->type=CHAR;
    a->u.c=*s;
    return s+1;
  }
}

/* needs to do "greedy" matching, i.e. match as often as possible */
static int matchpiece(void* x,const char* s,int ofs,regmatch_t* matches,int plus) {
  register struct piece* a=(struct piece*)x;
  int matchlen=0;
  int tmp,num=0;
  unsigned int *offsets=alloca(sizeof(int)*a->max);
  offsets[0]=0;
//  printf("matchpiece \"%s\"...\n",s);
  /* first, try to match the atom as often as possible, up to a->max times */
  while (num<a->max) {
    void* save=a->a.next;
    a->a.next=0;
    if ((tmp=a->a.m(&a->a,s+matchlen,ofs+matchlen,matches,0))>=0) {
      a->a.next=save;
      ++num;
      matchlen+=tmp;
      offsets[num]=tmp;
    } else {
      a->a.next=save;
      break;
    }
  }
  if (num<a->min) return -1;		/* already at minimum matches; signal mismatch */
  /* then, while the rest does not match, back off */
  for (;;) {
    if (a->next)
      tmp=((struct atom*)(a->next))->m(a->next,s+matchlen,ofs+matchlen,matches,plus+matchlen);
    else
      tmp=plus+matchlen;
    if (tmp>=0) break;	/* it did match; don't back off any further */
    matchlen-=offsets[num];
    --num;
  }
  return tmp;
}

static const char* parsepiece(struct piece* p,const char* s,int* bnum) {
  const char* tmp=parseatom(&p->a,s,bnum);
  if (tmp==s) return s;
  p->m=matchpiece;
  p->min=p->max=1;
  switch (*tmp) {
  case '*': p->min=0; p->max=RE_DUP_MAX; break;
  case '+': p->min=1; p->max=RE_DUP_MAX; break;
  case '?': p->min=0; p->max=1; break;
  case '{':
    if (isdigit(*++tmp)) {
      p->min=*tmp-'0'; p->max=RE_DUP_MAX;
      while (isdigit(*++tmp)) p->min=p->min*10+*tmp-'0';
      if (*tmp==',') {
	if (isdigit(*++tmp)) {
	  p->max=*tmp-'0';
	  while (isdigit(*++tmp)) p->max=p->max*10+*tmp-'0';
	}
      }
      if (*tmp!='}') return s;
      ++tmp;
    }
  default:
    return tmp;
  }
  return tmp+1;
}

/* trivial, just pass through */
static int matchbranch(void* x,const char* s,int ofs,regmatch_t* matches,int plus) {
  register struct branch* a=(struct branch*)x;
  int tmp;
  tmp=a->p->m(a->p,s,ofs,matches,plus);
  if (tmp>=0) {
    if (a->next)
      return ((struct atom*)(a->next))->m(a->next,s+tmp,ofs+tmp,matches,plus+tmp);
    else
      return plus+tmp;
  }
  return -1;
}

static const char* parsebranch(struct branch* b,const char* s,int* bnum,int* pieces) {
  struct piece p;
  const char *tmp;
  b->m=matchbranch;
  b->num=0; b->p=0;
  for (;;) {
    if (*s=='|') {
      if (b->num==0) {
	tmp=s+1;
	p.a.type=EMPTY;
	p.min=p.max=1;
      }
    } else {
      tmp=parsepiece(&p,s,bnum);
      if (tmp==s) return s;
    }
    if (!(b->p=realloc(b->p,++b->num*sizeof(p)))) return s;
    b->p[b->num-1]=p;
    if (*s=='|') { ++tmp; break; }
    s=tmp;
  }
  *pieces+=b->num;
  return tmp;
}

/* try the branches one by one */
static int matchregex(void* x,const char* s,int ofs,regmatch_t* matches,int plus) {
  register struct regex* a=(struct regex*)x;
  int i,tmp;
  for (i=0; i<a->num; ++i) {
    tmp=a->b[i].m(&a->b[i],s,ofs,matches,plus);
    if (tmp>=0) {
      if (a->next)
	return ((struct atom*)(a->next))->m(a->next,s+tmp,ofs+tmp,matches,plus+tmp);
      else
	return plus+tmp;
    }
  }
  return -1;
}

static const char* parseregex(struct regex* r,const char* s,int* bnum) {
  struct branch b;
  const char *tmp;
  r->m=matchregex;
  r->num=0; r->b=0; r->pieces=0;
  for (;;) {
    tmp=parsebranch(&b,s,bnum,&r->pieces);
    if (tmp==s) return s;
    if (!(r->b=realloc(r->b,++r->num*sizeof(b)))) return s;
    r->b[r->num-1]=b;
    s=tmp;
  }
  return tmp;
}


/* The matcher relies on the presence of next pointers, of which the
 * parser does not know the correct destination.  So we need an
 * additional pass through the data structure that sets the next
 * pointers correctly. */
static void regex_putnext(struct regex* r,void* next);

static void atom_putnext(struct atom* a,void* next) {
  a->next=next;
  if (a->type==REGEX)
    regex_putnext(&a->u.r,next);
}

static void piece_putnext(struct piece* p,void* next) {
  p->next=next;
  atom_putnext(&p->a,next);
}

static void branch_putnext(struct branch* b,void* next) {
  int i;
  for (i=0; i<b->num-1; ++i)
    piece_putnext(&b->p[i],&b->p[i+1]);
  piece_putnext(&b->p[i],0);
  b->next=next;
}

static void regex_putnext(struct regex* r,void* next) {
  int i;
  for (i=0; i<r->num; ++i)
    branch_putnext(&r->b[i],next);
  r->next=next;
}



int regcomp(regex_t* preg, const char* regex, int cflags) {
  const char* t=parseregex(&preg->r,regex,&preg->brackets);
  if (t==regex) return -1;
  regex_putnext(&preg->r,0);
  preg->cflags=cflags;
  return 0;
}

int regexec(const regex_t *preg, const char *string, size_t nmatch, regmatch_t pmatch[], int eflags) {
  regmatch_t *l;
  int matched;
  if ((preg->cflags&REG_NOSUB)==0)
    l=alloca(sizeof(regmatch_t)*(preg->brackets+1));
  else
    l=0;
  matched=preg->r.m((void*)&preg->r,string,0,l,0);
  if (matched>=0) return 0;
  return REG_NOMATCH;
}



void regfree(regex_t *preg) {
  int i;
  for (i=0; i<preg->r.num; ++i) {
    free(preg->r.b[i].p);
    free(preg->r.b);
  }
}



#if 0
int main() {
  struct regex r;
  int bnum=-1;
  const char* t=parseregex(&r,"^a*ab$",&bnum);
  regex_putnext(&r,0);
  printf("%d pieces, %s\n",r.pieces,t);
  printf("%d\n",r.m(&r,"aaab",0,0,0));
  return 0;
}
#endif
