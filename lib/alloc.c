/*
 * malloc/free by O.Dreesen
 */

#include <linux/unistd.h>
#include <asm/mman.h>
#include <linux/errno.h>
#include "dietfeatures.h"

#if 0
#include <sys/mman.h>
#define _LIBC
#include <errno.h>
#endif

#include <linux/types.h>

#if defined(MAP_ANONYMOUS) && !defined(MAP_ANON)
#define MAP_ANON MAP_ANONYMOUS
#endif

#ifndef MAP_FAILED
#define MAP_FAILED ((void*)-1)
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

extern void * mmap(void *start, size_t length, int prot , int flags, int fd, off_t offset);
extern void *memset(void *s, int c, size_t n);
extern void *memcpy(void *dest, const void *src, size_t n);

typedef struct t_alloc_head {
  unsigned int magic1;
  struct t_alloc_head *ptr;
  unsigned long size;
  unsigned int magic2;
} alloc_head;

/* guess what ? the virtual block size */
#define MEM_BLOCK_SIZE	4096

/* minimum allocated bytes */
#define MEM_ALLOC_MIN	4

/* Initial start position in memory */
#define MEM_ALLOC_START	((char*)0x18000000)

/* Make every block align */
#define MEM_ALIGN(s)	(((s)+MEM_ALLOC_MIN-1)&(~(MEM_ALLOC_MIN-1)))
#define PAGE_ALIGN(s)	(((s)+MEM_BLOCK_SIZE-1)&(~(MEM_BLOCK_SIZE-1)))
#define PAGE_ALIGNP(p)	((char*)PAGE_ALIGN((size_t)(p)))

#define END_OF_BLOCK(p)	((alloc_head*)(((char*)(p))+((p)->size)))
#define START_BLOCK(p)	((alloc_head*)(((char*)(p))-sizeof(alloc_head)))
#define START_DATA(p)	(((char*)(p))+sizeof(alloc_head))
#define MIN_ALLOC(s)	(((((s)+sizeof(alloc_head)-1)/MEM_ALLOC_MIN)+1)*MEM_ALLOC_MIN)

#define ALLOC_MAGIC1	0xbad2f7ee
#define ALLOC_MAGIC2	0xf7ee2bad

/* freelist handler */
static alloc_head base = {ALLOC_MAGIC1,&base,0,ALLOC_MAGIC2};
static char *alloc_get_end = MEM_ALLOC_START;

void __libc_free(void *ptr)
{
  alloc_head *prev,*p,*block;

  if (ptr==NULL) return;

  block=START_BLOCK(ptr);
  if (block->magic1 != ALLOC_MAGIC1) return;
  if (block->magic2 != ALLOC_MAGIC2) return;

  prev=&base;
  for (p=prev->ptr ; ; prev=p, p=p->ptr)
  {
    if ((block>prev)&&(block<p)) break; /* found the gap block belongs */
    if ((prev>p)&&(block<p)) break;	  /* block pre freelist */
    if ((prev>p)&&(block>prev)) break;  /* block after freelist */

    /* emergency escape: freelist has ONLY one entry the freelist base */
    if (p->ptr==p) break;
  }
  prev->ptr = block;

  if (END_OF_BLOCK(block)==p)
  { /* join right neighbor */
    block->ptr   = p->ptr;
    block->size += p->size;
  }
  else
    block->ptr = p;

  if (END_OF_BLOCK(prev)==block)
  { /* join left neighbor */
    prev->size += block->size;
    prev->ptr   = block->ptr;
  }
}
void free(void *ptr) __attribute__((weak,alias("__libc_free")));

static void *alloc_get_mem(unsigned long size)
{
  char *tmp;
  alloc_head *ah;

  size=PAGE_ALIGN(size);

  /* map free pages @ alloc_get_end */
  tmp=mmap(alloc_get_end, size, PROT_READ|PROT_WRITE,
	   MAP_ANONYMOUS|MAP_PRIVATE|MAP_FIXED, -1, 0);
  if (tmp==MAP_FAILED)
  {
    /* OK we can't map free pages @ alloc_get_end so try free position */
    tmp=mmap(0, size, PROT_READ|PROT_WRITE,
	     MAP_ANONYMOUS|MAP_PRIVATE, -1, 0);
    if (tmp==MAP_FAILED)
    {
      errno = ENOMEM;
      return NULL;	/* PANIC ! */
    }
    alloc_get_end=tmp;
  }

  alloc_get_end+=size;

  /* make a header */
  ah=(alloc_head*)tmp;
  ah->magic1=ALLOC_MAGIC1;
  ah->magic2=ALLOC_MAGIC2;
  ah->ptr=ah;
  ah->size=size;

  /* link new free maped pages in freelist */
  __libc_free(START_DATA(tmp));

  return &base;
}

void *__libc_malloc(size_t size)
{
  alloc_head *p, *prev;
  size_t need;

  /* needed MEM_ALLOC_MIN */
  need=MIN_ALLOC(size);

  prev=&base;
  for (p=prev->ptr;;prev=p,p=p->ptr)
  {
    if (p->size>=need)
    {
      if (p->size==need)
      { /* fit PERFECT */
	prev->ptr=p->ptr;	/* relink freelist */
      }
      else
      {
	alloc_head *tmp=(alloc_head*)(((char*)p)+need);
	if ((p->size-need)<sizeof(alloc_head))
	{ /* work around: if there is not enough space for freelist head.
	   * this waste some bytes ( < sizeof(alloc_head) ) */
	  need=p->size;
	  prev->ptr=p->ptr;	/* relink freelist */
	}
	else
	{
	  prev->ptr=tmp;
	  tmp->magic1=ALLOC_MAGIC1;
	  tmp->magic2=ALLOC_MAGIC2;
	  tmp->ptr=p->ptr;
	  tmp->size=p->size-need;	/* remaining size */
	}

	p->size=need;	/* set size */
      }
      p->ptr=p;		/* self-link */

      return (void*)START_DATA(p);
    }
    else if (p==&base)
    {
      if ((p=alloc_get_mem(need))==NULL) goto err_out;
    }
  }
err_out:
  return NULL;
}
void *malloc(size_t size) __attribute__((weak,alias("__libc_malloc")));

void *calloc(size_t nmemb,size_t size)
{
  size_t n=nmemb*size;
  void *tmp=malloc(n);
  if (tmp) memset(tmp,0,n);
  return tmp;
}

void *realloc(void *ptr,size_t size)
{
  alloc_head *tmp=0,*tf=0;
  long need=0;
  long diff=0;

  if (ptr)
  {
    if (size)
    {
      tmp=START_BLOCK(ptr);
      need=MIN_ALLOC(size);  /* only this size will survive */
      diff=tmp->size-need;
      if (diff<0)
      {
	if ((tf=malloc(size)))
	{
	  memcpy(tf,ptr,tmp->size-sizeof(alloc_head));
	  free(ptr);
	  return tf;
	}
	return NULL;
      }
      if (diff>=sizeof(alloc_head))
      {
	tmp->size=need;
	tf=END_OF_BLOCK(tmp);
	tf->magic1=ALLOC_MAGIC1;
	tf->magic2=ALLOC_MAGIC2;
	tf->ptr=tf;
	tf->size=diff;
	free(START_DATA(tf));
      }
      return ptr;
    }
    else
      free(ptr);
  }
  else if (size>0)
    return malloc(size);
  return NULL;
}
