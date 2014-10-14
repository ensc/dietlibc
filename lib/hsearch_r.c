#define _GNU_SOURCE
#include <search.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

static const size_t primes[] = {
  67, 127, 257, 521, 1031, 2053, 4099, 8191, 16381, 32771, 65537,
  131071, 262147, 524287, 1048583, 2097143 };

static const unsigned int nprimes = sizeof(primes)/sizeof(primes[0]);

int hsearch_r(ENTRY item, ACTION action, ENTRY** retval, struct hsearch_data* htab) {
  uint64_t hv=siphash24(htab->key,(const unsigned char*)item.key,strlen(item.key));
  struct bucket** b=&htab->table[hv%htab->size];
  while (*b) {
    if ((*b)->hv==hv && !strcmp((*b)->e.key,item.key)) {
      /* found */
      if (retval) *retval=&(*b)->e;
      return 1;
    }
    b=(&(*b)->next);
  }
  if (action==FIND) {
    errno=ESRCH;
    return 0;
  }
  /* if we get this far, we are supposed to add the key */
  if (htab -> size < primes[nprimes-1] && htab->filled+1 > htab->size) {
    /* attempt to resize */
    unsigned int i;
    struct bucket** n;
    for (i=0; i<nprimes; ++i)
      if (htab->size < primes[i]) break;
    i=primes[i];
    n=calloc(i,sizeof(struct bucket*));
    /* If the allocation failed, we can just limp on.
      * Performance will be reduced but we'll live. */
    if (n) {
      unsigned int j;

      for (j=0; j<htab->size; ++j) {
	struct bucket* B=htab->table[j];
	while (B) {
	  struct bucket* c=B;
	  B=B->next;
	  unsigned int m=c->hv % i;
	  c->next=n[m];
	  n[m]=c;
	}
      }
      free(htab->table);
      htab->table=n;
      htab->size=i;
      b=&htab->table[hv%htab->size];
      while (*b)
	b=(&(*b)->next);
    }
  }
  *b=malloc(sizeof(struct bucket));
  if (!*b) return 0;
  ++htab->filled;
  (*b)->next=0;
  (*b)->e=item;
  (*b)->hv=hv;
  if (retval) *retval=&(*b)->e;
  return 1;
}
